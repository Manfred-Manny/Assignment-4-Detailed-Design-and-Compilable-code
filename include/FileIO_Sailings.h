//************************************************************
//************************************************************
//  FileIO_Sailings.h
//  Binary I/O helper for Sailing records
//************************************************************
//************************************************************
#ifndef FILEIO_SAILINGS_H
#define FILEIO_SAILINGS_H

#include <fstream>
#include <string>
#include "CommonTypes.h"

class FileIO_Sailings
{
public:
//------------------------------------------------------------
// Re-opens Sailings.dat and rewinds.
// Preconditions : none
// Postconditions: ready for sequential getNextSailing().
    static void reset();

//------------------------------------------------------------
// Reads next sailing record.
// Preconditions : reset() already called.
// Postconditions: returns true if fields filled, false on EOF.
    static bool getNextSailing(
        SailingID        &sailingId,      // OUT : PK
        std::string      &vesselName,     // OUT : vessel assigned
        std::string      &arrivalCity,    // OUT : destination port
        std::string      &dateISO,        // OUT : YYYY-MM-DD
        std::string      &timeHHMM,       // OUT : 2400-style HHMM
        unsigned short   &laneHCL,        // OUT : high-ceiling metres
        unsigned short   &laneLCL         // OUT : low-ceiling metres
    );

//------------------------------------------------------------
// Writes a brand-new sailing record.
// Preconditions : data validated.
// Postconditions: record appended to Sailings.dat.
    static void writeSailing(
        SailingID          sailingId,     // IN  : generated PK
        const std::string &vesselName,    // IN  : vessel assigned
        const std::string &arrivalCity,   // IN  : destination port
        const std::string &dateISO,       // IN  : YYYY-MM-DD
        const std::string &timeHHMM,      // IN  : HHMM
        unsigned short     laneHCL,       // IN  : high-ceiling metres
        unsigned short     laneLCL        // IN  : low-ceiling metres
    );

//------------------------------------------------------------
// Removes sailing and cascades reservation deletes.
// Preconditions : sailing exists.
// Returns      : true on success, false if not found.
    static bool deleteSailing(
        SailingID          sailingId      // IN  : PK to delete
    );
};

#endif // FILEIO_SAILINGS_H
