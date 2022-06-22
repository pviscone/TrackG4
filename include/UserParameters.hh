#pragma once
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include <thread>

/**
 * @namespace TriggerParameters
 * @brief Just a container for the parameter of the Trigger
 */
namespace TriggerParameters {
inline G4double energyCut = 40 * keV;
}

/**
 * @brief Calculate the length of the side of the box given the number of modules, their size and spacing
 * @param nOfModulesOnAxis Number of modules on each axis
 * @param moduleSize Size of the module
 * @param moduleSpacing Spacing between the modules
 * @return The length of the side of the box
 */
inline auto sideLength = [](G4double moduleDimOnAxis, G4double spacingOnAxis, G4int nOfCols) {
    return (moduleDimOnAxis + spacingOnAxis) * (nOfCols + 1);
};

/**
 * @namespace SystemParameters
 * @brief  Just a container for the system parameters of the simulation
 *
 */
namespace SystemParameters {
inline int nThreads = std::thread::hardware_concurrency();
}

/**
 * @namespace ReadOutParameters
 * @brief Just a container for the parameters of the readout
 *
 * This namespace contains all the parameters that describe the readout.
 * (number and dimension of the pixels and the strips)
 *
 */
namespace ReadOutParameters {
inline G4int nXStrips = 960;
inline G4double stripDimX = 0.1 * mm;
inline G4int nYStrips = 2;
inline G4double stripDimY = 25 * mm;
inline G4int nXPixels = 960;
inline G4double pixelDimX = 0.1 * mm;
inline G4int nYPixels = 32;
inline G4double pixelDimY = 1.5625 * mm;
} // namespace ReadOutParameters

/** @namespace GeometryParameters
 * @brief Just a container for the geometry parameters of the detector
 *
 * This namespace contains all the materials used in the simulation
 * and the geometrical parameters of the detector
 */
namespace GeometryParameters {
// Dimension of the silicon module
inline G4double moduleDimX = ReadOutParameters::nXStrips * ReadOutParameters::stripDimX;
inline G4double moduleDimY = ReadOutParameters::nYStrips * ReadOutParameters::stripDimY;
inline G4double moduleDimZ = 0.3 * mm;
// Spacing between the modules
inline G4double moduleSpacingX = 10 * cm;
inline G4double moduleSpacingY = 10 * cm;
inline G4double moduleSpacingZ = 20 * cm;
// Spacing between pixels and strips
inline G4double pixelStripSpacing = 4 * mm;
// Number of silicon module in the x and y directions
inline G4int nOfColsX = 2;
inline G4int nOfColsY = 1;
inline G4int nOfLayers = 5;
// Put the world size equal the longest side of the detector multiplied by a factor chosen by the user
inline G4double worldSize = 1 * std::max({sideLength(moduleDimX, moduleSpacingX, nOfColsX), sideLength(moduleDimY, moduleSpacingY, nOfColsY), sideLength(moduleDimZ, moduleSpacingZ, nOfLayers)}) / 2;

}; // namespace GeometryParameters

/** @namespace Materials
 * @brief Just a container for the materials used in the simulation
 *
 * This namespace contains all the materials used in the simulation
 */
namespace Materials {
inline G4NistManager *nist = G4NistManager::Instance();
inline G4Material *Air = (nist)->FindOrBuildMaterial("G4_AIR");
inline G4Material *Si = (nist)->FindOrBuildMaterial("G4_Si");

}; // namespace Materials

/** @namespace BeamParameters
 * @brief Just a container for the beam parameters of the simulation
 *
 * This namespace contains all the energy parameters of the beam
 */
namespace BeamParameters {
inline double minBeamEnergy = 1;    // GeV
inline double maxBeamEnergy = 1000; // Gev
};                                  // namespace BeamParameters

/**
 * @namespace OutputParameters
 * @brief Just a container for the parameters of the output data
 *
 */
namespace OutputParameters {
inline std::string outputFileName = "output";
};