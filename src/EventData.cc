#include "EventData.hh"

void Event::Clear(){
    detectorData.TrackID.clear();
    detectorData.ParticleID.clear();
    detectorData.EnergyDeposited.clear();
    detectorData.posX.clear();
    detectorData.posY.clear();
    detectorData.posZ.clear();
    detectorData.Layer.clear();
}