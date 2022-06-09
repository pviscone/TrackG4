#include "Geometry.hh"
#include "Physics.hh"
#include "Action.hh"
#include "G4UImanager.hh"
#include "G4RunManager.hh"
#include "G4MTRunManager.hh"
#include "G4VisExecutive.hh"
#include "G4VisManager.hh"
#include "G4UIExecutive.hh"
#include <thread>


int main(int argc, char **argv){

    // If Geant was compiled in multithreaded mode run the simulation in multi-threaded mode
    #ifdef G4MULTITHREADED
        G4int nThreads = std::thread::hardware_concurrency();
        G4MTRunManager* runManager = new G4MTRunManager;
        runManager->SetNumberOfThreads(nThreads);
        G4cout << "Using " << runManager->GetNumberOfThreads() << " threads" << G4endl;
    #else
        G4RunManager *runManager=new G4RunManager();
        G4cout << "NO threads" << G4endl;
    #endif

    // Set up the geometry, the physics and the actions and initialize the run manager
    runManager->SetUserInitialization(new MyDetectorConstruction());
    runManager->SetUserInitialization(new MyPhysicsList());
    runManager->SetUserInitialization(new MyActionInitialization);
    runManager->Initialize();

    // Set up the visualization
    G4UIExecutive *ui = new G4UIExecutive (argc,argv);
    G4VisManager *visManager= new G4VisExecutive();
    visManager->Initialize();
    G4UImanager *UImanager = G4UImanager::GetUIpointer();


    //Run the executable with no arguments to run the vis macro and start the interactive session
    //Run the executable with a macro file to run the macro and start the interactive session only if the macro vis.mac file is passed as argument
    G4String command = "/control/execute ";
    if(argc==1){
        std::cout << "No input file specified. Using default macro." << std::endl;
        UImanager->ApplyCommand(command+"./vis.mac");
        ui->SessionStart();
    }
    else{
        G4String key;
        for (int i=1; i<argc; ++i){
            key=argv[i];
            G4cout << "Executed macro:" << i << "  "<< key << G4endl;
            G4cout <<command+key<< G4endl;
            UImanager->ApplyCommand(command+key);
            if (key=="vis.mac"){
                ui->SessionStart();
            }
        }
    }

    //Deallocate manager's  pointers on the heap
    delete ui;
    delete visManager;
    delete runManager;

    return 0;
}
