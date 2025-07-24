//************************************************************
//************************************************************
//  FileIO_Reservations.cpp
//  CMPT 276 – Assignment 4 (Fahad Y)
//    Implements file I/O operations for reservations, including
//    creation, deletion, check-in, and queries for available space.
//
//    • Encodes/decodes reservation records to fixed-length binary
//    • Supports appending, updating, and sequential retrieval
//    • Cascade deletion when a sailing is removed
//    • Works alongside VehicleRecord and Sailing I/O modules
//************************************************************
//************************************************************

#include "FileIO_Reservations.h"
#include "FileIO_VehicleRecord.h"
#include "FileIO_Sailings.h"
#include <fstream>
#include <iostream>
#include <cstring>

// ---------------------------------------------------------------------------
// Helper: Encode license to fixed 10-char format (space-padded)
// ---------------------------------------------------------------------------
static void encodeLicense(const std::string &src, char dest[10])
{
    std::memset(dest, ' ', 10);
    std::size_t n = (src.size() < 10) ? src.size() : 10;
    std::memcpy(dest, src.data(), n);
}

// ---------------------------------------------------------------------------
// Helper: Decode license from fixed 10-char format (trim spaces)
// ---------------------------------------------------------------------------
static std::string decodeLicense(const char src[10])
{
    std::string s(src, 10);
    while (!s.empty() && s.back() == ' ') s.pop_back();
    return s;
}

// ---------------------------------------------------------------------------
// Reset sequential read (no persistent file pointer needed here)
// ---------------------------------------------------------------------------
void FileIO_Reservations::reset()
{
    // Intentionally left blank – handled by fresh stream creation per call.
}

// ---------------------------------------------------------------------------
// Retrieve next reservation sequentially
// ---------------------------------------------------------------------------
bool FileIO_Reservations::getNextReservation(std::string &licensePlate,
                                             SailingID &sailingID,
                                             bool &checkedIn)
{
    static std::ifstream file("reservations.dat", std::ios::binary);
    if (!file) return false;

    ReservationRec rec{};
    if (file.read(reinterpret_cast<char*>(&rec), sizeof(rec))) {
        licensePlate = decodeLicense(rec.licenseplate);
        sailingID = rec.sailingID;
        checkedIn = rec.checkedIn;
        return true;
    }
    return false;
}

// ---------------------------------------------------------------------------
// Append new reservation to file
// ---------------------------------------------------------------------------
bool FileIO_Reservations::writeReservation(const std::string &licensePlate,
                                           SailingID sailingID)
{
    std::ofstream file("reservations.dat", std::ios::binary | std::ios::app);
    if (!file) return false;

    ReservationRec rec{};
    encodeLicense(licensePlate, rec.licenseplate);
    rec.sailingID = sailingID;
    rec.checkedIn = false;

    file.write(reinterpret_cast<const char*>(&rec), sizeof(rec));
    return true;
}

// ---------------------------------------------------------------------------
// Mark reservation as checked-in
// ---------------------------------------------------------------------------
bool FileIO_Reservations::writeCheckin(const std::string &licensePlate,
                                       SailingID sailingID)
{
    std::fstream file("reservations.dat", std::ios::binary | std::ios::in | std::ios::out);
    if (!file) return false;

    ReservationRec rec{};
    while (file.read(reinterpret_cast<char*>(&rec), sizeof(rec))) {
        std::string currentLicense = decodeLicense(rec.licenseplate);
        if (currentLicense == licensePlate && rec.sailingID == sailingID) {
            rec.checkedIn = true;
            file.seekp(-static_cast<int>(sizeof(rec)), std::ios::cur);
            file.write(reinterpret_cast<const char*>(&rec), sizeof(rec));
            return true;
        }
    }
    return false;
}

// ---------------------------------------------------------------------------
// Delete specific reservation
// ---------------------------------------------------------------------------
bool FileIO_Reservations::deleteReservation(const std::string &licensePlate,
                                            SailingID sailingID)
{
    std::ifstream inFile("reservations.dat", std::ios::binary);
    if (!inFile) return false;

    std::ofstream outFile("temp.dat", std::ios::binary);
    if (!outFile) return false;

    ReservationRec rec{};
    bool found = false;

    while (inFile.read(reinterpret_cast<char*>(&rec), sizeof(rec))) {
        std::string currentLicense = decodeLicense(rec.licenseplate);
        if (currentLicense == licensePlate && rec.sailingID == sailingID) {
            found = true; // Skip this record
        } else {
            outFile.write(reinterpret_cast<const char*>(&rec), sizeof(rec));
        }
    }

    inFile.close();
    outFile.close();

    std::remove("reservations.dat");
    std::rename("temp.dat", "reservations.dat");

    return found;
}

// ---------------------------------------------------------------------------
// Delete all reservations for a given sailing (cascade delete)
// ---------------------------------------------------------------------------
bool FileIO_Reservations::deleteReservationsBySailing(SailingID sailingID)
{
    std::ifstream inFile("reservations.dat", std::ios::binary);
    std::ofstream outFile("temp.dat", std::ios::binary);
    if (!inFile || !outFile) return false;

    ReservationRec rec{};
    bool found = false;

    while (inFile.read(reinterpret_cast<char*>(&rec), sizeof(rec))) {
        if (rec.sailingID != sailingID) {
            outFile.write(reinterpret_cast<char*>(&rec), sizeof(rec));
        } else {
            found = true; // Skip (delete) this reservation
        }
    }

    inFile.close();
    outFile.close();

    std::remove("reservations.dat");
    std::rename("temp.dat", "reservations.dat");

    return found;
}

// ---------------------------------------------------------------------------
// Compute total space available (HCL + LCL) for a sailing
// ---------------------------------------------------------------------------
int FileIO_Reservations::spaceAvailable(SailingID sailingID)
{
    Sailingrec sailing;
    if (!FileIO_Sailings::findSailing(sailingID, sailing)) {
        return -1; // Sailing not found
    }

    std::ifstream file("reservations.dat", std::ios::binary);
    if (!file) return sailing.remainingHCL + sailing.remainingLCL;

    ReservationRec rec{};
    int usedHCL = 0, usedLCL = 0;

    while (file.read(reinterpret_cast<char*>(&rec), sizeof(rec))) {
        if (rec.sailingID == sailingID) {
            std::string license = decodeLicense(rec.licenseplate);

            FerrySys::VehicleRecord vehicle;
            if (FerrySys::FileIO_VehicleRecord::findVehicle(license, vehicle)) {
                if (vehicle.isSpecial())
                    usedHCL++;
                else
                    usedLCL++;
            }
        }
    }

    int availableHCL = sailing.remainingHCL - usedHCL;
    int availableLCL = sailing.remainingLCL - usedLCL;
    return (availableHCL + availableLCL);
}

// ---------------------------------------------------------------------------
// Count reservations for a given sailing
// ---------------------------------------------------------------------------
int FileIO_Reservations::countReservationsForSailing(SailingID sailingID)
{
    std::fstream file("reservations.dat", std::ios::binary | std::ios::in);
    if (!file) return 0;

    ReservationRec rec{};
    int count = 0;

    while (file.read(reinterpret_cast<char*>(&rec), sizeof(rec))) {
        if (rec.sailingID == sailingID) {
            count++;
        }
    }

    return count;
}
