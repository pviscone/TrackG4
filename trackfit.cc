#include "./include/Fit.hh"

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
    //Create imgpath to store the images. Leave empty if you don't want to save the images
    std::string imgpath = "../data/img";
    imgpath="";

    //Set the file path
    std::string input_root_file="../data/output.root";
    std::string event_dict="../data/Event_Dict/AutoDict_Event_2947179584_cxx.so";
    std::string output_root_file="../data/fit_results.root";


    // Load events file, dictionary, get the event tree, create a Event object and set the branch address to it
    gSystem->Load(event_dict.c_str());
    TFile *file = new TFile(input_root_file.c_str());
    TTree *tree = (TTree *)file->Get("Events");
    Event *event = new Event();
    tree->SetBranchAddress("Event", &event);

    //Create the function to fit
    TFile *results = new TFile(output_root_file.c_str(), "RECREATE");

    // Create a file to store the results
    TNtuple *resNtuple = new TNtuple("fitResults", "fitResults", "evID:nHit:x0:x0_err:mx:mx_err:chi2zx:y0:y0_err:my:my_err:chi2zy");
    FitData fitdata;
    int nonEmptyEventCounter = 0;
    // create the linear function to fit (a line in the range of the world size)
    TF1 * line=new TF1("line", "pol1", -GeometryParameters::worldSize, GeometryParameters::worldSize);
    // Loop over the events in the file
    for (int i = 0; i < tree->GetEntries(); i++) {
        tree->GetEntry(i);

        // Skip the empty raw in detector data
        if ((event->detectorData.posX).empty()) {
            continue;
        } else {
            nonEmptyEventCounter++;

            // Perform the fit
            fitdata = Fit(event,line, imgpath, i);

            // If the fit results are meaningless (x0,y0 outside the world size or nan) or unsuccessful, skip the event
            if ((fitdata.status) || (abs(fitdata.x0) > GeometryParameters::worldSize) || (abs(fitdata.y0) > GeometryParameters::worldSize)
                || (abs(fitdata.x0_err) > GeometryParameters::worldSize) || (abs(fitdata.y0_err) > GeometryParameters::worldSize)
                || (std::isnan(fitdata.x0)) || (std::isnan(fitdata.y0)) || (std::isnan(fitdata.mx)) || (std::isnan(fitdata.my))
            ){
                continue;
            }
            // Fill the ntuple with the fit results
            resNtuple->Fill(event->eventID,fitdata.nHit, fitdata.x0, fitdata.x0_err, fitdata.mx, fitdata.mx_err, fitdata.chi2zx,
                            fitdata.y0, fitdata.y0_err, fitdata.my, fitdata.my_err, fitdata.chi2zy);
        }
    }
    std::cout << "Number of events with hits: " << nonEmptyEventCounter << std::endl;
    // Write and close the file
    resNtuple->Write();
    results->Close();
    file->Close();
    return 0;
}