#include <TFile.h>
#include <TH1.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TSystem.h>
#include <TStyle.h>

#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

//==================================================
// Global settings
//==================================================
const TString outputDir = "dvmp_jpsi_plots_April_2026";
//const TString outputDir = "dvmp_jpsi_plots";
const TString outputUncert = "Uncertainty_files_April_2026";
//const TString outputUncert = "Uncertainty_files";

const double L_bkg_fb    = 7.8e-4;
const double L_sig_fb    = 10.0;
const double L_target_fb = 1.0;

const double lumi_sys_frac = 0.015;

const double t_min = 0.0;
const double t_max = 1.6;

//==================================================
// Convert counts to d sigma / dt
//==================================================
void ScaleToCrossSection(TH1 *h, double lumi_nb)
{
    if (!h) return;

    for (int ib = 1; ib <= h->GetNbinsX(); ++ib)
    {
        double N    = h->GetBinContent(ib);
        double errN = h->GetBinError(ib);
        double bw   = h->GetBinWidth(ib);

        if (bw <= 0 || lumi_nb <= 0)
        {
            h->SetBinContent(ib, 0);
            h->SetBinError(ib, 0);
            continue;
        }

        h->SetBinContent(ib, N / (lumi_nb * bw));
        h->SetBinError(ib, errN / (lumi_nb * bw));
    }
}

//==================================================
// Style helper
//==================================================
void StyleCrossSectionHist(
    TH1 *h,
    int color,
    int marker,
    int width = 2
)
{
    if (!h) return;

    h->SetTitle("");
    h->SetStats(0);

    h->SetLineColor(color);
    h->SetMarkerColor(color);
    h->SetMarkerStyle(marker);
    h->SetMarkerSize(0.7);
    h->SetLineWidth(width);

    h->GetXaxis()->SetTitle("Momentum Transfer, -t [GeV^{2}]");
    h->GetYaxis()->SetTitle("d#sigma/dt [nb/GeV^{2}]");

    h->GetXaxis()->SetRangeUser(t_min, t_max);

    h->GetXaxis()->SetTitleOffset(1.3);
    h->GetYaxis()->SetTitleOffset(1.35);
}

//==================================================
// Plot 1: Signal truth, reco, corrected
//==================================================
void DrawSignalRecoComparison(
    TH1D *sig_truth,
    TH1D *sig_reco,
    TH1D *sig_corr
)
{
    TCanvas *c = new TCanvas("c_signal_reco_comparison", "", 900, 800);

    c->SetLeftMargin(0.14);
    c->SetBottomMargin(0.12);
    c->SetLogy();

    sig_truth->GetYaxis()->SetRangeUser(1e-6, 1e0);

    sig_truth->Draw("HIST");
    sig_reco->Draw("E1 SAME");
    //sig_corr->Draw("E1 SAME");

    TLegend *leg = new TLegend(0.65, 0.8, 0.9, 0.88);
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);

    leg->AddEntry(sig_truth, "MC", "l");
    leg->AddEntry(sig_reco,  "RECO", "ep");
    //leg->AddEntry(sig_corr,  "RECO Corrected", "ep");

    leg->Draw();
    
    TLegend *info =
        new TLegend(0.2, 0.73, 0.8, 0.85);

    info->SetBorderSize(0);
    info->SetFillStyle(0);

    info->AddEntry((TObject*)nullptr, "#bf{ePIC Performance}", "");
    info->AddEntry((TObject*)nullptr, "26.4.1 Campaign", "");
    info->AddEntry((TObject*)nullptr, "e+p DVJ/#psiP, 10 #times 130 GeV^{2}", "");
    info->AddEntry((TObject*)nullptr, "L_{proj} = 1 fb^{-1}, #sqrt{s} = 72 GeV", "");

    info->Draw();

    c->SaveAs(outputDir + "/cross_section_signal_truth_reco_corrected.pdf");
}

//==================================================
// Plot 2: Cross-section comparison
//==================================================
void DrawCrossSectionComparison(
    TH1D *sig_truth,
    TH1D *sig_corr,
    TH1D *bkg_truth,
    TH1D *bkg_reco
)
{
    TCanvas *c = new TCanvas("c_cross_section_signal_vs_bkg", "", 900, 800);

    c->SetLeftMargin(0.14);
    c->SetBottomMargin(0.12);
    c->SetLogy();

    sig_truth->GetYaxis()->SetRangeUser(1e-5, 1);

    sig_truth->Draw("HIST");
    sig_corr->Draw("E1 SAME");
    //bkg_truth->Draw("E1 SAME");
    //bkg_reco->Draw("E1 SAME");

    TLegend *leg = new TLegend(0.60, 0.72, 0.88, 0.88);
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);

    leg->AddEntry(sig_truth, "Signal Truth", "l");
    leg->AddEntry(sig_corr,  "Signal Corrected", "ep");
    //leg->AddEntry(bkg_truth, "DIS Background Truth", "ep");
    //leg->AddEntry(bkg_reco,  "DIS Background RECO", "ep");

    leg->Draw();
    
    TLegend *info =
        new TLegend(0.1, 0.73, 0.8, 0.85);

    info->SetBorderSize(0);
    info->SetFillStyle(0);

    info->AddEntry((TObject*)nullptr, "#bf{ePIC Performance}", "");
    info->AddEntry((TObject*)nullptr, "26.4.1 Campaign", "");
    info->AddEntry((TObject*)nullptr, "e+p DVJ/#psiP, 10 #times 130 GeV^{2}", "");
    info->AddEntry((TObject*)nullptr, "L_{proj} = 1 fb^{-1}, #sqrt{s} = 72 GeV", "");

    info->Draw();

    c->SaveAs(outputDir + "/cross_section_signal_vs_DIS_background.pdf");
}

//==================================================
// Build projected yields
//==================================================
void BuildProjectedYields(
    TH1 *sig_corr_raw,
    TH1 *bkg_reco_raw,
    TH1D *&sig_yield,
    TH1D *&bkg_yield
)
{
    sig_yield = (TH1D*)sig_corr_raw->Clone("sig_yield_1fb");
    bkg_yield = (TH1D*)bkg_reco_raw->Clone("bkg_yield_1fb");

    sig_yield->Sumw2();
    bkg_yield->Sumw2();

    sig_yield->Scale(L_target_fb / L_sig_fb);
    bkg_yield->Scale(L_target_fb / L_bkg_fb);

    for (int ib = 1; ib <= sig_yield->GetNbinsX(); ++ib)
    {
        double y = sig_yield->GetBinContent(ib);
        sig_yield->SetBinError(ib, y > 0 ? sqrt(y) : 0.0);
    }

    for (int ib = 1; ib <= bkg_yield->GetNbinsX(); ++ib)
    {
        double y = bkg_yield->GetBinContent(ib);
        bkg_yield->SetBinError(ib, y > 0 ? sqrt(y) : 0.0);
    }
}

void BuildProjectedYieldsEffAppliedWithCorrectedErrors(
    TH1 *sig_eff_applied_raw,
    TH1 *sig_corrected_raw,
    TH1 *bkg_reco_raw,
    TH1D *&sig_yield_effErr,
    TH1D *&bkg_yield_effErr
)
{
    sig_yield_effErr =
        (TH1D*)sig_eff_applied_raw->Clone("sig_yield_effApplied_correctedErr_1fb");

    bkg_yield_effErr =
        (TH1D*)bkg_reco_raw->Clone("bkg_yield_effApplied_correctedErr_1fb");

    sig_yield_effErr->Sumw2();
    bkg_yield_effErr->Sumw2();

    double sig_scale = L_target_fb / L_sig_fb;
    double bkg_scale = L_target_fb / L_bkg_fb;

    sig_yield_effErr->Scale(sig_scale);
    bkg_yield_effErr->Scale(bkg_scale);

    // Force signal errors to match projected_yield_signal_vs_background_scaled
    for (int ib = 1; ib <= sig_yield_effErr->GetNbinsX(); ++ib)
    {
        double y_corr_scaled =
            sig_corrected_raw->GetBinContent(ib) * sig_scale;

        double err =
            y_corr_scaled > 0 ? sqrt(y_corr_scaled) : 0.0;

        sig_yield_effErr->SetBinError(ib, err);
    }

    for (int ib = 1; ib <= bkg_yield_effErr->GetNbinsX(); ++ib)
    {
        double y = bkg_yield_effErr->GetBinContent(ib);
        bkg_yield_effErr->SetBinError(ib, y > 0 ? sqrt(y) : 0.0);
    }
}

//==================================================
// Save signal cross section to CSV/Excel-readable file
//==================================================
void SaveCrossSectionToExcel(TH1D *h)
{
    if (!h)
    {
        cerr << "ERROR: Null histogram passed to SaveCrossSectionToExcel"
             << endl;
        return;
    }
    
    //---------------------------------------------
    // Cross-section CSV file
    //---------------------------------------------
    TString outFileName =
        outputUncert + "/Combined_Cross_Section_HFS_18_22.csv";

    ofstream outFile(outFileName.Data());

    if (!outFile.is_open())
    {
        cerr << "ERROR: Cannot create output file"
             << endl;
        return;
    }

    outFile << "Bin,Cross_Section" << endl;

    for (int ib = 1; ib <= h->GetNbinsX(); ++ib)
    {
        double value = h->GetBinContent(ib);

        outFile << ib << "," << value << endl;
    }

    outFile.close();

    cout << "Saved cross section table:" << endl;
    cout << outFileName << endl;

    //---------------------------------------------
    // Separate t-bin CSV file
    //---------------------------------------------
    TString tFileName =
        outputUncert + "/Combined_Cross_Section_tBins.csv";

    ofstream tFile(tFileName.Data());

    if (!tFile.is_open())
    {
        cerr << "ERROR: Cannot create t-bin file"
             << endl;
        return;
    }

    tFile << "Bin,t_center,t_low,t_high" << endl;

    for (int ib = 1; ib <= h->GetNbinsX(); ++ib)
    {
        double t_center = h->GetBinCenter(ib);
        double t_low    = h->GetBinLowEdge(ib);
        double t_high   = t_low + h->GetBinWidth(ib);

        tFile
            << ib << ","
            << t_center << ","
            << t_low << ","
            << t_high
            << endl;
    }

    tFile.close();

    cout << "Saved t-bin table:" << endl;
    cout << tFileName << endl;
}

//==================================================
// Build signal systematic band
//==================================================
void BuildSignalSystematic(
    TH1D *sig_yield,
    TH1 *sig_reco_raw,
    TH1D *&sig_syst
)
{
    sig_syst = (TH1D*)sig_yield->Clone("sig_syst_1fb");

    TH1D *sig_reco_yield =
        (TH1D*)sig_reco_raw->Clone("sig_reco_yield_1fb");

    sig_reco_yield->Scale(L_target_fb / L_sig_fb);

    for (int ib = 1; ib <= sig_syst->GetNbinsX(); ++ib)
    {
        double corrected = sig_yield->GetBinContent(ib);
        double reco      = sig_reco_yield->GetBinContent(ib);

        double reco_syst = fabs(corrected - reco);
        double lumi_syst = lumi_sys_frac * corrected;

        double total_syst =
            sqrt(reco_syst * reco_syst +
                 lumi_syst * lumi_syst);

        sig_syst->SetBinContent(ib, corrected);
        sig_syst->SetBinError(ib, total_syst);
    }

    sig_syst->SetFillColorAlpha(kRed, 0.25);
    sig_syst->SetLineColor(kRed);
    sig_syst->SetMarkerSize(0);
}

//==================================================
// Write uncertainty summary to txt
//==================================================
void WriteUncertaintySummary(
    TH1D *sig_yield,
    TH1D *bkg_yield,
    TH1D *sig_syst
)
{
    ofstream outFile(outputUncert + "/t_Signal_DIS_uncertainty.txt");

    if (!outFile.is_open())
    {
        cerr << "ERROR: Cannot create uncertainty output file" << endl;
        return;
    }

    auto PrintBoth =
    [&](const TString &msg)
    {
        //cout << msg << endl;
        outFile << msg << endl;
    };

    PrintBoth(" ");
    PrintBoth(Form("DVMP Signal scaling factor: %.4f", L_target_fb / L_sig_fb));
    PrintBoth(Form("DIS Background scaling factor: %.4f", L_target_fb / L_bkg_fb));
    PrintBoth(" ");

    PrintBoth("==========================================");
    PrintBoth(Form("Signal Yield Uncertainty Summary (%.1f fb^{-1})", L_target_fb));
    PrintBoth("==========================================");

    for (int ib = 1; ib <= sig_yield->GetNbinsX(); ++ib)
    {
        double t_low  = sig_yield->GetBinLowEdge(ib);
        double t_high = t_low + sig_yield->GetBinWidth(ib);
        double yield  = sig_yield->GetBinContent(ib);

        if (yield <= 0)
            continue;

        double stat = sqrt(yield);
        double stat_pct = 100.0 * stat / yield;

        double total_syst = sig_syst->GetBinError(ib);
        double lumi_syst  = lumi_sys_frac * yield;

        double reco_syst =
            sqrt(max(0.0, total_syst * total_syst -
                          lumi_syst  * lumi_syst));

        double reco_syst_pct  = 100.0 * reco_syst / yield;
        double lumi_syst_pct  = 100.0 * lumi_syst / yield;
        double total_syst_pct = 100.0 * total_syst / yield;

        PrintBoth(
            Form(
                "Bin %d | -t = %.2f - %.2f | Yield = %.2f | Stat = %.2f (%.1f%%) | RecoSys = %.2f (%.1f%%) | LumiSys = %.2f (%.1f%%) | TotalSys = %.2f (%.1f%%)",
                ib,
                t_low,
                t_high,
                yield,
                stat,
                stat_pct,
                reco_syst,
                reco_syst_pct,
                lumi_syst,
                lumi_syst_pct,
                total_syst,
                total_syst_pct
            )
        );
    }

    PrintBoth(" ");
    PrintBoth("==========================================");
    PrintBoth(Form("Background Yield Uncertainty Summary (%.1f fb^{-1})", L_target_fb));
    PrintBoth("==========================================");

    for (int ib = 1; ib <= bkg_yield->GetNbinsX(); ++ib)
    {
        double t_low  = bkg_yield->GetBinLowEdge(ib);
        double t_high = t_low + bkg_yield->GetBinWidth(ib);
        double yield  = bkg_yield->GetBinContent(ib);

        if (yield <= 0)
            continue;

        double stat = sqrt(yield);
        double stat_pct = 100.0 * stat / yield;

        double lumi_syst = lumi_sys_frac * yield;
        double total_syst = lumi_syst;

        double total_syst_pct = 100.0 * total_syst / yield;

        PrintBoth(
            Form(
                "-t = %.2f - %.2f | BKG Yield = %.2f | Stat = %.2f (%.1f%%) | LumiSys = %.2f (1.5%%) | TotalSys = %.2f (%.1f%%)",
                t_low,
                t_high,
                yield,
                stat,
                stat_pct,
                lumi_syst,
                total_syst,
                total_syst_pct
            )
        );
    }

    outFile.close();

    cout << " " << endl;
    cout << "Saved uncertainty summary to:" << endl;
    cout << outputUncert << "/t_Signal_DIS_uncertainty.txt" << endl;
    cout << " " << endl;
}

//==================================================
// Plot projected yield
//==================================================
void DrawProjectedYield(
    TH1D *sig_yield,
    TH1D *bkg_yield,
    TH1D *sig_syst
)
{
    sig_yield->SetTitle("");
    sig_yield->SetStats(0);
    sig_yield->SetLineColor(kRed);
    sig_yield->SetMarkerColor(kRed);
    sig_yield->SetMarkerStyle(24);
    sig_yield->SetLineWidth(1);

    sig_yield->GetXaxis()->SetTitle("-t [GeV^{2}]");
    sig_yield->GetYaxis()->SetTitle("Projected counts / bin");
    sig_yield->GetXaxis()->SetRangeUser(t_min, t_max);
    sig_yield->GetYaxis()->SetRangeUser(1e-1, 1e6);
    sig_yield->GetYaxis()->SetTitleOffset(1.35);

    bkg_yield->SetLineColor(kBlue - 3);
    bkg_yield->SetMarkerColor(kBlue - 3);
    bkg_yield->SetMarkerStyle(25);
    bkg_yield->SetLineWidth(1);

    TCanvas *c = new TCanvas("c_projected_yield", "", 900, 800);

    c->SetLeftMargin(0.14);
    c->SetBottomMargin(0.12);
    c->SetLogy();

    //sig_syst->Draw("E2");
    sig_yield->Draw("E1");
    bkg_yield->Draw("HIST Same");

    TLegend *leg = new TLegend(0.50, 0.68, 0.88, 0.88);
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);

    leg->AddEntry(sig_yield, "Signal, 1 fb^{-1}", "ep");
    //leg->AddEntry(sig_yield, "Signal, 10 fb^{-1}", "ep");
    //leg->AddEntry(sig_syst, "Signal syst.", "f");
    //leg->AddEntry(bkg_yield, "DIS Background, 7.8#times10^{-4} fb^{-1}", "l");
    leg->AddEntry(bkg_yield, "DIS Background, 1 fb^{-1}", "l");

    leg->Draw();

    c->SaveAs(outputDir + "/projected_yield_signal_vs_background_scaled.pdf");
}

void DrawProjectedYieldEffAppliedWithCorrectedErrors(
    TH1D *sig_yield,
    TH1D *bkg_yield
)
{
    sig_yield->SetTitle("");
    sig_yield->SetStats(0);
    sig_yield->SetLineColor(kRed);
    sig_yield->SetMarkerColor(kRed);
    sig_yield->SetMarkerStyle(24);
    sig_yield->SetLineWidth(1);

    sig_yield->GetXaxis()->SetTitle("-t [GeV^{2}]");
    sig_yield->GetYaxis()->SetTitle("Projected counts / bin");
    sig_yield->GetXaxis()->SetRangeUser(t_min, t_max);
    sig_yield->GetYaxis()->SetRangeUser(1e-1, 1e6);
    sig_yield->GetYaxis()->SetTitleOffset(1.35);

    bkg_yield->SetLineColor(kBlue - 3);
    bkg_yield->SetMarkerColor(kBlue - 3);
    bkg_yield->SetMarkerStyle(25);
    bkg_yield->SetLineWidth(1);

    TCanvas *c =
        new TCanvas("c_projected_yield_effApplied_correctedErr", "", 900, 800);

    c->SetLeftMargin(0.14);
    c->SetBottomMargin(0.12);
    c->SetLogy();

    sig_yield->Draw("E1");
    bkg_yield->Draw("HIST SAME");

    TLegend *leg = new TLegend(0.55, 0.78, 0.88, 0.88);
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);

    leg->AddEntry(
        sig_yield,
        "Signal, 1 fb^{-1}",
        "ep"
    );

    leg->AddEntry(
        bkg_yield,
        "DIS Background, 1 fb^{-1}",
        "l"
    );

    leg->Draw();

    c->SaveAs(
        outputDir + "/projected_yield_effApplied_correctedErrors_signal_vs_background.pdf"
    );
}

//==================================================
// Plot background J/psi mass
//==================================================
void DrawBackgroundJPsiMass(TFile *file_bkg)
{
    TH1 *Jpsimass = nullptr;

    file_bkg->GetObject("h_EXCLU_jpsi_mass", Jpsimass);

    if (!Jpsimass)
    {
        cerr << "ERROR: Cannot find h_EXCLU_jpsi_mass in background file" << endl;
        return;
    }

    Jpsimass->SetTitle("");
    Jpsimass->SetStats(0);
    Jpsimass->SetLineColor(kBlack);
    Jpsimass->SetLineWidth(3);

    Jpsimass->GetXaxis()->SetRangeUser(2.7, 3.4);
    Jpsimass->GetXaxis()->SetTitle("M_{e^{+}e^{-}}");
    Jpsimass->GetYaxis()->SetTitle("Counts");

    TCanvas *c = new TCanvas("c_background_jpsi_mass", "", 900, 800);

    c->SetLeftMargin(0.12);
    c->SetBottomMargin(0.12);

    Jpsimass->Draw("HIST");

    TLegend *leg = new TLegend(0.55, 0.72, 0.88, 0.88);
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);
    leg->AddEntry(Jpsimass, "DIS Background RECO", "l");
    leg->Draw();

    c->SaveAs(outputDir + "/background_jpsi_mass.pdf");
}

//==================================================
// Save output ROOT file
//==================================================
void SaveOutputRootFile(
    TH1D *sig_truth,
    TH1D *sig_reco,
    TH1D *sig_corr,
    TH1D *bkg_truth,
    TH1D *bkg_reco,
    TH1D *sig_yield,
    TH1D *bkg_yield,
    TH1D *sig_syst
)
{
    TFile *out =
        new TFile("dvmp_cross_section_output_April_2026.root", "RECREATE");

    out->cd();

    sig_truth->Write();
    sig_reco->Write();
    sig_corr->Write();

    bkg_truth->Write();
    bkg_reco->Write();

    sig_yield->Write();
    bkg_yield->Write();
    sig_syst->Write();

    out->Write();
    out->Close();
}

//==================================================
// Main macro
//==================================================
void dvmp_cross_section()
{
    gSystem->mkdir(outputDir, kTRUE);
    gSystem->mkdir(outputUncert, kTRUE);
    gStyle->SetOptStat(0);

    double L_bkg_nb = L_bkg_fb * 1e6;
    double L_sig_nb = L_sig_fb * 1e6;

    //---------------------------------------------
    // Open files
    //---------------------------------------------
    TFile *file_bkg =
        //TFile::Open("background_Dec_2025_run_0.root");
        TFile::Open("DIS_background_data_10x130_June_2026_run_0.root");

    if (!file_bkg || file_bkg->IsZombie())
    {
        cerr << "ERROR: Cannot open background file" << endl;
        return;
    }

    TFile *file_sig =
        TFile::Open("benchmark_data_10x130_June_2026_From_April_2026_run_0.root");

    if (!file_sig || file_sig->IsZombie())
    {
        cerr << "ERROR: Cannot open signal file" << endl;
        return;
    }

    //---------------------------------------------
    // Get raw histograms
    //---------------------------------------------
    TH1 *bkg_truth_raw = nullptr;
    TH1 *bkg_reco_raw  = nullptr;
    TH1 *sig_truth_raw = nullptr;
    TH1 *sig_reco_raw  = nullptr;
    TH1 *sig_corr_raw  = nullptr;
    TH1 *sig_eff_applied_raw = nullptr;

    //file_bkg->GetObject("mtgg", bkg_truth_raw);
    //file_bkg->GetObject("two_meth_tdist", bkg_reco_raw);
    file_bkg->GetObject("h_t_MC", bkg_truth_raw);
    file_bkg->GetObject("h_methodL_RP_MethodL_B0_corrected", bkg_reco_raw);

    //file_sig->GetObject("h_t_lAger", sig_truth_raw);
    file_sig->GetObject("h_t_MC", sig_truth_raw);
    //file_sig->GetObject("h_methodL_RP_MethodL_B0", sig_reco_raw);
    file_sig->GetObject("h_t_RECO_RPB0_MethodL", sig_reco_raw);
    //file_sig->GetObject("h_methodL_RP_MethodL_B0_eff_applied", sig_corr_raw);
    file_sig->GetObject("h_methodL_RP_MethodL_B0_corrected", sig_corr_raw);
    //file_sig->GetObject("h_t_RPB0_MethodL_corrected", sig_corr_raw);
    file_sig->GetObject("h_methodL_RP_MethodL_B0_eff_applied", sig_eff_applied_raw);

    if (!bkg_truth_raw || !bkg_reco_raw ||
        !sig_truth_raw || !sig_reco_raw ||
        !sig_corr_raw || !sig_eff_applied_raw)
    {
        cerr << "ERROR: Missing one or more input histograms" << endl;
        return;
    }

    //---------------------------------------------
    // Clone for cross-section histograms
    //---------------------------------------------
    TH1D *bkg_truth = (TH1D*)bkg_truth_raw->Clone("dsdt_bkg_truth");
    TH1D *bkg_reco  = (TH1D*)bkg_reco_raw ->Clone("dsdt_bkg_reco");

    TH1D *sig_truth = (TH1D*)sig_truth_raw->Clone("dsdt_signal_truth");
    TH1D *sig_reco  = (TH1D*)sig_reco_raw ->Clone("dsdt_signal_reco");
    TH1D *sig_corr  = (TH1D*)sig_corr_raw ->Clone("dsdt_signal_corrected");

    //---------------------------------------------
    // Convert to d sigma / dt
    //---------------------------------------------
    ScaleToCrossSection(bkg_truth, L_bkg_nb);
    ScaleToCrossSection(bkg_reco,  L_bkg_nb);

    ScaleToCrossSection(sig_truth, L_sig_nb);
    ScaleToCrossSection(sig_reco,  L_sig_nb);
    ScaleToCrossSection(sig_corr,  L_sig_nb);

    //---------------------------------------------
    // Style
    //---------------------------------------------
    StyleCrossSectionHist(sig_truth, kBlack, 20, 2);
    StyleCrossSectionHist(sig_reco,  kBlue,  24, 1);
    StyleCrossSectionHist(sig_corr,  kBlue,   25, 1);

    StyleCrossSectionHist(bkg_truth, kGreen + 2, 25, 1);
    StyleCrossSectionHist(bkg_reco,  kViolet + 1, 26, 1);

    //---------------------------------------------
    // Draw cross-section plots
    //---------------------------------------------
    DrawSignalRecoComparison(sig_truth, sig_reco, sig_corr);

    DrawCrossSectionComparison(
        sig_truth,
        sig_corr,
        bkg_truth,
        bkg_reco
    );
    
    SaveCrossSectionToExcel(sig_corr);

    //---------------------------------------------
    // Projected yields and uncertainties
    //---------------------------------------------
    TH1D *sig_yield = nullptr;
    TH1D *bkg_yield = nullptr;
    TH1D *sig_syst  = nullptr;

    BuildProjectedYields(
        sig_corr_raw,
        bkg_reco_raw,
        sig_yield,
        bkg_yield
    );
    
    TH1D *sig_yield_effErr = nullptr;
    TH1D *bkg_yield_effErr = nullptr;

    BuildProjectedYieldsEffAppliedWithCorrectedErrors(
        sig_eff_applied_raw,
        sig_corr_raw,
        bkg_reco_raw,
        sig_yield_effErr,
        bkg_yield_effErr
    );

    DrawProjectedYieldEffAppliedWithCorrectedErrors(
        sig_yield_effErr,
        bkg_yield_effErr
    );

    BuildSignalSystematic(
        sig_yield,
        sig_reco_raw,
        sig_syst
    );

    WriteUncertaintySummary(
        sig_yield,
        bkg_yield,
        sig_syst
    );

    DrawProjectedYield(
        sig_yield,
        bkg_yield,
        sig_syst
    );

    //---------------------------------------------
    // Background J/psi mass
    //---------------------------------------------
    DrawBackgroundJPsiMass(file_bkg);

    //---------------------------------------------
    // Save output
    //---------------------------------------------
    SaveOutputRootFile(
        sig_truth,
        sig_reco,
        sig_corr,
        bkg_truth,
        bkg_reco,
        sig_yield,
        bkg_yield,
        sig_syst
    );

    file_bkg->Close();
    file_sig->Close();

    cout << " " << endl;
    cout << "Finished cross-section analysis." << endl;
    cout << "Plots saved in: " << outputDir << " and uncertaity files in " << outputUncert << endl;
    cout << " " << endl;
}
