#include "ReadOut.hh"
#include "../catch.hpp"
#include "cppitertools/enumerate.hpp"
#include "cppitertools/zip.hpp"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>


TEST_CASE("Test the closest_value function") {
    ReadOut* RO=ReadOut::GetInstance();
    std::vector<double> vec = {6, 7, 8, 9, 10, 1, 2, 3, 4, 5};
    SECTION("Value greater than the nearest") {
        std::vector<double> greater_values = {6.1, 7.2, 8.3, 9.4, 10.4,
                                              1.2, 2.1, 3.3, 4.1, 5.43};
        for (auto &&[idx, value] : iter::enumerate(greater_values)) {
            REQUIRE(RO->closest_value(vec, value) == vec[idx]);
        }
    }

    SECTION("Value less than the nearest") {
        std::vector<double> less_values = {5.6, 6.7, 7.8, 8.6, 9.6,
                                           0.9, 1.8, 2.7, 3.6, 4.55};
        for (auto &&[idx, value] : iter::enumerate(less_values)) {
            REQUIRE(RO->closest_value(vec, value) == vec[idx]);
        }
    }

    SECTION("Negative numbers") {
        std::vector<double> neg_vec = {-6, -7, -8, -9, -10, -1, -2, -3, -4, -5};
        std::vector<double> neg_values = {-5.8, -7.4, -8.3, -9.2, -10.4,
                                          -0.9, -1.8, -2.7, -3.6, -4.6};
        for (auto &&[idx, value] : iter::enumerate(neg_values)) {
            REQUIRE(RO->closest_value(neg_vec, value) == neg_vec[idx]);
        }
    }
}

TEST_CASE("Test the read out on single axis") {
    ReadOut* RO=ReadOut::GetInstance();
    double pitch = 0.1;
    std::vector<double> modulePositions{0, 3, -3, 2, -2, -1, 1};
    std::vector<double> hits{1.11, 2.19, -1.11, -1.49};
    SECTION("Test a generic readout") {

        std::vector<double> results{1.15, 2.15, -1.15, -1.45};

        for (auto &&[hit, res] : iter::zip(hits, results)) {
            REQUIRE(RO->closest_channel(hit, modulePositions, pitch) == res);
        }
    }
}

TEST_CASE("Test the read out passing the strings"){
    ReadOut* RO=ReadOut::GetInstance();
    RO->closest_channel(1.11, "x", 0);
}
