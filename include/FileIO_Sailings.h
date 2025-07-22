// ---------------------------------------------------------------------------
// FileIO_Sailings.h
// CMPT 276 – Assignment 4 (Fahad T9)
// 
// identifies functions for the sailings file as well as expected input and outputs of these fucnitons
//
// Variables needed in file taken from ORD in Assignment 1
#ifndef FILEIO_SAILINGS_H
#define FILEIO_SAILINGS_H
#pragma once
#include <fstream>
#include <string>
#include "CommonTypes.h"

struct Sailingrec{
    SailingID id;
    char VesselName[25];
    unsigned short remainingHCL;
    unsigned short remainingLCL;
};

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
        unsigned short   &remainingHCL,        // OUT : high-ceiling metres
        unsigned short   &remainingLCL         // OUT : low-ceiling metres
    );

//------------------------------------------------------------
// Writes a brand-new sailing record.
// Preconditions : data validated.
// Postconditions: record appended to Sailings.dat.
    static void writeSailing(
        SailingID          sailingId,     // IN  : generated PK
        const std::string &vesselName,    // IN  : vessel assigned
        unsigned short     remainingHCL,       // IN  : high-ceiling metres
        unsigned short     remainingLCL        // IN  : low-ceiling metres
    );

//------------------------------------------------------------
// Removes sailing and cascades reservation deletes.
// Preconditions : sailing exists.
// Returns      : true on success, false if not found.
    static bool deleteSailing(
        SailingID          sailingId      // IN  : PK to delete
    );
 
//------------------------------------------------------------    
// Finds a sailing in the binary file for making or deleting a reservation 
// Precondition: sailing exists
// Returns : Sailing Data for manipulation

   static bool findSailing(
        SailingID          sailingId,      // IN  : PK to find the record
        Sailingrec         &result         // OUT : record of the sailing attributes
    );


//------------------------------------------------------------    
// Finds a sailing in the binary file and outputs its status 
// Precondition: sailing exists
// Returns : Sailing attributes

   static bool sailingstatus(
        SailingID          sailingId      // IN  : PK to find the record
    );
//------------------------------------------------------------
// Lists all the sailings int he sailing in a sort of report format
// Precondition: sailing exists
// Returns : Sailing Data in a report format

    static void Sailingreport(
        // Lists all sailings so not a single one output or input
    );

 //------------------------------------------------------------
// Finds if a sailing exists 
// Precondition: 
// Returns : true or fals if sailing is found

    static bool Sailingexist(
        SailingID          sailingId     // IN  : PK to find the record
    );
   

};

#endif // FILEIO_SAILINGS_H
