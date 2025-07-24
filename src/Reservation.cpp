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
// ---------------------------------------------------------------------------
static const int HIGH_CEILING_THRESHOLD = 200; // cm

// ---------------------------------------------------------------------------
// Helper: Determine if vehicle requires HCL lane
// ---------------------------------------------------------------------------
static bool isHighCeiling(const FerrySys::VehicleRecord &vehicle)
{
    return vehicle.height_cm > HIGH_CEILING_THRESHOLD;
}

// ---------------------------------------------------------------------------
// Helper: Adjust sailing space (positive = restore, negative = reserve)
// ---------------------------------------------------------------------------
static void adjustSailingSpace(SailingID sailingID, bool useHCL, int amount)
{
    FileIO_Sailings::updateSailingSpace(sailingID, useHCL, amount);
}

// ---------------------------------------------------------------------------
// Helper: Determine lane choice and check availability
// Returns true if booking is possible and sets `useHCL` accordingly
// ---------------------------------------------------------------------------
static bool canBookSailing(const FerrySys::VehicleRecord &vehicle,
                           SailingID sailingID,
                           bool &useHCL)
{
    // Validate sailing existence
    if (!FileIO_Sailings::Sailingexist(sailingID))
        return false;

    // Retrieve remaining space from sailings file
    unsigned int remainingHCL = 0, remainingLCL = 0;
    FileIO_Sailings::getRemainingSpace(sailingID, remainingHCL, remainingLCL);

    // Decide lane based on vehicle type
    if (isHighCeiling(vehicle)) {
        // Must use HCL
        if (vehicle.length_cm <= remainingHCL) {
            useHCL = true;
            return true;
        }
    } else {
        // Regular vehicles prefer LCL
        if (vehicle.length_cm <= remainingLCL) {
            useHCL = false; // Use LCL
            return true;
        }
        // If LCL full, fallback to HCL
        if (vehicle.length_cm <= remainingHCL) {
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

    // Write reservation
    if (!FileIO_Reservations::writeReservation(vehicle.license, sailingID))
        return false;

    // Deduct space dynamically
    adjustSailingSpace(sailingID, useHCL, -static_cast<int>(vehicle.length_cm));
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
    adjustSailingSpace(sailingID, useHCL, -static_cast<int>(vehicle.length_cm));
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

    // Restore space dynamically (assumes same lane logic as booking)
    bool useHCL = isHighCeiling(vehicle);
    adjustSailingSpace(sailingID, useHCL, static_cast<int>(vehicle.length_cm));
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
