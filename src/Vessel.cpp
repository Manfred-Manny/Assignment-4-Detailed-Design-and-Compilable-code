//************************************************************
//************************************************************
//  Vessel.cpp
//  CMPT 276 – Assignment 4 (Fahad Y)
//    Implements the Vessel class logic, handling creation,
//    deletion, and existence checks for ferry vessels.
//
//    • Validates and prevents duplicate vessel entries
//    • Writes and deletes vessel records in persistent storage
//    • Provides utilities to check vessel existence
//************************************************************
//************************************************************

#include "Vessel.h"
#include "FileIO_Vessel.h"
#include <vector>
#include <fstream>

// ---------------------------------------------------------------------------
// Static member definitions
// ---------------------------------------------------------------------------
std::vector<Vessel> Vessel::vessels;
const std::string Vessel::vesselFilePath = "vessels.dat";

// ---------------------------------------------------------------------------
// Create a new vessel record
// ---------------------------------------------------------------------------
VesselStatus Vessel::CreateVessel(const std::string &vesselName,
                                  unsigned int laneHCL,
                                  unsigned int laneLCL)
{
    // Check if vessel already exists
    if (isVesselExist(vesselName)) {
        return VesselStatus::ALREADY_EXISTS;
    }

    // Write vessel to persistent file
    FileIO_Vessel::writeVessel(vesselName, laneHCL, laneLCL);
    return VesselStatus::SUCCESS;
}

// ---------------------------------------------------------------------------
// Delete an existing vessel record
// ---------------------------------------------------------------------------
VesselStatus Vessel::DeleteVessel(const std::string &vesselNametoDelete)
{
    // Ensure vessel exists before attempting deletion
    if (!isVesselExist(vesselNametoDelete))
        return VesselStatus::NOT_FOUND;

    if (FileIO_Vessel::deleteVessel(vesselNametoDelete))
        return VesselStatus::SUCCESS;

    // Could be enhanced: differentiate between not found vs I/O error
    return VesselStatus::NOT_FOUND;
}

// ---------------------------------------------------------------------------
// Check if a vessel with the given name exists
// ---------------------------------------------------------------------------
bool Vessel::isVesselExist(const std::string &vesselName)
{
    std::fstream file("vessels.dat", std::ios::binary | std::ios::in);
    if (!file) return false;

    Vesselrec rec;
    while (file.read(reinterpret_cast<char*>(&rec), sizeof(rec))) {
        std::string nameFromFile(rec.vesselName);
        nameFromFile = nameFromFile.substr(0, nameFromFile.find('\0'));

        if (nameFromFile == vesselName) {
            return true;
        }
    }

    return false;
}

// ---------------------------------------------------------------------------
// Lifecycle (Initialize / Shutdown)
// ---------------------------------------------------------------------------
void Vessel::initialize() {}
void Vessel::shutdown() {}
