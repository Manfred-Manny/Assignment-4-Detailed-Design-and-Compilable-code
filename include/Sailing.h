//************************************************************
//************************************************************
//  Sailing.h
//  CMPT 276 – Assignment 4 (Fahad M)
//    Declares the Sailing class, which implements the core
//    business logic for managing sailings.
//
//    • Create sailings (arrival city, vessel, date, time)
//    • Delete sailings and associated reservations
//    • Print sailing status or full sailing reports
//    • Manage sailing lifecycle (initialize/shutdown)
//************************************************************
//************************************************************

#ifndef SAILING_H
#define SAILING_H

#include <string>
#include "CommonTypes.h"

// Status results for Sailing operations
enum class SailingStatus {
    SUCCESS,                 // Operation successful
    VESSEL_NOT_FOUND,        // Vessel does not exist
    SAILING_ALREADY_EXISTS   // Sailing already exists at this date/time
};

class Sailing
{
public:
    // Create new sailing
    static SailingStatus CreateSailing(
        const std::string &ArrivalCity,   // IN: Arrival city (3-letter code)
        const std::string &VesselName,    // IN: Vessel name
        const std::string &Date,          // IN: Date (YYYYMMDD)
        const std::string &Time           // IN: Time (HHMM)
    );

    // Delete sailing
    static bool DeleteSailing(
        SailingID sailingID               // IN: Sailing ID
    );

    // Print single sailing status
    static bool printStatus(
        SailingID sailingID               // IN: Sailing ID
    );

    // Print all sailings report
    static void printReport();

    // Initialize/Shutdown
    static void initialize();
    static void shutdown();
};

#endif // SAILING_H
