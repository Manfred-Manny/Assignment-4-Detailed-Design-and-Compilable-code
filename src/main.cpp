/*******************************************************************************************
 * =========================================================================================
 * SeaLink Technologies Co.
 * Automobile Ferry Reservation System - Release 3
 *
 * CMPT 276 - Summer 2025 - Coding Conventions
 *
 * Revision History:
 * Rev. 1 - 2025/07/09 - Original version by Darko Reljic
 * Future changes must be updated here with a description of what was updated and why.
 *
 * -----------------------------------------------------------------------------------------
 * PURPOSE:
 *   This coding convention was created to ensure consistent commenting improve readability and to improve
 *   programmer understanding whether or not that programmer was the original author.
 *
 * -----------------------------------------------------------------------------------------
 * GENERAL CODING RULES:
 *
 * 1) FILE HEADERS:
 *    - Each .cpp or .h file will include the coding convention at the beginning of the document
 *      for easy reference and consistentcy
 * 
 *    - Additionaly each file after the coding convention section should follow with a section containing the following
 *      Filename
 *      Company Name
 *      Project Name
 *      Release #, Members involved
 *      Explanation of file purpose
 * 
 *    - Inclusion Gaurds, #include statements should follow
 *
 * 2) DIVIDER LINES:
 *    - Major sections are separated by double divider lines from stars
 *          ************************************************************
 *          ************************************************************
 * 
 *    - Each function signature in header files will be divided with place a single divider line from dashes
 *          //------------------------------------------------------------
 *
 * 3) FUNCTION COMMENTS & SIGNATURES:
 *
 *    - Each function prototype in a header file (.h) must be immediately
 *      preceded by a thin horizontal divider line.
 *      Example: ------------------------------------------------------------
 *
 *    - The function signature must come directly after the divider.
 *      The opening brace { for the function body goes on the same line
 *      as the signature, if possible.
 *
 *    - The function block comment goes ABOVE the function prototype
 *
 *    - Each function parameter is written on its own line.
 *      Each line must include:
 *          - The parameter name
 *          - The parameter type
 *          - The parameter direction:
 *              - IN    : input only
 *              - OUT   : output only
 *              - IN/OUT: input & output
 *          - Units, if relevant (ex, meters, seconds).
 *
 *    - Example Function Prototype:
 *
 *    //------------------------------------------------------------
 *    // Creates a new vessel record if it does not already exist.
 *    // Preconditions : vesselName must be unique.
 *    // Postconditions: record saved to vessel file.
 *    void createVessel(
 *        const std::string& vesselName,  // IN : name of vessel
 *        int hclLength,                  // IN : high ceiling lane length (meters)
 *        int lclLength                   // IN : low ceiling lane length (meters)
 *    );
 *
 * 4) NAMING:
 *    - Files: PascalCase (ex, CruiseShips)
 *    - Classes and types: PascalCase (ex, ReservationClass).
 *    - Variables and functions: camelCase (ex createReservation()).
 *    - Constants: UPPER_CASE_WITH_UNDERSCORES.
 *
 * 5) INDENTATION & SPACING:
 *    - Indent by 4 spaces, avoiding tabs as they are different sizes on different environments
 *    - Opening brace �{� on its own line (Allman style).
 *    - One space before and after operators (=, +, <, etc).
 *    - One space after commas in parameter lists.
 *
 * 6) ORDERING OF CLASS MEMBERS:
 *    - Order: public first, protected next, private last.
 *    - Arrange so that exported functions appear first.
 *
 * 7) FILE I/O:
 *    - Always check file open success.
 *    - Always close files in shutdown.
 *
 * 8) STUB FUNCTIONS:
 *    - Must match the system�s Use Cases in the design document.
 *    - All stubs include full signature, divider lines, and placeholder body.
 *
 * 9) JOINT FUNCTIONALITY STATEMENT:
 *    - Each module comment must state why its responsibilities belong together.
 *    - Example, the ReservationClass handles all reservation records and nothing else.
 *
 * 10) VERSIONING:
 *    - Each file�s revision block is always at the top.
 *    - Newest changes listed first. Old revisions always added below.
 *
 * 11) UNIFORMITY:
 *    - This coding convention applies to every .cpp and .h file in this project.
 *    - Group members must use this convention to increase code clarity and readability.
 *
 * -----------------------------------------------------------------------------------------
 * Coding Convention Prepared By:
 *   Darko Reljic
 * =========================================================================================
 *******************************************************************************************/


 // main.cpp
 // SeaLink Technologies Co.
 // Automobile Ferry Reservation System
 // Release 3 - Darko Reljic
 // This file initializes the reservation system and provides a basic text interface.
 // calls stub functions for each use case, and shuts down as itended.

#include <iostream>
#include "UserInterface.h"
#include "Vessel.h"
#include "Sailing.h"
#include "Reservation.h"

//------------------------------------------------------------
// Initializes all major data files that might be needed when the reservation system is in use
// Preconditions : None
// Postconditions: All major subsystems are initialized and ready.
void initialize()
{
    std::cout << "System Initialization Started...\n";

    // Initialize the user interface (menus, state).
    UserInterface::initialize();

    // Initialize vessel management module (loads vessels file).
    Vessel::initialize();

    // Initialize sailing management module (loads sailings file).
    Sailing::initialize();

    // Initialize reservations management module (loads reservations file).
    Reservation::initialize();

    std::cout << "System Initialization Complete.\n";
}

//------------------------------------------------------------
// Safely shuts down loaded files
// Preconditions : All modules have been initialized.
// Postconditions: All files are closed and dynamic memory is released
void shutdown()
{
    std::cout << "System Shutdown Started...\n";

    // Shutdown user interface module.
    UserInterface::shutdown();

    // Shutdown vessel management module.
    Vessel::shutdown();

    // Shutdown sailing management module.
    Sailing::shutdown();

    // Shutdown reservations management module.
    Reservation::shutdown();

    std::cout << "System Shutdown Complete.\n";
}

//------------------------------------------------------------
// Starting point of the Automobile Ferry Reservation System.
// Preconditions : None
// Postconditions: Initializes system, runs menu loop, then safely shuts down upon user request.
int main()
{
    initialize();

    // Initializing menu choice
    int choice; 

    do {
        // Displaying the main menu options.
        std::cout << "\n--- Automobile Ferry Reservation System Menu ---\n";
        std::cout << "1. Create New Vessel\n";
        std::cout << "2. Delete Vessel\n";
        std::cout << "3. Create New Sailing\n";
        std::cout << "4. Delete Sailing\n";
        std::cout << "5. Query Sailing Status\n";
        std::cout << "6. New Customer Reservation\n";
        std::cout << "7. Returning Customer Reservation\n";
        std::cout << "8. Delete Reservation\n";
        std::cout << "9. Check-In Vehicle\n";
        std::cout << "10. Print Sailing Report\n";
        std::cout << "0. Quit\n";
        std::cout << "Enter your choice: ";

        // Prompting user to input their next menu state
        std::cin >> choice;

        // Call the appropriate function according to the user input
        switch (choice)
        {
            case 1:
                Vessel::CreateVessel("DUMMY", 0, 0);
                break;
            case 2:
                Vessel::DeleteVessel("DUMMY");
                break;
            case 3:
                Sailing::CreateSailing("Anywhere", "DUMMY", "1970-01-01", "0000");
                break;
            case 4:
                Sailing::DeleteSailing(0);
                break;
            case 5:
                Sailing::printStatus(0);
                break;
            case 6:
                Reservation::newCustomerReservation("XXX000", "0000000000", false, 0);
                break;
            case 7:
                Reservation::returningCustomerReservation("XXX000", 0);
                break;
            case 8:
                Reservation::deleteReservation(0);
                break;
            case 9:
                Reservation::checkinVehicle("XXX000", 0);
                break;
            case 10:
                Sailing::printReport();
                break;
            case 0:
                std::cout << "Exiting reservation system...\n";
                break;
            default:
                std::cout << "Invalid input, please try again.\n";
                break;
        }
    } while (choice != 0);

    shutdown(); 
    return 0; 
}

