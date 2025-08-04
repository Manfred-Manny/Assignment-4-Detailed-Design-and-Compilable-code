// ---------------------------------------------------------------------------
// testFileOps.cpp
// CMPT 276 – Assignment 4 (Manny T5: UnitTest #1)
//
// PURPOSE
//   Minimal, self-contained test of fixed-length *binary* record I/O.
//   Creates a fresh data file, writes several VehicleRecord entries
//   one-at-a-time, then reopens and reads them back one-at-a-time to
//   verify data integrity. Also attempts a past-end read to confirm
//   proper EOF handling. Prints PASS/FAIL summary.
//
// SPEC COVERAGE
//   ✓ Writes a couple of records (one at a time).
//   ✓ Reads them back (one at a time) and checks equality.
//   ✓ Confirms read stops at end-of-file.
//   Meets A4 UnitTest #1 binary file I/O requirement.
//
// BUILD (from build/ folder, PowerShell):
//   g++ -std=c++20 -Wall -Wextra -pedantic -I../include \
//       ../tests/testFileOps.cpp \
//       ../src/VehicleRecord.cpp ../src/BinaryFileOps.cpp \
//       -o testFileOps.exe
//
// RUN:
//   ./testFileOps.exe        (Windows: .\\testFileOps.exe)
//
// EXIT CODE: 0 = PASS, 1 = FAIL
// ---------------------------------------------------------------------------

#include "VehicleRecord.hpp"
#include "BinaryFileOps.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>
#include <cassert>

namespace fs = std::filesystem;
using FerrySys::VehicleRecord;
using FerrySys::VehicleRaw;
using FerrySys::encodeVehicle;
using FerrySys::decodeVehicle;
using FerrySys::vehicleEqual;
using FerrySys::openBinaryFile;
using FerrySys::recordCount;
using FerrySys::readRecord;
using FerrySys::appendRecord;

static const char *kRelDataDir  = "../data";
static const char *kTestDataFile = "../data/vehicle_test.dat";

// ----------------------------------------------------------------------------
// helpers
// ----------------------------------------------------------------------------

static void ensureDataDir()
{
    fs::path dir = kRelDataDir;
    std::error_code ec;
    if (!fs::exists(dir, ec))
    {
        fs::create_directories(dir, ec);
    }
}

static VehicleRecord makeRec(std::string lic,
                             std::string phone,
                             int len_cm,
                             int ht_cm)
{
    VehicleRecord vr;
    vr.license   = std::move(lic);
    vr.phone     = std::move(phone);
    vr.length_m = len_cm;
    vr.height_m = ht_cm;
    return vr;
}

// ----------------------------------------------------------------------------
// main test
// ----------------------------------------------------------------------------
int main()
{
    ensureDataDir();

    // Remove any old test file
    {
        std::error_code ec;
        fs::remove(kTestDataFile, ec);
    }

    // ------------------------------------------------------------------------
    // Create sample in-memory records
    // ------------------------------------------------------------------------
    std::vector<VehicleRecord> orig {
        makeRec("ABC123",    "6045550001", 450, 175),
        makeRec("ZZTOP99",   "7781234567", 820, 250), // special
        makeRec("PL8FULL",   "2368889999", 700, 210)  // borderline height
    };

    // encode + write (append) one at a time
    {
        std::fstream fs = openBinaryFile(kTestDataFile);
        if (!fs.is_open())
        {
            std::cerr << "ERROR: cannot open " << kTestDataFile << " for write.\n";
            return 1;
        }

        for (const auto &rec : orig)
        {
            VehicleRaw raw{};
            encodeVehicle(rec, raw);
            if (!appendRecord(fs, FerrySys::VEH_REC_BYTES, raw.data()))
            {
                std::cerr << "ERROR: append failed.\n";
                return 1;
            }
        }
        fs.close();
    }

    // ------------------------------------------------------------------------
    // Re-open and read back sequentially, verifying equality
    // ------------------------------------------------------------------------
    bool pass = true;
    {
        std::fstream fs = openBinaryFile(kTestDataFile);
        if (!fs.is_open())
        {
            std::cerr << "ERROR: cannot reopen " << kTestDataFile << " for read.\n";
            return 1;
        }

        std::size_t count = recordCount(fs, FerrySys::VEH_REC_BYTES);
        if (count != orig.size())
        {
            std::cerr << "FAIL: recordCount mismatch. expected "
                      << orig.size() << " got " << count << "\n";
            pass = false;
        }

        VehicleRaw raw{};
        for (std::size_t i = 0; i < count; ++i)
        {
            if (!readRecord(fs, FerrySys::VEH_REC_BYTES, i, raw.data()))
            {
                std::cerr << "FAIL: readRecord failed at index " << i << "\n";
                pass = false;
                break;
            }
            VehicleRecord got;
            decodeVehicle(raw, got);
            if (!vehicleEqual(orig[i], got))
            {
                std::cerr << "FAIL: mismatch at index " << i << "\n"
                          << "  wrote: " << orig[i].license << ", "
                          << orig[i].phone << ", L=" << orig[i].length_m
                          << ", H=" << orig[i].height_m << "\n"
                          << "   read: " << got.license << ", "
                          << got.phone << ", L=" << got.length_m
                          << ", H=" << got.height_m << "\n";
                pass = false;
            }
        }

        // Past-end read (EOF behavior)
        if (pass)
        {
            if (readRecord(fs, FerrySys::VEH_REC_BYTES, count, raw.data()))
            {
                std::cerr << "FAIL: expected past-end read to fail, but it succeeded.\n";
                pass = false;
            }
        }
        fs.close();
    }

    // ------------------------------------------------------------------------
    // Report
    // ------------------------------------------------------------------------
    if (pass)
    {
        std::cout << "UnitTest #1 (binary file write/read/EOF) PASS\n";
        return 0;
    }
    else
    {
        std::cout << "UnitTest #1 (binary file write/read/EOF) FAIL\n";
        return 1;
    }
}
