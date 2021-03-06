<a href="https://pviscone.github.io/TrackG4/">
  <img align="center" src="https://github.com/pviscone/TrackG4/actions/workflows/docs.yml/badge.svg" />
</a>

<a href="https://github.com/pviscone/TrackG4/blob/main/report/TrackG4PS.pdf">
  <img align="center" src="https://img.shields.io/badge/-Report-blue" />
</a>

<a href="https://github.com/pviscone/TrackG4/blob/main/report/Presentation.pdf">
  <img align="center" src="https://img.shields.io/badge/-Presentation-orange" />
</a>



# TrackG4PS

This project is a simulation with Geant4 of the commissioning with cosmic rays of the PS module for the future CMS outer tracker detector.

With the data obtained by the simulation the muons track are reconstructed with a simple toy model for tracking.

**warning: Due to the lack of compatibility between RunManager and RunManagerMT methods, the simulation has to run in multithreading mode.
You can set the number of thread in the UserParameters.hh but you have to compile Geant with the flag -GEANT4_BUILD_MULTITHREADED.**

### How to run the simulation
1. Build and run the simulation in batch mode running the script G4batch.sh in the main folder.
The data will be saved in output.root in the build folder
2. To run the tests runs the tests_executable in the build folder
3. To run the tracking algorithm run track in the build folder
    (By default it takes the input data from oputput.root in the data folder and save
    the fit results in fits_result.root in the data folder. If you want to modify this paths
    or save the plot images change the paths and the flags in trackfit.cc before step 1)

## PS modules

<img src="docs/img/ps_module.png" style="zoom:20%;" />


A PS silicon module is composed by a layer of strips and a layer of pixels.
The dimension of a single layer is 10cm x 5cm x 0.3mm and two layers are spaced by 4mm.

The strip module is composed by 2 columns of 960 strips
The pixel module is composed by 32 columns of 960 pixels

In the commissioning the PS modules are placed in a grid of 2 columns, 1 row and 5 layers.

All the geometrical parameters can be modified in the header file UserParameters.hh
The default parameters for the spacing values in the grid are 10cm x ... x 20cm

<img src="docs/img/detector.png" style="zoom:40%;" />

## Simulation

The simulation is performed with the Geant4.
The physics module used is G4EmStandardPhysics.

### Particle Gun
The class that manage the particle gun is MyPrimaryGenerator .

The particle gun generate positive and negative muons according to their real charge ratio mu+/mu- = 1.3 

The beam is generated in a random position of the face of the world box perpendicular
to the z axis and its direction is parametrized by a spherical unit vector.
The polar angle of the direction vector of the beam is randomly generated between 0 and 2 pi

The azimuthal angle theta of the direction vector beam is randomly generated between 0 and pi
according to the distribution cos^2(theta)


The particles are generated with a energy between minBeamEnergy and maxBeamEnergy
(defined by the user in UserParameters.hh, default 1 and 1000 GeV)
according to the energy distribution for cosmic muons at the sea surface

This distribution are generated using a TF1 object from ROOT

To limit the presence of delta rays a range cut is applied in the run.mac macro
The defaults values are: 10cm in the air and 0.1mm in the silicon

### Trigger

A simple trigger was also implemented:
- If the released energy of the hit is less than the threshold, the hit is discarded
- If there isn't at least two hit in 2 different PS layers, the hit is discarded

The energy threshold is set to 40 keV and can be modified in the header file UserParameters.hh

### ReadOut
The readout is performed by the class ReadOut in the ReadOut.cc file.

### Output data
The results of the simulation are stored in a root file that contains a branch containing a event object defined in the EventData.hh file.

During the simulation the data are managed by the class DataManagerMT. Each thread has its own DataManager object and manage a different file.
DatamanagerMT is a singleton that contains a vector of DataManager objects.

- In the method MyPrimaryGenerator::GeneratePrimaries, that runs every time a beam is created, the DataManagerMT object fill the event object with
the data of the beam
- In the method MySensitiveDetector::ProcessHits, that runs at each step in the PS module, the DataManagerMT object fill the event object with the data of the hit
- In the method MyEventAction::EndOfEventAction, that runs at the end of the event, the DataManagerMT object fill the event object with the data of the event
- In the method MyRunAction::EndOfRunAction, that runs at the end of the run, the DataManagerMT object write and close the file

## Tracking
The tracking algorithm is a simple toy model that consist in a simple linear fit on the hits in the XZ and YZ projection.
Some tracks are not well fitted due to the presence of hits of delta rays.

The results of the fit (parameters and their errors) are stored in a root file.
