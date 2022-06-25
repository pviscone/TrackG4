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

    // Trigger: if the event:
    // - has less than 2 hits
    // - has hits only in the same pixel/strip module
    // - has only hits in the same PS module
    // then it is not saved
    Event *ev = dataManager->GetEvent();
    std::vector<int> layerVec = ev->detectorData.Layer;
    if (!layerVec.empty()) {
        int min_layer=std::min_element(layerVec.begin(), layerVec.end());
        int layer_diff=std::max_element(layerVec.begin(), layerVec.end()) - min_layer;
        if ( (layerVec.size()<2) || (layer_diff == 0) || ((layer_diff==1) && (min_layer%2=1)) ) {
            (ev->detectorData).Clear();
        }
    }

    // Fill the tree and clear the event
    TTree *tree = dataManager->GetTree();
    tree->Fill();
    dataManager->GetEvent()->Clear();
}
