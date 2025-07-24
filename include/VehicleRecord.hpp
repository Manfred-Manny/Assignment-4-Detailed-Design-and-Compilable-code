//************************************************************
//************************************************************
//  VehicleRecord.hpp
//  CMPT 276 – Assignment 4 (Manny)
//    Declares the VehicleRecord struct and related utilities for
//    fixed-length binary encoding/decoding of vehicle data.
//
//    • Defines vehicle record layout (license, phone, length, height)
//    • Provides encode/decode functions for on-disk binary format
//    • Includes helper for equality checking and “special” vehicle logic
//************************************************************
//************************************************************

#ifndef VEHICLE_RECORD_HPP
#define VEHICLE_RECORD_HPP
#pragma once

#include <cstdint>
#include <cstddef>
#include <array>
#include <string>
#include <string_view>

namespace FerrySys
{
    // -----------------------------------------------------------------------
    // Public constants for field sizes (shared across encode/decode & tests)
    // -----------------------------------------------------------------------
    constexpr std::size_t VEH_LIC_CHARS   = 10;   // License plate max chars
    constexpr std::size_t VEH_PHONE_CHARS = 14;   // Phone number max chars
    constexpr std::size_t VEH_REC_BYTES   = 32;   // Total record size in bytes

    // -----------------------------------------------------------------------
    // Struct: VehicleRecord
    // Description:
    //     Represents a single vehicle reservation record.
    //     Includes license plate, phone number, and vehicle dimensions.
    // -----------------------------------------------------------------------
    struct VehicleRecord
    {
        std::string license;     // Trimmed to <=10 chars when encoded
        std::string phone;       // Trimmed to <=14 chars when encoded
        std::int32_t length_cm = 0; // Vehicle length in centimeters
        std::int32_t height_cm = 0; // Vehicle height in centimeters

        // -------------------------------------------------------------------
        // Function: isSpecial
        // Description:
        //     Returns true if the vehicle is considered “special,” meaning
        //     either its height exceeds 200 cm or its length exceeds 700 cm.
        // -------------------------------------------------------------------
        bool isSpecial() const noexcept
        {
            return (height_cm > 200) || (length_cm > 700);
        }
    };

    // -----------------------------------------------------------------------
    // Type alias for raw encoded vehicle bytes (fixed 32-byte record)
    // -----------------------------------------------------------------------
    using VehicleRaw = std::array<unsigned char, VEH_REC_BYTES>;

    // Encode vehicle record to raw bytes
    void encodeVehicle(const VehicleRecord &in, VehicleRaw &out) noexcept;

    // Decode raw bytes to vehicle record
    void decodeVehicle(const VehicleRaw &in, VehicleRecord &out);

    // Compare two vehicle records for equality
    bool vehicleEqual(const VehicleRecord &a, const VehicleRecord &b) noexcept;
}

#endif // VEHICLE_RECORD_HPP
