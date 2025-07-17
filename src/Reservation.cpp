#include "Reservation.h"
#include <iostream>

// ---------------------------------------------------------------------------
// A4 STUB IMPLEMENTATION
// ---------------------------------------------------------------------------

bool Reservation::newCustomerReservation(const std::string &,
                                         const std::string &,
                                         bool,
                                         SailingID)
{
    std::cout << "[STUB] Reservation::newCustomerReservation()\n";
    return false;
}

bool Reservation::returningCustomerReservation(const std::string &,
                                               SailingID)
{
    std::cout << "[STUB] Reservation::returningCustomerReservation()\n";
    return false;
}

bool Reservation::deleteReservation(ReservationID)
{
    std::cout << "[STUB] Reservation::deleteReservation()\n";
    return false;
}

bool Reservation::checkinVehicle(const std::string &,
                                 SailingID)
{
    std::cout << "[STUB] Reservation::checkinVehicle()\n";
    return false;
}

bool Reservation::isVehicleExist(const std::string &)
{
    return false;
}

bool Reservation::isSailingIDExist(SailingID)
{
    return false;
}

void Reservation::decreaseSpaceAvailable(SailingID)
{
    // no-op stub
}

void Reservation::initialize()
{
    std::cout << "[STUB] Reservation::initialize()\n";
}

void Reservation::shutdown()
{
    std::cout << "[STUB] Reservation::shutdown()\n";
}
