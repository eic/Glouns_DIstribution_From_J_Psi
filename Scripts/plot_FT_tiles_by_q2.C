#include <TFile.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TSystem.h>
#include <TStyle.h>
#include <TMath.h>
#include <TLatex.h>
#include <TPad.h>

#include <iostream>
#include <vector>

using namespace std;

//--------------------------------------------------
// Get TH1D safely
//--------------------------------------------------
TH1D* GetH1D(TFile *file, const char *name)
{
    TH1D *h = nullptr;
    file->GetObject(name, h);

    if (!h)
    {
        cerr << "Missing histogram: " << name << endl;
        return nullptr;
    }

    TH1D *hc = (TH1D*)h->Clone(Form("%s_clone_%p", name, h));
    hc->SetDirectory(nullptr);

    return hc;
}

//--------------------------------------------------
// Draw one FT pad
//--------------------------------------------------
void DrawOneFTPad(TFile *file, TString xLabel)
{
    TH1D *hMC   = GetH1D(file, "hFTtruth_norm");
    TH1D *hReco = GetH1D(file, "hFTreco_norm");

    if (!hMC || !hReco)
    {
        TLatex latex;
        latex.SetNDC();
        latex.SetTextSize(0.055);
        latex.DrawLatex(0.15, 0.55, "Missing histogram");
        latex.DrawLatex(0.15, 0.45, xLabel);
        return;
    }

    gPad->SetLeftMargin(0.14);
    gPad->SetRightMargin(0.03);
    gPad->SetBottomMargin(0.13);
    gPad->SetTopMargin(0.07);

    hMC->SetStats(0);
    hReco->SetStats(0);

    hMC->SetTitle("");

    hMC->GetXaxis()->SetTitle("b_{T} [fm]");
    hMC->GetYaxis()->SetTitle("F(b_{T}) / #int F(b_{T}) db_{T}");

    hMC->GetXaxis()->SetRangeUser(-1.6, 1.6);
    hReco->GetXaxis()->SetRangeUser(-1.6, 1.6);

    hMC->GetYaxis()->SetRangeUser(0.0, 1.0);

    hMC->GetXaxis()->SetTitleSize(0.045);
    hMC->GetYaxis()->SetTitleSize(0.045);
    hMC->GetXaxis()->SetLabelSize(0.038);
    hMC->GetYaxis()->SetLabelSize(0.038);

    hMC->GetXaxis()->SetTitleOffset(1.05);
    hMC->GetYaxis()->SetTitleOffset(1.25);
    hMC->GetYaxis()->SetMaxDigits(3);

    hMC->SetLineColor(kBlack);
    hMC->SetLineWidth(2);

    hReco->SetLineColor(kBlue);
    hReco->SetMarkerColor(kBlue);
    hReco->SetMarkerStyle(24);
    hReco->SetMarkerSize(0.6);
    hReco->SetLineWidth(1);

    hMC->Draw("HIST E");
    hReco->Draw("E1 SAME");

    TLatex latex;
    latex.SetNDC();
    latex.SetTextSize(0.040);
    latex.DrawLatex(0.18, 0.93, xLabel);
}

//--------------------------------------------------
// Draw legend pad
//--------------------------------------------------
void DrawFTLegendPad()
{
    gPad->SetLeftMargin(0.05);
    gPad->SetRightMargin(0.05);
    gPad->SetBottomMargin(0.05);
    gPad->SetTopMargin(0.05);

    TH1D *hMC =
        new TH1D(Form("hMC_ft_leg_%p", gPad), "", 1, 0, 1);

    TH1D *hReco =
        new TH1D(Form("hReco_ft_leg_%p", gPad), "", 1, 0, 1);

    hMC->SetLineColor(kBlack);
    hMC->SetLineWidth(3);

    hReco->SetLineColor(kBlue);
    hReco->SetMarkerColor(kBlue);
    hReco->SetMarkerStyle(24);
    hReco->SetMarkerSize(1.0);
    hReco->SetLineWidth(2);

    TLegend *leg =
        new TLegend(0.15, 0.35, 0.88, 0.70);

    leg->SetBorderSize(0);
    leg->SetFillStyle(0);
    leg->SetTextSize(0.075);

    leg->AddEntry(hMC,   "MC",   "l");
    leg->AddEntry(hReco, "RECO", "ep");

    TLatex latex;
    latex.SetNDC();
    latex.SetTextSize(0.075);
    latex.DrawLatex(0.20, 0.80, "Legend");

    leg->Draw();
}

//--------------------------------------------------
// Main macro
//--------------------------------------------------
void plot_FT_tiles_by_q2()
{
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);

    TString inputFile =
        "dvmp_fourier_jihee_clean_output_April_2026.root";

    TString outputDir = "FT_tile_plots_by_q2";
    gSystem->mkdir(outputDir, kTRUE);

    vector<pair<TString, TString>> q2Bins = {
        {"q2_1_to_1.78",    "1 < Q^{2} < 1.78 GeV^{2}"},
        {"q2_1.78_to_3.16", "1.78 < Q^{2} < 3.16 GeV^{2}"},
        {"q2_3.16_to_5.62", "3.16 < Q^{2} < 5.62 GeV^{2}"},
        {"q2_5.62_to_10",   "5.62 < Q^{2} < 10 GeV^{2}"}
    };

    vector<pair<TString, TString>> xBins = {
        {"x_1e-4_to_2.5e-4",   "10^{-4} < x < 2.5#times10^{-4}"},
        {"x_2.5e-4_to_6.3e-4", "2.5#times10^{-4} < x < 6.3#times10^{-4}"},
        {"x_6.3e-4_to_1e-3",   "6.3#times10^{-4} < x < 10^{-3}"},
        {"x_1e-3_to_2.5e-3",   "10^{-3} < x < 2.5#times10^{-3}"},
        {"x_2.5e-3_to_6.3e-3", "2.5#times10^{-3} < x < 6.3#times10^{-3}"},
        {"x_6.3e-3_to_1e-2",   "6.3#times10^{-3} < x < 10^{-2}"},
        {"x_1e-2_to_2.5e-2",   "10^{-2} < x < 2.5#times10^{-2}"},
        {"x_2.5e-2_to_6.3e-2", "2.5#times10^{-2} < x < 6.3#times10^{-2}"},
        {"x_6.3e-2_to_0.1",    "6.3#times10^{-2} < x < 0.1"},
        {"x_0.1_to_0.25",      "0.1 < x < 0.25"},
        {"x_0.25_to_0.7",      "0.25 < x < 0.7"}
    };

    for (auto q2 : q2Bins)
    {
        TString q2Folder = q2.first;
        TString q2Title  = q2.second;

        TCanvas *c =
            new TCanvas("c_FT_" + q2Folder, q2Folder, 1800, 2200);

        c->Divide(3, 4, 0.001, 0.001);

        for (int ix = 0; ix < (int)xBins.size(); ++ix)
        {
            TString xFolder = xBins[ix].first;
            TString xLabel  = xBins[ix].second;

            TString fileName =
                q2Folder + "/" + xFolder + "/" + inputFile;

            c->cd(ix + 1);

            if (gSystem->AccessPathName(fileName))
            {
                cerr << "Missing FT file: " << fileName << endl;

                TLatex latex;
                latex.SetNDC();
                latex.SetTextSize(0.055);
                latex.DrawLatex(0.15, 0.55, "No FT output");
                latex.DrawLatex(0.15, 0.45, xLabel);

                continue;
            }

            TFile *file = TFile::Open(fileName);

            if (!file || file->IsZombie())
            {
                cerr << "Cannot open FT file: " << fileName << endl;

                TLatex latex;
                latex.SetNDC();
                latex.SetTextSize(0.055);
                latex.DrawLatex(0.15, 0.55, "Bad FT file");
                latex.DrawLatex(0.15, 0.45, xLabel);

                continue;
            }

            DrawOneFTPad(file, xLabel);

            file->Close();
        }

        c->cd(12);
        DrawFTLegendPad();

        c->cd();

        TLatex title;
        title.SetNDC();
        title.SetTextSize(0.020);
        title.DrawLatex(0.34, 0.985, q2Title);

        TString outName =
            outputDir + "/FT_DVMP_norm_" + q2Folder + "_all_x_bins_3x4.pdf";

        c->SaveAs(outName);

        cout << "Saved: " << outName << endl;

        delete c;
    }

    cout << "Finished all FT Q2 tile plots." << endl;
}
