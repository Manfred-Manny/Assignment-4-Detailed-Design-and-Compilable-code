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
 *          //************************************************************
 *          //************************************************************
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
... (152 lines left)
Collapse
message.txt
10 KB
this is the main function i had before, and the coding convention with file name at the top
4) NAMING:
 
Files: PascalCase (ex, CruiseShips)
Classes and types: PascalCase (ex, ReservationClass).
Variables and functions: camelCase (ex createReservation()).
Constants: UPPER_CASE_WITH_UNDERSCORES.

 =========================================================================================*/
 
#include "UserInterface.h"

int main() {
    UserInterface::initialize();
    UserInterface::runMainMenu();
    UserInterface::shutdown();
    return 0;
}
