#include <iostream>
#include <string>
#include <limits>
#include "UserInterface.h"
#include "Vessel.h"
#include "Sailing.h"
#include "Reservation.h"
#include "FileIO_Sailings.h"
#include "FileIO_Reservations.h"
#include "FileIO_VehicleRecord.h"
#include <cctype>   
#include <algorithm> 

//=====================================================
// HELPER FUNCTIONS
//=====================================================
void UserInterface::clearInput() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

bool UserInterface::promptYesNo(const std::string &message) {
    std::string input;
    while (true) {
        std::cout << message << " [Y/N or 0 = Cancel]: ";
        std::getline(std::cin, input);

        if (input == "0") return false;
        if (input == "Y" || input == "y") return true;
        if (input == "N" || input == "n") return false;

        std::cout << "Invalid input. Enter Y, N or 0.\n";
    }
}

std::string UserInterface::getVesselName() {
    std::string name;
    while (true) {
        std::cout << "Vessel Name (max 25 chars) [0 = Cancel]: ";
        std::getline(std::cin, name);

        if (name == "0") return "";
        if (!name.empty() && name.length() <= 25) return name;

        std::cout << "Error: Must be 1-25 characters.\n";
    }
}

unsigned int UserInterface::getLaneLength(const std::string &label) {
    std::string input;
    while (true) {
        std::cout << label << " (4 digits max) [0 = Cancel]: ";
        std::getline(std::cin, input);

        if (input == "0") return 0;
        if (!input.empty() && input.find_first_not_of("0123456789") == std::string::npos) {
            unsigned int value = std::stoi(input);
            if (value <= 9999) return value;
        }
        std::cout << "Error: Enter number up to 4 digits.\n";
    }
}

std::string UserInterface::getCityCode() {
    std::string city;
    while (true) {
        std::cout << "What is the City of Arrival (3 letter abbreviation) [0 = Cancel]: ";
        std::getline(std::cin, city);
        if (city == "0") return "";
        if (city.length() == 3) return city;
        std::cout << "Error: Must be 3 letters.\n";
    }
}



std::string UserInterface::getDate() {
    std::string date;

    while (true) {
        std::cout << "Date of Departure (YY-MM-DD format) [0 = Cancel]: ";
        std::getline(std::cin, date);

        // Remove all spaces
        date.erase(std::remove_if(date.begin(), date.end(), ::isspace), date.end());

        // Cancel
        if (date == "0")
            return "";

        // Validate format YY-MM-DD
        if (date.length() == 8 && date[2] == '-' && date[5] == '-') {
            // Ensure digits are in correct positions
            if (isdigit(date[0]) && isdigit(date[1]) &&
                isdigit(date[3]) && isdigit(date[4]) &&
                isdigit(date[6]) && isdigit(date[7])) {
                return date; // valid date
            }
        }

        std::cout << "Error: Must be in YY-MM-DD format (e.g., 25-07-30).\n";
    }
}


std::string UserInterface::getTime() {
    std::string time;
    while (true) {
        std::cout << "Time of Departure (HHMM 24hr Clock) [0 = Cancel]: ";
        std::getline(std::cin, time);

        time.erase(std::remove_if(time.begin(), time.end(), ::isspace), time.end());

        if (time == "0")
            return "";

        if (time.length() == 4 &&
            std::all_of(time.begin(), time.end(), ::isdigit)) {
            return time;
        }

        std::cout << "Error: Must be 4-digit 24hr time (e.g., 0930).\n";
    }
}


std::string UserInterface::getSailingID() {
    std::string id;
    while (true) {
        std::cout << "Sailing ID (ttt:dd:hh) [0 = Cancel]: ";
        std::getline(std::cin, id);
        if (id == "0") return "";
        if (id.length() == 9 && id[3] == ':' && id[6] == ':') return id;
        std::cout << "Error: Must be in ttt:dd:hh format.\n";
    }
}

std::string UserInterface::getPhoneNumber() {
    std::string phone;
    while (true) {
        std::cout << "Phone (999-999-9999) [0 = Cancel]: ";
        std::getline(std::cin, phone);
        if (phone == "0") return "";
        if (phone.length() == 12 && phone[3] == '-' && phone[7] == '-') return phone;
        std::cout << "Error: Must be in 999-999-9999 format.\n";
    }
}

std::string UserInterface::getLicensePlate() {
    std::string plate;
    while (true) {
        std::cout << "License Plate (max 10) [0 = Cancel]: ";
        std::getline(std::cin, plate);
        if (plate == "0") return "";
        if (!plate.empty() && plate.length() <= 10) return plate;
        std::cout << "Error: Must be 1-10 characters.\n";
    }
}

unsigned int UserInterface::getDimension(const std::string &label) {
    std::string input;
    while (true) {
        std::cout << label << " (cm) [0 = Cancel]: ";
        std::getline(std::cin, input);
        if (input == "0") return 0;
        if (!input.empty() && input.find_first_not_of("0123456789") == std::string::npos)
            return std::stoi(input);
        std::cout << "Error: Enter numeric value.\n";
    }
}

//=====================================================
// INITIALIZATION
//=====================================================
void UserInterface::initialize() {
    std::cout << "User Interface Initialized.\n";
}

//=====================================================
// MAIN MENU
//=====================================================
void UserInterface::runMainMenu() {
    while (true) {
        std::cout << "\n--- MAIN MENU ---\n"
                  << "1) Vessel Maintenance\n"
                  << "2) Sailing Maintenance\n"
                  << "3) Reservations\n"
                  << "4) Report Generation\n"
                  << "0) Exit\n"
                  << "Select one of the numbers above (0-4): ";

        int choice;
        std::cin >> choice;
        clearInput();

        switch (choice) {
            case 0:
                std::cout << "Thank you for using the Ferry Reservation System.\n";
                return;
            case 1: vesselMenu(); break;
            case 2: sailingMenu(); break;
            case 3: reservationMenu(); break;
            case 4: Sailing::printReport(); break;
            default:
                std::cout << "Invalid selection. Try again.\n";
        }
    }
}

//=====================================================
// VESSEL MENU
//=====================================================
void UserInterface::vesselMenu() {
    while (true) {
        std::cout << "\n--- Vessel Maintenance ---\n"
                  << "1) Create New Vessel\n"
                  << "2) Delete Existing Vessel\n"
                  << "0) Back to Main Menu\n"
                  << "Select one of the numbers above (0-2): ";

        int choice;
        std::cin >> choice;
        clearInput();

        if (choice == 0) break;

        else if (choice == 1) {
            while (true) {
                std::string name = getVesselName();
                if (name.empty()) break;

                unsigned int hcl = getLaneLength("High Ceiling Lane Length in metres");
                if (hcl == 0) break;

                unsigned int lcl = getLaneLength("Low Ceiling Lane Length in metres");
                if (lcl == 0) break;

                if (!promptYesNo("Create Vessel Record (Y/N)?")) break;

                VesselStatus status = Vessel::CreateVessel(name, hcl, lcl);

                if (status == VesselStatus::SUCCESS) {
                    std::cout << "Vessel successfully added.\n";
                    break;
                } else if (status == VesselStatus::ALREADY_EXISTS) {
                    if (!promptYesNo("The Vessel already exists. Do you wish to go back to the form (Y/N)?")) break;
                }
            }
        }

        else if (choice == 2) {
            while (true) {
                std::string name = getVesselName();
                if (name.empty()) break;

                if (!promptYesNo("Are you sure you want to delete this vessel (Y/N)?")) break;

                VesselStatus status = Vessel::DeleteVessel(name);

                if (status == VesselStatus::SUCCESS) {
                    std::cout << "Vessel successfully deleted.\n";
                    break;
                } else {
                    if (!promptYesNo("The vessel does not exist. Do you wish to go back to the form (Y/N)?")) break;
                }
            }
        }
        else std::cout << "Invalid selection.\n";
    }
}

//=====================================================
// SAILING MENU
//=====================================================
void UserInterface::sailingMenu() {
    while (true) {
        std::cout << "\n--- Sailing Maintenance ---\n"
                  << "1) Create New Sailing\n"
                  << "2) Delete Existing Sailing\n"
                  << "3) Query Sailing Status\n"
                  << "0) Back to Main Menu\n"
                  << "Select one of the numbers above (0-3): ";

        int choice;
        std::cin >> choice;
        clearInput();

        if (choice == 0) break;

        else if (choice == 1) {
            while (true) {
                std::string city = getCityCode();
                if (city.empty()) break;

                std::string vessel = getVesselName();
                if (vessel.empty()) break;

                std::string date = getDate();
                if (date.empty()) break;

                std::string time = getTime();
                if (time.empty()) break;

                if (!promptYesNo("Create Sailing (Y/N)?")) break;

                SailingStatus status = Sailing::CreateSailing(city, vessel, date, time);
                if (status == SailingStatus::SUCCESS) {
                    // Validate lengths
                    if (date.size() >= 8 && time.size() >= 2) {
                        std::cout << "Sailing successfully created. The Sailing ID is " << city << ":" << date.substr(6,2) << ":" << time.substr(0,2) << "\n";
                    }
                    else {
                         std::cout << "Sailing successfully created, but could not generate a valid Sailing ID.\n";
                    }
                    break;
                    }
                     else if (status == SailingStatus::VESSEL_NOT_FOUND) {
                        if (!promptYesNo("Vessel does not exist. Do you wish to go back to the form (Y/N)?")) break;
                    }
                     else if (status == SailingStatus::SAILING_ALREADY_EXISTS) {
                      if (!promptYesNo("Sailing already exists at that time. Do you wish to change the time (Y/N)?")) break;
                    }
            }
        }

        else if (choice == 2) {
            while (true) {
                std::string id = getSailingID();
                if (id.empty()) break;

                if (!promptYesNo("Confirm Delete Sailing (Y/N)?")) break;

                if (Sailing::DeleteSailing(id))
                    std::cout << "Sailing successfully deleted.\n";
                else {
                    if (!promptYesNo("Sailing ID does not exist. Go back to form (Y/N)?")) break;
                }
            }
        }

        else if (choice == 3) {
            while (true) {
                std::string id = getSailingID();
                if (id.empty()) break;

                if (!Sailing::printStatus(id))
                    if (!promptYesNo("Sailing ID does not exist. Do you wish to go back to the form (Y/N)?")) break;

                if (!promptYesNo("Check another Sailing (Y/N)?")) break;
            }
        }

        else std::cout << "Invalid selection.\n";
    }
}

//=====================================================
// RESERVATION MENU
//=====================================================
void UserInterface::reservationMenu() {
    while (true) {
        std::cout << "\n--- Reservations ---\n"
                  << "1) Create Reservation\n"
                  << "2) Delete Reservation\n"
                  << "3) Check-In Vehicles\n"
                  << "0) Back to Main Menu\n"
                  << "Select one of the numbers above (0-3): ";

        int choice;
        std::cin >> choice;
        clearInput();

        if (choice == 0) break;

        // CREATE RESERVATION
        else if (choice == 1) {
            std::cout << "1) New Customer\n2) Returning Customer\n0) Cancel\nSelect: ";
            int sub; std::cin >> sub; clearInput();

            if (sub == 0) continue;

            // NEW CUSTOMER
            if (sub == 1) {
                FerrySys::VehicleRecord vehicle;
                std::string sailingID;

                vehicle.phone = getPhoneNumber();
                if (vehicle.phone.empty()) continue;

                vehicle.license = getLicensePlate();
                if (vehicle.license.empty()) continue;

                vehicle.height_cm = getDimension("Vehicle Height");
                if (vehicle.height_cm == 0) continue;

                vehicle.length_cm = getDimension("Vehicle Length");
                if (vehicle.length_cm == 0) continue;

                sailingID = getSailingID();
                if (sailingID.empty()) continue;

                if (!promptYesNo("Make Reservation (Y/N)?")) continue;

                if (Reservation::newCustomerReservation(vehicle, sailingID))
                    std::cout << "Reservation successfully made.\n";
                else
                    std::cout << "Failed to make reservation.\n";
            }

            // RETURNING CUSTOMER
            else if (sub == 2) {
                std::string license = getLicensePlate();
                if (license.empty()) continue;

                std::string sailingID = getSailingID();
                if (sailingID.empty()) continue;

                if (!promptYesNo("Make Reservation (Y/N)?")) continue;

                if (Reservation::returningCustomerReservation(license, sailingID))
                    std::cout << "Reservation successfully made.\n";
                else
                    std::cout << "Failed to make reservation.\n";
            }
        }

        // DELETE RESERVATION
        else if (choice == 2) {
            std::string sailingID = getSailingID();
            if (sailingID.empty()) continue;

            std::string license = getLicensePlate();
            if (license.empty()) continue;

            if (!promptYesNo("Delete Reservation (Y/N)?")) continue;

            if (Reservation::deleteReservation(license, sailingID))
                std::cout << "Reservation successfully deleted.\n";
            else
                if (!promptYesNo("Reservation not found. Go back to form (Y/N)?")) continue;
        }

        // CHECK-IN VEHICLES
        else if (choice == 3) {
            while (true) {
                std::string sailingID = getSailingID();
                if (sailingID.empty()) break;

                std::string license = getLicensePlate();
                if (license.empty()) break;

                if (!promptYesNo("Check-In Vehicle (Y/N)?")) break;

                if (Reservation::checkinVehicle(license, sailingID))
                    std::cout << "Vehicle successfully checked-in.\n";
                else {
                    std::cout << "Check-in failed (reservation not found).\n";
                    if (!promptYesNo("Do you wish to modify details (Y/N)?")) break;
                }

                if (!promptYesNo("Continue check-in (Y/N)?")) break;
            }
        }
    }
}

//=====================================================
// SHUTDOWN
//=====================================================
void UserInterface::shutdown() {
    std::cout << "User Interface Shutdown.\n";
}