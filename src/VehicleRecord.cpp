// ---------------------------------------------------------------------------
// VehicleRecord.cpp
// CMPT 276 – Assignment 4 (Manny T4/T5)
// 
// Implements encode/decode helpers for the fixed-length *on-disk* vehicle
// record layout documented in VehicleRecord.hpp (32 bytes: 10 + 14 + 4 + 4).
//
// Field widths taken from team A3 roll-up storage decision (License=10,
// Phone=14, Length=4, Height=4).  A4 requires records be *fixed-length binary*
// for efficient random access file I/O. 
// ---------------------------------------------------------------------------
#include "VehicleRecord.hpp"
#include <cstring> // memcpy

namespace FerrySys
{
    void encodeVehicle(const VehicleRecord &in, VehicleRaw &out) noexcept
    {
        // License [0..9]
        encodeField(in.license, out.data(), VEH_LIC_CHARS);

        // Phone [10..23]
        encodeField(in.phone, out.data() + VEH_LIC_CHARS, VEH_PHONE_CHARS);

        // Length [24..27]
        std::int32_t len = in.length_m;
        std::memcpy(out.data() + VEH_LIC_CHARS + VEH_PHONE_CHARS, &len, sizeof(len));

        // Height [28..31]
        std::int32_t ht = in.height_m;
        std::memcpy(out.data() + VEH_LIC_CHARS + VEH_PHONE_CHARS + sizeof(len),
                    &ht, sizeof(ht));
    }

    void decodeVehicle(const VehicleRaw &in, VehicleRecord &out)
    {
        // License
        out.license = decodeField(in.data(), VEH_LIC_CHARS);

        // Phone
        out.phone   = decodeField(in.data() + VEH_LIC_CHARS, VEH_PHONE_CHARS);

        // Length & height
        std::int32_t len = 0, ht = 0;
        std::memcpy(&len, in.data() + VEH_LIC_CHARS + VEH_PHONE_CHARS, sizeof(len));
        std::memcpy(&ht,  in.data() + VEH_LIC_CHARS + VEH_PHONE_CHARS + sizeof(len),
                    sizeof(ht));

        out.length_m = len;
        out.height_m = ht;
    }

    bool vehicleEqual(const VehicleRecord &a, const VehicleRecord &b) noexcept
    {
        return a.license == b.license &&
               a.phone   == b.phone &&
               a.length_m == b.length_m &&
               a.height_m == b.height_m;
    }
}
