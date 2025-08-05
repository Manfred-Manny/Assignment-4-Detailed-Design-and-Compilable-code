//************************************************************
//************************************************************
//  Sailing.cpp
//  CMPT 276 – Assignment 4 (Fahad M)
//
//  PURPOSE:
//    Implements the Sailing class business logic.
//************************************************************
//************************************************************

#include "Sailing.h"
#include "FileIO_Vessel.h"
#include "FileIO_Sailings.h"

// Create a new sailing
SailingStatus Sailing::CreateSailing(const std::string &ArrivalCity,
                                     const std::string &VesselName,
                                     const std::string &Date,
                                     const std::string &Time)
{
    // Build Sailing ID (CITY:DD:HH)
    std::string day = Date.substr(6, 2);
    std::string hour = Time.substr(0, 2);
    std::string SailingID = ArrivalCity + ":" + day + ":" + hour;

    // Validate vessel
    unsigned int laneHCL = 0, laneLCL = 0;
    if (!FileIO_Vessel::getVesselByName(VesselName, laneHCL, laneLCL))
        return SailingStatus::VESSEL_NOT_FOUND;

    // Check if sailing exists
    if (FileIO_Sailings::Sailingexist(SailingID))
        return SailingStatus::SAILING_ALREADY_EXISTS;

    // Save to file
    FileIO_Sailings::writeSailing(SailingID, VesselName, laneHCL, laneLCL);
    return SailingStatus::SUCCESS;
}

// Delete sailing
bool Sailing::DeleteSailing(SailingID sailingID)
{
    if (!FileIO_Sailings::Sailingexist(sailingID))
        return false;
    return FileIO_Sailings::deleteSailing(sailingID);
}

// Print sailing status
bool Sailing::printStatus(SailingID sailingID)
{
    if (!FileIO_Sailings::Sailingexist(sailingID))
        return false;
    return FileIO_Sailings::sailingstatus(sailingID);
}

// Lifecycle
void Sailing::initialize() {}
void Sailing::shutdown() {}
