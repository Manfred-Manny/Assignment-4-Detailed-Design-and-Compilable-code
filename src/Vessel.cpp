#include "Vessel.h"
#include "FileIO_Vessel.h"
#include <vector>
#include <fstream>

std::vector<Vessel> Vessel::vessels;
const std::string Vessel::vesselFilePath = "vessels.dat";

// ---------------------------------------------------------------------------
// Creates a vessel record (pure logic)
VesselStatus Vessel::CreateVessel(const std::string &vesselName,
                                  unsigned int laneHCL,
                                  unsigned int laneLCL)
{
    // Check if vessel already exists
    if (isVesselExist(vesselName)) {
        return VesselStatus::ALREADY_EXISTS;
    }

    // Write vessel to file
    FileIO_Vessel::writeVessel(vesselName, laneHCL, laneLCL);
    return VesselStatus::SUCCESS;
}

// ---------------------------------------------------------------------------
// Deletes a vessel (pure logic)
VesselStatus Vessel::DeleteVessel(const std::string &vesselNametoDelete)
{
    if (!isVesselExist(vesselNametoDelete))
        return VesselStatus::NOT_FOUND;

    if (FileIO_Vessel::deleteVessel(vesselNametoDelete))
        return VesselStatus::SUCCESS;

    return VesselStatus::NOT_FOUND; // Or another error state
}

// ---------------------------------------------------------------------------
// Checks if a vessel with the given name exists
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
void Vessel::initialize() {}
void Vessel::shutdown() {}
