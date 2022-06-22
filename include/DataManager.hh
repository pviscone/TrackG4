#pragma once
#include "EventData.hh"
#include "TFile.h"
#include "TTree.h"

/**
 *@class DataManager
 *
 * @brief This class manage TFiles and TTrees to store the data exploiting the Event class structure.
 *
 * This class is a singleton
 */
class DataManager {
private:
    /**
     * @brief Private constructor.
     *
     * The constructor is private because the class is a singleton.
     */
    DataManager();

    /**
     * @brief pointer to the single instance of the class.
     *
     */
    static DataManager *instance;

    /**
     * @brief The file where the data is stored.
     *
     */
    TFile *file;

    /**
     * @brief The tree where the data is stored.
     *
     */
    TTree *tree;

    /**
     * @brief The event class object used to store data.
     *
     */
    Event *event;

    /**
     * @brief Class used to use the DataManager class in multithreading mode.
     *
     * A Singleton object is not thread safe.\n
     * To be thread safe the strategy usually adopted by geant is to use a different file for each working thread.
     *
     * With a singleton we cannot use multiple files so we define a friend class DataManagerMT (another singleton) that is able to create multiple instances
     * of DataManager, one for each working thread
     *
     */
    friend class DataManagerMT;

public:
    /**
     * @brief Method to construct the singleton.
     * The first time create the singleton, then return the pointer to it.
     * @return DataManager* the pointer to the singleton instance
     */
    static DataManager *GetInstance();

    /**
     * @brief Singleton destructor
     *
     */
    ~DataManager();

    /**
     * @brief Open the TFile
     *
     * @param fileName the name of the file
     * @param option the mode of the file
     *
     * @return TFile* the pointer to the TFile
     */
    TFile *OpenFile(std::string filename, std::string option = "");

    /**
     * @brief Create a Tree object
     *
     * @param name name of the TTree
     * @return TTree*  the pointer to the TTree
     */
    TTree *CreateTree(std::string name);

    /**
     * @brief Return the pointer to the file of the DataManager singleton
     *
     * @return TFile* the pointer to the file
     */
    TFile *GetFile();

    /**
     * @brief Return the pointer to the tree of the DataManager singleton
     *
     * @return TTree* the pointer to the tree
     */
    TTree *GetTree();

    /**
     * @brief Return the pointer to the event of the DataManager singleton
     *
     *This class has to be filled with the data.
     *At the end of each event this object has to be written in the tree and cleared
     *
     * @return Event* the pointer to the event
     */
    Event *GetEvent();
};