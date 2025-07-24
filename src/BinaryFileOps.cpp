// ---------------------------------------------------------------------------
// BinaryFileOps.cpp
// CMPT 276 – Assignment 4 (Manny T4/T5)
//
// Implements low-level *fixed-record binary* file operations needed for
// Assignment 4 UnitTest #1.  Records are constant-size byte blocks; we use
// a simple *unsorted* organization:
//   • Append new records to end.
//   • Linear search to find a match.
//   • Delete by overwriting target with last record, then truncating file.
// These behaviors satisfy the A4 storage rules (fixed-length binary records;
// simple unsorted delete model). 
// ---------------------------------------------------------------------------

#include "BinaryFileOps.hpp"

#include <fstream>
#include <functional>
#include <filesystem>
#include <filesystem>
#include <vector>
#include <cassert>

namespace FerrySys
{
    using std::fstream;
    using std::ios;
    namespace fs = std::filesystem;

    // Utility: reopen create-if-missing
    fstream openBinaryFile(const std::string &path)
    {
        // Try open existing for read/write update.
        fstream fs(path, ios::in | ios::out | ios::binary);
        if (!fs.is_open())
        {
            // Create new (truncate) then reopen update.
            fstream create(path, ios::out | ios::binary);
            create.close();
            fs.open(path, ios::in | ios::out | ios::binary);
        }
        return fs;
    }

    std::size_t recordCount(fstream &fs, std::size_t recordSize)
    {
        fs.clear();
        auto cur = fs.tellg();
        fs.seekg(0, ios::end);
        std::streampos endPos = fs.tellg();
        fs.seekg(cur); // restore
        if (endPos < 0)
        {
            return 0;
        }
        std::uintmax_t bytes = static_cast<std::uintmax_t>(endPos);
        return static_cast<std::size_t>(bytes / recordSize);
    }

    bool readRecord(fstream &fs, std::size_t recordSize,
                    std::size_t index, void *outBytes)
    {
        fs.clear();
        fs.seekg(static_cast<std::streamoff>(index * recordSize), ios::beg);
        if (!fs.good())
        {
            return false;
        }
        fs.read(static_cast<char*>(outBytes),
                static_cast<std::streamsize>(recordSize));
        return fs.good();
    }

    bool writeRecord(fstream &fs, std::size_t recordSize,
                     std::size_t index, const void *inBytes)
    {
        fs.clear();
        fs.seekp(static_cast<std::streamoff>(index * recordSize), ios::beg);
        if (!fs.good())
        {
            return false;
        }
        fs.write(static_cast<const char*>(inBytes),
                 static_cast<std::streamsize>(recordSize));
        fs.flush();
        return fs.good();
    }

    bool appendRecord(fstream &fs, std::size_t recordSize,
                      const void *inBytes)
    {
        fs.clear();
        fs.seekp(0, ios::end);
        if (!fs.good())
        {
            return false;
        }
        fs.write(static_cast<const char*>(inBytes),
                 static_cast<std::streamsize>(recordSize));
        fs.flush();
        return fs.good();
    }

    bool swapDeleteRecord(fstream &fs, const std::string &path,
                          std::size_t recordSize, std::size_t index)
    {
        std::size_t count = recordCount(fs, recordSize);
        if (index >= count)
        {
            return false; // out of range
        }

        // If deleting last record, just truncate.
        std::size_t lastIdx = count - 1;
        if (index != lastIdx)
        {
            // Read last record
            std::vector<unsigned char> buf(recordSize);
            if (!readRecord(fs, recordSize, lastIdx, buf.data()))
            {
                return false;
            }
            // Overwrite index with last
            if (!writeRecord(fs, recordSize, index, buf.data()))
            {
                return false;
            }
        }

        // Truncate file by one record (resize_file closes underlying handle
        // cleanly; need to flush first).
        fs.flush();
        try
        {
            auto newSize = static_cast<std::uintmax_t>((count - 1) * recordSize);
            fs::resize_file(path, newSize);
        }
        catch (...)
        {
            return false;
        }
        return true;
    }

    std::size_t linearSearch(fstream &fs, std::size_t recordSize,
                             const std::function<bool(std::size_t, const void*)> &predicate)
    {
        std::size_t count = recordCount(fs, recordSize);
        std::vector<unsigned char> buf(recordSize);

        for (std::size_t i = 0; i < count; ++i)
        {
            if (!readRecord(fs, recordSize, i, buf.data()))
            {
                break; // I/O error; abort
            }
            if (predicate(i, buf.data()))
            {
                return i;
            }
        }
        return count; // not found
    }
} // namespace FerrySys
