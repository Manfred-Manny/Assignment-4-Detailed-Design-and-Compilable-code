#include "FileIO_Reservations.h"
#include <iostream>

// ---------------------------------------------------------------------------
// A4 STUB IMPLEMENTATION
// NOTE: Real binary I/O NOT implemented yet.
// Manny's BinaryFileOps.* + VehicleRecord.* provide the working demo code.
// ---------------------------------------------------------------------------

void FileIO_Reservations::reset()
{
    std::cout << "[STUB] FileIO_Reservations::reset()\n";
}

bool FileIO_Reservations::getNextVehicle(std::string &licencePlate,
                                         std::string &phoneNumber,
                                         bool        &isSpecial,
                                         SailingID   &sailingId,
                                         ReservationID &reservationId)
{
    std::cout << "[STUB] FileIO_Reservations::getNextVehicle()\n";
    licencePlate.clear();
    phoneNumber.clear();
    isSpecial = false;
    sailingId = 0;
    reservationId = 0;
    return false; // EOF in stub
}

void FileIO_Reservations::writeReservation(const std::string &,
                                           const std::string &,
                                           bool,
                                           SailingID,
                                           ReservationID)
{
    std::cout << "[STUB] FileIO_Reservations::writeReservation()\n";
}

bool FileIO_Reservations::writeCheckin(ReservationID)
{
    std::cout << "[STUB] FileIO_Reservations::writeCheckin()\n";
    return false;
}

bool FileIO_Reservations::deleteRes(ReservationID)
{
    std::cout << "[STUB] FileIO_Reservations::deleteRes()\n";
    return false;
}

int FileIO_Reservations::spaceAvailable(SailingID)
{
    std::cout << "[STUB] FileIO_Reservations::spaceAvailable()\n";
    return -1;
}
