CMPT 276 – Assignment 4
Manny Tasks T4 & T5: Binary File Ops + UnitTest #1
==================================================

Author: Manny
Term: Summer 2025
Compiler: MinGW‑w64 g++ 14.2.0 (C++20)
Project Root: cmpt276-a4/

Overview
--------
This package provides:
  • Fixed‑length *binary* Vehicle record layout (32 bytes: 10+14+4+4).
  • Low‑level binary file utilities (append, indexed read, swap‑delete, record count).
  • UnitTest #1 (required by A4): writes several Vehicle records one‑at‑a‑time,
    reads them back one‑at‑a‑time, verifies equality, and checks EOF behavior.
  • Stub implementations for all other modules (Vessel, Sailing, Reservation, FileIO_*)
    so the entire codebase compiles now; teammates can fill in logic later.

Directory Layout
----------------
cmpt276-a4/
 ├─ include/             # Public headers (A3 originals + new Manny headers)
 │   ├─ *.h              # Team headers from A3
 │   ├─ VehicleRecord.hpp    # Fixed-length record encode/decode
 │   └─ BinaryFileOps.hpp    # Low-level binary file functions
 ├─ src/
 │   ├─ *.cpp            # Stubs for all team headers (compile only)
 │   ├─ VehicleRecord.cpp
 │   └─ BinaryFileOps.cpp
 ├─ tests/
 │   ├─ testFileOps.cpp  # UnitTest #1 (binary write/read/EOF)
 │   └─ placeholder_UnitTest2.cpp (empty; fill later)
 ├─ data/                # Runtime data files created by tests (ignored by git)
 └─ build/               # Out-of-source build output (ignored by git)

Quick Build: UnitTest #1 Only
-----------------------------
From a PowerShell prompt in project root:

  cd build
  g++ -std=c++20 -Wall -Wextra -pedantic -I..\\include ^
      ..\\tests\\testFileOps.cpp ^
      ..\\src\\VehicleRecord.cpp ..\\src\\BinaryFileOps.cpp ^
      -o testFileOps.exe

(If line continuations cause trouble, paste as one line.)

Run:

  .\\testFileOps.exe

Expected output:

  UnitTest #1 (binary file write/read/EOF) PASS

What UnitTest #1 Does
---------------------
1. Ensures ../data/ exists (relative to build/).
2. Deletes any prior ../data/vehicle_test.dat.
3. Writes 3 Vehicle records *one at a time* using BinaryFileOps::appendRecord().
4. Reopens file; reads back records *one at a time* via readRecord().
5. Compares decoded records to originals.
6. Attempts a past-end read to confirm EOF handling.
7. Prints PASS / FAIL and returns 0 / 1 exit code.

Record Format (Vehicle)
-----------------------
  Offset  Size  Field
  ------  ----  -----------------------------
  0       10    licensePlate   (space-padded)
  10      14    phoneNumber    (space-padded)
  24       4    length_cm      (int32 little-endian)
  28       4    height_cm      (int32 little-endian)
  TOTAL   32 bytes

Why Fixed-Length Binary?
------------------------
• Direct index math: byteOffset = recordIndex * 32.
• No parsing / conversion overhead.
• Compatible with random access (seekg/seekp).
• Stable cross-module contract for later team code.

Simple Unsorted File Organization
---------------------------------
• Insert: append at end of file.
• Read: indexed or sequential scan.
• Delete: overwrite target record with last record; truncate file by 1 record.
  (Supported in BinaryFileOps::swapDeleteRecord(); not exercised in UnitTest #1,
   but ready for UnitTest #2.)

Building All Sources (Optional Compile Check)
--------------------------------------------
To verify *all modules compile* (A4 expectation), build every .cpp (object-only or link):

  cd build
  for %f in (..\\src\\*.cpp) do echo %f
  g++ -std=c++20 -Wall -Wextra -pedantic -I..\\include -c ..\\src\\VehicleRecord.cpp
  g++ -std=c++20 -Wall -Wextra -pedantic -I..\\include -c ..\\src\\BinaryFileOps.cpp
  g++ -std=c++20 -Wall -Wextra -pedantic -I..\\include -c ..\\src\\FileIO_Reservations.cpp
  g++ -std=c++20 -Wall -Wextra -pedantic -I..\\include -c ..\\src\\FileIO_Sailings.cpp
  g++ -std=c++20 -Wall -Wextra -pedantic -I..\\include -c ..\\src\\FileIO_Vessel.cpp
  g++ -std=c++20 -Wall -Wextra -pedantic -I..\\include -c ..\\src\\Reservation.cpp
  g++ -std=c++20 -Wall -Wextra -pedantic -I..\\include -c ..\\src\\Sailing.cpp
  g++ -std=c++20 -Wall -Wextra -pedantic -I..\\include -c ..\\src\\Vessel.cpp
  g++ -std=c++20 -Wall -Wextra -pedantic -I..\\include -c ..\\src\\UserInterface.cpp

(You can link into a single dummy exe if you want; not required by A4.)

Teammate Integration Notes
--
