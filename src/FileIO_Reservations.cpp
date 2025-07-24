//************************************************************
//************************************************************
//  FileIO_Reservations.cpp
//  CMPT 276 – Assignment 4 (Fahad Y)
//
//  PURPOSE:
//    Implements binary I/O for ferry reservations.
//
//    • Encodes/decodes fixed-length license and sailing IDs.
//    • Supports creation, deletion, check-in, and queries.
//    • Ensures compatibility with FileIO_Sailings (16-char ID).
//************************************************************
//************************************************************

#include "FileIO_Reservations.h"
#include "FileIO_VehicleRecord.h"
#include "FileIO_Sailings.h"
#include <fstream>
#include <iostream>
#include <cstring>

// ============================================================
// Helper functions for fixed-length field encoding/decoding
// ============================================================

// Encode license plate (10 chars, space padded)
static void encodeLicense(const std::string &src, char dest[10]) {
    std::memset(dest, ' ', 10);
    std::size_t n = (src.size() < 10) ? src.size() : 10;
    std::memcpy(dest, src.data(), n);
}

// Decode license plate (trim spaces)
static std::string decodeLicense(const char src[10]) {
    std::string s(src, 10);
    while (!s.empty() && s.back() == ' ') s.pop_back();
    return s;
}

// Encode sailing ID (16 chars, space padded)
static void encodeID(const std::string &src, char dest[16]) {
    std::memset(dest, ' ', 16);
    std::size_t n = (src.size() < 16) ? src.size() : 16;
    std::memcpy(dest, src.data(), n);
}

// Decode sailing ID (trim spaces)
static std::string decodeID(const char src[16]) {
    std::string s(src, 16);
    while (!s.empty() && s.back() == ' ') s.pop_back();
    return s;
}

// ============================================================
// Reset sequential read
// ============================================================
void FileIO_Reservations::reset() {
    // No persistent pointer; each read opens fresh stream
}

// ============================================================
// Sequentially retrieve next reservation
// ============================================================
bool FileIO_Reservations::getNextReservation(
    std::string &licensePlate,
    SailingID   &sailingID,
    bool        &checkedIn)
{
    static std::ifstream file("reservations.dat", std::ios::binary);
    if (!file) return false;

    ReservationRec rec{};
    if (file.read(reinterpret_cast<char*>(&rec), sizeof(rec))) {
        licensePlate = decodeLicense(rec.licenseplate);
        sailingID = decodeID(rec.sailingID);
        checkedIn = rec.checkedIn;
        return true;
    }
    return false;
}

// ============================================================
// Append new reservation to file
// ============================================================
bool FileIO_Reservations::writeReservation(
    const std::string &licensePlate,
    SailingID sailingID)
{
    std::ofstream file("reservations.dat", std::ios::binary | std::ios::app);
    if (!file) return false;

    ReservationRec rec{};
    encodeLicense(licensePlate, rec.licenseplate);
    encodeID(sailingID, rec.sailingID);
    rec.checkedIn = false;

    file.write(reinterpret_cast<const char*>(&rec), sizeof(rec));
    return true;
}

// ============================================================
// Mark reservation as checked-in
// ============================================================
bool FileIO_Reservations::writeCheckin(
    const std::string &licensePlate,
    SailingID sailingID)
{
    std::fstream file("reservations.dat", std::ios::binary | std::ios::in | std::ios::out);
    if (!file) return false;

    ReservationRec rec{};
    while (file.read(reinterpret_cast<char*>(&rec), sizeof(rec))) {
        std::string currentLicense = decodeLicense(rec.licenseplate);
        std::string currentID = decodeID(rec.sailingID);

        if (currentLicense == licensePlate && currentID == sailingID) {
            rec.checkedIn = true;
            file.seekp(-static_cast<int>(sizeof(rec)), std::ios::cur);
            file.write(reinterpret_cast<const char*>(&rec), sizeof(rec));
            return true;
        }
    }
    return false;
}

// ============================================================
// Delete specific reservation
// ============================================================
bool FileIO_Reservations::deleteReservation(
    const std::string &licensePlate,
    SailingID sailingID)
{
    std::ifstream inFile("reservations.dat", std::ios::binary);
    if (!inFile) return false;

    std::ofstream outFile("temp.dat", std::ios::binary);
    if (!outFile) {
        inFile.close();
        return false;
    }

    ReservationRec rec{};
    bool found = false;

    while (inFile.read(reinterpret_cast<char*>(&rec), sizeof(rec))) {
        std::string currentLicense = decodeLicense(rec.licenseplate);
        std::string currentID = decodeID(rec.sailingID);

        if (currentLicense == licensePlate && currentID == sailingID) {
            found = true; // Skip writing this record (delete it)
        } else {
            outFile.write(reinterpret_cast<const char*>(&rec), sizeof(rec));
        }
    }

    inFile.close();
    outFile.close();

    if (!found) {
        std::remove("temp.dat");
        return false;
    }

    if (std::remove("reservations.dat") != 0 ||
        std::rename("temp.dat", "reservations.dat") != 0) {
        return false;
    }

    return true;
}

// ============================================================
// Delete all reservations for a given sailing (cascade delete)
// ============================================================
bool FileIO_Reservations::deleteReservationsBySailing(SailingID sailingID)
{
    std::ifstream inFile("reservations.dat", std::ios::binary);
    std::ofstream outFile("temp.dat", std::ios::binary);
    if (!inFile || !outFile) return false;

    ReservationRec rec{};
    bool found = false;

    while (inFile.read(reinterpret_cast<char*>(&rec), sizeof(rec))) {
        std::string currentID = decodeID(rec.sailingID);
        if (currentID != sailingID) {
            outFile.write(reinterpret_cast<const char*>(&rec), sizeof(rec));
        } else {
            found = true; // Delete this record
        }
    }

    inFile.close();
    outFile.close();

    std::remove("reservations.dat");
    std::rename("temp.dat", "reservations.dat");

    return found;
}

// ============================================================
// Count reservations for a given sailing
// ============================================================
int FileIO_Reservations::countReservationsForSailing(SailingID sailingID)
{
    std::fstream file("reservations.dat", std::ios::binary | std::ios::in);
    if (!file) return 0;

    ReservationRec rec{};
    int count = 0;

    while (file.read(reinterpret_cast<char*>(&rec), sizeof(rec))) {
        std::string currentID = decodeID(rec.sailingID);
        if (currentID == sailingID) {
            count++;
        }
    }

    return count;
}

// ============================================================
// Compute total space available (HCL + LCL) for a sailing
// ============================================================
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
        std::string currentID = decodeID(rec.sailingID);
        if (currentID == sailingID) {
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
