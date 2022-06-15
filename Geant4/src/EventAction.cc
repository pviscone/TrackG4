#include "EventAction.hh"

MyEventAction::MyEventAction(){}


MyEventAction::~MyEventAction(){}


void MyEventAction::BeginOfEventAction(const G4Event*){
    DataManager *dataManager = DataManager::GetInstance();
    dataManager->GetEvent()->Clear();
}



void MyEventAction::EndOfEventAction(const G4Event*){
    DataManager *dataManager = DataManager::GetInstance();
    TTree *tree = dataManager->GetTree();
    tree->Fill();
}