//************************************************************
//************************************************************
//  UserInterface.h
//  High-level menu presentation & input validation
//************************************************************
//************************************************************
#ifndef USERINTERFACE_H
#define USERINTERFACE_H

class UserInterface
{
public:
//------------------------------------------------------------
// Initialises UI state (loads language strings, clears screen).
// Preconditions : none
// Postconditions: ready for menu loop.
    static void initialize();

//------------------------------------------------------------
// Flushes any UI caches, restores terminal state.
// Preconditions : initialize() previously called.
// Postconditions: console left tidy.
    static void shutdown();
};

#endif // USERINTERFACE_H
