#include "FileIO_Sailings.h"
#include <iostream>

// ---------------------------------------------------------------------------
// A4 STUB IMPLEMENTATION
// ---------------------------------------------------------------------------

void FileIO_Sailings::reset()
{
    std::cout << "[STUB] FileIO_Sailings::reset()\n";
}

bool FileIO_Sailings::getNextSailing(SailingID &sailingId,
                                     std::string &vesselName,
                                     std::string &arrivalCity,
                                     std::string &dateISO,
                                     std::string &timeHHMM,
                                     unsigned short &laneHCL,
                                     unsigned short &laneLCL)
{
    std::cout << "[STUB] FileIO_Sailings::getNextSailing()\n";
    sailingId = 0;
    vesselName.clear();
    arrivalCity.clear();
    dateISO.clear();
    timeHHMM.clear();
    laneHCL = 0;
    laneLCL = 0;
    return false; // EOF in stub
}

void FileIO_Sailings::writeSailing(SailingID,
                                   const std::string &,
                                   const std::string &,
                                   const std::string &,
                                   const std::string &,
                                   unsigned short,
                                   unsigned short)
{
    std::cout << "[STUB] FileIO_Sailings::writeSailing()\n";
}

bool FileIO_Sailings::deleteSailing(SailingID)
{
    std::cout << "[STUB] FileIO_Sailings::deleteSailing()\n";
    return false;
}
