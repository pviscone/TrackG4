#include "DataManagerMT.hh"

DataManagerMT *DataManagerMT::instanceMT = nullptr;

DataManagerMT::DataManagerMT() {
    // fill the vector with DataManager objects
    for (int i = 0; i < nThreads; i++) {
        DataManager *dataManager = new DataManager();
        dataManagerVector.push_back(dataManager);
    }
}

DataManagerMT::~DataManagerMT() {
    // delete the DataManager objects
    for (int i = 0; i < nThreads; i++) {
        delete dataManagerVector[i];
    }
    delete instanceMT;
}

DataManagerMT *DataManagerMT::GetInstance() {
    // If the instance is nullptr, create a new instance, otherwise return the existing instance
    if (instanceMT == nullptr) {
        instanceMT = new DataManagerMT();
    }
    return instanceMT;
}

DataManager *DataManagerMT::GetSTDataManager(int i) {
    // return the DataManager object used by the i-th thread
    return dataManagerVector[i];
}