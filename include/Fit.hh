#pragma once

#include "../include/EventData.hh"
#include "../include/UserParameters.hh"
#include <TAxis.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TFile.h>
#include <TGraphErrors.h>
#include <TNtuple.h>
#include <TSystem.h>
#include <TTree.h>
#include <iostream>
#include <math.h>
#include <numeric>
#include <string>

/**
 * @struct FitData
 * @brief Struct that contains the results of the fit of the track
 *
 * The line is parametrized in the ZX and ZY plane in cartesian coordinates in the frame of the world of the simulation.
 * \f$ x= m_x z + x_0 \f$ and \f$ y= m_y z + y_0 \f$
 *
 * All the distance units are in mm
 *
 * The data stored in the struct are:
 * - x0
 * - xo_err
 * - y0
 * - yo_err
 * - mx
 * - mx_err
 * - my
 * - my_err
 * - chi2zx (chi2 of the fit in the ZX plane)
 * - chi2zy (chi2 of the fit in the ZY plane)
 * - nHits (number of hits in the fit)
 *
 *
 */
struct FitData {
    long double x0;
    long double x0_err;
    long double y0;
    long double y0_err;
    long double mx;
    long double mx_err;
    long double my;
    long double my_err;
    long double chi2zx;
    long double chi2zy;
    int nHit;
    bool status;
};

/**
 * @brief Function that, given a event data object, returns the fitdata struct containing the results of the fit
 *
 * This is a simple toy model for tracking in which are performed two different fits in the ZX and ZY planes.
 * The frame of coordinate is the frame of the world of the simulation.
 *
 * The initial parameters for the fit are estimated computing the parameter of the line considering the first and the last point
 * on the z axis
 *
 *
 * @param event Event class object containing the hits to be fitted
 * @param imgpath Folder where the output images will be saved
 * @param i: index of the event
 *
 *
 * @return FitData The struct containing the result of the fit
 */
FitData Fit(Event *event, TF1 *line, std::string imgpath = "", int i = 0) {
    FitData fitdata;
    std::string name, title;
    long double prev_x0, prev_y0, prev_mx, prev_my;

    // Store the hits and order them according to the z coordinate
    std::vector<double> z = (event->detectorData).posZ;
    std::vector<double> x, y;
    std::vector<int> idx(z.size());
    std::iota(idx.begin(), idx.end(), 0);
    std::sort(idx.begin(), idx.end(), [z](const size_t &i1, const size_t &i2) { return z[i1] < z[i2]; });
    for (auto idx_val : idx) {
        x.push_back((event->detectorData).posX[idx_val]);
        y.push_back((event->detectorData).posY[idx_val]);
    }
    std::sort(z.begin(), z.end());

    // Create vectors of errors. The even layers are pixel module, the odds are strips.
    std::vector<double> dx, dy;
    std::vector<double> dz(x.size(), GeometryParameters::moduleDimZ / sqrt(12));
    for (auto lay : (event->detectorData).Layer) {
        if (lay % 2 == 0) {
            dx.push_back(ReadOutParameters::pixelDimX / sqrt(12));
            dy.push_back(ReadOutParameters::pixelDimY / sqrt(12));
        } else {
            dx.push_back(ReadOutParameters::stripDimX / sqrt(12));
            dy.push_back(ReadOutParameters::stripDimY / sqrt(12));
        }
    }
    // Create the graph containing the hits in the ZX plane projection and fill the struct with the fit results
    TCanvas c1("c1", "c1", 800, 600);
    TGraphErrors GR_zx(x.size(), &z[0], &x[0], &dz[0], &dx[0]);
    prev_mx = (x.back() - x[0]) / (z.back() - z[0]);
    prev_x0 = x[0] - prev_mx * z[0];
    line->SetParameter(0, prev_x0);
    line->SetParameter(1, prev_mx);
    auto status_zx = GR_zx.Fit("line", "Q");
    fitdata.x0 = line->GetParameter(0);
    fitdata.x0_err = line->GetParError(0);
    fitdata.mx = line->GetParameter(1);
    fitdata.mx_err = line->GetParError(1);
    fitdata.chi2zx = line->GetChisquare();

    // Save the ZX fig if saveFig is true
    if (imgpath != "") {
        title = "ZX evID:" + std::to_string(i);
        GR_zx.SetTitle(title.c_str());
        GR_zx.GetXaxis()->SetTitle("Z [mm]");
        GR_zx.GetYaxis()->SetTitle("X [mm]");
        GR_zx.Draw("AP");
        name = imgpath + "/zx_" + std::to_string(i) + ".png";
        c1.SaveAs(name.c_str());
    }

    // Create the graph containing the hits in the ZY plane projection and fill the struct with the fit results
    TCanvas c2("c2", "c2", 800, 600);
    TGraphErrors GR_zy(y.size(), &z[0], &y[0], &dz[0], &dy[0]);
    prev_my = (y.back() - y[0]) / (z.back() - z[0]);
    prev_y0 = y[0] - prev_my * z[0];
    line->SetParameter(0, prev_y0);
    line->SetParameter(1, prev_my);
    auto status_zy = GR_zy.Fit("line", "Q");
    fitdata.y0 = line->GetParameter(0);
    fitdata.y0_err = line->GetParError(0);
    fitdata.my = line->GetParameter(1);
    fitdata.my_err = line->GetParError(1);
    fitdata.chi2zy = line->GetChisquare();

    // Save the ZY fig if saveFig is true
    if (imgpath != "") {
        title = "ZY evID:" + std::to_string(i);
        GR_zy.SetTitle(title.c_str());
        GR_zy.GetXaxis()->SetTitle("Z [mm]");
        GR_zy.GetYaxis()->SetTitle("Y [mm]");
        GR_zy.Draw("AP");
        name = imgpath + "/zy_" + std::to_string(i) + ".png";
        c2.SaveAs(name.c_str());
    }
    // Check that the fit on both projection was successful
    fitdata.status = status_zx && status_zy;

    // Save the number of hits
    fitdata.nHit = z.size();

    // Return the fitdata struct
    return fitdata;
}