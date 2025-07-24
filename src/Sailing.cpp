#include "Sailing.h"
#include "FileIO_Vessel.h"
#include "FileIO_Sailings.h"

// ---------------------------------------------------------------------------
// Create New Sailing
SailingStatus Sailing::CreateSailing(const std::string &ArrivalCity,const std::string &VesselName,const std::string &Date,
const std::string &Time)
{
    // Build Sailing ID
    // Date: YY-MM-DD → we want DD (last 2 chars)
    std::string day = Date.substr(6, 2);  // Starts at position 6, length 2
    // Time: HHMM → we want HH (first 2 chars)
    std::string hour = Time.substr(0, 2); // Starts at position 0, length 2
   // Build SailingID
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
// Delete Sailing (still simple yes/no result)
bool Sailing::DeleteSailing(SailingID sailingID)
{
    if (!FileIO_Sailings::Sailingexist(sailingID))
        return false;

    return FileIO_Sailings::deleteSailing(sailingID);
}

// ---------------------------------------------------------------------------
// Print Status
bool Sailing::printStatus(SailingID sailingID)
{
    if (!FileIO_Sailings::Sailingexist(sailingID))
        return false;

    return FileIO_Sailings::sailingstatus(sailingID);
}

// ---------------------------------------------------------------------------
void Sailing::printReport()
{
    FileIO_Sailings::Sailingreport();
}

void Sailing::initialize() {}
void Sailing::shutdown() {}
