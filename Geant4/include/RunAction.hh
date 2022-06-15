#pragma once
#include "G4UserRunAction.hh"
#include "G4AnalysisManager.hh"
#include "EventData.hh"
#include "TFile.h"
#include "TSystem.h"
#include "TTree.h"
#include "DataManager.hh"
#include <string>

/**
 * @class MyRunAction
 * @brief The class that manages the run action.
 *
 * In this class we define the structure of the output file to store the sensitive detector data.
 *
 *
 */
class MyRunAction : public G4UserRunAction {
    public:
        /**
         * @brief Constructor of the run action.
         */
        MyRunAction();

        /**
         * @brief Destructor of the run action.
         */
        virtual ~MyRunAction();

        /**
         * @brief Method that is called at the beginning of the run.
         *
         * This method runs at the beginning of the run and it is used to initialize the analysis manager and define the output file.
         *
         * @param run The run that is initialized.
         */
        virtual void BeginOfRunAction(const G4Run*);

        /**
         * @brief Method that is called at the end of the run.
         *
         * This method runs at the end of the run and it is used to save the data in the output file and close the file.
         *
         * @param run The run that is finished.
         */
        virtual void EndOfRunAction(const G4Run*);
};