//************************************************************
//************************************************************
//  FileIO_Vessel.h
//  Binary I/O helper for Vessel master list
//************************************************************
//************************************************************
#ifndef FILEIO_VESSEL_H
#define FILEIO_VESSEL_H

#include <fstream>
#include <string>

class FileIO_Vessel
{
public:
//------------------------------------------------------------
// Rewinds Vessel.dat for sequential scans.
// Preconditions : none
// Postconditions: iterator ready.
    static void reset();

//------------------------------------------------------------
// Retrieves next vessel record.
// Preconditions : reset() already called.
// Postconditions: returns true if fields populated, false on EOF.
    static bool getNextVessel(
        std::string  &vesselName,  // OUT : vessel mnemonic
        unsigned int &laneHCL,     // OUT : high-ceiling metres
        unsigned int &laneLCL      // OUT : low-ceiling metres
    );

//------------------------------------------------------------
// Adds a new vessel to file.
// Preconditions : name unique.
// Postconditions: record appended.
    static void writeVessel(
        const std::string &vesselName, // IN : mnemonic
        unsigned int        laneHCL,   // IN : high-ceiling metres
        unsigned int        laneLCL    // IN : low-ceiling metres
    );

//------------------------------------------------------------
// Removes vessel record.
// Preconditions : vessel not currently assigned to a sailing.
// Returns      : true on success, false if not found / in-use.
    static bool deleteVessel(
        const std::string &vesselName   // IN : mnemonic
    );

//------------------------------------------------------------
// Convenience helper: returns vessel mnemonic for given index.
// (Used by UI drop-down lists.)
// Preconditions : index valid.
// Returns      : mnemonic string, empty if OOB.
    static std::string getVesselName(
        unsigned int        index       // IN : ordinal index
    );
};

#endif // FILEIO_VESSEL_H
