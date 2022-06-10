#pragma once
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "G4UniformRandPool.hh"
#include "CLHEP/Units/PhysicalConstants.h"
#include "G4GeneralParticleSource.hh"

class MyPrimaryGenerator : public G4VUserPrimaryGeneratorAction{
    private:
        G4ParticleGun* fParticleGun;
        G4ParticleTable* particleTable;
    public:
        MyPrimaryGenerator();
        ~MyPrimaryGenerator();
        virtual void GeneratePrimaries(G4Event *event);
};