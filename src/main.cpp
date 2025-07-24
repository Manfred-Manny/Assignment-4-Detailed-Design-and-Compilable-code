#include "UserInterface.h"

int main() {
    UserInterface::initialize();
    UserInterface::runMainMenu();
    UserInterface::shutdown();
    return 0;
}
