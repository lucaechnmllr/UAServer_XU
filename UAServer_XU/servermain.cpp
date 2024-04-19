/******************************************************************************
** servermain.cpp
**
** Copyright (c) 2006-2024 Unified Automation GmbH. All rights reserved.
**
** Software License Agreement ("SLA") Version 2.8
**
** Unless explicitly acquired and licensed from Licensor under another
** license, the contents of this file are subject to the Software License
** Agreement ("SLA") Version 2.8, or subsequent versions
** as allowed by the SLA, and You may not copy or use this file in either
** source code or executable form, except in compliance with the terms and
** conditions of the SLA.
**
** All software distributed under the SLA is provided strictly on an
** "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
** AND LICENSOR HEREBY DISCLAIMS ALL SUCH WARRANTIES, INCLUDING WITHOUT
** LIMITATION, ANY WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
** PURPOSE, QUIET ENJOYMENT, OR NON-INFRINGEMENT. See the SLA for specific
** language governing rights and limitations under the SLA.
**
** The complete license agreement can be found here:
** http://unifiedautomation.com/License/SLA/2.8/
**
** Project: C++ OPC Server SDK sample code
**
** Description: Main entry for the application
**
******************************************************************************/
#include "opcserver.h"
#pragma warning(push,2)
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
            cerr << " Press " << SHUTDOWN_SEQUENCE << " to shutdown the server!" << endl;
            cerr << "***************************************************\n";

            
            // Wait for user command to terminate the server thread.
            while ( ShutDownFlag() == 0 )
            {
                UaThread::msleep(1000);
            }

            cerr << "***************************************************\n";
            cerr << " Shutting down server\n";
            cerr << "***************************************************\n";


            //- Stop OPC server -------------------------
            // This code can be integrated into a shutdown
            // sequence of the application where the
            // OPC server should be integrated
            //-------------------------------------------
            // Stop the server and wait three seconds if clients are connected
            // to allow them to disconnect after they received the shutdown signal
            pServer->stop(3, UaLocalizedText("", "User shutdown"));
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
int main(int, char*[])
#endif
{
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
