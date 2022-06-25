#include "Generator.hh"

using namespace GeometryParameters;
using namespace BeamParameters;
/**
 * Create the particle gun and the particle table
 *
 */
MyPrimaryGenerator::MyPrimaryGenerator() {

    fParticleGun = new G4ParticleGun(1);
    particleTable = G4ParticleTable::GetParticleTable();
}

/**
 * Destroy the particle gun
 *
 */
MyPrimaryGenerator::~MyPrimaryGenerator() {
    delete fParticleGun;
}

/**
 *Every time a beam is generated this function generate randomly a muon
 *according to the muons charge ratio \f$ \frac{\mu_{+}}{\mu_{-}} = 1.3 \f$
 *
 * The beam is generated in a random position of the face of the world box perpendicular
 * to the z axis and its direction is parametrized by a spherical unit vector (\f$ (\cos(\phi)\sin(\theta),\sin(\phi)\sin(\theta),\cos(\theta)) \f$)
 *
 * The polar angle of the direction vector of the beam is randomly generated between 0 and \f$ 2 \pi \f$
 *
 * The azimuthal angle \f$ \theta \f$ of the direction vector beam is randomly generated between 0 and pi
 * according to the distribution \f$ \cos^2{\theta} \f$
 *
 * The beam is generated with a random energy between minBeamEnergy and maxBeamEnergy
 * (defined by the user in UserParameters.hh)
 * according to the energy distribution for cosmic muons at the sea surface
 *
 * \f$ \frac{dN_{\mu}}{dE_{\mu} d \Omega} \approx \frac{0.14 E_{\mu}^{-2.7}}{cm^2 \; s \;sr \; GeV} \left( \frac{1}{1+\frac{1.1 E_{\mu}\cos{\theta}}{115 GeV}} + \frac{0.054}{1+\frac{1.1 E_{\mu}\cos{\theta}}{850 GeV}} \right)\f$
 *
 * The data of every beam is stored as an ntuple in the output root file.
 * The saved data are:
 * - EventID
 * - posX X position of the particle gun (mm)
 * - posY Y position of the particle gun (mm)
 * - posZ Z position of the particle gun (mm)
 * - phi Polar angle of the direction vector of the beam
 * - theta Azimuthal angle of the direction vector of the beam
 * - energy Beam energy (GeV)
 * - ParticleID
 * - ParticleName
 */
void MyPrimaryGenerator::GeneratePrimaries(G4Event *event) {

    /********************************PARTICLE DEFINITION********************************/
    // Definition of mu+ and mu-
    G4ParticleDefinition *mu_p = particleTable->FindParticle("mu+");
    G4ParticleDefinition *mu_n = particleTable->FindParticle("mu-");

    // Random number to generate random mu+ or mu- according to their charge ratio
    G4double muCharge = (G4UniformRand());
    // The charge ratio of cosmic muons (mu+/mu-) is 1.3 (assumed constant in energy)
    // This means that the 57% of the muons are mu+ and 43% of them are mu-
    if (muCharge > 0.57) {
        fParticleGun->SetParticleDefinition(mu_n);
    } else {
        fParticleGun->SetParticleDefinition(mu_p);
    }

    /********************************PARTICLE POSITION********************************/
    // Starting point of the beam.
    // It covers uniformly the whole side of the box
    G4double xPos = G4UniformRand() * worldSize * 2 - worldSize;
    G4double yPos = G4UniformRand() * worldSize * 2 - worldSize;
    G4double zPos = -worldSize;

    // Set the starting point of the beam
    fParticleGun->SetParticlePosition(G4ThreeVector(xPos, yPos, zPos));

    /********************************PARTICLE DIRECTION******************************/
    // Set the particle direction using the cos^2 distribution for the azimuthal angle and the uniform distribution for the polar angle
    double phi = G4UniformRand() * 2 * TMath::Pi();
    TF1 fTheta = TF1("f2", "cos(x)*cos(x)", 0, TMath::Pi() / 2);
    double theta = fTheta.GetRandom();
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(cos(phi) * sin(theta), sin(phi) * sin(theta), cos(theta)));

    /********************************PARTICLE ENERGY********************************/
    // Set the particle energy according to the muon energy distribution at the surface
    TF1 fSpectrum = TF1("f4", "0.14*TMath::Power(x,-2.7)*(1/(1+(1.1*x*TMath::Cos([0]))/115)+0.054/(1+(1.1*x*TMath::Cos([0]))/850))", minBeamEnergy, maxBeamEnergy);
    fSpectrum.SetParameter(0, theta);
    G4double energy = (fSpectrum.GetRandom()) * GeV;
    fParticleGun->SetParticleEnergy(energy);

    /******************************** SAVE THE DATA ********************************/
    int evID = event->GetEventID();

    // Get the DataManager object of the i-th thread
    DataManagerMT *dataManagerMT = DataManagerMT::GetInstance();
    int threadID = G4Threading::G4GetThreadId();
    DataManager *dataManager = dataManagerMT->GetSTDataManager(threadID);

    // Fill the event object with the data
    Event *ev = dataManager->GetEvent();
    ev->eventID = evID;
    ev->truthBeamData.posX = (double)(xPos / mm);
    ev->truthBeamData.posY = (double)(yPos / mm);
    ev->truthBeamData.posZ = (double)(zPos / mm);
    ev->truthBeamData.phi = (double)(phi);
    ev->truthBeamData.theta = (double)(theta);
    ev->truthBeamData.energy = (double)(energy / GeV);
    if (muCharge > 0.57) {
        ev->truthBeamData.ParticleID = 0; // mu-
    } else {
        ev->truthBeamData.ParticleID = 1; // mu+
    }

    /********************************PARTICLE GENERATION****************************/
    // Generate the primary particle
    fParticleGun->GeneratePrimaryVertex(event);
}
