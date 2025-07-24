// ---------------------------------------------------------------------------
// FileIO_Reservations.cpp
// CMPT 276 – Assignment 4 (Fahad Y)
//
// Implements binary I/O for ferry reservations:
//   • Appending new reservations
//   • Searching and deleting by license + sailing ID
//   • Marking reservations as checked-in
//   • Counting reservations per sailing
//   • Computing available space per sailing
//
// Uses packed 27-byte struct to ensure consistent reads/writes.
// License and Sailing ID comparisons are case-insensitive.
// ---------------------------------------------------------------------------

#include "FileIO_Reservations.h"
#include "FileIO_VehicleRecord.h"
#include "FileIO_Sailings.h"
#include <fstream>
#include <iostream>
#include <cstdio>     // remove, rename
#include <algorithm>  // transform for case-insensitive compare

// ============================================================
// Helper: Convert string to uppercase (case-insensitive compare)
// ============================================================
static std::string toUpper(const std::string &s)
{
    std::string result = s;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::toupper(c); });
    return result;
}

// ============================================================
// Reset sequential read (no persistent pointer in this design)
// ============================================================
void FileIO_Reservations::reset()
{
    // Each getNextReservation opens its own stream; nothing to reset
}

// ============================================================
// Sequentially retrieve next reservation
// ============================================================
bool FileIO_Reservations::getNextReservation(std::string &licensePlate,
                                             SailingID &sailingID,
                                             bool &checkedIn)
{
    static std::ifstream file("reservations.dat", std::ios::binary);

    if (!file)
        return false;

    ReservationRec rec{};
    if (file.read(reinterpret_cast<char*>(&rec), sizeof(rec)))
    {
        licensePlate = FerrySys::decodeField(
            reinterpret_cast<unsigned char*>(rec.licenseplate),
            FerrySys::VEH_LIC_CHARS);
        sailingID = FerrySys::decodeField(
            reinterpret_cast<unsigned char*>(rec.sailingID),
            16);
        checkedIn = rec.checkedIn;
        return true;
    }

    return false;
}

// ============================================================
// Append new reservation to file
// ============================================================
bool FileIO_Reservations::writeReservation(const std::string &licensePlate,
                                           SailingID sailingID)
{
    std::ofstream file("reservations.dat", std::ios::binary | std::ios::app);
    if (!file)
        return false;

    ReservationRec rec{};
    FerrySys::encodeField(licensePlate,
                          reinterpret_cast<unsigned char*>(rec.licenseplate),
                          FerrySys::VEH_LIC_CHARS);
    FerrySys::encodeField(sailingID,
                          reinterpret_cast<unsigned char*>(rec.sailingID),
                          16);
    rec.checkedIn = false;

    file.write(reinterpret_cast<const char*>(&rec), sizeof(rec));
    return true;
}

// ============================================================
// Mark reservation as checked-in
// ============================================================
bool FileIO_Reservations::writeCheckin(const std::string &licensePlate,
                                       SailingID sailingID)
{
    std::fstream file("reservations.dat",
                      std::ios::binary | std::ios::in | std::ios::out);
    if (!file)
        return false;

    ReservationRec rec{};
    std::string searchLicense = toUpper(licensePlate);
    std::string searchID = toUpper(sailingID);

    while (file.read(reinterpret_cast<char*>(&rec), sizeof(rec)))
    {
        std::string currentLicense = toUpper(FerrySys::decodeField(
            reinterpret_cast<unsigned char*>(rec.licenseplate),
            FerrySys::VEH_LIC_CHARS));
        std::string currentID = toUpper(FerrySys::decodeField(
            reinterpret_cast<unsigned char*>(rec.sailingID),
            16));

        if (currentLicense == searchLicense && currentID == searchID)
        {
            rec.checkedIn = true;
            file.seekp(-static_cast<int>(sizeof(rec)), std::ios::cur);
            file.write(reinterpret_cast<const char*>(&rec), sizeof(rec));
            return true;
        }
    }

    return false;
}

// ============================================================
// Delete specific reservation (case-insensitive, safe rewrite)
// ============================================================
bool FileIO_Reservations::deleteReservation(const std::string &licensePlate,
                                            SailingID sailingID)
{
    std::ifstream inFile("reservations.dat", std::ios::binary);
    if (!inFile)
        return false;

    std::ofstream outFile("temp.dat", std::ios::binary);
    if (!outFile)
        return false;

    ReservationRec rec{};
    bool found = false;

    std::string searchLicense = toUpper(licensePlate);
    std::string searchID = toUpper(sailingID);

    while (inFile.read(reinterpret_cast<char*>(&rec), sizeof(rec)))
    {
        std::string currentLicense = toUpper(FerrySys::decodeField(
            reinterpret_cast<unsigned char*>(rec.licenseplate),
            FerrySys::VEH_LIC_CHARS));
        std::string currentID = toUpper(FerrySys::decodeField(
            reinterpret_cast<unsigned char*>(rec.sailingID),
            16));
        if (currentLicense == searchLicense && currentID == searchID)
        {
            found = true; // Skip this record 
        }
        else
        {
            outFile.write(reinterpret_cast<const char*>(&rec), sizeof(rec));
        }
    }

    inFile.close();
    outFile.close();

    if (found)
    {
        std::remove("reservations.dat");
        std::rename("temp.dat", "reservations.dat");
    }
    else
    {
        std::remove("temp.dat");
    }

    return found;
}

// ============================================================
// Count reservations for a given sailing (case-insensitive ID)
// ============================================================
int FileIO_Reservations::countReservationsForSailing(SailingID sailingID)
{
    std::ifstream file("reservations.dat", std::ios::binary);
    if (!file)
        return 0;

    ReservationRec rec{};
    int count = 0;
    std::string searchID = toUpper(sailingID);

    while (file.read(reinterpret_cast<char*>(&rec), sizeof(rec)))
    {
        std::string currentID = toUpper(FerrySys::decodeField(
            reinterpret_cast<unsigned char*>(rec.sailingID),
            16));

        if (currentID == searchID)
            count++;
    }

    return count;
}

// ============================================================
// Compute total space available (HCL + LCL) for a sailing
// ============================================================
int FileIO_Reservations::spaceAvailable(SailingID sailingID)
{
    Sailingrec sailing;
    if (!FileIO_Sailings::findSailing(sailingID, sailing))
        return -1;

    std::ifstream file("reservations.dat", std::ios::binary);
    if (!file)
        return sailing.remainingHCL + sailing.remainingLCL;

    ReservationRec rec{};
    int usedHCL = 0, usedLCL = 0;
    std::string searchID = toUpper(sailingID);

    while (file.read(reinterpret_cast<char*>(&rec), sizeof(rec)))
    {
        std::string currentID = toUpper(FerrySys::decodeField(
            reinterpret_cast<unsigned char*>(rec.sailingID),
            16));

        if (currentID == searchID)
        {
            std::string license = FerrySys::decodeField(
                reinterpret_cast<unsigned char*>(rec.licenseplate),
                FerrySys::VEH_LIC_CHARS);

            FerrySys::VehicleRecord vehicle;
            if (FerrySys::FileIO_VehicleRecord::findVehicle(license, vehicle))
            {
                if (vehicle.isSpecial())
                    usedHCL++;
                else
                    usedLCL++;
            }
        }
    }

    int availableHCL = sailing.remainingHCL - usedHCL;
    int availableLCL = sailing.remainingLCL - usedLCL;
    return availableHCL + availableLCL;
}
