#pragma once
#include "TSystem.h"
#include <string>
#include <vector>
/**
 * @class TruthBeamData
 * @brief Class to store the truth data of the primary beam
 *
 */
class TruthBeamData  {
public:
    double posX;
    double posY;
    double posZ;
    double phi;
    double theta;
    double energy;
    int ParticleID;
    TruthBeamData() {}
    ~TruthBeamData() {}
};

/**
 * @class DetectorData
 * @brief Class to store the data of the sensitive detector
 *
 */
class DetectorData  {
public:
    std::vector<int> TrackID;
    
    std::vector<double> EnergyDeposited;
    std::vector<int> channelX;
    std::vector<int> channelY;
    
    std::vector<int> Layer;
    DetectorData() {
        TrackID.clear();

        EnergyDeposited.clear();
        channelX.clear();
        channelY.clear();

        Layer.clear();
    }
    ~DetectorData() {}

    /**
     * @brief Clear all the vectors in the detector data object
     *
     */
    void Clear();
};

/**
 * @class Event
 * @brief Class to store the data of the simulation
 */
class Event  {
public:
    int eventID;
    DetectorData detectorData;
    TruthBeamData truthBeamData;
    /**
     * @brief Clear the detectorData vectors
     *
     */
    void Clear();
    Event() {}
    ~Event() {}
};