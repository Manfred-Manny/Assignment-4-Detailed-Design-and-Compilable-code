#include "Vessel.h"
#include <iostream>

// ---------------------------------------------------------------------------
// A4 STUB IMPLEMENTATION
// ---------------------------------------------------------------------------

bool Vessel::CreateVessel(const std::string &,
                          unsigned int,
                          unsigned int)
{
    std::cout << "[STUB] Vessel::CreateVessel()\n";
    return false;
}

bool Vessel::DeleteVessel(const std::string &)
{
    std::cout << "[STUB] Vessel::DeleteVessel()\n";
    return false;
}

bool Vessel::isVesselExist(const std::string &)
{
    return false;
}

void Vessel::initialize()
{
    std::cout << "[STUB] Vessel::initialize()\n";
}

void Vessel::shutdown()
{
    std::cout << "[STUB] Vessel::shutdown()\n";
}
