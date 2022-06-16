#include "EventAction.hh"

MyEventAction::MyEventAction() {}

MyEventAction::~MyEventAction() {}

void MyEventAction::BeginOfEventAction(const G4Event *)
{
}

void MyEventAction::EndOfEventAction(const G4Event *)
{
    DataManagerMT *dataManagerMT = DataManagerMT::GetInstance();
    int threadId = G4Threading::G4GetThreadId();
    DataManager *dataManager = dataManagerMT->GetSTDataManager(threadId);
    TTree *tree = dataManager->GetTree();
    tree->Fill();
    dataManager->GetEvent()->Clear();

}