// ---------------------------------------------------------------------------
// FileIO_VehicleRecord.h
// CMPT 276 – Assignment 4 (Fahad Y)
//
// Binary file operations for vehicle records:
//   • Append new vehicles
//   • Search by license plate
//   • Check if vehicle exists
//   • Delete vehicle
//   • List all vehicles (debug/reporting)
//
// Records are stored in fixed-length binary format (32 bytes).
// ---------------------------------------------------------------------------

#ifndef FILEIO_VEHICLERECORD_H
#define FILEIO_VEHICLERECORD_H

#include "VehicleRecord.hpp" // This already defines VEH_LIC_CHARS, VEH_PHONE_CHARS, VEH_REC_BYTES
#include <string>

namespace FerrySys
{

class FileIO_VehicleRecord
{
public:
    // Append a new vehicle record to vehicles.dat
    static bool writeVehicle(const VehicleRecord &vehicle);

    // Find a vehicle by license plate (returns decoded record in result)
    static bool findVehicle(const std::string &license, VehicleRecord &result);

    // Check if vehicle exists by license
    static bool vehicleExists(const std::string &license);

    // Delete a vehicle by license (safe file rewrite)
    static bool deleteVehicle(const std::string &license);

    // List all vehicles in vehicles.dat (formatted debug output)
    static void listVehicles();
};

} // namespace FerrySys

#endif // FILEIO_VEHICLERECORD_H
