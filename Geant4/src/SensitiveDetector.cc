#include "SensitiveDetector.hh"


MySensitiveDetector::MySensitiveDetector(G4String name) : G4VSensitiveDetector(name){
}


MySensitiveDetector::~MySensitiveDetector(){
}



/**
 * The data stored at each step is:
 * - EventID
 * - TrackID
 * - ParticleID
 * - ParticleName
 * - EnergyDeposited (GeV)
 * - XPixel X position of the hit (m)
 * - YPixel Y position of the hit (m)
 * - ZPixel Z position of the hit (m)
 * - Layer layer that the hit is in
 *
 */
G4bool MySensitiveDetector::ProcessHits(G4Step* aStep, G4TouchableHistory* R0hist){
    //Get the trackID and the particle name from the track object
    G4Track *track = aStep->GetTrack();
    G4int trackID = track->GetTrackID();
    G4String particleName = track->GetDefinition()->GetParticleName();
    std::string particle=particleName;


    //Get the position of the interaction
    G4StepPoint *preStepPoint = aStep -> GetPreStepPoint();
    G4ThreeVector hitPos = preStepPoint->GetPosition();

    //Get the event ID from the run manager
    G4int evID=G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

    //Get the energy deposit in the step
    G4double edep= aStep->GetTotalEnergyDeposit();

    //Get the copy number of the physical volume that was hit (corresponding to the layer)
    const G4VTouchable* touchable = preStepPoint->GetTouchable();
    //Uncomment to get the position of the detector that was hit
    //G4ThreeVector detPos = (touchable->GetVolume())->GetTranslation();
    G4double layer = touchable->GetCopyNumber();

    //Set a particle ID according to the particle name
    G4double particleID;
    if(particle=="mu-"){
        particleID=0;
    }
    else if(particle=="mu+"){
        particleID=1;
    }
    else if(particle=="e-"){
        particleID=2;
    }
    else if(particle=="e+"){
        particleID=3;
    }
    else if(particle=="gamma"){
        particleID=4;
    }
    else{
        particleID=-1;
    }
    //Get the pointer to the analysis manager
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    //Fill the ntuple
    analysisManager->FillNtupleDColumn(0, 0,  evID);
    analysisManager->FillNtupleDColumn(0, 1, trackID);
    analysisManager->FillNtupleDColumn(0, 2, particleID);
    analysisManager->FillNtupleSColumn(0, 3, particle);
    analysisManager->FillNtupleDColumn(0, 4, edep);
    analysisManager->FillNtupleDColumn(0, 5, hitPos.x());
    analysisManager->FillNtupleDColumn(0, 6, hitPos.y());
    analysisManager->FillNtupleDColumn(0, 7, hitPos.z());
    analysisManager->FillNtupleDColumn(0, 8, layer);
    analysisManager->AddNtupleRow(0);

/*     track->SetTrackStatus(fStopAndKill);
    if (trackID != 1 && particleName != "gamma"){
        track->SetTrackStatus(fStopAndKill);
    } */

    return true;
}