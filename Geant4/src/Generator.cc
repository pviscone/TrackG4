#include "Generator.hh"

MyPrimaryGenerator::MyPrimaryGenerator()
{

    //numero di particelle per evento
    fParticleGun = new G4ParticleGun(1);
    particleTable = G4ParticleTable::GetParticleTable();

}

MyPrimaryGenerator::~MyPrimaryGenerator()
{
    delete fParticleGun;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event *event)
{
    //G4cout<< "======== NEW BEAM =======" << G4endl;
    G4ParticleDefinition* mu_p = particleTable->FindParticle("mu+");
    G4ParticleDefinition* mu_n = particleTable->FindParticle("mu-");
    G4ParticleDefinition* particle;
    G4int partID = (int) (G4UniformRand()*2);
    G4double phi = G4UniformRand()*CLHEP::twopi;
    G4double theta;
    G4double xPos = G4UniformRand()*0.5*m-0.25*m;
    G4double yPos = G4UniformRand()*0.5*m-0.25*m;
    G4double zPos = -0.3*m;

    switch (partID)
    {
        case 0:
            particle=mu_n;
            break;
        case 1:
            particle=mu_p;
            break;
        default:
            break;
    }

    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleEnergy(4*GeV);

    fParticleGun->SetParticlePosition(G4ThreeVector(xPos,yPos,zPos));
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0,0,1));




    fParticleGun->GeneratePrimaryVertex(event);
}
