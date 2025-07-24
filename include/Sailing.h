#ifndef SAILING_H
#define SAILING_H

#include <string>
#include "CommonTypes.h"

enum class SailingStatus {
    SUCCESS,
    VESSEL_NOT_FOUND,
    SAILING_ALREADY_EXISTS
};

class Sailing
{
public:
    static SailingStatus CreateSailing(
        const std::string &ArrivalCity,         //IN:Arrival City
        const std::string &VesselName,          //IN:Vessel Name
        const std::string &Date,                //IN:Date
        const std::string &Time                 //IN:Time
    );

    static bool DeleteSailing(
        SailingID sailingID                     //IN:SailingID
    );

    static bool printStatus(
        SailingID sailingID                      //IN:SailingID
    );

    static void printReport();

    static void initialize();
    static void shutdown();
};

#endif // SAILING_H
