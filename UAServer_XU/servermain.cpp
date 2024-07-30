
#include "opcserver.h"
#pragma warning(push,0)
#include "uaplatformlayer.h"
#include "uathread.h"
#if SUPPORT_XML_PARSER
  #include "xmldocument.h"
#endif
#pragma warning(pop)

#include "shutdown.h"
#include "xu_nodemanagerxunamespace.h"
#include "xu_xuobjecttype.h"

#include <iostream>

using std::cerr; using std::endl;

int OpcServerMain(const char* szAppPath)
{
    int ret = 0;

    //- Initialize the environment --------------
#if SUPPORT_XML_PARSER
    // Initialize the XML Parser
    UaXmlDocument::initParser();
#endif
    // Initialize the UA Stack platform layer
    ret = UaPlatformLayer::init();
    //-------------------------------------------

    if ( ret == 0 )
    {
        // Create configuration file name
        UaString sConfigFileName(szAppPath);

#if SUPPORT_XML_PARSER
        sConfigFileName += "/ServerConfig.xml";
#else
        sConfigFileName += "/ServerConfig.ini";
#endif


        //- Start up OPC server ---------------------
        // This code can be integrated into a startup
        // sequence of the application where the
        // OPC server should be integrated
        //-------------------------------------------
        // Create and initialize server object
        OpcServer* pServer = new OpcServer;
        pServer->setServerConfig(sConfigFileName, szAppPath);

        // Add NodeManager for the server specific nodes
        XU::NodeManagerXUNamespace* pNM = new XU::NodeManagerXUNamespace(false);
        pServer->addNodeManager(pNM);



        // Start server object
        ret = pServer->start();
        if ( ret != 0 )
        {
            delete pServer;
            pServer = 0;
        }
        //-------------------------------------------

        if ( ret == 0 )
        {
            cerr << "***************************************************\n";
            cerr << " Press " << "CTRL_BREAK" << " or " << "CTRL_C" << " to shutdown the server!" << endl;
            cerr << "***************************************************\n";

            
            // Wait for user command to terminate the server thread.
            while ( ShutDownFlag() == 0 )
            {
                UaThread::msleep(1000);
            }

            cerr << "***************************************************\n";
            cerr << " Shutting down server (cleanup)\n";
            cerr << "***************************************************\n";


            //- Stop OPC server -------------------------
            // This code can be integrated into a shutdown
            // sequence of the application where the
            // OPC server should be integrated
            //-------------------------------------------
            // Stop the server and wait three seconds if clients are connected
            // to allow them to disconnect after they received the shutdown signal
            pServer->stop(3, UaLocalizedText("", "User shutdown"));

            cerr << "***************************************************\n";
            cerr << " Server has been successfully terminated!\n";
            cerr << "***************************************************\n";

            delete pServer;
            pServer = NULL;
            //-------------------------------------------
        }
    }

    //- Clean up the environment --------------
    // Clean up the UA Stack platform layer
    UaPlatformLayer::cleanup();
#if SUPPORT_XML_PARSER
    // Clean up the XML Parser
    UaXmlDocument::cleanupParser();
#endif
    //-------------------------------------------

    return ret;
}

#ifdef _WIN32_WCE
int WINAPI WinMain( HINSTANCE, HINSTANCE, LPWSTR, int)
#else
int main(int argc, char* argv[])
#endif
{
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <0|1>" << std::endl;
        return 1;
    }

    // Read the argument and convert it to an integer
    int ServerMode = std::stoi(argv[1]);

    // Check the value and set the static variable
    if (ServerMode == 0 || ServerMode == 1) {
        XU::NodeManagerXUNamespace::setServerMode(ServerMode);
    }
    else {
        std::cerr << "Invalid argument. Please use 0 or 1." << std::endl;
        return 1;
    }

    std::cerr << "***************************************************\n";
    if (ServerMode)
        std::cerr << " Server will be started in RapidMode.\n Every write operation will be forced!\n";
    else
        std::cerr << " Server is started in normal mode.\n The internal cycle of STDINtoXU is used for write operations!\n";
    std::cerr << "***************************************************\n";

    int ret = 0;

    RegisterSignalHandler();

    // Extract application path
    char* pszAppPath = getAppPath();

    //-------------------------------------------
    // Call the OPC server main method
    ret = OpcServerMain(pszAppPath);
    //-------------------------------------------

    if ( pszAppPath ) delete [] pszAppPath;

    return ret;
}
