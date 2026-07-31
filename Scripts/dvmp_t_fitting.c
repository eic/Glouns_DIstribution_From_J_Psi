#include <TFile.h>
#include <TH1.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TSystem.h>
#include <TStyle.h>
#include <TF1.h>
#include <iostream>

using namespace std;

void FitAndSaveT(
    TH1 *h_input,
    TString tag,
    TString label,
    double lumi_nb
)
{
    if (!h_input) return;

    TH1D *dSigmadt =
        (TH1D*)h_input->Clone("dSigmadt_" + tag);

    for (int ib = 1; ib <= dSigmadt->GetNbinsX(); ++ib)
    {
        double N    = h_input->GetBinContent(ib);
        double errN = h_input->GetBinError(ib);
        double bw   = h_input->GetBinWidth(ib);

        if (bw <= 0.0 || lumi_nb <= 0.0)
            continue;

        dSigmadt->SetBinContent(ib, N / (lumi_nb * bw));
        dSigmadt->SetBinError(ib, errN / (lumi_nb * bw));
    }

    dSigmadt->SetTitle("");
    dSigmadt->SetStats(0);
    dSigmadt->SetMarkerStyle(24);
    dSigmadt->SetMarkerSize(0.8);
    dSigmadt->SetMarkerColor(kBlack);
    dSigmadt->SetLineColor(kBlack);
    dSigmadt->SetLineWidth(2);
    dSigmadt->GetXaxis()->SetRangeUser(0.0, 2.0);
    dSigmadt->GetXaxis()->SetTitle("Momentum Transfer, -t [GeV^{2}]");
    dSigmadt->GetYaxis()->SetTitle("d#sigma/dt [nb/GeV^{2}]");
    dSigmadt->GetXaxis()->SetTitleOffset(1.25);
    dSigmadt->GetYaxis()->SetTitleOffset(1.45);

    TF1 *fitFunc =
        new TF1("fit_" + tag, "[0]*exp(-[1]*x)", 0.0, 1.6);

    fitFunc->SetParNames("A", "B");
    fitFunc->SetParameters(dSigmadt->GetMaximum(), 1.0);
    fitFunc->SetLineColor(kRed);
    fitFunc->SetLineWidth(3);

    dSigmadt->Fit(fitFunc, "R E");

    double A    = fitFunc->GetParameter(0);
    double Aerr = fitFunc->GetParError(0);
    double B    = fitFunc->GetParameter(1);
    double Berr = fitFunc->GetParError(1);

    TCanvas *c = new TCanvas("c_" + tag, "", 900, 800);
    c->SetLeftMargin(0.14);
    c->SetBottomMargin(0.12);
    c->SetLogy();

    dSigmadt->GetYaxis()->SetRangeUser(1e-5, 1e-1);
    dSigmadt->GetXaxis()->SetRangeUser(0, 1.6);
    dSigmadt->Draw("E1P");
    fitFunc->Draw("SAME");

    TLegend *leg = new TLegend(0.50, 0.65, 0.88, 0.88);
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);
    leg->AddEntry((TObject*)nullptr, label, "");
    leg->AddEntry((TObject*)nullptr, "Fit: A exp(-B|t|)", "");
    leg->AddEntry(dSigmadt, "d#sigma/dt", "ep");
    leg->AddEntry(fitFunc, Form("A = %.3g #pm %.2g", A, Aerr), "l");
    leg->AddEntry(fitFunc, Form("B = %.3g #pm %.2g", B, Berr), "l");
    leg->Draw();

    c->SaveAs("dvmp_jpsi_plots_April_2026/t_slope_fit_" + tag + ".pdf");
    
    TFile *paramFile =
        new TFile("dvmp_t_fit_parameters_April_2026.root", "UPDATE");

    TNamed *fitName =
        new TNamed(
            tag.Data(),
            Form("%f %f %f %f", A, Aerr, B, Berr)
        );

    fitName->Write(tag, TObject::kOverwrite);

    paramFile->Close();
}

void dvmp_t_fitting()
{
    gSystem->mkdir("dvmp_jpsi_plots_April_2026", kTRUE);
    gStyle->SetOptStat(0);

    //---------------------------------------------
    // Open ROOT file
    //---------------------------------------------
    TFile *file =
        TFile::Open("benchmark_data_10x130_June_2026_From_April_2026_run_0.root");

    if (!file || file->IsZombie())
    {
        cerr << "ERROR: Cannot open signal ROOT file" << endl;
        return;
    }

    //---------------------------------------------
    // Get histograms
    //---------------------------------------------
    TH1 *h_t_corr  = nullptr;
    TH1 *h_t_truth = nullptr;

    file->GetObject("h_methodL_RP_MethodL_B0_corrected", h_t_corr);
    file->GetObject("h_t_MC", h_t_truth);

    if (!h_t_corr)
    {
        cerr << "ERROR: Cannot find h_methodL_RP_MethodL_B0_eff_applied" << endl;
        file->Close();
        return;
    }

    if (!h_t_truth)
    {
        cerr << "ERROR: Cannot find h_t_MC" << endl;
        file->Close();
        return;
    }

    //---------------------------------------------
    // Luminosity
    //---------------------------------------------
    double L_sig_fb = 10.0;
    double L_sig_nb = L_sig_fb * 1e6;

    //---------------------------------------------
    // Fit corrected RECO
    //---------------------------------------------
    FitAndSaveT(
        h_t_corr,
        "RECO_Corrected",
        //"RECO_Corrected",
        " ",
        L_sig_nb
    );

    //---------------------------------------------
    // Fit truth lAger
    //---------------------------------------------
    FitAndSaveT(
        h_t_truth,
        "MC",
        " ",
        //"MC",
        L_sig_nb
    );

    file->Close();

    cout << endl;
    cout << "Saved t-slope fitting plots in dvmp_jpsi_plots_April_2026/" << endl;
}
