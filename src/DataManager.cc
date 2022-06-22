#include "DataManager.hh"

DataManager *DataManager::instance = nullptr;

DataManager::DataManager() {
    // initialize the pointers to nullptr
    file = nullptr;
    tree = nullptr;
    event = nullptr;
}

DataManager *DataManager::GetInstance() {
    // If the instance is nullptr, create a new instance, otherwise return the existing instance
    if (instance == nullptr) {
        instance = new DataManager();
    }
    return instance;
}

DataManager::~DataManager() {
    // delete the instance
    delete instance;
}

TFile *DataManager::OpenFile(std::string filename, std::string option) {
    // open the file
    file = new TFile(filename.c_str(), option.c_str());
    return file;
}

TTree *DataManager::CreateTree(std::string name) {
    // create the tree and a event branch
    tree = new TTree(name.c_str(), name.c_str());
    event = new Event();
    tree->Branch("Event", &event);
    return tree;
}

TFile *DataManager::GetFile() {
    return file;
}

TTree *DataManager::GetTree() {
    return tree;
}

Event *DataManager::GetEvent() {
    return event;
}
