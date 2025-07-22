#include "Sailing.h"
#include "FileIO_Vessel.h"
#include <FileIO_Sailings.h>
#include<string>
#include <iostream>

// ---------------------------------------------------------------------------
bool Sailing::CreateSailing(std::string Arrivalcity, std::string VesselName, std::string Date,  std::string Time)
{
    std::cout << "What is the City of Arrival ( 3 letter abbreviation): \n";
    std::getline(std::cin , Arrivalcity);
    
    std::cout << "Vessel Name (Length: 25 max.): \n";
    std::getline(std::cin , VesselName);

  /*   if (!FileIO_Vessel::vesselExists(vesselName)) {
        std::cout << "The vessel doesn’t exist. Do you wish to go back to the form (Y/N)? ";
        char choice;
        std::cin >> choice;
        std::cin.ignore();  // Clear newline
        return (choice == 'Y' || choice == 'y') ? CreateSailing() : false;
    } */   // validation check after the vessel file is done

    std::cout << "Date of Departure ( YY-MM-DD format): \n";
    std::getline(std::cin , Date);

    std::cout << "Time of Departure ( 24hour Clock): \n";
    std::getline(std::cin , Time);
 
    std::string SailingID = Arrivalcity + ":" + Date.substr(2) + ":" + Time ;

    if (FileIO_Sailings::Sailingexist(SailingID)) {
        std::cout << "Sailing already exists at that time. Do you wish to change the time (Y/N)? ";
        char choice;
        std::cin >> choice;
        std::cin.ignore();  // Clear newline
        return (choice == 'Y' || choice == 'y') ? CreateSailing(Arrivalcity, VesselName, Date, Time) : false;
    }

    std::cout << "Create Sailing (Y/N)? ";
    char confirm;
    std::cin >> confirm;
    std::cin.ignore();  // Clear newline

    if (confirm == 'Y' || confirm == 'y') {
        // Write the record with placeholder HCL/LCL values for now will need to get these values from the vessel file
        FileIO_Sailings::writeSailing(SailingID, VesselName, 100, 100);
        std::cout << "Sailing successfully created. The Sailing ID is " << SailingID << "\n";
        return true;
    } else {
        std::cout << "Sailing creation aborted.\n";
        return false;
    }
}
// ---------------------------------------------------------------------------
bool Sailing::DeleteSailing(SailingID sailingID)
{
    std::cout << "What is the Sailing ID(format of ttt-dd-hh): \n";
    std::getline(std::cin , sailingID);

     if (!FileIO_Sailings::Sailingexist(sailingID)) {
        std::cout << "Sailing ID doesn't exists. Do you wish to go back to the form (Y/N)? ";
        char choice;
        std::cin >> choice;
        std::cin.ignore();  // Clear newline
        return (choice == 'Y' || choice == 'y') ? DeleteSailing(sailingID) : false;
    }

    std::cout << "Confirm Delete Sailing (Y/N)? ";
    char confirm;
    std::cin >> confirm;
    std::cin.ignore();  // Clear newline

    if (confirm == 'Y' || confirm == 'y') {
        FileIO_Sailings::deleteSailing(sailingID);
        std::cout << "The Sailing is successfully deleted.\n";
        return true;
        } else {
        std::cout << "Sailing deletion aborted.\n";
        return false;
    }

}
// ---------------------------------------------------------------------------
void Sailing::printStatus(SailingID sailingID)
{
    std::cout << "What is the Sailing ID(format of ttt-dd-hh): \n";
    std::getline(std::cin , sailingID);

     if (!FileIO_Sailings::Sailingexist(sailingID)) {
        std::cout << "Sailing ID doesn't exists. Do you wish to go back to the form (Y/N)? ";
        char choice;
        std::cin >> choice;
        std::cin.ignore();  // Clear newline
        if (choice == 'Y' || choice == 'y') {
            printStatus(sailingID);
    }
    else{
        FileIO_Sailings::sailingstatus(sailingID);
    }
     }
    }
// ---------------------------------------------------------------------------
void Sailing::printReport()
{
    FileIO_Sailings::Sailingreport();
}


void Sailing::initialize()
{
    std::cout << "[STUB] Sailing::initialize()\n";
}

void Sailing::shutdown()
{
    std::cout << "[STUB] Sailing::shutdown()\n";
}
