#include "Detector.hh"

using namespace GeometryParameters;
using namespace Materials;

MyDetectorConstruction::MyDetectorConstruction(){}

MyDetectorConstruction::~MyDetectorConstruction(){}

/**
 * If the number of units is odd then it add to the position vector 0 and \f$ \pm \f$ i*(size+spacing)/2 where i is the loop index and start form 1 to nOfUnits+1/2 \n
 * If the number of units is even then it add to the vector  \f$ \pm \f$ (i+1/2)*(size+spacing)/2 where i is the loop index and start form 0 to nOfUnits/2
 */
std::vector <G4double> MyDetectorConstruction::ComputeModulePosition(G4int nOfModulesOnAxis,G4double moduleSize,G4double moduleSpacing){
    std::vector <G4double> modulePosition;
    int nOfLoops=nOfModulesOnAxis;  //number of loops
    G4double offset; //Position offset
    int startloop; //Value of the index of the first loop

    //Distinguish the case of odd and even number of modules
    if (nOfModulesOnAxis%2==1){
        modulePosition.push_back(0);
        nOfLoops=((nOfLoops+1)/2);
        offset=0;
        startloop=1;
    }
    else{
        nOfLoops=nOfLoops/2;
        offset=(moduleSpacing+moduleSize)/2;
        startloop=0;
    }
    //push_back in the position vector the position of the center of the modules
    for (;startloop<nOfLoops;startloop++){
        modulePosition.push_back(offset+startloop*(moduleSpacing+moduleSize));
        modulePosition.push_back(-offset-startloop*(moduleSpacing+moduleSize));
    }
    return modulePosition;
}


G4VPhysicalVolume *MyDetectorConstruction::Construct(){

    //Size of the world box (compute the length of every side and get the largest one)
     // Divided by 2 because in G4box the dimension represent the half length

    //Create the world
    G4Box *solidWorld = new G4Box("World",worldSize,worldSize,worldSize);
    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld,Air,"World");
    G4VPhysicalVolume *physWorld = new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),logicWorld,"World",0,false,0,false);

    //Create the logical volume of silicon module
    G4Box *solidSiModule = new G4Box("Module",moduleDimX/2,moduleDimY/2,moduleDimZ/2);
    G4LogicalVolume *logicSiModule = new G4LogicalVolume(solidSiModule,Si,"Module");

    //For loop on the positions vectors to create the physical volume of the modules
    for (auto layerPos : ComputeModulePosition(nOfLayers,moduleDimZ,moduleSpacingZ)){
        for (auto colPosX : ComputeModulePosition(nOfColsX,moduleDimX,moduleSpacingX)){
            for (auto colPosY : ComputeModulePosition(nOfColsY,moduleDimY,moduleSpacingY)){
                new G4PVPlacement(0,G4ThreeVector(colPosX,colPosY,layerPos),logicSiModule,"Module",logicWorld,false,0,false);
            }

        }
    }


    //Return the physical world
    return physWorld;
}

