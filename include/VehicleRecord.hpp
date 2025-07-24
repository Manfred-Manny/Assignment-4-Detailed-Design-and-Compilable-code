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
    // Public constants (shared across encode/decode & tests)
    constexpr std::size_t VEH_LIC_CHARS   = 10;
    constexpr std::size_t VEH_PHONE_CHARS = 14;
    constexpr std::size_t VEH_REC_BYTES   = 32;

    struct VehicleRecord
    {
        std::string license;     // Trimmed to <=10 when encoded
        std::string phone;       // Trimmed to <=14 when encoded
        std::int32_t length_cm = 0;
        std::int32_t height_cm = 0;

        // Special classification
        bool isSpecial() const noexcept
        {
            return (height_cm > 200) || (length_cm > 700);
        }
    };

    using VehicleRaw = std::array<unsigned char, VEH_REC_BYTES>;

    void encodeVehicle(const VehicleRecord &in, VehicleRaw &out) noexcept;
    void decodeVehicle(const VehicleRaw &in, VehicleRecord &out);
    bool vehicleEqual(const VehicleRecord &a, const VehicleRecord &b) noexcept;
}

#endif // VEHICLE_RECORD_HPP
