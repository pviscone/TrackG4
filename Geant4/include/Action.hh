#pragma once
#include "G4VUserActionInitialization.hh"
#include "Generator.hh"
#include "RunAction.hh"

/**
 * @brief The class that manages the action initialization.
 *
 * The action initialization is the class that manages the action initialization.
 *
 *
 */
class MyActionInitialization : public G4VUserActionInitialization{
    public:
        /**
         * @brief Constructor of the action initialization.
         *
         * @param generator The generator of the particles.
         */
        MyActionInitialization();
        /**
         * @brief Destructor of the action initialization.
         */
        ~MyActionInitialization();

        /**
         * @brief Method that is called when the action initialization is built.
         *
         * This method runs at the beginning of the program and it is used to build the action initialization.
         * In multithreading, the method is called by each thread.
         */
        virtual void Build() const;

        /**
         * @brief Method that is called when the action initialization is built for the master thread.
         *
         * This method runs at the beginning of the program and it is used to build the action initialization for the master thread.
         * In multithreading, the method is called by the master thread.
         */
        virtual void BuildForMaster() const;
};