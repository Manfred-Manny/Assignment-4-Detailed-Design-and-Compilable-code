//************************************************************
//************************************************************
//  FileIO_Vessel.h
//  CMPT 276 – Assignment 4 (Fahad M)
//
//  PURPOSE:
//    This header defines binary file operations for managing
//    vessel records within `vessels.dat`. Each record contains
//    a vessel name and lane capacities for high-ceiling (HCL)
//    and low-ceiling (LCL) vehicles.
//
//    Operations supported include sequential reads (reset and
//    getNextVessel), appending new vessels, deleting vessels,
//    and fetching vessel data by name for use by the business
//    logic (e.g., during sailing creation).
//
//************************************************************
//************************************************************

#ifndef FILEIO_VESSEL_H
#define FILEIO_VESSEL_H

#include <fstream>
#include <string>

// ------------------------------------------------------------
// Binary layout for a vessel record in `vessels.dat`
// ------------------------------------------------------------
struct Vesselrec {
    char vesselName[25];       // Vessel name (null-terminated)
    unsigned short laneHCL;    // High-ceiling lane length (m)
    unsigned short laneLCL;    // Low-ceiling lane length (m)
};

class FileIO_Vessel
{
public:
    //------------------------------------------------------------
    // Reset file pointer for sequential scans.
    // Preconditions : none
    // Postconditions: file is ready for reading from start.
    static void reset();

    //------------------------------------------------------------
    // Sequentially read next vessel record.
    // Preconditions : reset() called first.
    // Postconditions: fills output params if read successful.
    static bool getNextVessel(
        std::string  &vesselName,  // OUT: Vessel name
        unsigned int &laneHCL,     // OUT: HCL capacity
        unsigned int &laneLCL      // OUT: LCL capacity
    );

    //------------------------------------------------------------
    // Append a new vessel record.
    // Preconditions : validated inputs (unique name).
    // Postconditions: record is appended to `vessels.dat`.
    static void writeVessel(
        const std::string &vesselName, // IN: Vessel name
        unsigned int laneHCL,          // IN: HCL capacity
        unsigned int laneLCL           // IN: LCL capacity
    );

    //------------------------------------------------------------
    // Remove a vessel by name (rewrite file without it).
    // Preconditions : vessel is not in use by a sailing.
    // Postconditions: returns true if removed, false otherwise.
    static bool deleteVessel(
        const std::string &vesselName // IN: Vessel name
    );

    //------------------------------------------------------------
    // Retrieve next vessel using an existing file stream.
    // Preconditions : file is open and valid.
    // Postconditions: populates outputs if read successful.
    static bool getNextVessel(
        std::fstream &file,
        std::string  &vesselName,
        unsigned int &laneHCL,
        unsigned int &laneLCL
    );

    //------------------------------------------------------------
    // Lookup a vessel by name and retrieve its lane capacities.
    // Preconditions : name must exist in file.
    // Postconditions: fills outputs if found, else returns false.
    static bool getVesselByName(
    const std::string &vesselName,
    unsigned int &laneHCL,
    unsigned int &laneLCL
);

};

#endif // FILEIO_VESSEL_H
