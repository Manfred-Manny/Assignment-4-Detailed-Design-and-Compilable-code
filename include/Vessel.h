//************************************************************
//************************************************************
//  Vessel.h
//  CMPT 276 – Assignment 4 (Fahad M)
//    Declares the Vessel class, which manages ferry vessel
//    records for the reservation system.
//
//    • Create and delete vessels
//    • Check existence of vessels
//    • Manage lifecycle (initialize/shutdown)
//    • Store vessel data for use in sailings and reservations
//************************************************************
//************************************************************

#ifndef VESSEL_H
#define VESSEL_H

#include <string>
#include <vector>
#include "CommonTypes.h"

// ---------------------------------------------------------------------------
// Status codes for vessel operations (create/delete)
// ---------------------------------------------------------------------------
enum class VesselStatus {
    SUCCESS,        // Operation successful
    ALREADY_EXISTS, // Vessel already exists in records
    NOT_FOUND       // Vessel not found in records
};

// ---------------------------------------------------------------------------
// Class: Vessel
// Description:
//     Provides static methods to manage vessel records, including creation,
//     deletion, and existence checking. Vessels are stored persistently and
//     used when creating sailings.
// ---------------------------------------------------------------------------
class Vessel
{
public:
    // Create a new vessel
    static VesselStatus CreateVessel(
        const std::string &vesselName,  // IN: Vessel name
        unsigned int laneHCL,           // IN: Number of High-Clearance Lanes
        unsigned int laneLCL            // IN: Number of Low-Clearance Lanes
    );

    // Delete an existing vessel
    static VesselStatus DeleteVessel(
        const std::string &vesselName   // IN: Vessel name
    );

    // Check if a vessel exists
    static bool isVesselExist(
        const std::string &vesselName   // IN: Vessel name
    );

    // Initialize/Shutdown vessel subsystem
    static void initialize();
    static void shutdown();

private:
    // Container for in-memory vessel records
    static std::vector<Vessel> vessels;

    // Path to persistent vessel data file
    static const std::string vesselFilePath;
};

#endif // VESSEL_H
