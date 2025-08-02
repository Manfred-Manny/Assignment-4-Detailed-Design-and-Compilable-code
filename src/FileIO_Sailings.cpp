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
    unsigned short &remainingHCL,
    unsigned short &remainingLCL
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
    unsigned short remainingHCL,
    unsigned short remainingLCL
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
// Print formatted report of all sailings
//------------------------------------------------------------
void FileIO_Sailings::Sailingreport() {
    std::fstream file("sailings.dat", std::ios::binary | std::ios::in);
    if (!file) {
        std::cerr << "No sailings available (sailings.dat not found).\n";
        return;
    }

    Sailingrec rec{};
    std::cout << "----------------------------------------------------------------------------------------------------------------\n";
    std::cout << std::left << std::setw(15) << "Sailing ID"
              << std::setw(25) << "Vessel Name"
              << std::setw(20) << "Remaining HCL"
              << std::setw(20) << "Remaining LCL" << "\n";
    std::cout << "----------------------------------------------------------------------------------------------------------------\n";

    while (file.read(reinterpret_cast<char*>(&rec), sizeof(rec))) {
        std::cout << std::left << std::setw(15) << sanitizeCharArray(rec.id)
                  << std::setw(25) << sanitizeCharArray(rec.VesselName)
                  << std::setw(20) << rec.remainingHCL
                  << std::setw(20) << rec.remainingLCL << "\n";
    }

    std::cout << "----------------------------------------------------------------------------------------------------------------\n";
}

//------------------------------------------------------------
// Display formatted status of single sailing
//------------------------------------------------------------
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

            std::cout << std::left << std::setw(15) << sanitizeCharArray(rec.id)
                      << std::setw(25) << reservationCount
                      << std::setw(20) << rec.remainingHCL
                      << std::setw(20) << rec.remainingLCL << "\n";
            std::cout << "----------------------------------------------------------------------------------------------------------------\n";
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
    unsigned int &remainingHCL,
    unsigned int &remainingLCL
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
bool FileIO_Sailings::updateSailingSpace(
    SailingID sailingID,
    bool      isHighCeiling,
    int       amount                // len_cm (±)
) {
    std::fstream file("sailings.dat",
                      std::ios::binary | std::ios::in | std::ios::out);
    if (!file) return false;

    Sailingrec rec{};
    while (file.read(reinterpret_cast<char*>(&rec), sizeof(rec))) {
        if (sanitizeCharArray(rec.id) == sailingID) {

            auto adjust = [&](unsigned short &field) {
                int tmp = static_cast<int>(field) + amount; // do signed math
                if (tmp < 0)      tmp = 0;       // clamp lower
                if (tmp > 65535)  tmp = 65535;   // clamp upper (ushort max)
                field = static_cast<unsigned short>(tmp);
            };

            if (isHighCeiling)
                adjust(rec.remainingHCL);
            else
                adjust(rec.remainingLCL);

            // write back current record
            file.seekp(-static_cast<std::streamoff>(sizeof(rec)), std::ios::cur);
            file.write(reinterpret_cast<const char*>(&rec), sizeof(rec));
            return true;
        }
    }
    return false; // sailingID not found
}