//Code written by Olaiya Olokunboyo, University of New Hampshire for
//generating gluons distribution and necessary kinematics.
//To run on the default input file (streaming files from server), make sure you are in eic-shell environment.
//Feb.23.2026
///volatile/eic/EPIC/RECO/25.07.0/epic_craterlake/EXCLUSIVE/DIFFRACTIVE_JPSI_ABCONV/lAger3.6.1-1.0/10x100/hiAcc

#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <vector>
#include <cmath>

//Root headers
#include <TFile.h>
#include <TTree.h>
#include <TTreeReader.h>
#include <TTreeReaderArray.h>
#include <TLorentzVector.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TMath.h>

#define MASS_ELECTRON 0.000510999
#define MASS_PROTON 0.938272

using std::cout;
using std::endl;
using std::ifstream;
using std::string;

void modify_jpsi() {
    
    //lets read the t-distribution from lAger
    TChain *T = new TChain("lAger");
    //T->Add("lager-vmp-00mrad.jpsi-10on100.4pi.disp-jpsi-00-electron.run00001-lumi10.root");
    T->Add("../10x130_jpsi/lager-vmp-00mrad.jpsi-10on130.4pi.disp-jpsi-00-electron.run00001-lumi10.root");
    
    // Variable to hold the branch value
    Float_t t;
    T->SetBranchAddress("t", &t);
    
    // Create histogram for -t
    TH1F *t_lAger = new TH1F("t_lAger", "-t distribution", 200, 0, 5);
    
    //TString fileList = "August_2025_1.list";
    //TString fileList = "Jan_2025_1.list"; // local
    //TString fileList = "July_25_1.list"; // stream
    TString fileList = "../10x130_jpsi/October_2025_2.list"; // stream
    //TString fileList = "../10x130_jpsi/June_1.list"; // stream
    //TString fileList = "../10x130_jpsi/August_2025_0.list"; // stream
    //TString fileList = "../10x130_jpsi/Background_full.list"; // stream
    //TString fileList = "October_2025_2.list"; // stream
    //TString fileList = "../10x130_jpsi/June_2025_full.list"; // stream
    //TString fileList = "June_1.list"; // stream
    //TString fileList = "June_2025_sep.list"; // stream
    //TString fileList = "June_2025_full.list"; // stream
    //TString fileList = "Background_noise.list"; // stream
    //TString fileList = "Background_noise_3.list"; // stream
    //TString fileList = "test.list";
    
    // Output and run configuration
    TString outputName   = "../10x130_jpsi/benchmark_data_10x130_";
    //TString outputName   = "background_";
    //TString outputName   = "benchmark_data_10x100_";
    //TString outputName   = "../10x130_jpsi/background_";
    TString date         = "Feb_2026_";
    TString run          = "run_0";
    TString fileType     = ".root";
    
    TString outputFileName = outputName + date + run + fileType;
    
    cout << "Input FileList: " << fileList << endl;
    cout << "Output file: " << outputFileName << endl;
    
    string fileName;
    TFile * inputRootFile;
    TTree * rootTree;
    
    // Open file list
    ifstream fileListStream(fileList.Data());
    if (!fileListStream) {
        cout << "ERROR: File list not found: " << fileList << endl;
        return;
    }
    
    // Electrons histograms
    TH1D *fElectron = new TH1D("fElectron","Final/Scattered Electron", 100, -10, 10);
    TH1D *rcElectron = new TH1D("rcElectron","rcElectron", 100, -10, 10);
    TH1D *B0fElectron = new TH1D("B0fElectron","B0fElectron", 100, -6, 6);
    
    // Positrons histograms
    TH1D *fPositron = new TH1D("fPositron","fp+", 100, -10, 10);
    TH1D *rcPositron = new TH1D("rcPositron","rcPositron", 100, -10, 10);
    
    // Protons histograms
    TH1D *fProton = new TH1D("fProton","", 100, 4, 12);
    TH1D *B0fProton = new TH1D("B0fProton","B0fp", 100, 4, 12);
    TH1D *RPfProton = new TH1D("RPfProton","RPfp", 100, 4, 12);
    TH1D *B0RPfProton = new TH1D("B0RPfProton","B0RPfp", 100, 4, 12);
    
    // J/ψ Mass histograms
    TH1D *JPsiMass1 = new TH1D("JPsiMass1","", 500, -10, 30);
    TH1D *rcJPsiMass1 = new TH1D("rcJPsiMass1","rcJ/#psi-Mass", 500, -10, 30);
    TH1D *rcJPsiMassa = new TH1D("rcJPsiMassa","mcJ/#psi-Mass", 500, -10, 30);
    TH1D *rcJPsiMass11 = new TH1D("rcJPsiMass11","rcJ/#psi-Mass", 500, -10, 30);
    TH1D *rcJPsiMass111 = new TH1D("rcJPsiMass111","rcJ/#psi-Mass", 500, -10, 30);
    
    // Rapidity histograms
    TH1D *mcrapid = new TH1D("mcrapid",";rapidity, #y",100,-0.01,1.05);
    TH1D *mcrapida = new TH1D("mcrapida"," ",100,-0.01,1.05);
    TH1D *mcrapidrc = new TH1D("mcrapidrc"," ",100,-0.01,1.05);
    
    TH1D *t_anotherhist = new TH1D("t_anotherhist",";t [GeV]^{2}", 200, 0, 5);
    TH1D *t_anotherhist_neg = new TH1D("t_anotherhist_neg",";t [GeV]^{2}", 200, -5, 0);
    TH1D *t_anotherhist_acc = new TH1D("t_anotherhist_acc"," ", 200, 0, 5);
    TH1D *t_anotherhist_rc = new TH1D("t_anotherhist_rc"," ", 200, 0, 5);
    TH2D* t_another_2D = new TH2D("t_another_2D"," ", 200, 0, 2, 200, 0, 2);
    
    //Resolution/differnces
    TH1D *diffjpsimass = new TH1D("diffjpsimass"," ", 500, -5, 5);
    TH1D *diffx_v = new TH1D("diffx_v"," ", 500, -5, 5);
    TH1D *diffxbjk = new TH1D("diffxbjk"," ", 500, -5, 5);
    TH1D *diffq2 = new TH1D("diffq2"," ", 500, -5, 5);
    TH1D *diffq2_2 = new TH1D("diffq2_2"," ", 500, -5, 5);
    TH1D *diffy = new TH1D("diffy"," ", 500, -5, 5);
    TH1D *diff_eSig_q2 = new TH1D("diff_eSig_q2"," ", 500, -5, 5);
    TH1D *diff_eSig_xbjk = new TH1D("diff_eSig_xbjk"," ", 500, -5, 5);
    TH1D *diff_eSig_y = new TH1D("diff_eSig_y"," ", 500, -5, 5);
    TH1D *diff_JB_q2 = new TH1D("diff_JB_q2"," ", 500, -5, 5);
    TH1D *diff_DA_q2 = new TH1D("diff_DA_q2"," ", 500, -5, 5);
    TH1D *diff_JB_y = new TH1D("diff_JB_y"," ", 500, -5, 5);
    TH1D *diff_DA_y = new TH1D("diff_DA_y"," ", 500, -5, 5);
    TH1D *diff_JB_xbjk = new TH1D("diff_JB_xbjk"," ", 500, -5, 5);
    TH1D *diff_DA_x = new TH1D("diff_DA_x"," ", 500, -5, 5);
    
    //Missing Transverse Quantities
    TH1D *mtg = new TH1D("mtg","", 200, 0, 5);
    TH1D *mtgB0 = new TH1D("mtgB0","", 200, 0, 5);
    TH1D *mtgTot = new TH1D("mtgTot","", 200, 0, 5);
    TH1D *mtgTot1 = new TH1D("mtgTot1","", 200, 0, 5);
    TH1D *mtgg = new TH1D("mtgg","", 200, 0, 5);
    TH1D *mtggtest = new TH1D("mtggtest","", 200, -5, 0);
    TH1D *mtggtest_rc = new TH1D("mtggtest_rc","", 200, -5, 0);
    TH1D *mtgg_accept = new TH1D("mtgg_accept","", 200, 0, 5);
    TH1D *two_meth_tdist = new TH1D("two_meth_tdist","", 200, 0, 5);
    TH1D *two_meth_tdist_accept = new TH1D("two_meth_tdist_accept","", 200, 0, 5);
    TH1D *mtq = new TH1D("mtq","", 50, 0, 11);
    TH1D *mtqq = new TH1D("mtqq","", 50, 0, 11);
    TH1D *mtqq_2 = new TH1D("mtqq_2","", 50, 0, 11);
    TH1D *mtq_acc = new TH1D("mtq_acc","", 50, 0, 11);
    
    //eSigma Method Kinematics
    TH1D *eSig_y_MC = new TH1D("eSig_y_MC","", 100,-0.01,1.05);
    TH1D *eSig_y_MC_a = new TH1D("eSig_y_MC_a","", 100,-0.01,1.05);
    TH1D *eSig_y_RC = new TH1D("eSig_y_RC","", 100,-0.01,1.05);
    TH1D *eSig_q2_MC = new TH1D("eSig_q2_MC","", 50, 0, 11);
    TH1D *eSig_q2_acc = new TH1D("eSig_q2_acc","", 50, 0, 11);
    TH1D *eSig_q2_RC = new TH1D("eSig_q2_RC","", 50, 0, 11);
    TH1D *eSig_x_MC = new TH1D("eSig_x_MC","", 2000, -0.01, 1.01);
    TH1D *eSig_x_RC = new TH1D("eSig_x_RC","", 2000, -0.01, 1.01);
    TH1D *eSig_x_acc = new TH1D("eSig_x_acc","", 2000, -0.01, 1.01);
    TH1D *eS_HFS_MC = new TH1D("eS_HFS_MC","", 100, 0, 30);
    TH1D *eS_HFS_acc = new TH1D("eS_HFS_acc","", 100, 0, 30);
    TH1D *eS_HFS_rc = new TH1D("eS_HFS_rc","", 100, 0, 30);
    
    //Jacquet-Blondel (JB) Method Kinematics
    TH1D *JB_pt2_MC = new TH1D("JB_pt2_MC","", 100, 0, 5);
    TH1D *JB_pt2_acc = new TH1D("JB_pt2_acc","", 100, 0, 5);
    TH1D *JB_pt2_rc = new TH1D("JB_pt2_rc","", 100, 0, 5);
    TH1D *JB_HFS_MC = new TH1D("JB_HFS_MC","", 100, 0, 30);
    TH1D *JB_HFS_acc = new TH1D("JB_HFS_acc","", 100, 0, 30);
    TH1D *JB_HFS_rc = new TH1D("JB_HFS_rc","", 100, 0, 30);
    TH1D *JB_y_MC = new TH1D("JB_y_MC","", 100,-0.01,1.05);
    TH1D *JB_y_MC_a = new TH1D("JB_y_MC_a","", 100,-0.01,1.05);
    TH1D *JB_y_RC = new TH1D("JB_y_RC","", 100,-0.01,1.05);
    TH1D *JB_q2_MC = new TH1D("JB_q2_MC","", 50, 0, 11);
    TH1D *JB_q2_acc = new TH1D("JB_q2_acc","", 50, 0, 11);
    TH1D *JB_q2_RC = new TH1D("JB_q2_RC","", 50, 0, 11);
    TH1D *JB_x_MC = new TH1D("JB_x_MC","", 2000, -0.01, 1.01);
    TH1D *JB_x_RC = new TH1D("JB_x_RC","", 2000, -0.01, 1.01);
    TH1D *JB_x_acc = new TH1D("JB_x_acc","", 2000, -0.01, 1.01);
    
    //double angle method
    TH1D *DA_y_MC = new TH1D("DA_y_MC","", 100,-0.01,1.05);
    TH1D *DA_y_RC = new TH1D("DA_y_RC","", 100,-0.01,1.05);
    TH1D *DA_y_MC_a = new TH1D("DA_y_MC_a","", 100,-0.01,1.05);
    TH1D *DA_q2_MC = new TH1D("DA_q2_MC","", 50, 0, 11);
    TH1D *DA_q2_acc = new TH1D("DA_q2_acc","", 50, 0, 11);
    TH1D *DA_q2_RC = new TH1D("DA_q2_RC","", 50, 0, 11);
    TH1D *DA_x_MC = new TH1D("DA_x_MC","", 2000, -0.01, 1.01);
    TH1D *DA_x_RC = new TH1D("DA_x_RC","", 2000, -0.01, 1.01);
    TH1D *DA_x_acc = new TH1D("DA_x_acc","", 2000, -0.01, 1.01);
    
    //Inclusive Variables
    TH1D *EM_x = new TH1D("EM_x","", 2000, -0.01, 1.01);
    TH1D *EM_Q2 = new TH1D("EM_Q2","", 50, 0, 11);
    TH1D *EM_y = new TH1D("EM_y","", 100,-0.01,1.05);
    TH1D *ESig_x_in = new TH1D("ESig_x_in","", 2000, -0.01, 1.01);
    TH1D *ESig_Q2_in = new TH1D("ESig_Q2_in","", 50, 0, 11);
    TH1D *ESig_y_in = new TH1D("ESig_y_in","", 100,-0.01,1.05);
    TH1D *JB_x_in = new TH1D("JB_x_in","", 2000, -0.01, 1.01);
    TH1D *JB_Q2_in = new TH1D("JB_Q2_in","", 50, 0, 11);
    TH1D *JB_y_in = new TH1D("JB_y_in","", 100,-0.01,1.05);
    TH1D *DA_x_in = new TH1D("DA_x_in","", 2000, -0.01, 1.01);
    TH1D *DA_Q2_in = new TH1D("DA_Q2_in","", 50, 0, 11);
    TH1D *DA_y_in = new TH1D("DA_y_in","", 100,-0.01,1.05);
    
    //x-Variables
    TH1D *xb = new TH1D("xb","",  2000, -0.01, 1.01);
    TH1D *xba = new TH1D("xba","",  2000, -0.01, 1.01);
    TH1D *xb1 = new TH1D("xb1","",  2000, -0.01, 1.01);
    TH1D *xb2 = new TH1D("xb2","",  2000, -0.01, 1.01);
    TH1D *xv = new TH1D("xv","",  2000, -0.01, 1.01);
    TH1D *xv1 = new TH1D("xv1","",  2000, -0.01, 1.01);
    TH1D *xv2 = new TH1D("xv2","",  2000, -0.01, 1.01);
    
    //eta/pt and energy_vs_pz
    TH1D *ecaleta = new TH1D("ecaleta","ecaleta", 100, -6, 6);
    TH1D* rcElectronpt = new TH1D("rcElectronpt", ";p_{t} [GeV/c]", 100, -10, 10.0);
    TH1D* rcPositronpt = new TH1D("rcPositronpt", ";p_{t} [GeV/c]", 100, -10, 10.0);
    TH1D* rcElectronjpsipt = new TH1D("rcElectronjpsipt", ";p_{t} [GeV/c]", 100, -10,10.0);
    TH1D* rcElectronjpsieta = new TH1D("rcElectronjpsieta",";Pseudorapidity, #eta",100,-10,10.0);
    TH1D* mcjpsie_e_minus_pzmc = new TH1D("mcjpsie_e_minus_pzmc"," ",500, -50, 150.0);
    TH1D* rcjpsie_e_minus_pz = new TH1D("rcjpsie_e_minus_pz","rcjpsie_e_minus_pz",500, -50, 150.0);
    
    //2D histograms
    TH2D* q2_2D = new TH2D("q2_2D","q2_2D", 100, 1, 10, 100, 1, 10);
    TH2D* xbjk_2D = new TH2D("xbjk_2D","xbjk_2D", 500, 0, 0.5, 500, 0, 0.5);
    TH2D* y_2D = new TH2D("y_2D","y_2D", 100, 0, 1, 100, 0, 1);
    TH2D* t_2D = new TH2D("t_2D","t_2D", 100, 0, 2, 100, 0, 2);
    TH2D* dxbjk_2D = new TH2D("dxbjk_2D","dxbjk_2D", 500, 0, 0.03, 100, -1, 1);
    TH2D* dybjk_2D = new TH2D("dybjk_2D","dybjk_2D", 100, 0.1, 1, 100, -1, 1);
    TH2D* dq2bjk_2D = new TH2D("dq2bjk_2D","dq2bjk_2D", 100, 0, 10, 100, -1, 1);
    TH2D* eEta2D = new TH2D("eEta2D","eEta2D", 100, -4, 4, 100, -4, 4);
    TH2D* pEta2D = new TH2D("pEta2D","pEta2D", 100, -4, 4, 100, -4, 4);
    TH2D* JB_HFS_2D = new TH2D("JB_HFS_2D"," ", 100, 0, 21, 100, 0, 21);
    
    //eSigma 2D
    TH2D* eSig_q2_2D = new TH2D("eSig_q2_2D"," ", 100, 1, 10, 100, 1, 10);
    TH2D* deSig_q2_2D = new TH2D("deSig_q2_2D"," ", 100, 1, 10, 100, -1, 1);
    TH2D* eSig_y_2D = new TH2D("eSig_y_2D"," ", 100, 0, 1, 100, 0, 1);
    TH2D* deSig_y_2D = new TH2D("deSig_y_2D"," ", 100, 0, 1, 100, -1, 1);
    TH2D* eSig_xbjk_2D = new TH2D("eSig_xbjk_2D"," ", 100, 0, 0.5, 100, 0, 0.5);
    TH2D* deSig_xbjk_2D = new TH2D("deSig_xbjk_2D"," ", 100, 0, 0.5, 100, -1, 1);
    
    //JB
    TH2D* JB_q2_2D = new TH2D("JB_q2_2D"," ", 100, 1, 10, 100, 1, 10);
    TH2D* dJB_q2_2D = new TH2D("dJB_q2_2D"," ", 100, 1, 10, 100, -1, 1);
    TH2D* JB_y_2D = new TH2D("JB_y_2D"," ", 100, 0, 1, 100, 0, 1);
    TH2D* dJB_y_2D = new TH2D("dJB_y_2D"," ", 100, 0, 1, 100, -1, 1);
    TH2D* JB_xbjk_2D = new TH2D("JB_xbjk_2D"," ", 100, 0, 0.5, 100, 0, 0.5);
    TH2D* dJB_xbjk_2D = new TH2D("dJB_xbjk_2D"," ", 100, 0, 0.5, 100, -1, 1);
    
    //DA
    TH2D* DA_x_2D = new TH2D("DA_q2_2D"," ", 500, 0, 0.5, 100, 0, 0.5);
    TH2D* DA_q2_2D = new TH2D("DA_q2_2D"," ", 100, 1, 10, 100, 1, 10);
    TH2D* DA_y_2D = new TH2D("DA_y_2D"," ", 100, 0, 1, 100, 0, 1);
    
    //fProton
    TH1D* h6_eta_MC = new TH1D("h6_eta",";Pseudorapidity, #eta",100,0.0,15.0);
    TH1D* h6_px_MC = new TH1D("px6_MC", ";p_{x} [GeV/c]", 100, -8.5, 10);
    TH1D* h6_py_MC = new TH1D("py6_MC", ";p_{y} [GeV/c]", 100, -2.0, 2.0);
    TH1D* h6_pt_MC = new TH1D("pt6_MC", ";p_{t} [GeV/c]", 100, -1, 2);
    TH1D* h6_pz_MC = new TH1D("pz6_MC", ";p_{z} [GeV/c]", 100, 0.0, 320.0);
    TH1D* h6_e_MC = new TH1D("e6_MC", ";E [GeV]", 100, 0.0, 320.0);
    
    //fPositron
    TH1D* h7_eta_MC = new TH1D("h7_eta",";Pseudorapidity, #eta",100,-10,10.0);
    TH1D* h7_px_MC = new TH1D("px7_MC", ";p_{x} [GeV/c]", 100, -10.0, 10.0);
    TH1D* h7_py_MC = new TH1D("py7_MC", ";p_{y} [GeV/c]", 100, -10.0, 10.0);
    TH1D* h7_pt_MC = new TH1D("pt7_MC", ";p_{t} [GeV/c]", 100, -10, 10.0);
    TH1D* h7_pz_MC = new TH1D("pz7_MC", ";p_{z} [GeV/c]", 100, 0.0, 320.0);
    TH1D* h7_e_MC = new TH1D("e7_MC", ";E [GeV]", 100, 0.0, 320.0);
    
    //fElectron
    TH1D* h8_eta_MC = new TH1D("h8_eta",";Pseudorapidity, #eta",100,-10,10.0);
    TH1D* h8_px_MC = new TH1D("px8_MC", ";p_{x} [GeV/c]", 100, -10.0, 10.0);
    TH1D* h8_py_MC = new TH1D("py8_MC", ";p_{y} [GeV/c]", 100, -10.0, 10.0);
    TH1D* h8_pt_MC = new TH1D("pt8_MC", ";p_{t} [GeV/c]", 100, -10, 10.0);
    TH1D* h8_pz_MC = new TH1D("pz8_MC", ";p_{z} [GeV/c]", 100, 0.0, 320.0);
    TH1D* h8_e_MC = new TH1D("e8_MC", ";E [GeV]", 100, 0.0, 320.0);
    
    //jpsi electron
    TH1D* hjpsi_eta_MC = new TH1D("hjpsi_eta",";Pseudorapidity, #eta",100,-10,10.0);
    TH1D* hjpsi_px_MC = new TH1D("pxjpsi_MC", ";p_{x} [GeV/c]", 100, -10.0, 10.0);
    TH1D* hjpsi_py_MC = new TH1D("pyjpsi_MC", ";p_{y} [GeV/c]", 100, -10.0, 10.0);
    TH1D* hjpsi_pt_MC = new TH1D("ptjpsi_MC", ";p_{t} [GeV/c]", 100, -10, 10.0);
    TH1D* hjpsi_pz_MC = new TH1D("pzjpsi_MC", ";p_{z} [GeV/c]", 100, 0.0, 320.0);
    TH1D* hjpsi_e_MC = new TH1D("ejpsi_MC", ";E [GeV]", 100, 0.0, 320.0);
    
    TH1D* h1_eta_MC = new TH1D("h1_eta",";Pseudorapidity, #eta",100,0.0,15.0);
    TH1D* h1_px_MC = new TH1D("px1_MC", ";p_{x} [GeV/c]", 100, -10.0, 10.0);
    TH1D* h1_py_MC = new TH1D("py1_MC", ";p_{y} [GeV/c]", 100, -10.0, 10.0);
    TH1D* h1_pt_MC = new TH1D("pt1_MC", ";p_{t} [GeV/c]", 100, 0.0, 10.0);
    TH1D* h1_pz_MC = new TH1D("pz1_MC", ";p_{z} [GeV/c]", 100, 0.0, 320.0);
    TH1D* h1_e_MC = new TH1D("e1_MC", ";E [GeV]", 100, 0.0, 320.0);
    
    TH1D* h2_e_MC = new TH1D("e2_MC", ";E [GeV]", 200, -20, 20.0);
    TH1D* h2_e_acc = new TH1D("h2_e_acc", ";E [GeV]", 200, -20, 20.0);
    
    TH1D* scat_e_RC = new TH1D("scat_e_RC", ";E [GeV]", 200, -20, 20.0);
    TH1D* scat_e_EcalEnd = new TH1D("scat_e_EcalEnd", ";E [GeV]", 200, -20, 20.0);
    TH1D* Ecal_vs_pz = new TH1D("Ecal_vs_pz", ";E/P", 200, -5, 5.0);
    TH1D* e_pz_MC = new TH1D("e_pz_MC", ";E/p_{z}", 200, 0.9, 2);
    TH1D* e_pz_acc = new TH1D("e_pz_acc", ";E/p_{z}", 200, 0.9, 2);
    TH1D* e_pz_rc = new TH1D("e_pz_rc", ";E/p_{z}", 200, 0.9, 2);
    TH2D* e_pz_2D = new TH2D("e_pz_2D", "E/p_{z} (Reco);E/p_{z} (MC)", 100, 0.9, 2, 100, 0.9, 2);
    
    TH1D* E_P_final_SCe_MC = new TH1D("E_P_final_SCe_MC", ";E/p_{z}", 200, 0, 2);
    TH1D* E_P_final_DCe_MC = new TH1D("E_P_final_DCe_MC", ";E/p_{z}", 200, 0, 2);
    TH1D* E_P_final_Posit_MC = new TH1D("E_P_final_Posit_MC", ";E/p_{z}", 200, 0, 2);
    TH1D* E_P_final_SCe_RC = new TH1D("E_P_final_SCe_RC", ";E/p_{z}", 200, 0, 2);
    TH1D* E_P_final_DCe_RC = new TH1D("E_P_final_DCe_RC", ";E/p_{z}", 200, 0, 2);
    TH1D* E_P_final_Posit_RC = new TH1D("E_P_final_Posit_RC", ";E/p_{z}", 200, 0, 2);
    
    //Roman pots
    TH1D* h_px_RomanPots = new TH1D("px_RomanPots", ";p_{x} [GeV/c]", 100, -10.0, 10.0);
    TH1D* h_py_RomanPots = new TH1D("py_RomanPots", ";p_{y} [GeV/c]", 100, -10.0, 10.0);
    TH1D* h_pt_RomanPots = new TH1D("pt_RomanPots", ";p_{t} [GeV/c]", 100, -1, 2.0);
    TH1D* h_pt_B0RomanPots = new TH1D("h_pt_B0RomanPots", ";p_{t} [GeV/c]", 100, -1, 2.0);
    TH1D* h_pz_RomanPots = new TH1D("pz_RomanPots", ";p_{z} [GeV/c]", 100, 0.0, 320.0);
    TH1D* h_e_RomanPots = new TH1D("e_RomanPots", ";E [GeV]", 100, 0.0, 320.0);
    TH2D* h_rp_occupancy_map = new TH2D("Roman_pots_occupancy_map", "hit y [mm];hit x [mm]", 100, -1250, -1000, 100, -30, 30);
    
    
    //B0 tracker hits
    TH2D* h_B0_occupancy_map_layer_0 = new TH2D("B0_occupancy_map_0", "B0_occupancy_map_0", 100, -400, 0, 100, -170, 170);
    TH2D* h_B0_occupancy_map_layer_1 = new TH2D("B0_occupancy_map_1", "B0_occupancy_map_1", 100, -400, 0, 100, -170, 170);
    TH2D* h_B0_occupancy_map_layer_2 = new TH2D("B0_occupancy_map_2", "B0_occupancy_map_2", 100, -400, 0, 100, -170, 170);
    TH2D* h_B0_occupancy_map_layer_3 = new TH2D("B0_occupancy_map_3", "B0_occupancy_map_3", 100, -400, 0, 100, -170, 170);
    TH1D* h_B0_hit_energy_deposit = new TH1D("B0_tracker_hit_energy_deposit", ";Deposited Energy [keV]", 100, 0.0, 500.0);
    
    //B0 EMCAL clusters
    TH2D* h_B0_emcal_occupancy_map = new TH2D("B0_emcal_occupancy_map", "B0_emcal_occupancy_map", 100, -400, 0, 100, -170, 170);
    TH1D* h_B0_emcal_cluster_energy = new TH1D("B0_emcal_cluster_energy", ";Cluster Energy [GeV]", 100, 0.0, 100.0);
    
    //Reconstructed tracks (for usage with B0 too!!)
    TH1D* h_px_reco_track = new TH1D("px_reco_track", ";p_{x} [GeV/c]", 100, -10.0, 10.0);
    TH1D* h_py_reco_track = new TH1D("py_reco_track", ";p_{y} [GeV/c]", 100, -10.0, 10.0);
    TH1D* h_pt_reco_track = new TH1D("pt_reco_track", ";p_{t} [GeV/c]", 100, -1, 4);
    TH1D* h_pz_reco_track = new TH1D("pz_reco_track", ";p_{z} [GeV/c]", 100, 0.0, 320);
    TH1D* h_e_reco_track = new TH1D("e_reco_track", ";E [GeV]", 100, 0.0, 320);
    
    //testing
    TH1D* Teta = new TH1D("Teta", " ", 100, -0.001, 0.025);
    TH1D* Teta_e = new TH1D("Teta_e", " ", 100, 2, 3.2);
    TH1D* Teta_e_acc = new TH1D("Teta_e_acc", " ", 100, 2, 3.2);
    TH1D* Teta_e_rc = new TH1D("Teta_e_rc", " ", 100, 2, 3.2);
    TH1D* TetaRP = new TH1D("TetaRP", " ", 100, -0.001, 0.025);
    TH1D* TetaB0 = new TH1D("TetaB0", " ", 100, -0.001, 0.025);
    TH1D* TetaAC = new TH1D("TetaAC", " ", 100, -0.001, 0.025);
    TH1D* TetaRPB0 = new TH1D("TetaRPB0", " ", 100, -0.001, 0.025);
    TH2D* theta2D = new TH2D("theta2D"," ", 200, 0, 0.025, 200, 0, 0.025);
    TH2D* theta2D_e = new TH2D("theta2D_e"," ", 200, 2, 3.2, 200, 2, 3.2);
    TH2D* q2_v_x_e = new TH2D("q2_v_x_e"," ", 200, 0, 0.1, 200, 1, 10);
    TH2D* q2_v_x_e_rc = new TH2D("q2_v_x_e_rc"," ", 200, 0, 0.1, 200, 1, 10);
    TH2D* energy_2D = new TH2D("energy_2D"," ", 200, 0, 10, 200, 0, 10);
    TH2D* q2vst = new TH2D("q2vst"," ", 200, 0, 2, 200, 0, 10);
    TH2D* q2vst_acc = new TH2D("q2vst_acc"," ", 200, 0, 2, 200, 0, 10);
    TH2D* t2D_with_diff_rp = new TH2D("t2D_with_diff_rp"," ", 200, 0, 2, 200, 0, 2);
    TH2D* t2D_with_diff_B0 = new TH2D("t2D_with_diff_B0"," ", 200, 0, 2, 200, 0, 2);
    TH2D* t2D_with_diff_methL = new TH2D("t2D_with_diff_methL"," ", 200, 0, 2, 200, 0, 2);
    TH2D* t2D_with_diff_allCroll = new TH2D("t2D_with_diff_allCroll"," ", 200, 0, 2, 200, 0, 2);
    
    TH2D* eptest = new TH2D("eptest", " ", 100, -2, 2, 100, -2, 2);
    TH1D* AllMCFinalEta = new TH1D("AllMCFinalEta", " ; #eta", 200, -10, 10);
    
    //purity and stabilty
    int nbins = 100;
    double q2min = 1, q2max = 10.0;
    TH2D* h2_migration = new TH2D("h2_migration", "Reco vs MC Q^{2};Q^{2}_{MC};Q^{2}_{Reco}",
                                  nbins, q2min, q2max,
                                  nbins, q2min, q2max);
    TH1D* h_purity = new TH1D("h_purity", "Purity per Q^{2} bin;Q^{2}_{Reco} bin;Purity", nbins, q2min, q2max);
    TH1D* h_stability = new TH1D("h_stability", "Stability per Q^{2} bin;Q^{2}_{Truth} bin;Stability", nbins, q2min, q2max);
    TH1D* h_purity_JB = new TH1D("h_purity_JB", "Purity per Q^{2} bin;Q^{2}_{Reco} bin;Purity", nbins, q2min, q2max);
    TH1D* h_stability_JB = new TH1D("h_stability_JB", "Stability per Q^{2} bin;Q^{2}_{Truth} bin;Stability", nbins, q2min, q2max);
    TH1D* h_purity_eS = new TH1D("h_purity_eS", "Purity per Q^{2} bin;Q^{2}_{Reco} bin;Purity", nbins, q2min, q2max);
    TH1D* h_stability_eS = new TH1D("h_stability_eS", "Stability per Q^{2} bin;Q^{2}_{Truth} bin;Stability", nbins, q2min, q2max);
    
    TH1D* h_missingMass = new TH1D("h_missingMass", ";Missing mass;", 100, -10, 10);
    TH1D* h_missingMass2 = new TH1D("h_missingMass2", ";Missing mass squared;", 100, -10, 20);
    TH1D* h_missingMass_rc = new TH1D("h_missingMass_rc", ";Missing mass RECO;", 100, -10, 10);
    TH1D* h_missingMass2_rc = new TH1D("h_missingMass2_rc", ";Missing mass squared RECO;", 100, -10, 20);
    
    
    
    // File and event counters
    int fileCounter = 0;
    int iflagproton = 0, iflagproton2 = 0;
    int iflagproton_rp = 0, iflagproton2_B0 = 0;
    
    // t-distribution variables
    double t1 = 0, t2 = 0, t3 = 0, t_Tot = 0;
    double t_accept1 = 0, t_accept2 = 0;
    
    // Scattering angles and energies
    double thetaMC = 0, thetaChPct = 0, raprc = 0;
    double scatElecEnergChPct = 0, scatElecEnergChPct2 = 0;
    
    // Jacquet-Blondel method variables
    double jbpt2 = 0, jbsig = 0;
    double y_JB = 0, q2_JB = 0, x_JB = 0;
    double jbpt2_rc = 0, jbsig_rc = 0;
    double y_JB_rc = 0, q2_JB_rc = 0, x_JB_rc = 0;
    
    // Electron method (eSigma) variables
    double y_e = 0, q2_e = 0, q2_eSig = 0, y_eSig = 0;
    double Sig_e = 0, Sig_tot = 0, y_Sig = 0, x_Sig = 0, q2_Sig = 0;
    
    // Electron method with radiative corrections
    double y_e_rc = 0, q2_e_rc = 0, q2_eSig_rc = 0, y_eSig_rc = 0;
    double Sig_e_rc = 0, Sig_tot_rc = 0, y_Sig_rc = 0, x_Sig_rc = 0, q2_Sig_rc = 0;
    
    // MC truth and other analysis variables
    double q2 = 0, q2_1 = 0, q2_2 = 0;
    double ixb = 0, ixv = 0, ixb1 = 0, ixb2 = 0, ixv1 = 0, ixv2 = 0;
    double ixba = 0, rap = 0, rapa = 0, caleta = 0;
    
    // Energy-momentum method values
    double EM_x_value = 0, EM_Q2_value = 0, EM_y_value = 0;
    
    // eSigma input values
    double ESig_x_in_value = 0, ESig_Q2_in_value = 0, ESig_y_in_value = 0;
    
    // JB input values
    double JB_x_in_value = 0, JB_Q2_in_value = 0, JB_y_in_value = 0;
    // DA input values
    double DA_x_in_value = 0, DA_Q2_in_value = 0, DA_y_in_value = 0;
    
    // Weights and total momentum
    double tot_p = 0, tg1 = 0, tg2 = 0;
    double weight = 0, weight1 = 0, weight2 = 0, t_another = 0, t_another_rc = 0, t_another_acc = 0, t_another_neg = 0;
    int z0 = 0, z1 = 0, z2 = 0, z3 = 0, z4 = 0, z5 = 0, z55 = 0, z6 = 0, mcj = 0, rcj = 0, t_di = 0;//counting
    
    // Luminosity in nb^-1 (10 fb^-1)
    const double lumi_gen = 1.0e7;
    const double lumi_gen_ub = 1.0e10;  // 10 fb^-1 = 1e10 ub^-1
    
    
    while (getline(fileListStream, fileName)) {
        TString tmpFileName = fileName;
        cout << "Input file " << fileCounter << ": " << tmpFileName << endl;
        
        auto inputRootFile = std::unique_ptr<TFile>(TFile::Open(tmpFileName));
        if (!inputRootFile) {
            cerr << "ERROR: Missing ROOT file: " << tmpFileName << endl;
            continue;
        }
        
        fileCounter++;
        
        //TTree* evtTree = static_cast<TTree*>(inputRootFile->Get("events"));
        TTree* evtTree = (TTree*)inputRootFile->Get("events");
        if (!evtTree) {
            cerr << "ERROR: Could not find 'events' tree in " << tmpFileName << endl;
            continue;
        }
        
        int numEvents = evtTree->GetEntries();
        cout << "File contains " << numEvents << " events." << endl;
        
        TTreeReader treeReader(evtTree);
        
        // MC particles
        TTreeReaderArray<double> mc_px(treeReader, "MCParticles.momentum.x");
        TTreeReaderArray<double> mc_py(treeReader, "MCParticles.momentum.y");
        TTreeReaderArray<double> mc_pz(treeReader, "MCParticles.momentum.z");
        TTreeReaderArray<double> mc_mass(treeReader, "MCParticles.mass");
        TTreeReaderArray<int> mc_pdg(treeReader, "MCParticles.PDG");
        TTreeReaderArray<int> mc_gen_status(treeReader, "MCParticles.generatorStatus");
        TTreeReaderArray<float> mc_charge(treeReader, "MCParticles.charge");
        TTreeReaderArray<int> mc_sim_status(treeReader, "MCParticles.simulatorStatus");
        TTreeReaderArray<unsigned int> mc_parents_begin(treeReader, "MCParticles.parents_begin");
        TTreeReaderArray<unsigned int> mc_parents_end(treeReader, "MCParticles.parents_end");
        TTreeReaderArray<unsigned int> mc_daughters_begin(treeReader, "MCParticles.daughters_begin");
        TTreeReaderArray<unsigned int> mc_daughters_end(treeReader, "MCParticles.daughters_end");
        
        TTreeReaderArray<float> reco_track_central_x = {treeReader, "CentralCKFTruthSeededTracks.momentum.x"};
        TTreeReaderArray<float> reco_track_central_y = {treeReader, "CentralCKFTruthSeededTracks.momentum.y"};
        TTreeReaderArray<float> reco_track_central_z = {treeReader, "CentralCKFTruthSeededTracks.momentum.z"};
        TTreeReaderArray<int> reco_track_central_PDG = {treeReader, "CentralCKFTruthSeededTracks.pdg"};
        TTreeReaderArray<float> reco_track_central_charge = {treeReader, "CentralCKFTruthSeededTracks.charge"};
        
        TTreeReaderArray<float> reco_track_CKFT_x = {treeReader, "CentralCKFTracks.momentum.x"};
        TTreeReaderArray<float> reco_track_CKFT_y = {treeReader, "CentralCKFTracks.momentum.y"};
        TTreeReaderArray<float> reco_track_CKFT_z = {treeReader, "CentralCKFTracks.momentum.z"};
        TTreeReaderArray<int> reco_track_CKFT_PDG = {treeReader, "CentralCKFTracks.pdg"};
        TTreeReaderArray<float> reco_track_CKFT_charge = {treeReader, "CentralCKFTracks.charge"};
        
        // Roman Pots
        TTreeReaderArray<float> rp_px(treeReader, "ForwardRomanPotRecParticles.momentum.x");
        TTreeReaderArray<float> rp_py(treeReader, "ForwardRomanPotRecParticles.momentum.y");
        TTreeReaderArray<float> rp_pz(treeReader, "ForwardRomanPotRecParticles.momentum.z");
        TTreeReaderArray<int>   rp_pdg(treeReader, "ForwardRomanPotRecParticles.PDG");
        TTreeReaderArray<float> rp_mass(treeReader, "ForwardRomanPotRecParticles.mass");
        TTreeReaderArray<float> rp_energy(treeReader, "ForwardRomanPotRecParticles.energy");
        TTreeReaderArray<float> rp_charge(treeReader, "ForwardRomanPotRecParticles.charge");
        
        // Reference points (hit locations)
        TTreeReaderArray<float> rp_ref_x(treeReader, "ForwardRomanPotRecParticles.referencePoint.x");
        TTreeReaderArray<float> rp_ref_y(treeReader, "ForwardRomanPotRecParticles.referencePoint.y");
        TTreeReaderArray<float> rp_ref_z(treeReader, "ForwardRomanPotRecParticles.referencePoint.z");
        
        // Reconstructed tracks
        TTreeReaderArray<float> reco_part_px(treeReader, "ReconstructedChargedParticles.momentum.x");
        TTreeReaderArray<float> reco_part_py(treeReader, "ReconstructedChargedParticles.momentum.y");
        TTreeReaderArray<float> reco_part_pz(treeReader, "ReconstructedChargedParticles.momentum.z");
        TTreeReaderArray<float> reco_part_energy(treeReader, "ReconstructedChargedParticles.energy");
        TTreeReaderArray<float> reco_part_mass(treeReader, "ReconstructedChargedParticles.mass");
        TTreeReaderArray<int>   reco_part_pdg(treeReader, "ReconstructedChargedParticles.PDG");
        TTreeReaderArray<float> reco_part_charge(treeReader, "ReconstructedChargedParticles.charge");
        
        TTreeReaderArray<float> reco_track_px(treeReader, "ReconstructedTruthSeededChargedParticles.momentum.x");
        TTreeReaderArray<float> reco_track_py(treeReader, "ReconstructedTruthSeededChargedParticles.momentum.y");
        TTreeReaderArray<float> reco_track_pz(treeReader, "ReconstructedTruthSeededChargedParticles.momentum.z");
        TTreeReaderArray<float> reco_track_energy(treeReader, "ReconstructedTruthSeededChargedParticles.energy");
        TTreeReaderArray<float> reco_track_mass(treeReader, "ReconstructedTruthSeededChargedParticles.mass");
        TTreeReaderArray<int>   reco_track_pdg(treeReader, "ReconstructedTruthSeededChargedParticles.PDG");
        TTreeReaderArray<float> reco_track_charge(treeReader, "ReconstructedTruthSeededChargedParticles.charge");
        
        // ECal Endcap North Clusters and Hits
        TTreeReaderArray<float> ecalN_energy(treeReader, "EcalEndcapNClusters.energy");
        TTreeReaderArray<float> ecalN_x(treeReader, "EcalEndcapNClusters.position.x");
        TTreeReaderArray<float> ecalN_y(treeReader, "EcalEndcapNClusters.position.y");
        TTreeReaderArray<float> ecalN_z(treeReader, "EcalEndcapNClusters.position.z");
        TTreeReaderArray<float> ecalN_t(treeReader, "EcalEndcapNClusters.time");
        TTreeReaderArray<float> ecalN_theta(treeReader, "EcalEndcapNClusters.intrinsicTheta");
        TTreeReaderArray<float> ecalN_phi(treeReader, "EcalEndcapNClusters.intrinsicPhi");
        
        // Inclusive kinematics
        TTreeReaderArray<float> ik_electron_x(treeReader, "InclusiveKinematicsElectron.x");
        TTreeReaderArray<float> ik_electron_Q2(treeReader, "InclusiveKinematicsElectron.Q2");
        TTreeReaderArray<float> ik_electron_y(treeReader, "InclusiveKinematicsElectron.y");
        
        TTreeReaderArray<float> ik_esigma_x(treeReader, "InclusiveKinematicsESigma.x");
        TTreeReaderArray<float> ik_esigma_Q2(treeReader, "InclusiveKinematicsESigma.Q2");
        TTreeReaderArray<float> ik_esigma_y(treeReader, "InclusiveKinematicsESigma.y");
        
        TTreeReaderArray<float> ik_jb_x(treeReader, "InclusiveKinematicsJB.x");
        TTreeReaderArray<float> ik_jb_Q2(treeReader, "InclusiveKinematicsJB.Q2");
        TTreeReaderArray<float> ik_jb_y(treeReader, "InclusiveKinematicsJB.y");
        
        TTreeReaderArray<float> ik_DA_x(treeReader, "InclusiveKinematicsDA.x");
        TTreeReaderArray<float> ik_DA_Q2(treeReader, "InclusiveKinematicsDA.Q2");
        TTreeReaderArray<float> ik_DA_y(treeReader, "InclusiveKinematicsDA.y");
        
        // Set the range of events to read
        treeReader.SetEntriesRange(0, numEvents);
        
        while (treeReader.Next()) {
            z0++;
            //inclusive info for electron, eSig and JB methods
            TLorentzVector iphoton(0,0,0,0), vec1(0,0,0,0), vec2(0,0,0,0), vec3ck_1(0,0,0,0), vec3ck_2(0,0,0,0), incbeam(0,0,0,0), incbeame(0,0,0,0), ijpsi(0,0,0,0), JPsichk(0,0,0,0), JPsi1_cor(0,0,0,0);
            TLorentzVector vecjpsi(0,0,0,0), scatteredElectron(0,0,0,0), decayElectron(0,0,0,0), JPsi1(0,0,0,0), vec3(0,0,0,0);
            bool has_incbeam=false, has_incbeame=false;
            bool has_vec1=false, has_vec2=false, has_vec3ck_1=false, has_vec3ck_2=false;
            bool has_scattered=false, has_decay=false;
            
            // safer indexing: track how many electrons and record their indices
            int n_electrons_final = 0;
            int num_find = 0;
            for (int imc = 0; imc < mc_px.GetSize(); imc++) {
                
                int pdg = mc_pdg[imc];
                int status = mc_gen_status[imc];
                double px = mc_px[imc];
                double py = mc_py[imc];
                double pz = mc_pz[imc];
                double m  = mc_mass[imc];
                int charge = mc_charge[imc];
                
                // --- Incident beams ---
                if (status == 4) {
                    if (pdg == 11 && charge == -1) {
                        incbeame.SetXYZM(px, py, pz, m);
                        has_incbeame = true;
                    } else if (pdg == 2212 && charge == 1) {
                        incbeam.SetXYZM(px, py, pz, m);
                        incbeam.RotateY(0.025);
                        if (incbeam.M() <= 0) continue;
                        has_incbeam = true;
                    }
                }
                
                // Final-state stable particles
                else if (status == 1) {
                    if (pdg == 11 && charge == -1) {
                        ++n_electrons_final;
                        if (!has_vec3ck_1) { vec3ck_1.SetXYZM(px, py, pz, m); has_vec3ck_1 = true; }
                        else if (!has_vec3ck_2) { vec3ck_2.SetXYZM(px, py, pz, m); has_vec3ck_2 = true; }
                    }
                    else if (pdg == -11 && charge == 1) {
                        vec2.SetXYZM(px, py, pz, m); has_vec2 = true;
                    }
                    else if (pdg == 2212 && charge == 1) {
                        vec1.SetXYZM(px, py, pz, m); vec1.RotateY(0.025); has_vec1 = true;
                    }
                    else if (pdg == 22 && charge == 0) {
                        iphoton.SetXYZM(px, py, pz, m);
                    }
                }
                // Intermediate J/psi
                else if (status == 2 && pdg == 443) {
                    ijpsi.SetXYZM(px, py, pz, m);
                }
            } // end mc loop
            
            if (incbeam.M() <= 0) continue;
            
            if (!has_incbeam || !has_incbeame || !has_vec2 || (!has_vec3ck_1 && !has_vec3ck_2) || !has_vec1) {
                // skip event or handle differently
                continue;
            }
            
            // identify decay electron and scattered electron robustly
            // compute both J/psi candidate masses if both electron candidates exist
            if (has_vec3ck_1 && has_vec3ck_2) {
                JPsichk = vec2 + vec3ck_1;
                JPsi1   = vec2 + vec3ck_2;
                
                bool mass1_inwindow = (JPsichk.M() > 2.7 && JPsichk.M() < 3.2);
                bool mass2_inwindow = (JPsi1.M()   > 2.7 && JPsi1.M()   < 3.2);
                
                if (mass1_inwindow && !mass2_inwindow) {
                    decayElectron = vec3ck_1; has_decay = true;
                    scatteredElectron = vec3ck_2; has_scattered = true;
                } else if (mass2_inwindow && !mass1_inwindow) {
                    decayElectron = vec3ck_2; has_decay = true;
                    scatteredElectron = vec3ck_1; has_scattered = true;
                } else if (mass1_inwindow && mass2_inwindow) {
                    // both make good J/psi: choose the one that gives mass closest to PDG mass (3.0969)
                    double dm1 = fabs(JPsichk.M() - 3.0969);
                    double dm2 = fabs(JPsi1.M()   - 3.0969);
                    if (dm1 < dm2) { decayElectron = vec3ck_1; has_decay = true; scatteredElectron = vec3ck_2; has_scattered = true; }
                    else { decayElectron = vec3ck_2; has_decay = true; scatteredElectron = vec3ck_1; has_scattered = true; }
                } else {
                    // none in window: assume the more forward (larger Pz) is decay, more backward is scattered
                    if (vec3ck_1.Pz() > vec3ck_2.Pz()) { decayElectron = vec3ck_1; has_decay = true; scatteredElectron = vec3ck_2; has_scattered = true; }
                    else { decayElectron = vec3ck_2; has_decay = true; scatteredElectron = vec3ck_1; has_scattered = true; }
                }
            } else if (has_vec3ck_1) {
                // only one electron found
                JPsichk = vec2 + vec3ck_1;
                if (JPsichk.M() > 2.9 && JPsichk.M() < 3.2) { decayElectron = vec3ck_1; has_decay = true; }
                else { scatteredElectron = vec3ck_1; has_scattered = true; }
            } else if (has_vec3ck_2) {
                JPsi1 = vec2 + vec3ck_2;
                if (JPsi1.M() > 2.9 && JPsi1.M() < 3.2) { decayElectron = vec3ck_2; has_decay = true; }
                else { scatteredElectron = vec3ck_2; has_scattered = true; }
            }
            
            // require scatteredElectron present before using it
            if (!has_scattered) continue;
            
            // check momentum nonzero
            if (scatteredElectron.P() <= 0) continue;
            
            // energy/momentum ratio check (use TMath::Abs)
            double e_over_p = scatteredElectron.E() / TMath::Abs(scatteredElectron.P());
            if (e_over_p < 0.9 || e_over_p > 1.1) continue;
            
            double denom_rap_mc = incbeam.Dot(incbeame);
            if (TMath::Abs(denom_rap_mc) < 1e-12) continue; // avoid div-by-zero
            rap = incbeam.Dot(incbeame - scatteredElectron) / denom_rap_mc;
            
            thetaMC = scatteredElectron.Theta();
            // if scattering defined wrt opposite direction, comment this:
            thetaMC = thetaMC - TMath::Pi();
            
            double sinHalf = TMath::Sin(thetaMC/2.0);
            q2 = 4.0 * incbeame.E() * scatteredElectron.E() * sinHalf * sinHalf;
            
            if (q2 < 1.0 || q2 > 10.0) continue;
            if (rap <= 0.1 || rap >= 1.0) continue;
            z3++;
            
            // corrected JPsi
            if (has_decay) {
                JPsi1_cor = vec2 + decayElectron;
                if (JPsi1_cor.M() < 2.9 || JPsi1_cor.M() > 3.2) continue;
                
                // Compute missing mass squared
                TLorentzVector total_in  = incbeame + incbeam;
                TLorentzVector total_out = scatteredElectron + vec1 + JPsi1_cor;
                TLorentzVector missing   = total_in - total_out;
                
                double Mx2 = missing.M2();  // missing mass squared
                double Mx  = missing.M();   // missing mass
                if (fabs(Mx) > 3) continue;
                h_missingMass2->Fill(Mx2);
                h_missingMass->Fill(Mx);
                
                mcj++;
                JPsiMass1->Fill(JPsi1_cor.M());
                
                // t3 for MC
                TLorentzVector diff3 = incbeam - vec1;
                t3 = -1.0 * diff3.Dot(diff3); // check sign convention
                t_di++;
                mtgg->Fill(t3);
                mtggtest->Fill(-1*t3);
            }
            else continue;
            
            mtq->Fill(q2);
            mcrapid->Fill(rap);
            
            // ---- fill proton and electron histos only if vectors exist ----
            if (has_vec1) {
                fProton->Fill(vec1.Eta());
                AllMCFinalEta->Fill(vec1.Eta());
                h6_eta_MC->Fill(vec1.Eta());
                h6_px_MC->Fill(vec1.Px());
                h6_py_MC->Fill(vec1.Py());
                h6_pt_MC->Fill(vec1.Pt());
                h6_pz_MC->Fill(vec1.Pz());
                h6_e_MC->Fill(vec1.E());
                Teta->Fill(vec1.Theta());
            }
            
            // scattered electron histos
            h2_e_MC->Fill(scatteredElectron.E());
            fElectron->Fill(scatteredElectron.Eta());
            h8_eta_MC->Fill(scatteredElectron.Eta());
            h8_px_MC->Fill(scatteredElectron.Px());
            h8_py_MC->Fill(scatteredElectron.Py());
            h8_pt_MC->Fill(scatteredElectron.Pt());
            h8_pz_MC->Fill(scatteredElectron.Pz());
            h8_e_MC->Fill(scatteredElectron.E());
            Teta_e->Fill(scatteredElectron.Theta());
            
            // jpsi decay electron histos
            if (has_decay) {
                hjpsi_eta_MC->Fill(decayElectron.Eta());
                hjpsi_px_MC->Fill(decayElectron.Px());
                hjpsi_py_MC->Fill(decayElectron.Py());
                hjpsi_pt_MC->Fill(decayElectron.Pt());
                hjpsi_pz_MC->Fill(decayElectron.Pz());
                hjpsi_e_MC->Fill(decayElectron.E());
            }
            
            // positron (vec2)
            if (has_vec2) {
                fPositron->Fill(vec2.Eta());
                h7_eta_MC->Fill(vec2.Eta());
                h7_px_MC->Fill(vec2.Px());
                h7_py_MC->Fill(vec2.Py());
                h7_pt_MC->Fill(vec2.Pt());
                h7_pz_MC->Fill(vec2.Pz());
                h7_e_MC->Fill(vec2.E());
            }
            
            e_pz_MC->Fill(scatteredElectron.E() / scatteredElectron.P());
            E_P_final_SCe_MC->Fill(scatteredElectron.E() / scatteredElectron.P());
            E_P_final_DCe_MC->Fill(decayElectron.E() / decayElectron.P());
            E_P_final_Posit_MC->Fill(vec2.E() / vec2.P());
            
            // compute ixb, ixv safely
            double denom2 = 2.0 * incbeam.Dot(incbeame - scatteredElectron);
            if (TMath::Abs(denom2) > 1e-12) {
                ixv = (q2 + JPsi1_cor.M() * JPsi1_cor.M()) / denom2;
                ixb = q2 / denom2;
                xb->Fill(ixb);
                xv->Fill(ixv);
            }
            
            // --------------- EEMC highest-energy cluster: find maximum properly ---------------
            double maxEnergy = -std::numeric_limits<double>::infinity();
            double ecal_theta = -999., ecal_phi = -999.;
            double best_x = 0, best_y = 0, best_z = 0;
            for (int iclus = 0; iclus < ecalN_energy.GetSize(); ++iclus) {
                double clusterEnergy = ecalN_energy[iclus];
                if (clusterEnergy > maxEnergy) {
                    maxEnergy = clusterEnergy;
                    best_x = ecalN_x[iclus];
                    best_y = ecalN_y[iclus];
                    best_z = ecalN_z[iclus];
                    ecal_phi = ecalN_phi[iclus];
                }
            }
            if (maxEnergy > -1e8) {
                double findtheta = TMath::ATan2(TMath::Sqrt(best_x*best_x + best_y*best_y), best_z);
                ecal_theta = findtheta;
                double eta = -TMath::Log(TMath::Tan(findtheta / 2.0));
                ecaleta->Fill(eta);
                scat_e_EcalEnd->Fill(maxEnergy);
                
                // compute q2_2 carefully
                double thetaShifted = ecal_theta - TMath::Pi(); // if that's intended
                double sinHalfTheta = TMath::Sin(thetaShifted / 2.0);
                q2_2 = 4.0 * incbeame.E() * maxEnergy * sinHalfTheta * sinHalfTheta;
                mtqq_2->Fill(q2_2);
            }
            
            //---------------inclusive data--------------------------------
            for (int iEMx = 0; iEMx < ik_electron_x.GetSize(); iEMx++){
                EM_x_value = ik_electron_x[iEMx];
                EM_Q2_value = ik_electron_Q2[iEMx];
                EM_y_value = ik_electron_y[iEMx];
                if (EM_x_value > 0) EM_x->Fill(EM_x_value);
                if (EM_Q2_value > 1 && EM_Q2_value < 10) EM_Q2->Fill(EM_Q2_value);
                if (EM_y_value > 0.1 && EM_y_value < 1) EM_y->Fill(EM_y_value);
            }
            for (int iES = 0; iES < ik_esigma_x.GetSize(); iES++){
                ESig_x_in_value = ik_esigma_x[iES];
                ESig_Q2_in_value = ik_esigma_Q2[iES];
                ESig_y_in_value = ik_esigma_y[iES];
                if (ESig_x_in_value > 0) ESig_x_in->Fill(ESig_x_in_value);
                if (ESig_Q2_in_value > 1 && ESig_Q2_in_value < 10) ESig_Q2_in->Fill(ESig_Q2_in_value);
                if (ESig_y_in_value > 0.1 && ESig_y_in_value < 1) ESig_y_in->Fill(ESig_y_in_value);
            }
            for (int iJB = 0; iJB < ik_jb_x.GetSize(); iJB++){
                JB_x_in_value = ik_jb_x[iJB];
                JB_Q2_in_value = ik_jb_Q2[iJB];
                JB_y_in_value = ik_jb_y[iJB];
                if (JB_x_in_value > 0) JB_x_in->Fill(JB_x_in_value);
                if (JB_Q2_in_value > 1 && JB_Q2_in_value < 10) JB_Q2_in->Fill(JB_Q2_in_value);
                if (JB_y_in_value > 0.1 && JB_y_in_value < 1) JB_y_in->Fill(JB_y_in_value);
            }
            for (int iDA = 0; iDA < ik_DA_x.GetSize(); iDA++){
                DA_x_in_value = ik_DA_x[iDA];
                DA_Q2_in_value = ik_DA_Q2[iDA];
                DA_y_in_value = ik_DA_y[iDA];
                if (DA_x_in_value > 0) DA_x_in->Fill(DA_x_in_value);
                if (DA_Q2_in_value > 1 && DA_Q2_in_value < 10) DA_Q2_in->Fill(DA_Q2_in_value);
                if (DA_y_in_value > 0.1 && DA_y_in_value < 1) DA_y_in->Fill(DA_y_in_value);
            }
            //--------------------------------------------------------------
            
            // JB / eSigma: compute combined vector once
            TLorentzVector sumJ = vec1 + (has_decay ? decayElectron : TLorentzVector(0,0,0,0)) + vec2;
            jbpt2 = sumJ.Px()*sumJ.Px() + sumJ.Py()*sumJ.Py();
            JB_pt2_MC->Fill(sqrt(jbpt2));
            jbsig = sumJ.E() - sumJ.Pz();
            JB_HFS_MC->Fill(jbsig);
            y_JB = jbsig / (2.0 * incbeame.E());
            if (y_JB > 0.1 && y_JB < 1) JB_y_MC->Fill(y_JB);
            q2_JB = (y_JB < 0.999999) ? jbpt2 / (1.0 - y_JB) : -1;
            if (q2_JB > 1 && q2_JB < 10) JB_q2_MC->Fill(q2_JB);
            if (y_JB > 1e-12) {
                x_JB = q2_JB / (4.0 * incbeame.E() * incbeam.E() * y_JB);
                JB_x_MC->Fill(x_JB);
            }
            
            // eSigma method (keep your formulas but guard divisions)
            Sig_e = scatteredElectron.E() - scatteredElectron.Pz();
            y_e = 1.0 - (Sig_e / (2.0 * incbeame.E()));
            Sig_tot = Sig_e + jbsig;
            y_Sig = (TMath::Abs(Sig_tot) > 1e-12) ? jbsig / Sig_tot : -1;
            q2_Sig = (TMath::Abs(1.0 - y_Sig) > 1e-12) ? (scatteredElectron.Pt()*scatteredElectron.Pt()) / (1.0 - y_Sig) : -1;
            q2_e   = (TMath::Abs(1.0 - y_e)   > 1e-12) ? (scatteredElectron.Pt()*scatteredElectron.Pt()) / (1.0 - y_e)   : -1;
            x_Sig  = (TMath::Abs(4.0*incbeame.E()*incbeam.E()*y_Sig) > 1e-12) ? q2_Sig / (4.0*incbeame.E()*incbeam.E()*y_Sig) : -1;
            y_eSig = (TMath::Abs(4.0*incbeame.E()*incbeam.E()*x_Sig) > 1e-12) ? q2_e / (4.0*incbeame.E()*incbeam.E()*x_Sig) : -1;
            
            if (y_eSig > 0.1 && y_eSig < 1) eSig_y_MC->Fill(y_eSig);
            if (q2_e > 1 && q2_e < 10) eSig_q2_MC->Fill(q2_e);
            if (x_Sig > 0) eSig_x_MC->Fill(x_Sig);
            eS_HFS_MC->Fill(Sig_tot);
            
            //computing the DA metho kinematics TMath::Sin(thetaShifted / 2.0)
            double hadronHalfAngle = jbsig / sqrt(jbpt2) ;
            double y_DA = hadronHalfAngle / (TMath::Tan(scatteredElectron.Theta()/2.0) + hadronHalfAngle);
            if (y_DA > 0.1 && y_DA < 1) DA_y_MC->Fill(y_DA);
            double q2_DA = 4.0*incbeame.E()*incbeame.E() / ((TMath::Tan(scatteredElectron.Theta()/2.0))*(TMath::Tan(scatteredElectron.Theta()/2.0)) + hadronHalfAngle);
            if (q2_DA > 1 && q2_DA < 10) DA_q2_MC->Fill(q2_DA);
            double x_DA = q2_DA / (4.0*incbeame.E()*incbeam.E()*y_DA);
            if (x_DA > 0) DA_x_MC->Fill(x_DA);
            
            // t and -t fill (keep your T->GetEntry logic if necessary)
            T->GetEntry(z6);
            t_lAger->Fill(-t);
            z6++;
            
            TLorentzVector diff_forRP_acc(0,0,0,0);
            TLorentzVector diff_forB0_acc(0,0,0,0);
            
            //another method for t distribution
            TLorentzVector pr_beam_scattered_corr(0,0,0,0);
            TLorentzVector pr_beam_scattered = incbeam - (JPsi1_cor+scatteredElectron-incbeame);
            double p_Aplus = pr_beam_scattered.E()+pr_beam_scattered.Pz();
            double p_TAsquared = (pr_beam_scattered.Pt())*(pr_beam_scattered.Pt());
            double p_Aminus = (incbeam.M()*incbeam.M() + p_TAsquared) / p_Aplus;
            pr_beam_scattered_corr.SetPxPyPzE(pr_beam_scattered.Px(),pr_beam_scattered.Py(),(p_Aplus-p_Aminus)/2, (p_Aplus+p_Aminus)/2);
            
            t_another = -1*(incbeam - pr_beam_scattered_corr).Dot(incbeam - pr_beam_scattered_corr);
            t_another_neg = (incbeam - pr_beam_scattered_corr).Dot(incbeam - pr_beam_scattered_corr);
            t_anotherhist_neg->Fill(t_another_neg);
            t_anotherhist->Fill(t_another);
            //cout << "t_another: " << t_another << " t3: " << t3 << endl;
            
            q2vst->Fill(t_another, q2);
            q2_v_x_e->Fill(ixb, q2);
            
            // Check if scatteredElectron is more energetic than decayElectron
            if (scatteredElectron.E() > decayElectron.E()) {
                
                // Build the full hadronic system
                TLorentzVector hadronicFinalState = decayElectron + vec1 + vec2 + scatteredElectron;
                
                // Compute E - Pz for the system
                double eMinusPz = hadronicFinalState.E() - hadronicFinalState.Pz();
                
                // Fill histogram if in desired range
                if (eMinusPz > 15 && eMinusPz < 25) mcjpsie_e_minus_pzmc->Fill(eMinusPz); //change for the energies
            }
            
            //------reconstruction of proton in the roman pots--------
            // Loop over reconstructed Roman Pot tracks
            TLorentzVector vec5(0,0,0,0), accept1(0,0,0,0), diff_rp(0,0,0,0);
            iflagproton = 0;
            for (int iRPPart = 0; iRPPart < rp_px.GetSize(); ++iRPPart) {
                
                // Construct 4-momentum for the current RP track
                TLorentzVector p_recoRP(rp_px[iRPPart], rp_py[iRPPart], rp_pz[iRPPart], rp_energy[iRPPart]);
                
                // Apply basic selection: positive proton with sufficient pT
                bool isProton = (rp_charge[iRPPart] == 1) && (rp_pdg[iRPPart] == 2212);
                bool passPtCut = (p_recoRP.Pt() > 0.2);
                
                if (isProton && passPtCut) {
                    
                    // Set 4-vector with mass from the collection
                    vec5.SetXYZM(rp_px[iRPPart], rp_py[iRPPart], rp_pz[iRPPart], rp_mass[iRPPart]);
                    //if (vec5.M() <= 0) continue;
                    
                    RPfProton->Fill(vec5.Eta());
                    B0RPfProton->Fill(vec5.Eta()); //reserved for combined subsystems use
                    h_pt_B0RomanPots->Fill(vec5.Perp());
                    
                    h_px_RomanPots->Fill(vec5.Px());
                    h_py_RomanPots->Fill(vec5.Py());
                    h_pt_RomanPots->Fill(vec5.Pt());
                    h_pz_RomanPots->Fill(vec5.Pz());
                    h_e_RomanPots->Fill(vec5.E());
                    
                    //testing
                    TetaRP->Fill(vec5.Theta());
                    TetaAC->Fill(vec1.Theta());
                    TetaRPB0->Fill(vec5.Theta());//for combined subsystems use
                    theta2D->Fill(vec1.Theta(), vec5.Theta());
                    
                    // Hit map (for spatial occupancy studies)
                    h_rp_occupancy_map->Fill(rp_ref_x[iRPPart], rp_ref_y[iRPPart]);
                    diff_forRP_acc = incbeam - vec1;
                    
                    z1++;
                    iflagproton = 1;
                    
                }
                else continue;
            }
            
            
            //------reconstructed tracks with ACTS -- used for B0---------
            // Loop over reconstructed B0 tracker tracks
            TLorentzVector vec4;
            iflagproton2 = 0;
            for (int iRecoTrk = 0; iRecoTrk < reco_track_px.GetSize(); ++iRecoTrk) {
                // Create 4-vector for the reconstructed track
                TLorentzVector p_recoB0(reco_track_px[iRecoTrk], reco_track_py[iRecoTrk], reco_track_pz[iRecoTrk], reco_track_energy[iRecoTrk]);
                
                // Basic selection for neutral, forward-going B0 proton candidate
                bool isProton = (reco_track_pdg[iRecoTrk] == 0);
                bool isForward = (p_recoB0.Eta() > 4.0);
                bool isCharged = (reco_track_charge[iRecoTrk] == 1);
                if (isProton && isForward && isCharged) {
                    
                    // Set mass-corrected 4-vector and rotate to remove crossing angle
                    vec4.SetXYZM(reco_track_px[iRecoTrk], reco_track_py[iRecoTrk], reco_track_pz[iRecoTrk], reco_track_mass[iRecoTrk]);
                    vec4.RotateY(0.025); // Remove crossing angle for B0
                    //if (vec4.M() <= 0) continue;
                    
                    // Fill histograms
                    B0fProton->Fill(vec4.Eta());
                    B0RPfProton->Fill(vec4.Eta()); //for combined subsystems use
                    
                    h_px_reco_track->Fill(vec4.Px());
                    h_py_reco_track->Fill(vec4.Py());
                    h_pt_reco_track->Fill(vec4.Pt());
                    h_pt_B0RomanPots->Fill(vec4.Pt());
                    h_pz_reco_track->Fill(vec4.Pz());
                    h_e_reco_track->Fill(vec4.E());
                    
                    //testing
                    TetaB0->Fill(vec4.Theta());
                    TetaAC->Fill(vec1.Theta());
                    TetaRPB0->Fill(vec4.Theta());
                    theta2D->Fill(vec1.Theta(), vec4.Theta());
                    diff_forB0_acc = incbeam - vec1;
                    
                    z2++;
                    iflagproton2 = 1;
                    
                }
                else continue;
            }
            
            //RECO electron selection
            TLorentzVector JPsi_cand1(0,0,0,0), JPsi_cand2(0,0,0,0);
            TLorentzVector JPsi22(0,0,0,0), JPsi2_cor(0,0,0,0);
            TLorentzVector scatteredElectron_rc(0,0,0,0), decayElectron_rc(0,0,0,0), positron_rc(0,0,0,0);
            TLorentzVector eq1rc(0,0,0,0), rcpv1rc(0,0,0,0), eq1ck(0,0,0,0), rcpv1ck(0,0,0,0);
            TLorentzVector eq1(0,0,0,0), vgen(0,0,0,0), vrecop(0,0,0,0);
            TLorentzVector ixva(0,0,0,0), JPsia(0,0,0,0), jpsicheck3(0,0,0,0);
            TLorentzVector neg1(0,0,0,0), neg2(0,0,0,0), pos(0,0,0,0), jpsicheck1(0,0,0,0), jpsicheck2(0,0,0,0);
            double q2_acc = 0, rap_acc = 0, y_JB_acc = 0, y_eSig_acc = 0, q2_e_acc = 0, q2_JB_acc = 0;
            
            if (reco_part_px.GetSize() != 3) continue;
            if (reco_part_px.GetSize() != 3) continue;
            
            // Identify indices by charge
            std::vector<int> ele_indices;
            std::vector<int> pos_indices;
            for (int i = 0; i < reco_part_px.GetSize(); ++i) {
                int charge = reco_part_charge[i];
                if (charge == -1) ele_indices.push_back(i);
                if (charge == 1)  pos_indices.push_back(i);
            }
            
            // Require e⁺e⁻e⁻
            if (ele_indices.size() < 2 || pos_indices.size() < 1) continue;
            
            // Assign based on pz ordering (consistent with MC)
            neg1.SetXYZM(reco_part_px[ele_indices[0]], reco_part_py[ele_indices[0]], reco_part_pz[ele_indices[0]], reco_part_mass[ele_indices[0]]);
            neg2.SetXYZM(reco_part_px[ele_indices[1]], reco_part_py[ele_indices[1]], reco_part_pz[ele_indices[1]], reco_part_mass[ele_indices[1]]);
            
            // Build positron
            pos.SetXYZM(reco_part_px[pos_indices[0]], reco_part_py[pos_indices[0]], reco_part_pz[pos_indices[0]], reco_part_mass[pos_indices[0]]);
            positron_rc = pos;
            
            // Build possible J/psi candidates
            JPsi_cand1 = positron_rc + neg1;
            JPsi_cand2 = positron_rc + neg2;
            
            const double M_JPSI = 3.0969;
            const double MASS_WINDOW = 0.7; // ±0.35 GeV
            
            // Compute which combination is closer to J/psi mass
            double dM1 = fabs(JPsi_cand1.M() - M_JPSI);
            double dM2 = fabs(JPsi_cand2.M() - M_JPSI);
            
            // Select decay and scattered electrons
            if ( (JPsi_cand1.M() > 2.9 && JPsi_cand1.M() < 3.2) && abs(neg2.Pz()) > abs(neg1.Pz())  ) {
                decayElectron_rc = neg1;
                scatteredElectron_rc = neg2;
            }
            else if ( (JPsi_cand2.M() > 2.9 && JPsi_cand2.M() < 3.2) && abs(neg2.Pz()) < abs(neg1.Pz()) ) {
                decayElectron_rc = neg2;
                scatteredElectron_rc = neg1;
            }
            else continue;
            
            // E/P cut for scattered electron
            double ep_ratio = scatteredElectron_rc.E() / fabs(scatteredElectron_rc.P());
            if (ep_ratio < 0.9 || ep_ratio > 1.1) continue;
            if (scatteredElectron_rc.P() <= 0 || decayElectron_rc.P() <= 0) continue;//sanity check
            
            raprc = (incbeam.Dot(incbeame - scatteredElectron_rc)) / (incbeam.Dot(incbeame));
            thetaChPct = scatteredElectron_rc.Theta() - TMath::Pi();
            q2_1 = 4 * incbeame.E() * scatteredElectron_rc.E() * TMath::Power(TMath::Sin(thetaChPct / 2.0), 2);
            if ( !(scatteredElectron_rc.P() > 0)) continue;
            if (q2_1 < 1 || q2_1 > 10) continue;
            if (raprc <= 0.1 || raprc >= 1.0) continue;
            
            mtqq->Fill(q2_1);
            mcrapidrc->Fill(raprc);
            z4++;
            
            E_P_final_SCe_RC->Fill(scatteredElectron_rc.E() / scatteredElectron_rc.P());
            E_P_final_DCe_RC->Fill(decayElectron_rc.E() / decayElectron_rc.P());
            E_P_final_Posit_RC->Fill(positron_rc.E() / positron_rc.P());
            
            //------------acceptance----------------------------------------
            q2_acc = q2;
            q2_e_acc = q2_e;
            q2_JB_acc = q2_JB;
            rap_acc = rap;
            y_JB_acc = y_JB;
            y_eSig_acc = y_eSig;
            
            ixba = q2 / (2.0 * incbeam.Dot(incbeame - scatteredElectron_rc));
            
            JB_x_acc->Fill(x_JB);
            eSig_x_acc->Fill(x_Sig);
            DA_x_acc->Fill(x_DA);
            
            mtq_acc->Fill(q2_acc);
            mcrapida->Fill(rap_acc);
            if (y_JB_acc > 0.1 && y_JB_acc < 1) JB_y_MC_a->Fill(y_JB_acc);
            if (y_DA > 0.1 && y_DA < 1) DA_y_MC_a->Fill(y_DA);
            if (y_eSig_acc > 0.1 && y_eSig_acc < 1) eSig_y_MC_a->Fill(y_eSig_acc);
            if (q2_JB_acc > 1 && q2_JB_acc < 10) JB_q2_acc->Fill(q2_JB_acc);
            if (q2_DA > 1 && q2_DA < 10) DA_q2_acc->Fill(q2_DA);
            if (q2_e_acc > 1 && q2_JB_acc < 10) eSig_q2_acc->Fill(q2_e_acc);
            
            xba->Fill(ixba);
            //--------------------------------------------------------------
            
            JPsi22 = positron_rc + scatteredElectron_rc;
            rcJPsiMass111->Fill(JPsi22.M());
            JPsi2_cor = positron_rc + decayElectron_rc;
            if (JPsi2_cor.M() > 2.9 && JPsi2_cor.M() < 3.2) {
                
                //-------------RP t------------------------------
                diff_rp = incbeam - vec5;
                if (iflagproton == 1 ) {
                    t_accept1 = -1*diff_forRP_acc.Dot(diff_forRP_acc);//for MC;// for acceptance
                    mtgg_accept->Fill(t_accept1);
                    if (t_accept1 > 0) two_meth_tdist_accept->Fill(t_accept1);
                    
                    t1 = - diff_rp.Mag2();
                    mtg->Fill(t1);
                    
                    
                    mtgTot->Fill(t1);//for combined subsystems use
                    mtggtest_rc->Fill(-1*t1);
                    mtgTot1->Fill(t1); //for separate use, later
                    
                    // --- 2D Plot: true vs. RP-based t ---
                    t_2D->Fill(t3, t1);
                    two_meth_tdist->Fill(t1);
                    t2D_with_diff_rp->Fill(t3, t1);
                    t2D_with_diff_allCroll->Fill(t3, t1);
                }
                //------------------------------------------------
                //--------------------B0-only |t|-----------------
                // Acceptance-corrected t from MC
                TLorentzVector diff_b0 = incbeam - vec4;
                if (iflagproton2 == 1 ) {
                    t_accept2 = -1*diff_forB0_acc.Dot(diff_forB0_acc);//for MC;
                    mtgg_accept->Fill(t_accept2);
                    if (t_accept2 > 0) two_meth_tdist_accept->Fill(t_accept2);
                    t2 = - diff_b0.Mag2();
                    mtgB0->Fill(t2);
                    
                    mtgTot->Fill(t2);
                    mtggtest_rc->Fill(-1*t2);
                    mtgTot1->Fill(t2);
                    
                    // 2D correlation plot
                    t_2D->Fill(t3, t2);
                    
                    two_meth_tdist->Fill(t2);
                    t2D_with_diff_B0->Fill(t3, t2);
                    t2D_with_diff_allCroll->Fill(t3, t2);
                }
                //-------------------------------------------------------------
                
                rcJPsiMassa->Fill(JPsi1_cor.M());
                
                t_another_acc = t_another;
                t_anotherhist_acc->Fill(t_another_acc);
                
                TLorentzVector pr_beam_scattered_corr_rc;
                TLorentzVector pr_beam_scattered_rc = incbeam - (JPsi2_cor+scatteredElectron_rc-incbeame);
                double p_Aplus_rc = pr_beam_scattered_rc.E()+pr_beam_scattered_rc.Pz();
                double p_TAsquared_rc = (pr_beam_scattered_rc.Pt())*(pr_beam_scattered_rc.Pt());
                double p_Aminus_rc = (incbeam.M()*incbeam.M() + p_TAsquared_rc) / p_Aplus_rc;
                pr_beam_scattered_corr_rc.SetPxPyPzE(pr_beam_scattered_rc.Px(),pr_beam_scattered_rc.Py(),(p_Aplus_rc-p_Aminus_rc)/2, (p_Aplus_rc+p_Aminus_rc)/2);
                
                t_another_rc = -(incbeam - pr_beam_scattered_corr_rc).Dot(incbeam - pr_beam_scattered_corr_rc);
                t_anotherhist_rc->Fill(t_another_rc);
                t_another_2D->Fill(t_another, t_another_rc);
                
                rcJPsiMass1->Fill(JPsi2_cor.M());
                
                if (iflagproton == 1 || iflagproton2 == 1 ) {
                    TLorentzVector reco_in  = incbeame + incbeam;
                    TLorentzVector reco_out;
                    if (iflagproton == 1 ) reco_out = scatteredElectron_rc + vec5 + JPsi2_cor;
                    if (iflagproton2 == 1) reco_out = scatteredElectron_rc + vec4 + JPsi2_cor;
                    TLorentzVector missing_rc   = reco_in - reco_out;
                    
                    double Mx2_rc = missing_rc.M2();  // missing mass squared
                    double Mx_rc  = missing_rc.M();   // missing mass
                    if (fabs(Mx_rc) > 3) continue;
                    h_missingMass2_rc->Fill(Mx2_rc);
                    h_missingMass_rc->Fill(Mx_rc);
                    
                    diffjpsimass->Fill((JPsi1_cor.M() - JPsi2_cor.M()) / JPsi2_cor.M());
                }
            }
            
            rcJPsiMass11->Fill(JPsi2_cor.M());
            rcj++;
            
            q2_2D->Fill(q2, q2_1); // 2D Q² comparison
            
            //theta
            Teta_e_acc->Fill(scatteredElectron.Theta());
            Teta_e_rc->Fill(scatteredElectron_rc.Theta());
            theta2D_e->Fill(scatteredElectron.Theta(), scatteredElectron_rc.Theta());
            
            // Compute xV from 4-momenta (acceptance)
            ixv2 = (q2 + JPsi1_cor.M() * JPsi1_cor.M()) / (2.0 * incbeam.Dot(incbeame - scatteredElectron));
            xv2->Fill(ixv2);
            
            e_pz_acc->Fill(abs(scatteredElectron.E()/scatteredElectron.Pz()));
            e_pz_rc->Fill(abs(scatteredElectron_rc.E()/scatteredElectron_rc.Pz()));
            e_pz_2D->Fill(abs(scatteredElectron_rc.E()/scatteredElectron_rc.Pz()),abs(scatteredElectron.E()/scatteredElectron.Pz()));
            if (scatteredElectron_rc.E() > 0 && scatteredElectron_rc.E() < 10 ) scat_e_RC->Fill(scatteredElectron_rc.E());
            if (scatteredElectron.E() > 0 && scatteredElectron.E() < 10) h2_e_acc->Fill(scatteredElectron.E()); //make sure to change it for other energies
            
            // Fill reconstructed electron and positron kinematics
            rcElectron->Fill(scatteredElectron_rc.Eta());
            rcElectronpt->Fill(scatteredElectron_rc.Pt());
            rcPositron->Fill(positron_rc.Eta());
            rcPositronpt->Fill(positron_rc.Pt());
            
            // Fill J/ψ daughter electron kinematics
            rcElectronjpsieta->Fill(decayElectron_rc.Eta());
            rcElectronjpsipt->Fill(decayElectron_rc.Pt());
            
            // 2D η distributions
            eEta2D->Fill(decayElectron.Eta(), vec2.Eta());// where vec2 is the MC positron
            pEta2D->Fill(decayElectron_rc.Eta(), positron_rc.Eta());
            
            // Calculate and fill Ecal/pz
            tot_p = sqrt(scatteredElectron_rc.Pt()*scatteredElectron_rc.Pt() + scatteredElectron_rc.Pz()*scatteredElectron_rc.Pz());
            Ecal_vs_pz->Fill(maxEnergy / tot_p);
            
            // Reconstructed kinematics (x_bj and x_v)
            double denom = 2.0 * incbeam.Dot(incbeame - scatteredElectron_rc);
            ixb1 = q2_1 / denom;
            ixv1 = (q2_1 + JPsi2_cor.M() * JPsi2_cor.M()) / denom;
            
            xb1->Fill(ixb1);
            xv1->Fill(ixv1);
            
            // Kinematic difference histograms
            diffq2->Fill((q2 - q2_1) / q2);
            diffx_v->Fill((ixb - ixb1) / ixb);
            diffxbjk->Fill((ixb - ixb1) / ixb);
            diffy->Fill((rap - raprc) / rap);
            
            // 2D correlation plots
            xbjk_2D->Fill(ixb, ixb1);
            y_2D->Fill(rap, raprc);
            dxbjk_2D->Fill(ixb, (ixb - ixb1)/ixb);
            dybjk_2D->Fill(rap, (rap - raprc)/rap);
            dq2bjk_2D->Fill(q2, (q2 - q2_1)/q2);
            
            q2_v_x_e_rc->Fill(ixb1, q2_1);
            energy_2D->Fill(scatteredElectron.E(), scatteredElectron_rc.E());
            
            h2_migration->Fill(q2, q2_1);
            
            //--------------t_reco------------------------------------
            q2vst_acc->Fill(t_another, q2);
            
            if (iflagproton != 1 && iflagproton2 != 1) {
                two_meth_tdist->Fill(t_another_rc);
                if (t_another_acc > 0) two_meth_tdist_accept->Fill(t_another_acc);
                t2D_with_diff_methL->Fill(t3, t_another_rc);
                t2D_with_diff_allCroll->Fill(t3, t_another_rc);
            }
            //---------------------------------------------------------
            
            // --- Reconstructed J/ψ combinations ---
            if (iflagproton == 1 || iflagproton2 == 1) {
                TLorentzVector jb_vec;
                if (iflagproton == 1) jb_vec = vec5 + decayElectron_rc + positron_rc;
                else jb_vec = vec4 + decayElectron_rc + positron_rc;
                
                
                jbpt2_rc = jb_vec.Px()*jb_vec.Px() + jb_vec.Py()*jb_vec.Py();
                jbsig_rc = jb_vec.E() - jb_vec.Pz();
                y_JB_rc = jbsig_rc / (2 * incbeame.E());
                q2_JB_rc = jbpt2_rc / (1 - y_JB_rc);
                x_JB_rc = q2_JB_rc / (4 * incbeame.E() * incbeam.E() * y_JB_rc);
                
                // Fill 1D histograms
                if (y_JB_rc > 0.1 && y_JB_rc < 1) JB_y_RC->Fill(y_JB_rc);
                if (q2_JB_rc > 1 && q2_JB_rc < 10) JB_q2_RC->Fill(q2_JB_rc);
                JB_x_RC->Fill(x_JB_rc);
                
                // Fill differences
                diff_JB_q2->Fill((q2_JB - q2_JB_rc) / q2_JB);
                diff_JB_y->Fill((y_JB - y_JB_rc) / y_JB);
                diff_JB_xbjk->Fill((x_JB - x_JB_rc) / x_JB);
                
                // Fill 2D histograms
                dJB_q2_2D->Fill(q2_JB, (q2_JB - q2_JB_rc) / q2_JB);
                JB_y_2D->Fill(y_JB, y_JB_rc);
                dJB_y_2D->Fill(y_JB, (y_JB - y_JB_rc) / y_JB);
                JB_xbjk_2D->Fill(x_JB, x_JB_rc);
                dJB_xbjk_2D->Fill(x_JB, (x_JB - x_JB_rc) / x_JB);
                JB_q2_2D->Fill(q2_JB, q2_JB_rc);
                
                double hadronHalfAngle_rc = jbsig_rc / sqrt(jbpt2_rc) ;
                double y_DA_rc = hadronHalfAngle_rc / (TMath::Tan(scatteredElectron_rc.Theta()/2.0) + hadronHalfAngle_rc);
                if (y_DA_rc > 0.1 && y_DA_rc < 1) DA_y_RC->Fill(y_DA_rc);
                
                double q2_DA_rc = 4.0*incbeame.E()*incbeame.E() / ((TMath::Tan(scatteredElectron_rc.Theta()/2.0))*(TMath::Tan(scatteredElectron_rc.Theta()/2.0)) + hadronHalfAngle_rc);
                if (q2_DA_rc > 1 && q2_DA_rc < 10) DA_q2_RC->Fill(q2_DA_rc);
                
                double x_DA_rc = q2_DA_rc / (4.0*incbeame.E()*incbeam.E()*y_DA_rc);
                if (x_DA_rc > 0) DA_x_RC->Fill(x_DA_rc);
                
                //DA 2D plots
                DA_x_2D->Fill(x_DA, x_DA_rc);
                DA_q2_2D->Fill(q2_DA, q2_DA_rc);
                DA_y_2D->Fill(y_DA, y_DA_rc);
                
                diff_DA_q2->Fill((q2_DA - q2_DA_rc) / q2_DA);
                diff_DA_y->Fill((y_DA - y_DA_rc) / y_DA);
                diff_DA_x->Fill((x_DA - x_DA_rc) / x_DA);
            }
            
            
            //HFS
            JB_HFS_acc->Fill(jbsig);
            if (iflagproton2 == 1) {
                double hfsjb2 = (vec4 + decayElectron_rc + positron_rc).E() - (vec4 + decayElectron_rc + positron_rc).Pz();
                JB_HFS_rc->Fill(hfsjb2);
                JB_pt2_rc->Fill(sqrt((vec4 + decayElectron_rc + positron_rc).Px()*(vec4 + decayElectron_rc + positron_rc).Px() + (vec4 + decayElectron_rc + positron_rc).Py()*(vec4 + decayElectron_rc + positron_rc).Py()));
                JB_HFS_2D->Fill(jbsig,hfsjb2);
                
                auto totalVec = decayElectron_rc + positron_rc + scatteredElectron_rc + vec4;
                double eMinusPz = totalVec.E() - totalVec.Pz();
                if (eMinusPz > 15 && eMinusPz < 25) {
                    rcjpsie_e_minus_pz->Fill(eMinusPz);
                }
                
            }
            JB_pt2_acc->Fill(sqrt(jbpt2));
            if (iflagproton == 1) {
                double hfsjb = (vec5 + decayElectron_rc + positron_rc).E() - (vec5 + decayElectron_rc + positron_rc).Pz();
                JB_HFS_rc->Fill(hfsjb);
                JB_pt2_rc->Fill(sqrt((vec5 + decayElectron_rc + positron_rc).Px()*(vec5 + decayElectron_rc + positron_rc).Px() + (vec5 + decayElectron_rc + positron_rc).Py()*(vec5 + decayElectron_rc + positron_rc).Py()));
                JB_HFS_2D->Fill(jbsig,hfsjb);
                
                auto totalVec = decayElectron_rc + positron_rc + scatteredElectron_rc + vec5;
                double eMinusPz = totalVec.E() - totalVec.Pz();
                if (eMinusPz > 15 && eMinusPz < 25) {
                    rcjpsie_e_minus_pz->Fill(eMinusPz);
                }
                
            }
            
            // --- eSigma Method -----------------------------
            if (iflagproton == 1 || iflagproton2 == 1) {
                Sig_e_rc = scatteredElectron_rc.E() - scatteredElectron_rc.Pz();
                y_e_rc = 1 - (Sig_e_rc / (2 * incbeame.E()));
                Sig_tot_rc = Sig_e_rc + jbsig_rc;
                Sig_tot_rc = Sig_e_rc + jbsig_rc;
                y_Sig_rc = jbsig_rc / Sig_tot_rc;
                
                q2_Sig_rc = (scatteredElectron_rc.Pt()*scatteredElectron_rc.Pt()) / (1 - y_Sig_rc);
                q2_e_rc = (scatteredElectron_rc.Pt()*scatteredElectron_rc.Pt()) / (1 - y_e_rc);
                x_Sig_rc = q2_Sig_rc / (4 * incbeame.E() * incbeam.E() * y_Sig_rc);
                y_eSig_rc = q2_e_rc / (4 * incbeame.E() * incbeam.E() * x_Sig_rc);
                
                // Fill 1D histograms
                if (y_eSig_rc > 0.1 && y_eSig_rc < 1) eSig_y_RC->Fill(y_eSig_rc);
                if (q2_e_rc > 1 && q2_e_rc < 10)  eSig_q2_RC->Fill(q2_e_rc);
                eSig_x_RC->Fill(x_Sig_rc);
                
                eS_HFS_acc->Fill(Sig_tot);
                eS_HFS_rc->Fill(Sig_tot_rc);
                
                // Fill differences
                diff_eSig_q2->Fill((q2_e - q2_e_rc) / q2_e);
                diff_eSig_y->Fill((y_eSig - y_eSig_rc) / y_eSig);
                diff_eSig_xbjk->Fill((x_Sig - x_Sig_rc) / x_Sig);
                
                // Fill 2D histograms
                //eSig_q2_2D->Fill(q2_e, q2_e_rc);
                deSig_q2_2D->Fill(q2_e, (q2_e - q2_e_rc) / q2_e);
                eSig_y_2D->Fill(y_eSig, y_eSig_rc);
                deSig_y_2D->Fill(y_eSig, (y_eSig - y_eSig_rc) / y_eSig);
                eSig_xbjk_2D->Fill(x_Sig, x_Sig_rc);
                deSig_xbjk_2D->Fill(x_Sig, (x_Sig - x_Sig_rc) / x_Sig);
                eSig_q2_2D->Fill(q2_e, q2_e_rc);
            }
            
            //Compute Purity and Stability
            for (int i = 1; i <= nbins; ++i) {
                double matched = h2_migration->GetBinContent(i, i);  // Diagonal
                double matched_JB = JB_q2_2D->GetBinContent(i, i);  // Diagonal
                double matched_eS = eSig_q2_2D->GetBinContent(i, i);  // Diagonal
                
                double reco_total = h2_migration->Integral(1, nbins, i, i); // Column: fixed reco bin
                double reco_total_JB = JB_q2_2D->Integral(1, nbins, i, i); // Column: fixed reco bin
                double reco_total_eS = eSig_q2_2D->Integral(1, nbins, i, i); // Column: fixed reco bin
                
                double truth_total = h2_migration->Integral(i, i, 1, nbins); // Row: fixed truth bin
                double truth_total_JB = JB_q2_2D->Integral(i, i, 1, nbins); // Row: fixed truth bin
                double truth_total_eS = eSig_q2_2D->Integral(i, i, 1, nbins); // Row: fixed truth bin
                
                double purity = (reco_total > 0) ? matched / reco_total : 0;
                double purity_JB = (reco_total_JB > 0) ? matched_JB / reco_total_JB : 0;
                double purity_eS = (reco_total_eS > 0) ? matched_eS / reco_total_eS : 0;
                
                double stability = (truth_total > 0) ? matched / truth_total : 0;
                double stability_JB = (truth_total_JB > 0) ? matched_JB / truth_total_JB : 0;
                double stability_eS = (truth_total_eS > 0) ? matched_eS / truth_total_eS : 0;
                
                /*std::cout << "Bin " << i
                 << ": Purity = " << purity
                 << ", Stability = " << stability
                 << std::endl;*/
                
                h_purity->SetBinContent(i, purity);
                h_purity_JB->SetBinContent(i, purity_JB);
                h_purity_eS->SetBinContent(i, purity_eS);
                
                h_stability->SetBinContent(i, stability);
                h_stability_JB->SetBinContent(i, stability_JB);
                h_stability_eS->SetBinContent(i, stability_eS);
            }
            
        }
        inputRootFile->Close();
    }
    cout << "Check integrals: " << endl;
    cout << "pt6_mc integral = " << h6_pt_MC->Integral() << endl;
    cout << "pt_RP_reco integral = " << h_pt_RomanPots->Integral() << endl;
    
    // Get number of entries (events)
    Long64_t Nevents_lAger = T->GetEntries();
    std::cout << "Total entries from lAger: " << Nevents_lAger <<" but " << z6 << " are used." <<std::endl;
    
    /* // Loop through all events
     for (Long64_t l = 0; l < h6_pt_MC->Integral(); l++) {
     T->GetEntry(l);
     t_lAger->Fill(-t); // Fill histogram with -t
     }*/
    
    // Differential Cross Section dσ/dt
    auto h_dsigma_dt = (TH1D*)mtgg->Clone("h_dsigma_dt");
    auto h_dsigma_dt_rc = (TH1D*)mtgTot->Clone("h_dsigma_dt_rc");
    auto h_dsigma_dt_test = (TH1D*)mtggtest->Clone("h_dsigma_dt_test");
    auto h_dsigma_dt_test_rc = (TH1D*)mtggtest_rc->Clone("h_dsigma_dt_test_rc");
    auto h_dsigma_dt_rscl = (TH1D*)mtgg->Clone("h_dsigma_dt_rscl");
    auto h_dsigma_dt_rscl_rc = (TH1D*)mtgTot1->Clone("h_dsigma_dt_rscl_rc");
    
    TFile * outputFile = new TFile(outputFileName, "RECREATE");
    
    h6_px_MC->Write();
    h6_py_MC->Write();
    h6_pt_MC->Write();
    h6_pz_MC->Write();
    h6_e_MC->Write();
    
    h8_pt_MC->Write();
    rcElectronpt->Write();
    rcElectron->Write();
    
    fProton->Write();
    fPositron->Write();
    fElectron->Write();
    
    JPsiMass1->Write();
    rcJPsiMass1->Write();
    
    h_px_RomanPots->Write();
    h_py_RomanPots->Write();
    h_pt_RomanPots->Write();
    h_pz_RomanPots->Write();
    h_e_RomanPots->Write();
    RPfProton->Write();
    h_rp_occupancy_map->Write();
    
    h_px_reco_track->Write();
    h_py_reco_track->Write();
    h_pt_reco_track->Write();
    h_pz_reco_track->Write();
    h_e_reco_track->Write();
    B0fProton->Write();
    
    mtgg->Write();
    mtgTot1->Write();
    mtggtest->Write();
    mtggtest_rc->Write();
    t_anotherhist_neg->Write();
    t_anotherhist->Write();
    t_anotherhist_rc->Write();
    t_anotherhist_acc->Write();
    
    //outputFile->Close();
    
    TCanvas *c1 = new TCanvas("c1", "Final #eta not overlayed", 800, 800);
    
    // Draw MC proton eta distribution
    fProton->SetStats(0);
    fProton->SetLineColor(kBlack);
    fProton->SetLineWidth(4);
    fProton->GetXaxis()->SetTitle("#eta");
    fProton->GetYaxis()->SetTitle("Counts");
    fProton->GetYaxis()->SetTitleSize(0.03);
    fProton->Draw();
    
    // Overlay RECO proton tracks from RP
    RPfProton->SetLineColor(kRed);
    RPfProton->SetLineWidth(0);
    RPfProton->Draw("SAME");
    
    // Overlay RECO proton tracks from B0
    B0fProton->SetLineColor(kBlue);
    B0fProton->SetLineWidth(0);
    B0fProton->Draw("SAME");
    
    // Add legend
    TLegend *leg = new TLegend(0.55, 0.5, 0.85, 0.65);
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);
    leg->AddEntry(fProton, "p' MC", "l");
    leg->AddEntry(RPfProton, "p' RECO track - RP", "l");
    leg->AddEntry(B0fProton, "p' RECO track - B0", "l");
    leg->Draw();
    
    TLegend *label0 = new TLegend(0.5, 0.7, 0.89, 0.8);
    label0->SetBorderSize(0);
    //label0->AddEntry("","ePIC Preliminary", "");
    label0->AddEntry("","lAger, 1<Q^{2}<10, 10fb^{-1}", "");
    label0->AddEntry("","10x100 GeV, J/#psi #rightarrow (ee^{+})", "");
    label0->Draw();
    
    TCanvas *c1t = new TCanvas("c1t", "Final Proton #eta overlayed", 800, 800);
    
    // Plot MC truth proton eta
    fProton->SetStats(0);
    fProton->SetLineColor(kBlack);
    fProton->SetLineWidth(4);
    fProton->GetXaxis()->SetTitle("#eta");
    fProton->GetXaxis()->SetRangeUser(-10, 20);
    fProton->GetYaxis()->SetTitle("Counts");
    fProton->GetYaxis()->SetTitleSize(0.03);
    fProton->Draw();
    
    // Overlay RECO track (RP + B0)
    B0RPfProton->SetLineColor(kRed);
    B0RPfProton->SetLineWidth(4);
    B0RPfProton->Draw("SAME");
    
    // Main legend
    TLegend *legt = new TLegend(0.55, 0.5, 0.85, 0.65);
    legt->SetBorderSize(0);
    legt->SetFillStyle(0);
    legt->AddEntry(fProton, "p' MC", "l");
    legt->AddEntry(B0RPfProton, "p' RECO track - RP + B0", "l");
    legt->Draw();
    
    // Context/label legend
    TLegend *label0t = new TLegend(0.45, 0.7, 0.89, 0.85);
    label0t->SetBorderSize(0);
    label0t->SetFillStyle(0);
    //label0t->AddEntry("","ePIC Preliminary", "");
    label0t->AddEntry((TObject*)nullptr, "lAger, 1 < Q^{2} < 10 GeV^{2}, 10 fb^{-1}", "");
    label0t->AddEntry((TObject*)nullptr, "10x100 GeV, J/#psi #rightarrow e^{+}e^{-}", "");
    label0t->Draw();
    
    TCanvas *Scattered_Electron = new TCanvas("Scattered_Electron", "Scattered Electron #eta", 800, 800);
    
    // MC truth electron
    fElectron->SetStats(0);
    fElectron->SetLineColor(kBlack);
    fElectron->SetLineWidth(4);
    fElectron->GetXaxis()->SetTitle("#eta");
    fElectron->GetXaxis()->SetRangeUser(-4.5, 1);
    fElectron->GetYaxis()->SetTitle("Counts");
    fElectron->GetYaxis()->SetTitleSize(0.04);
    fElectron->Draw();
    
    // Reconstructed electron
    rcElectron->SetLineColor(kRed);
    rcElectron->SetLineWidth(4);
    rcElectron->SetLineStyle(1);
    rcElectron->Draw("SAME");
    
    // Legend
    TLegend *leg7 = new TLegend(0.65, 0.75, 0.88, 0.88);
    leg7->SetBorderSize(0);
    leg7->SetFillStyle(0);
    leg7->AddEntry(fElectron, "MC e", "l");
    leg7->AddEntry(rcElectron, "RECO e", "l");
    leg7->Draw();
    
    // Canvas with two panels: comparing reco mass methods
    TCanvas *c4 = new TCanvas("c4", "Reco J/#psi Mass Comparison", 1000, 800);
    c4->Divide(2, 1);
    
    // Panel 1
    c4->cd(1);
    rcJPsiMass11->GetXaxis()->SetTitle("M_{e^{+}e^{-}} [GeV]");
    rcJPsiMass11->GetXaxis()->SetRangeUser(2.2, 3.6);
    rcJPsiMass11->SetStats(0);
    rcJPsiMass11->GetYaxis()->SetTitle("Counts");
    rcJPsiMass11->GetYaxis()->SetTitleSize(0.04);
    rcJPsiMass11->SetLineColor(kBlack);
    rcJPsiMass11->SetLineWidth(4);
    rcJPsiMass11->Draw();
    
    rcJPsiMass111->SetLineColor(kRed);
    rcJPsiMass111->SetLineWidth(4);
    rcJPsiMass111->SetLineStyle(1);
    rcJPsiMass111->Draw("SAME");
    
    TLegend *leg22 = new TLegend(0.65, 0.75, 0.88, 0.88);
    leg22->SetBorderSize(0);
    leg22->SetFillStyle(0);
    leg22->AddEntry(rcJPsiMass11, "e^{+}e^{-}", "l");
    leg22->AddEntry(rcJPsiMass111, "e'e^{+}", "l");
    leg22->Draw();
    
    // Panel 2
    c4->cd(2);
    rcJPsiMass1->GetXaxis()->SetTitle("M_{e^{+}e^{-}} [GeV]");
    rcJPsiMass1->GetXaxis()->SetRangeUser(2.2,3.6);
    rcJPsiMass1->SetStats(0);
    rcJPsiMass1->GetYaxis()->SetTitle("Counts");
    rcJPsiMass1->GetYaxis()->SetTitleSize(0.04);
    rcJPsiMass1->SetLineColor(kRed);
    rcJPsiMass1->SetLineWidth(4);
    rcJPsiMass1->Draw();
    
    // Separate canvas with MC vs RECO vs acceptance
    TCanvas *jps_mass = new TCanvas("jps_mass", "J/#psi Invariant Mass", 800, 800);
    JPsiMass1->GetXaxis()->SetTitle("Invariant Mass M_{e^{+}e^{-}} [GeV]");
    JPsiMass1->GetXaxis()->SetRangeUser(2, 3.6);
    JPsiMass1->SetStats(0);
    JPsiMass1->SetLineColor(kBlack);
    JPsiMass1->SetLineWidth(4);
    JPsiMass1->Draw();
    
    // Acceptance
    rcJPsiMassa->SetLineColor(kBlue);
    rcJPsiMassa->SetLineWidth(0);
    rcJPsiMassa->Draw("CP SAME");
    
    // Reco track
    rcJPsiMass1->SetLineColor(kRed);
    rcJPsiMass1->SetLineWidth(0);
    rcJPsiMass1->Draw("CP SAME");
    
    // Legend for MC vs RECO
    TLegend *jpsi_MCReco = new TLegend(0.2, 0.6, 0.6, 0.68);
    jpsi_MCReco->SetBorderSize(0);
    jpsi_MCReco->SetFillStyle(0);
    jpsi_MCReco->AddEntry(JPsiMass1, "MC", "l");
    jpsi_MCReco->AddEntry(rcJPsiMassa, "Acceptance", "l");
    jpsi_MCReco->AddEntry(rcJPsiMass1, "RECO track", "l");
    jpsi_MCReco->Draw();
    
    // Label for context
    TLegend *label2 = new TLegend(0.2, 0.7, 0.59, 0.85);
    label2->SetBorderSize(0);
    label2->SetFillStyle(0);
    //label2->AddEntry("","ePIC Preliminary", "");
    label2->AddEntry("", "lAger, 1 < Q^{2} < 10 GeV^{2}, 10 fb^{-1}", "");
    label2->AddEntry("", "10x100 GeV, J/#psi #rightarrow e^{+}e^{-}", "");
    label2->Draw();
    
    TCanvas *t_distribution = new TCanvas("t_distribution", "t_distribution", 800, 800);
    t_distribution->SetLogy();
    
    // Ratio plot: mtg / mtgg
    TRatioPlot* rp4 = new TRatioPlot(mtg, mtgg, "pois");
    rp4->Draw("AL");
    rp4->SetSeparationMargin(0.02);
    
    // Format ratio (lower) pad
    rp4->GetLowerRefGraph()->SetMinimum(0);
    rp4->GetLowerRefGraph()->SetMaximum(1.5);
    rp4->GetLowerRefGraph()->SetMarkerStyle(8);
    rp4->GetLowerRefGraph()->SetMarkerColor(kBlack);
    rp4->GetLowerRefYaxis()->SetTitle("Ratio");
    rp4->GetLowerRefYaxis()->CenterTitle();
    
    // Format upper (main) pad
    rp4->GetUpperPad()->cd();
    mtgg->GetXaxis()->SetTitle("-t [GeV^{2}]");
    mtgg->GetXaxis()->SetRangeUser(0.00001, 2);
    mtgg->GetYaxis()->SetTitle("Counts");
    mtgg->SetStats(0);
    mtgg->SetLineColor(kBlack);
    mtgg->SetLineWidth(4);
    mtgg->Draw("HIST");
    
    // RECO RP
    mtg->SetMarkerStyle(8);
    mtg->SetMarkerColor(kRed + 1);
    mtg->SetLineWidth(0);
    mtg->Draw("CP SAME");
    
    // RECO B0
    mtgB0->SetMarkerStyle(41);
    mtgB0->SetMarkerColor(kBlue + 1);
    mtgB0->SetLineWidth(0);
    mtgB0->Draw("CP SAME");
    
    // Legend
    TLegend *leg2 = new TLegend(0.55, 0.5, 0.85, 0.65);
    leg2->SetBorderSize(0);
    leg2->SetFillStyle(0);
    leg2->AddEntry(mtgg, "p' MC", "l");
    leg2->AddEntry(mtg, "p' RECO - RP", "f");
    leg2->AddEntry(mtgB0, "p' RECO - B0", "f");
    leg2->Draw();
    
    // Label panel
    TLegend *label14 = new TLegend(0.4, 0.7, 0.89, 0.87);
    label14->SetBorderSize(0);
    label14->SetFillStyle(0);
    //label14->AddEntry("","ePIC Preliminary", "");
    label14->AddEntry("", "lAger, 1 < Q^{2} < 10 GeV^{2}, 10 fb^{-1}", "");
    label14->AddEntry("", "10x100 GeV, J/#psi #rightarrow e^{+}e^{-}", "");
    label14->AddEntry("", "Exclusive cut applied", "");
    label14->Draw();
    
    // ===== Canvas 1: Ratio plot of RECO (RP + B0) / MC =====
    TCanvas *tt_distribution = new TCanvas("tt_distribution", "tt_distribution", 800, 800);
    tt_distribution->SetLogy();
    
    TRatioPlot* rpt = new TRatioPlot(mtgTot, mtgg, "pois");
    rpt->Draw();
    rpt->SetSeparationMargin(0.02);
    
    // --- Lower pad formatting ---
    rpt->GetLowerRefYaxis()->SetTitle("Ratio");
    rpt->GetLowerRefGraph()->SetMinimum(0);
    rpt->GetLowerRefGraph()->SetMaximum(1.5);
    rpt->SetSeparationMargin(0.0);
    rpt->GetLowerRefGraph()->SetMarkerStyle(8);
    rpt->GetLowerRefGraph()->SetMarkerColor(2);
    rpt->GetLowerRefYaxis()->CenterTitle();
    
    // --- Upper pad formatting ---
    rpt->GetUpperPad()->cd();
    mtgg->GetYaxis()->SetNdivisions(0);
    mtgg->GetXaxis()->SetTitle("-t [GeV^{2}]");
    mtgg->GetXaxis()->SetRangeUser(0.00001, 2);
    mtgg->GetYaxis()->SetTitle("Counts");
    mtgg->SetStats(0);
    mtgg->SetLineColor(kBlack);
    mtgg->SetLineWidth(4);
    mtgg->Draw("HIST");
    
    mtgTot->SetMarkerStyle(43);
    mtgTot->SetMarkerColor(kMagenta);
    mtgTot->SetLineWidth(0);
    mtgTot->SetStats(0);
    mtgTot->Draw("CP SAME");
    
    // --- Legend ---
    TLegend *leg2t = new TLegend(0.55, 0.5, 0.87, 0.65);
    leg2t->SetBorderSize(0);
    leg2t->SetFillStyle(0);
    leg2t->AddEntry(mtgg, "p' MC", "l");
    leg2t->AddEntry(mtgTot, "p' RECO - RP + B0", "p f");
    leg2t->Draw();
    
    // --- Label block ---
    TLegend *label14t = new TLegend(0.4, 0.7, 0.89, 0.87);
    label14t->SetBorderSize(0);
    label14t->SetFillStyle(0);
    //label14t->AddEntry("","ePIC Preliminary", "");
    label14t->AddEntry("", "lAger, 1 < Q^{2} < 10 GeV^{2}, 10 fb^{-1}", "");
    label14t->AddEntry("", "10x100 GeV, J/#psi #rightarrow e^{+}e^{-}", "");
    label14t->AddEntry("", "Exclusive cut applied", "");
    label14t->Draw();
    
    
    // ===== Canvas 2: Acceptance view in -t =====
    TCanvas *tt_distribution_accept = new TCanvas("tt_distribution_accept", "tt_distribution_accept", 800, 800);
    tt_distribution_accept->SetLogy();
    
    mtgg->GetYaxis()->SetNdivisions(0);
    mtgg->GetXaxis()->SetTitle("-t [GeV^{2}]");
    mtgg->GetXaxis()->SetRangeUser(0.00001, 2);
    mtgg->GetYaxis()->SetTitle("Counts");
    mtgg->SetStats(0);
    mtgg->SetLineColor(kBlack);
    mtgg->SetLineWidth(4);
    mtgg->Draw("HIST");
    
    mtgg_accept->SetMarkerStyle(3);
    mtgg_accept->SetMarkerColor(kBlue);
    mtgg_accept->SetLineColor(kBlue);
    mtgg_accept->SetLineWidth(2);
    mtgg_accept->Draw("LP SAME");
    
    mtgTot->SetMarkerStyle(43);
    mtgTot->SetMarkerColor(kMagenta);
    mtgTot->SetLineWidth(0);
    mtgTot->SetStats(0);
    mtgTot->Draw("CP SAME");
    
    // --- Legend ---
    TLegend *leg2t_accept = new TLegend(0.55, 0.5, 0.87, 0.68);
    leg2t_accept->SetBorderSize(0);
    leg2t_accept->SetFillStyle(0);
    leg2t_accept->AddEntry(mtgg, "p' MC", "l");
    leg2t_accept->AddEntry(mtgg_accept, "p' Acceptance - RP only", "lp");
    leg2t_accept->AddEntry(mtgTot, "p' RECO track - RP + B0", "p f");
    leg2t_accept->Draw();
    
    // === Canvas c10t: pT comparison - MC and RP+B0 ===
    TCanvas *c10t = new TCanvas("c10t", "Transverse momentum plot", 800, 800);
    //c10t->SetLogy();
    
    h6_pt_MC->GetXaxis()->SetTitle("p_{T} [GeV]");
    h6_pt_MC->GetYaxis()->SetTitle("Counts");
    h6_pt_MC->SetStats(0);
    h6_pt_MC->GetXaxis()->SetRangeUser(0.0, 1.7);
    h6_pt_MC->SetLineColor(kBlack);
    h6_pt_MC->SetLineWidth(4);
    h6_pt_MC->Draw(" HIST");
    
    h_pt_B0RomanPots->SetLineColor(kMagenta);
    h_pt_B0RomanPots->SetLineWidth(0);
    h_pt_B0RomanPots->Draw("SAME");
    
    TLegend *leg5t = new TLegend(0.55, 0.55, 0.87, 0.68);
    leg5t->SetBorderSize(0);
    leg5t->SetFillStyle(0);
    leg5t->AddEntry(h6_pt_MC, "p' MC", "l");
    leg5t->AddEntry(h_pt_B0RomanPots, "p' RECO - RP + B0", "l");
    leg5t->Draw();
    
    TLegend *label3t = new TLegend(0.5, 0.7, 0.89, 0.82);
    label3t->SetBorderSize(0);
    label3t->SetFillStyle(0);
    //label3t->AddEntry("","ePIC Preliminary", "");
    label3t->AddEntry("", "lAger, 1 < Q^{2} < 10 GeV^{2}, 10 fb^{-1}", "");
    label3t->AddEntry("", "10x100 GeV, J/#psi #rightarrow e^{+}e^{-}", "");
    label3t->Draw();
    
    TCanvas *Scattered_Electron_P_T = new TCanvas("Scattered_Electron_P_T", "Scattered Electron P_{T}", 800, 800);
    
    // MC truth electron
    h8_pt_MC->GetXaxis()->SetTitle("p_{T} [GeV/c]");
    h8_pt_MC->GetYaxis()->SetTitle("Counts");
    h8_pt_MC->GetXaxis()->SetRangeUser(0, 3);
    h8_pt_MC->SetStats(0);
    h8_pt_MC->SetLineColor(kBlack);
    h8_pt_MC->SetLineWidth(4);
    h8_pt_MC->Draw("HIST");
    
    // Reconstructed electron
    rcElectronpt->SetLineColor(kRed);
    rcElectronpt->SetLineWidth(4);
    rcElectronpt->SetLineStyle(1); //
    rcElectronpt->Draw("SAME");
    
    // Legend
    TLegend *leg8 = new TLegend(0.6, 0.75, 0.88, 0.88);
    leg8->SetBorderSize(0);
    leg8->SetFillStyle(0);
    leg8->AddEntry(h8_pt_MC, "MC e'", "l");
    leg8->AddEntry(rcElectronpt, "RECO e'", "l");
    leg8->Draw();
    
    TCanvas *Final_Positron_P_T = new TCanvas("Final_Positron_P_T", "Final_PositronElectron_P_T", 1400, 1000);
    Final_Positron_P_T->Divide(3, 2);
    
    // ---------- Plot 1: pT of positron from J/ψ ----------
    Final_Positron_P_T->cd(1);
    h7_pt_MC->SetStats(0);
    h7_pt_MC->SetLineColor(kBlack);
    h7_pt_MC->SetLineWidth(4);
    h7_pt_MC->GetXaxis()->SetTitle("p_{T} [GeV]");
    h7_pt_MC->GetXaxis()->SetRangeUser(-5, 5);
    h7_pt_MC->GetYaxis()->SetTitleSize(0.03);
    h7_pt_MC->Draw();
    
    rcPositronpt->SetMarkerStyle(8);
    rcPositronpt->SetMarkerColor(kRed);
    rcPositronpt->SetLineWidth(0);
    rcPositronpt->Draw("CP SAME");
    
    TLegend *leg1 = new TLegend(0.2, 0.55, 0.45, 0.65);
    leg1->SetBorderSize(0);
    leg1->AddEntry(h7_pt_MC, "e^{+} MC", "l");
    leg1->AddEntry(rcPositronpt, "e^{+} RECO track", "p f");
    leg1->Draw();
    
    TLegend *info1 = new TLegend(0.15, 0.7, 0.45, 0.85);
    info1->SetBorderSize(0);
    info1->AddEntry("", "lAger, 1<Q^{2}<10, 10 fb^{-1}", "");
    info1->AddEntry("", "10x100 GeV", "");
    info1->Draw();
    
    // ---------- Plot 2: pT of electron from J/ψ ----------
    Final_Positron_P_T->cd(2);
    hjpsi_pt_MC->SetStats(0);
    hjpsi_pt_MC->SetLineColor(kBlack);
    hjpsi_pt_MC->SetLineWidth(4);
    hjpsi_pt_MC->GetXaxis()->SetTitle("p_{T} [GeV]");
    hjpsi_pt_MC->GetXaxis()->SetRangeUser(-0.1, 4);
    //hjpsi_pt_MC->GetYaxis()->SetRangeUser(0, 25000);
    hjpsi_pt_MC->GetYaxis()->SetTitleSize(0.03);
    hjpsi_pt_MC->Draw();
    
    rcElectronjpsipt->SetMarkerStyle(8);
    rcElectronjpsipt->SetMarkerColor(kRed);
    rcElectronjpsipt->SetLineWidth(0);
    rcElectronjpsipt->Draw("CP SAME");
    
    TLegend *leg2pt = new TLegend(0.52, 0.55, 0.82, 0.7);
    leg2pt->SetBorderSize(0);
    leg2pt->AddEntry(hjpsi_pt_MC, "e J/#psi MC", "l");
    leg2pt->AddEntry(rcElectronjpsipt, "e J/#psi RECO track", "p f");
    leg2pt->Draw();
    
    TLegend *info2 = new TLegend(0.5, 0.7, 0.89, 0.85);
    info2->SetBorderSize(0);
    info2->AddEntry("", "lAger, 1<Q^{2}<10, 10 fb^{-1}", "");
    info2->AddEntry("", "10x100 GeV", "");
    info2->Draw();
    
    // ---------- Plot 3: pT of scattered electron ----------
    Final_Positron_P_T->cd(3);
    h8_pt_MC->SetStats(0);
    h8_pt_MC->SetLineColor(kBlack);
    h8_pt_MC->SetLineWidth(4);
    h8_pt_MC->GetXaxis()->SetTitle("p_{T} [GeV]");
    h8_pt_MC->GetXaxis()->SetRangeUser(-0.1, 4);
    //h8_pt_MC->GetYaxis()->SetRangeUser(0, 25000);
    h8_pt_MC->GetYaxis()->SetTitleSize(0.03);
    h8_pt_MC->Draw();
    
    rcElectronpt->SetMarkerStyle(8);
    rcElectronpt->SetMarkerColor(kRed);
    rcElectronpt->SetLineWidth(0);
    rcElectronpt->Draw("CP SAME");
    
    TLegend *leg3 = new TLegend(0.52, 0.55, 0.82, 0.7);
    leg3->SetBorderSize(0);
    leg3->AddEntry(h8_pt_MC, "e' MC", "l");
    leg3->AddEntry(rcElectronpt, "e' RECO track", "p f");
    leg3->Draw();
    
    TLegend *info3 = new TLegend(0.5, 0.7, 0.89, 0.85);
    info3->SetBorderSize(0);
    info3->AddEntry("", "lAger, 1<Q^{2}<10, 10 fb^{-1}", "");
    info3->AddEntry("", "10x100 GeV", "");
    info3->Draw();
    
    // ---------- Plot 4: η of positron from J/ψ ----------
    Final_Positron_P_T->cd(4);
    h7_eta_MC->SetStats(0);
    h7_eta_MC->SetLineColor(kBlack);
    h7_eta_MC->SetLineWidth(4);
    h7_eta_MC->GetXaxis()->SetTitle("#eta");
    h7_eta_MC->GetXaxis()->SetRangeUser(-5, 5);
    h7_eta_MC->Draw();
    
    rcPositron->SetMarkerStyle(8);
    rcPositron->SetMarkerColor(kRed);
    rcPositron->SetLineWidth(0);
    rcPositron->Draw("CP SAME");
    
    TLegend *leg4 = new TLegend(0.6, 0.57, 0.87, 0.7);
    leg4->SetBorderSize(0);
    leg4->AddEntry(h7_eta_MC, "e^{+} MC", "l");
    leg4->AddEntry(rcPositron, "e^{+} RECO track", "p f");
    leg4->Draw();
    
    TLegend *info4 = new TLegend(0.55, 0.73, 0.89, 0.85);
    info4->SetBorderSize(0);
    info4->AddEntry("", "lAger, 1<Q^{2}<10, 10 fb^{-1}", "");
    info4->AddEntry("", "10x100 GeV", "");
    info4->Draw();
    
    // ---------- Plot 5: η of electron from J/ψ ----------
    Final_Positron_P_T->cd(5);
    hjpsi_eta_MC->SetStats(0);
    hjpsi_eta_MC->SetLineColor(kBlack);
    hjpsi_eta_MC->SetLineWidth(4);
    hjpsi_eta_MC->GetXaxis()->SetTitle("#eta");
    hjpsi_eta_MC->GetXaxis()->SetRangeUser(-5, 5);
    //hjpsi_eta_MC->GetYaxis()->SetRangeUser(0, 35000);
    hjpsi_eta_MC->GetYaxis()->SetTitleSize(0.03);
    hjpsi_eta_MC->Draw();
    
    rcElectronjpsieta->SetMarkerStyle(8);
    rcElectronjpsieta->SetMarkerColor(kRed);
    rcElectronjpsieta->SetLineWidth(0);
    rcElectronjpsieta->Draw("CP SAME");
    
    TLegend *leg5 = new TLegend(0.52, 0.55, 0.82, 0.7);
    leg5->SetBorderSize(0);
    leg5->AddEntry(hjpsi_eta_MC, "e J/#psi MC", "l");
    leg5->AddEntry(rcElectronjpsieta, "e J/#psi RECO track", "p f");
    leg5->Draw();
    
    TLegend *info5 = new TLegend(0.5, 0.7, 0.89, 0.85);
    info5->SetBorderSize(0);
    info5->AddEntry("", "lAger, 1<Q^{2}<10, 10 fb^{-1}", "");
    info5->AddEntry("", "10x100 GeV", "");
    info5->Draw();
    
    // ---------- Plot 6: η of scattered electron ----------
    Final_Positron_P_T->cd(6);
    h8_eta_MC->SetStats(0);
    h8_eta_MC->SetLineColor(kBlack);
    h8_eta_MC->SetLineWidth(4);
    h8_eta_MC->GetXaxis()->SetTitle("#eta");
    h8_eta_MC->GetXaxis()->SetRangeUser(-5, 5);
    h8_eta_MC->GetYaxis()->SetTitleSize(0.03);
    h8_eta_MC->Draw();
    
    rcElectron->SetMarkerStyle(8);
    rcElectron->SetMarkerColor(kRed);
    rcElectron->SetLineWidth(0);
    rcElectron->Draw("CP SAME");
    
    TLegend *leg6 = new TLegend(0.52, 0.55, 0.82, 0.7);
    leg6->SetBorderSize(0);
    leg6->AddEntry(h8_eta_MC, "e' MC", "l");
    leg6->AddEntry(rcElectron, "e' RECO track", "p f");
    leg6->Draw();
    
    TLegend *info6 = new TLegend(0.5, 0.7, 0.89, 0.85);
    info6->SetBorderSize(0);
    info6->AddEntry("", "lAger, 1<Q^{2}<10, 10 fb^{-1}", "");
    info6->AddEntry("", "10x100 GeV", "");
    info6->Draw();
    
    // Create canvas for Q² plot with logarithmic Y-axis
    TCanvas *q_sq = new TCanvas("q_sq", "Q^{2} Comparison", 800, 800);
    q_sq->cd(1)->SetLogy();
    
    // Create and draw ratio plot (RECO / MC)
    /*TRatioPlot* rp3 = new TRatioPlot(mtqq, mtq, "pois");
     rp3->Draw();
     
     // Configure the lower pad (ratio)
     rp3->GetLowerPad()->cd();
     rp3->SetSeparationMargin(0);
     rp3->GetLowerRefYaxis()->SetTitle("Ratio");
     rp3->GetLowerRefGraph()->SetMinimum(0.6);
     rp3->GetLowerRefGraph()->SetMaximum(1.2);
     rp3->GetLowerRefGraph()->SetMarkerStyle(8);
     rp3->GetLowerRefGraph()->SetMarkerColor(2);
     
     // Configure the upper pad (main plots)
     rp3->GetUpperPad()->cd();*/
    
    // MC curve
    mtq->SetStats(0);
    mtq->SetLineColor(1);
    mtq->SetLineWidth(4);
    mtq->GetXaxis()->SetTitle("Q^{2} [GeV^{2}]");
    mtq->GetYaxis()->SetNdivisions(0);
    mtq->GetYaxis()->SetTitle("Counts");
    mtq->Draw();
    
    // Inclusive curve
    EM_Q2->SetStats(0);
    EM_Q2->SetLineColor(kViolet);
    EM_Q2->SetLineWidth(4);
    EM_Q2->Draw("SAME");
    
    // RECO track (primary)
    mtqq->SetStats(0);
    mtqq->SetLineColor(kRed);
    mtqq->SetLineWidth(4);
    mtqq->Draw("SAME");
    
    // RECO from EEMC
    mtqq_2->SetStats(0);
    mtqq_2->SetLineColor(kOrange);
    mtqq_2->SetLineWidth(4);
    mtqq_2->Draw("SAME");
    
    // acceptance
    mtq_acc->SetStats(0);
    mtq_acc->SetLineColor(kBlue);
    mtq_acc->SetLineWidth(4);
    mtq_acc->Draw("CP SAME");
    
    // Add legend for plot elements
    TLegend *qq2 = new TLegend(0.55, 0.55, 0.8, 0.65);
    qq2->SetBorderSize(0);
    qq2->AddEntry(mtq, "MC", "l");
    qq2->AddEntry(mtq_acc, "Accept", "l");
    qq2->AddEntry(mtqq, "RECO track", "l");
    qq2->AddEntry(mtqq_2, "EEMC", "l");
    qq2->AddEntry(EM_Q2, "Inclusive", "l");
    qq2->Draw();
    
    // Label with kinematics info
    TLegend *label15 = new TLegend(0.4, 0.7, 0.89, 0.87);
    label15->SetBorderSize(0);
    // label15->AddEntry("","ePIC Preliminary", "");
    label15->AddEntry("", "lAger, 1 < Q^{2} < 10, 10 fb^{-1}", "");
    label15->AddEntry("", "10x100 GeV, J/#psi #rightarrow e^{+}e^{-}", "");
    label15->Draw();
    
    // Final canvas update
    q_sq->Update();
    
    // Create canvas for x_Bj plot with logarithmic Y-axis
    TCanvas *xbjk = new TCanvas("xbjk", "x_{bj} Comparison", 800, 800);
    xbjk->cd(1)->SetLogy();
    
    // Create and draw ratio plot (RECO / MC)
    /*TRatioPlot* rp1 = new TRatioPlot(xb1, xb, "pois");
     rp1->Draw();
     
     // Configure lower pad (ratio)
     rp1->SetSeparationMargin(0.0);
     rp1->GetLowerRefYaxis()->SetTitle("Ratio");
     rp1->GetLowerRefGraph()->SetMinimum(0);
     rp1->GetLowerRefGraph()->SetMaximum(1.5);
     rp1->GetLowerRefGraph()->SetMarkerStyle(8);
     rp1->GetLowerRefGraph()->SetMarkerColor(2);
     
     // Configure upper pad (main plot)
     rp1->GetUpperPad()->cd();*/
    
    // MC curve
    xb->SetStats(0);
    xb->SetLineColor(1);
    xb->SetLineWidth(4);
    xb->GetXaxis()->SetRangeUser(0,0.05);
    xb->GetXaxis()->SetTitle("x_{bj}");
    xb->GetYaxis()->SetNdivisions(0);
    xb->GetYaxis()->SetTitle("Counts");
    xb->Draw();
    
    // Inclusive curve
    EM_x->SetStats(0);
    EM_x->SetLineColor(6);
    EM_x->SetLineWidth(4);
    EM_x->Draw("SAME");
    
    xba->SetStats(0);
    xba->SetLineColor(4);
    xba->SetLineWidth(4);
    xba->Draw("SAME");
    
    // RECO curve
    xb1->SetStats(0);
    xb1->SetLineColor(kRed);
    xb1->SetLineWidth(4);
    xb1->Draw("SAME");
    
    // Legend for plot components
    TLegend *xbjkk = new TLegend(0.55, 0.55, 0.8, 0.65);
    xbjkk->SetBorderSize(0);
    xbjkk->AddEntry(xb, "MC", "l");
    xbjkk->AddEntry(xba, "Accept", "l");
    xbjkk->AddEntry(xb1, "RECO track", "l");
    xbjkk->AddEntry(EM_x, "Inclusive", "l");
    xbjkk->Draw();
    
    // Label for kinematic conditions
    TLegend *label16 = new TLegend(0.4, 0.7, 0.89, 0.85);
    label16->SetBorderSize(0);
    // label16->AddEntry("", "ePIC Preliminary", "");
    label16->AddEntry("", "lAger, 1 < Q^{2} < 10, 10 fb^{-1}", "");
    label16->AddEntry("", "10x100 GeV, J/#psi #rightarrow e^{+}e^{-}", "");
    label16->Draw();
    
    // Final canvas update
    xbjk->Update();
    
    // Create canvas for x_v plot with logarithmic Y-axis
    TCanvas *x_v = new TCanvas("x_v", "x_{v} Comparison", 800, 800);
    x_v->cd(1)->SetLogy();
    
    // Create and draw ratio plot (RECO / MC)
    TRatioPlot* rp2 = new TRatioPlot(xv1, xv, "pois");
    rp2->Draw();
    
    // Configure lower pad (ratio)
    rp2->SetSeparationMargin(0.0);
    rp2->GetLowerRefYaxis()->SetTitle("Ratio");
    rp2->GetLowerRefGraph()->SetMinimum(0);
    rp2->GetLowerRefGraph()->SetMaximum(1.6);
    rp2->GetLowerRefGraph()->SetMarkerStyle(8);
    rp2->GetLowerRefGraph()->SetMarkerColor(2);
    
    // Configure upper pad (main plot)
    rp2->GetUpperPad()->cd();
    
    // MC curve
    xv->SetStats(0);
    xv->SetLineColor(1);
    xv->SetLineWidth(4);
    xv->GetYaxis()->SetTitle("Counts");
    xv->GetXaxis()->SetRangeUser(0,0.055);
    xv->GetXaxis()->SetTitle("x_{v}");
    xv->GetYaxis()->SetNdivisions(0);
    xv->Draw();
    
    // RECO curve
    xv1->SetStats(0);
    xv1->SetMarkerStyle(8);
    xv1->SetMarkerColor(2);
    xv1->SetLineWidth(0);
    xv1->Draw("CP SAME");
    
    // Optional: Add second RECO source (e.g., EEMC or acceptance)
    xv2->SetStats(0);
    xv2->SetMarkerStyle(45);
    xv2->SetMarkerColor(4);
    xv2->SetLineWidth(0);
    xv2->Draw("CP SAME");
    
    // Legend for components
    TLegend *xvv = new TLegend(0.55, 0.55, 0.8, 0.65);
    xvv->SetBorderSize(0);
    xvv->AddEntry(xv, "MC", "l");
    xvv->AddEntry(xv1, "RECO track", "p f");
    xvv->AddEntry(xv2, "J/#psi Acceptance", "kFullFourTrianglesPlus");
    xvv->Draw();
    
    // Label for kinematic conditions
    TLegend *label17 = new TLegend(0.4, 0.7, 0.89, 0.87);
    label17->SetBorderSize(0);
    // label17->AddEntry("", "ePIC Preliminary", "");
    label17->AddEntry("", "lAger, 1 < Q^{2} < 10, 10 fb^{-1}", "");
    label17->AddEntry("", "10x100 GeV, J/#psi #rightarrow e^{+}e^{-}", "");
    label17->Draw();
    
    // Final canvas update
    x_v->Update();
    
    // Create a 2x2 canvas for resolution studies
    TCanvas *resolution = new TCanvas("resolution", "Resolution Plots", 1200, 1200);
    resolution->Divide(2, 2);
    
    // ---------- Subpad 1: J/ψ mass resolution ----------
    resolution->cd(1);
    diffjpsimass->GetXaxis()->SetRangeUser(-1, 1);
    diffjpsimass->SetLineColor(kRed);;
    diffjpsimass->SetStats(0);
    diffjpsimass->SetLineWidth(4);
    diffjpsimass->Draw();
    
    // Legend for J/ψ mass
    TLegend *legend_mass = new TLegend(0.6, 0.4, 0.7, 0.5);
    legend_mass->SetBorderSize(0);
    legend_mass->AddEntry(diffjpsimass, "RECO", "kFullFourTrianglesPlus");
    legend_mass->Draw();
    
    // Kinematics label
    TLegend *label_mass = new TLegend(0.52, 0.55, 0.89, 0.8);
    label_mass->SetBorderSize(0);
    label_mass->AddEntry("", "lAger, 1 < Q^{2} < 10, 10 fb^{-1}", "");
    label_mass->AddEntry("", "10x100 GeV", "");
    label_mass->AddEntry("", "", "");
    label_mass->AddEntry("", "#Delta_{J/#psi_{mass}}^{rel} = (truth - RECO) / truth", "");
    label_mass->Draw();
    
    // ---------- Subpad 2: Q² resolution ----------
    resolution->cd(2);
    diffq2->GetXaxis()->SetRangeUser(-1, 1);
    diffq2->GetYaxis()->SetRangeUser(0, 60000);
    diffq2->SetLineColor(1);
    diffq2->SetLineWidth(4);
    diffq2->SetStats(0);
    diffq2->Draw();
    
    diff_eSig_q2->SetLineColor(kRed);
    diff_eSig_q2->SetLineWidth(4);
    diff_eSig_q2->SetStats(0);
    diff_eSig_q2->Draw("SAME");
    
    diff_JB_q2->SetLineColor(kBlue);
    diff_JB_q2->SetLineWidth(4);
    diff_JB_q2->SetStats(0);
    diff_JB_q2->Draw("SAME");
    
    diff_DA_q2->SetLineColor(kGray);
    diff_DA_q2->SetLineWidth(4);
    diff_DA_q2->SetStats(0);
    diff_DA_q2->Draw("SAME");
    
    // Legend for Q²
    TLegend *legend_q2 = new TLegend(0.6, 0.5, 0.85, 0.6);
    legend_q2->SetBorderSize(0);
    legend_q2->AddEntry(diffq2, "E Method", "l");
    legend_q2->AddEntry(diff_eSig_q2, "e#Sigma Method", "l");
    legend_q2->AddEntry(diff_JB_q2, "JB Method", "l");
    legend_q2->AddEntry(diff_DA_q2, "DA Method", "l");
    legend_q2->Draw();
    
    // Label
    TLegend *label_q2 = new TLegend(0.55, 0.65, 0.89, 0.85);
    label_q2->SetBorderSize(0);
    label_q2->AddEntry("", "lAger, 1 < Q^{2} < 10, 10 fb^{-1}", "");
    label_q2->AddEntry("", "10x100 GeV", "");
    label_q2->AddEntry("", "", "");
    label_q2->AddEntry("", "#Delta_{Q^{2}}^{rel} = (truth - RECO) / truth", "");
    label_q2->Draw();
    
    // ---------- Subpad 3: y resolution ----------
    resolution->cd(3);
    diffy->GetXaxis()->SetRangeUser(-1, 1);
    diffy->GetYaxis()->SetRangeUser(0, 60000);
    diffy->SetLineColor(1);
    diffy->SetLineWidth(4);
    diffy->SetStats(0);
    diffy->Draw("CP");
    
    diff_eSig_y->SetLineColor(kRed);
    diff_eSig_y->SetLineWidth(4);
    diff_eSig_y->SetStats(0);
    diff_eSig_y->Draw("CP SAME");
    
    diff_JB_y->SetLineColor(kBlue);
    diff_JB_y->SetLineWidth(4);
    diff_JB_y->SetStats(0);
    diff_JB_y->Draw("CP SAME");
    
    diff_DA_y->SetLineColor(kGray);
    diff_DA_y->SetLineWidth(4);
    diff_DA_y->SetStats(0);
    diff_DA_y->Draw("CP SAME");
    
    // Legend for y
    TLegend *legend_y = new TLegend(0.6, 0.5, 0.85, 0.6);
    legend_y->SetBorderSize(0);
    legend_y->AddEntry(diffy, "E Method", "l");
    legend_y->AddEntry(diff_eSig_y, "e#Sigma Method", "l");
    legend_y->AddEntry(diff_JB_y, "JB Method", "l");
    legend_y->AddEntry(diff_DA_y, "DA Method", "l");
    legend_y->Draw();
    
    // Label
    TLegend *label_y = new TLegend(0.6, 0.6, 0.89, 0.8);
    label_y->SetBorderSize(0);
    label_y->AddEntry("", "lAger, 1 < Q^{2} < 10, 10 fb^{-1}", "");
    label_y->AddEntry("", "10x100 GeV", "");
    label_y->AddEntry("", "", "");
    label_y->AddEntry("", "#Delta_{y}^{rel} = (truth - RECO) / truth", "");
    label_y->Draw();
    
    // ---------- Subpad 4: x_{Bj} resolution ----------
    resolution->cd(4);
    diffxbjk->GetXaxis()->SetRangeUser(-1, 1);
    diffxbjk->GetYaxis()->SetRangeUser(0, 60000);
    diffxbjk->SetLineColor(1);
    diffxbjk->SetLineWidth(4);
    diffxbjk->SetStats(0);
    diffxbjk->Draw("CP");
    
    diff_eSig_xbjk->SetLineColor(kRed);
    diff_eSig_xbjk->SetLineWidth(4);
    diff_eSig_xbjk->SetStats(0);
    diff_eSig_xbjk->Draw("CP SAME");
    
    diff_JB_xbjk->SetLineColor(kBlue);
    diff_JB_xbjk->SetLineWidth(4);
    diff_JB_xbjk->SetStats(0);
    diff_JB_xbjk->Draw("CP SAME");
    
    diff_DA_x->SetLineColor(kGray);
    diff_DA_x->SetLineWidth(4);
    diff_DA_x->SetStats(0);
    diff_DA_x->Draw("CP SAME");
    
    // Legend for x_{Bj}
    TLegend *legend_xbj = new TLegend(0.6, 0.5, 0.85, 0.6);
    legend_xbj->SetBorderSize(0);
    legend_xbj->AddEntry(diffxbjk, "E Method", "l");
    legend_xbj->AddEntry(diff_eSig_xbjk, "e#Sigma Method", "l");
    legend_xbj->AddEntry(diff_JB_xbjk, "JB Method", "l");
    legend_xbj->AddEntry(diff_DA_x, "DA Method", "l");
    legend_xbj->Draw();
    
    // Label
    TLegend *label_xbj = new TLegend(0.55, 0.55, 0.89, 0.8);
    label_xbj->SetBorderSize(0);
    label_xbj->AddEntry("", "lAger, 1 < Q^{2} < 10, 10 fb^{-1}", "");
    label_xbj->AddEntry("", "10x100 GeV", "");
    label_xbj->AddEntry("", "", "");
    label_xbj->AddEntry("", "#Delta_{x_{Bj}}^{rel} = (truth - RECO) / truth", "");
    label_xbj->Draw();
    
    // Update canvas
    resolution->Update();
    
    // Canvas for E - Pz
    TCanvas *E_minus_pz = new TCanvas("E_minus_pz", "E_minus_pz", 800, 800);
    
    mcjpsie_e_minus_pzmc->GetXaxis()->SetTitle("E - P_{z} [GeV]");
    mcjpsie_e_minus_pzmc->GetXaxis()->SetRangeUser(15, 25);
    mcjpsie_e_minus_pzmc->GetYaxis()->SetTitleSize(0.03);
    mcjpsie_e_minus_pzmc->SetStats(0);
    mcjpsie_e_minus_pzmc->SetLineColor(1);
    mcjpsie_e_minus_pzmc->SetLineWidth(4);
    mcjpsie_e_minus_pzmc->Draw();
    
    rcjpsie_e_minus_pz->SetLineColor(kRed);
    rcjpsie_e_minus_pz->SetLineWidth(0);
    rcjpsie_e_minus_pz->Draw("l SAME");
    
    // Legend for E - Pz
    TLegend *mceminuspz = new TLegend(0.6, 0.5, 0.87, 0.6);
    mceminuspz->SetBorderSize(0);
    mceminuspz->AddEntry(mcjpsie_e_minus_pzmc, "MC J/#psi e", "l");
    mceminuspz->AddEntry(rcjpsie_e_minus_pz, "RECO J/#psi e", "l");
    mceminuspz->Draw();
    
    // Info label
    TLegend *labelemp = new TLegend(0.52, 0.62, 0.89, 0.87);
    labelemp->SetBorderSize(0);
    labelemp->AddEntry("", "lAger, 1 < Q^{2} < 10, 10 fb^{-1}", "");
    labelemp->AddEntry("", "10x100 GeV", "");
    labelemp->AddEntry("", "", "");
    labelemp->AddEntry("", "#sum_{h}E_{h} - p_{z,h} + e'", "");
    labelemp->Draw();
    
    
    // Canvas for Inelasticity (y)
    TCanvas *Inelasticity = new TCanvas("Inelasticity", "Inelasticity", 800, 800);
    Inelasticity->SetLogy();
    
    mcrapid->GetXaxis()->SetTitle("y");
    mcrapid->GetYaxis()->SetTitleSize(0.03);
    mcrapid->SetStats(0);
    mcrapid->SetLineColor(1);
    mcrapid->SetLineWidth(6);
    mcrapid->Draw();
    
    EM_y->SetLineColor(6);
    EM_y->SetLineWidth(6);
    EM_y->Draw("SAME");
    
    mcrapida->SetLineColor(4);
    mcrapida->SetLineWidth(6);
    mcrapida->Draw("SAME");
    
    mcrapidrc->SetLineColor(2);
    mcrapidrc->SetLineWidth(6);
    mcrapidrc->Draw("SAME");
    
    // Legend for inelasticity
    TLegend *rapch = new TLegend(0.2, 0.35, 0.6, 0.6);
    rapch->SetBorderSize(0);
    rapch->AddEntry(mcrapid, "e' MC", "l");
    rapch->AddEntry(mcrapida, "e' Accept", "l");
    rapch->AddEntry(mcrapidrc, "e' RECO", "l");
    rapch->AddEntry(EM_y, "Inclusive", "l");
    rapch->Draw();
    
    // Info label
    TLegend *label9 = new TLegend(0.4, 0.6, 0.89, 0.85);
    label9->SetBorderSize(0);
    label9->AddEntry("", "lAger, 1 < Q^{2} < 10, 10 fb^{-1}", "");
    label9->AddEntry("", "10x100 GeV", "");
    label9->AddEntry("", "", "");
    label9->AddEntry("", "#it{y} = #frac{#it{P} #upoint #it{q}}{#it{P} #upoint #it{k}}", "");
    label9->Draw();
    
    // Canvas for energy comparison
    TCanvas *Energies = new TCanvas("Energies_Comparison", "Energies Comparison", 800, 800);
    Energies->SetLogy();
    
    // MC electron energy
    h2_e_MC->GetXaxis()->SetTitle("E [GeV]");
    h2_e_MC->GetYaxis()->SetTitle("Counts");
    h2_e_MC->GetXaxis()->SetRangeUser(0, 10);
    h2_e_MC->GetYaxis()->SetTitleSize(0.03);
    h2_e_MC->SetStats(0);
    h2_e_MC->SetLineColor(1);
    h2_e_MC->SetLineWidth(4);
    h2_e_MC->Draw();
    
    h2_e_acc->SetLineColor(kBlue);
    h2_e_acc->SetLineWidth(0);
    h2_e_acc->Draw("SAME");
    
    // RECO track electron energy
    scat_e_RC->SetLineColor(kBlue);
    scat_e_RC->SetLineWidth(0);
    scat_e_RC->Draw("SAME");
    
    // EEMC-detected electron energy
    scat_e_EcalEnd->SetLineColor(kViolet);
    scat_e_EcalEnd->SetLineWidth(0);
    scat_e_EcalEnd->Draw("SAME");
    
    // Legend for energy sources
    TLegend *e_MC_RC_EcalEnd = new TLegend(0.2, 0.55, 0.6, 0.68);
    e_MC_RC_EcalEnd->SetBorderSize(0);
    e_MC_RC_EcalEnd->AddEntry(h2_e_MC, "e' MC", "l");
    e_MC_RC_EcalEnd->AddEntry(h2_e_acc, "e' Accept", "l");
    e_MC_RC_EcalEnd->AddEntry(scat_e_RC, "e' RECO track", "l");
    e_MC_RC_EcalEnd->AddEntry(scat_e_EcalEnd, "e' EEMC", "l");
    e_MC_RC_EcalEnd->Draw();
    
    // Label for configuration
    TLegend *label4 = new TLegend(0.2, 0.7, 0.59, 0.8);
    label4->SetBorderSize(0);
    // label4->AddEntry("", "ePIC Preliminary", "");
    label4->AddEntry("", "lAger, 1 < Q^{2} < 10, 10 fb^{-1}", "");
    label4->AddEntry("", "10x100 GeV", "");
    label4->Draw();
    
    TCanvas *Ecal_vs_tot_p = new TCanvas("Ecal_vs_tot_p","Ecal_vs_tot_p",1400,700);
    Ecal_vs_tot_p->Divide(3,1);
    
    Ecal_vs_tot_p->cd(1);
    Ecal_vs_pz->GetXaxis()->SetTitle("E/|P|");
    Ecal_vs_pz->GetXaxis()->SetRangeUser(-1,3);
    Ecal_vs_pz->SetTitle("Tracking_Energy/Calorimeter_Energy");
    Ecal_vs_pz->SetStats(0);
    Ecal_vs_pz->SetLineColor(1);
    Ecal_vs_pz->SetLineWidth(4);
    Ecal_vs_pz->Draw();
    
    Ecal_vs_tot_p->cd(2)->SetLogy();
    e_pz_MC->SetLineColor(1);
    e_pz_MC->SetStats(0);
    e_pz_MC->GetXaxis()->SetRangeUser(0.8,1.2);
    e_pz_MC->SetLineWidth(4);
    e_pz_MC->Draw();
    
    e_pz_acc->SetLineColor(kBlue);
    e_pz_acc->SetLineWidth(4);
    e_pz_acc->Draw("Same");
    
    e_pz_rc->SetLineColor(kRed);
    e_pz_rc->SetLineWidth(4);
    e_pz_rc->Draw("Same");
    
    TLegend *e_p_leg = new TLegend(0.2, 0.7, 0.59, 0.8);
    e_p_leg->SetBorderSize(0);
    e_p_leg->AddEntry(e_pz_MC, "e' MC", "l");
    e_p_leg->AddEntry(e_pz_acc, "e' Accept", "l");
    e_p_leg->AddEntry(e_pz_rc, "e' Reco", "l");
    e_p_leg->Draw();
    
    Ecal_vs_tot_p->cd(3);
    e_pz_2D->SetStats(0);
    e_pz_2D->GetXaxis()->SetRangeUser(0.8,1.2);
    e_pz_2D->GetYaxis()->SetRangeUser(0.8,1.2);
    e_pz_2D->GetXaxis()->SetTitle("E/p_{z} (MC)");
    e_pz_2D->GetYaxis()->SetTitle("E/p_{z} (RECO)");
    e_pz_2D->Draw("COLZ");
    
    //-----------------------------
    TCanvas *DA_MC_plot = new TCanvas("DA_MC_plot", "DA_MC_plot", 1400, 1000);
    DA_MC_plot->Divide(3, 2);
    
    // Panel 1: JB x ratio
    DA_MC_plot->cd(1)->SetLogy();
    DA_x_MC->SetLineColor(kBlack);
    DA_x_MC->SetLineWidth(2);
    DA_x_MC->SetStats(0);
    DA_x_MC->GetXaxis()->SetTitle("x_{bj}");
    DA_x_MC->Draw();
    
    DA_x_acc->SetLineColor(4);
    DA_x_acc->SetLineWidth(2);
    DA_x_acc->SetStats(0);
    DA_x_acc->Draw("SAME");
    
    DA_x_in->SetLineColor(kMagenta + 2);
    DA_x_in->SetLineWidth(2);
    DA_x_in->SetStats(0);
    DA_x_in->Draw("SAME");
    
    DA_x_RC->SetLineColor(kRed);
    DA_x_RC->SetLineWidth(2);
    DA_x_RC->SetStats(0);
    DA_x_RC->Draw("SAME");
    
    TLegend *legDA_x = new TLegend(0.4, 0.65, 0.89, 0.87);
    legDA_x->SetBorderSize(0);
    legDA_x->AddEntry(DA_x_MC, "x_{DA} MC", "l");
    legDA_x->AddEntry(DA_x_acc, "Accept", "l");
    legDA_x->AddEntry(DA_x_RC, "x_{DA} Reco", "l");
    legDA_x->AddEntry(DA_x_in, "Inclusive", "l");
    legDA_x->AddEntry("", "", "");
    legDA_x->AddEntry("", "x = #frac{Q^{2}}{4E_{e}E_{p}y}", "");
    legDA_x->Draw();
    
    // Panel 2: JB Q^2 ratio
    DA_MC_plot->cd(2)->SetLogy();
    DA_q2_MC->SetLineColor(kBlack);
    DA_q2_MC->SetLineWidth(2);
    DA_q2_MC->SetStats(0);
    DA_q2_MC->GetXaxis()->SetTitle("Q^{2} [GeV^{2}]");
    DA_q2_MC->Draw();
    
    DA_Q2_in->SetLineColor(kMagenta + 2);
    DA_Q2_in->SetLineWidth(2);
    DA_Q2_in->SetStats(0);
    DA_Q2_in->Draw("SAME");
    
    DA_q2_RC->SetLineColor(kRed);
    DA_q2_RC->SetLineWidth(4);
    DA_q2_RC->SetStats(0);
    DA_q2_RC->Draw("SAME");
    
    DA_q2_acc->SetLineColor(4);
    DA_q2_acc->SetLineWidth(2);
    DA_q2_acc->SetStats(0);
    DA_q2_acc->Draw("SAME");
    
    TLegend *legDA_q2 = new TLegend(0.4, 0.65, 0.89, 0.87);
    legDA_q2->SetBorderSize(0);
    legDA_q2->AddEntry(DA_q2_MC, "Q^{2}_{DA} MC", "l");
    legDA_q2->AddEntry(DA_q2_acc, "Q^{2}_{DA} Accept", "l");
    legDA_q2->AddEntry(DA_q2_RC, "Q^{2}_{DA} Reco", "l");
    legDA_q2->AddEntry(DA_Q2_in, "Inclusive", "l");
    legDA_q2->AddEntry("", "", "");
    legDA_q2->AddEntry("", "Q^{2} = #frac{4E_{e}^{2}}{tan^{2}(#theta/2) + #Sigma/p_{T,h}}", "");
    legDA_q2->Draw();
    
    // Panel 3: JB y comparison
    DA_MC_plot->cd(3)->SetLogy();
    DA_y_MC->SetLineColor(kBlack);
    DA_y_MC->SetLineWidth(2);
    DA_y_MC->SetStats(0);
    DA_y_MC->GetXaxis()->SetTitle("y");
    DA_y_MC->Draw();
    
    DA_y_MC_a->SetLineColor(kBlue);
    DA_y_MC_a->SetLineWidth(2);
    DA_y_MC_a->SetStats(0);
    DA_y_MC_a->Draw("SAME");
    
    DA_y_in->SetLineColor(kMagenta + 2);
    DA_y_in->SetLineWidth(2);
    DA_y_in->SetStats(0);
    DA_y_in->Draw("SAME");
    
    DA_y_RC->SetLineColor(kRed);
    DA_y_RC->SetLineWidth(2);
    DA_y_RC->SetStats(0);
    DA_y_RC->Draw("SAME");
    
    TLegend *legDA_y = new TLegend(0.4, 0.65, 0.89, 0.87);
    legDA_y->SetBorderSize(0);
    legDA_y->AddEntry(DA_y_MC, "y_{DA} MC", "l");
    legDA_y->AddEntry(DA_y_MC_a, "y_{DA} Accept", "l");
    legDA_y->AddEntry(DA_y_RC, "y_{DA} Reco", "l");
    legDA_y->AddEntry(DA_y_in, "Inclusive", "l");
    legDA_y->AddEntry("", "", "");
    legDA_y->AddEntry("", "y = #frac{#Sigma / p_{T,h}}{tan(#theta/2) + #Sigma / p_{T,h}}", "");
    legDA_y->Draw();
    
    DA_MC_plot->cd(4);
    DA_x_2D->SetStats(0);
    DA_x_2D->GetXaxis()->SetTitle("x_{bj} (MC)");
    DA_x_2D->GetYaxis()->SetTitle("x_{bj} (RECO)");
    DA_x_2D->Draw("COLZ");
    
    DA_MC_plot->cd(5);
    DA_q2_2D->SetStats(0);
    DA_q2_2D->GetXaxis()->SetTitle("Q^{2} (MC) [GeV^{2}]");
    DA_q2_2D->GetYaxis()->SetTitle("Q^{2} (RECO) [GeV^{2}]");
    DA_q2_2D->Draw("COLZ");
    
    DA_MC_plot->cd(6);
    DA_y_2D->SetStats(0);
    DA_y_2D->GetXaxis()->SetTitle("y (MC)");
    DA_y_2D->GetYaxis()->SetTitle("y (RECO)");
    DA_y_2D->Draw("COLZ");
    //---------------------------------
    
    TCanvas *JB_MC_plot = new TCanvas("JB_MC_plot", "JB_MC_plot", 1400, 1000);
    JB_MC_plot->Divide(4, 3);
    
    // Panel 1: JB pt2 MC
    JB_MC_plot->cd(1);
    JB_pt2_MC->SetLineColor(kBlack);
    JB_pt2_MC->SetLineWidth(4);
    JB_pt2_MC->SetStats(0);
    JB_pt2_MC->Draw();
    
    JB_pt2_acc->SetLineColor(4);
    JB_pt2_acc->SetLineWidth(4);
    JB_pt2_acc->Draw("Same");
    
    JB_pt2_rc->SetLineColor(2);
    JB_pt2_rc->SetLineWidth(4);
    JB_pt2_rc->Draw("Same");
    
    TLegend *leg_pt2 = new TLegend(0.4, 0.65, 0.89, 0.87);
    leg_pt2->SetBorderSize(0);
    leg_pt2->AddEntry(JB_pt2_MC, "MC", "l");
    leg_pt2->AddEntry(JB_pt2_acc, "Accept", "l");
    leg_pt2->AddEntry(JB_pt2_rc, "Reco", "l");
    leg_pt2->AddEntry("", "p_{T,h} = #sqrt{(#sum p_{x})^{2} + (#sum p_{y})^{2}}", "");
    leg_pt2->Draw();
    
    // Panel 2: JB HFS MC
    JB_MC_plot->cd(2);
    JB_HFS_MC->SetLineColor(kBlack);
    JB_HFS_MC->SetLineWidth(4);
    JB_HFS_MC->SetStats(0);
    JB_HFS_MC->Draw();
    
    JB_HFS_acc->SetLineColor(4);
    JB_HFS_acc->SetLineWidth(4);
    JB_HFS_acc->Draw("Same");
    
    JB_HFS_rc->SetLineColor(2);
    JB_HFS_rc->SetLineWidth(4);
    JB_HFS_rc->Draw("Same");
    
    TLegend *leg_HFS = new TLegend(0.4, 0.65, 0.89, 0.87);
    leg_HFS->SetBorderSize(0);
    leg_HFS->AddEntry(JB_HFS_MC, "MC", "l");
    leg_HFS->AddEntry(JB_HFS_acc, "Accept", "l");
    leg_HFS->AddEntry(JB_HFS_rc, "Reco", "l");
    leg_HFS->AddEntry("", "#Sigma = #sum_{h} E_{h} - #sum_{h} p_{z,h}", "");
    leg_HFS->Draw();
    
    // Panel 3: JB y comparison
    JB_MC_plot->cd(3)->SetLogy();
    JB_y_MC->SetLineColor(kBlack);
    JB_y_MC->SetLineWidth(4);
    JB_y_MC->SetStats(0);
    JB_y_MC->GetXaxis()->SetTitle("y");
    JB_y_MC->Draw();
    
    JB_y_MC_a->SetLineColor(kBlue);
    JB_y_MC_a->SetLineWidth(4);
    JB_y_MC_a->SetStats(0);
    JB_y_MC_a->Draw("SAME");
    
    JB_y_in->SetLineColor(kMagenta + 2);
    JB_y_in->SetLineWidth(4);
    JB_y_in->SetStats(0);
    JB_y_in->Draw("SAME");
    
    JB_y_RC->SetLineColor(kRed);
    JB_y_RC->SetLineWidth(4);
    JB_y_RC->SetStats(0);
    JB_y_RC->Draw("SAME");
    
    TLegend *leg_y = new TLegend(0.4, 0.65, 0.89, 0.87);
    leg_y->SetBorderSize(0);
    leg_y->AddEntry(JB_y_MC, "y_{JB} MC", "l");
    leg_y->AddEntry(JB_y_MC_a, "y_{JB} Accept", "l");
    leg_y->AddEntry(JB_y_RC, "y_{JB} Reco", "l");
    leg_y->AddEntry(JB_y_in, "Inclusive", "l");
    leg_y->AddEntry("", "", "");
    leg_y->AddEntry("", "y = #frac{#Sigma}{2E_{e}}", "");
    leg_y->Draw();
    
    // Panel 4: JB Q^2 ratio
    JB_MC_plot->cd(4)->SetLogy();
    /*TRatioPlot* ratio_Q2 = new TRatioPlot(JB_q2_RC, JB_q2_MC, "pois");
     ratio_Q2->Draw();
     
     // Lower pad (ratio)
     ratio_Q2->GetLowerPad()->cd();
     ratio_Q2->GetLowerRefYaxis()->SetTitle("ratio");
     ratio_Q2->GetLowerRefGraph()->SetMinimum(0.4);
     ratio_Q2->GetLowerRefGraph()->SetMaximum(1.5);
     ratio_Q2->SetSeparationMargin(0);
     ratio_Q2->GetLowerRefGraph()->SetMarkerStyle(8);
     ratio_Q2->GetLowerRefGraph()->SetMarkerColor(kRed);
     
     // Upper pad (distributions)
     ratio_Q2->GetUpperPad()->cd();*/
    JB_q2_MC->SetLineColor(kBlack);
    JB_q2_MC->SetLineWidth(4);
    JB_q2_MC->SetStats(0);
    JB_q2_MC->GetXaxis()->SetTitle("Q^{2} [GeV^{2}]");
    JB_q2_MC->Draw();
    
    JB_Q2_in->SetLineColor(kMagenta + 2);
    JB_Q2_in->SetLineWidth(4);
    JB_Q2_in->SetStats(0);
    JB_Q2_in->Draw("SAME");
    
    JB_q2_RC->SetLineColor(kRed);
    JB_q2_RC->SetLineWidth(4);
    JB_q2_RC->SetStats(0);
    JB_q2_RC->Draw("SAME");
    
    JB_q2_acc->SetLineColor(4);
    JB_q2_acc->SetLineWidth(4);
    JB_q2_acc->SetStats(0);
    JB_q2_acc->Draw("SAME");
    
    TLegend *leg_q2 = new TLegend(0.4, 0.65, 0.89, 0.87);
    leg_q2->SetBorderSize(0);
    leg_q2->AddEntry(JB_q2_MC, "Q^{2}_{JB} MC", "l");
    leg_q2->AddEntry(JB_q2_acc, "Q^{2}_{JB} Accept", "l");
    leg_q2->AddEntry(JB_q2_RC, "Q^{2}_{JB} Reco", "l");
    leg_q2->AddEntry(JB_Q2_in, "Inclusive", "l");
    leg_q2->AddEntry("", "", "");
    leg_q2->AddEntry("", "Q^{2} = #frac{p_{T,h}^{2}}{1 - y}", "");
    leg_q2->Draw();
    
    // Panel 5: JB x ratio
    JB_MC_plot->cd(5)->SetLogy();
    /*TRatioPlot* ratio_x = new TRatioPlot(JB_x_RC, JB_x_MC, "pois");
     ratio_x->Draw();
     
     // Lower pad (ratio)
     ratio_x->GetLowerPad()->cd();
     ratio_x->GetLowerRefYaxis()->SetTitle("ratio");
     ratio_x->GetLowerRefGraph()->SetMinimum(0.4);
     ratio_x->GetLowerRefGraph()->SetMaximum(1.5);
     ratio_x->SetSeparationMargin(0);
     ratio_x->GetLowerRefGraph()->SetMarkerStyle(8);
     ratio_x->GetLowerRefGraph()->SetMarkerColor(kRed);
     
     // Upper pad (distributions)
     ratio_x->GetUpperPad()->cd();*/
    JB_x_MC->SetLineColor(kBlack);
    JB_x_MC->SetLineWidth(4);
    JB_x_MC->SetStats(0);
    JB_x_MC->GetXaxis()->SetTitle("x_{bj}");
    JB_x_MC->Draw();
    
    JB_x_acc->SetLineColor(4);
    JB_x_acc->SetLineWidth(4);
    JB_x_acc->SetStats(0);
    JB_x_acc->Draw("SAME");
    
    JB_x_in->SetLineColor(kMagenta + 2);
    JB_x_in->SetLineWidth(4);
    JB_x_in->SetStats(0);
    JB_x_in->Draw("SAME");
    
    JB_x_RC->SetLineColor(kRed);
    JB_x_RC->SetLineWidth(4);
    JB_x_RC->SetStats(0);
    JB_x_RC->Draw("SAME");
    
    TLegend *leg_x = new TLegend(0.4, 0.65, 0.89, 0.87);
    leg_x->SetBorderSize(0);
    leg_x->AddEntry(JB_x_MC, "x_{JB} MC", "l");
    leg_x->AddEntry(JB_x_acc, "Accept", "l");
    leg_x->AddEntry(JB_x_RC, "x_{JB} Reco", "l");
    leg_x->AddEntry(JB_x_in, "Inclusive", "l");
    leg_x->AddEntry("", "", "");
    leg_x->AddEntry("", "x = #frac{Q^{2}}{4E_{e}E_{p}y}", "");
    leg_x->Draw();
    
    // Panel 6–11: 2D maps and relative differences
    JB_MC_plot->cd(6);
    JB_y_2D->SetStats(0);
    JB_y_2D->GetXaxis()->SetTitle("y (MC)");
    JB_y_2D->GetYaxis()->SetTitle("y (RECO)");
    JB_y_2D->Draw("COLZ");
    
    JB_MC_plot->cd(7);
    JB_q2_2D->SetStats(0);
    JB_q2_2D->GetXaxis()->SetTitle("Q^{2} (MC) [GeV^{2}]");
    JB_q2_2D->GetYaxis()->SetTitle("Q^{2} (RECO) [GeV^{2}]");
    JB_q2_2D->Draw("COLZ");
    
    JB_MC_plot->cd(8);
    JB_xbjk_2D->SetStats(0);
    JB_xbjk_2D->GetXaxis()->SetTitle("x_{bj} (MC)");
    JB_xbjk_2D->GetYaxis()->SetTitle("x_{bj} (RECO)");
    JB_xbjk_2D->Draw("COLZ");
    
    JB_MC_plot->cd(9);
    dJB_y_2D->SetStats(0);
    dJB_y_2D->GetXaxis()->SetTitle("y (MC)");
    dJB_y_2D->GetYaxis()->SetTitle("Rel. diff. y");
    dJB_y_2D->Draw("COLZ");
    
    JB_MC_plot->cd(10);
    dJB_q2_2D->SetStats(0);
    dJB_q2_2D->GetXaxis()->SetTitle("Q^{2} (MC) [GeV^{2}]");
    dJB_q2_2D->GetYaxis()->SetTitle("Rel. diff. Q^{2}");
    dJB_q2_2D->Draw("COLZ");
    
    JB_MC_plot->cd(11);
    dJB_xbjk_2D->SetStats(0);
    dJB_xbjk_2D->GetXaxis()->SetTitle("x_{bj} (MC)");
    dJB_xbjk_2D->GetYaxis()->SetTitle("Rel. diff. x_{bj}");
    dJB_xbjk_2D->Draw("COLZ");
    
    JB_MC_plot->cd(12);
    JB_HFS_2D->SetStats(0);
    JB_HFS_2D->GetXaxis()->SetTitle("HFS (MC)");
    JB_HFS_2D->GetYaxis()->SetTitle("HFS (Reco)");
    JB_HFS_2D->Draw("COLZ");
    
    TCanvas *x_eSigma = new TCanvas("x_eSigma", "x_eSigma", 1400, 1000);
    x_eSigma->Divide(4, 3);
    
    // === Canvas 1: y_eSigma distributions ===
    x_eSigma->cd(1)->SetLogy();
    
    eSig_y_MC->SetLineColor(1);
    eSig_y_MC->SetLineWidth(4);
    eSig_y_MC->SetStats(0);
    eSig_y_MC->GetXaxis()->SetTitle("y_{e#Sigma}");
    eSig_y_MC->Draw();
    
    eSig_y_MC_a->SetLineColor(4);
    eSig_y_MC_a->SetLineWidth(4);
    eSig_y_MC_a->SetStats(0);
    eSig_y_MC_a->Draw("SAME");
    
    ESig_y_in->SetLineColor(6);
    ESig_y_in->SetLineWidth(4);
    ESig_y_in->SetStats(0);
    ESig_y_in->Draw("SAME");
    
    eSig_y_RC->SetLineColor(2);
    eSig_y_RC->SetLineWidth(4);
    eSig_y_RC->SetStats(0);
    eSig_y_RC->Draw("SAME");
    
    TLegend *leg_yes = new TLegend(0.4, 0.65, 0.89, 0.87);
    leg_yes->SetBorderSize(0);
    leg_yes->AddEntry(eSig_y_MC, "y_{e#Sigma} MC", "l");
    leg_yes->AddEntry(eSig_y_MC_a, "y_{e#Sigma} Accept", "l");
    leg_yes->AddEntry(eSig_y_RC, "y_{e#Sigma} Reco", "l");
    leg_yes->AddEntry(ESig_y_in, "Inclusive", "l");
    leg_yes->AddEntry("", "", "");
    leg_yes->AddEntry("", "y_{e#Sigma} = #frac{Q_{e}^{2}}{4E_{e}E_{p}x_{#Sigma}}", "");
    leg_yes->Draw();
    
    // === Canvas 2: Q^2 with ratio plot ===
    x_eSigma->cd(2)->SetLogy();
    
    /*TRatioPlot *ratio_q2 = new TRatioPlot(eSig_q2_RC, eSig_q2_MC, "pois");
     ratio_q2->Draw();
     
     ratio_q2->GetLowerPad()->cd();
     ratio_q2->GetLowerRefYaxis()->SetTitle("ratio");
     ratio_q2->GetLowerRefGraph()->SetMinimum(0.4);
     ratio_q2->GetLowerRefGraph()->SetMaximum(1.2);
     ratio_q2->SetSeparationMargin(0);
     ratio_q2->GetLowerRefGraph()->SetMarkerStyle(8);
     ratio_q2->GetLowerRefGraph()->SetMarkerColor(2);
     
     ratio_q2->GetUpperPad()->cd();*/
    eSig_q2_MC->SetLineColor(1);
    eSig_q2_MC->SetLineWidth(4);
    eSig_q2_MC->SetStats(0);
    eSig_q2_MC->GetXaxis()->SetTitle("Q_{e}^{2} [GeV^{2}]");
    eSig_q2_MC->GetYaxis()->SetNdivisions(0);
    eSig_q2_MC->Draw();
    
    ESig_Q2_in->SetLineColor(6);
    ESig_Q2_in->SetLineWidth(4);
    ESig_Q2_in->SetStats(0);
    ESig_Q2_in->Draw("SAME");
    
    eSig_q2_RC->SetLineColor(2);
    eSig_q2_RC->SetLineWidth(4);
    eSig_q2_RC->SetStats(0);
    eSig_q2_RC->Draw("SAME");
    
    eSig_q2_acc->SetLineColor(4);
    eSig_q2_acc->SetLineWidth(4);
    eSig_q2_acc->SetStats(0);
    eSig_q2_acc->Draw("SAME");
    
    TLegend *leg_q2es = new TLegend(0.4, 0.65, 0.89, 0.87);
    leg_q2es->SetBorderSize(0);
    leg_q2es->AddEntry(eSig_q2_MC, "Q_{e}^{2} MC", "l");
    leg_q2es->AddEntry(eSig_q2_acc, "Q_{e}^{2} Accept", "l");
    leg_q2es->AddEntry(eSig_q2_RC, "Q_{e}^{2} Reco", "l");
    leg_q2es->AddEntry(ESig_Q2_in, "Inclusive", "l");
    leg_q2es->AddEntry("", "", "");
    leg_q2es->AddEntry("", "Q^{2}_{e} = #frac{p^{2}_{T,e}}{1 - y_{e}}", "");
    leg_q2es->Draw();
    
    // === Canvas 3: x_Sigma with ratio plot ===
    x_eSigma->cd(3)->SetLogy();
    
    /*TRatioPlot *ratio_xes = new TRatioPlot(eSig_x_RC, eSig_x_MC, "pois");
     ratio_xes->Draw();
     
     ratio_xes->GetLowerPad()->cd();
     ratio_xes->GetLowerRefYaxis()->SetTitle("ratio");
     ratio_xes->GetLowerRefGraph()->SetMinimum(0.4);
     ratio_xes->GetLowerRefGraph()->SetMaximum(1.2);
     ratio_xes->SetSeparationMargin(0);
     ratio_xes->GetLowerRefGraph()->SetMarkerStyle(8);
     ratio_xes->GetLowerRefGraph()->SetMarkerColor(2);
     
     ratio_xes->GetUpperPad()->cd();*/
    eSig_x_MC->SetLineColor(1);
    eSig_x_MC->SetLineWidth(4);
    eSig_x_MC->SetStats(0);
    eSig_x_MC->GetXaxis()->SetTitle("x_{#Sigma}");
    eSig_x_MC->GetYaxis()->SetNdivisions(0);
    eSig_x_MC->Draw();
    
    eSig_x_acc->SetLineColor(4);
    eSig_x_acc->SetLineWidth(4);
    eSig_x_acc->SetStats(0);
    eSig_x_acc->Draw("SAME");
    
    ESig_x_in->SetLineColor(6);
    ESig_x_in->SetLineWidth(4);
    ESig_x_in->SetStats(0);
    ESig_x_in->Draw("SAME");
    
    eSig_x_RC->SetLineColor(2);
    eSig_x_RC->SetLineWidth(4);
    eSig_x_RC->SetStats(0);
    eSig_x_RC->Draw("SAME");
    
    TLegend *leg_xes = new TLegend(0.4, 0.65, 0.89, 0.87);
    leg_xes->SetBorderSize(0);
    leg_xes->AddEntry(eSig_x_MC, "x_{Sigma} MC", "l");
    leg_xes->AddEntry(eSig_x_acc, "x_{Sigma} Accept", "l");
    leg_xes->AddEntry(eSig_x_RC, "x_{Sigma} Reco", "l");
    leg_xes->AddEntry(ESig_x_in, "Inclusive", "l");
    leg_xes->AddEntry("", "", "");
    leg_xes->AddEntry("", "x_{#Sigma} = #frac{Q^{2}_{#Sigma}}{4E_{e}E_{p}y_{#Sigma}}", "");
    leg_xes->Draw();
    
    // === Canvas 4-6: 2D correlations ===
    x_eSigma->cd(4);
    eSig_y_2D->SetStats(0);
    eSig_y_2D->GetXaxis()->SetTitle("y (MC)");
    eSig_y_2D->GetYaxis()->SetTitle("y (RECO)");
    eSig_y_2D->Draw("COLZ");
    
    x_eSigma->cd(5);
    eSig_q2_2D->SetStats(0);
    eSig_q2_2D->GetXaxis()->SetTitle("Q^{2} (MC) [GeV^{2}]");
    eSig_q2_2D->GetYaxis()->SetTitle("Q^{2} (RECO) [GeV^{2}]");
    eSig_q2_2D->Draw("COLZ");
    
    x_eSigma->cd(6);
    eSig_xbjk_2D->SetStats(0);
    eSig_xbjk_2D->GetXaxis()->SetTitle("x_{bj} (MC)");
    eSig_xbjk_2D->GetYaxis()->SetTitle("x_{bj} (RECO)");
    eSig_xbjk_2D->Draw("COLZ");
    
    // === Canvas 7-9: Relative differences ===
    x_eSigma->cd(7);
    deSig_y_2D->SetStats(0);
    deSig_y_2D->GetXaxis()->SetTitle("y (MC)");
    deSig_y_2D->GetYaxis()->SetTitle("Rel. diff. y");
    deSig_y_2D->Draw("COLZ");
    
    x_eSigma->cd(8);
    deSig_q2_2D->SetStats(0);
    deSig_q2_2D->GetXaxis()->SetTitle("Q^{2} (MC) [GeV^{2}]");
    deSig_q2_2D->GetYaxis()->SetTitle("Rel. diff. Q^{2}");
    deSig_q2_2D->Draw("COLZ");
    
    x_eSigma->cd(9);
    deSig_xbjk_2D->SetStats(0);
    deSig_xbjk_2D->GetXaxis()->SetTitle("x_{bj} (MC)");
    deSig_xbjk_2D->GetYaxis()->SetTitle("Rel. diff. x_{bj}");
    deSig_xbjk_2D->Draw("COLZ");
    
    // Panel 2: JB HFS MC
    x_eSigma->cd(10);
    eS_HFS_MC->SetLineColor(kBlack);
    eS_HFS_MC->SetLineWidth(4);
    eS_HFS_MC->SetStats(0);
    eS_HFS_MC->Draw();
    
    eS_HFS_acc->SetLineColor(4);
    eS_HFS_acc->SetLineWidth(4);
    eS_HFS_acc->Draw("Same");
    
    eS_HFS_rc->SetLineColor(2);
    eS_HFS_rc->SetLineWidth(4);
    eS_HFS_rc->Draw("Same");
    
    TLegend *leg_HFS_eS = new TLegend(0.4, 0.65, 0.89, 0.87);
    leg_HFS_eS->SetBorderSize(0);
    leg_HFS_eS->AddEntry(eS_HFS_MC, "MC", "l");
    leg_HFS_eS->AddEntry(eS_HFS_acc, "Accept", "l");
    leg_HFS_eS->AddEntry(eS_HFS_rc, "Reco", "l");
    leg_HFS_eS->AddEntry("", "#Sigma = #Sigma_{e} + #Sigma_{h}", "");
    leg_HFS_eS->Draw();
    
    // 2D Resolution Plots: MC vs RECO
    auto q2_2D_plot = new TCanvas("q2_2D_plot", "Q^{2} Resolution", 800, 800);
    q2_2D->SetStats(0);
    q2_2D->GetXaxis()->SetTitle("Q^{2} (MC) [GeV^{2}]");
    q2_2D->GetYaxis()->SetTitle("Q^{2} (RECO) [GeV^{2}]");
    q2_2D->Draw("COLZ");
    
    // 2D Resolution Plots: MC vs RECO
    TCanvas *q2_migration = new TCanvas("q2_migration", "Q^{2} migration", 800, 800);
    h2_migration->SetStats(0);
    h2_migration->GetXaxis()->SetTitle("Q^{2} (MC) [GeV^{2}]");
    h2_migration->GetYaxis()->SetTitle("Q^{2} (RECO) [GeV^{2}]");
    h2_migration->Draw("COLZ");
    
    auto xbjk_2D_plot = new TCanvas("xbjk_2D_plot", "x_{bj} Resolution", 800, 800);
    xbjk_2D->SetStats(0);
    xbjk_2D->GetXaxis()->SetTitle("x_{bj} (MC)");
    xbjk_2D->GetYaxis()->SetTitle("x_{bj} (RECO)");
    xbjk_2D->Draw("COLZ");
    
    auto y_2D_plot = new TCanvas("y_2D_plot", "y Resolution", 800, 800);
    y_2D->SetStats(0);
    y_2D->GetXaxis()->SetTitle("y (MC)");
    y_2D->GetYaxis()->SetTitle("y (RECO)");
    y_2D->Draw("COLZ");
    
    auto t_2D_plot = new TCanvas("t_2D_plot", "t Resolution", 800, 800);
    t_2D->SetStats(0);
    t_2D->GetXaxis()->SetTitle("t (MC)");
    t_2D->GetYaxis()->SetTitle("t (RECO)");
    t_2D->Draw("COLZ");
    
    auto dbjk_plot = new TCanvas("dbjk_plot", "Diff_e_2D_x,y,q2", 1600, 500);
    dbjk_plot->Divide(3, 1);
    
    dbjk_plot->cd(1);
    dxbjk_2D->SetStats(0);
    dxbjk_2D->GetXaxis()->SetTitle("x_{bj}");
    dxbjk_2D->GetYaxis()->SetTitle("#Delta x_{bj}");
    dxbjk_2D->Draw("COLZ");
    
    dbjk_plot->cd(2);
    dybjk_2D->SetStats(0);
    dybjk_2D->GetXaxis()->SetTitle("y");
    dybjk_2D->GetYaxis()->SetTitle("#Delta y");
    dybjk_2D->Draw("COLZ");
    
    dbjk_plot->cd(3);
    dq2bjk_2D->SetStats(0);
    dq2bjk_2D->GetXaxis()->SetTitle("Q^{2}");
    dq2bjk_2D->GetYaxis()->SetTitle("#Delta Q^{2}");
    dq2bjk_2D->Draw("COLZ");
    
    // J/ψ Electron η Distribution
    auto ElectronEta = new TCanvas("ElectronEta", "J/psi Electron eta", 800, 800);
    hjpsi_eta_MC->SetStats(0);
    hjpsi_eta_MC->GetXaxis()->SetTitle("#eta");
    hjpsi_eta_MC->GetXaxis()->SetRangeUser(-4.5, 5);
    hjpsi_eta_MC->GetYaxis()->SetTitleSize(0.03);
    hjpsi_eta_MC->SetLineColor(1);
    hjpsi_eta_MC->SetLineWidth(4);
    hjpsi_eta_MC->Draw();
    
    rcElectronjpsieta->SetFillStyle(0);
    rcElectronjpsieta->SetLineColor(2);
    rcElectronjpsieta->SetLineWidth(6);
    rcElectronjpsieta->Draw("SAME");
    
    fPositron->SetLineColor(4);
    fPositron->SetLineWidth(4);
    fPositron->Draw("SAME");
    
    rcPositron->SetFillStyle(0);
    rcPositron->SetLineColor(6);
    rcPositron->SetLineWidth(6);
    rcPositron->Draw("SAME");
    
    auto legEta = new TLegend(0.8, 0.8, 0.9, 0.9);
    legEta->AddEntry(hjpsi_eta_MC, "MC J/#psi e", "l");
    legEta->AddEntry(fPositron, "MC J/#psi e^{+}", "l");
    legEta->AddEntry(rcElectronjpsieta, "RECO J/#psi e", "l");
    legEta->AddEntry(rcPositron, "RECO J/#psi e^{+}", "l");
    legEta->Draw();
    
    // 2D η Correlations
    auto Eta2D = new TCanvas("Eta2D", "MC eta Correlation", 800, 800);
    eEta2D->SetStats(0);
    eEta2D->GetXaxis()->SetTitle("MC J/#psi e #eta");
    eEta2D->GetYaxis()->SetTitle("MC J/#psi e^{+} #eta");
    eEta2D->Draw("COLZ");
    
    auto Eta2Dp = new TCanvas("Eta2Dp", "RECO eta Correlation", 800, 800);
    pEta2D->SetStats(0);
    pEta2D->GetXaxis()->SetTitle("RECO J/#psi e #eta");
    pEta2D->GetYaxis()->SetTitle("RECO J/#psi e^{+} #eta");
    pEta2D->Draw("COLZ");
    
    for (int i = 1; i <= h_dsigma_dt_test->GetNbinsX(); ++i) {
        double dsdt = h_dsigma_dt_test->GetBinContent(i) / (lumi_gen * h_dsigma_dt_test->GetBinWidth(i));
        h_dsigma_dt_test->SetBinContent(i, dsdt);
    }
    
    for (int i = 1; i <= h_dsigma_dt_test_rc->GetNbinsX(); ++i) {
        double dsdt_rc = h_dsigma_dt_test_rc->GetBinContent(i) / (lumi_gen * h_dsigma_dt_test_rc->GetBinWidth(i));
        h_dsigma_dt_test_rc->SetBinContent(i, dsdt_rc);
    }
    for (int i = 1; i <= h_dsigma_dt_rscl->GetNbinsX(); ++i) {
        double N = h_dsigma_dt_rscl->GetBinContent(i); // raw counts
        double dNdt = N / (lumi_gen_ub * h_dsigma_dt_rscl->GetBinWidth(i));
        double err = (N > 0) ? sqrt(N) / (lumi_gen_ub * h_dsigma_dt_rscl->GetBinWidth(i)) : 0.0;
        
        h_dsigma_dt_rscl->SetBinContent(i, dNdt);
        h_dsigma_dt_rscl->SetBinError(i, err);
    }
    for (int i = 1; i <= h_dsigma_dt_rscl_rc->GetNbinsX(); ++i) {
        double N = h_dsigma_dt_rscl_rc->GetBinContent(i); // raw counts
        double dNdt = N / (lumi_gen_ub * h_dsigma_dt_rscl_rc->GetBinWidth(i));
        double err = (N > 0) ? sqrt(N) / (lumi_gen_ub * h_dsigma_dt_rscl_rc->GetBinWidth(i)) : 0.0;
        
        h_dsigma_dt_rscl_rc->SetBinContent(i, dNdt);
        h_dsigma_dt_rscl_rc->SetBinError(i, err);
    }
    
    // Compute integral
    double current_integral = 0.0;
    double current_integral_rc = 0.0;
    for (int i = 1; i <= h_dsigma_dt_rscl->GetNbinsX(); ++i) {
        current_integral += h_dsigma_dt_rscl->GetBinContent(i) * h_dsigma_dt_rscl->GetBinWidth(i);
    }
    for (int i = 1; i <= h_dsigma_dt_rscl_rc->GetNbinsX(); ++i) {
        current_integral_rc += h_dsigma_dt_rscl_rc->GetBinContent(i) * h_dsigma_dt_rscl_rc->GetBinWidth(i);
    }
    
    // Compute statistical uncertainties
    for (int i = 1; i <= h_dsigma_dt->GetNbinsX(); ++i) {
        double counts = mtgg->GetBinContent(i);
        double binWidth = h_dsigma_dt->GetBinWidth(i);
        double dsdt = counts / (lumi_gen * binWidth);
        //double dsdt = counts / (binWidth);
        double err = sqrt(counts) / (binWidth);  // Error propagation assuming Poisson stats
        h_dsigma_dt->SetBinContent(i, dsdt);
        h_dsigma_dt->SetBinError(i, err);
    }
    
    for (int i = 1; i <= h_dsigma_dt_rc->GetNbinsX(); ++i) {
        double counts = mtgTot->GetBinContent(i);
        double binWidth = h_dsigma_dt_rc->GetBinWidth(i);
        //double dsdt_rc = counts / ( binWidth);
        double dsdt_rc = counts / (lumi_gen * binWidth);
        //double err = sqrt(counts) / ( binWidth);  // Same here
        double err = sqrt(counts) / (lumi_gen * binWidth);  // Same here
        h_dsigma_dt_rc->SetBinContent(i, dsdt_rc);
        h_dsigma_dt_rc->SetBinError(i, err);
    }
    
    // Plot with error bars
    auto t_distribution_weight = new TCanvas("t_distribution_weight", "d #sigma/dt vs -t nb", 800, 800);
    t_distribution_weight->SetLogy();
    
    h_dsigma_dt->SetStats(0);
    //h_dsigma_dt->SetMarkerStyle(8);
    //h_dsigma_dt->SetMarkerColor(1);
    h_dsigma_dt->SetLineColor(1);
    h_dsigma_dt->SetLineWidth(3);
    h_dsigma_dt->GetXaxis()->SetRangeUser(0, 1.6);
    h_dsigma_dt->GetXaxis()->SetTitle("Momentum Transfer, -t [GeV^{2}]");
    h_dsigma_dt->GetYaxis()->SetTitle("d #sigma/dt [nb/GeV^{2}]");
    h_dsigma_dt->Draw("HIST");  // Draw with error bars
    
    h_dsigma_dt_rc->SetMarkerStyle(24);
    h_dsigma_dt_rc->SetMarkerColor(4);
    //h_dsigma_dt_rc->SetLineColor(3);
    h_dsigma_dt_rc->SetLineWidth(3);
    h_dsigma_dt_rc->Draw("E1 SAME");  // Also draw with error bars
    
    auto new_t_dist = new TLegend(0.4, 0.65, 0.89, 0.87);
    new_t_dist->SetBorderSize(0);
    //new_t_dist->AddEntry("", "lAger, 1<Q^{2}<10, 10 fb^{-1}", "");
    //new_t_dist->AddEntry("", "10x100 GeV, J/#psi #rightarrow e^{+}e^{-}", "");
    new_t_dist->AddEntry(h_dsigma_dt, "MC", "l");
    //new_t_dist->AddEntry(h_dsigma_dt, "p' MC", "l");
    new_t_dist->AddEntry(h_dsigma_dt_rc, "RECO corrected", " kOpenCircle");
    //new_t_dist->AddEntry(h_dsigma_dt_rc, "p' RECO (B0+RP)", "p");
    new_t_dist->Draw();
    
    TLegend *legp1 = new TLegend(0.5, 0.5, 0.88, 0.88); // x1,y1,x2,y2 (NDC coords)
    legp1->SetBorderSize(0);
    legp1->SetFillStyle(0);
    legp1->AddEntry(" ", "#bf{ePIC Performance}", " ");
    legp1->AddEntry(" ", "e+p DVMP J/#psi, #sqrt{s} #approx 72 GeV", " ");
    legp1->AddEntry(" ", "L_{proj}=10fb^{-1}, 25.06.1", " ");
    legp1->Draw();
    
    h_dsigma_dt->Write();
    h_dsigma_dt_rc->Write();
    h_dsigma_dt_test->Write();
    h_dsigma_dt_test_rc->Write();
    
    //Scaling the Luminosity of the background to the signal
    TH1D *t_mc_BKG_unscaled = (TH1D*) mtgg->Clone("t_mc_BKG_unscaled");
    TH1D *t_Lum_scaled_rc = (TH1D*) two_meth_tdist->Clone("t_Lum_scaled_rc");
    double lumi_gen_Lum = 782.1;
    for (int i = 1; i <= t_mc_BKG_unscaled->GetNbinsX(); ++i) {
        double counts = t_mc_BKG_unscaled->GetBinContent(i);
        double binWidth = t_mc_BKG_unscaled->GetBinWidth(i);
        //double binWidth = t_mc_BKG_unscaled->GetBinWidth(i);
        double dsdt = counts / (lumi_gen_Lum * binWidth);  // lumi_gen in nb^-1
        t_mc_BKG_unscaled->SetBinContent(i, dsdt);
        t_mc_BKG_unscaled->SetBinError(i, t_mc_BKG_unscaled->GetBinError(i) / binWidth);
    }
    for (int i = 1; i <= t_Lum_scaled_rc->GetNbinsX(); ++i) {
        double counts = t_Lum_scaled_rc->GetBinContent(i);
        double binWidth = t_Lum_scaled_rc->GetBinWidth(i);
        //double dsdt = counts / (lumi_gen_Lum * binWidth);  // lumi_gen in nb^-1
        double dsdt = counts / (binWidth);  // lumi_gen in nb^-1
        t_Lum_scaled_rc->SetBinContent(i, dsdt);
    }
    
    TH1D *t_Lum_scaled = (TH1D*) t_mc_BKG_unscaled->Clone("t_Lum_scaled");
    
    double Lum_Backgr = 7.821e-4;  // fb^-1
    double Lum_Signal = 10.0;   // fb^-1
    double Lum_scaleFactor = Lum_Signal / Lum_Backgr;
    cout << "Lum_scaling factor: " << Lum_scaleFactor << endl;
    
    // Now scale it
    t_Lum_scaled->Scale(Lum_scaleFactor);
    t_Lum_scaled_rc->Scale(Lum_scaleFactor);
    
    TCanvas* t_Lum_scaled_plot = new TCanvas("t_Lum_scaled_plot", "t_Lum_scaled_plot", 800, 800);
    t_Lum_scaled_plot->SetLogy();
    
    t_Lum_scaled->GetXaxis()->SetTitle("-t [GeV^{2}]");
    t_Lum_scaled->GetYaxis()->SetTitle("d #sigma/dt [nb/GeV^{2}]");
    t_Lum_scaled->SetStats(0);
    t_Lum_scaled->SetLineWidth(3);
    t_Lum_scaled->SetLineColor(1);
    t_Lum_scaled->Draw("HIST");
    
    t_Lum_scaled_rc->SetLineWidth(3);
    t_Lum_scaled_rc->SetLineColor(2);
    t_Lum_scaled_rc->Draw("HIST SAME");
    
    mtgg->Draw("HIST SAME");
    
    TLegend *t_Lum_scaled_leg = new TLegend(0.8, 0.8, 0.9, 0.9);
    t_Lum_scaled_leg->SetBorderSize(0);
    t_Lum_scaled_leg->AddEntry(t_Lum_scaled, "MC scaled", "l");
    t_Lum_scaled_leg->AddEntry(t_Lum_scaled_rc, "RECO scaled", "l");
    t_Lum_scaled_leg->AddEntry(mtgg, "MC Unscaled", "l");
    t_Lum_scaled_leg->Draw();
    
    t_Lum_scaled->Write();
    t_mc_BKG_unscaled->Write();
    t_Lum_scaled_rc->Write();
    //h_dsigma_dt_Lum_scaled->Write();
    
    //testing
    TCanvas *Testtheta = new TCanvas("Testtheta", "Testtheta_proton", 1400, 1200);
    //Inelasticity->SetLogy();
    Testtheta->Divide(2, 2);
    
    Testtheta->cd(1);
    Teta->GetXaxis()->SetTitle("#theta'");
    Teta->SetStats(0);
    Teta->SetLineColor(1);
    Teta->SetLineWidth(4);
    Teta->Draw();
    
    TetaAC->SetLineColor(4);
    TetaAC->SetLineWidth(4);
    TetaAC->Draw("SAME");
    
    TetaRP->SetLineColor(2);
    TetaRP->SetLineWidth(4);
    TetaRP->Draw("SAME");
    
    TetaB0->SetLineColor(3);
    TetaB0->SetLineWidth(4);
    TetaB0->Draw("SAME");
    
    TetaRPB0->SetLineColor(6);
    TetaRPB0->SetLineWidth(4);
    TetaRPB0->Draw("Same");
    
    // Legend for testing proton angle
    TLegend *tangle = new TLegend(0.8, 0.8, 0.9, 0.9);
    tangle->SetBorderSize(0);
    tangle->AddEntry(Teta, "#theta' MC", "l");
    tangle->AddEntry(TetaAC, "#theta' Accept", "l");
    tangle->AddEntry(TetaRP, "#theta' RP", "l");
    tangle->AddEntry(TetaB0, "#theta' B0", "l");
    tangle->AddEntry(TetaRPB0, "#theta' RP+B0", "l");
    tangle->Draw();
    
    Testtheta->cd(2);
    TetaRPB0->SetLineColor(6);
    TetaRPB0->SetStats(0);
    TetaRPB0->SetLineWidth(4);
    TetaRPB0->Draw();
    
    TLegend *tangle1 = new TLegend(0.8, 0.8, 0.9, 0.9);
    tangle1->SetBorderSize(0);
    tangle1->AddEntry(TetaRPB0, "#theta' RP+B0", "l");
    tangle1->Draw();
    
    Testtheta->cd(3);
    theta2D->SetStats(0);
    theta2D->GetXaxis()->SetTitle("#theta' (MC)");
    theta2D->GetYaxis()->SetTitle("#theta' (RECO)");
    theta2D->Draw("COLZ");
    
    TCanvas *Testtheta_e = new TCanvas("Testtheta_e", "Testtheta_electron", 1000, 600);
    Testtheta_e->Divide(2, 1);
    
    Testtheta_e->cd(1)->SetLogy();
    Teta_e->GetXaxis()->SetTitle("#theta");
    Teta_e->SetStats(0);
    Teta_e->SetLineColor(1);
    Teta_e->SetLineWidth(4);
    Teta_e->Draw();
    
    Teta_e_acc->SetLineColor(4);
    Teta_e_acc->SetLineWidth(4);
    Teta_e_acc->Draw("SAME");
    
    Teta_e_rc->SetLineColor(2);
    Teta_e_rc->SetLineWidth(4);
    Teta_e_rc->Draw("SAME");
    
    // Legend for testing proton angle
    TLegend *tangle_e = new TLegend(0.8, 0.8, 0.9, 0.9);
    tangle_e->SetBorderSize(0);
    tangle_e->AddEntry(Teta_e, "#theta' MC", "l");
    tangle_e->AddEntry(Teta_e_acc, "#theta' Accept", "l");
    tangle_e->AddEntry(Teta_e_rc, "#theta' Reco", "l");
    tangle_e->Draw();
    
    Testtheta_e->cd(2);
    theta2D_e->SetStats(0);
    theta2D_e->GetXaxis()->SetTitle("#theta' (Accept)");
    theta2D_e->GetYaxis()->SetTitle("#theta' (RECO)");
    theta2D_e->Draw("COLZ");
    
    TCanvas* Purity_e = new TCanvas("Purity_e", "Purity", 800, 600);
    h_purity->SetMinimum(0);
    h_purity->SetStats(0);
    h_purity->SetMaximum(1.1);  // max a bit above 1 for clarity
    h_purity->SetLineColor(1);
    h_purity->SetLineWidth(4);
    h_purity->Draw("HIST");
    
    h_purity_JB->SetLineColor(2);
    h_purity_JB->SetLineWidth(4);
    h_purity_JB->Draw("Same");
    
    h_purity_eS->SetLineColor(4);
    h_purity_eS->SetLineWidth(4);
    h_purity_eS->Draw("Same");
    
    TLegend *purityLeg = new TLegend(0.8, 0.8, 0.9, 0.9);
    purityLeg->SetBorderSize(0);
    purityLeg->AddEntry(h_purity, "Electron Method", "l");
    purityLeg->AddEntry(h_purity_JB, "JB Method", "l");
    purityLeg->AddEntry(h_purity_eS, "e#sigma Method", "l");
    purityLeg->Draw();
    
    TCanvas* Staability_e = new TCanvas("Staability_e", "Stability", 800, 600);
    h_stability->SetMinimum(0);
    h_stability->SetStats(0);
    h_stability->SetMaximum(1.1);
    h_stability->SetLineColor(1);
    h_stability->SetLineWidth(4);
    h_stability->Draw("HIST");
    
    h_stability_JB->SetLineColor(2);
    h_stability_JB->SetLineWidth(4);
    h_stability_JB->Draw("Same");
    
    h_stability_eS->SetLineColor(4);
    h_stability_eS->SetLineWidth(4);
    h_stability_eS->Draw("Same");
    
    TLegend *stabilityLeg = new TLegend(0.8, 0.8, 0.9, 0.9);
    stabilityLeg->SetBorderSize(0);
    stabilityLeg->AddEntry(h_stability, "Electron Method", "l");
    stabilityLeg->AddEntry(h_stability_JB, "JB Method", "l");
    stabilityLeg->AddEntry(h_stability_eS, "e#sigma Method", "l");
    stabilityLeg->Draw();
    
    auto q2_vs_x = new TCanvas("q2_vs_x", " q2_v_x", 1200, 600);
    q2_vs_x->Divide(2,1);
    
    q2_vs_x->cd(1)->SetLogy();
    q2_vs_x->cd(1)->SetLogx();
    q2_v_x_e->SetStats(0);
    q2_v_x_e->GetXaxis()->SetTitle("x_{bj} - MC");
    q2_v_x_e->GetYaxis()->SetTitle("Q^{2} - MC");
    q2_v_x_e->Draw("COLZ");
    
    q2_vs_x->cd(2)->SetLogy();
    q2_vs_x->cd(2)->SetLogx();
    q2_v_x_e_rc->SetStats(0);
    q2_v_x_e_rc->GetXaxis()->SetTitle("x_{bj} - Reco");
    q2_v_x_e_rc->GetYaxis()->SetTitle("Q^{2} - Reco");
    q2_v_x_e_rc->Draw("COLZ");
    
    auto energy2D = new TCanvas("energy2D", " energy2D", 800, 800);
    energy_2D->SetStats(0);
    energy_2D->GetXaxis()->SetTitle("Energy(MC)");
    energy_2D->GetYaxis()->SetTitle("Energy(Reco)");
    energy_2D->Draw("COLZ");
    
    TCanvas* t_dist_another = new TCanvas("t_dist_another", "t_dist_another", 1200, 600);
    t_dist_another->Divide(2,1);
    
    t_dist_another->cd(1)->SetLogy();
    t_anotherhist->SetLineColor(1);
    t_anotherhist->SetLineWidth(4);
    t_anotherhist->SetStats(0);
    t_anotherhist->Draw();
    
    t_anotherhist_acc->SetLineColor(4);
    t_anotherhist_acc->SetLineWidth(4);
    t_anotherhist_acc->Draw("Same");
    
    t_anotherhist_rc->SetLineColor(2);
    t_anotherhist_rc->SetLineWidth(4);
    t_anotherhist_rc->Draw("Same");
    
    TLegend *t_methodL_Leg = new TLegend(0.8, 0.8, 0.9, 0.9);
    t_methodL_Leg->SetBorderSize(0);
    t_methodL_Leg->AddEntry(t_anotherhist, "MC", "l");
    t_methodL_Leg->AddEntry(t_anotherhist_acc, "Accept", "l");
    t_methodL_Leg->AddEntry(t_anotherhist_rc, "Reco", "l");
    t_methodL_Leg->Draw();
    
    t_dist_another->cd(2);
    t_another_2D->SetStats(0);
    t_another_2D->GetXaxis()->SetTitle("t (Accept)");
    t_another_2D->GetYaxis()->SetTitle("t (Reco)");
    t_another_2D->Draw("COLZ");
    
    TCanvas* q2_vs_tMl = new TCanvas("q2_vs_tMl", "q2_vs_tMl", 1200, 600);
    q2_vs_tMl->Divide(2,1);
    
    q2_vs_tMl->cd(1);
    q2vst->SetStats(0);
    q2vst->GetXaxis()->SetTitle("t (MC)");
    q2vst->GetYaxis()->SetTitle("Q^{2} (MC)");
    q2vst->Draw("COLZ");
    
    q2_vs_tMl->cd(2);
    q2vst_acc->SetStats(0);
    q2vst_acc->GetXaxis()->SetTitle("t (Accept)");
    q2vst_acc->GetYaxis()->SetTitle("Q^{2} (Accept)");
    q2vst_acc->Draw("COLZ");
    
    cout << " " << endl;
    
    TH1D* h_CorrectionFactor = (TH1D*) mtgg->Clone("h_CorrectionFactor");
    h_CorrectionFactor->SetTitle("Correction Factor c_f = MC / Acceptance");
    h_CorrectionFactor->Divide(two_meth_tdist_accept);  // c_f = mc / acc
    
    // Step 2: apply correction → corrected = reco * c_f
    TH1D* t_from_2_method_corrected = (TH1D*) two_meth_tdist->Clone("t_from_2_method_corrected");
    t_from_2_method_corrected->Multiply(h_CorrectionFactor);
    
    TCanvas* t_from_2_method = new TCanvas("t_from_2_method", "t_from_2_method", 800, 800);
    t_from_2_method->SetLogy();
    
    TRatioPlot* tall = new TRatioPlot(two_meth_tdist, mtgg, "pois");
    tall->Draw("AL");
    tall->SetSeparationMargin(0);
    
    tall->GetLowerRefGraph()->SetMinimum(0);
    tall->GetLowerRefGraph()->SetMaximum(1.5);
    tall->GetLowerRefGraph()->SetMarkerStyle(8);
    tall->GetLowerRefGraph()->SetMarkerColor(kBlack);
    tall->GetLowerRefYaxis()->SetTitle("Ratio");
    tall->GetLowerRefYaxis()->CenterTitle();
    
    // Format upper (main) pad
    tall->GetUpperPad()->cd();
    mtgg->Draw();
    
    two_meth_tdist->SetLineWidth(3);
    two_meth_tdist->SetLineColor(2);
    two_meth_tdist->GetXaxis()->SetTitle("-t [GeV^{2}]");
    two_meth_tdist->GetYaxis()->SetTitle("Counts");
    two_meth_tdist->Draw("SAME");
    
    two_meth_tdist_accept->SetLineWidth(3);
    two_meth_tdist_accept->SetLineColor(4);
    two_meth_tdist_accept->GetXaxis()->SetTitle("-t [GeV^{2}]");
    two_meth_tdist_accept->GetYaxis()->SetTitle("Counts");
    two_meth_tdist_accept->Draw("SAME");
    
    TLegend *t_many_method = new TLegend(0.8, 0.8, 0.9, 0.9);
    t_many_method->SetBorderSize(0);
    t_many_method->AddEntry(mtgg, "Truth MC", "l");
    t_many_method->AddEntry(two_meth_tdist_accept, "Accept", "l");
    t_many_method->AddEntry(two_meth_tdist, "RECO", "l");
    t_many_method->Draw();
    
    auto checking_t = new TCanvas("checking_t", " checking_t", 1000, 1000);
    checking_t->Divide(2,2);
    
    checking_t->cd(1);//->SetLogy();
    t2D_with_diff_rp->SetStats(1);
    t2D_with_diff_rp->GetXaxis()->SetTitle("MC Truth");
    t2D_with_diff_rp->GetYaxis()->SetTitle("RP");
    t2D_with_diff_rp->Draw("COLZ");
    
    checking_t->cd(2);//->SetLogy();
    t2D_with_diff_methL->SetStats(1);
    t2D_with_diff_methL->GetXaxis()->SetTitle("MC Truth");
    t2D_with_diff_methL->GetYaxis()->SetTitle("Method L");
    t2D_with_diff_methL->Draw("COLZ");
    
    checking_t->cd(3);//->SetLogy();
    t2D_with_diff_B0->SetStats(1);
    t2D_with_diff_B0->GetXaxis()->SetTitle("MC Truth");
    t2D_with_diff_B0->GetYaxis()->SetTitle("B0");
    t2D_with_diff_B0->Draw("COLZ");
    
    checking_t->cd(4);//->SetLogy();
    t2D_with_diff_allCroll->SetStats(1);
    t2D_with_diff_allCroll->GetXaxis()->SetTitle("MC Truth");
    t2D_with_diff_allCroll->GetYaxis()->SetTitle("RECO");
    t2D_with_diff_allCroll->Draw("COLZ");
    
    TCanvas* ep_test = new TCanvas("ep_test", "ep_test", 800, 800);
    ep_test->Divide(2,2);
    
    ep_test->cd(1);
    h_missingMass->SetLineWidth(3);
    h_missingMass->SetLineColor(1);
    h_missingMass->Draw();
    
    ep_test->cd(2);
    h_missingMass_rc->SetLineWidth(3);
    h_missingMass_rc->SetLineColor(1);
    h_missingMass_rc->Draw();
    
    ep_test->cd(3);
    h_missingMass2->SetLineWidth(3);
    h_missingMass2->SetLineColor(1);
    h_missingMass2->Draw();
    
    ep_test->cd(4);
    h_missingMass2_rc->SetLineWidth(3);
    h_missingMass2_rc->SetLineColor(1);
    h_missingMass2_rc->Draw();
    
    /*TCanvas* MC_all_final_state = new TCanvas("MC_all_final_state", "MC_all_final_state", 800, 800);
    
    AllMCFinalEta->SetStats(0);
    AllMCFinalEta->GetYaxis()->SetTitle("Counts");
    AllMCFinalEta->SetLineColor(kBlue);
    AllMCFinalEta->GetYaxis()->SetRangeUser(0, 50000);
    AllMCFinalEta->SetLineWidth(4);
    AllMCFinalEta->Draw();
    fElectron->Draw("Same");
    hjpsi_eta_MC->SetLineColor(kRed);
    hjpsi_eta_MC->Draw("Same");
    fPositron->SetLineColor(kViolet);
    fPositron->Draw("Same");
    
    TLegend *MCAllFinalState = new TLegend(0.8, 0.8, 0.9, 0.9);
    MCAllFinalState->SetBorderSize(0);
    MCAllFinalState->AddEntry(AllMCFinalEta, "Proton", "l");
    MCAllFinalState->AddEntry(fElectron, "SC. Electron", "l");
    MCAllFinalState->AddEntry(hjpsi_eta_MC, "DC. Electron", "l");
    MCAllFinalState->AddEntry(fPositron, "Positron", "l");
    MCAllFinalState->Draw();*/
    
    TCanvas *E_P_final_DCePositron = new TCanvas("E_P_final_DCePositron","E_P_final_DCePositron",1800,600);
    E_P_final_DCePositron->Divide(3,1);
    
    E_P_final_DCePositron->cd(1);
    E_P_final_SCe_MC->GetXaxis()->SetTitle("E/|P|");
    E_P_final_SCe_MC->GetXaxis()->SetRangeUser(-1,3);
    E_P_final_SCe_MC->SetStats(0);
    E_P_final_SCe_MC->SetLineColor(1);
    E_P_final_SCe_MC->SetLineWidth(4);
    E_P_final_SCe_MC->Draw();
    
    E_P_final_SCe_RC->SetLineColor(kRed);
    E_P_final_SCe_RC->SetLineWidth(4);
    E_P_final_SCe_RC->Draw("SAME");
    
    TLegend *ScatElect = new TLegend(0.8, 0.8, 0.9, 0.9);
    ScatElect->SetBorderSize(0);
    ScatElect->AddEntry(E_P_final_SCe_MC, "SC. Electron (MC)", "l");
    ScatElect->AddEntry(E_P_final_SCe_RC, "SC. Electron (RECO)", "l");
    ScatElect->Draw();
    
    E_P_final_DCePositron->cd(2);
    E_P_final_DCe_MC->GetXaxis()->SetTitle("E/|P|");
    E_P_final_DCe_MC->GetXaxis()->SetRangeUser(-1,3);
    E_P_final_DCe_MC->SetStats(0);
    E_P_final_DCe_MC->SetLineColor(1);
    E_P_final_DCe_MC->SetLineWidth(4);
    E_P_final_DCe_MC->Draw();
    
    E_P_final_DCe_RC->SetLineColor(kRed);
    E_P_final_DCe_RC->SetLineWidth(4);
    E_P_final_DCe_RC->Draw("SAME");
    
    TLegend *DecayElect = new TLegend(0.8, 0.8, 0.9, 0.9);
    DecayElect->SetBorderSize(0);
    DecayElect->AddEntry(E_P_final_DCe_MC, "DC. Electron (MC)", "l");
    DecayElect->AddEntry(E_P_final_DCe_RC, "DC. Electron (RECO)", "l");
    DecayElect->Draw();
    
    E_P_final_DCePositron->cd(3);
    E_P_final_Posit_MC->GetXaxis()->SetTitle("E/|P|");
    E_P_final_Posit_MC->GetXaxis()->SetRangeUser(-1,3);
    E_P_final_Posit_MC->SetStats(0);
    E_P_final_Posit_MC->SetLineColor(1);
    E_P_final_Posit_MC->SetLineWidth(4);
    E_P_final_Posit_MC->Draw();
    
    E_P_final_Posit_RC->SetLineColor(kRed);
    E_P_final_Posit_RC->SetLineWidth(4);
    E_P_final_Posit_RC->Draw("SAME");
    
    TLegend *Final_Posit = new TLegend(0.8, 0.8, 0.9, 0.9);
    Final_Posit->SetBorderSize(0);
    Final_Posit->AddEntry(E_P_final_Posit_MC, "Final Positron (MC)", "l");
    Final_Posit->AddEntry(E_P_final_Posit_RC, "Final Positron (RECO)", "l");
    Final_Posit->Draw();
    
    // Target cross section in barns
    double CrosSec_mc = mtgg->GetEntries() * 1e-7 ;
    //double CrosSec_rc = mtgTot->GetEntries() * 1e-7;
    double CrosSec_rc = t_from_2_method_corrected->GetEntries() * 1e-7;
    cout << " " << endl;
    std::cout << "MC #sigma = " << CrosSec_mc <<" nb"<< std::endl;
    std::cout << "RECO #sigma = " << CrosSec_rc <<" nb"<< std::endl;
    
    h_missingMass->Write();
    h_missingMass_rc->Write();
    two_meth_tdist->Write();
    two_meth_tdist_accept->Write();
    h_CorrectionFactor->Write();
    t_from_2_method_corrected->Write();
    
    outputFile->Close();
    
    cout << " " << endl;
    cout << " " << endl;
    cout << "------------------------------------------- " << endl;
    cout << "   SOME USEFUL EVENTS ANALYSIS  " << endl;
    cout << "------------------------------------------- " << endl;
    cout << "   ------- " << endl;
    cout << "   ------- " << endl;
    cout << "Total Number Of Events: " << z0 <<endl;
    cout << "Total Number Of Events in RP Tracks: " << z1 <<endl;
    cout << "Total Number Of Events in B0: " << z2 << endl;
    cout << "MC electrons: " << z3 << endl;
    cout << "RECo electrons: " << z4 << endl;
    cout << "MC J/#psi " << mcj << endl;
    cout << "RECO J/#psi " << rcj << endl;
    cout << "t_distribution  " << t_di << endl;
    
    return;
    
}
