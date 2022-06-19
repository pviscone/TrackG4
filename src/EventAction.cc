#include "EventAction.hh"

MyEventAction::MyEventAction() {}

MyEventAction::~MyEventAction() {}

void MyEventAction::BeginOfEventAction(const G4Event *)
{
}

/**
 * At the end of each action this function fill the tree and clear the event object
 *
 */
void MyEventAction::EndOfEventAction(const G4Event *)
{
    // Get the DataManager of the i-th thread
    DataManagerMT *dataManagerMT = DataManagerMT::GetInstance();
    int threadId = G4Threading::G4GetThreadId();
    DataManager *dataManager = dataManagerMT->GetSTDataManager(threadId);

    // Fill the tree and clear the event
    TTree *tree = dataManager->GetTree();
    tree->Fill();
    dataManager->GetEvent()->Clear();
}