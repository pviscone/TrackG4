#include "../catch.hpp"
#include "DataManagerMT.hh"

TEST_CASE("Verify that DataManagerMT is a singleton")
{
    DataManagerMT *dataManagerMT = DataManagerMT::GetInstance();
    DataManagerMT *dataManagerMT2 = DataManagerMT::GetInstance();
    REQUIRE(dataManagerMT == dataManagerMT2);
}
