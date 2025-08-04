//************************************************************
//************************************************************
//  Reservation.cpp
//  CMPT 276 – Assignment 4 (Fahad M)
//    Implements the Reservation class logic, handling the core
//    workflows for creating, deleting, and managing vehicle
//    reservations on sailings.
//
//    • Handles new and returning customer reservations
//    • Validates sailing and vehicle data before booking
//    • Deducts/restores space dynamically from sailings
//    • Supports check-in and existence checks for vehicles/sailings
//************************************************************
//************************************************************

#include "Reservation.h"
#include "FileIO_VehicleRecord.h"
#include "FileIO_Sailings.h"
#include "FileIO_Reservations.h"

// ---------------------------------------------------------------------------
// Threshold to determine high-ceiling vehicles (HCL lane requirement)
// Now in METERS (was 200 cm previously)
// ---------------------------------------------------------------------------
static const double HIGH_CEILING_THRESHOLD = 2.0; // meters

// ---------------------------------------------------------------------------
// Helper: Determine if vehicle requires HCL lane (now in meters)
// ---------------------------------------------------------------------------
static bool isHighCeiling(const FerrySys::VehicleRecord &vehicle)
{
    return vehicle.height_m > HIGH_CEILING_THRESHOLD;
}

// ---------------------------------------------------------------------------
// Helper: Adjust sailing space (positive = restore, negative = reserve)
// Passes meters directly to updateSailingSpace
// ---------------------------------------------------------------------------
static void adjustSailingSpace(SailingID sailingID,
                               const FerrySys::VehicleRecord &vehicle,
                               int amount)
{
    FileIO_Sailings::updateSailingSpace(
        sailingID,
        static_cast<int>(vehicle.length_m), // meters now
        static_cast<int>(vehicle.height_m), // meters now
        amount
    );
}

// ---------------------------------------------------------------------------
// Helper: Determine lane choice and check availability
// Returns true if booking is possible and sets `useHCL` accordingly
// (This still checks space manually; fallback handled by updateSailingSpace)
// ---------------------------------------------------------------------------
static bool canBookSailing(const FerrySys::VehicleRecord &vehicle,
                           SailingID sailingID,
                           bool &useHCL)
{
    // Validate sailing existence
    if (!FileIO_Sailings::Sailingexist(sailingID))
        return false;

    // Retrieve remaining space from sailings file (space still treated as units, not meters)
    unsigned int remainingHCL = 0, remainingLCL = 0;
    FileIO_Sailings::getRemainingSpace(sailingID, remainingHCL, remainingLCL);

    // Decide lane based on vehicle type (height in meters)
    if (isHighCeiling(vehicle)) {
        // Must use HCL
        if (vehicle.length_m <= remainingHCL) {
            useHCL = true;
            return true;
        }
    } else {
        // Regular vehicles prefer LCL
        if (vehicle.length_m <= remainingLCL) {
            useHCL = false;
            return true;
        }
        // If LCL full, fallback to HCL
        if (vehicle.length_m <= remainingHCL) {
            useHCL = true;
            return true;
        }
    }

    return false; // No suitable space available
}

// ---------------------------------------------------------------------------
// New Customer Reservation
// ---------------------------------------------------------------------------
bool Reservation::newCustomerReservation(const FerrySys::VehicleRecord &vehicle,
                                         SailingID sailingID)
{
    bool useHCL = false;

    // Check sailing existence & space
    if (!canBookSailing(vehicle, sailingID, useHCL))
        return false;

    // Save vehicle to vehicles.dat if not already saved
    if (!FerrySys::FileIO_VehicleRecord::vehicleExists(vehicle.license))
    {
        if (!FerrySys::FileIO_VehicleRecord::writeVehicle(vehicle))
        {
            return false;
        }
    }

    // Write reservation
    if (!FileIO_Reservations::writeReservation(vehicle.license, sailingID))
        return false;

    // Deduct space dynamically
    adjustSailingSpace(sailingID, vehicle, -static_cast<int>(vehicle.length_m));
    return true;
}

// ---------------------------------------------------------------------------
// Returning Customer Reservation
// ---------------------------------------------------------------------------
bool Reservation::returningCustomerReservation(const std::string &licensePlate,
                                               SailingID sailingID)
{
    FerrySys::VehicleRecord vehicle;

    // Ensure vehicle exists
    if (!FerrySys::FileIO_VehicleRecord::findVehicle(licensePlate, vehicle))
        return false;

    bool useHCL = false;

    // Check sailing existence & space
    if (!canBookSailing(vehicle, sailingID, useHCL))
        return false;

    // Write reservation
    if (!FileIO_Reservations::writeReservation(licensePlate, sailingID))
        return false;

    // Deduct space dynamically
    adjustSailingSpace(sailingID, vehicle, -static_cast<int>(vehicle.length_m));
    return true;
}

// ---------------------------------------------------------------------------
// Delete Reservation
// ---------------------------------------------------------------------------
bool Reservation::deleteReservation(const std::string &licensePlate,
                                    SailingID sailingID)
{
    FerrySys::VehicleRecord vehicle;
    if (!FerrySys::FileIO_VehicleRecord::findVehicle(licensePlate, vehicle))
        return false;

    // Delete reservation record
    if (!FileIO_Reservations::deleteReservation(licensePlate, sailingID))
        return false;

    // Restore space dynamically
    adjustSailingSpace(sailingID, vehicle, static_cast<int>(vehicle.length_m));
    return true;
}

// ---------------------------------------------------------------------------
// Check-in Vehicle
// ---------------------------------------------------------------------------
bool Reservation::checkinVehicle(const std::string &licensePlate,
                                 SailingID sailingID)
{
    return FileIO_Reservations::writeCheckin(licensePlate, sailingID);
}

// ---------------------------------------------------------------------------
// Utility: Check if vehicle exists
// ---------------------------------------------------------------------------
bool Reservation::isVehicleExist(const std::string &licensePlate)
{
    FerrySys::VehicleRecord vehicle;
    return FerrySys::FileIO_VehicleRecord::findVehicle(licensePlate, vehicle);
}

// ---------------------------------------------------------------------------
// Utility: Check if sailing exists
// ---------------------------------------------------------------------------
bool Reservation::isSailingIDExist(SailingID sailingID)
{
    return FileIO_Sailings::Sailingexist(sailingID);
}

// ---------------------------------------------------------------------------
// Lifecycle (Initialize/Shutdown)
// ---------------------------------------------------------------------------
void Reservation::initialize() {}
void Reservation::shutdown() {}
