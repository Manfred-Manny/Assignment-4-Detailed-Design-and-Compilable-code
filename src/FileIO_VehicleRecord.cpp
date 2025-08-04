// ---------------------------------------------------------------------------
// FileIO_VehicleRecord.cpp
// CMPT 276 – Assignment 4 (Fahad Y)
//
// Implements binary I/O for vehicle records using fixed-length encoding.
// Uses helpers from VehicleRecord.hpp for license padding and full record
// encode/decode. Ensures safe deletion and consistent formatting.
// ---------------------------------------------------------------------------

#include "FileIO_VehicleRecord.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cstdio> // for remove, rename

namespace FerrySys
{

// ============================================================
// Append new vehicle record to vehicles.dat
// ============================================================
bool FileIO_VehicleRecord::writeVehicle(const VehicleRecord &vehicle)
{
    VehicleRaw raw{};
    encodeVehicle(vehicle, raw); // Encode full record (license, phone, dims)

    std::ofstream file("vehicles.dat", std::ios::binary | std::ios::app);
    if (!file)
    {
        std::cerr << "Error: Could not open vehicles.dat for writing.\n";
        return false;
    }

    file.write(reinterpret_cast<const char*>(raw.data()), VEH_REC_BYTES);
    return true;
}

// ============================================================
// Find vehicle by license plate
// ============================================================
bool FileIO_VehicleRecord::findVehicle(const std::string &license,
                                       VehicleRecord &result)
{
    std::ifstream file("vehicles.dat", std::ios::binary);
    if (!file)
        return false;

    VehicleRaw raw{};
    while (file.read(reinterpret_cast<char*>(raw.data()), VEH_REC_BYTES))
    {
        VehicleRecord temp;
        decodeVehicle(raw, temp);

        if (temp.license == license)
        {
            result = temp;
            return true;
        }
    }

    return false;
}

// ============================================================
// Check if vehicle exists by license
// ============================================================
bool FileIO_VehicleRecord::vehicleExists(const std::string &license)
{
    VehicleRecord temp;
    return findVehicle(license, temp);
}

// ============================================================
// List all vehicles in vehicles.dat (formatted for debugging)
// ============================================================
void FileIO_VehicleRecord::listVehicles()
{
    std::ifstream file("vehicles.dat", std::ios::binary);
    if (!file)
    {
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
    while (file.read(reinterpret_cast<char*>(raw.data()), VEH_REC_BYTES))
    {
        VehicleRecord vehicle;
        decodeVehicle(raw, vehicle);

        std::cout << std::left << std::setw(12) << vehicle.license
                  << std::setw(16) << vehicle.phone
                  << std::setw(10) << vehicle.length_m
                  << std::setw(10) << vehicle.height_m
                  << (vehicle.isSpecial() ? "Special (HCL)" : "Standard (LCL)") << "\n";
    }

    std::cout << "------------------------------------------------------------\n";
}

// ============================================================
// Delete vehicle by rewriting file without target record
// ============================================================
bool FileIO_VehicleRecord::deleteVehicle(const std::string &license)
{
    std::ifstream file("vehicles.dat", std::ios::binary);
    if (!file)
        return false;

    std::ofstream temp("temp.dat", std::ios::binary);
    if (!temp)
        return false;

    VehicleRaw raw{};
    bool found = false;

    while (file.read(reinterpret_cast<char*>(raw.data()), VEH_REC_BYTES))
    {
        VehicleRecord vehicle;
        decodeVehicle(raw, vehicle);

        if (vehicle.license == license)
        {
            found = true; // Skip this record (delete it)
        }
        else
        {
            temp.write(reinterpret_cast<const char*>(raw.data()), VEH_REC_BYTES);
        }
    }

    file.close();
    temp.close();

    if (found)
    {
        std::remove("vehicles.dat");
        std::rename("temp.dat", "vehicles.dat");
    }
    else
    {
        std::remove("temp.dat");
    }

    return found;
}

} // namespace FerrySys
