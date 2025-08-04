//************************************************************
//************************************************************
//  VehicleRecord.hpp
//  CMPT 276 – Assignment 4 (Manny)
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
#include <cstring> // for memset, memcpy

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
    // -----------------------------------------------------------------------
    struct VehicleRecord
    {
        std::string license;     // Trimmed to <=10 chars when encoded
        std::string phone;       // Trimmed to <=14 chars when encoded
        std::int32_t length_m = 0;
        std::int32_t height_m = 0;

        bool isSpecial() const noexcept
        {
            return (height_m > 200) || (length_m > 700);
        }
    };

    // -----------------------------------------------------------------------
    // Raw 32-byte vehicle record
    // -----------------------------------------------------------------------
    using VehicleRaw = std::array<unsigned char, VEH_REC_BYTES>;

    // -----------------------------------------------------------------------
    // Shared helper: encode/decode fixed-length string fields (space-padded)
    // -----------------------------------------------------------------------
    inline void encodeField(const std::string &src, unsigned char *dest, std::size_t len)
    {
        std::memset(dest, ' ', len);
        std::size_t n = (src.size() < len) ? src.size() : len;
        std::memcpy(dest, src.data(), n);
    }

    inline std::string decodeField(const unsigned char *src, std::size_t len)
    {
        std::string s(reinterpret_cast<const char*>(src), len);
        while (!s.empty() && s.back() == ' ') s.pop_back();
        return s;
    }

    // Encode full vehicle record to raw bytes
    void encodeVehicle(const VehicleRecord &in, VehicleRaw &out) noexcept;

    // Decode raw bytes to full vehicle record
    void decodeVehicle(const VehicleRaw &in, VehicleRecord &out);

    // Compare two vehicle records
    bool vehicleEqual(const VehicleRecord &a, const VehicleRecord &b) noexcept;
}

#endif // VEHICLE_RECORD_HPP
