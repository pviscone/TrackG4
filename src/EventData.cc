#include "EventData.hh"

// Clear all the vectors in the event
void Event::Clear() {
    detectorData.TrackID.clear();
    detectorData.ParticleID.clear();
    detectorData.EnergyDeposited.clear();
    detectorData.posX.clear();
    detectorData.posY.clear();
    detectorData.posZ.clear();
    detectorData.Layer.clear();
}

void DetectorData::Clear() {
    TrackID.clear();
    ParticleID.clear();
    EnergyDeposited.clear();
    posX.clear();
    posY.clear();
    posZ.clear();
    Layer.clear();
}