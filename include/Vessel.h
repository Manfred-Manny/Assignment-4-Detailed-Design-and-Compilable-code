//************************************************************
//************************************************************
//  Vessel.h
//  Domain logic for vessel master list
//************************************************************
//************************************************************
#ifndef VESSEL_H
#define VESSEL_H

#include <string>

class Vessel
{
public:
//------------------------------------------------------------
// Creates vessel record if name unique.
// Preconditions : name not in master list.
// Postconditions: record saved, true returned on success.
    static bool CreateVessel(
        const std::string &vesselName,   // IN : mnemonic
        unsigned int        laneHCL,     // IN : high-ceiling metres
        unsigned int        laneLCL      // IN : low-ceiling metres
    );

//------------------------------------------------------------
// Deletes vessel from master list.
// Preconditions : vessel not assigned to an active sailing.
// Postconditions: record removed, true returned on success.
    static bool DeleteVessel(
        const std::string &vesselName    // IN : mnemonic
    );

//------------------------------------------------------------
// Checks for vessel’s existence.
// Preconditions : none
// Returns      : true if vesselName found.
    static bool isVesselExist(
        const std::string &vesselName    // IN : mnemonic
    );

//------------------------------------------------------------
// Loads master list into memory.
// Preconditions : called once at startup.
    static void initialize();

//------------------------------------------------------------
// Persists list & releases memory.
// Preconditions : called once at shutdown.
    static void shutdown();
};

#endif // VESSEL_H
