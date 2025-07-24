//************************************************************
//************************************************************
//  Sailing.cpp
//  CMPT 276 – Assignment 4 (Fahad M)
//    Implements the Sailing class, which manages the creation,
//    deletion, and reporting of ferry sailings.
//
//    • Builds SailingID using Arrival City, Date, and Time
//    • Validates vessel existence before creating sailings
//    • Prevents duplicate sailings (same date/time)
//    • Supports printing status and full sailing reports
//************************************************************
//************************************************************

#include "Sailing.h"
#include "FileIO_Vessel.h"
#include "FileIO_Sailings.h"

// ---------------------------------------------------------------------------
// Create New Sailing
// ---------------------------------------------------------------------------
SailingStatus Sailing::CreateSailing(const std::string &ArrivalCity,
                                     const std::string &VesselName,
                                     const std::string &Date,
                                     const std::string &Time)
{
    // Build Sailing ID
    // Extract day from date (YY-MM-DD → DD)
    std::string day = Date.substr(6, 2);

    // Extract hour from time (HHMM → HH)
    std::string hour = Time.substr(0, 2);

    // Combine parts into SailingID format: CITY:DD:HH
    std::string SailingID = ArrivalCity + ":" + day + ":" + hour;

    // 1. Check vessel existence
    unsigned int laneHCL = 0;
    unsigned int laneLCL = 0;
    if (!FileIO_Vessel::getVesselByName(VesselName, laneHCL, laneLCL))
        return SailingStatus::VESSEL_NOT_FOUND;

    // 2. Check if sailing already exists
    if (FileIO_Sailings::Sailingexist(SailingID))
        return SailingStatus::SAILING_ALREADY_EXISTS;

    // 3. Write sailing record
    FileIO_Sailings::writeSailing(SailingID, VesselName, laneHCL, laneLCL);
    return SailingStatus::SUCCESS;
}

// ---------------------------------------------------------------------------
// Delete Sailing
// ---------------------------------------------------------------------------
bool Sailing::DeleteSailing(SailingID sailingID)
{
    if (!FileIO_Sailings::Sailingexist(sailingID))
        return false;

    return FileIO_Sailings::deleteSailing(sailingID);
}

// ---------------------------------------------------------------------------
// Print Status for a Single Sailing
// ---------------------------------------------------------------------------
bool Sailing::printStatus(SailingID sailingID)
{
    if (!FileIO_Sailings::Sailingexist(sailingID))
        return false;

    return FileIO_Sailings::sailingstatus(sailingID);
}

// ---------------------------------------------------------------------------
// Print Full Sailing Report
// ---------------------------------------------------------------------------
void Sailing::printReport()
{
    FileIO_Sailings::Sailingreport();
}

// ---------------------------------------------------------------------------
// Lifecycle (Initialize / Shutdown)
// ---------------------------------------------------------------------------
void Sailing::initialize() {}
void Sailing::shutdown() {}
