//************************************************************
//************************************************************
//  Reservation.h
//************************************************************
//************************************************************
#ifndef RESERVATION_H
#define RESERVATION_H

#include "CommonTypes.h"
#include <string>

class Reservation
{
public:
//------------------------------------------------------------
// Books a reservation for a brand-new customer.
// Preconditions : licence not already stored.
// Postconditions: vehicle record + reservation saved.
    static bool newCustomerReservation(
        const std::string &licencePlate,  // IN  : vehicle licence
        const std::string &phoneNumber,   // IN  : customer phone
        bool               isSpecial,     // IN  : special-vehicle flag
        SailingID          sailingId      // IN  : sailing requested
    );

//------------------------------------------------------------
// Books a reservation for an existing customer.
// Preconditions : vehicle already on file.
// Postconditions: reservation saved.
    static bool returningCustomerReservation(
        const std::string &licencePlate,  // IN  : vehicle licence
        SailingID          sailingId      // IN  : sailing requested
    );

//------------------------------------------------------------
// Deletes a reservation.
// Preconditions : reservation exists.
// Postconditions: record removed and space counter updated.
    static bool deleteReservation(
        ReservationID      reservationId  // IN  : reservation to delete
    );

//------------------------------------------------------------
// Marks a vehicle as checked-in.
// Preconditions : matching reservation exists.
// Postconditions: check-in flag set.
    static bool checkinVehicle(
        const std::string &licencePlate,  // IN  : vehicle licence
        SailingID          sailingId      // IN  : sailing booked
    );

//------------------------------------------------------------
// True if vehicle already exists in system.
// Preconditions : none
// Postconditions: none
    static bool isVehicleExist(
        const std::string &licencePlate   // IN  : licence to search
    );

//------------------------------------------------------------
// True if sailingId exists and has capacity.
// Preconditions : none
// Postconditions: none
    static bool isSailingIDExist(
        SailingID          sailingId      // IN  : id to search
    );

//------------------------------------------------------------
// Decrements space available on a sailing (after successful reservation).
// Preconditions : sailingId valid and spaceAvailable>0.
// Postconditions: counter decremented.
    static void decreaseSpaceAvailable(
        SailingID          sailingId      // IN/OUT : sailing affected
    );

//------------------------------------------------------------
// Opens reservation file and prepares in-memory caches.
// Preconditions : called once at startup.
// Postconditions: ready for reservation ops.
    static void initialize();

//------------------------------------------------------------
// Flushes caches and closes reservation file.
// Preconditions : called once at shutdown.
// Postconditions: all updates persisted.
    static void shutdown();
};

#endif // RESERVATION_H
