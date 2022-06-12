#include "RunAction.hh"


MyRunAction::MyRunAction(){}

MyRunAction::~MyRunAction(){}

void MyRunAction::BeginOfRunAction(const G4Run*){
    //G4AnalysisManager is a wrapper to ROOT
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetVerboseLevel(2);
    analysisManager->OpenFile("output.root");

    //Define the data from the sensitive detector to store in the output file
    analysisManager->CreateNtuple("Detector", "Detector");
    analysisManager->CreateNtupleDColumn(0,"EventID");
    analysisManager->CreateNtupleDColumn(0,"TrackID");
    analysisManager->CreateNtupleDColumn(0,"ParticleID");
    analysisManager->CreateNtupleSColumn(0,"ParticleName");
    analysisManager->CreateNtupleDColumn(0,"EnergyDeposited");
    analysisManager->CreateNtupleDColumn(0,"XPixel");
    analysisManager->CreateNtupleDColumn(0,"YPixel");
    analysisManager->CreateNtupleDColumn(0,"ZPixel");
    analysisManager->CreateNtupleDColumn(0,"Layer");
    analysisManager->FinishNtuple(0);

    //Define the truth value to store in the output file as an ntuple
    analysisManager->CreateNtuple("TruthBeam","TruthBeam");
    analysisManager->CreateNtupleDColumn(1,"EventID");
    analysisManager->CreateNtupleDColumn(1,"posX");
    analysisManager->CreateNtupleDColumn(1,"posY");
    analysisManager->CreateNtupleDColumn(1,"posZ");
    analysisManager->CreateNtupleDColumn(1,"phi");
    analysisManager->CreateNtupleDColumn(1,"theta");
    analysisManager->CreateNtupleDColumn(1,"energy");
    analysisManager->CreateNtupleDColumn(1,"ParticleID");
    analysisManager->CreateNtupleSColumn(1,"ParticleName");
    analysisManager->FinishNtuple(1);

}

void MyRunAction::EndOfRunAction(const G4Run*){
    //The analysis manager is a singleton. This instance return the pointer to the analysis manager instanced before in the begin of the run action.
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    //Write and close the file
    analysisManager->Write();
    analysisManager->CloseFile();
}