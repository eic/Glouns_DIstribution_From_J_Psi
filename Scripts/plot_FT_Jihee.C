#include <TFile.h>
#include <TH1D.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TStyle.h>
#include <TSystem.h>
#include <TROOT.h>
#include <TGaxis.h>
#include <TFitResult.h>
#include <TFitResultPtr.h>
#include <TMatrixDSym.h>
#include <iostream>
#include <cmath>

using namespace std;

//--------------------------------------------------
// Style
//--------------------------------------------------
void SetPlotStyle()
{
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);
    gStyle->SetOptFit(0);

    //---------------------------------
    // Axis fonts
    //---------------------------------
    gStyle->SetLabelSize(0.040, "xyz");
    gStyle->SetTitleSize(0.045, "xyz");

    //---------------------------------
    // Axis title offsets
    //---------------------------------
    gStyle->SetTitleOffset(1.1, "x");
    gStyle->SetTitleOffset(1.25, "y");

    //---------------------------------
    // Canvas margins
    //---------------------------------
    gStyle->SetPadLeftMargin(0.20);
    gStyle->SetPadBottomMargin(0.14);
    gStyle->SetPadRightMargin(0.05);
    gStyle->SetPadTopMargin(0.05);

    //---------------------------------
    // Ticks
    //---------------------------------
    gStyle->SetPadTickX(1);
    gStyle->SetPadTickY(1);

    //---------------------------------
    // Scientific notation
    //---------------------------------
    TGaxis::SetMaxDigits(3);
}

//--------------------------------------------------
// Fit exponential A exp(-B t)
//--------------------------------------------------
TFitResultPtr FitTDistribution(
    TH1D *h,
    TString fitName,
    int color,
                               double t_min,
                               double t_max
)
{
    TF1 *fit =
        new TF1(
            fitName,
            "[0]*exp(-[1]*x)",
                t_min,
                t_max
        );

    fit->SetParNames("A", "B");
    fit->SetParameters(h->GetMaximum(), 1.0);

    fit->SetLineColor(color);
    fit->SetLineWidth(3);

    return h->Fit(fit, "R S");
}

//--------------------------------------------------
// Build Fourier transform histogram
//--------------------------------------------------
TH1D* BuildFourierHist(
    TString name,
    double A,
    double B,
    double dA,
    double dB,
    double covAB,
    int color,
    int marker
)
{
    TH1D *h =
        new TH1D(
            name,
            "",
            100,
            -10.0,
            10.0
        );

    h->Sumw2();

    for (int ibin = 1; ibin <= h->GetNbinsX(); ++ibin)
    {
        double b = h->GetBinCenter(ibin);

        
        double D = B*B + b*b, t_max = 2.0, t_min = 0.0;
        double E = exp(-t_max * B);
        double C = cos(t_max * b);
        double S = sin(t_max * b);

        double F =
            (-A * B / D) * (E*C - 1.0)
            +
            ( A * b / D) * (E*S);

        double dF_dA =
            (-B / D) * (E*C - 1.0)
            +
            ( b / D) * (E*S);

        double dF_dB =
            -A *
            (
                ((b*b - B*B) / (D*D)) * (E*C - 1.0)
                -
             t_max * (B / D) * E * C
            )
            +
            A *
            (
                -2.0 * (B*b) / (D*D) * E * S
                -
             t_max * (b / D) * E * S
            );

        double sigma2 =
            dF_dA*dF_dA*dA*dA
            +
            dF_dB*dF_dB*dB*dB
            +
            2.0*dF_dA*dF_dB*covAB;

        double sigma =
            (sigma2 > 0.0) ? sqrt(sigma2) : 0.0;

        h->SetBinContent(ibin, F);
        h->SetBinError(ibin, sigma);
    }

    h->SetStats(0);
    h->SetLineColor(color);
    h->SetMarkerColor(color);
    h->SetMarkerStyle(marker);
    h->SetMarkerSize(0.8);
    h->SetLineWidth(2);

    h->GetXaxis()->SetTitle("b_{T} [fm]");
    h->GetYaxis()->SetTitle("F(b_{T})");
    h->GetXaxis()->CenterTitle(true);
    h->GetYaxis()->CenterTitle(true);
    h->GetYaxis()->SetTitleOffset(1.5);
    h->GetYaxis()->SetMaxDigits(3);

    return h;
}

//--------------------------------------------------
// Normalize histogram by integral
//--------------------------------------------------
TH1D* NormalizeHist(
    TH1D *h,
    TString name,
    double bMin = 0.0,
    double bMax = 10.0
)
{
    TH1D *hNorm =
        (TH1D*)h->Clone(name);

    hNorm->SetDirectory(nullptr);

    int binMin =
        hNorm->GetXaxis()->FindBin(bMin);

    int binMax =
        hNorm->GetXaxis()->FindBin(bMax);

    double integral =
        hNorm->Integral(binMin, binMax, "width");

    if (integral != 0.0)
        hNorm->Scale(1.0 / integral);

    hNorm->GetXaxis()->SetRangeUser(bMin, bMax);
    hNorm->GetYaxis()->SetTitle("F(b_{T}) / #int F(b_{T}) db_{T}");

    return hNorm;
}

//--------------------------------------------------
// Main macro
//--------------------------------------------------
void plot_FT_Jihee()
{
    double t_max = 2.0, t_min = 0;
    gSystem->mkdir("dvmp_jpsi_plots", kTRUE);
    SetPlotStyle();
    
    //---------------------------------------------
    // Input file
    //---------------------------------------------
    TFile *file =
    TFile::Open("dvmp_cross_section_output.root");
    
    if (!file || file->IsZombie())
    {
        cerr << "ERROR: Cannot open dvmp_cross_section_output.root" << endl;
        return;
    }
    
    TH1 *hTruthRaw = nullptr;
    TH1 *hRecoRaw  = nullptr;
    
    file->GetObject("dsdt_signal_truth", hTruthRaw);
    file->GetObject("dsdt_signal_corrected", hRecoRaw);
    
    if (!hTruthRaw || !hRecoRaw)
    {
        cerr << "ERROR: Missing dsdt_signal_truth or dsdt_signal_corrected" << endl;
        file->ls();
        file->Close();
        return;
    }
    
    TH1D *hTruth =
    (TH1D*)hTruthRaw->Clone("hTruth_for_FT");
    
    TH1D *hReco =
    (TH1D*)hRecoRaw->Clone("hReco_for_FT");
    
    //---------------------------------------------
    // Style t distributions
    //---------------------------------------------
    hTruth->SetStats(0);
    hTruth->SetLineColor(kBlack);
    hTruth->SetLineWidth(3);
    hTruth->GetXaxis()->SetTitle("-t [GeV^{2}]");
    hTruth->GetYaxis()->SetTitle("d#sigma/dt [nb/GeV^{2}]");
    hTruth->GetXaxis()->SetRangeUser(0.0, 2.0);
    hTruth->GetYaxis()->SetRangeUser(1e-5, 1e-1);
    hTruth->GetYaxis()->SetTitleOffset(1.5);
    
    hReco->SetStats(0);
    hReco->SetLineColor(kRed);
    hReco->SetMarkerColor(kRed);
    hReco->SetMarkerStyle(24);
    hReco->SetMarkerSize(0.8);
    hReco->SetLineWidth(2);
    
    //---------------------------------------------
    // Fit t distributions
    //---------------------------------------------
    TFitResultPtr rTruth =
    FitTDistribution(
                     hTruth,
                     "fit_truth",
                     kBlack,
                     t_min,
                     t_max
                     );
    
    TFitResultPtr rReco =
    FitTDistribution(
                     hReco,
                     "fit_reco",
                     kRed,
                     t_min,
                     t_max
                     );
    
    TF1 *fitTruth =
    hTruth->GetFunction("fit_truth");
    
    TF1 *fitReco =
    hReco->GetFunction("fit_reco");
    
    //---------------------------------------------
    // Draw t fit
    //---------------------------------------------
    TCanvas *cFit =
    new TCanvas(
                "c_t_distribution_fit",
                "",
                1100,
                800
                );
    
    cFit->SetLeftMargin(0.20);
    cFit->SetBottomMargin(0.14);
    cFit->SetLogy();
    
    hTruth->Draw("HIST");
    hReco->Draw("E1 SAME");
    
    if (fitTruth) fitTruth->Draw("SAME");
    if (fitReco)  fitReco->Draw("SAME");
    
    TLegend *legFit =
    new TLegend(0.55, 0.62, 0.88, 0.88);
    
    legFit->SetBorderSize(0);
    legFit->SetFillStyle(0);
    
    legFit->AddEntry(hTruth, "Truth", "l");
    legFit->AddEntry(hReco, "Corrected RECO", "ep");
    
    if (fitTruth)
    {
        legFit->AddEntry(
                         fitTruth,
                         Form(
                              "Truth B = %.3g #pm %.2g",
                              rTruth->Parameter(1),
                              rTruth->ParError(1)
                              ),
                         "l"
                         );
    }
    
    if (fitReco)
    {
        legFit->AddEntry(
                         fitReco,
                         Form(
                              "RECO B = %.3g #pm %.2g",
                              rReco->Parameter(1),
                              rReco->ParError(1)
                              ),
                         "l"
                         );
    }
    
    legFit->Draw();
    
    cFit->SaveAs(
                 "dvmp_jpsi_plots/Jihee_t_distribution_exp_fit_clean.pdf"
                 );
    
    //---------------------------------------------
    // Extract fit parameters
    //---------------------------------------------
    const TMatrixDSym &covTruth =
    rTruth->GetCovarianceMatrix();
    
    const TMatrixDSym &covReco =
    rReco->GetCovarianceMatrix();
    
    double A_truth     = rTruth->Parameter(0);
    double B_truth     = rTruth->Parameter(1);
    double dA_truth    = rTruth->ParError(0);
    double dB_truth    = rTruth->ParError(1);
    double covAB_truth = covTruth(0,1);
    
    double A_reco     = rReco->Parameter(0);
    double B_reco     = rReco->Parameter(1);
    double dA_reco    = rReco->ParError(0);
    double dB_reco    = rReco->ParError(1);
    double covAB_reco = covReco(0,1);
    
    //---------------------------------------------
    // Build Fourier histograms
    //---------------------------------------------
    TH1D *hFTtruth =
    BuildFourierHist(
                     "hFTtruth",
                     A_truth,
                     B_truth,
                     dA_truth,
                     dB_truth,
                     covAB_truth,
                     kBlack,
                     20
                     );
    
    TH1D *hFTreco =
    BuildFourierHist(
                     "hFTreco",
                     A_reco,
                     B_reco,
                     dA_reco,
                     dB_reco,
                     covAB_reco,
                     kRed,
                     24
                     );
    
    //---------------------------------------------
    // Draw non-normalized Fourier result
    //---------------------------------------------
    TCanvas *cFT =
    new TCanvas(
                "c_fourier_non_normalized",
                "",
                900,
                800
                );
    
    cFT->SetLeftMargin(0.20);
    cFT->SetBottomMargin(0.1);
    
    hFTtruth->GetXaxis()->SetRangeUser(0.0, 10.0);
    
    hFTtruth->Draw("HIST E");
    hFTreco->Draw("E1 SAME");
    
    TLegend *legFT =
    new TLegend(0.55, 0.68, 0.88, 0.88);
    
    legFT->SetBorderSize(0);
    legFT->SetFillStyle(0);
    legFT->AddEntry(hFTtruth, "Truth", "l");
    legFT->AddEntry(hFTreco, "Corrected RECO", "ep");
    legFT->Draw();
    
    cFT->SaveAs(
                "dvmp_jpsi_plots/Jihee_ft_distribution_non_norm_clean.pdf"
                );
    
    //---------------------------------------------
    // Normalized Fourier histograms
    //---------------------------------------------
    TH1D *hFTtruthNorm =
    NormalizeHist(
                  hFTtruth,
                  "hFTtruth_norm"
                  );
    
    TH1D *hFTrecoNorm =
    NormalizeHist(
                  hFTreco,
                  "hFTreco_norm"
                  );
    
    //---------------------------------------------
    // Draw normalized Fourier result
    //---------------------------------------------
    TCanvas *cFTnorm =
    new TCanvas(
                "c_fourier_normalized",
                "",
                900,
                800
                );
    
    cFTnorm->SetLeftMargin(0.15);
    cFTnorm->SetBottomMargin(0.13);
    
    hFTtruthNorm->GetYaxis()->SetRangeUser(0.02, 0.28);

    hFTtruthNorm->GetYaxis()->SetTitleOffset(1.15);
    hFTtruthNorm->GetXaxis()->SetTitleOffset(1.0);
    hFTtruthNorm->GetXaxis()->SetTitleSize(0.04);

    hFTtruthNorm->GetYaxis()->SetLabelSize(0.040);
    hFTtruthNorm->GetYaxis()->SetTitleSize(0.050);

    hFTtruthNorm->GetXaxis()->SetLabelSize(0.040);
    hFTtruthNorm->GetXaxis()->SetTitleSize(0.050);
    hFTtruthNorm->GetXaxis()->SetRangeUser(0.0, 10.0);
    
    hFTtruthNorm->Draw("HIST E");
    hFTrecoNorm->Draw("E1 SAME");
    
    TLegend *legNorm =
    new TLegend(0.50, 0.62, 0.88, 0.88);
    
    legNorm->SetBorderSize(0);
    legNorm->SetFillStyle(0);
    
    legNorm->AddEntry(hFTtruthNorm, "Truth", "l");
    legNorm->AddEntry(hFTrecoNorm, "Corrected RECO", "ep");
    //legNorm->AddEntry((TObject*)nullptr, "#bf{ePIC Performance, 26.03.1}", "");
    //legNorm->AddEntry((TObject*)nullptr, "e+p DVMP J/#psi #rightarrow e^{+}e^{-}", "");
    //legNorm->AddEntry((TObject*)nullptr, "10 #times 130 GeV", "");
    
    legNorm->Draw();
    
    cFTnorm->SaveAs(
                    "dvmp_jpsi_plots/Jihee_ft_distribution_norm_clean.pdf"
                    );
    
    //---------------------------------------------
    // Save ROOT output
    //---------------------------------------------
    TFile *out =
    new TFile(
              "dvmp_fourier_jihee_clean_output.root",
              "RECREATE"
              );
    
    out->cd();
    
    hFTtruth->Write();
    hFTreco->Write();
    hFTtruthNorm->Write();
    hFTrecoNorm->Write();
    
    if (fitTruth) fitTruth->Write("fit_truth");
    if (fitReco)  fitReco->Write("fit_reco");
    
    out->Write();
    out->Close();
    
    file->Close();
    
    cout << endl;
    cout << "Saved Fourier plots:" << endl;
    cout << "  dvmp_jpsi_plots/Jihee_t_distribution_exp_fit_clean.pdf" << endl;
    cout << "  dvmp_jpsi_plots/Jihee_ft_distribution_non_norm_clean.pdf" << endl;
    cout << "  dvmp_jpsi_plots/Jihee_ft_distribution_norm_clean.pdf" << endl;
}
