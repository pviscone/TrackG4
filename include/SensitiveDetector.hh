#pragma once
#include  "G4VSensitiveDetector.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "DataManager.hh"
#include "DataManagerMT.hh"

/**
 * @brief The class that manages the sensitive detector.
 *
 * The sensitive detector is the volume that is used to detect the particles and save the data.
 *
 *
 */
class MySensitiveDetector : public G4VSensitiveDetector{
    public:
        /**
         * @brief Constructor of the sensitive detector.
         *
         * @param name The name of the sensitive detector.
         */
        MySensitiveDetector(G4String name);

        /**
         * @brief Destructor of the sensitive detector.
         */
        ~MySensitiveDetector();
    private:
        /**
         * @brief Method that is called when a step is detected.
         *
         * This method runs at every step of the particle in the sensitive detector and it is used to save the data.
         *
         * @param aStep The step that is detected.
         * @param R0hist The history of the step.
         */
        virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);

};
