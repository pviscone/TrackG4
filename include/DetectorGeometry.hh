#pragma once
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Region.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VUserDetectorConstruction.hh"
#include "UserParameters.hh"
#include "cppitertools/enumerate.hpp"

/**
 * @brief The class that build the geometry of the simulation
 */
class MyDetectorConstruction : public G4VUserDetectorConstruction {

private:
    /**
     * @brief The logical volume of the sensitive silicon module
     */
    G4LogicalVolume *logicSiModule;

    /**
     * @brief The constructor of the sensitive volume
     */
    void ConstructSDandField();

public:
    /**
     * @brief Construct a new My Detector Construction object. Inherited from G4VUserDetectorConstruction
     */
    MyDetectorConstruction();
    /**
     * @brief Destroy the My Detector Construction object. Inherited from G4VUserDetectorConstruction
     */
    ~MyDetectorConstruction();

    /**
     * @brief Build the geometry of the simulation
     * @return The physical volume of the world
     */
    G4VPhysicalVolume *Construct();

    /**
     * @brief Compute the positions of the center of the modules on a given axis
     * @param nOfModulesOnAxis Number of modules on the axis
     * @param moduleSize Size of the module on a given axis
     * @param moduleSpacing Spacing between the modules on a given axis axis
     * @return std::vector<G4double> Vector that contains the position of the modules on a given axis
     */
    static std::vector<G4double> ComputeModulePosition(G4int nOfModulesOnAxis, G4double moduleSize, G4double moduleSpacing);
};
