//************************************************************
//************************************************************
//  FileIO_Sailings.cpp
//  CMPT 276 – Assignment 4 (Fahad M)
//
//  PURPOSE:
//    Implements binary file I/O operations for managing ferry sailings.
//    Supports adding, searching, deleting sailings, dynamically updating
//    lane capacities, and cascading deletion of related reservations.
//
//  FILE LAYOUT (sailings.dat):
//    • Sailing ID (string, primary key)
//    • Vessel name (associated vessel)
//    • Remaining lane space for High-Ceiling (HCL) and Low-Ceiling (LCL)
//
//************************************************************
//************************************************************

#include "FileIO_Sailings.h"
#include "FileIO_Reservations.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>

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
        sailingID = rec.id;
        vesselName = rec.VesselName;
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
    rec.id = sailingID;
    std::strncpy(rec.VesselName, vesselName.c_str(), sizeof(rec.VesselName) - 1);
    rec.VesselName[sizeof(rec.VesselName) - 1] = '\0';
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
        if (rec.id == sailingID) {
            result = rec;
            return true;
        }
    }
    return false;
}

//------------------------------------------------------------
// Delete sailing by ID (also deletes reservations for it)
//------------------------------------------------------------
bool FileIO_Sailings::deleteSailing(SailingID sailingIDtoDelete) {
    std::fstream file("sailings.dat", std::ios::binary | std::ios::in);
    if (!file) {
        std::cerr << "Error: Unable to open sailings.dat for reading!\n";
        return false;
    }

    std::fstream temp("temp.dat", std::ios::binary | std::ios::out);
    if (!temp) {
        std::cerr << "Error: Unable to open temp.dat for writing!\n";
        return false;
    }

    Sailingrec rec{};
    bool found = false;

    while (file.read(reinterpret_cast<char*>(&rec), sizeof(rec))) {
        if (rec.id == sailingIDtoDelete) {
            found = true; // Skip this sailing
            FileIO_Reservations::deleteReservationsBySailing(sailingIDtoDelete);
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
// Delete all sailings for a vessel (and related reservations)
//------------------------------------------------------------
bool FileIO_Sailings::deleteSailingsByVessel(const std::string &vesselName) {
    std::ifstream inFile("sailings.dat", std::ios::binary);
    std::ofstream outFile("temp.dat", std::ios::binary);
    if (!inFile || !outFile) return false;

    Sailingrec rec{};
    bool found = false;

    while (inFile.read(reinterpret_cast<char*>(&rec), sizeof(rec))) {
        std::string fileVesselName(rec.VesselName);
        fileVesselName = fileVesselName.substr(0, fileVesselName.find('\0'));

        if (fileVesselName == vesselName) {
            FileIO_Reservations::countReservationsForSailing(rec.id);
            found = true; // This sailing will be deleted
        } else {
            outFile.write(reinterpret_cast<char*>(&rec), sizeof(rec));
        }
    }

    inFile.close();
    outFile.close();

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
        if (rec.id == sailingID) {
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
        std::cout << std::left << std::setw(15) << rec.id
                  << std::setw(25) << rec.VesselName
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
        if (rec.id == sailingID) {
            int reservationCount = FileIO_Reservations::countReservationsForSailing(sailingID);

            std::cout << "----------------------------------------------------------------------------------------------------------------\n";
            std::cout << std::left << std::setw(15) << "Sailing ID"
                      << std::setw(25) << "Reservations"
                      << std::setw(20) << "Remaining HCL"
                      << std::setw(20) << "Remaining LCL" << "\n";
            std::cout << "----------------------------------------------------------------------------------------------------------------\n";

            std::cout << std::left << std::setw(15) << rec.id
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
        std::string idFromFile(rec.id);
        idFromFile = idFromFile.substr(0, idFromFile.find('\0'));

        if (idFromFile == sailingID) {
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
    bool isHighCeiling,
    int amount
) {
    std::fstream file("sailings.dat", std::ios::binary | std::ios::in | std::ios::out);
    if (!file) return false;

    Sailingrec rec{};
    while (file.read(reinterpret_cast<char*>(&rec), sizeof(rec))) {
        std::string idFromFile(rec.id);
        idFromFile = idFromFile.substr(0, idFromFile.find('\0'));

        if (idFromFile == sailingID) {
            if (isHighCeiling)
                rec.remainingHCL += amount;
            else
                rec.remainingLCL += amount;

            // Prevent negatives
            if (rec.remainingHCL < 0) rec.remainingHCL = 0;
            if (rec.remainingLCL < 0) rec.remainingLCL = 0;

            file.seekp(-static_cast<int>(sizeof(rec)), std::ios::cur);
            file.write(reinterpret_cast<const char*>(&rec), sizeof(rec));
            return true;
        }
    }
    return false;
}
