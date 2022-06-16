#pragma once
#include <vector>
#include <string>
#include "TSystem.h"

class TruthBeamData{
    public:
        double posX;
        double posY;
        double posZ;
        double phi;
        double theta;
        double energy;
        int ParticleID;
        TruthBeamData(){}
        ~TruthBeamData(){}
};

class DetectorData{
    public:
        std::vector<int> TrackID;
        std::vector<int> ParticleID;
        std::vector<double> EnergyDeposited;
        std::vector<double> posX;
        std::vector<double> posY;
        std::vector<double> posZ;
        std::vector<int> Layer;
        DetectorData(){
            TrackID.clear();
            ParticleID.clear();
            EnergyDeposited.clear();
            posX.clear();
            posY.clear();
            posZ.clear();
            Layer.clear();
        }
        ~DetectorData(){}
};


class Event{
    public:
        int eventID;
        DetectorData detectorData;
        TruthBeamData truthBeamData;
        void Clear();
        Event(){}
        ~Event(){}
};