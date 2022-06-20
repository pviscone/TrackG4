#include "../include/EventData.hh"
#include "../include/UserParameters.hh"
#include <TF1.h>
#include <TFile.h>
#include <TGraphErrors.h>
#include <TNtuple.h>
#include <TSystem.h>
#include <TTree.h>
#include <iostream>
#include <math.h>

/**
 * @struct FitData
 * @brief Struct that contains the results of the fit of the track
 *
 * The line is parametrized in the ZX and ZY plane in cartesian coordinates in the frame of the world of the simulation.
 * All the distance units are in mm
 *
 */
struct FitData {
    double x0;
    double x0_err;
    double y0;
    double y0_err;
    double mx;
    double mx_err;
    double my;
    double my_err;
    double chi2zx;
    double chi2zy;
};

/**
 * @brief Function that, given a event data object, returns the fitdata struct containing the results of the fit
 *
 * This is a simple toy model for tracking in which are performed two different fits in the ZX and ZY planes.
 * The frame of coordinate is the frame of the world of the simulation.
 *
 *
 * @param event Event class object containing the hits to be fitted
 * @return FitData The struct containing the result of the fit
 */
FitData Fit(Event *event) {
    FitData fitdata;
    // create the linear function to fit (a line in the range of the world size)
    TF1 line("line", "pol1", -GeometryParameters::worldSize, GeometryParameters::worldSize);

    // Store the hits
    std::vector<double> x = (event->detectorData).posX;
    std::vector<double> y = (event->detectorData).posY;
    std::vector<double> z = (event->detectorData).posZ;
    std::vector<double> dx, dy;
    std::vector<double> dz(x.size(), GeometryParameters::moduleDimZ / 2);
    // Create vectors of errors. The even layers are pixel module, the odds are strips.
    for (auto lay : (event->detectorData).Layer) {
        if (lay % 2 == 0) {
            dx.push_back(ReadOutParameters::pixelDimX / 2);
            dy.push_back(ReadOutParameters::pixelDimY / 2);
        } else {
            dx.push_back(ReadOutParameters::stripDimX / 2);
            dy.push_back(ReadOutParameters::stripDimY / 2);
        }
    }
    // Create the graph containing the hits in the ZX plane projection and fill the struct
    TGraphErrors GR_zx(x.size(), &z[0], &x[0], &dz[0], &dx[0]);
    GR_zx.Fit("line");
    fitdata.x0 = line.GetParameter(0);
    fitdata.x0_err = line.GetParError(0);
    fitdata.mx = line.GetParameter(1);
    fitdata.mx_err = line.GetParError(1);
    fitdata.chi2zx = line.GetChisquare();
    // Create the graph containing the hits in the ZY plane projection and fill the struct
    TGraphErrors GR_zy(y.size(), &z[0], &y[0], &dz[0], &dy[0]);
    GR_zy.Fit("line");
    fitdata.y0 = line.GetParameter(0);
    fitdata.y0_err = line.GetParError(0);
    fitdata.my = line.GetParameter(1);
    fitdata.my_err = line.GetParError(1);
    fitdata.chi2zy = line.GetChisquare();
    return fitdata;
}

/**
 * @brief Save the fit results in a root file
 *
 * The steps performed are:
 * 1. Load the root file and the branch containing the hits
 * 2. Create a new root file and a new NTuple
 * 3. Loop over the event and skip the events without hits
 * 4. Perform the fit
 * 5. Discard the results if are meaningless (x0,y0 outside the world box)
 * 6. Fill the NTuple with the fit results, save and close the file
 *
 *All the distance units are in mm
 */
int main() {
    // Load events file, dictionary, get the event tree, create a Event object and set the branch address to it
    gSystem->Load("../data/Event_Dict/AutoDict_Event_2947179584_cxx.so");
    TFile *file = new TFile("../data/output.root");
    TTree *tree = (TTree *)file->Get("Events");
    Event *event = new Event();
    tree->SetBranchAddress("Event", &event);

    // Create a file to store the results
    TFile *results = new TFile("../data/fit_results.root", "RECREATE");
    TNtuple *resNtuple = new TNtuple("fitResults", "fitResults", "evID:x0:x0_err:mx:mx_err:chi2zx:y0:y0_err:my:my_err:chi2zy");
    FitData fitdata;

    // Loop over the events in the file
    for (int i = 0; i < tree->GetEntries(); i++) {
        tree->GetEntry(i);

        // Skip the empty raw in detector data
        if ((event->detectorData.posX).empty()) {
            continue;
        } else {
            fitdata = Fit(event);
            // If the fit results are meaningless (x0,y0 outside the world size), skip the event
            if (abs(fitdata.x0) > GeometryParameters::worldSize || abs(fitdata.y0) > GeometryParameters::worldSize) {
                continue;
            }
            // Fill the ntuple with the fit results
            resNtuple->Fill(event->eventID, fitdata.x0, fitdata.x0_err, fitdata.mx, fitdata.mx_err, fitdata.chi2zx,
                            fitdata.y0, fitdata.y0_err, fitdata.my, fitdata.my_err, fitdata.chi2zy);
        }
    }
    // Write and close the file
    resNtuple->Write();
    results->Close();
    file->Close();
    return 0;
}