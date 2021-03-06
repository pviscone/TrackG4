#include "SensitiveDetector.hh"

MySensitiveDetector::MySensitiveDetector(G4String name) : G4VSensitiveDetector(name) {
}

MySensitiveDetector::~MySensitiveDetector() {
}

/**
 * The data stored at each step is:
 * - EventID
 * - TrackID
 * - ParticleID
 * - ParticleName
 * - EnergyDeposited (keV)
 * - posX X position of the channel that was hit (mm)
 * - posY Y position of the channel that was hit (mm)
 * - posZ Z position of the channel that was hit (mm)
 * - Layer layer that the hit is in (odd layer are pixels module, even layer are strips module)
 *
 */
G4bool MySensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *R0hist) {
    /********************** TRACK ID AND PARTICLE NAME **********************/
    G4Track *track = aStep->GetTrack();
    G4int trackID = track->GetTrackID();
    G4String particleName = track->GetDefinition()->GetParticleName();
    std::string particle = particleName;

    /*************************** HIT POSITION ******************************/
    G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
    G4ThreeVector realHitPos = preStepPoint->GetPosition();

    /*************************** ENERGY DEPOSITED ****************************/
    G4double edep = aStep->GetTotalEnergyDeposit();

    /*************************** LAYER ****************************/
    // Get the copy number of the physical volume that was hit (corresponding to the layer)
    const G4VTouchable *touchable = preStepPoint->GetTouchable();
    // Uncomment to get the position of the detector that was hit
    // G4ThreeVector detPos = (touchable->GetVolume())->GetTranslation();
    G4int layer = touchable->GetCopyNumber();

    /*************************** PARTICLE ID ****************************/
    // Set a particle ID according to the particle name
    G4double particleID;
    if (particle == "mu-") {
        particleID = 0;
    } else if (particle == "mu+") {
        particleID = 1;
    } else if (particle == "e-") {
        particleID = 2;
    } else if (particle == "e+") {
        particleID = 3;
    } else if (particle == "gamma") {
        particleID = 4;
    } else {
        particleID = -1;
    }

    /*************************** FILL DETECTOR DATA ****************************/
    // Get the DataManager object of the i-th thread
    DataManagerMT *dataManagerMT = DataManagerMT::GetInstance();
    G4int threadId = G4Threading::G4GetThreadId();
    DataManager *dataManager = dataManagerMT->GetSTDataManager(threadId);

    ReadOut *RO = ReadOut::GetInstance();
    // Fill the event object with the detector data
    Event *ev = dataManager->GetEvent();

    if (edep / keV > TriggerParameters::energyCut / keV) {
        ev->detectorData.TrackID.push_back((int)trackID);
        ev->detectorData.ParticleID.push_back((int)particleID);
        ev->detectorData.EnergyDeposited.push_back((double)(edep / keV));
        ev->detectorData.posX.push_back((RO->closest_channel((double)realHitPos.x() / mm, "x", (int)layer)));
        ev->detectorData.posY.push_back((RO->closest_channel((double)realHitPos.y() / mm, "y", (int)layer)));
        ev->detectorData.posZ.push_back((RO->closest_channel((double)realHitPos.z() / mm, "z", (int)layer)));
        ev->detectorData.Layer.push_back((int)layer);
        // Uncomment to get the real position of the hit
        /*     ev->detectorData.posX.push_back((double)(realHitPos.x()/mm));
            ev->detectorData.posY.push_back((double)(realHitPos.y()/mm));
            ev->detectorData.posZ.push_back((double)(realHitPos.z()/mm)); */
    }

    return true;
}