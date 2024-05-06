
#pragma warning(push,0)
#include "uaplatformlayer.h"
#pragma warning(pop)

#include "shutdown.h"

#include <stdio.h>
#include <string.h>
#include <iostream>


# ifndef WIN32
#  include <unistd.h>
#  include <limits.h>
# endif

/* shutdown flag */
static volatile unsigned int g_ShutDown = 0;

#ifdef _WIN32_WCE
#  include <windows.h>
#elif defined(_WIN32) && !defined(USE_CTRLC_ON_WINDOWS)
#  include <conio.h> /* DOS header for _kbhit() and _getch() */
#endif

/* Return shutdown flag. */
unsigned int ShutDownFlag()
{
#if defined(_WIN32) && !defined(USE_CTRLC_ON_WINDOWS)
  #ifndef _WIN32_WCE
    if (_kbhit() && _getch() == 'x') return 1;
    else return 0;
  #else
    if (GetAsyncKeyState(VK_ESCAPE)) return 1;
    else return 0;
  #endif
#else
    return g_ShutDown;
#endif
}

/****************************************
 * Linux SIGINT Handler implementation. *
 ****************************************/
#ifdef __linux__
#include <signal.h>

/** Signal handler for SIG_INT and SIGTERM. */
void signal_handler(int signo)
{
    SHUTDOWN_TRACE("Received signal %i\n", signo);
    g_ShutDown = 1;
}

void RegisterSignalHandler()
{
    /* register signal handlers. */
    struct sigaction new_action, old_action;

    /* Set up the structure to specify the new action. */
    new_action.sa_handler = signal_handler;
    sigemptyset(&new_action.sa_mask);
    new_action.sa_flags = 0;

    /* install new signal handler for SIGINT */
    sigaction(SIGINT, NULL, &old_action);
    if (old_action.sa_handler != SIG_IGN)
    {
        sigaction(SIGINT, &new_action, NULL);
    }
    /* install new signal handler for SIGTERM */
    sigaction(SIGTERM, NULL, &old_action);
    if (old_action.sa_handler != SIG_IGN)
    {
        sigaction(SIGTERM, &new_action, NULL);
    }

    /* Set up the structure to prevent program termination on interrupted connections. */
    new_action.sa_handler = SIG_IGN;
    sigemptyset(&new_action.sa_mask);
    new_action.sa_flags = 0;

    /* install new signal handler for SIGPIPE*/
    sigaction(SIGPIPE, NULL, &old_action);
    if (old_action.sa_handler != SIG_IGN)
    {
        sigaction(SIGPIPE, &new_action, NULL);
    }
}
#elif defined(_WIN32)
/******************************************
 * Windows CTRL-C Handler implementation. *
 ******************************************/
# ifdef USE_CTRLC_ON_WINDOWS
#  include <windows.h>
#  include <conio.h>
BOOL CtrlHandler(DWORD fdwCtrlType)
{
    switch ( fdwCtrlType )
    {
    // Handle the CTRL-C signal.
    case CTRL_C_EVENT:
        std::cerr << '\n' << " Received CTRL-C signal." << '\n';
        g_ShutDown = 1;
        return( TRUE );     
    default:
        break;
    }
    return FALSE;
}
void RegisterSignalHandler()
{
    SetConsoleCtrlHandler( (PHANDLER_ROUTINE) CtrlHandler, TRUE );
}
# else /* USE_CTRLC_ON_WINDOWS */
void RegisterSignalHandler()
{
}
# endif /* USE_CTRLC_ON_WINDOWS */
#else /* __linux__ */
/********************************************************************
 * Dummy implementation for embedded systems. They never shut down. *
 ********************************************************************/
void RegisterSignalHandler()
{
}
#endif

char* getAppPath()
{
    char* pszAppPath = NULL;
    char* pszFind    = NULL;
    int   maxPath    = 0;
#ifdef _WIN32
#ifdef _WIN32_WCE
    TCHAR result[MAX_PATH];
    char  szAppPath[MAX_PATH];
    maxPath = MAX_PATH;
    GetModuleFileName(NULL, result, MAX_PATH);
    for ( int i=0; i<MAX_PATH; i++ ) szAppPath[i] = (char)result[i];
    pszFind = strrchr(szAppPath, '\\');
#else // #ifdef _WIN32_WCE
    char szAppPath[MAX_PATH];
    maxPath = MAX_PATH;
    GetModuleFileNameA(NULL, szAppPath, MAX_PATH);
    szAppPath[MAX_PATH-1] = 0;
    pszFind = strrchr(szAppPath, '\\');
#endif // #ifdef _WIN32_WCE
#elif defined(__QNX__)
    char szAppPath[PATH_MAX] = {0};
    getcwd(szAppPath, sizeof(szAppPath)-1);
#else // #ifdef _WIN32
    char szAppPath[PATH_MAX];
    maxPath = PATH_MAX;
    memset(szAppPath, 0, sizeof(szAppPath)); /* initialize with zeros, readlink does not null terminate the string */
#if defined (__VXWORKS__)
    strncpy(szAppPath, "ide0:/ESystem/opcua/dummy.exe", sizeof(szAppPath)-1);
#else //defined (__VXWORKS__)
    if ( readlink("/proc/self/exe", szAppPath, sizeof(szAppPath)-1) < 0 )
    {
        return NULL;
    }
#endif //defined (__VXWORKS__)
    pszFind = strrchr(szAppPath, '/');
#endif // #ifdef _WIN32
    if (pszFind)
    {
        *pszFind = 0; // cut off appname
    }
    pszAppPath = new char [maxPath+1];
    pszAppPath[maxPath] = 0;
    strncpy(pszAppPath, szAppPath, maxPath + 1);

    return pszAppPath;
}
