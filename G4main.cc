/*! \mainpage TrackG4PS
 *
 *
 * This project is a simulation with Geant4 of the commissioning with cosmic rays of the PS module for the future CMS outer tracker detector.
 *
 * With the data obtained by the simulation the muons track are reconstructed with a simple toy model for tracking.
 *
 *  **warning**: Due to the lack of compatibility between RunManager and RunManagerMT the simulation has to run in multithreading mode.
 * You can set the number of thread in the UserParameters.hh but you have to compile Geant with the flag GEANT4_BUILD_MULTITHREADED ON.
 *
 * \subsection HOW How to run the simulation
 * 1. Build and run the simulation in batch mode running the script G4batch.sh in the main folder.
 * The data will be saved in output.root in the build folder
 * 2. To run the tests runs the tests_executable in the build folder
 * 3. To run the tracking algorithm run track in the build folder
 *  (By default it takes the input data from oputput.root in the data folder and save
 *  the fit results in fits_result.root in the data folder. If you want to modify this paths
 * or save the plot images change the paths and the flags in trackfit.cc before step 1)
 *
 * \section PSModules PS modules
 * A PS silicon module is composed by a layer of strips and a layer of pixels. \n
 * The dimension of a single layer is 10cm x 5cm x 0.3mm and two layers are spaced by 4mm.
 *
 * The strip module is composed by 2 columns of 960 strips \n
 * The pixel module is composed by 32 columns of 960 pixels
 *
 * In the commissioning the PS modules are placed in a grid of 2 columns, 1 row and 5 layers.
 *
 * All the geometrical parameters can be modified in the header file UserParameters.hh \n
 * The default parameters for the spacing values in the grid are 10cm x ... x 20cm
 *
 * \image html detector.png "Geometry of the detector" width=500
 *
 * \section Simulation
 *
 * The simulation is performed with the Geant4. \n
 * The physics module used is G4EmStandardPhysics.
 *
 * \subsection PG Particle Gun
 * The class that manage the particle gun is MyPrimaryGenerator .
 *
 * The particle gun generate positive and negative muons according to their real charge ratio \f$ \frac{\mu_{+}}{\mu_{-}} = 1.3 \f$
 *
 * The beam is generated in a random position of the face of the world box perpendicular
 * to the z axis and its direction is parametrized by a spherical unit vector (\f$ (\cos(\phi)\sin(\theta),\sin(\phi)\sin(\theta),\cos(\theta)) \f$) \n
 * The polar angle of the direction vector of the beam is randomly generated between 0 and \f$ 2 \pi \f$
 *
 * The azimuthal angle \f$ \theta \f$ of the direction vector beam is randomly generated between 0 and pi
 * according to the distribution \f$ \cos^2{\theta} \f$
 *
 *
 * The particles are generated with a energy between minBeamEnergy and maxBeamEnergy
 * (defined by the user in UserParameters.hh, default 1 and 1000 GeV)
 * according to the energy distribution for cosmic muons at the sea surface
 *
 * \f$ \frac{dN_{\mu}}{dE_{\mu} d \Omega} \approx \frac{0.14 E_{\mu}^{-2.7}}{cm^2 \; s \;sr \; GeV} \left( \frac{1}{1+\frac{1.1 E_{\mu}\cos{\theta}}{115 GeV}} + \frac{0.054}{1+\frac{1.1 E_{\mu}\cos{\theta}}{850 GeV}} \right)\f$
 *
 * This distribution are generated using a TF1 object from ROOT
 *
 * To limit the presence of delta rays a range cut is applied in the run.mac macro \n
 * The defaults values are: 10cm in the air and 0.1mm in the silicon
 *
 * \subsection Trigger
 *
 * A simple trigger was also implemented:
 * - If the released energy of the hit is less than the threshold, the hit is discarded
 * - If there isn't at least two hit in 2 different PS layers, the hit is discarded
 *
 * The energy threshold is set to 40 keV and can be modified in the header file UserParameters.hh
 *
 * \subsection ReadOut
 * The readout is performed by the class ReadOut in the ReadOut.cc file.
 *
 * \subsection DATA Output data
 * The results of the simulation are stored in a root file that contains a branch containing a event object defined in the EventData.hh file.
 *
 * During the simulation the data are managed by the class DataManagerMT. Each thread has its own DataManager object and manage a different file.
 * DatamanagerMT is a singleton that contains a vector of DataManager objects.
 *
 * - In the method MyPrimaryGenerator::GeneratePrimaries, that runs every time a beam is created, the DataManagerMT object fill the event object with
 * the data of the beam
 * - In the method MySensitiveDetector::ProcessHits, that runs at each step in the PS module, the DataManagerMT object fill the event object with the data of the hit
 * - In the method MyEventAction::EndOfEventAction, that runs at the end of the event, the DataManagerMT object fill the event object with the data of the event
 * - In the method MyRunAction::EndOfRunAction, that runs at the end of the run, the DataManagerMT object write and close the file
 *
 *\section Tracking
 *The tracking algorithm is a simple toy model that consist in a simple linear fit on the hits in the XZ and YZ projection.
 *Some tracks are not well fitted due to the presence of hits of delta rays. \n
 *
 * The results of the fit (parameters and their errors) are stored in a root file.
 */

#include "Action.hh"
#include "DetectorGeometry.hh"
#include "G4MTRunManager.hh"
#include "G4RunManager.hh"
#include "G4Threading.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4VisManager.hh"
#include "Physics.hh"
#include "UserParameters.hh"
#include <TROOT.h>
#include <exception>
#include <thread>

int main(int argc, char **argv) {

    gSystem->Load("AutoDict_Event_2947179584_cxx.so");
// gInterpreter->GenerateDictionary("Event;TruthBeamData;DetectorData","../include/EventData.hh");
//  If Geant was compiled in multithreaded mode run the simulation in multi-threaded mode
#ifdef G4MULTITHREADED
    // Needed by the TFunction in Generator.cc
    ROOT::EnableImplicitMT();
    ROOT::EnableThreadSafety();
    // Set the max number of threads that can run in parallel
    G4MTRunManager *runManager = new G4MTRunManager;
    runManager->SetNumberOfThreads(SystemParameters::nThreads);
#else
// raise an exception at compile time if the user tries to run the simulation in single-threaded mode
#error "Geant4 was not compiled in multithreaded mode. Please compile with GEANT4_BUILD_MULTITHREADED ON");
    // Single threaded mode disabled due to the lack of support of the G4RunManager
    // G4RunManager *runManager=new G4RunManager();
#endif

    // Set up the geometry, the physics and the actions and initialize the run manager
    runManager->SetUserInitialization(new MyDetectorConstruction());
    runManager->SetUserInitialization(new MyPhysicsList());
    runManager->SetUserInitialization(new MyActionInitialization());
    runManager->Initialize();

    // Set up the visualization
    G4UIExecutive *ui = new G4UIExecutive(argc, argv);
    G4UImanager *UImanager = G4UImanager::GetUIpointer();

    // Run the executable with no arguments to run the vis macro and start the interactive session
    // Run the executable with a macro file to run the macro and start the interactive session only if the macro vis.mac file is passed as argument
    G4String command = "/control/execute ";
    if (argc == 1) {
        std::cout << "No input file specified. Using default macro." << std::endl;
        G4VisManager *visManager = new G4VisExecutive();
        visManager->Initialize();
        UImanager->ApplyCommand(command + "./vis.mac");
        UImanager->ApplyCommand(command + "./run.mac");

        ui->SessionStart();
    } else {
        G4String key;
        for (int i = 1; i < argc; ++i) {
            key = argv[i];
            G4cout << "Executed macro:" << i << "  " << key << G4endl;
            G4cout << command + key << G4endl;
            UImanager->ApplyCommand(command + key);
            if (key == "vis.mac") {
                G4VisManager *visManager = new G4VisExecutive();
                visManager->Initialize();
                ui->SessionStart();
            }
        }
    }

    // Deallocate manager's  pointers on the heap
    delete ui;
    // delete visManager;
    delete runManager;

    return 0;
}
