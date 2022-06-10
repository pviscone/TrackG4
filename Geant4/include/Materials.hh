#pragma once
#include "G4NistManager.hh"
#include "G4Material.hh"

/** @struct MyMaterials
 * @brief The Materials class
 *
 * This class contains the materials used in the simulation.
 */
struct{
        G4NistManager* nist = G4NistManager::Instance();
        G4Material* Air = (this->nist)->FindOrBuildMaterial("G4_AIR");
        G4Material* Si = (this->nist)->FindOrBuildMaterial("G4_Si");
} MyMaterials;
