#include "ReadOut.hh"

using namespace ReadOutParameters;
using namespace GeometryParameters;

ReadOut *ReadOut::instance = nullptr;

/**
 * Initialize the modulePosition vectors with the values of the module position
 * This avoid to compute the module position every time the function is called
 *
 */
ReadOut::ReadOut() : xModulePosition{MyDetectorConstruction::ComputeModulePosition(nOfColsX, moduleDimX, moduleSpacingX)},
                     yModulePosition{MyDetectorConstruction::ComputeModulePosition(nOfColsY, moduleDimY, moduleSpacingY)},
                     zModulePosition{MyDetectorConstruction::ComputeModulePosition(nOfLayers, moduleDimZ, moduleSpacingZ)} {
}

ReadOut::~ReadOut() {}

ReadOut *ReadOut::GetInstance() {
    if (instance == nullptr) {
        instance = new ReadOut();
    }
    return instance;
}

/**
 * The closest value is found sorting the vector and then finding the closest value iterating on the vector.
 * When the distance between the element of the array and the value is greater than the previous distance,
 * the for loop is broken and the element of the vector is returned.
 *
 */
G4double ReadOut::closest_value(std::vector<G4double> vec, G4double value) {
    // Set closest do the maximum value of G4double
    G4double closest = std::numeric_limits<double>::max();
    // sort the vector
    std::sort(vec.begin(), vec.end());

    // iterate on the vector and compute the distance between the value and the element of the vector
    // When the distance starts to grow stop the loop
    for (auto elem : vec) {
        if (std::abs(elem - value) < std::abs(closest - value)) {
            closest = elem;
        } else {
            break;
        }
    }
    return closest;
}

/**
 * The function finds the position of the center of the closest silicon module on a given axis and then
 * translate the hit position on the frame of the module.
 *
 * The position of the channel in this frame  is computed by the formula:
 * \f$ (\floor{h/p}+0.5)*p   \f$
 * where: \n
 * - h is the hit position in the frame of the module \n
 * - p is the pitch of the pixel/strip on the given axis \n
 *
 * The value obtained is translated back to the original frame of the detector
 *
 * Achtung: this function works only if the number of channel on the given axis is even. If the number of channels is odd,
 * there will be an offset of 0.5 of the pitch of the channel on the axis.
 *
 */
G4double ReadOut::closest_channel(G4double hit_position, std::vector<G4double> modulePositions, G4double pitchOnAxis) {

    G4double positionOfHitModule = closest_value(modulePositions, hit_position);
    G4double hitPositionTranslated = hit_position - positionOfHitModule;

    return ((std::floor(hitPositionTranslated / pitchOnAxis)));
}

/**
 * It is just a wrapper to the function G4double closest_channel(G4double hit_position, std::vector<G4double> modulePositions, G4double pitchOnAxis)
 */
G4double ReadOut::closest_channel(G4double hit_position, const std::string &axis, const std::string &moduleType) {
    std::vector<G4double> modulePositions;
    G4double pitchOnAxis;

    // Check that axis and moduleType are valid
    if ((axis != "x") && (axis != "y") && (axis != "z")) {
        throw std::invalid_argument("Axis must be x, y or z");
    }
    if ((moduleType != "pixel") && (moduleType != "strip")) {
        throw std::invalid_argument("moduleType must be pixel or strip");
    }

    // Set the correct parameters for the given axis
    if (axis == "x") {
        modulePositions = xModulePosition;
    } else if (axis == "y") {
        modulePositions = yModulePosition;
    } else if (axis == "z") {
        modulePositions = zModulePosition;
        pitchOnAxis = moduleDimZ;
        if (moduleType == "pixel") {
            return closest_value(modulePositions, hit_position + pixelStripSpacing / 2) - pixelStripSpacing / 2;
        } else if (moduleType == "strip") {
            return closest_value(modulePositions, hit_position - pixelStripSpacing / 2) + pixelStripSpacing / 2;
        }
    }
    // Set the correct parameters for the given moduleType
    if (moduleType == "pixel") {
        if (axis == "x") {
            pitchOnAxis = pixelDimX;
        } else if (axis == "y") {
            pitchOnAxis = pixelDimY;
        }
    } else if (moduleType == "strip") {
        if (axis == "x") {
            pitchOnAxis = stripDimX;
        } else if (axis == "y") {
            pitchOnAxis = stripDimY;
        }
    }

    return closest_channel(hit_position, modulePositions, pitchOnAxis);
}

/**
 * It is just a wrapper to the function G4double closest_channel(G4double hit_position, std::string axis, std::string moduleType)
 */
G4double ReadOut::closest_channel(G4double hit_position, const std::string &axis, int layerNumber) {
    if (layerNumber % 2 != 0) {
        return closest_channel(hit_position, axis, "pixel");
    } else {
        return closest_channel(hit_position, axis, "strip");
    }
}