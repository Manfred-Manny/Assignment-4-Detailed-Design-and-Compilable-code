// ---------------------------------------------------------------------------
// FileIO_Sailing.cpp
// CMPT 276 – Assignment 4 (Fahad T9)
// 
// Implements binary file for Sailings manipulation functions
// Functions already defined in the FileIO_Sailings.h

#pragma once
#include "FileIO_Sailings.h"
#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <FileIO_Vessel.h>
#include <iomanip>


static std::fstream f;
void FileIO_Sailings::reset()
{
    if (f.is_open()) f.close();
    f.open("sailings.dat", std::ios::binary);
}
// ---------------------------------------------------------------------------
bool FileIO_Sailings::getNextSailing(SailingID &sailingId,std::string &vesselName, unsigned short &remainingHCL,unsigned short &remainingLCL)
{
    if (!f.is_open()){
        return false;
    }

    Sailingrec rec;
   if (f.read(reinterpret_cast<char*>(&rec), sizeof(rec))) {
        sailingId = rec.id;
        vesselName = rec.VesselName; // C-string to std::string
        remainingHCL = rec.remainingHCL;
        remainingLCL = rec.remainingLCL;
        return true;  // successfully read one record
    }
    return false; // reached end of file or error
}
// ---------------------------------------------------------------------------
void FileIO_Sailings::writeSailing(SailingID sailingID,const std::string &vesselName, unsigned short remainingHCL,unsigned short remainingLCL)
{
    std::fstream f("sailings.dat" , std::ios::app | std::ios::binary);
    Sailingrec rec;
    rec.id = sailingID;
    std::strcpy(rec.VesselName, vesselName.c_str());
    rec.VesselName[sizeof(rec.VesselName) - 1] = '\0'; 
    rec.remainingHCL = remainingHCL;
    rec.remainingLCL = remainingLCL;
    if(f){
       f.write(reinterpret_cast<const char*> (&rec), sizeof(Sailingrec));
       f.close();
    }
    else {
        std::cout << "Error opening sailings.dat!\n";
    }
}
// ---------------------------------------------------------------------------
bool FileIO_Sailings::findSailing(SailingID sailingID, Sailingrec &result)
{
    std::fstream f("sailings.dat" , std::ios::binary);
    Sailingrec rec;
    while (f.read(reinterpret_cast<char*>(&rec), sizeof(rec))){
        if(f){
            if(rec.id == sailingID){
            result = rec;
            return true;
            }
        }
        else{
            std::cout << "Error opening sailings.dat!\n";
        }
    }
    return false;
}
// ---------------------------------------------------------------------------
bool FileIO_Sailings::deleteSailing(SailingID sailingIDtoDelete)
{
    std::fstream f("sailings.dat" ,std::ios::binary);
    std::fstream f2("temp.dat" , std::ios::binary);
    Sailingrec rec;
    bool found = false;
    while(f.read(reinterpret_cast<char*>(&rec), sizeof(rec))){
        if(rec.id != sailingIDtoDelete){
            found == true;
        }
        else{
            f2.write(reinterpret_cast<char*>(&rec) , sizeof(rec));
        }
    }
    f.close();
    f2.close();
    std::remove("sailings.dat");
    std::rename("temp.dat" , "sailings.dat");


    return found;
}
// ---------------------------------------------------------------------------
void FileIO_Sailings::Sailingreport()
{
    std::fstream f("sailings.dat" , std::ios::binary);
    Sailingrec rec;
    std::cout << "--------Sailing Report----------\n";
    std::cout << std::left << std::setw(10) << "Sailing ID" << "| " << std::setw(20) <<
     "Vessel Name" << " | " << std::setw(16) << "Remaining HCL" << " |" << std::setw(14) <<
     "Remaining LCL" << " \n";
     std::cout << std::string(10 + 3 + 20 + 3 + 16 + 3 + 14, '-') << "\n";
    while(f.read(reinterpret_cast<char*>(&rec) , sizeof(rec))){
        std::cout << std::left << std::setw(10) << rec.id << "| " << std::setw(20) <<
      rec.VesselName << " | " << std::setw(16) << rec.remainingHCL << " |" << std::setw(14) <<
      rec.remainingLCL << " \n";
    }
    f.close();
}
// ---------------------------------------------------------------------------

bool FileIO_Sailings::Sailingexist(SailingID sailingID)
{
    std::fstream f("sailings.dat" , std::ios::binary);
    Sailingrec rec;
    while (f.read(reinterpret_cast<char*>(&rec), sizeof(rec))){
        if(f){
            if(rec.id == sailingID){
            return true;
            }
    }
   }
  return false;
}

// ---------------------------------------------------------------------------
bool FileIO_Sailings::sailingstatus(SailingID sailingID)
{
    std::fstream f("sailings.dat" , std::ios::binary);
    if (!f) {
        std::cerr << "Error opening sailings.dat!\n";
        return false;
    }
    
    Sailingrec rec;
    
    while (f.read(reinterpret_cast<char*>(&rec), sizeof(rec))){
            if(rec.id == sailingID){
            std::cout << "--------Sailing Report----------\n";
            std::cout << std::left << std::setw(10) << "Sailing ID" << "| " << std::setw(20) <<
               "Vessel Name" << " | " << std::setw(16) << "Remaining HCL" << " |" << std::setw(14) <<
                "Remaining LCL" << " \n";


           std::cout << std::string(10 + 3 + 20 + 3 + 16 + 3 + 14, '-') << "\n";

           std::cout << std::left << std::setw(10) << rec.id << "| " << std::setw(20) <<
              rec.VesselName << " | " << std::setw(16) << rec.remainingHCL << " |" << std::setw(14) <<
              rec.remainingLCL << " \n";
            return true;
            }
        }
    return false;
}

