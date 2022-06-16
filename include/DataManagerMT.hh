#pragma once
#include <thread>
#include <vector>
#include "DataManager.hh"
#include "UserParameters.hh"


class DataManagerMT{
    private:
        static DataManagerMT *instanceMT;
        int nThreads = SystemParameters::nThreads;
        std::vector<DataManager *> dataManagerVector;
        DataManagerMT();
    public:
        ~DataManagerMT();
        static DataManagerMT *GetInstance();
        DataManager *GetSTDataManager(int i);
};
