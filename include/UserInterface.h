//************************************************************
//************************************************************
//  UserInterface.h
//  CMPT 276 – Assignment 4 (Fahad Y)
//
//  Declares the UserInterface class, which manages all menu
//  interactions and user inputs for the ferry reservation system.
//
//  • Provides main menu and submenus (vessels, sailings, reservations)
//  • Handles user input with cancellation support (0 = cancel)
//  • Includes input validation for codes, dimensions, and IDs
//************************************************************
//************************************************************

#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <string>

class UserInterface
{
public:
    // Initialize user interface (called at program start)
    static void initialize();

    // Run the main menu loop until user exits
    static void runMainMenu();

    // Clean up resources before program exit
    static void shutdown();

private:
    // Submenu handlers
    static void vesselMenu();
    static void sailingMenu();
    static void reservationMenu();
    static void printSailingReport();

    // Yes/No prompt with 0 = cancel
    static bool promptYesNo(const std::string &message);

    // Clear input buffer (for invalid input recovery)
    static void clearInput();

    // Input helpers (all support 0 = cancel)
    static std::string getVesselName();
    static unsigned int getLaneLength(const std::string &label);
    static std::string getCityCode();
    static std::string getDate();
    static std::string getTime();
    static std::string getSailingID();
    static std::string getPhoneNumber();
    static std::string getLicensePlate();
    static unsigned int getDimension(const std::string &label);
};

#endif // USERINTERFACE_H
