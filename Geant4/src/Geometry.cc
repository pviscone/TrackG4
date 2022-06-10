#include "Geometry.hh"
#include "Materials.hh"

MyDetectorConstruction::MyDetectorConstruction(){}

MyDetectorConstruction::~MyDetectorConstruction(){}


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

    //Dimension of the silicon module
    G4double moduleDimX=20*cm;
    G4double moduleDimY=5*cm;
    G4double moduleDimZ=1*mm;
    //Spacing between the modules
    G4double moduleSpacingX=10*cm;
    G4double moduleSpacingY=3*cm;
    G4double moduleSpacingZ=20*cm;
    //Number of silicon module in the x and y directions
    G4int nOfColsX=3;
    G4int nOfColsY=1;
    G4int nOfLayers=6;

    //Size of the world box (compute the length of every side and get the largest one)
    auto sideLenght = [] (G4double moduleDimOnAxis, G4double spacingOnAxis, G4int nOfCols){
        return (moduleDimOnAxis+spacingOnAxis)*(nOfCols+1);};
    G4double worldSize=std::max({sideLenght(moduleDimX,moduleSpacingX,nOfColsX),
                                sideLenght(moduleDimY,moduleSpacingY,nOfColsY),
                                sideLenght(moduleDimZ,moduleSpacingZ,nOfLayers)})/2; // Divided by 2 because in G4box the dimension represent the half length

    //Create the world
    G4Box *solidWorld = new G4Box("World",worldSize,worldSize,worldSize);
    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld,MyMaterials.Air,"World");
    G4VPhysicalVolume *physWorld = new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),logicWorld,"World",0,false,0,false);

    //Create the logical volume of silicon module
    G4Box *solidSiModule = new G4Box("Module",moduleDimX/2,moduleDimY/2,moduleDimZ/2);
    G4LogicalVolume *logicSiModule = new G4LogicalVolume(solidSiModule,MyMaterials.Si,"Module");

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

