#include "RunAction.hh"

MyRunAction::MyRunAction() {}

MyRunAction::~MyRunAction() {}

void MyRunAction::BeginOfRunAction(const G4Run *) {

    G4int threadId = G4Threading::G4GetThreadId();
    
    // The master thread (id=-1) create the only instance of DataManagerMT
    if (threadId == -1) {
        DataManagerMT::GetInstance();
    }
    // Exclude the master thread (id=-1) from the file creation
    if (threadId > -1) {
        // Create a file and a tree for each working thread
        std::string filename = OutputParameters::outputFileName + std::to_string(threadId) + ".root";
        DataManagerMT *dataManagerMT = DataManagerMT::GetInstance();
        DataManager *dataManager = dataManagerMT->GetSTDataManager(threadId);
        dataManager->OpenFile(filename, "RECREATE");
        dataManager->CreateTree("Events");
    }
}

void MyRunAction::EndOfRunAction(const G4Run *) {
    // Get the DataManager of the i-th thread
    DataManagerMT *dataManagerMT = DataManagerMT::GetInstance();
    G4int threadId = G4Threading::G4GetThreadId();

    // Each working thread has to write and close the file
    if (threadId > -1) {
        DataManager *dataManager = dataManagerMT->GetSTDataManager(threadId);
        dataManager->GetFile()->Write();
        dataManager->GetFile()->Close();
    }
}
