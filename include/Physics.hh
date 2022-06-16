#pragma once
#include "G4VModularPhysicsList.hh"
#include "G4EmStandardPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4SystemOfUnits.hh"

/** @class MyPhysicsList
 * @brief The PhysicsList class that defines the physics processes involved in the simulation
 *
 * This class contains the physics processes used in the simulation.
 */
class MyPhysicsList : public G4VModularPhysicsList{
    public:
        /** @brief Constructor of the PhysicsList class
         *
         * This constructor initializes the PhysicsList.
         */
        MyPhysicsList();

        /** @brief Destructor of the PhysicsList class
         *
         * This destructor deletes the PhysicsList.
         */
        ~MyPhysicsList();

        /**
         * @brief Set the cuts for the physics processes.
         *
         */
        virtual void SetCuts();
};