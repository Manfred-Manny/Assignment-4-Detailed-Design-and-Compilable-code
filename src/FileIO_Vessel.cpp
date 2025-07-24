//************************************************************
//************************************************************
//  FileIO_Vessel.cpp
//  CMPT 276 – Assignment 4 (Fahad Y)
//
//  PURPOSE:
//    Implements binary file I/O operations for managing ferry vessels.
//    Each vessel record contains:
//      - Vessel name (unique identifier)
//      - Lane capacities for high-ceiling (HCL) and low-ceiling (LCL) vehicles
//
//    Supported operations:
//      • Sequential scan for listing vessels
//      • Adding new vessel records
//      • Deleting vessels (with cascading deletion of related sailings)
//      • Lookup by vessel name
//
//************************************************************
//************************************************************

#include "FileIO_Vessel.h"
#include "FileIO_Sailings.h"
#include <cstring>
#include <iostream>

//------------------------------------------------------------
// Reset file pointer for sequential scan of `vessels.dat`
//
// Preconditions : File must exist; otherwise no action is taken.
// Postconditions: File pointer is positioned at start (future reads).
//------------------------------------------------------------
void FileIO_Vessel::reset()
{
    std::fstream file("vessels.dat", std::ios::binary | std::ios::in);
    if (!file) {
        std::cout << "vessels.dat not found.\n";
        return;
    }
    // File automatically closes when going out of scope.
}

//------------------------------------------------------------
// Sequentially read next vessel record (from start)
//
// Preconditions : `reset()` should be called before sequential iteration.
// Postconditions: Fills output variables if successful; returns false on EOF.
//------------------------------------------------------------
bool FileIO_Vessel::getNextVessel(
    std::string &vesselName,
    unsigned int &laneHCL,
    unsigned int &laneLCL
) {
    std::ifstream file("vessels.dat", std::ios::binary);
    if (!file)
        return false;

    Vesselrec rec{};
    if (file.read(reinterpret_cast<char*>(&rec), sizeof(rec))) {
        vesselName = rec.vesselName;
        laneHCL = rec.laneHCL;
        laneLCL = rec.laneLCL;
        return true;
    }
    return false;
}

//------------------------------------------------------------
// Append a new vessel record to `vessels.dat`
//
// Preconditions : `vesselName` must be unique (checked by caller).
// Postconditions: Vessel record is appended to end of file.
//------------------------------------------------------------
void FileIO_Vessel::writeVessel(
    const std::string &vesselName,
    unsigned int laneHCL,
    unsigned int laneLCL
) {
    std::fstream file("vessels.dat", std::ios::binary | std::ios::app);
    if (!file) {
        std::cout << "Error: Unable to open vessels.dat for writing!\n";
        return;
    }

    Vesselrec rec{};
    std::strncpy(rec.vesselName, vesselName.c_str(), sizeof(rec.vesselName) - 1);
    rec.vesselName[sizeof(rec.vesselName) - 1] = '\0';
    rec.laneHCL = laneHCL;
    rec.laneLCL = laneLCL;

    file.write(reinterpret_cast<const char*>(&rec), sizeof(Vesselrec));
}

//------------------------------------------------------------
// Delete a vessel record (and cascade delete sailings/reservations)
//
// Preconditions : Vessel must exist in `vessels.dat`.
// Postconditions: Vessel is removed from file. Any sailings and reservations
//                 tied to this vessel are also deleted via FileIO_Sailings.
//------------------------------------------------------------
bool FileIO_Vessel::deleteVessel(const std::string &vesselName)
{
    std::ifstream inFile("vessels.dat", std::ios::binary);
    std::ofstream outFile("temp.dat", std::ios::binary);

    if (!inFile || !outFile)
        return false;

    Vesselrec rec{};
    bool found = false;

    while (inFile.read(reinterpret_cast<char*>(&rec), sizeof(rec))) {
        std::string nameFromFile(rec.vesselName);
        nameFromFile = nameFromFile.substr(0, nameFromFile.find('\0'));

        if (nameFromFile == vesselName) {
            found = true; // Skip this record (delete it)
            continue;
        }

        outFile.write(reinterpret_cast<char*>(&rec), sizeof(rec));
    }

    inFile.close();
    outFile.close();

    std::remove("vessels.dat");
    std::rename("temp.dat", "vessels.dat");
    return found;
}

//------------------------------------------------------------
// Retrieve next vessel using an existing file stream
//
// Preconditions : `file` must be an open valid stream positioned at a record.
// Postconditions: Fills vessel fields; returns false on EOF.
//------------------------------------------------------------
bool FileIO_Vessel::getNextVessel(
    std::fstream &file,
    std::string &vesselName,
    unsigned int &laneHCL,
    unsigned int &laneLCL
) {
    if (!file.is_open())
        return false;

    Vesselrec rec{};
    if (file.read(reinterpret_cast<char*>(&rec), sizeof(rec))) {
        vesselName = rec.vesselName;
        laneHCL = rec.laneHCL;
        laneLCL = rec.laneLCL;
        return true;
    }
    return false;
}

//------------------------------------------------------------
// Lookup a vessel by name (linear search)
//
// Preconditions : `vessels.dat` must exist.
// Postconditions: Returns true and fills lane values if found; false otherwise.
//------------------------------------------------------------
bool FileIO_Vessel::getVesselByName(
    const std::string &vesselName,
    unsigned int &laneHCL,
    unsigned int &laneLCL
) {
    std::ifstream file("vessels.dat", std::ios::binary);
    if (!file)
        return false;

    Vesselrec rec{};
    while (file.read(reinterpret_cast<char*>(&rec), sizeof(rec))) {
        std::string name(rec.vesselName);
        name = name.substr(0, name.find('\0'));

        if (name == vesselName) {
            laneHCL = rec.laneHCL;
            laneLCL = rec.laneLCL;
            return true;
        }
    }
    return false;
}
