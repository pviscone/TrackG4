#pragma once
#include "TFile.h"
#include "TTree.h"
#include "EventData.hh"

class DataManager{
    private:
        DataManager();
        static DataManager *instance;
        TFile *file;
        TTree *tree;
        Event *event;
    public:
        static DataManager *GetInstance();
        ~DataManager();
        TFile *OpenFile(std::string filename, std::string option="");
        TTree *CreateTree(std::string name);
        TFile *GetFile();
        TTree *GetTree();
        Event *GetEvent();


};