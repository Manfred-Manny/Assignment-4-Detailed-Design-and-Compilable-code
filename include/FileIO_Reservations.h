// ---------------------------------------------------------------------------
// FileIO_Reservations.h
// CMPT 276 – Assignment 4 (Fahad Y)
//
// PURPOSE:
//    Provides low-level binary file operations for managing ferry reservations.
//    Each reservation links a vehicle (license plate) to a specific sailing ID
//    and optionally tracks check-in status.
//
// DESIGN NOTES:
//    • Records are stored as fixed-length binary blocks.
//    • Searching and deletions use linear scans.
//    • Higher-level validation and business logic reside outside this class.
// ---------------------------------------------------------------------------

#ifndef FILEIO_RESERVATIONS_H
#define FILEIO_RESERVATIONS_H

#include <string>
#include "CommonTypes.h"

// ---------------------------------------------------------------------------
// Binary reservation record structure
// ---------------------------------------------------------------------------
struct ReservationRec {
    char licenseplate[10];  // Fixed-length (10 chars), space-padded
    SailingID sailingID;    // Associated sailing ID
    bool checkedIn;         // Check-in status
};

class FileIO_Reservations
{
public:
    //------------------------------------------------------------
    // Reset sequential read pointer (not heavily used).
    static void reset();

    //------------------------------------------------------------
    // Retrieve next reservation sequentially.
    // Returns true if record read successfully, false at EOF.
    static bool getNextReservation(
        std::string &licensePlate,  // OUT: vehicle license
        SailingID   &sailingID,     // OUT: sailing ID
        bool        &checkedIn      // OUT: check-in status
    );

    //------------------------------------------------------------
    // Append new reservation (license + sailing).
    static bool writeReservation(
        const std::string &licensePlate,  // IN: vehicle license
        SailingID sailingID               // IN: sailing ID
    );

    //------------------------------------------------------------
    // Mark an existing reservation as checked-in.
    static bool writeCheckin(
        const std::string &licensePlate,  // IN: vehicle license
        SailingID sailingID               // IN: sailing ID
    );

    //------------------------------------------------------------
    // Delete a specific reservation.
    // Returns true if deleted, false if not found.
    static bool deleteReservation(
        const std::string &licensePlate,  // IN: vehicle license
        SailingID sailingID               // IN: sailing ID
    );

    //------------------------------------------------------------
    // Return total remaining space for a sailing (HCL + LCL).
    // Falls back to sailing capacity if no reservations exist.
    static int spaceAvailable(
        SailingID sailingID               // IN: sailing ID
    );

    //------------------------------------------------------------
    // Count number of reservations on a given sailing.
    static int countReservationsForSailing(
        SailingID sailingID               // IN: sailing ID
    );

    //------------------------------------------------------------
    // Delete all reservations for a specific sailing (cascade delete).
    static bool deleteReservationsBySailing(
        SailingID sailingID               // IN: sailing ID
    );
};

#endif // FILEIO_RESERVATIONS_H
