#include "FileIO_Vessel.h"
#include <iostream>

// ---------------------------------------------------------------------------
// A4 STUB IMPLEMENTATION
// ---------------------------------------------------------------------------

void FileIO_Vessel::reset()
{
    std::cout << "[STUB] FileIO_Vessel::reset()\n";
}

bool FileIO_Vessel::getNextVessel(std::string &vesselName,
                                  unsigned int &laneHCL,
                                  unsigned int &laneLCL)
{
    std::cout << "[STUB] FileIO_Vessel::getNextVessel()\n";
    vesselName.clear();
    laneHCL = 0;
    laneLCL = 0;
    return false; // EOF in stub
}

void FileIO_Vessel::writeVessel(const std::string &,
                                unsigned int,
                                unsigned int)
{
    std::cout << "[STUB] FileIO_Vessel::writeVessel()\n";
}

bool FileIO_Vessel::deleteVessel(const std::string &)
{
    std::cout << "[STUB] FileIO_Vessel::deleteVessel()\n";
    return false;
}

std::string FileIO_Vessel::getVesselName(unsigned int)
{
    std::cout << "[STUB] FileIO_Vessel::getVesselName()\n";
    return {};
}
