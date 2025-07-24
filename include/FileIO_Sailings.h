//************************************************************
//************************************************************
//  FileIO_Sailings.h
//  CMPT 276 – Assignment 4 (Fahad M)
//
//  PURPOSE:
//    This header defines binary file operations for ferry sailings,
//    stored in `sailings.dat`. Each record contains:
//
//      • Sailing ID (string, serves as primary key)
//      • Vessel name associated with sailing
//      • Remaining lane space for high-ceiling (HCL) and low-ceiling (LCL) vehicles
//
//  FUNCTIONALITY:
//    • Add new sailing records (append mode)
//    • Sequential or targeted search by sailing ID
//    • Delete sailings by ID or all sailings for a vessel
//    • Update remaining lane capacities dynamically (reservations)
//    • Generate formatted sailing reports or specific sailing status
//
//  NOTES:
//    - File structure is fixed-length binary for simplicity
//    - No sorting or indexing; operations are linear scans
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
    SailingID id;                  // Unique sailing identifier
    char VesselName[25];           // Associated vessel (null-terminated)
    unsigned short remainingHCL;   // Remaining high-ceiling lane length (m)
    unsigned short remainingLCL;   // Remaining low-ceiling lane length (m)
};

class FileIO_Sailings
{
public:
    //------------------------------------------------------------
    // Sequentially read next sailing record.
    // Preconditions : File must exist and contain records.
    // Postconditions: Returns true and populates fields if successful,
    //                 false on EOF or error.
    static bool getNextSailing(
        SailingID        &sailingID,      // OUT: Sailing primary key
        std::string      &vesselName,     // OUT: Vessel name
        unsigned short   &remainingHCL,   // OUT: HCL lane space
        unsigned short   &remainingLCL    // OUT: LCL lane space
    );

    //------------------------------------------------------------
    // Append a new sailing record to `sailings.dat`.
    // Preconditions : Data validated (unique ID, valid vessel).
    // Postconditions: Record is added to file.
    static void writeSailing(
        SailingID          sailingID,     // IN: Unique ID
        const std::string &vesselName,    // IN: Vessel name
        unsigned short     remainingHCL,  // IN: HCL lane space
        unsigned short     remainingLCL   // IN: LCL lane space
    );

    //------------------------------------------------------------
    // Delete a sailing record by its ID.
    // Preconditions : File must exist.
    // Postconditions: Returns true if found and deleted; false otherwise.
    static bool deleteSailing(
        SailingID sailingID                // IN: ID of sailing to delete
    );

    //------------------------------------------------------------
    // Find a sailing record by ID and return details.
    // Preconditions : File must exist.
    // Postconditions: Returns true if found, populates `result`.
    static bool findSailing(
        SailingID  sailingID,              // IN: ID to search
        Sailingrec &result                 // OUT: Sailing record
    );

    //------------------------------------------------------------
    // Display formatted status of a single sailing.
    // Preconditions : File must exist.
    // Postconditions: Prints status and returns true if found.
    static bool sailingstatus(
        SailingID sailingID                // IN: ID to query
    );

    //------------------------------------------------------------
    // Print a formatted report of all sailings.
    // Preconditions : None (prints empty table if file missing/empty).
    // Postconditions: Outputs sailing details to console.
    static void Sailingreport();

    //------------------------------------------------------------
    // Check whether a sailing exists by ID.
    // Preconditions : File must exist.
    // Postconditions: Returns true if found, false otherwise.
    static bool Sailingexist(
        SailingID sailingID                // IN: ID to check
    );

    //------------------------------------------------------------
    // Retrieve remaining lane space for a sailing.
    // Preconditions : Sailing must exist.
    // Postconditions: Populates remainingHCL and remainingLCL.
    static bool getRemainingSpace(
        SailingID sailingID,               // IN: ID to query
        unsigned int &remainingHCL,        // OUT: HCL lane space
        unsigned int &remainingLCL         // OUT: LCL lane space
    );

    //------------------------------------------------------------
    // Update remaining lane space dynamically after reservation changes.
    // Preconditions : Sailing must exist; amount can be positive (restore)
    //                 or negative (deduct).
    // Postconditions: Updates lane capacities; prevents negatives.
    static bool updateSailingSpace(
        SailingID sailingID,               // IN: ID to update
        bool isHighCeiling,                // IN: Target lane type (HCL/LCL)
        int amount                         // IN: Adjustment (±length)
    );

    //------------------------------------------------------------
    // Delete all sailings for a given vessel.
    // Also removes reservations tied to these sailings.
    // Preconditions : File must exist.
    // Postconditions: Returns true if any sailings were deleted.
    static bool deleteSailingsByVessel(
        const std::string &vesselName      // IN: Vessel whose sailings to delete
    );
};

#endif // FILEIO_SAILINGS_H
