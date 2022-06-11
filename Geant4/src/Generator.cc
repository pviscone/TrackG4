#include "Generator.hh"

using namespace GeometryParameters;
/**
 * Create the particle gun and the particle table
 *
 */
MyPrimaryGenerator::MyPrimaryGenerator()
{

    fParticleGun = new G4ParticleGun(1);
    particleTable = G4ParticleTable::GetParticleTable();

}

/**
 * Destroy the particle gun
 *
 */
MyPrimaryGenerator::~MyPrimaryGenerator()
{
    delete fParticleGun;
}

/**
 *Every time a beam is generated this function generate randomly a muon
 *according to the muons charge ratio (mu+/mu- = 1.3)
 *
 * The beam is generated in a random position in the face of the world perpendicular
 * to the z axis.
 *
 */
void MyPrimaryGenerator::GeneratePrimaries(G4Event *event)
{


    //Definition of mu+ and mu-
    G4ParticleDefinition* mu_p = particleTable->FindParticle("mu+");
    G4ParticleDefinition* mu_n = particleTable->FindParticle("mu-");
    //Dummy variable to store the particle definition
    G4ParticleDefinition* particle;
    // Random number to generate random mu+ or mu- according to their charge ratio
    G4double muCharge = (G4UniformRand());
    // Starting point of the beam.
    // It covers uniformly the whole side of the box
    G4double xPos = G4UniformRand()*worldSize*2-worldSize;
    G4double yPos = G4UniformRand()*worldSize*2-worldSize;
    G4double zPos = -worldSize;

    //The charge ratio of cosmic muons (mu+/mu-) is 1.3 (assumed constant in energy)
    //This means that the 57% of the muons are mu+ and 43% of them are mu-
    if(muCharge>0.57)
    {
        particle=mu_p;
    }
    else
        particle=mu_n;
    }
    //Set the particle definition
    fParticleGun->SetParticleDefinition(particle);
    //Set the starting point of the beam
    fParticleGun->SetParticlePosition(G4ThreeVector(xPos,yPos,zPos));

    //TODO Set the energy and direction distribution with gps macros
    fParticleGun->SetParticleEnergy(4*GeV);
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0,0,1));



    //Generate the primary particle
    fParticleGun->GeneratePrimaryVertex(event);
}
