#include "../catch.hpp"
#include "DataManager.hh"

TEST_CASE("Verify that DataManager is a singleton")
{
    DataManager *dataManager = DataManager::GetInstance();
    DataManager *dataManager2 = DataManager::GetInstance();
    REQUIRE(dataManager == dataManager2);
}
