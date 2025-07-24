#ifndef USERINTERFACE_H
#define USERINTERFACE_H

class UserInterface {
public:
    // Initializes UI system
    static void initialize();

    // Runs main menu loop
    static void runMainMenu();

    // Cleans up UI system
    static void shutdown();

private:
    // Submenus for different modules
    static void vesselMenu();
    static void sailingMenu();
    static void reservationMenu();
};

#endif
