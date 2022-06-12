#pragma once
#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4SystemOfUnits.hh"






/** @namespace GeometryParameters
 * @brief Just a container for the geometry parameters of the detector
 *
 * This struct contains all the materials used in the simulation
 * and the geometrical parameters of the detector
 */
namespace GeometryParameters{
        //Dimension of the silicon module
        inline G4double moduleDimX=20*cm;
        inline G4double moduleDimY=5*cm;
        inline G4double moduleDimZ=1*mm;
        //Spacing between the modules
        inline G4double moduleSpacingX=10*cm;
        inline G4double moduleSpacingY=10*cm;
        inline G4double moduleSpacingZ=20*cm;
        //Number of silicon module in the x and y directions
        inline G4int nOfColsX=3;
        inline G4int nOfColsY=6;
        inline G4int nOfLayers=6;
        //Put the world size equal the longest side of the detector * 2
        inline auto sideLength = [] (G4double moduleDimOnAxis, G4double spacingOnAxis, G4int nOfCols){
                return (moduleDimOnAxis+spacingOnAxis)*(nOfCols+1);
                };
        inline G4double worldSize=2*std::max({sideLength(moduleDimX,moduleSpacingX,nOfColsX),
                                sideLength(moduleDimY,moduleSpacingY,nOfColsY),
                                sideLength(moduleDimZ,moduleSpacingZ,nOfLayers)})/2;


};

/** @namespace Materials
 * @brief Just a container for the materials used in the simulation
 *
 * This struct contains all the materials used in the simulation
 */
namespace Materials{
        inline G4NistManager* nist = G4NistManager::Instance();
        inline G4Material* Air = (nist)->FindOrBuildMaterial("G4_AIR");
        inline G4Material* Si = (nist)->FindOrBuildMaterial("G4_Si");

};

/** @namespace BeamParameters
 * @brief Just a container for the beam parameters of the simulation
 *
 * This namespace contains all the energy parameters of the beam
 */
namespace BeamParameters{
        inline double minBeamEnergy=1;          //GeV
        inline double maxBeamEnergy=1000;       //Gev
};
