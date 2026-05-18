#include <iostream>
#include <vector>
#include <TFile.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TSystem.h>
#include <TStyle.h>
#include <TMath.h>

using namespace std;

TH1F* GetH1(TFile *file, const char *name)
{
    TH1F *h = (TH1F*)file->Get(name);

    if (!h)
        cerr << "ERROR: histogram not found: " << name << endl;

    return h;
}

void DrawCombinedH1(
    vector<TH1F*> hists,
    vector<TString> labels,
    vector<int> colors,
    TString title,
    TString xTitle,
    TString yTitle,
    TString outputName,
    bool useLogX = false,
    bool useLogY = false,
    bool useLogZ = false,
    bool useXRange = false,
    double xMin = 0,
    double xMax = 0
)
{
    TCanvas *c =
        new TCanvas(outputName, outputName, 900, 800);

    if (useLogX)
        c->SetLogx();

    if (useLogY)
        c->SetLogy();

    if (useLogZ)
        c->SetLogz();

    double maxY = 0.0;

    for (auto h : hists)
    {
        if (!h) return;

        maxY = TMath::Max(maxY, h->GetMaximum());
    }

    TLegend *leg =
        new TLegend(0.60, 0.70, 0.88, 0.88);

    leg->SetBorderSize(0);
    leg->SetFillStyle(0);

    for (int i = 0; i < hists.size(); ++i)
    {
        TH1F *h = hists[i];

        h->SetStats(0);
        h->SetTitle(title);
        h->GetXaxis()->SetTitle(xTitle);
        h->GetYaxis()->SetTitle(yTitle);
        h->GetYaxis()->SetMaxDigits(3);
        
        if (useXRange)
            h->GetXaxis()->SetRangeUser(xMin, xMax);

        if (i == 0)
        {
            if (useLogY)
            {
                h->SetMinimum(0.1);
                h->SetMaximum(maxY * 10.0);
            }
            else
            {
                h->SetMinimum(0.0);
                h->SetMaximum(1.25 * maxY);
            }
        }

        h->SetLineColor(colors[i]);
        h->SetLineWidth(3);

        if (i == 0)
            h->Draw("hist");
        else
            h->Draw("hist same");

        leg->AddEntry(h, labels[i], "l");
    }

    leg->Draw();
    c->SaveAs(outputName + ".pdf");

    delete c;
}

TH2F* GetH2(TFile *file, const char *name)
{
    TH2F *h = (TH2F*)file->Get(name);

    if (!h)
        cerr << "ERROR: 2D histogram not found: " << name << endl;

    return h;
}

void DrawH2(
    TH2F *h,
    TString xTitle,
    TString yTitle,
    TString outputName,
    bool useLogX = false,
    bool useLogY = false,
    bool useLogZ = true
)
{
    if (!h) return;

    TCanvas *c = new TCanvas(outputName, outputName, 900, 800);

    c->SetLeftMargin(0.14);
    c->SetRightMargin(0.16);
    c->SetBottomMargin(0.13);

    if (useLogX) c->SetLogx();
    if (useLogY) c->SetLogy();
    if (useLogZ) c->SetLogz();

    h->SetStats(0);
    h->SetTitle("");
    h->GetXaxis()->SetTitle(xTitle);
    h->GetYaxis()->SetTitle(yTitle);
    h->GetZaxis()->SetMaxDigits(3);
    
    h->GetXaxis()->SetTitleOffset(1.2);
    h->GetYaxis()->SetTitleOffset(1.5);
    h->GetZaxis()->SetTitleOffset(1.4);

    h->Draw("COLZ");

    c->SaveAs(outputName + ".pdf");

    delete c;
}

void plot_dvmp_jpsi()
{
    TString inputFileName = "dvmp_benchmark_data_10x130_May_2026_run_0.root";
    TString outputDir = "dvmp_jpsi_plots";
    
    gSystem->mkdir(outputDir, kTRUE);
    gStyle->SetOptStat(0);
    
    TFile *file = TFile::Open(inputFileName);
    
    if (!file || file->IsZombie())
    {
        cerr << "ERROR: Cannot open file: " << inputFileName << endl;
        return;
    }
    
    //---------------------------------------------
    // MC final-state eta
    //---------------------------------------------
    DrawCombinedH1(
                   {
                       GetH1(file, "h_mc_recoil_proton_eta"),
                       GetH1(file, "h_mc_scattered_electron_eta"),
                       GetH1(file, "h_mc_decay_electron_eta"),
                       GetH1(file, "h_mc_positron_eta")
                   },
                   {
                       "Scattered Proton",
                       "Scattered Electron",
                       "Decay Electron",
                       "Decay Positron"
                   },
                   {
                       kBlue,
                       kBlack,
                       kRed,
                       kViolet
                   },
                   // "MC Final-State #eta Distributions",
                   " ",
                   "#eta_{MC}",
                   "Counts",
                   outputDir + "/MC_final_state_eta",
                   false,
                   false,
                   false
                   );
    
    //---------------------------------------------
    // RECO final-state eta
    //---------------------------------------------
    //---------------------------------------------
    // RECO final-state eta
    //---------------------------------------------
    DrawCombinedH1(
                   {
                       GetH1(file, "B0RPfinalProton"),
                       GetH1(file, "h_scattered_electron_eta_RC"),
                       GetH1(file, "h_decay_electron_eta_RC"),
                       GetH1(file, "h_scattered_positron_eta_RC")
                   },
                   {
                       "Scattered Proton",
                       "Scattered Electron",
                       "Decay Electron",
                       "Decay Positron"
                   },
                   {
                       kBlue,
                       kBlack,
                       kRed,
                       kViolet
                   },
                   //"RECO Final-State #eta Distributions",
                   " ",
                   "#eta_{RECO}",
                   "Counts",
                   outputDir + "/RECO_final_state_eta",
                   false,
                   false,
                   false
                   );
    
    //---------------------------------------------
    // xB comparison: Electron method
    //---------------------------------------------
    DrawCombinedH1(
                   {
                       GetH1(file, "h_xb_lAger"),
                       GetH1(file, "h_reco_EM_x"),
                       GetH1(file, "EM_x")
                   },
                   {
                       "Truth",
                       "RECO Electron",
                       "Inclusive Electron"
                   },
                   {
                       kBlack,
                       kRed,
                       kBlue
                   },
                   //"x_{B} Comparison: Electron Method",
                   "",
                   "x_{B}",
                   "Counts",
                   outputDir + "/xb_comparison_Electron",
                   false,
                   true,
                   false
                   );
    
    //---------------------------------------------
    // xB comparison: JB method
    //---------------------------------------------
    DrawCombinedH1(
                   {
                       GetH1(file, "h_xb_lAger"),
                       GetH1(file, "h_reco_JB_x"),
                       GetH1(file, "JB_x_in")
                   },
                   {
                       "Truth",
                       "RECO JB",
                       "Inclusive JB"
                   },
                   {
                       kBlack,
                       kRed,
                       kBlue
                   },
                   //"x_{B} Comparison: JB Method",
                   "",
                   "x_{B}",
                   "Counts",
                   outputDir + "/xb_comparison_JB",
                   false,
                   true,
                   false
                   );
    
    //---------------------------------------------
    // xB comparison: eSigma method
    //---------------------------------------------
    DrawCombinedH1(
                   {
                       GetH1(file, "h_xb_lAger"),
                       GetH1(file, "h_reco_eSigma_x"),
                       GetH1(file, "ESig_x_in")
                   },
                   {
                       "Truth",
                       "RECO eSigma",
                       "Inclusive eSigma"
                   },
                   {
                       kBlack,
                       kRed,
                       kBlue
                   },
                   //"x_{B} Comparison: eSigma Method",
                   "",
                   "x_{B}",
                   "Counts",
                   outputDir + "/xb_comparison_eSigma",
                   false,
                   true,
                   false
                   );
    
    //---------------------------------------------
    // xB comparison: DA method
    //---------------------------------------------
    DrawCombinedH1(
                   {
                       GetH1(file, "h_xb_lAger"),
                       GetH1(file, "h_reco_DA_x"),
                       GetH1(file, "DA_x_in")
                   },
                   {
                       "Truth",
                       "RECO DA",
                       "Inclusive DA"
                   },
                   {
                       kBlack,
                       kRed,
                       kBlue
                   },
                   //"x_{B} Comparison: DA Method",
                   "",
                   "x_{B}",
                   "Counts",
                   outputDir + "/xb_comparison_DA",
                   false,
                   true,
                   false
                   );
    
    //---------------------------------------------
    // Q2 comparison: Electron method
    //---------------------------------------------
    DrawCombinedH1(
                   {
                       GetH1(file, "h_Q2_lAger"),
                       GetH1(file, "h_reco_EM_Q2"),
                       GetH1(file, "EM_Q2")
                   },
                   {
                       "Truth",
                       "RECO Electron",
                       "Inclusive Electron"
                   },
                   {
                       kBlack,
                       kRed,
                       kBlue
                   },
                   "",
                   "Q^{2} [GeV^{2}]",
                   "Counts",
                   outputDir + "/Q2_comparison_Electron",
                   false,
                   true,
                   false
                   );
    
    //---------------------------------------------
    // Q2 comparison: JB method
    //---------------------------------------------
    DrawCombinedH1(
                   {
                       GetH1(file, "h_Q2_lAger"),
                       GetH1(file, "h_reco_JB_Q2"),
                       GetH1(file, "JB_Q2_in")
                   },
                   {
                       "Truth",
                       "RECO JB",
                       "Inclusive JB"
                   },
                   {
                       kBlack,
                       kRed,
                       kBlue
                   },
                   "",
                   "Q^{2} [GeV^{2}]",
                   "Counts",
                   outputDir + "/Q2_comparison_JB",
                   false,
                   true,
                   false
                   );
    
    //---------------------------------------------
    // Q2 comparison: eSigma method
    //---------------------------------------------
    DrawCombinedH1(
                   {
                       GetH1(file, "h_Q2_lAger"),
                       GetH1(file, "h_reco_eSigma_Q2"),
                       GetH1(file, "ESig_Q2_in")
                   },
                   {
                       "Truth",
                       "RECO eSigma",
                       "Inclusive eSigma"
                   },
                   {
                       kBlack,
                       kRed,
                       kBlue
                   },
                   "",
                   "Q^{2} [GeV^{2}]",
                   "Counts",
                   outputDir + "/Q2_comparison_eSigma",
                   false,
                   true,
                   false
                   );
    
    //---------------------------------------------
    // Q2 comparison: DA method
    //---------------------------------------------
    DrawCombinedH1(
                   {
                       GetH1(file, "h_Q2_lAger"),
                       GetH1(file, "h_reco_DA_Q2"),
                       GetH1(file, "DA_Q2_in")
                   },
                   {
                       "Truth",
                       "RECO DA",
                       "Inclusive DA"
                   },
                   {
                       kBlack,
                       kRed,
                       kBlue
                   },
                   "",
                   "Q^{2} [GeV^{2}]",
                   "Counts",
                   outputDir + "/Q2_comparison_DA",
                   false,
                   true,
                   false
                   );
    
    //---------------------------------------------
    // y comparison: Electron method
    //---------------------------------------------
    DrawCombinedH1(
                   {
                       GetH1(file, "h_y_lAger"),
                       GetH1(file, "h_reco_EM_y"),
                       GetH1(file, "EM_y")
                   },
                   {
                       "Truth",
                       "RECO Electron",
                       "Inclusive Electron"
                   },
                   {
                       kBlack,
                       kRed,
                       kBlue
                   },
                   "",
                   "y",
                   "Counts",
                   outputDir + "/y_comparison_Electron",
                   false,
                   true,
                   false
                   );
    
    //---------------------------------------------
    // y comparison: JB method
    //---------------------------------------------
    DrawCombinedH1(
                   {
                       GetH1(file, "h_y_lAger"),
                       GetH1(file, "h_reco_JB_y"),
                       GetH1(file, "JB_y_in")
                   },
                   {
                       "Truth",
                       "RECO JB",
                       "Inclusive JB"
                   },
                   {
                       kBlack,
                       kRed,
                       kBlue
                   },
                   "",
                   "y",
                   "Counts",
                   outputDir + "/y_comparison_JB",
                   false,
                   true,
                   false
                   );
    
    //---------------------------------------------
    // y comparison: eSigma method
    //---------------------------------------------
    DrawCombinedH1(
                   {
                       GetH1(file, "h_y_lAger"),
                       GetH1(file, "h_reco_eSigma_y"),
                       GetH1(file, "ESig_y_in")
                   },
                   {
                       "Truth",
                       "RECO eSigma",
                       "Inclusive eSigma"
                   },
                   {
                       kBlack,
                       kRed,
                       kBlue
                   },
                   "",
                   "y",
                   "Counts",
                   outputDir + "/y_comparison_eSigma",
                   false,
                   true,
                   false
                   );
    
    //---------------------------------------------
    // y comparison: DA method
    //---------------------------------------------
    DrawCombinedH1(
                   {
                       GetH1(file, "h_y_lAger"),
                       GetH1(file, "h_reco_DA_y"),
                       GetH1(file, "DA_y_in")
                   },
                   {
                       "Truth",
                       "RECO DA",
                       "Inclusive DA"
                   },
                   {
                       kBlack,
                       kRed,
                       kBlue
                   },
                   "",
                   "y",
                   "Counts",
                   outputDir + "/y_comparison_DA",
                   false,
                   true,
                   false
                   );
    
    //---------------------------------------------
    // xB resolution plots
    //---------------------------------------------
    DrawH2(GetH2(file, "h2_x_EM_res"), "#it{x}_{truth}", "#it{x}_{reco}", outputDir + "/res_x_Electron", false, false, true);
    DrawH2(GetH2(file, "h2_x_JB_res"), "#it{x}_{truth}", "#it{x}_{reco}", outputDir + "/res_x_JB", false, false, true);
    DrawH2(GetH2(file, "h2_x_eSigma_res"), "#it{x}_{truth}", "#it{x}_{reco}", outputDir + "/res_x_eSigma", false, false, true);
    DrawH2(GetH2(file, "h2_x_DA_res"), "#it{x}_{truth}", "#it{x}_{reco}", outputDir + "/res_x_DA", false, false, true);
    
    //---------------------------------------------
    // Q2 resolution plots
    //---------------------------------------------
    DrawH2(GetH2(file, "h2_Q2_EM_res"), "#it{Q}^{2}_{truth} [GeV^{2}]", "#it{Q}^{2}_{reco} [GeV^{2}]", outputDir + "/res_Q2_Electron", false, false, true);
    DrawH2(GetH2(file, "h2_Q2_JB_res"), "#it{Q}^{2}_{truth} [GeV^{2}]", "#it{Q}^{2}_{reco} [GeV^{2}]", outputDir + "/res_Q2_JB", false, false, true);
    DrawH2(GetH2(file, "h2_Q2_eSigma_res"), "#it{Q}^{2}_{truth} [GeV^{2}]", "#it{Q}^{2}_{reco} [GeV^{2}]", outputDir + "/res_Q2_eSigma", false, false, true);
    DrawH2(GetH2(file, "h2_Q2_DA_res"), "#it{Q}^{2}_{truth} [GeV^{2}]", "#it{Q}^{2}_{reco} [GeV^{2}]", outputDir + "/res_Q2_DA", false, false, true);
    
    //---------------------------------------------
    // y resolution plots
    //---------------------------------------------
    DrawH2(GetH2(file, "h2_y_EM_res"), "#it{y}_{truth}", "#it{y}_{reco}", outputDir + "/res_y_Electron", false, false, true);
    DrawH2(GetH2(file, "h2_y_JB_res"), "#it{y}_{truth}", "#it{y}_{reco}", outputDir + "/res_y_JB", false, false, true);
    DrawH2(GetH2(file, "h2_y_eSigma_res"), "#it{y}_{truth}", "#it{y}_{reco}", outputDir + "/res_y_eSigma", false, false, true);
    DrawH2(GetH2(file, "h2_y_DA_res"), "#it{y}_{truth}", "#it{y}_{reco}", outputDir + "/res_y_DA", false, false, true);
    
    //---------------------------------------------
    // x resolution comparison
    //---------------------------------------------
    DrawCombinedH1(
                   {
                       GetH1(file, "h_dx_EM"),
                       GetH1(file, "h_dx_JB"),
                       GetH1(file, "h_dx_eSigma"),
                       GetH1(file, "h_dx_DA")
                   },
                   {
                       "Electron",
                       "JB",
                       "eSigma",
                       "DA"
                   },
                   {
                       kBlack,
                       kRed,
                       kBlue,
                       kViolet
                   },
                   "",
                   "(x_{truth} - x_{reco}) / x_{truth}",
                   "Counts",
                   outputDir + "/dx_resolution_comparison",
                   false,
                   false,
                   false
                   );
    
    //---------------------------------------------
    // Q2 resolution comparison
    //---------------------------------------------
    DrawCombinedH1(
                   {
                       GetH1(file, "h_dQ2_EM"),
                       GetH1(file, "h_dQ2_JB"),
                       GetH1(file, "h_dQ2_eSigma"),
                       GetH1(file, "h_dQ2_DA")
                   },
                   {
                       "Electron",
                       "JB",
                       "eSigma",
                       "DA"
                   },
                   {
                       kBlack,
                       kRed,
                       kBlue,
                       kViolet
                   },
                   "",
                   "(Q^{2}_{truth} - Q^{2}_{reco}) / Q^{2}_{truth}",
                   "Counts",
                   outputDir + "/dQ2_resolution_comparison",
                   false,
                   false,
                   false
                   );
    
    //---------------------------------------------
    // y resolution comparison
    //---------------------------------------------
    DrawCombinedH1(
                   {
                       GetH1(file, "h_dy_EM"),
                       GetH1(file, "h_dy_JB"),
                       GetH1(file, "h_dy_eSigma"),
                       GetH1(file, "h_dy_DA")
                   },
                   {
                       "Electron",
                       "JB",
                       "eSigma",
                       "DA"
                   },
                   {
                       kBlack,
                       kRed,
                       kBlue,
                       kViolet
                   },
                   "",
                   "(y_{truth} - y_{reco}) / y_{truth}",
                   "Counts",
                   outputDir + "/dy_resolution_comparison",
                   false,
                   false,
                   false
                   );
    
    //---------------------------------------------
    // Electron energy resolution comparison
    //---------------------------------------------
    DrawCombinedH1(
                   {
                       GetH1(file, "h_TrackEnergy_minus_SCElecEnergy"),
                       GetH1(file, "h_CalEnergy_minus_SCElecEnergy")
                   },
                   {
                       "Track Energy Resolution",
                       "Calorimeter Energy Resolution"
                   },
                   {
                       kBlue,
                       kRed
                   },
                   "",
                   "(E_{MC} - E_{reco}) / E_{MC}",
                   "Counts",
                   outputDir + "/electron_energy_resolution_comparison",
                   false,
                   false,
                   false
                   );
    
    //---------------------------------------------
    // Calorimeter energy resolution vs eta
    //---------------------------------------------
    DrawH2(
           GetH2(file, "h_Cal_DE_over_E_vs_Eta"),
           "#eta_{MC}",
           "(E_{MC} - E_{cal}) / E_{MC}",
           outputDir + "/cal_dE_over_E_vs_eta",
           false,
           false,
           true
           );
    
    //---------------------------------------------
    // Track energy resolution vs eta
    //---------------------------------------------
    DrawH2(
           GetH2(file, "h_Track_DE_over_E_vs_Eta"),
           "#eta_{MC}",
           "(E_{MC} - E_{track}) / E_{MC}",
           outputDir + "/track_dE_over_E_vs_eta",
           false,
           false,
           true
           );
    
    //---------------------------------------------
    // E/p matching distribution
    //---------------------------------------------
    DrawCombinedH1(
                   {
                       GetH1(file, "h_matched_eop")
                   },
                   {
                       "Electron"
                   },
                   {
                       kBlack
                   },
                   "",
                   "E_{Cal or RECO}/p",
                   "Counts",
                   outputDir + "/matched_eop",
                   false,
                   false,
                   false
                   );
    
    //---------------------------------------------
    // J/psi decay positron eta: MC vs RECO
    //---------------------------------------------
    DrawCombinedH1(
                   {
                       GetH1(file, "h_mc_decay_electron_eta"),
                       GetH1(file, "h_decay_electron_eta_RC"),
                       GetH1(file, "h_mc_positron_eta"),
                       GetH1(file, "h_scattered_positron_eta_RC")
                   },
                   {
                       "MC Decay Electron",
                       "RECO Decay Electron",
                       "MC Positron",
                       "RECO Positron"
                   },
                   {
                       kBlack,
                       kRed,
                       kBlue,
                       kViolet
                   },
                   "",
                   "#eta",
                   "Counts",
                   outputDir + "/electron_positron_eta_MC_RECO",
                   false,
                   false,
                   false
                   );
    
    //---------------------------------------------
    // E - Pz: MC vs RECO
    //---------------------------------------------
    DrawCombinedH1(
                   {
                       GetH1(file, "h_mc_e_minus_pz"),
                       GetH1(file, "h_reco_e_minus_pz")
                   },
                   {
                       "MC",
                       "RECO"
                   },
                   {
                       kBlack,
                       kRed
                   },
                   "",
                   "E - P_{z} [GeV]",
                   "Counts",
                   outputDir + "/e_minus_pz_MC_RECO",
                   false,
                   false,
                   false
                   );
    
    //---------------------------------------------
    // Proton eta: MC vs RECO RP/B0
    //---------------------------------------------
    DrawCombinedH1(
                   {
                       GetH1(file, "h_mc_recoil_proton_eta"),
                       GetH1(file, "RPfinalProton"),
                       GetH1(file, "B0finalProton"),
                       GetH1(file, "B0RPfinalProton")
                   },
                   {
                       "MC Recoil Proton",
                       "RECO RP Proton",
                       "RECO B0 Proton",
                       "RECO RP+B0 Proton"
                   },
                   {
                       kBlack,
                       kRed,
                       kBlue,
                       kViolet
                   },
                   "",
                   "#eta",
                   "Counts",
                   outputDir + "/proton_eta_MC_RECO_RP_B0",
                   false,
                   false,
                   false,
                   true,
                   4.0,
                   10.0
                   );
    
    //---------------------------------------------
    // Missing mass: MC vs RECO RP/B0
    //---------------------------------------------
    DrawCombinedH1(
                   {
                       GetH1(file, "h_missingMass_MC"),
                       GetH1(file, "h_missingMass_RP"),
                       GetH1(file, "h_missingMass_B0")
                   },
                   {
                       "MC",
                       "RECO RP",
                       "RECO B0"
                   },
                   {
                       kBlack,
                       kRed,
                       kBlue
                   },
                   "",
                   "M_{X} [GeV]",
                   "Counts",
                   outputDir + "/missing_mass_MC_RECO_RP_B0",
                   false,
                   false,
                   false
                   );
    
    //---------------------------------------------
    // Scattered electron energy: MC vs RECO vs ECal
    //---------------------------------------------
    DrawCombinedH1(
                   {
                       GetH1(file, "h_mc_scattered_electron_E"),
                       GetH1(file, "h_scattered_electron_energy_RC"),
                       GetH1(file, "h_matched_ecal_energy")
                   },
                   {
                       "MC Scattered Electron",
                       "RECO Scattered Electron",
                       "Matched ECal Cluster"
                   },
                   {
                       kBlack,
                       kRed,
                       kBlue
                   },
                   "",
                   "E [GeV]",
                   "Counts",
                   outputDir + "/scattered_electron_energy_MC_RECO_ECal",
                   false,
                   true,
                   false
                   );
    
    //---------------------------------------------
    // -t comparison: Truth, RP, B0
    //---------------------------------------------
    DrawCombinedH1(
                   {
                       GetH1(file, "h_t_lAger"),
                       GetH1(file, "h_t_RECO_RP"),
                       GetH1(file, "h_t_RECO_B0")
                   },
                   {
                       "Truth",
                       "RECO RP",
                       "RECO B0"
                   },
                   {
                       kBlack,
                       kRed,
                       kBlue
                   },
                   "",
                   "-t [GeV^{2}]",
                   "Counts",
                   outputDir + "/t_comparison_truth_RP_B0",
                   false,
                   true,
                   false,
                   true,
                   0.0,
                   2.0
                   );
    
    //---------------------------------------------
    // -t comparison: Truth and Method L
    //---------------------------------------------
    DrawCombinedH1(
                   {
                       GetH1(file, "h_t_lAger"),
                       GetH1(file, "h_t_MethodL_RECO")
                   },
                   {
                       "Truth",
                       "RECO Method L"
                   },
                   {
                       kBlack,
                       kRed
                   },
                   "",
                   "-t [GeV^{2}]",
                   "Counts",
                   outputDir + "/t_comparison_truth_MethodL",
                   false,
                   true,
                   false,
                   true,
                   0.0,
                   2.0
                   );
    
    //---------------------------------------------
    // -t comparison: Truth, RP, B0, Method L, Acceptance
    //---------------------------------------------
    DrawCombinedH1(
        {
            GetH1(file, "h_t_lAger"),
            GetH1(file, "h_t_RECO_RP"),
            GetH1(file, "h_t_RECO_B0"),
            GetH1(file, "h_t_MethodL_RECO"),
            GetH1(file, "h_t_lAger_accept")
        },
        {
            "Truth",
            "RECO RP",
            "RECO B0",
            "RECO Method L",
            "Accepted Truth"
        },
        {
            kBlack,
            kRed,
            kBlue,
            kViolet,
            kGreen + 2
        },
        "",
        "-t [GeV^{2}]",
        "Counts",
        outputDir + "/t_comparison_truth_RP_B0_MethodL_accept",
        false,
        true,
        false,
        true,
        0.0,
        2.0
    );
    
    //---------------------------------------------
    // -t 2D correlations
    //---------------------------------------------
    DrawH2(GetH2(file, "h_t2D_RPB0_MethodL"), "Truth -t [GeV^{2}]", "RECO RP+B0+Method L -t [GeV^{2}]", outputDir + "/t2D_RPB0_MethodL", false, false, true);
    
    DrawH2(GetH2(file, "h_t2D_MethodL"), "Truth -t [GeV^{2}]", "RECO Method L -t [GeV^{2}]", outputDir + "/t2D_MethodL", false, false, true);
    
    DrawH2(GetH2(file, "h_t2D_RPB0"), "Truth -t [GeV^{2}]", "RECO RP+B0 -t [GeV^{2}]", outputDir + "/t2D_RPB0", false, false, true);
    
    DrawH2(GetH2(file, "h_t2D_B0"), "Truth -t [GeV^{2}]", "RECO B0 -t [GeV^{2}]", outputDir + "/t2D_B0", false, false, true);
    
    DrawH2(GetH2(file, "h_t2D_RP"), "Truth -t [GeV^{2}]", "RECO RP -t [GeV^{2}]", outputDir + "/t2D_RP", false, false, true);
    
    //---------------------------------------------
    // -t comparison: Truth vs corrected RP+B0+MethodL
    //---------------------------------------------
    DrawCombinedH1(
        {
            GetH1(file, "h_t_lAger"),
            GetH1(file, "h_t_RPB0_MethodL_corrected")
        },
        {
            "Truth",
            "Corrected RECO RP+B0+MethodL"
        },
        {
            kBlack,
            kRed
        },
        "",
        "-t [GeV^{2}]",
        "Counts",
        outputDir + "/t_comparison_truth_corrected_RPB0_MethodL",
        false,
        true,
        false,
        true,
        0.0,
        2.0
    );

    //---------------------------------------------
    // Correction factor only
    //---------------------------------------------
    DrawCombinedH1(
        {
            GetH1(file, "h_CorrectionFactor")
        },
        {
            "Correction Factor"
        },
        {
            kBlack
        },
        "",
        "-t [GeV^{2}]",
        "C_{f}",
        outputDir + "/t_correction_factor",
        false,
        false,
        false,
        true,
        0.0,
        2.0
    );
    
    file->Close();
    
    cout << " " << endl;
    cout << " ========================= " << endl;
    cout << "Saved plots in: " << outputDir << endl;
    cout << " ========================= " << endl;
    cout << "  " << endl;
}
