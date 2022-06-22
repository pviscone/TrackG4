#include "DataManagerMT.hh"
#include "../catch.hpp"
#include "UserParameters.hh"
#include <cppitertools/combinations.hpp>
#include <thread>

TEST_CASE("Verify that DataManagerMT is a singleton") {
    DataManagerMT *dataManagerMT = DataManagerMT::GetInstance();
    DataManagerMT *dataManagerMT2 = DataManagerMT::GetInstance();
    REQUIRE(dataManagerMT == dataManagerMT2);
}

TEST_CASE("Verify that DataManagerMt has a vector of different instances of DataManager") {
    DataManagerMT *dataManagerMT = DataManagerMT::GetInstance();
    int nThreads = SystemParameters::nThreads;
    for (int i = 0; i < nThreads; i++) {
        for (int j = i + 1; j < nThreads; j++) {
            REQUIRE(dataManagerMT->GetSTDataManager(i) != dataManagerMT->GetSTDataManager(j));
        }
    }
}
