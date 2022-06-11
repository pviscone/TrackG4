#pragma once
#include "G4VModularPhysicsList.hh"
#include "G4EmStandardPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4SystemOfUnits.hh"

/** @class MyPhysicsList
 * @brief The PhysicsList class
 *
 * This class contains the physics processes used in the simulation.
 */
class MyPhysicsList : public G4VModularPhysicsList{
    public:
        MyPhysicsList();
        ~MyPhysicsList();
        virtual void SetCuts();
};