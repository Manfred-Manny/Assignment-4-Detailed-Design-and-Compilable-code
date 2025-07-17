#ifndef VEHICLE_RECORD_HPP
#define VEHICLE_RECORD_HPP
#pragma once
// ---------------------------------------------------------------------------
// VehicleRecord.hpp
// CMPT 276 – Assignment 4 (Manny T4/T5)
// 
// Fixed-length *on-disk* binary vehicle record format used for low-level
// file I/O testing (UnitTest #1) and as a reference layout for later
// Vehicle / Reservation storage modules.
//
// Layout (total = 32 bytes, no padding):
//   [0..9]   : license plate  (10 chars, left-justified, space-padded)
//   [10..23] : phone number   (14 chars, left-justified, space-padded)
//   [24..27] : length_cm      (int32 little-endian)
//   [28..31] : height_cm      (int32 little-endian)
//
// These field widths come from the team’s A3 roll-up storage design
// (License=10, Phone=14, Length=4, Height=4).  A4 requires fixed-length
// binary records for efficient random access. 
// ---------------------------------------------------------------------------

#include <cstdint>
#include <cstddef>
#include <array>
#include <string>
#include <string_view>

namespace FerrySys
{
    // Public constants (shared across encode/decode & tests)
    inline constexpr std::size_t VEH_LIC_CHARS   = 10;
    inline constexpr std::size_t VEH_PHONE_CHARS = 14;
    inline constexpr std::size_t VEH_REC_BYTES   = 32;

    struct VehicleRecord
    {
        // In-memory friendly form
        std::string license;     // will be trimmed to <=10 when encoded
        std::string phone;       // trimmed to <=14 when encoded
        std::int32_t length_cm{0};
        std::int32_t height_cm{0};

        // Helper: derived classification
        bool isSpecial() const noexcept
        {
            // Spec threshold: >2m height OR >7m length => Special (High Ceiling).
            // Height/length stored in cm; 200cm == 2m, 700cm == 7m.
            return (height_cm > 200) || (length_cm > 700);
        }
    };

    // Raw 32-byte buffer type used for on-disk transfer.
    using VehicleRaw = std::array<unsigned char, VEH_REC_BYTES>;

    // Encode an in-memory VehicleRecord into a 32-byte raw buffer.
    // Strings longer than the field width are truncated; shorter are space-padded.
    void encodeVehicle(const VehicleRecord &in, VehicleRaw &out) noexcept;

    // Decode a 32-byte raw buffer into an in-memory VehicleRecord (trims trailing spaces).
    void decodeVehicle(const VehicleRaw &in, VehicleRecord &out);

    // Convenience compare used in tests: true if all fields equal.
    bool vehicleEqual(const VehicleRecord &a, const VehicleRecord &b) noexcept;
}

#endif // VEHICLE_RECORD_HPP
