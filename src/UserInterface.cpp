#include <iostream>
#include <string>
#include "UserInterface.h"
#include "Vessel.h"
#include "Sailing.h"
#include "Reservation.h"
#include "FileIO_Sailings.h"
#include "FileIO_Reservations.h"
#include "FileIO_VehicleRecord.h"

// ---------------- INITIALIZATION ----------------
void UserInterface::initialize() {
    std::cout << "User Interface Initialized.\n";
}

// ---------------- MAIN MENU LOOP ----------------
void UserInterface::runMainMenu() {
    int choice;
    do {
        std::cout << "\n--- MAIN MENU ---\n";
        std::cout << "1. Vessel Maintenance\n";
        std::cout << "2. Sailing Maintenance\n";
        std::cout << "3. Reservations\n";
        std::cout << "4. Exit\n";
        std::cout << "Enter choice: ";
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
            case 1: vesselMenu(); break;
            case 2: sailingMenu(); break;
            case 3: reservationMenu(); break;
            case 4: std::cout << "Exiting...\n"; break;
            default: std::cout << "Invalid choice.\n";
        }
    } while (choice != 4);
}

// ---------------- VESSEL MENU ----------------
void UserInterface::vesselMenu() {
    int choice;
    do {
        std::cout << "\n--- Vessel Maintenance ---\n";
        std::cout << "1. Create New Vessel\n";
        std::cout << "2. Delete Existing Vessel\n";
        std::cout << "3. Back to Main Menu\n";
        std::cout << "Enter choice: ";
        std::cin >> choice;
        std::cin.ignore();

        if (choice == 1) {
            std::string name;
            unsigned int hcl, lcl;

            std::cout << "Vessel Name (max 25 chars): ";
            std::getline(std::cin, name);
            std::cout << "High Ceiling Lane Length (m): ";
            std::cin >> hcl;
            std::cout << "Low Ceiling Lane Length (m): ";
            std::cin >> lcl;
            std::cin.ignore();

            VesselStatus status = Vessel::CreateVessel(name, hcl, lcl);
            if (status == VesselStatus::SUCCESS)
                std::cout << "Vessel successfully added.\n";
            else if (status == VesselStatus::ALREADY_EXISTS)
                std::cout << "The vessel already exists.\n";
        }
        else if (choice == 2) {
            std::string name;
            std::cout << "Vessel Name to delete: ";
            std::getline(std::cin, name);

            VesselStatus status = Vessel::DeleteVessel(name);
            if (status == VesselStatus::SUCCESS)
                std::cout << "Vessel successfully deleted.\n";
            else
                std::cout << "The vessel doesn’t exist.\n";
        }
    } while (choice != 3);
}

// ---------------- SAILING MENU ----------------
void UserInterface::sailingMenu() {
    int choice;
    do {
        std::cout << "\n--- Sailing Maintenance ---\n";
        std::cout << "1. Create New Sailing\n";
        std::cout << "2. Delete Existing Sailing\n";
        std::cout << "3. Query Sailing Status\n";
        std::cout << "4. Print Sailing Report\n";
        std::cout << "5. Back to Main Menu\n";
        std::cout << "Enter choice: ";
        std::cin >> choice;
        std::cin.ignore();

        if (choice == 1) {
            std::string city, vessel, date, time;
            std::cout << "City (3-letter code): ";
            std::cin >> city;
            std::cin.ignore();
            std::cout << "Vessel Name: ";
            std::getline(std::cin, vessel);
            std::cout << "Date (YY-MM-DD): ";
            std::cin >> date;
            std::cout << "Time (HHMM): ";
            std::cin >> time;

            SailingStatus status = Sailing::CreateSailing(city, vessel, date, time);
            if (status == SailingStatus::SUCCESS) {
                // Build SailingID = city + day + hour
                std::string sailingID = city + ":" + date.substr(6, 2) + ":" + time.substr(0, 2);
                std::cout << "Sailing successfully created. The sailing ID is: " << sailingID << "\n";
            }
            else if (status == SailingStatus::VESSEL_NOT_FOUND) {
                std::cout << "Error: Vessel doesn’t exist.\n";
            }
            else if (status == SailingStatus::SAILING_ALREADY_EXISTS) {
                std::cout << "Error: Sailing already exists.\n";
            }
        }
        else if (choice == 2) {
            std::string sailingID;
            std::cout << "Enter Sailing ID (ttt-dd-hh): ";
            std::cin >> sailingID;
            if (Sailing::DeleteSailing(sailingID))
                std::cout << "Sailing successfully deleted.\n";
            else
                std::cout << "Sailing ID doesn’t exist.\n";
        }
        else if (choice == 3) {
            std::string sailingID;
            std::cout << "Enter Sailing ID (ttt-dd-hh): ";
            std::cin >> sailingID;
            if (!Sailing::printStatus(sailingID))
                std::cout << "Sailing ID doesn’t exist.\n";
        }
        else if (choice == 4) {
            Sailing::printReport();
        }
    } while (choice != 5);
}

// ---------------- RESERVATION MENU ----------------
void UserInterface::reservationMenu() {
    int choice;
    do {
        std::cout << "\n--- Reservations ---\n";
        std::cout << "1. Create Reservation\n";
        std::cout << "2. Delete Reservation\n";
        std::cout << "3. Check-In Vehicles\n";
        std::cout << "4. Back to Main Menu\n";
        std::cout << "Enter choice: ";
        std::cin >> choice;
        std::cin.ignore();

        if (choice == 1) {
            std::cout << "1. New Customer\n2. Returning Customer\n";
            int sub; std::cin >> sub; std::cin.ignore();

            if (sub == 1) {
                // New customer
                FerrySys::VehicleRecord vehicle;
                std::string sailingID;
                std::cout << "Phone (999-999-9999): "; std::cin >> vehicle.phone;
                std::cout << "License Plate (max 10): "; std::cin >> vehicle.license;
                std::cout << "Length (cm): "; std::cin >> vehicle.length_cm;
                std::cout << "Height (cm): "; std::cin >> vehicle.height_cm;
                std::cout << "Enter Sailing ID: "; std::cin >> sailingID;

                if (Reservation::newCustomerReservation(vehicle, sailingID))
                    std::cout << "Reservation successfully made.\n";
                else
                    std::cout << "Failed to make reservation (check sailing or space).\n";
            }
            else if (sub == 2) {
                // Returning customer
                std::string license, sailingID;
                std::cout << "License Plate: "; std::cin >> license;
                std::cout << "Sailing ID: "; std::cin >> sailingID;

                if (Reservation::returningCustomerReservation(license, sailingID))
                    std::cout << "Reservation successfully made.\n";
                else
                    std::cout << "Failed to make reservation (check vehicle or sailing).\n";
            }
        }
        else if (choice == 2) {
            std::string license, sailingID;
            std::cout << "Enter Sailing ID: "; std::cin >> sailingID;
            std::cout << "Enter License Plate: "; std::cin >> license;

            if (Reservation::deleteReservation(license, sailingID))
                std::cout << "Reservation successfully deleted.\n";
            else
                std::cout << "Reservation not found.\n";
        }
        else if (choice == 3) {
            std::string license, sailingID;
            std::cout << "Enter Sailing ID: "; std::cin >> sailingID;
            std::cout << "Enter License Plate: "; std::cin >> license;

            if (Reservation::checkinVehicle(license, sailingID))
                std::cout << "Vehicle successfully checked-in.\n";
            else
                std::cout << "Check-in failed (reservation not found).\n";
        }
    } while (choice != 4);
}

// ---------------- SHUTDOWN ----------------
void UserInterface::shutdown() {
    std::cout << "User Interface Shutdown.\n";
}
