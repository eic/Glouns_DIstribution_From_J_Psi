#include <TFile.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TSystem.h>
#include <TStyle.h>
#include <iostream>

using namespace std;

void ScaleToCrossSection(TH1 *h, double lumi_nb, int t_max, int t_min)
{
    if (!h) return;

    for (int ib = 1; ib <= h->GetNbinsX(); ++ib)
    {
        double N    = h->GetBinContent(ib);
        double errN = h->GetBinError(ib);
        double bw   = h->GetBinWidth(ib);

        double val = (bw > 0.0 && lumi_nb > 0.0) ? N / (lumi_nb * bw) : 0.0;
        double err = (bw > 0.0 && lumi_nb > 0.0) ? errN / (lumi_nb * bw) : 0.0;

        h->SetBinContent(ib, val);
        h->SetBinError(ib, err);
    }
}

void StyleHist(TH1 *h, int color, int marker, int width = 2)
{
    if (!h) return;

    int t_max = 2.0, t_min = 0.0;
    h->SetTitle("");
    h->SetStats(0);
    h->SetLineColor(color);
    h->SetMarkerColor(color);
    h->SetMarkerStyle(marker);
    h->SetLineWidth(width);
    h->SetMarkerSize(0.7);
    h->GetXaxis()->SetTitle("-t [GeV^{2}]");
    h->GetYaxis()->SetTitle("d#sigma/dt [nb/GeV^{2}]");
    h->GetXaxis()->SetRangeUser(t_min, t_max);
    //h->GetYaxis()->SetMaxDigits(3);
    h->GetYaxis()->SetTitleOffset(1.3);
    h->GetXaxis()->SetTitleOffset(1.3);
}

void dvmp_cross_section()
{
    gSystem->mkdir("dvmp_jpsi_plots", kTRUE);
    gStyle->SetOptStat(0);

    //---------------------------------------------
    // Luminosities
    //---------------------------------------------
    double L_bkg_fb    = 7.8e-4;
    double L_sig_fb    = 10.0;
    double L_target_fb = 5.0;

    double L_bkg_nb = L_bkg_fb * 1e6;
    double L_sig_nb = L_sig_fb * 1e6;
    int t_max = 2.0, t_min = 0.0;

    //---------------------------------------------
    // Open files
    //---------------------------------------------
    TFile *file_bkg =
        TFile::Open("background_Dec_2025_run_0.root");

    if (!file_bkg || file_bkg->IsZombie())
    {
        cerr << "ERROR: Cannot open background file" << endl;
        return;
    }

    TFile *file_sig =
        TFile::Open("dvmp_benchmark_data_10x130_May_2026_run_0.root");

    if (!file_sig || file_sig->IsZombie())
    {
        cerr << "ERROR: Cannot open signal file" << endl;
        return;
    }

    //---------------------------------------------
    // Get histograms
    //---------------------------------------------
    TH1 *bkg_truth_raw = nullptr;
    TH1 *bkg_reco_raw  = nullptr;
    TH1 *sig_truth_raw = nullptr;
    TH1 *sig_reco_raw  = nullptr;
    TH1 *sig_corr_raw  = nullptr;

    file_bkg->GetObject("mtgg", bkg_truth_raw);
    file_bkg->GetObject("two_meth_tdist", bkg_reco_raw);

    file_sig->GetObject("h_t_lAger", sig_truth_raw);
    file_sig->GetObject("h_t_RECO_RPB0_MethodL", sig_reco_raw);
    file_sig->GetObject("h_t_RPB0_MethodL_corrected", sig_corr_raw);

    if (!bkg_truth_raw || !bkg_reco_raw ||
        !sig_truth_raw || !sig_reco_raw || !sig_corr_raw)
    {
        cerr << "ERROR: Missing one or more input histograms" << endl;
        return;
    }

    //---------------------------------------------
    // Clone
    //---------------------------------------------
    TH1D *bkg_truth = (TH1D*)bkg_truth_raw->Clone("dsdt_bkg_truth");
    TH1D *bkg_reco  = (TH1D*)bkg_reco_raw ->Clone("dsdt_bkg_reco");

    TH1D *sig_truth = (TH1D*)sig_truth_raw->Clone("dsdt_signal_truth");
    TH1D *sig_reco  = (TH1D*)sig_reco_raw ->Clone("dsdt_signal_reco");
    TH1D *sig_corr  = (TH1D*)sig_corr_raw ->Clone("dsdt_signal_corrected");

    //---------------------------------------------
    // Convert to d sigma / dt
    //---------------------------------------------
    ScaleToCrossSection(bkg_truth, L_bkg_nb, t_max, t_min);
    ScaleToCrossSection(bkg_reco,  L_bkg_nb, t_max, t_min);

    ScaleToCrossSection(sig_truth, L_sig_nb, t_max, t_min);
    ScaleToCrossSection(sig_reco,  L_sig_nb, t_max, t_min);
    ScaleToCrossSection(sig_corr,  L_sig_nb, t_max, t_min);

    //---------------------------------------------
    // Project all to target luminosity if desired
    //---------------------------------------------
    double sig_proj_scale = L_target_fb / L_sig_fb;
    double bkg_proj_scale = L_target_fb / L_bkg_fb;

    TH1D *bkg_truth_proj = (TH1D*)bkg_truth->Clone("dsdt_bkg_truth_projected");
    TH1D *bkg_reco_proj  = (TH1D*)bkg_reco ->Clone("dsdt_bkg_reco_projected");

    TH1D *sig_truth_proj = (TH1D*)sig_truth->Clone("dsdt_signal_truth_projected");
    TH1D *sig_reco_proj  = (TH1D*)sig_reco ->Clone("dsdt_signal_reco_projected");
    TH1D *sig_corr_proj  = (TH1D*)sig_corr ->Clone("dsdt_signal_corrected_projected");

    bkg_truth_proj->Scale(bkg_proj_scale);
    bkg_reco_proj ->Scale(bkg_proj_scale);

    sig_truth_proj->Scale(sig_proj_scale);
    sig_reco_proj ->Scale(sig_proj_scale);
    sig_corr_proj ->Scale(sig_proj_scale);

    //---------------------------------------------
    // Style
    //---------------------------------------------
    StyleHist(sig_truth, kBlack, 20, 2);
    StyleHist(sig_reco,  kBlue,  24, 1);
    StyleHist(sig_corr,  kRed,   25, 1);

    StyleHist(bkg_truth, kGreen + 2, 25, 1);
    StyleHist(bkg_reco,  kViolet + 1, 26, 1);

    //---------------------------------------------
    // Plot 1: signal truth, reco, corrected
    //---------------------------------------------
    TCanvas *c1 = new TCanvas("c1", "", 900, 800);

    c1->SetLeftMargin(0.12);
    c1->SetBottomMargin(0.12);

    c1->SetLogy();

    sig_truth->GetYaxis()->SetRangeUser(1e-6, 1e-1);
    sig_truth->Draw("HIST");
    sig_reco->Draw("E1 SAME");
    sig_corr->Draw("E1 SAME");

    TLegend *leg1 = new TLegend(0.55, 0.65, 0.88, 0.88);
    leg1->SetBorderSize(0);
    leg1->SetFillStyle(0);
    leg1->AddEntry(sig_truth, "Truth", "l");
    leg1->AddEntry(sig_reco,  "RECO", "ep");
    leg1->AddEntry(sig_corr,  "RECO Corrected", "ep");
    leg1->Draw();

    cout << " " << endl;
    c1->SaveAs("dvmp_jpsi_plots/cross_section_signal_truth_reco_corrected.pdf");

    //---------------------------------------------
    // Plot 2: signal corrected vs DIS background
    //---------------------------------------------
    TCanvas *c2 = new TCanvas("c_signal_vs_bkg", "", 900, 800);

    c2->SetLeftMargin(0.14);
    c2->SetBottomMargin(0.12);

    c2->SetLogy();

    sig_truth->GetYaxis()->SetRangeUser(1e-5, 1e0);
    //sig_truth->GetXaxis()->SetRangeUser(t_min, t_max);

    sig_truth->Draw("HIST");
    sig_corr->Draw("E1 SAME");
    bkg_truth->Draw("E1 SAME");
    bkg_reco->Draw("E1 SAME");

    TLegend *leg2 = new TLegend(0.50, 0.62, 0.88, 0.88);
    leg2->SetBorderSize(0);
    leg2->SetFillStyle(0);

    leg2->AddEntry(sig_truth, "Signal Truth", "l");
    leg2->AddEntry(sig_corr,  "Signal Corrected", "ep");
    leg2->AddEntry(bkg_truth, "DIS Background Truth", "ep");
    leg2->AddEntry(bkg_reco,  "DIS Background RECO", "ep");

    leg2->Draw();

    c2->SaveAs("dvmp_jpsi_plots/cross_section_signal_vs_DIS_background.pdf");

    //---------------------------------------------
    // Plot 3: projected event yield comparison
    //---------------------------------------------
    TCanvas *c3 = new TCanvas("c_projected_yield", "", 900, 800);
    
    c3->SetLeftMargin(0.12);
    c3->SetBottomMargin(0.12);
    
    c3->SetLogy();

    sig_corr_proj->GetXaxis()->SetTitle("-t [GeV^{2}]");
    sig_corr_proj->GetYaxis()->SetTitle("d#sigma/dt [nb/Gev^{2}]");
    sig_corr_proj->GetXaxis()->SetRangeUser(t_min, t_max);
    sig_corr_proj->GetYaxis()->SetRangeUser(1e-5, 1e6);
    sig_corr_proj->SetLineWidth(2);

    sig_corr_proj->SetLineColor(kRed);
    sig_corr_proj->Draw("HIST");

    bkg_reco_proj->SetLineColor(kBlue);
    bkg_reco_proj->SetLineWidth(2);
    bkg_reco_proj->Draw("HIST SAME");

    TLegend *leg3 = new TLegend(0.50, 0.68, 0.88, 0.88);
    leg3->SetBorderSize(0);
    leg3->SetFillStyle(0);
    leg3->AddEntry(sig_corr_proj, "Scaled Signal, 5 fb^{-1}", "l");
    leg3->AddEntry(bkg_reco_proj,  "Scaled DIS Background, 5 fb^{-1}", "l");
    leg3->Draw();

    c3->SaveAs("dvmp_jpsi_plots/cross_section_signal_vs_background.pdf");
    
    //---------------------------------------------
    // Plot 4: DIS background reconstructed J/psi mass
    //---------------------------------------------
    TH1 *Jpsimass = nullptr;

    file_bkg->GetObject("rcJPsiMass1", Jpsimass);
    // file_bkg->GetObject("JPsiMass1", Jpsimass);

    if (!Jpsimass)
    {
        cerr << "ERROR: Cannot find histogram rcJPsiMass1 in background file" << endl;
    }
    else
    {
        Jpsimass->SetTitle("");
        Jpsimass->SetStats(0);
        Jpsimass->SetLineColor(kBlack);
        Jpsimass->SetLineWidth(3);

        Jpsimass->GetXaxis()->SetRangeUser(2.7, 3.4);
        Jpsimass->GetXaxis()->SetTitle("M_{e^{+}e^{-}}");
        Jpsimass->GetYaxis()->SetTitle("Counts");
        Jpsimass->GetXaxis()->SetTitleOffset(1.3);
        Jpsimass->GetYaxis()->SetTitleOffset(1.3);

        TCanvas *c4 = new TCanvas("c_background_jpsi_mass", "", 900, 800);

        c4->SetLeftMargin(0.12);
        c4->SetBottomMargin(0.12);

        Jpsimass->Draw("HIST");

        TLegend *leg4 = new TLegend(0.55, 0.72, 0.88, 0.88);
        leg4->SetBorderSize(0);
        leg4->SetFillStyle(0);
        leg4->AddEntry(Jpsimass, "DIS Background RECO", "l");
        leg4->Draw();

        c4->SaveAs("dvmp_jpsi_plots/background_jpsi_mass.pdf");
    }
    
    //---------------------------------------------
    // Save output ROOT file
    //---------------------------------------------
    TFile *out =
        new TFile(
            "dvmp_cross_section_output.root",
            "RECREATE"
        );

    out->cd();

    sig_truth->Write();
    sig_reco->Write();
    sig_corr->Write();

    bkg_truth->Write();
    bkg_reco->Write();

    sig_truth_proj->Write();
    sig_reco_proj->Write();
    sig_corr_proj->Write();

    bkg_truth_proj->Write();
    bkg_reco_proj->Write();
    
    if (Jpsimass)
        Jpsimass->Write("background_rcJPsiMass1");

    out->Write();
    out->Close();
    
    return;
}
