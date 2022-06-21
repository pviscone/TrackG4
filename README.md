<a href="https://pviscone.github.io/TrackG4/">
  <img align="center" src="https://github.com/pviscone/TrackG4/actions/workflows/docs.yml/badge.svg" />
</a>
# TrackG4

In this project is the simulation with Geant4 of the commissioning with cosmic rays of the PS module for the future CMS outer tracker detector.

  With the data obtained by the simulation the muons track are reconstructed with a simple toy model for tracking.
**warning: Due to the lack of compatibility between RunManager and RunManagerMT the simulation has to run in multithreading mode.
You can set the number of thread in the G4main.cc but you have to compile Geant with the flag -DG4_USE_THREADS.**

## How to run the simulation
1. Build and run the simulation in batch mode running the script G4batch.sh in the main folder. 
The data will be saved in output.root in the build folder
2. To run the tests runs the tests_executable in the build folder
3. To run the tracking run track in the build folder
   (By default it takes the input data from oputput.root in the data folder and save the fit results in fits_result.root in the data folder.If you want to modify this path or save the plot images change the paths and the flags in trackfit.cc before step 1)


# PS module
  A PS module is composed by a layer of strips and a layer of pixels.
  The dimension of a single layer is 10cm x 5cm x 0.3mm and two layers are spaced by 4mm.

  The strip module is composed by 2 columns of 960 strips
  The pixel module is composed by 32 columns of 960 pixels

  In the commissioning the PS modules are placed in a grid of 2 columns, 1 row and 5 layers.

  All the geometrical parameters can be modified in the header file ./include/UserParameters.hh
  The default parameters for the spacing values in the grid are 10cm x 10cm x 20cm

  \section Simulation

  The simulation is performed with the Geant4.
  The physics module used is G4EmStandardPhysics.

  ## Particle Gun
  The class that manage the particle gun is MyGenerator in the ./src/Generator.cc file.

  The particle gun generate positive and negative muons according to their real charge ratio

  The beam is generated in a random position of the face of the world box perpendicular
  to the z axis and its direction is parametrized by a spherical unit vector
  The polar angle of the direction vector of the beam is randomly generated between 0 and 2 pi

  The azimuthal angle theta of the direction vector beam is randomly generated between 0 and pi
  according to the distribution cos^2(theta)


  The particles are gerenated with a energy between minBeamEnergy and maxBeamEnergy
  (defined by the user in UserParameters.hh, default 1 and 1000 GeV)
  according to the energy distribution for cosmic muons at the sea surface

  This distribution are generated using a TF1 object from ROOT

  To limit the presence of delta rays a range cut is applied in the run.mac macro
  The defaults values are: 10cm in the air and 0.1mm in the silicon

  ## Trigger

  A simple trigger was also implemented:
  - If the released energy of the hit is less than the threshold, the hit is discarded
  - If there isn't at list two hit in 2 different PS layers, the hit is discarded

  The energy threshold is set to 40 keV and can be modified in the header file ./include/UserParameters.hh

  ## ReadOut
  The readout is performed by the class MyReadOut in the ./src/ReadOut.cc file.

  ## Output data
  The results of the simulation are stored in a root file that contains a branch containing a event object defined in the ./include/EventData.hh file.

  During the simulation the data are managed by the class DataManagerMT. Each thread has its own DataManager object and manage a different file.
  DatamanagerMT is a singleton that contains a vector of DataManager objects.

  - In the method MyGenerator::GeneratePrimaries that runs every time a beam is created the DataManagerMT object fill the event object with
  the data of the beam
  - In the method MySensitiveDetector::ProcessHits that runs at each step in the PS module the DataManagerMT object fill the event object with the data of the hit
  - In the method MyEventAction::EndOfEventAction that runs at the end of the event the DataManagerMT object fill the event object with the data of the event
  - In the method MyRunAction::EndOfRunAction that runs at the end of the run the DataManagerMT object write and close the file

 # Tracking
 The tracking algorithm is a simple toy model that consist in a simple linear fit on the hits in the XZ and YZ projection.
 Some tracks are not well fitted due to the presence of hits of delta rays. A simple filter to remove some of this tracks consists in
  remove all the fit with parameters that force the linear function to be outside the world box.

  The results of the fit (parameters and their errors) are stored in a root file.
