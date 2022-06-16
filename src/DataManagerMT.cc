#include "DataManagerMT.hh"

DataManagerMT* DataManagerMT::instanceMT = nullptr;

DataManagerMT::DataManagerMT(){
    for (int i = 0; i < nThreads; i++){
        DataManager *dataManager = new DataManager();
        dataManagerVector.push_back(dataManager);
    }
}

DataManagerMT::~DataManagerMT(){
    for (int i = 0; i < nThreads; i++){
        delete dataManagerVector[i];
    }
    delete instanceMT;
}


DataManagerMT* DataManagerMT::GetInstance(){
    if (instanceMT == nullptr){
        instanceMT = new DataManagerMT();
    }
    return instanceMT;
}

DataManager* DataManagerMT::GetSTDataManager(int i){
    return dataManagerVector[i];
}