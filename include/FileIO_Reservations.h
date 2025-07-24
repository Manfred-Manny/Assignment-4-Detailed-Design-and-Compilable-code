//************************************************************
//************************************************************
//  FileIO_Reservations.h
//  CMPT 276 – Assignment 4 (Fahad Y)
//
//  PURPOSE:
//    Provides low-level binary I/O for ferry reservations.
//    Ensures consistent encoding/decoding of license plates
//    and Sailing IDs with built-in trimming.
//
//************************************************************
//************************************************************

#ifndef FILEIO_RESERVATIONS_H
#define FILEIO_RESERVATIONS_H

#include <string>
#include "CommonTypes.h"

//************************************************************
// Binary reservation record format
//************************************************************
struct ReservationRec {
    char licenseplate[10];   // Fixed 10 chars (space-padded)
    char sailingID[16];      // Fixed 16 chars (space-padded)
    bool checkedIn;          // Check-in status
};

class FileIO_Reservations
{
public:
    //------------------------------------------------------------
    // Reset sequential read pointer
    static void reset();

    //------------------------------------------------------------
    // Sequentially retrieve next reservation
    static bool getNextReservation(
        std::string &licensePlate,  // OUT: license
        SailingID   &sailingID,     // OUT: sailing ID
        bool        &checkedIn      // OUT: check-in flag
    );

    //------------------------------------------------------------
    // Append new reservation to reservations.dat
    static bool writeReservation(
        const std::string &licensePlate,  // IN: vehicle license
        SailingID sailingID               // IN: sailing ID
    );

    //------------------------------------------------------------
    // Mark existing reservation as checked-in
    static bool writeCheckin(
        const std::string &licensePlate,  // IN: vehicle license
        SailingID sailingID               // IN: sailing ID
    );

    //------------------------------------------------------------
    // Delete specific reservation (by license + sailingID)
    static bool deleteReservation(
        const std::string &licensePlate,  // IN: vehicle license
        SailingID sailingID               // IN: sailing ID
    );

    //------------------------------------------------------------
    // Count reservations for a sailing
    static int countReservationsForSailing(
        SailingID sailingID               // IN: sailing ID
    );

    //------------------------------------------------------------
    // Compute total available space (HCL + LCL) for a sailing
    static int spaceAvailable(
        SailingID sailingID               // IN: sailing ID
    );

    //------------------------------------------------------------
    // Delete all reservations for a given sailing (cascade delete)
    static bool deleteReservationsBySailing(
        SailingID sailingID               // IN: sailing ID
    );
};

#endif // FILEIO_RESERVATIONS_H
