#include <TFile.h>
#include <TNamed.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TF1.h>
#include <TGraphAsymmErrors.h>
#include <TLegend.h>
#include <TStyle.h>
#include <TSystem.h>
#include <TString.h>
#include <iostream>
#include <cmath>

using namespace std;

//---------------------------------------------
// Load fit parameters saved by dvmp_t_fitting.c
//---------------------------------------------
bool LoadFitParams(
    TString fitName,
    double &A,
    double &dA,
    double &B,
    double &dB
)
{
    TFile *file =
        TFile::Open("dvmp_t_fit_parameters.root");

    if (!file || file->IsZombie())
    {
        cerr << "ERROR: Cannot open dvmp_t_fit_parameters.root" << endl;
        return false;
    }

    TNamed *entry =
        (TNamed*)file->Get(fitName.Data());

    if (!entry)
    {
        cerr << "ERROR: Cannot find fit entry: "
             << fitName << endl;
        file->Close();
        return false;
    }

    sscanf(
        entry->GetTitle(),
        "%lf %lf %lf %lf",
        &A,
        &dA,
        &B,
        &dB
    );

    file->Close();

    return true;
}

//---------------------------------------------
// Real part function
//---------------------------------------------
TF1* MakeRealFunction(
    TString name,
    double A,
    double B,
    int color
)
{
    TF1 *f =
        new TF1(
            name,
            "([0]*[1]/([1]*[1] + x*x))*(1 - exp(-1.6*[1])*cos(1.6*x)) + "
            "([0]*x/([1]*[1] + x*x))*(exp(-1.6*[1])*sin(1.6*x))",
            0.0,
            20.0
        );

    f->SetParameters(A, B);
    f->SetLineColor(color);
    f->SetLineWidth(3);

    return f;
}

//---------------------------------------------
// Build uncertainty band
//---------------------------------------------
TGraphAsymmErrors* MakeBand(
    TString name,
    TF1 *f,
    double A,
    double dA,
    double B,
    double dB,
    int color
)
{
    const int N = 1000;

    TGraphAsymmErrors *band =
        new TGraphAsymmErrors(N);

    band->SetName(name);

    for (int i = 0; i < N; ++i)
    {
        double x =
            20.0 * i / (N - 1);

        f->SetParameters(A, B);
        double y0 = f->Eval(x);

        f->SetParameters(A + dA, B + dB);
        double yup = f->Eval(x);

        f->SetParameters(A - dA, B - dB);
        double ydn = f->Eval(x);

        band->SetPoint(i, x, y0);

        band->SetPointError(
            i,
            0.0,
            0.0,
            fabs(y0 - ydn),
            fabs(yup - y0)
        );
    }

    f->SetParameters(A, B);

    band->SetFillColorAlpha(color, 0.30);
    band->SetLineColor(color);
    band->SetLineWidth(2);

    return band;
}

//---------------------------------------------
// Fourier macro
//---------------------------------------------
void dvmp_fourier()
{
    gSystem->mkdir("dvmp_jpsi_plots", kTRUE);
    gStyle->SetOptStat(0);

    //---------------------------------------------
    // Load truth and RECO fit parameters
    //---------------------------------------------
    double A_truth  = 0.0;
    double dA_truth = 0.0;
    double B_truth  = 0.0;
    double dB_truth = 0.0;

    double A_reco  = 0.0;
    double dA_reco = 0.0;
    double B_reco  = 0.0;
    double dB_reco = 0.0;

    bool okTruth =
        LoadFitParams(
            "truth_lAger",
            A_truth,
            dA_truth,
            B_truth,
            dB_truth
        );

    bool okReco =
        LoadFitParams(
            "corrected_RPB0_MethodL",
            A_reco,
            dA_reco,
            B_reco,
            dB_reco
        );

    if (!okTruth || !okReco)
        return;

    cout << endl;
    cout << "Truth fit: A = " << A_truth
         << " +/- " << dA_truth
         << ", B = " << B_truth
         << " +/- " << dB_truth << endl;

    cout << "RECO fit : A = " << A_reco
         << " +/- " << dA_reco
         << ", B = " << B_reco
         << " +/- " << dB_reco << endl;

    //---------------------------------------------
    // Build real-part Fourier functions
    //---------------------------------------------
    TF1 *f_truth =
        MakeRealFunction(
            "f_real_truth",
            A_truth,
            B_truth,
            kBlack
        );

    TF1 *f_reco =
        MakeRealFunction(
            "f_real_reco",
            A_reco,
            B_reco,
            kRed
        );

    TGraphAsymmErrors *band_truth =
        MakeBand(
            "band_real_truth",
            f_truth,
            A_truth,
            dA_truth,
            B_truth,
            dB_truth,
            kGray + 2
        );

    TGraphAsymmErrors *band_reco =
        MakeBand(
            "band_real_reco",
            f_reco,
            A_reco,
            dA_reco,
            B_reco,
            dB_reco,
            kRed
        );

    //---------------------------------------------
    // Draw comparison
    //---------------------------------------------
    TCanvas *c =
        new TCanvas(
            "c_fourier_real_truth_reco",
            "",
            900,
            800
        );

    c->SetLeftMargin(0.18);
    c->SetBottomMargin(0.14);
    c->SetRightMargin(0.04);
    c->SetTopMargin(0.04);

    band_truth->SetTitle(
        ";b_{T} [fm];#tilde{f}(b_{T})"
    );

    band_truth->Draw("A E2");
    band_truth->GetXaxis()->SetRangeUser(0.0, 10.0);
    band_truth->GetYaxis()->SetTitleOffset(1.9);
    band_truth->GetXaxis()->SetTitleOffset(1.3);

    band_truth->GetYaxis()->SetMaxDigits(3);

    band_reco->Draw("E2 SAME");

    f_truth->Draw("L SAME");
    f_reco->Draw("L SAME");

    TLegend *leg = new TLegend(0.58, 0.58, 0.90, 0.88);

    leg->SetBorderSize(0);
    leg->SetFillStyle(0);
    leg->SetTextSize(0.025);

    leg->AddEntry(
        f_truth,
        Form(
            "Truth: A=%.3g, B=%.3g",
            A_truth,
            B_truth
        ),
        "l"
    );

    leg->AddEntry(
        band_truth,
        "Truth uncertainty",
        "f"
    );

    leg->AddEntry(
        f_reco,
        Form(
            "RECO: A=%.3g, B=%.3g",
            A_reco,
            B_reco
        ),
        "l"
    );

    leg->AddEntry(
        band_reco,
        "RECO uncertainty",
        "f"
    );

    leg->Draw();

    /*TLegend *info = new TLegend(0.50, 0.38, 0.90, 0.52);

    info->SetBorderSize(0);
    info->SetFillStyle(0);
    info->SetTextSize(0.023);

    info->AddEntry(
        (TObject*)nullptr,
        "#bf{ePIC Performance, 26.03.1}",
        ""
    );

    info->AddEntry(
        (TObject*)nullptr,
        "e+p DVMP J/#psi #rightarrow e^{+}e^{-}, 10 #times 130 GeV",
        ""
    );

    info->AddEntry(
        (TObject*)nullptr,
        "Real part of Fourier transform",
        ""
    );

    info->Draw();*/

    c->SaveAs(
        "dvmp_jpsi_plots/fourier_real_truth_vs_reco.pdf"
    );

    //---------------------------------------------
    // Save output ROOT file
    //---------------------------------------------
    TFile *out =
        new TFile(
            "dvmp_fourier_output.root",
            "RECREATE"
        );

    out->cd();

    f_truth->Write();
    f_reco->Write();
    band_truth->Write();
    band_reco->Write();

    out->Write();
    out->Close();

    cout << endl;
    cout << "Saved Fourier plot: "
         << "dvmp_jpsi_plots/fourier_real_truth_vs_reco.pdf"
         << endl;
}
