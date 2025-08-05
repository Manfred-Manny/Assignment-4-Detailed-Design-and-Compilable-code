//************************************************************
//************************************************************
//  FileIO_Sailings.h
//  CMPT 276 – Assignment 4 (Fahad M)
//
//  PURPOSE:
//    Defines binary file operations for ferry sailings, stored
//    in `sailings.dat`. Each record contains:
//      • Fixed-length Sailing ID (primary key)
//      • Fixed-length Vessel name
//      • Remaining lane space for HCL and LCL vehicles
//
//************************************************************
//************************************************************

#ifndef FILEIO_SAILINGS_H
#define FILEIO_SAILINGS_H

#include <fstream>
#include <string>
#include "CommonTypes.h"

//------------------------------------------------------------
// Binary layout of a single sailing record in `sailings.dat`
//------------------------------------------------------------
struct Sailingrec {
    char id[16];                  // Fixed-length Sailing ID (e.g., "YYZ:23:12")
    char VesselName[25];          // Fixed-length Vessel name
    float remainingHCL;  // Remaining high-ceiling lane length
    float remainingLCL;  // Remaining low-ceiling lane length
};

class FileIO_Sailings
{
public:
    // Sequentially read next sailing record
    static bool getNextSailing(
        SailingID &sailingID,
        std::string &vesselName,
        float &remainingHCL,
        float &remainingLCL
    );

    // Append new sailing record
    static void writeSailing(
        SailingID sailingID,
        const std::string &vesselName,
        float remainingHCL,
        float remainingLCL
    );

    // Delete sailing by ID (cascades reservation deletion)
    static bool deleteSailing(
        SailingID sailingIDtoDelete
    );

    // Find sailing record by ID
    static bool findSailing(
        SailingID sailingID,
        Sailingrec &result
    );

    // Display formatted status of single sailing
    static bool sailingstatus(
        SailingID sailingID
    );

    // Returns all sailings
   static std::vector<Sailingrec> Sailingreport();


    // Check if sailing exists by ID
    static bool Sailingexist(
        SailingID sailingID
    );

    // Get remaining space for a sailing
    static bool getRemainingSpace(
        SailingID sailingID,
        float &remainingHCL,
        float &remainingLCL
    );

    // Update remaining space dynamically after reservation changes
    static bool updateSailingSpace(
        SailingID sailingID,
        float carLength,
        float carHeight,
        int amount
    );

};

#endif // FILEIO_SAILINGS_H
