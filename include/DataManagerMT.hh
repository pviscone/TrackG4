#pragma once
#include "DataManager.hh"
#include "UserParameters.hh"
#include <thread>
#include <vector>

/**
 * @class DataManagerMT
 * @brief Singleton class that manage the data in multithreading mode
 *
 * This class is a friend class of DataManager.\n
 * This class is a singleton container for multiple instance of DataManager
 *
 */
class DataManagerMT {
private:
    /**
     * @brief Private constructor.
     *
     * The constructor is private because the class is a singleton.
     */
    static DataManagerMT *instanceMT;

    /**
     * @brief The number of thread used to run the simulation.
     *
     * The number of thread is set in the UserParameters file
     *
     */
    int nThreads = SystemParameters::nThreads;

    /**
     * @brief The vector of DataManager objects.
     *
     * The vector is used to store the DataManager objects.
     * Each element of the vector contain a DataManager object used by the same thread
     *
     */
    std::vector<DataManager *> dataManagerVector;

    /**
     * @brief Private constructor.
     *
     * The constructor is private because the class is a singleton.
     */
    DataManagerMT();

public:
    /**
     * @brief Destroy the DataManagerMT object
     *
     */
    ~DataManagerMT();

    /**
     * @brief Get the instance of the class.
     * The first time create the singleton, then return the pointer to it.
     *
     * @return The instance of the single class object.
     */
    static DataManagerMT *GetInstance();

    /**
     * @brief Return the DataManager object used by the i-th thread.
     *
     * @param i threadId
     * @return DataManager*
     */
    DataManager *GetSTDataManager(int i);
};
