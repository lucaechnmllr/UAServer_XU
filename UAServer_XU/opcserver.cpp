

#pragma warning(push, 0)
#include "uamodule.h"
#include "uasession.h"
#pragma warning(pop)

#include "opcserver.h"

#include <iostream>

#ifndef UA_BUILD_DATE_ZONE
#define UA_BUILD_DATE_ZONE 1 // Must match UTC offset and daylight saving time at build date
#endif /* UA_BUILD_DATE_ZONE */

/** Construction. */
OpcServer::OpcServer()
{}

/** Construction. */
OpcServer::OpcServer(int argc, char* argv[], bool bRunAsService, const UaString &sApplicationName)
: UaServerApplication(argc, argv, bRunAsService, sApplicationName)
{}

/** Destruction. */
OpcServer::~OpcServer()
{
    if ( isStarted() != OpcUa_False )
    {
        UaLocalizedText reason("en","Application shut down");
        stop(0, reason);
    }
}

UaStatus OpcServer::afterStartUp()
{
    UaStatus ret = UaServerApplication::afterStartUp();
    if ( ret.isGood() )
    {
        UaString        sRejectedCertificateDirectory;
        OpcUa_UInt32    nRejectedCertificatesCount;
        UaEndpointArray uaEndpointArray;
        getServerConfig()->getEndpointConfiguration(
            sRejectedCertificateDirectory,
            nRejectedCertificatesCount,
            uaEndpointArray);
        if ( uaEndpointArray.length() > 0 )
        {
            std::cerr << "***************************************************\n";
            std::cerr << " Server opened endpoints for following URLs:\n";
            OpcUa_UInt32 idx;
            bool bError = false;
            for ( idx=0; idx<uaEndpointArray.length(); idx++ )
            {
                if ( uaEndpointArray[idx]->isOpened() )
                {
                    std::cerr << "     " <<  uaEndpointArray[idx]->sEndpointUrl().toUtf8() << "\n";
                }
                else
                {
                    bError = true;
                }
            }
            if ( bError )
            {
                std::cerr << "\n";
                std::cerr << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
                std::cerr << "!!!! The following endpoints URLs failed:\n";
                for ( idx=0; idx<uaEndpointArray.length(); idx++ )
                {
                    if ( uaEndpointArray[idx]->isOpened() == false )
                    {
                        std::cerr << "!!!! %s\n", uaEndpointArray[idx]->sEndpointUrl().toUtf8();
                    }
                }
                std::cerr << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
                std::cerr << "\n";
            }
            std::cerr << "***************************************************\n";
        }
    }

    return ret;
}

/** Get the build date from the static compiled in string.
 *  @return the build date from the static compiled in string.
 */
OpcUa_DateTime OpcServer::getBuildDate() const
{
    static OpcUa_DateTime date;
    static const char szDate[] = __DATE__; /* "Mon DD YYYY" */
    static char szISO[] = "YYYY-MM-DDT" __TIME__ "Z";
    static const char* Months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
    char mon = 0;

    /* set year */
    szISO[0] = szDate[7];
    szISO[1] = szDate[8];
    szISO[2] = szDate[9];
    szISO[3] = szDate[10];

    /* set month */
    while ( (strncmp(Months[(int)mon], szDate, 3) != 0) && (mon < 11) )
    {
        mon++;
    }
    mon++;
    szISO[5] = '0' + mon / 10;
    szISO[6] = '0' + mon % 10;

    /* set day */
    szISO[8] = szDate[4];
    szISO[9] = szDate[5];

    /* convert to UA time */
    OpcUa_DateTime_GetDateTimeFromString(szISO, &date);

    /* correct time */
    UaDateTime buildDate(date);
    buildDate.addSecs(UA_BUILD_DATE_ZONE * 3600 * -1);

    return buildDate;
}
