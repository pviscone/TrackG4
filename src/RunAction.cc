#include "RunAction.hh"


MyRunAction::MyRunAction(){}

MyRunAction::~MyRunAction(){}

void MyRunAction::BeginOfRunAction(const G4Run*){

    G4int threadId = G4Threading::G4GetThreadId();
    if (threadId>-1){
        std::string filename = OutputParameters::outputFileName + std::to_string(threadId) + ".root";
        DataManagerMT *dataManagerMT = DataManagerMT::GetInstance();
        DataManager *dataManager = dataManagerMT->GetSTDataManager(threadId);
        dataManager->OpenFile(filename,"RECREATE");
        dataManager->CreateTree("Events");
    }

}

void MyRunAction::EndOfRunAction(const G4Run*){
    DataManagerMT *dataManagerMT = DataManagerMT::GetInstance();
    G4int threadId = G4Threading::G4GetThreadId();
    if (threadId>-1){
        DataManager *dataManager = dataManagerMT->GetSTDataManager(threadId);
        dataManager->GetFile()->Write();
        dataManager->GetFile()->Close();
    }
}