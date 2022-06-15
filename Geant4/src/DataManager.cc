#include "DataManager.hh"


DataManager* DataManager::instance = nullptr;

DataManager::DataManager(){
    file = nullptr;
    tree = nullptr;
    event = nullptr;
}

DataManager* DataManager::GetInstance(){
    if (instance == nullptr){
        instance = new DataManager();
    }
    return instance;
}

DataManager::~DataManager(){
    delete instance;
}


TFile *DataManager::OpenFile(std::string filename, std::string option){
    file = new TFile(filename.c_str(), option.c_str());
    return file;
}

TTree *DataManager::CreateTree(std::string name){
    tree = new TTree(name.c_str(), name.c_str());
    event = new Event();
    tree->Branch("Event", &event);
    return tree;
}

TFile *DataManager::GetFile(){
    return file;
}

TTree *DataManager::GetTree(){
    return tree;
}

Event *DataManager::GetEvent(){
    return event;
}

