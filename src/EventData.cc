#include "EventData.hh"

// Clear all the vectors in the event
void Event::Clear() {
    detectorData.TrackID.clear();
    detectorData.EnergyDeposited.clear();
    detectorData.channelX.clear();
    detectorData.channelY.clear();
    detectorData.Layer.clear();
}

void DetectorData::Clear() {
    TrackID.clear();
    EnergyDeposited.clear();
    channelX.clear();
    channelY.clear();
    Layer.clear();
}