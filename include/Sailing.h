//************************************************************
//************************************************************
//  Sailing.h
//************************************************************
//************************************************************
#ifndef SAILING_H
#define SAILING_H

#include "CommonTypes.h"
#include <string>

class Sailing
{
public:
//------------------------------------------------------------
    static bool CreateSailing(
         std::string arrivalCity,   // IN  : port of arrival
         std::string vesselName,    // IN  : vessel assigned
         std::string dateISO,       // IN  : "YYYY-MM-DD"
         std::string timeHHMM       // IN  : "HHMM"
    );
//------------------------------------------------------------
    static bool DeleteSailing(
        SailingID          sailingId      // IN  : sailing to delete
    );
//------------------------------------------------------------
    static void printStatus(
        SailingID          sailingId      // IN  : sailing to report
    );
//------------------------------------------------------------
    static void printReport();            // Prints 28-day report
//------------------------------------------------------------
    static bool isSailingExist( SailingID sailingId );
    static bool isVesselExist ( const std::string &vesselName );

//------------------------------------------------------------
    static void initialize();
    static void shutdown();
};

#endif // SAILING_H
