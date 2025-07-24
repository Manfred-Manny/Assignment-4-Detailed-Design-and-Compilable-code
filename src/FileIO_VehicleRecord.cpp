//************************************************************
//************************************************************
//  FileIO_VehicleRecord.cpp
//  CMPT 276 – Assignment 4 (Fahad Y)
//    Implements file I/O operations for vehicle records,
//    supporting creation, lookup, listing, and deletion.
//
//    • Encodes/decodes VehicleRecord to fixed-length binary
//    • Appends new vehicles to vehicles.dat
//    • Supports searching and deleting vehicles by license
//    • Provides debug listing of all vehicles
//************************************************************
//************************************************************

#include "FileIO_VehicleRecord.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cstring>

namespace FerrySys {

// ---------------------------------------------------------------------------
// Append new vehicle to vehicles.dat
// ---------------------------------------------------------------------------
bool FileIO_VehicleRecord::writeVehicle(const VehicleRecord &vehicle)
{
    VehicleRaw raw{};
    encodeVehicle(vehicle, raw);

    std::ofstream file("vehicles.dat", std::ios::binary | std::ios::app);
    if (!file) {
        std::cerr << "Error: Could not open vehicles.dat for writing.\n";
        return false;
    }

    file.write(reinterpret_cast<const char*>(raw.data()), VEH_REC_BYTES);
    return true;
}

// ---------------------------------------------------------------------------
// Find vehicle by license and load into result
// ---------------------------------------------------------------------------
bool FileIO_VehicleRecord::findVehicle(const std::string &license, VehicleRecord &result)
{
    std::ifstream file("vehicles.dat", std::ios::binary);
    if (!file) return false;

    VehicleRaw raw{};
    while (file.read(reinterpret_cast<char*>(raw.data()), VEH_REC_BYTES)) {
        VehicleRecord temp;
        decodeVehicle(raw, temp);

        if (temp.license == license) {
            result = temp;
            return true;
        }
    }
    return false;
}

// ---------------------------------------------------------------------------
// Check if a vehicle exists by license
// ---------------------------------------------------------------------------
bool FileIO_VehicleRecord::vehicleExists(const std::string &license)
{
    VehicleRecord temp;
    return findVehicle(license, temp);
}

// ---------------------------------------------------------------------------
// List all vehicles (formatted table output for debugging/reporting)
// ---------------------------------------------------------------------------
void FileIO_VehicleRecord::listVehicles()
{
    std::ifstream file("vehicles.dat", std::ios::binary);
    if (!file) {
        std::cout << "No vehicle records found.\n";
        return;
    }

    std::cout << "------------------------------------------------------------\n";
    std::cout << std::left << std::setw(12) << "License"
              << std::setw(16) << "Phone"
              << std::setw(10) << "Length(cm)"
              << std::setw(10) << "Height(cm)"
              << "Type\n";
    std::cout << "------------------------------------------------------------\n";

    VehicleRaw raw{};
    while (file.read(reinterpret_cast<char*>(raw.data()), VEH_REC_BYTES)) {
        VehicleRecord vehicle;
        decodeVehicle(raw, vehicle);

        std::cout << std::left << std::setw(12) << vehicle.license
                  << std::setw(16) << vehicle.phone
                  << std::setw(10) << vehicle.length_cm
                  << std::setw(10) << vehicle.height_cm
                  << (vehicle.isSpecial() ? "Special (HCL)" : "Standard (LCL)") << "\n";
    }
    std::cout << "------------------------------------------------------------\n";
}

// ---------------------------------------------------------------------------
// Delete vehicle by rewriting file without the target record
// ---------------------------------------------------------------------------
bool FileIO_VehicleRecord::deleteVehicle(const std::string &license)
{
    std::ifstream file("vehicles.dat", std::ios::binary);
    if (!file) return false;

    std::ofstream temp("temp.dat", std::ios::binary);
    if (!temp) return false;

    VehicleRaw raw{};
    bool found = false;

    while (file.read(reinterpret_cast<char*>(raw.data()), VEH_REC_BYTES)) {
        VehicleRecord vehicle;
        decodeVehicle(raw, vehicle);

        if (vehicle.license == license) {
            found = true; // Skip this record (delete)
        } else {
            temp.write(reinterpret_cast<const char*>(raw.data()), VEH_REC_BYTES);
        }
    }

    file.close();
    temp.close();

    std::remove("vehicles.dat");
    std::rename("temp.dat", "vehicles.dat");

    return found;
}

} // namespace FerrySys
