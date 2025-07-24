// ---------------------------------------------------------------------------
// FileIO_VehicleRecord.h
// CMPT 276 – Assignment 4(Fahad Y)
//
// This file provides binary file operations for managing `VehicleRecord`
// entries in the system. Each record is stored in a fixed-length binary format
// (32 bytes) and accessed by license plate.
//
// Supported operations include:
//   - Adds new vehicles
//   - Searching by license plate
//   - Deleting records
//   - Listing all records (optional/debug)
// ---------------------------------------------------------------------------

#ifndef FILEIO_VEHICLERECORD_H
#define FILEIO_VEHICLERECORD_H

#include "VehicleRecord.hpp"
#include <string>

namespace FerrySys {

class FileIO_VehicleRecord
{
public:
    //------------------------------------------------------------
    // Appends a new vehicle record to vehicles.dat
    // Returns true on success, false if write failed.
    static bool writeVehicle(
        const VehicleRecord &vehicle   // IN : fully populated vehicle record
    );

    //------------------------------------------------------------
    // Finds a vehicle by license plate.
    // Returns true if found and fills 'result', false otherwise.
    static bool findVehicle(
        const std::string &license,   // IN  : vehicle license (PK)
        VehicleRecord      &result    // OUT : decoded vehicle info
    );

    //------------------------------------------------------------
    // Checks if a vehicle with the given license exists.
    // Returns true if found, false otherwise.
    static bool vehicleExists(
        const std::string &license    // IN : license plate (PK)
    );

    //------------------------------------------------------------
    // Deletes a vehicle record by license plate.
    // Rewrites file without matching record.
    // Returns true if deleted, false if not found.
    static bool deleteVehicle(
        const std::string &license    // IN : license plate (PK)
    );

    //------------------------------------------------------------
    // Lists all vehicles in vehicles.dat (for debugging or reporting).
    // Simply prints each decoded record to std::cout.
    static void listVehicles();
};

} // namespace FerrySys

#endif // FILEIO_VEHICLERECORD_H
