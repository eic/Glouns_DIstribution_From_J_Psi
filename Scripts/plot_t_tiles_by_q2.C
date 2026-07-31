#include <TFile.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TSystem.h>
#include <TStyle.h>
#include <TMath.h>
#include <TLatex.h>
#include <iostream>
#include <vector>

using namespace std;

//--------------------------------------------------
// Get histogram safely
//--------------------------------------------------
TH1F* GetH1(TFile *file, const char *name)
{
    TH1F *h = nullptr;
    file->GetObject(name, h);

    if (!h)
    {
        cerr << "Missing histogram: " << name << endl;
        return nullptr;
    }

    TH1F *hc = (TH1F*)h->Clone(Form("%s_clone_%p", name, h));
    hc->SetDirectory(nullptr);

    return hc;
}

//--------------------------------------------------
// Draw one x-bin pad
//--------------------------------------------------
void DrawOnePad(TFile *file, TString xLabel)
{
    vector<TH1F*> hists = {
        GetH1(file, "h_t_MC"),
        GetH1(file, "h_t_RECO_RP"),
        GetH1(file, "h_t_RECO_B0"),
        GetH1(file, "h_t_MethodL_RECO")
        //GetH1(file, "h_t_lAger_accept")
    };

    vector<TString> labels = {
        "MC",
        "RECO RP",
        "RECO B0",
        "RECO Method 2"
        //"Accept"
    };

    vector<int> colors = {
        kBlack,
        kRed,
        kBlue,
        kViolet,
        kGreen + 2
    };

    double maxY = 0.0;

    for (auto h : hists)
    {
        if (!h) return;
        maxY = TMath::Max(maxY, h->GetMaximum());
    }

    gPad->SetLogy();
    gPad->SetLeftMargin(0.16);
    gPad->SetBottomMargin(0.14);
    gPad->SetRightMargin(0.04);
    gPad->SetTopMargin(0.08);

    TLegend *leg = new TLegend(0.50, 0.58, 0.88, 0.88);
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);
    leg->SetTextSize(0.035);

    for (int i = 0; i < (int)hists.size(); ++i)
    {
        TH1F *h = hists[i];

        h->SetStats(0);
        h->SetTitle("");

        h->GetXaxis()->SetTitle("-t [GeV^{2}]");
        h->GetYaxis()->SetTitle("Counts");

        h->GetXaxis()->SetRangeUser(0.0, 1.6);

        h->GetXaxis()->SetTitleSize(0.045);
        h->GetYaxis()->SetTitleSize(0.045);
        h->GetXaxis()->SetLabelSize(0.038);
        h->GetYaxis()->SetLabelSize(0.038);

        h->GetXaxis()->SetTitleOffset(1.1);
        h->GetYaxis()->SetTitleOffset(1.3);
        h->GetYaxis()->SetMaxDigits(3);

        h->SetMinimum(0.1);
        h->SetMaximum(maxY > 0 ? maxY * 20.0 : 10.0);

        h->SetLineColor(colors[i]);
        h->SetLineWidth(1);

        if (i == 0)
            h->Draw("HIST E");
        else
            h->Draw("HIST E SAME");

        leg->AddEntry(h, labels[i], "l");
    }

    TLatex latex;
    latex.SetNDC();
    latex.SetTextSize(0.040);
    latex.DrawLatex(0.18, 0.93, xLabel);

    leg->Draw();
}

void DrawOnePad_Corr(TFile *file, TString xLabel)
{
    TH1F *hSignal = GetH1(file, "h_methodL_RP_MethodL_B0_eff_applied");
    //TH1F *hError  = GetH1(file, "h_methodL_RP_MethodL_B0_corrected");
    TH1F *hError  = GetH1(file, "h_methodL_RP_MethodL_B0_eff_applied");

    if (!hSignal || !hError) return;

    // -------------------------------------------------
    // Copy bin errors from corrected histogram
    // -------------------------------------------------
    int nbins = TMath::Min(hSignal->GetNbinsX(), hError->GetNbinsX());

    for (int ib = 1; ib <= nbins; ++ib)
    {
        hSignal->SetBinError(ib, hError->GetBinError(ib));
    }

    vector<TH1F*> hists = { hSignal };

    vector<TString> labels = {
        "Signal"
    };

    vector<int> colors = {
        kBlack
    };

    double maxY = 0.0;

    for (auto h : hists)
    {
        if (!h) return;
        maxY = TMath::Max(maxY, h->GetMaximum());
    }

    gPad->SetLogy();
    gPad->SetLeftMargin(0.16);
    gPad->SetBottomMargin(0.14);
    gPad->SetRightMargin(0.04);
    gPad->SetTopMargin(0.08);

    TLegend *leg = new TLegend(0.50, 0.58, 0.88, 0.88);
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);
    leg->SetTextSize(0.035);

    for (int i = 0; i < (int)hists.size(); ++i)
    {
        TH1F *h = hists[i];

        h->SetStats(0);
        h->SetTitle("");

        h->GetXaxis()->SetTitle("-t [GeV^{2}]");
        h->GetYaxis()->SetTitle("Counts");

        h->GetXaxis()->SetRangeUser(0.0, 1.6);

        h->GetXaxis()->SetTitleSize(0.045);
        h->GetYaxis()->SetTitleSize(0.045);
        h->GetXaxis()->SetLabelSize(0.038);
        h->GetYaxis()->SetLabelSize(0.038);

        h->GetXaxis()->SetTitleOffset(1.1);
        h->GetYaxis()->SetTitleOffset(1.3);
        h->GetYaxis()->SetMaxDigits(3);

        h->SetMinimum(0.1);
        h->SetMaximum(maxY > 0 ? maxY * 20.0 : 10.0);

        h->SetLineColor(colors[i]);
        h->SetLineWidth(1);

        // Marker settings
        h->SetLineColor(kBlack);
        h->SetMarkerColor(kBlack);
        h->SetMarkerStyle(20);
        h->SetMarkerSize(0.8);
        h->SetLineWidth(1);

        if (i == 0)
            h->Draw("E1 X0");
        else
            h->Draw("E1 X0 SAME");

        leg->AddEntry(h, labels[i], "lep");
    }

    TLatex latex;
    latex.SetNDC();
    latex.SetTextSize(0.040);
    latex.DrawLatex(0.18, 0.93, xLabel);

    leg->Draw();
}


//--------------------------------------------------
// Main macro
//--------------------------------------------------
void plot_t_tiles_by_q2()
{
    gStyle->SetOptStat(0);

    TString inputFile =
        "benchmark_data_10x130_June_2026_From_April_2026_run_0.root";

    TString outputDir = "t_tile_plots_by_q2";
    gSystem->mkdir(outputDir, kTRUE);

    vector<pair<TString, TString>> q2Bins = {
        {"q2_1_to_1.78",    "1 < Q^{2} < 1.78"},
        {"q2_1.78_to_3.16", "1.78 < Q^{2} < 3.16"},
        {"q2_3.16_to_5.62", "3.16 < Q^{2} < 5.62"},
        {"q2_5.62_to_10",   "5.62 < Q^{2} < 10"}
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
            new TCanvas("c_" + q2Folder, q2Folder, 1800, 2200);

        TCanvas *cCorr =
            new TCanvas("cCorr_" + q2Folder, q2Folder + "_corrected", 1800, 2200);

        c->Divide(3, 4);
        cCorr->Divide(3, 4);

        for (int ix = 0; ix < (int)xBins.size(); ++ix)
        {
            TString xFolder = xBins[ix].first;
            TString xLabel  = xBins[ix].second;

            TString fileName =
                q2Folder + "/" + xFolder + "/" + inputFile;

            TFile *file = TFile::Open(fileName);

            if (!file || file->IsZombie())
            {
                cerr << "Cannot open: " << fileName << endl;

                c->cd(ix + 1);
                TLatex latex1;
                latex1.SetNDC();
                latex1.SetTextSize(0.06);
                latex1.DrawLatex(0.15, 0.55, "Missing file");
                latex1.DrawLatex(0.15, 0.45, xLabel);

                cCorr->cd(ix + 1);
                TLatex latex2;
                latex2.SetNDC();
                latex2.SetTextSize(0.06);
                latex2.DrawLatex(0.15, 0.55, "Missing file");
                latex2.DrawLatex(0.15, 0.45, xLabel);

                continue;
            }

            c->cd(ix + 1);
            DrawOnePad(file, xLabel);

            cCorr->cd(ix + 1);
            DrawOnePad_Corr(file, xLabel);

            file->Close();
        }

        c->cd();

        TLatex title;
        title.SetNDC();
        title.SetTextSize(0.020);
        title.DrawLatex(0.38, 0.985, q2Title);
        
        cCorr->cd();

        TLatex titleCorr;
        titleCorr.SetNDC();
        titleCorr.SetTextSize(0.020);
        titleCorr.DrawLatex(0.38, 0.985, q2Title);

        TString outName =
            outputDir + "/t_" + q2Folder + "_all_x_bins_3x4.pdf";

        c->SaveAs(outName);

        TString outNameCorr =
            outputDir + "/t_corr_" + q2Folder + "_all_x_bins_3x4.pdf";

        cCorr->SaveAs(outNameCorr);

        cout << "Saved: " << outName << endl;
        cout << "Saved: " << outNameCorr << endl;

        delete c;
        delete cCorr;
    }

    cout << "Finished all Q2 tile plots." << endl;
}
