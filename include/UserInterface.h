#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <string>

class UserInterface {
public:
    // Initialize and shutdown UI
    static void initialize();
    static void runMainMenu();
    static void shutdown();

private:
    // Menus
    static void vesselMenu();
    static void sailingMenu();
    static void reservationMenu();

    // Input Helpers (all with 0 cancel support)
    static bool promptYesNo(const std::string &message);
    static void clearInput();

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

#endif