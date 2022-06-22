#pragma once
#include "CLHEP/Units/PhysicalConstants.h"
#include "DataManager.hh"
#include "DataManagerMT.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UniformRandPool.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "TF1.h"
#include "UserParameters.hh"

/**
 * @class MyPrimaryGenerator
 * @brief The generator of the primary beam.
 *
 * This class manages the primary particles generation.
 */
class MyPrimaryGenerator : public G4VUserPrimaryGeneratorAction {
private:
    /**
     * @brief The particle gun.
     *
     * The particle gun is used to generate the primary particles.
     */
    G4ParticleGun *fParticleGun;

    /**
     * @brief Particle table.
     *
     * The particle table is used to find the particle definition.
     */
    G4ParticleTable *particleTable;

public:
    /**
     * @brief Constructor of the particle gun.
     *
     * This constructor initializes the particle gun.
     */
    MyPrimaryGenerator();

    /**
     * @brief Destructor of the particle gun.
     *
     * This destructor deletes the particle gun.
     */
    ~MyPrimaryGenerator();

    /**
     * @brief Generate the primary particles.
     *
     * This method runs every time a beam is generated and it takes care of
     * generation of the primary particles.
     * @param event The event to fill with the primary particles.
     */
    virtual void GeneratePrimaries(G4Event *event);
};