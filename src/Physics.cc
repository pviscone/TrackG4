#include "Physics.hh"

/**
 *Define the physics module G4EmStandardPhysics \n
 *It takes account of the most common electromagnetic interaction involved in HEP.
 */
MyPhysicsList::MyPhysicsList() {
    // Definition of all standard electromagnetic processes
    RegisterPhysics(new G4EmStandardPhysics());
}

MyPhysicsList::~MyPhysicsList() {}

/**
 * The cuts are setted in the run macro
 * Here we just decided to show the cut table in the output
 */
void MyPhysicsList::SetCuts() {
    DumpCutValuesTable();
}