//************************************************************
//************************************************************
//  Reservation.h
//  CMPT 276 – Assignment 4(Fahad Y)
//    Declares the Reservation class, which implements the core
//    business logic for managing vehicle reservations on sailings.
//    This includes creating reservations for new or returning
//    customers, deleting reservations, and checking vehicles in.

//    • Validate vehicle and sailing IDs before reservation actions.
//    • Differentiate new vs returning customer workflows.
//    • Manage reservation lifecycle (create, delete, check-in).
//    • Provide utilities for checking existence of vehicles or sailings.
//************************************************************
//************************************************************

#ifndef RESERVATION_H
#define RESERVATION_H

#include <string>
#include "CommonTypes.h"
#include "VehicleRecord.hpp"  // for VehicleRecord type

class Reservation
{
public:
    // New customer reservation
    static bool newCustomerReservation(
        const FerrySys::VehicleRecord &vehicle,  //IN:vehicle record
        SailingID sailingID                     //IN:SailingID
    );

    // Returning customer reservation
    static bool returningCustomerReservation(
        const std::string &licensePlate,          //IN:LicensePlate
        SailingID sailingID                       //IN:sailingID
    );

    // Delete reservation
    static bool deleteReservation(
        const std::string &licensePlate,         //IN:LicensePlate
        SailingID sailingID                      //IN:sailingID
    );

    // Check-in vehicle
    static bool checkinVehicle(
        const std::string &licensePlate,         //IN:LicensePlate
        SailingID sailingID                      //IN:sailingID
    );

    // Utilities
    static bool isVehicleExist(
        const std::string &licensePlate         //IN:LicensePlate
    );

    static bool isSailingIDExist(
        SailingID sailingID                      //IN:sailingID
    );

    static void initialize();
    static void shutdown();
};

#endif // RESERVATION_H
