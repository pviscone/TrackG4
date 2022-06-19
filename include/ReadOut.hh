#pragma once
#include "DetectorGeometry.hh"
#include "UserParameters.hh"
#include <algorithm>
#include <limits>
#include <math.h>
#include <string>
#include <vector>

/**
 * @class ReadOut
 * @brief This class manages the readout of the simulation
 *
 *  This class is responsible to the transformation of the real hits into the position of the readout channels.
 *
 * As the other managers used in the simulation, this class is a singleton
 *
 */
class ReadOut {
private:
    /**
     * @brief Position of the center of the silicon modules on the x axis.
     */
    std::vector<G4double> xModulePosition;
    /**
     * @brief Position of the center of the silicon modules on the y axis.
     */
    std::vector<G4double> yModulePosition;
    /**
     * @brief Position of the center of the silicon modules on the z axis.
     */
    std::vector<G4double> zModulePosition;
    /**
     * @brief Private constructor (because is a singleton)
     */
    ReadOut();

    /**
     * @brief Single istance of the class
     */
    static ReadOut *instance;

public:
    ~ReadOut();

    /**
     * @brief Function that, given a vector and a value, find the closest value in the vector
     *
     * @param vec The vector in which we search the closest value
     * @param value The value we want to find the closest value in the vector
     * @return G4double The closest value in the vector to "value"
     */
    G4double closest_value(std::vector<G4double> vec, G4double value);

    /**
     * @brief Function to find the position on the axis of the pixel/strip channel that was hit
     *
     * @param hit_position Real position of the hit
     * @param modulePositions Position of the center of the all modules on the axis
     * @param pitchOnAxis Pitch of the pixel/strip on the axis
     * @return G4double Postion on the given axis of the pixel/strip channel that was hit
     */
    G4double closest_channel(G4double hit_position, std::vector<G4double> modulePositions, G4double pitchOnAxis);

    /**
     * @brief Function to find the position on the axis of the pixel/strip channel that was hit
     *
     * @param hit_position Real position of the hit
     * @param axis Axis on which we want to find the channel ("x", "y" or "z")
     * @param moduleType Module type ("pixel" or "strip")
     * @return G4double Postion on the given axis of the pixel/strip channel that was hit
     */
    G4double closest_channel(G4double hit_position, const std::string &axis, const std::string &moduleType);

    /**
     * @brief Function to find the position on the axis of the pixel/strip channel that was hit
     *
     * @param hit_position Real position of the hit
     * @param axis Axis on which we want to find the channel ("x", "y" or "z")
     * @param layerNumber Number of the layer that was hit (the odds are pixels, the evens are strips)
     * @return G4double Postion on the given axis of the pixel/strip channel that was hit
     */
    G4double closest_channel(G4double hit_position, const std::string &axis, int layerNumber);

    /**
     * @brief Get the Instance object
     * If the instance is not created, it will be created
     * If the instance is already created, it will be returned the pointer to the previous instance
     *
     * @return ReadOut*
     */
    static ReadOut *GetInstance();
};