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

#include <algorithm>
#include <array>
#include <cctype>
#include <cstring>
#include <string>

namespace FerrySys
{
    // --- local helpers -----------------------------------------------------

    // Trim trailing spaces from a string (in-place).
    static void rtrimSpaces(std::string &s)
    {
        while (!s.empty() && s.back() == ' ')
        {
            s.pop_back();
        }
    }

    // Copy src into dest field (space-pad; truncate if longer).
    static void copyFieldSpacePad(std::string_view src,
                                  unsigned char *dest,
                                  std::size_t fieldLen) noexcept
    {
        // copy up to fieldLen bytes
        std::size_t n = (src.size() < fieldLen) ? src.size() : fieldLen;
        if (n > 0)
        {
            std::memcpy(dest, src.data(), n);
        }
        // pad remainder with spaces
        if (fieldLen > n)
        {
            std::memset(dest + n, ' ', fieldLen - n);
        }
    }

    // Read fieldLen bytes, build std::string, trim spaces.
    static std::string extractFieldTrim(const unsigned char *src,
                                        std::size_t fieldLen)
    {
        std::string out(reinterpret_cast<const char*>(src), fieldLen);
        rtrimSpaces(out);
        return out;
    }

    // --- public encode/decode ----------------------------------------------

    void encodeVehicle(const VehicleRecord &in, VehicleRaw &out) noexcept
    {
        // license [0..9]
        copyFieldSpacePad(in.license, out.data(), VEH_LIC_CHARS);

        // phone [10..23]
        copyFieldSpacePad(in.phone, out.data() + VEH_LIC_CHARS, VEH_PHONE_CHARS);

        // length_cm [24..27]
        std::int32_t len = in.length_cm;
        std::memcpy(out.data() + VEH_LIC_CHARS + VEH_PHONE_CHARS, &len, sizeof(len));

        // height_cm [28..31]
        std::int32_t ht = in.height_cm;
        std::memcpy(out.data() + VEH_LIC_CHARS + VEH_PHONE_CHARS + sizeof(len),
                    &ht, sizeof(ht));
    }

    void decodeVehicle(const VehicleRaw &in, VehicleRecord &out)
    {
        out.license = extractFieldTrim(in.data(), VEH_LIC_CHARS);
        out.phone   = extractFieldTrim(in.data() + VEH_LIC_CHARS, VEH_PHONE_CHARS);

        std::int32_t len = 0;
        std::int32_t ht  = 0;
        std::memcpy(&len, in.data() + VEH_LIC_CHARS + VEH_PHONE_CHARS, sizeof(len));
        std::memcpy(&ht,  in.data() + VEH_LIC_CHARS + VEH_PHONE_CHARS + sizeof(len),
                    sizeof(ht));

        out.length_cm = len;
        out.height_cm = ht;
    }

    bool vehicleEqual(const VehicleRecord &a, const VehicleRecord &b) noexcept
    {
        return a.license == b.license
            && a.phone   == b.phone
            && a.length_cm == b.length_cm
            && a.height_cm == b.height_cm;
    }
} // namespace FerrySys
