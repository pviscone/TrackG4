#include "EventAction.hh"

MyEventAction::MyEventAction() {}

MyEventAction::~MyEventAction() {}

void MyEventAction::BeginOfEventAction(const G4Event *) {
}

/**
 * At the end of each action this function fill the tree and clear the event object
 *
 * If there aren't at least two hit in two different layers, the event is not saved
 */
void MyEventAction::EndOfEventAction(const G4Event *) {
    // Get the DataManager of the i-th thread
    DataManagerMT *dataManagerMT = DataManagerMT::GetInstance();
    int threadId = G4Threading::G4GetThreadId();
    DataManager *dataManager = dataManagerMT->GetSTDataManager(threadId);

    // Trigger: if the event has not at least two hit in two different layers, it is not saved
    Event *ev = dataManager->GetEvent();
    std::vector<int> layerVec = ev->detectorData.Layer;
    if (!layerVec.empty()) {
        if (std::max_element(layerVec.begin(), layerVec.end()) - std::min_element(layerVec.begin(), layerVec.end()) < 2) {
            (ev->detectorData).Clear();
        }
    }

    // Fill the tree and clear the event
    TTree *tree = dataManager->GetTree();
    tree->Fill();
    dataManager->GetEvent()->Clear();
}