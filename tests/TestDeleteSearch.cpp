// ---------------------------------------------------------------------------
// testDeleteSearch.cpp
// CMPT 276 – A4  
// Focus: search + delete (swap-with-last + truncate) on fixed-length records
//
// Scenario:
//   1. Start empty test file.
//   2. Append 5 VehicleRecords.
//   3. Search by licence, delete that record with swapDeleteRecord().
//   4. Verify:
//        • recordCount decreased by 1
//        • deleted licence no longer found
//        • last record moved into the deleted slot (data integrity)
//   5. Repeat edge cases: delete first, middle, last, and only record.
//   6. Print PASS/FAIL.
// ---------------------------------------------------------------------------

#include "VehicleRecord.hpp"
#include "BinaryFileOps.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>
#include <cstring>  


namespace fs = std::filesystem;
using namespace FerrySys;

static const char *kRelDataDir   = "../data";
static const char *kTestDataFile = "../data/vehicle_ds_test.dat";

static void ensureDataDir()
{
    std::error_code ec;
    if (!fs::exists(kRelDataDir, ec)) fs::create_directories(kRelDataDir, ec);
}

static VehicleRecord mk(std::string lic, std::string ph, int len, int ht)
{
    return VehicleRecord{ lic, ph, len, ht };
}

// Helper: encode+append one record
static bool appendVR(std::fstream &fs, const VehicleRecord &vr)
{
    VehicleRaw raw{};
    encodeVehicle(vr, raw);
    return appendRecord(fs, VEH_REC_BYTES, raw.data());
}

// Helper: decode record at index
static bool getVR(std::fstream &fs, std::size_t idx, VehicleRecord &out)
{
    VehicleRaw raw{};
    if (!readRecord(fs, VEH_REC_BYTES, idx, raw.data())) return false;
    decodeVehicle(raw, out);
    return true;
}

// Linear search by licence using BinaryFileOps::linearSearch
static std::size_t findByLicence(std::fstream &fs, const std::string &lic)
{
    return linearSearch(fs, VEH_REC_BYTES,
        [&](std::size_t, const void *bytes)
        {
            const unsigned char *b = static_cast<const unsigned char*>(bytes);
            VehicleRaw raw{};
            std::memcpy(raw.data(), b, VEH_REC_BYTES);
            VehicleRecord tmp;
            decodeVehicle(raw, tmp);
            return tmp.license == lic;
        });
}

int main()
{
    ensureDataDir();
    // Fresh file
    {
        std::error_code ec;
        fs::remove(kTestDataFile, ec);
    }

    // Build initial set
    std::vector<VehicleRecord> orig = {
        mk("AAA111","6040000001",500,180),
        mk("BBB222","6040000002",720,250),
        mk("CCC333","6040000003",300,150),
        mk("DDD444","6040000004",820,260),
        mk("EEE555","6040000005",650,190)
    };

    bool pass = true;

    // 1) Append all
    {
        std::fstream fs = openBinaryFile(kTestDataFile);
        if (!fs.is_open()) { std::cerr << "Open failed\n"; return 1; }
        for (auto &vr : orig) {
            if (!appendVR(fs, vr)) { std::cerr << "Append failed\n"; return 1; }
        }
        fs.close();
    }

    auto checkDelete = [&](const std::string &licToDelete) {
        std::fstream fs = openBinaryFile(kTestDataFile);
        if (!fs.is_open()) { std::cerr << "Open failed\n"; pass=false; return; }

        std::size_t countBefore = recordCount(fs, VEH_REC_BYTES);
        std::size_t idx = findByLicence(fs, licToDelete);
        if (idx == countBefore) {
            std::cerr << "FAIL: licence not found before delete: " << licToDelete << "\n";
            pass = false; fs.close(); return;
        }

        // capture last record to verify move
        VehicleRecord lastVR;
        getVR(fs, countBefore - 1, lastVR);

        // Perform delete
        if (!swapDeleteRecord(fs, kTestDataFile, VEH_REC_BYTES, idx)) {
            std::cerr << "FAIL: swapDeleteRecord failed\n";
            pass = false; fs.close(); return;
        }
        fs.close();

        // Reopen and verify
        fs = openBinaryFile(kTestDataFile);
        std::size_t countAfter = recordCount(fs, VEH_REC_BYTES);
        if (countAfter != countBefore - 1) {
            std::cerr << "FAIL: count mismatch after delete\n";
            pass = false;
        }

        // Deleted licence should not be found
        std::size_t idx2 = findByLicence(fs, licToDelete);
        if (idx2 != countAfter) {
            std::cerr << "FAIL: deleted licence still found\n";
            pass = false;
        }

        // If we didn't delete the last element, ensure last moved correctly
        if (idx < countAfter) { // there is a record now at deleted slot
            VehicleRecord moved;
            if (!getVR(fs, idx, moved)) {
                std::cerr << "FAIL: cannot read moved record\n";
                pass = false;
            } else if (!vehicleEqual(moved, lastVR)) {
                std::cerr << "FAIL: moved record data mismatch\n";
                pass = false;
            }
        }

        fs.close();
    };

    // Case 1: delete middle
    checkDelete("CCC333");
    // Case 2: delete first
    checkDelete("AAA111");
    // Case 3: delete last (current last is EEE555 or DDD444 depending on moves)
    checkDelete("EEE555");
    // Case 4: delete remaining until one left, then delete that one
    checkDelete("BBB222");
    checkDelete("DDD444"); // last/only record

    {
    std::fstream fs = openBinaryFile(kTestDataFile);
    std::size_t finalCount = recordCount(fs, VEH_REC_BYTES);
    if (finalCount != 0) {
        std::cerr << "FAIL: final recordCount not zero (" << finalCount << ")\n";
        pass = false;
    }
    fs.close();
}

// Optional negative test: delete non-existent
{
    std::fstream fs = openBinaryFile(kTestDataFile);
    std::size_t before = recordCount(fs, VEH_REC_BYTES);
    std::size_t idx = findByLicence(fs, "NOTHERE");
    if (idx != before) { // linearSearch should return count if not found
        std::cerr << "FAIL: unexpected find result for non-existent licence\n";
        pass = false;
    }
    fs.close();
}

    if (pass) {
        std::cout << "UnitTest #2 (search + swap-delete) PASS\n";
        return 0;
    } else {
        std::cout << "UnitTest #2 (search + swap-delete) FAIL\n";
        return 1;
    }
}
