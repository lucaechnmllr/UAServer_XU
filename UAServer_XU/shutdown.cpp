
#pragma warning(push,0)
#include "uaplatformlayer.h"
#pragma warning(pop)

#include "shutdown.h"

#include <Windows.h>
#include <iostream>
#include <string.h>


/* shutdown flag */
static volatile unsigned int g_ShutDown = 0;



BOOL WINAPI ConsoleHandler(DWORD signal)
{
    if (signal == CTRL_BREAK_EVENT)
    {
        std::cerr << "CTRL_BREAK_EVENT received!\n";
        g_ShutDown = 1;
    }
    if (signal == CTRL_C_EVENT)
    {
        std::cerr << "CTRL_C_EVENT received\n";
        g_ShutDown = 1;
    }
    return TRUE;
}

/* Return shutdown flag. */
unsigned int ShutDownFlag()
{
    return g_ShutDown;
}


void RegisterSignalHandler()
{
    SetConsoleCtrlHandler(ConsoleHandler, TRUE);
}


char* getAppPath()
{
    char* pszAppPath = NULL;
    char* pszFind = NULL;
    int   maxPath = 0;
#ifdef _WIN32
#ifdef _WIN32_WCE
    TCHAR result[MAX_PATH];
    char  szAppPath[MAX_PATH];
    maxPath = MAX_PATH;
    GetModuleFileName(NULL, result, MAX_PATH);
    for (int i = 0; i < MAX_PATH; i++) szAppPath[i] = (char)result[i];
    pszFind = strrchr(szAppPath, '\\');
#else // #ifdef _WIN32_WCE
    char szAppPath[MAX_PATH];
    maxPath = MAX_PATH;
    GetModuleFileNameA(NULL, szAppPath, MAX_PATH);
    szAppPath[MAX_PATH - 1] = 0;
    pszFind = strrchr(szAppPath, '\\');
#endif // #ifdef _WIN32_WCE
#elif defined(__QNX__)
    char szAppPath[PATH_MAX] = { 0 };
    getcwd(szAppPath, sizeof(szAppPath) - 1);
#else // #ifdef _WIN32
    char szAppPath[PATH_MAX];
    maxPath = PATH_MAX;
    memset(szAppPath, 0, sizeof(szAppPath)); /* initialize with zeros, readlink does not null terminate the string */
#if defined (__VXWORKS__)
    strncpy(szAppPath, "ide0:/ESystem/opcua/dummy.exe", sizeof(szAppPath) - 1);
#else //defined (__VXWORKS__)
    if (readlink("/proc/self/exe", szAppPath, sizeof(szAppPath) - 1) < 0)
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
    pszAppPath = new char[maxPath + 1];
    pszAppPath[maxPath] = 0;
    strncpy(pszAppPath, szAppPath, maxPath + 1);

    return pszAppPath;
}
