#include "Fit.hh"
#include "../catch.hpp"
#include "EventData.hh"
#include <TF1.h>

using namespace Catch::literals;

TEST_CASE("test fit with 3 points in line") {
    Event event;
    DetectorData detector;
    detector.posX = {0, 1, 2};
    detector.posY = {1, 3, 5};
    detector.posZ = {0, 1, 2};
    detector.Layer = {1, 3, 6};
    (event.detectorData) = detector;
    TF1 *line = new TF1("line", "pol1", 0, 3);
    FitData fitdata = Fit(&event, line);
    REQUIRE(fitdata.mx == 1._a);
    REQUIRE(fitdata.my == 2._a);
    REQUIRE(fitdata.x0 == 0._a);
    REQUIRE(fitdata.y0 == 1._a);
}