//************************************************************
//************************************************************
//  FileIO_Reservations.h
//  Low-level binary I/O for reservation records
//************************************************************
//************************************************************
#ifndef FILEIO_RESERVATIONS_H
#define FILEIO_RESERVATIONS_H

#include <fstream>
#include <string>
#include "CommonTypes.h"

class FileIO_Reservations
{
public:
//------------------------------------------------------------
// Positions file pointer at first reservation record.
// Preconditions : none
// Postconditions: ready for sequential reads.
    static void reset();

//------------------------------------------------------------
// Reads next reservation record from file.
// Preconditions : reset() already called.
// Postconditions: returns true if fields populated, false on EOF.
    static bool getNextVehicle(
        std::string      &licencePlate,   // OUT : licence (max 10)
        std::string      &phoneNumber,    // OUT : phone (14 chars)
        bool             &isSpecial,      // OUT : high-clearance flag
        SailingID        &sailingId,      // OUT : sailing booked
        ReservationID    &reservationId   // OUT : unique reservation PK
    );

//------------------------------------------------------------
// Appends a brand-new reservation record.
// Preconditions : caller validated data.
// Postconditions: record written at end-of-file.
    static void writeReservation(
        const std::string &licencePlate,  // IN  : vehicle licence
        const std::string &phoneNumber,   // IN  : phone number E164
        bool               isSpecial,     // IN  : high-clearance flag
        SailingID          sailingId,     // IN  : sailing booked
        ReservationID      reservationId  // IN  : generated PK
    );

//------------------------------------------------------------
// Flags an existing reservation as checked-in.
// Preconditions : matching record exists.
// Postconditions: check-in byte toggled.
// Returns      : true on success, false if not found.
    static bool writeCheckin(
        ReservationID      reservationId  // IN  : reservation to mark
    );

//------------------------------------------------------------
// Deletes a reservation (logical delete).
// Preconditions : reservation is valid.
// Postconditions: record flagged as free.
// Returns      : true on success, false if not found.
    static bool deleteRes(
        ReservationID      reservationId  // IN  : reservation to delete
    );

//------------------------------------------------------------
// Counts remaining spaces on the given sailing.
// Preconditions : sailingId valid.
// Postconditions: none.
// Returns      : available count, −1 on error.
    static int spaceAvailable(
        SailingID          sailingId      // IN  : sailing of interest
    );
};

#endif // FILEIO_RESERVATIONS_H
