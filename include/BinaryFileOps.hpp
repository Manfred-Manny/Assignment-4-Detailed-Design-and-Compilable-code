#ifndef BINARY_FILE_OPS_HPP
#define BINARY_FILE_OPS_HPP
#pragma once
// ---------------------------------------------------------------------------
// BinaryFileOps.hpp
// CMPT 276 – Assignment 4 (Manny T4/T5)
//
// Minimal fixed-record binary file utilities supporting the *simple,
// unsorted* file organization required by the A4 spec:
//
//   • Records are fixed-length binary blocks. (Spec: fixed-length binary.)
//   • Insert = append to end of file.
//   • Search = linear scan (caller-supplied predicate).
//   • Delete = overwrite target record with last record, then truncate file
//              by 1 record. (Spec: simple unsorted deletion model.)
//
// These utilities are record-size agnostic: you pass the record byte size and
// a raw byte buffer. Higher-level encode/decode is done by the caller
// (e.g., see VehicleRecord.hpp).
// ---------------------------------------------------------------------------

#include <cstddef>
#include <cstdint>
#include <string>
#include <functional>
#include <iosfwd>

namespace FerrySys
{
    // Open an existing binary file for update (in/out), creating it if missing.
    // Returns open fstream object (by value) in binary mode positioned at end.
    // Throws std::ios_base::failure on open errors if exceptions enabled;
    // otherwise caller must test .is_open().
    std::fstream openBinaryFile(const std::string &path);

    // Return number of whole records currently in the file.
    // Computed by file size / recordSize. Assumes no partial record at end.
    std::size_t recordCount(std::fstream &fs, std::size_t recordSize);

    // Read record at index into caller buffer. Returns true on success.
    bool readRecord(std::fstream &fs, std::size_t recordSize,
                    std::size_t index, void *outBytes);

    // Overwrite record at index from caller buffer. Returns true on success.
    bool writeRecord(std::fstream &fs, std::size_t recordSize,
                     std::size_t index, const void *inBytes);

    // Append record to end of file. Returns true on success.
    bool appendRecord(std::fstream &fs, std::size_t recordSize,
                      const void *inBytes);

    // Swap-delete: overwrite record 'index' with last record in file,
    // then truncate file by one record. Returns true on success.
    bool swapDeleteRecord(std::fstream &fs, const std::string &path,
                          std::size_t recordSize, std::size_t index);

    // Linear search: iterate all records; predicate called with (index, bytesPtr).
    // Predicate returns true to signal match; search stops and returns index.
    // Returns recordCount() if not found (caller can treat as npos).
    std::size_t linearSearch(std::fstream &fs, std::size_t recordSize,
                             const std::function<bool(std::size_t, const void*)> &predicate);
}

#endif // BINARY_FILE_OPS_HPP
