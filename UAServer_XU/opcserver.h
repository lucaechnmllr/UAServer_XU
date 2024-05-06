
#ifndef MAIN_OPCSERVER_H
#define MAIN_OPCSERVER_H

#pragma warning(push, 0)
#include "uaserverapplication.h"
#pragma warning(pop)

class UaServer;

class OpcServer: public UaServerApplication
{
public:
    // construction / destruction
    OpcServer();
    OpcServer(int argc, char* argv[], bool bRunAsService, const UaString &sApplicationName);
    virtual ~OpcServer();

    virtual OpcUa_DateTime getBuildDate() const;

protected:
    virtual UaStatus afterStartUp();
};


#endif // MAIN_OPCSERVER_H


