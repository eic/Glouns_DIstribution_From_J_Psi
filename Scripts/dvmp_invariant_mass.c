#include <TFile.h>
#include <TH1.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TLine.h>
#include <TSystem.h>
#include <TStyle.h>
#include <iostream>

using namespace std;

void dvmp_invariant_mass()
{
    gSystem->mkdir("dvmp_jpsi_plots", kTRUE);
    gStyle->SetOptStat(0);

    //---------------------------------------------
    // Open ROOT file
    //---------------------------------------------
    TFile *file =
        TFile::Open("dvmp_benchmark_data_10x130_May_2026_run_0.root");

    if (!file || file->IsZombie())
    {
        cerr << "ERROR: Cannot open ROOT file" << endl;
        return;
    }

    //---------------------------------------------
    // Get J/psi mass histogram
    //---------------------------------------------
    TH1 *jpsimass = nullptr;

    file->GetObject("h_jpsi_mass_RC", jpsimass);

    if (!jpsimass)
    {
        cerr << "ERROR: Cannot find histogram h_jpsi_mass_RC" << endl;
        file->Close();
        return;
    }

    //---------------------------------------------
    // Fit range
    //---------------------------------------------
    double fitMin = 3.044;
    double fitMax = 3.190;

    TF1 *fitFcn =
        new TF1(
            "fitFcn",
            "gaus(0) + pol1(3)",
            fitMin,
            fitMax
        );

    fitFcn->SetParameters(
        jpsimass->GetMaximum(),
        3.096,
        0.04,
        10.0,
        -1.0
    );

    fitFcn->SetParLimits(1, 3.0, 3.15);
    fitFcn->SetParLimits(2, 0.005, 0.15);

    fitFcn->SetNpx(1000);

    //---------------------------------------------
    // Fit
    //---------------------------------------------
    jpsimass->Fit(fitFcn, "RQE");

    //---------------------------------------------
    // Style
    //---------------------------------------------
    jpsimass->SetTitle("");
    jpsimass->SetStats(0);

    jpsimass->SetLineColor(kBlack);
    jpsimass->SetLineWidth(3);

    jpsimass->GetXaxis()->SetRangeUser(2.85, 3.32);
    jpsimass->GetXaxis()->SetTitle("M_{e^{+}e^{-}} [GeV]");
    jpsimass->GetYaxis()->SetTitle("Counts");

    jpsimass->GetXaxis()->SetTitleOffset(1.25);
    jpsimass->GetYaxis()->SetTitleOffset(1.45);
    jpsimass->GetYaxis()->SetMaxDigits(3);

    fitFcn->SetLineColor(kRed);
    fitFcn->SetLineWidth(3);

    //---------------------------------------------
    // Canvas
    //---------------------------------------------
    TCanvas *canvas =
        new TCanvas("c_jpsi_invariant_mass", "", 900, 800);

    canvas->SetLeftMargin(0.14);
    canvas->SetBottomMargin(0.12);

    jpsimass->Draw("HIST");
    fitFcn->Draw("SAME");

    //---------------------------------------------
    // Extract parameters
    //---------------------------------------------
    double amp      = fitFcn->GetParameter(0);
    double mean     = fitFcn->GetParameter(1);
    double sigma    = fitFcn->GetParameter(2);
    double constBkg = fitFcn->GetParameter(3);
    double slopeBkg = fitFcn->GetParameter(4);

    double meanErr  = fitFcn->GetParError(1);
    double sigmaErr = fitFcn->GetParError(2);

    //---------------------------------------------
    // Legends
    //---------------------------------------------
    TLegend *leg =
        new TLegend(0.6, 0.8, 0.9, 0.88);

    leg->SetBorderSize(0);
    leg->SetFillStyle(0);
    leg->AddEntry(jpsimass, "RECO J/#psi", "l");
    leg->AddEntry(fitFcn, "Gaussian + linear fit", "l");
    leg->Draw();

    TLegend *info =
        new TLegend(0.05, 0.75, 0.5, 0.85);

    info->SetBorderSize(0);
    info->SetFillStyle(0);

    //info->AddEntry((TObject*)nullptr, "#bf{ePIC Performance}", "");
    //info->AddEntry((TObject*)nullptr, "e+p DVMP J/#psi #rightarrow e^{+}e^{-}", "");
    //info->AddEntry((TObject*)nullptr, "10 #times 130 GeV, 26.03.1", "");
    info->AddEntry((TObject*)nullptr, Form("#mu = %.4f #pm %.4f GeV", mean, meanErr), "");
    info->AddEntry((TObject*)nullptr, Form("#sigma = %.4f #pm %.4f GeV", sigma, sigmaErr), "");
    info->AddEntry((TObject*)nullptr, Form("%.2f < M_{e^{+}e^{-}} < %.2f GeV", fitMin, fitMax), "");

    info->Draw();

    //---------------------------------------------
    // Fit-range lines
    //---------------------------------------------
    double ymax =
        0.45 * jpsimass->GetMaximum();

    TLine *l1 =
        new TLine(fitMin, 0, fitMin, ymax);

    l1->SetLineStyle(2);
    l1->SetLineWidth(2);
    l1->SetLineColor(kBlack);
    l1->Draw();

    TLine *l2 =
        new TLine(fitMax, 0, fitMax, ymax);

    l2->SetLineStyle(2);
    l2->SetLineWidth(2);
    l2->SetLineColor(kBlack);
    l2->Draw();

    //---------------------------------------------
    // Save
    //---------------------------------------------
    cout << "" << endl;
    canvas->SaveAs("dvmp_jpsi_plots/jpsi_invariant_mass_fit.pdf");
    cout << "jpsi_invariant_mass_fit.pdf saved in dvmp_jpsi_plots" << endl;

    TFile *out =
        new TFile("dvmp_jpsi_mass_fit_output.root", "RECREATE");

    out->cd();
    jpsimass->Write("h_jpsi_mass_RC");
    fitFcn->Write("fit_jpsi_mass");
    out->Write();
    out->Close();

    file->Close();

    //---------------------------------------------
    // Print
    //---------------------------------------------
    cout << "" << endl;
    cout << "Fit parameters:" << endl;
    cout << "Amplitude = " << amp << endl;
    cout << "Mean      = " << mean  << " +/- " << meanErr  << " GeV" << endl;
    cout << "Sigma     = " << sigma << " +/- " << sigmaErr << " GeV" << endl;
    cout << "Bkg const = " << constBkg << endl;
    cout << "Bkg slope = " << slopeBkg << endl;
    cout << "" << endl;
}
