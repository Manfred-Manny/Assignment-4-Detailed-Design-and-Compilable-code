// ---------------------------------------------------------------------------
// FileIO_Reservations.h
// CMPT 276 – Assignment 4 (Fahad Y)
//
// Binary file operations for ferry reservations:
//   • Appending new reservations
//   • Searching and deleting by license + sailing ID
//   • Marking reservations as checked-in
//   • Counting reservations per sailing
//   • Calculating available space for a sailing
//
// Reservations are stored with fixed-length fields:
//   License = 10 chars, SailingID = 16 chars, CheckedIn = bool
// ---------------------------------------------------------------------------

#ifndef FILEIO_RESERVATIONS_H
#define FILEIO_RESERVATIONS_H

#include "VehicleRecord.hpp"     // for encodeField/decodeField helpers
#include "FileIO_VehicleRecord.h"
#include "FileIO_Sailings.h"
#include <string>

// ---------------------------------------------------------------------------
// Fixed-length reservation record layout
// ---------------------------------------------------------------------------
#pragma pack(push, 1)
struct ReservationRec
{
    char licenseplate[FerrySys::VEH_LIC_CHARS]; // 10 chars
    char sailingID[16];                         // 16 chars
    bool checkedIn;                             // 1 byte
};
#pragma pack(pop)

// Alias for sailing ID type
using SailingID = std::string;

class FileIO_Reservations
{
public:
    // Reset sequential read state (not persistent in this implementation)
    static void reset();

    // Sequentially retrieve next reservation
    static bool getNextReservation(std::string &licensePlate,
                                   SailingID &sailingID,
                                   bool &checkedIn);

    // Append new reservation to file
    static bool writeReservation(const std::string &licensePlate,
                                 SailingID sailingID);

    // Mark reservation as checked-in
    static bool writeCheckin(const std::string &licensePlate,
                             SailingID sailingID);

    // Delete specific reservation
    static bool deleteReservation(const std::string &licensePlate,
                                  SailingID sailingID);

    // Count reservations for a specific sailing
    static int countReservationsForSailing(SailingID sailingID);

    // Check if a reservation already exists for a given license and sailing
    static bool reservationExists(const std::string &licensePlate,
                              SailingID sailingID);


    // Compute total available space (HCL + LCL) for a sailing
    static int spaceAvailable(SailingID sailingID);
};

#endif // FILEIO_RESERVATIONS_H
