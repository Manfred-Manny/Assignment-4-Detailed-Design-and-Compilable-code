//************************************************************
//************************************************************
//  FileIO_Sailings.cpp
//  CMPT 276 – Assignment 4 (Fahad M)
//
//  PURPOSE:
//    Implements binary file I/O for ferry sailings with fixed-size
//    fields to ensure predictable storage and retrieval. Supports
//    adding, searching, deleting, updating, and reporting sailings.
//************************************************************
//************************************************************

#include "FileIO_Sailings.h"
#include "FileIO_Reservations.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>

//------------------------------------------------------------
// Helper: Sanitize char[] to std::string (strip trailing \0)
//------------------------------------------------------------
static std::string sanitizeCharArray(const char* raw) {
    std::string s(raw);
    return s.substr(0, s.find('\0'));
}

//------------------------------------------------------------
// Sequentially retrieve next sailing record
//------------------------------------------------------------
bool FileIO_Sailings::getNextSailing(
    SailingID &sailingID,
    std::string &vesselName,
    float &remainingHCL,
    float &remainingLCL
) {
    std::fstream file("sailings.dat", std::ios::binary | std::ios::in);
    if (!file) return false;

    Sailingrec rec{};
    if (file.read(reinterpret_cast<char*>(&rec), sizeof(rec))) {
        sailingID = sanitizeCharArray(rec.id);
        vesselName = sanitizeCharArray(rec.VesselName);
        remainingHCL = rec.remainingHCL;
        remainingLCL = rec.remainingLCL;
        return true;
    }

    return false;
}

//------------------------------------------------------------
// Append new sailing record
//------------------------------------------------------------
void FileIO_Sailings::writeSailing(
    SailingID sailingID,
    const std::string &vesselName,
    float remainingHCL,
    float remainingLCL
) {
    std::fstream file("sailings.dat", std::ios::binary | std::ios::app);
    if (!file) {
        std::cerr << "Error: Unable to open sailings.dat for writing!\n";
        return;
    }

    Sailingrec rec{};
    std::memset(rec.id, '\0', sizeof(rec.id));
    std::strncpy(rec.id, sailingID.c_str(), sizeof(rec.id) - 1);

    std::memset(rec.VesselName, '\0', sizeof(rec.VesselName));
    std::strncpy(rec.VesselName, vesselName.c_str(), sizeof(rec.VesselName) - 1);

    rec.remainingHCL = remainingHCL;
    rec.remainingLCL = remainingLCL;

    file.write(reinterpret_cast<const char*>(&rec), sizeof(Sailingrec));
}

//------------------------------------------------------------
// Find sailing record by ID
//------------------------------------------------------------
bool FileIO_Sailings::findSailing(SailingID sailingID, Sailingrec &result) {
    std::fstream file("sailings.dat", std::ios::binary | std::ios::in);
    if (!file) return false;

    Sailingrec rec{};
    while (file.read(reinterpret_cast<char*>(&rec), sizeof(rec))) {
        if (sanitizeCharArray(rec.id) == sailingID) {
            result = rec;
            return true;
        }
    }
    return false;
}

//------------------------------------------------------------
// Delete sailing by ID (cascade reservations)
//------------------------------------------------------------
bool FileIO_Sailings::deleteSailing(SailingID sailingIDtoDelete) {
    std::ifstream file("sailings.dat", std::ios::binary);
    if (!file) {
        std::cerr << "Error: Unable to open sailings.dat for reading!\n";
        return false;
    }

    std::ofstream temp("temp.dat", std::ios::binary);
    if (!temp) {
        std::cerr << "Error: Unable to open temp.dat for writing!\n";
        return false;
    }

    Sailingrec rec{};
    bool found = false;

    while (file.read(reinterpret_cast<char*>(&rec), sizeof(rec))) {
        if (sanitizeCharArray(rec.id) == sailingIDtoDelete) {
            found = true;
        } else {
            temp.write(reinterpret_cast<char*>(&rec), sizeof(rec));
        }
    }

    file.close();
    temp.close();

    std::remove("sailings.dat");
    std::rename("temp.dat", "sailings.dat");

    return found;
}

//------------------------------------------------------------
// Check if sailing exists by ID
//------------------------------------------------------------
bool FileIO_Sailings::Sailingexist(SailingID sailingID) {
    std::fstream file("sailings.dat", std::ios::binary | std::ios::in);
    if (!file) return false;

    Sailingrec rec{};
    while (file.read(reinterpret_cast<char*>(&rec), sizeof(rec))) {
        if (sanitizeCharArray(rec.id) == sailingID) {
            return true;
        }
    }

    return false;
}

//------------------------------------------------------------
// Get remaining space for a sailing
//------------------------------------------------------------
bool FileIO_Sailings::getRemainingSpace(
    SailingID sailingID,
    float &remainingHCL,
    float &remainingLCL
) {
    std::ifstream file("sailings.dat", std::ios::binary);
    if (!file) return false;

    Sailingrec rec{};
    while (file.read(reinterpret_cast<char*>(&rec), sizeof(rec))) {
        if (sanitizeCharArray(rec.id) == sailingID) {
            remainingHCL = rec.remainingHCL;
            remainingLCL = rec.remainingLCL;
            return true;
        }
    }
    return false;
}

//------------------------------------------------------------
// Update sailing space dynamically (after reservation changes)
//------------------------------------------------------------
bool FileIO_Sailings::updateSailingSpace(SailingID sailingID, float carLength, float carHeight, int amount)
{
    std::fstream file("sailings.dat", std::ios::binary | std::ios::in | std::ios::out);
    if (!file) return false;

    Sailingrec rec{};
    while (file.read(reinterpret_cast<char*>(&rec), sizeof(rec))) {
        if (sanitizeCharArray(rec.id) == sailingID) {

            // Add buffer for parking space
            float spaceNeeded = carLength + 0.5f;

            // Determine if this should use HCL or LCL
            bool useHCL = false;
            if (spaceNeeded > 7 && carHeight > 2) {
                useHCL = true;
            } else if (rec.remainingLCL <= 0 && amount < 0) {
                // fallback if LCL is full when adding
                useHCL = true;
            }

            // Adjust remaining space (in meters)
            if (useHCL)
                rec.remainingHCL += (amount * spaceNeeded);
            else
                rec.remainingLCL += (amount * spaceNeeded);

            // Clamp to avoid negative space
            if (rec.remainingHCL < 0) rec.remainingHCL = 0;
            if (rec.remainingLCL < 0) rec.remainingLCL = 0;

            file.seekp(-static_cast<int>(sizeof(rec)), std::ios::cur);
            file.write(reinterpret_cast<const char*>(&rec), sizeof(rec));
            return true;
        }
    }

    return false;
}

//------------------------------------------------------------
// Return all sailings (for UI pagination)
//------------------------------------------------------------
std::vector<Sailingrec> FileIO_Sailings::Sailingreport()
{
    std::vector<Sailingrec> sailings;
    std::ifstream file("sailings.dat", std::ios::binary);
    if (!file) return sailings;

    Sailingrec rec{};
    while (file.read(reinterpret_cast<char*>(&rec), sizeof(rec))) {
        sailings.push_back(rec);
    }

    return sailings;
}

bool FileIO_Sailings::sailingstatus(SailingID sailingID) {
    std::fstream file("sailings.dat", std::ios::binary | std::ios::in);
    if (!file) {
        std::cerr << "Error opening sailings.dat!\n";
        return false;
    }

    Sailingrec rec{};
    while (file.read(reinterpret_cast<char*>(&rec), sizeof(rec))) {
        if (sanitizeCharArray(rec.id) == sailingID) {
            int reservationCount = FileIO_Reservations::countReservationsForSailing(sailingID);

            std::cout << "----------------------------------------------------------------------------------------------------------------\n";
            std::cout << std::left << std::setw(15) << "Sailing ID"
                      << std::setw(25) << "Reservations"
                      << std::setw(20) << "Remaining HCL"
                      << std::setw(20) << "Remaining LCL" << "\n";
            std::cout << "----------------------------------------------------------------------------------------------------------------\n";

            std::cout << std::fixed << std::setprecision(1)
                      << std::left << std::setw(15) << sanitizeCharArray(rec.id)
                      << std::setw(25) << reservationCount
                      << std::setw(20) << rec.remainingHCL
                      << std::setw(20) << rec.remainingLCL << "\n";
            std::cout << "----------------------------------------------------------------------------------------------------------------\n";

            return true;
        }
    }

    return false;
}