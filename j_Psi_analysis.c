//Code written by Olaiya Olokunboyo, University of New Hampshire, for
//generating gluons distribution and necessary kinematics.
//To run on the default input file (streaming files from server), make sure you are in the eic-shell environment.
//May.21.2025


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

using std::cout;
using std::endl;
using std::ifstream;
using std::string;

void j_Psi_analysis() {
    //TString fileList = "q2_1-10.list";
    //TString fileList = "Jan_2025_1.list"; // local file
    TString fileList = "April_2025_1.list"; // stream
    
    // Output and run configuration
    TString outputName   = "benchmark_data_10x100_";
    TString date         = "May_2025_";
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
    
    //DEfining the four-vectors
    TLorentzVector vec1, vec2, vec3, vec4, vec5, vec6, vecTot, accept1;
    TLorentzVector vecjpsi, vec3mc, vecjpsimc, vec3ck, vecjpsick;
    TLorentzVector JPsi1, JPsi2, JPsi22, JPsi222;
    TLorentzVector rcpv1, rcpv2;
    TLorentzVector incbeam, incbeame, eq1rc, rcpv1rc, eq1ck, rcpv1ck;
    TLorentzVector eq1, iphoton, ijpsi, vgen, vrecop;
    
    // Electrons histograms
    TH1D *fElectron = new TH1D("fElectron","Final/Scattered Electron", 100, -10, 10);
    TH1D *ffElectron = new TH1D("ffElectron"," ", 100, -10, 10);
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
    
    //Resolution/differnces
    TH1D *diffjpsimass = new TH1D("diffjpsimass"," ", 500, -5, 5);
    TH1D *diffx_v = new TH1D("diffx_v"," ", 500, -5, 5);
    TH1D *diffxbjk = new TH1D("diffxbjk"," ", 500, -5, 5);
    TH1D *difft = new TH1D("difft"," ", 500, -5, 5);
    TH1D *difftTot = new TH1D("difftTot"," ", 500, -5, 5);
    TH1D *diffq2 = new TH1D("diffq2"," ", 500, -5, 5);
    TH1D *diffq2_2 = new TH1D("diffq2_2"," ", 500, -5, 5);
    TH1D *diffy = new TH1D("diffy"," ", 500, -5, 5);
    TH1D *diff_eSig_q2 = new TH1D("diff_eSig_q2"," ", 500, -5, 5);
    TH1D *diff_eSig_xbjk = new TH1D("diff_eSig_xbjk"," ", 500, -5, 5);
    TH1D *diff_eSig_y = new TH1D("diff_eSig_y"," ", 500, -5, 5);
    TH1D *diff_JB_q2 = new TH1D("diff_JB_q2"," ", 500, -5, 5);
    TH1D *diff_JB_y = new TH1D("diff_JB_y"," ", 500, -5, 5);
    TH1D *diff_JB_xbjk = new TH1D("diff_JB_xbjk"," ", 500, -5, 5);
    
    //Missing Transverse Quantities
    TH1D *mtg = new TH1D("mtg","", 200, 0, 5);
    TH1D *mtgB0 = new TH1D("mtgB0","", 200, 0, 5);
    TH1D *mtgTot = new TH1D("mtgTot","", 200, 0, 5);
    TH1D *mtgTot1 = new TH1D("mtgTot1","", 200, 0, 5);
    TH1D *mtgg = new TH1D("mtgg","", 200, 0, 5);
    TH1D *mtgg_accept = new TH1D("mtgg_accept","", 200, 0, 5);
    TH1D *mtq = new TH1D("mtq","", 50, 0, 11);
    TH1D *mtqq = new TH1D("mtqq","", 50, 0, 11);
    TH1D *mtqq_2 = new TH1D("mtqq_2","", 50, 0, 11);
    
    //eSigma Method Kinematics
    TH1D *eSig_y_MC = new TH1D("eSig_y_MC","", 100,-0.01,1.05);
    TH1D *eSig_y_MC_a = new TH1D("eSig_y_MC_a","", 100,-0.01,1.05);
    TH1D *eSig_y_RC = new TH1D("eSig_y_RC","", 100,-0.01,1.05);
    TH1D *eSig_q2_MC = new TH1D("eSig_q2_MC","", 50, 0, 11);
    TH1D *eSig_q2_RC = new TH1D("eSig_q2_RC","", 50, 0, 11);
    TH1D *eSig_x_MC = new TH1D("eSig_x_MC","", 4000, -0.01, 1.01);
    TH1D *eSig_x_RC = new TH1D("eSig_x_RC","", 4000, -0.01, 1.01);
    
    //Jacquet-Blondel (JB) Method Kinematics
    TH1D *JB_pt2_MC = new TH1D("JB_pt2_MC","", 100, 0, 60);
    TH1D *JB_HFS_MC = new TH1D("JB_HFS_MC","", 100, 0, 60);
    TH1D *JB_y_MC = new TH1D("JB_y_MC","", 100,-0.01,1.05);
    TH1D *JB_y_MC_a = new TH1D("JB_y_MC_a","", 100,-0.01,1.05);
    TH1D *JB_y_RC = new TH1D("JB_y_RC","", 100,-0.01,1.05);
    TH1D *JB_q2_MC = new TH1D("JB_q2_MC","", 50, 0, 11);
    TH1D *JB_q2_RC = new TH1D("JB_q2_RC","", 50, 0, 11);
    TH1D *JB_x_MC = new TH1D("JB_x_MC","", 4000, -0.01, 1.01);
    TH1D *JB_x_RC = new TH1D("JB_x_RC","", 4000, -0.01, 1.01);
    
    //Inclusive Variables
    TH1D *EM_x = new TH1D("EM_x","", 4000, -0.01, 1.01);
    TH1D *EM_Q2 = new TH1D("EM_Q2","", 50, 0, 11);
    TH1D *EM_y = new TH1D("EM_y","", 100,-0.01,1.05);
    TH1D *ESig_x_in = new TH1D("ESig_x_in","", 4000, -0.01, 1.01);
    TH1D *ESig_Q2_in = new TH1D("ESig_Q2_in","", 50, 0, 11);
    TH1D *ESig_y_in = new TH1D("ESig_y_in","", 100,-0.01,1.05);
    TH1D *JB_x_in = new TH1D("JB_x_in","", 4000, -0.01, 1.01);
    TH1D *JB_Q2_in = new TH1D("JB_Q2_in","", 50, 0, 11);
    TH1D *JB_y_in = new TH1D("JB_y_in","", 100,-0.01,1.05);
    
    //x-Variables
    TH1D *xb = new TH1D("xb","",  4000, -0.01, 1.01);
    TH1D *xba = new TH1D("xba","",  4000, -0.01, 1.01);
    TH1D *xb1 = new TH1D("xb1","",  4000, -0.01, 1.01);
    TH1D *xb2 = new TH1D("xb2","",  4000, -0.01, 1.01);
    TH1D *xv = new TH1D("xv","",  4000, -0.01, 1.01);
    TH1D *xv1 = new TH1D("xv1","",  4000, -0.01, 1.01);
    TH1D *xv2 = new TH1D("xv2","",  4000, -0.01, 1.01);
    
    //eta/pt and energy_vs_pz
    TH1D *ecaleta = new TH1D("ecaleta","ecaleta", 100, -6, 6);
    TH1D* rcElectronpt = new TH1D("rcElectronpt", ";p_{t} [GeV/c]", 100, -10, 10.0);
    TH1D* rcPositronpt = new TH1D("rcPositronpt", ";p_{t} [GeV/c]", 100, -10, 10.0);
    TH1D* rcElectronjpsipt = new TH1D("rcElectronjpsipt", ";p_{t} [GeV/c]", 100, -10,10.0);
    TH1D* rcElectronjpsieta = new TH1D("rcElectronjpsieta",";Pseudorapidity, #eta",100,-10,10.0);
    TH1D* mcjpsie_e_minus_pzmc = new TH1D("mcjpsie_e_minus_pzmc"," ",500, -50, 150.0);
    TH1D* rcjpsie_e_minus_pz = new TH1D("rcjpsie_e_minus_pz","rcjpsie_e_minus_pz",500, -50, 150.0);
    
    //2D histograms
    TH2D* q2_2D = new TH2D("q2_2D","q2_2D", 400, 0, 10, 400, 0, 10);
    TH2D* xbjk_2D = new TH2D("xbjk_2D","xbjk_2D", 400, 0, 0.5, 400, 0, 0.5);
    TH2D* y_2D = new TH2D("y_2D","y_2D", 400, 0, 1, 400, 0, 1);
    TH2D* t_2D = new TH2D("t_2D","t_2D", 400, 0, 2, 400, 0, 2);
    TH2D* dxbjk_2D = new TH2D("dxbjk_2D","dxbjk_2D", 400, 0, 10, 400, -1, 1);
    TH2D* eEta2D = new TH2D("eEta2D","eEta2D", 200, -4, 4, 400, -4, 4);
    TH2D* pEta2D = new TH2D("pEta2D","pEta2D", 200, -4, 4, 400, -4, 4);
    
    //eSigma 2D
    TH2D* eSig_q2_2D = new TH2D("eSig_q2_2D"," ", 400, 0, 10, 400, 0, 10);
    TH2D* deSig_q2_2D = new TH2D("deSig_q2_2D"," ", 400, 0, 10, 400, -1, 1);
    TH2D* eSig_y_2D = new TH2D("eSig_y_2D"," ", 400, 0, 1, 400, 0, 1);
    TH2D* deSig_y_2D = new TH2D("deSig_y_2D"," ", 400, 0, 1, 400, -1, 1);
    TH2D* eSig_xbjk_2D = new TH2D("eSig_xbjk_2D"," ", 400, 0, 0.5, 400, 0, 0.5);
    TH2D* deSig_xbjk_2D = new TH2D("deSig_xbjk_2D"," ", 400, 0, 0.5, 400, -1, 1);
    
    //JB
    TH2D* JB_q2_2D = new TH2D("JB_q2_2D"," ", 400, 0, 10, 400, 0, 10);
    TH2D* dJB_q2_2D = new TH2D("dJB_q2_2D"," ", 400, 0, 10, 400, -1, 1);
    TH2D* JB_y_2D = new TH2D("JB_y_2D"," ", 400, 0, 1, 400, 0, 1);
    TH2D* dJB_y_2D = new TH2D("dJB_y_2D"," ", 400, 0, 1, 400, -1, 1);
    TH2D* JB_xbjk_2D = new TH2D("JB_xbjk_2D"," ", 400, 0, 0.5, 400, 0, 0.5);
    TH2D* dJB_xbjk_2D = new TH2D("dJB_xbjk_2D"," ", 400, 0, 0.5, 400, -1, 1);
    
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
    
    TH1D* h2_eta_MC = new TH1D("h2_eta",";Pseudorapidity, #eta",100,-15,15.0);
    TH1D* h2_px_MC = new TH1D("px2_MC", ";p_{x} [GeV/c]", 100, -10.0, 10.0);
    TH1D* h2_py_MC = new TH1D("py2_MC", ";p_{y} [GeV/c]", 100, -10.0, 10.0);
    TH1D* h2_pt_MC = new TH1D("pt2_MC", ";p_{t} [GeV/c]", 100, 0.0, 10.0);
    TH1D* h2_e_MC = new TH1D("e2_MC", ";E [GeV]", 200, -20, 20.0);
    
    TH1D* scat_e_RC = new TH1D("scat_e_RC", ";E [GeV]", 200, -20, 20.0);
    TH1D* scat_e_EcalEnd = new TH1D("scat_e_EcalEnd", ";E [GeV]", 200, -20, 20.0);
    TH1D* Ecal_vs_pz = new TH1D("Ecal_vs_pz", ";E/P", 200, -5, 5.0);
    
    //Roman pots
    TH1D* h_px_RomanPots = new TH1D("px_RomanPots", ";p_{x} [GeV/c]", 100, -10.0, 10.0);
    TH1D* h_py_RomanPots = new TH1D("py_RomanPots", ";p_{y} [GeV/c]", 100, -10.0, 10.0);
    TH1D* h_pt_RomanPots = new TH1D("pt_RomanPots", ";p_{t} [GeV/c]", 100, -1, 2.0);
    TH1D* h_pt_B0RomanPots = new TH1D("h_pt_B0RomanPots", ";p_{t} [GeV/c]", 100, -1, 2.0);
    TH1D* h_pz_RomanPots = new TH1D("pz_RomanPots", ";p_{z} [GeV/c]", 100, 0.0, 320.0);
    TH1D* h_e_RomanPots = new TH1D("e_RomanPots", ";E [GeV]", 100, 0.0, 320.0);
    TH2D* h_rp_occupancy_map = new TH2D("Roman_pots_occupancy_map", "hit y [mm];hit x [mm]", 100, -150, 150, 100, -70, 70);
    
    
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
    
    
    // File and event counters
    int fileCounter = 0;
    int iEvent = 0;
    int iflagproton = 0, iflagproton2 = 0;
    int ic = 2;
    
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
    
    // Weights and total momentum
    double tot_p = 0;
    double weight = 0, weight1 = 0, weight2 = 0;
    
    // Luminosity in nb^-1 (10 fb^-1)
    const double lumi_gen = 1.0e7;
    
    while (getline(fileListStream, fileName)) {
        TString tmpFileName = fileName;
        cout << "Input file " << fileCounter << ": " << tmpFileName << endl;
        
        auto inputRootFile = std::unique_ptr<TFile>(TFile::Open(tmpFileName));
        if (!inputRootFile) {
            cerr << "ERROR: Missing ROOT file: " << tmpFileName << endl;
            continue;
        }
        
        fileCounter++;
        
        TTree* evtTree = static_cast<TTree*>(inputRootFile->Get("events"));
        if (!evtTree) {
            cerr << "ERROR: Could not find 'events' tree in " << tmpFileName << endl;
            continue;
        }
        
        int numEvents = evtTree->GetEntries();
        cout << "File contains " << numEvents << " events." << endl;
        
        TTreeReader treeReader(evtTree);
        
        // MC particles
        TTreeReaderArray<float> mc_px(treeReader, "MCParticles.momentum.x");
        TTreeReaderArray<float> mc_py(treeReader, "MCParticles.momentum.y");
        TTreeReaderArray<float> mc_pz(treeReader, "MCParticles.momentum.z");
        TTreeReaderArray<double> mc_mass(treeReader, "MCParticles.mass");
        TTreeReaderArray<int> mc_pdg(treeReader, "MCParticles.PDG");
        
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
        
        // B0 Tracker
        TTreeReaderArray<float> b0_hit_x(treeReader, "B0TrackerRecHits.position.x");
        TTreeReaderArray<float> b0_hit_y(treeReader, "B0TrackerRecHits.position.y");
        TTreeReaderArray<float> b0_hit_z(treeReader, "B0TrackerRecHits.position.z");
        TTreeReaderArray<float> b0_hit_eDep(treeReader, "B0TrackerRecHits.edep");
        
        // B0 EMCAL
        TTreeReaderArray<float> b0_cluster_x(treeReader, "B0ECalClusters.position.x");
        TTreeReaderArray<float> b0_cluster_y(treeReader, "B0ECalClusters.position.y");
        TTreeReaderArray<float> b0_cluster_z(treeReader, "B0ECalClusters.position.z");
        TTreeReaderArray<float> b0_cluster_energy(treeReader, "B0ECalClusters.energy");
        
        // Reconstructed tracks
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
        
        TTreeReaderArray<float> ecalN_hit_x(treeReader, "EcalEndcapNRecHits.position.x");
        TTreeReaderArray<float> ecalN_hit_y(treeReader, "EcalEndcapNRecHits.position.y");
        TTreeReaderArray<float> ecalN_hit_energy(treeReader, "EcalEndcapNRecHits.energy");
        
        // Barrel ECAL
        TTreeReaderArray<float> ecalBarrelScFi_energy(treeReader, "EcalBarrelScFiClusters.energy");
        TTreeReaderArray<float> ecalBarrelScFi_theta(treeReader, "EcalBarrelScFiClusters.intrinsicTheta");
        TTreeReaderArray<float> ecalBarrelImaging_energy(treeReader, "EcalBarrelImagingClusters.energy");
        TTreeReaderArray<float> ecalBarrelImaging_theta(treeReader, "EcalBarrelImagingClusters.intrinsicTheta");
        
        // Generated Particles
        TTreeReaderArray<float> gen_px(treeReader, "GeneratedParticles.momentum.x");
        TTreeReaderArray<float> gen_py(treeReader, "GeneratedParticles.momentum.y");
        TTreeReaderArray<float> gen_pz(treeReader, "GeneratedParticles.momentum.z");
        TTreeReaderArray<float> gen_mass(treeReader, "GeneratedParticles.mass");
        TTreeReaderArray<int>   gen_pdg(treeReader, "GeneratedParticles.PDG");
        TTreeReaderArray<float> gen_charge(treeReader, "GeneratedParticles.charge");
        
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
        
        // Set the range of events to read
        treeReader.SetEntriesRange(0, numEvents);
        
        while (treeReader.Next()) {
            
            //for the incoming electron
            incbeame.SetXYZM(mc_px[0], mc_py[0], mc_pz[0],mc_mass[0]);
            
            //for the incoming proton with 25mrad applied crossing angle
            incbeam.SetXYZM(mc_px[3], mc_py[3], mc_pz[3],mc_mass[3]);
            incbeam.RotateY(0.025);
            
            TLorentzVector mctrk4q2;
            
            //start of prequesite conditions confining q_squared between 1 and 10
            mctrk4q2.SetXYZM(mc_px[ic], mc_py[ic],mc_pz[ic],mc_mass[ic]);
            
            
            h2_eta_MC->Fill(mctrk4q2.Eta());
            h2_e_MC->Fill(mctrk4q2.E());
            
            //for mediating photon
            iphoton.SetXYZM(mc_px[1], mc_py[1], mc_pz[1],mc_mass[1]);
            
            //for jpsi
            ijpsi.SetXYZM(mc_px[5], mc_py[5], mc_pz[5],mc_mass[5]);
            
            vec2.SetXYZM(mc_px[7], mc_py[7], mc_pz[7],mc_mass[7]);
            
            vec1.SetXYZM(mc_px[6], mc_py[6], mc_pz[6],mc_mass[6]);
            
            //for the jpsi electron
            vecjpsick.SetXYZM(mc_px[8], mc_py[8], mc_pz[8],mc_mass[8]);
            
            //for the scattered/ final electron
            vec3ck.SetXYZM(mc_px[2], mc_py[2], mc_pz[2],mc_mass[2]);
            
            //using the momentum for further remove impurities from the scattered electrons
            rap = (incbeam.Dot(incbeame - vec3ck)) / (incbeam.Dot(incbeame));
            
            //applying the condition that 0.1<y<1
            if ((vec3ck.Pz() < vecjpsick.Pz()) && rap > 0.1 && rap < 1){
                vec3 = vec3ck;
                vecjpsi = vecjpsick;
                mcrapid->Fill(rap);
            }
            else {
            continue;
            }
            
            //applying the condition 1<q2<10
            thetaMC = vec3.Theta();
            thetaMC = thetaMC - TMath::Pi();
            q2 = 4 * incbeame.E() * vec3.E() * TMath::Sin(thetaMC/2.0) * TMath::Sin(thetaMC/2.0);
            if (q2 < 1 && q2 > 10) {
                    continue;
                }
            mtq->Fill(q2);
            
            //inclusive info for electron, eSig and JB methods
            for (int iEMx = 0; iEMx < ik_electron_x.GetSize(); iEMx++){
                EM_x_value = ik_electron_x[iEMx];
                EM_Q2_value = ik_electron_Q2[iEMx];
                EM_y_value = ik_electron_y[iEMx];
                if (EM_x_value > 0){
                    EM_x->Fill(EM_x_value);
                }
                if (EM_Q2_value > 0){
                    EM_Q2->Fill(EM_Q2_value);
                }
                if (EM_y_value > 0){
                    EM_y->Fill(EM_y_value);
                }
            }
            for (int iES = 0; iES < ik_esigma_x.GetSize(); iES++){
                ESig_x_in_value = ik_esigma_x[iES];
                ESig_Q2_in_value = ik_esigma_Q2[iES];
                ESig_y_in_value = ik_esigma_y[iES];
                if (ESig_x_in_value > 0){
                    ESig_x_in->Fill(ESig_x_in_value);
                }
                if (ESig_Q2_in_value > 0){
                    ESig_Q2_in->Fill(ESig_Q2_in_value);
                }
                if (ESig_y_in_value > 0){
                    ESig_y_in->Fill(ESig_y_in_value);
                }
            }
            for (int iJB = 0; iJB < ik_jb_x.GetSize(); iJB++){
                JB_x_in_value = ik_jb_x[iJB];
                JB_Q2_in_value = ik_jb_Q2[iJB];
                JB_y_in_value = ik_jb_y[iJB];
                if (JB_x_in_value > 0){
                    JB_x_in->Fill(JB_x_in_value);
                }
                if (JB_Q2_in_value > 0){
                    JB_Q2_in->Fill(JB_Q2_in_value);
                }
                if (JB_y_in_value > 0){
                    JB_y_in->Fill(JB_y_in_value);
                }
            }
            
            vec1.RotateY(0.025);
            fProton->Fill(vec1.Eta());
            h6_eta_MC->Fill(vec1.Eta());
            h6_px_MC->Fill(vec1.Px());
            h6_py_MC->Fill(vec1.Py());
            h6_pt_MC->Fill(vec1.Pt());
            h6_pz_MC->Fill(vec1.Pz());
            h6_e_MC->Fill(vec1.E());
            
            // for the jpsi positron
            fPositron->Fill(vec2.Eta());
            h7_eta_MC->Fill(vec2.Eta());
            h7_px_MC->Fill(vec2.Px());
            h7_py_MC->Fill(vec2.Py());
            h7_pt_MC->Fill(vec2.Pt());
            h7_pz_MC->Fill(vec2.Pz());
            h7_e_MC->Fill(vec2.E());
            
            hjpsi_eta_MC->Fill(vecjpsi.Eta());
            hjpsi_px_MC->Fill(vecjpsi.Px());
            hjpsi_py_MC->Fill(vecjpsi.Py());
            hjpsi_pt_MC->Fill(vecjpsi.Pt());
            hjpsi_pz_MC->Fill(vecjpsi.Pz());
            hjpsi_e_MC->Fill(vecjpsi.E());
            
            fElectron->Fill(vec3.Eta());
            h8_eta_MC->Fill(vec3.Eta());
            h8_px_MC->Fill(vec3.Px());
            h8_py_MC->Fill(vec3.Py());
            h8_pt_MC->Fill(vec3.Pt());
            h8_pz_MC->Fill(vec3.Pz());
            h8_e_MC->Fill(vec3.E());
            
            JPsi1 = vec2 + vecjpsi;
            JPsiMass1->Fill(JPsi1.M());
            
            ixb = q2/(2.0 * incbeam.Dot(incbeame - vec3));
            xb->Fill(ixb);
            ixv = (q2 + JPsi1.M() * JPsi1.M())/(2.0 * incbeam.Dot(incbeame - vec3));
            xv->Fill(ixv);
            
            //JB method
            jbpt2 = (vec1 + vecjpsi + vec2).Px()*(vec1 + vecjpsi + vec2).Px() + (vec1 + vecjpsi + vec2).Py()*(vec1 + vecjpsi + vec2).Py();
            JB_pt2_MC->Fill(jbpt2);
            jbsig = (vec1 + vecjpsi + vec2).E() - (vec1 + vecjpsi + vec2).Pz();
            JB_HFS_MC->Fill(jbsig);
            y_JB = jbsig / (2*incbeame.E());
            JB_y_MC->Fill(y_JB);
            q2_JB = jbpt2 / (1 - y_JB);
            JB_q2_MC->Fill(q2_JB);
            x_JB = q2_JB / (4*incbeame.E()*incbeam.E()*y_JB);
            JB_x_MC->Fill(x_JB);
            
            //eSigma method y_e, q2_eSig, Sig_e, y_eSig,Sig_e, Sig_tot, y_Sig, x_Sig
            Sig_e = (vec3.E() - vec3.Pz());
            y_e = 1 - (Sig_e / (2*incbeame.E()));
            Sig_tot = Sig_e + jbsig;
            y_Sig = jbsig / Sig_tot;
            q2_Sig = ((vec3.Px()*vec3.Px()) + (vec3.Py()*vec3.Py())) / (1 - y_Sig);
            q2_e = ((vec3.Px()*vec3.Px()) + (vec3.Py()*vec3.Py())) / (1 - y_e);
            x_Sig = q2_Sig / (4*incbeame.E()*incbeam.E()*y_Sig);
            y_eSig = q2_e / (4*incbeame.E()*incbeam.E()*x_Sig);
            eSig_y_MC->Fill(y_eSig);
            eSig_q2_MC->Fill(q2_e);
            eSig_x_MC->Fill(x_Sig);
            
            //Kinematics calculations for the MC
            TLorentzVector diff3 = incbeam - vec1;
            t3 = -1*diff3.Dot(diff3);//for MC
            mtgg->Fill(t3);
            
            // Check if vec3 is more energetic than vecjpsi
            if (vec3.E() > vecjpsi.E()) {
                
                // Assign MC-level J/ψ and third hadron
                vec3mc = vec3;
                vecjpsimc = vecjpsi;
                
                // Build the full hadronic system
                TLorentzVector hadronicFinalState = vecjpsimc + vec1 + vec2 + vec3mc;
                
                // Compute E - Pz for the system
                double eMinusPz = hadronicFinalState.E() - hadronicFinalState.Pz();
                
                // Fill histogram if in desired range
                if (eMinusPz > 15 && eMinusPz < 25) {
                    mcjpsie_e_minus_pzmc->Fill(eMinusPz);
                }
            }
            
            // Loop over reconstructed Roman Pot tracks
            for (int iRPPart = 0; iRPPart < rp_px.GetSize(); ++iRPPart) {
                
                // Construct 4-momentum for the current RP track
                TLorentzVector p_recoRP(rp_px[iRPPart], rp_py[iRPPart], rp_pz[iRPPart], rp_energy[iRPPart]);
                
                // Apply basic selection: positive proton with sufficient pT
                iflagproton = 0;
                bool isProton = (rp_charge[iRPPart] == 1) && (rp_pdg[iRPPart] == 2212);
                bool passPtCut = (p_recoRP.Pt() >= 0.2);
                
                if (isProton && passPtCut) {
                    
                    // Set 4-vector with mass from the collection
                    vec5.SetXYZM(rp_px[iRPPart], rp_py[iRPPart], rp_pz[iRPPart], rp_mass[iRPPart]);
                    
                    // Fill single-variable histograms
                    RPfProton->Fill(vec5.Eta());
                    B0RPfProton->Fill(vec5.Eta());
                    h_pt_B0RomanPots->Fill(vec5.Perp());
                    
                    h_px_RomanPots->Fill(p_recoRP.Px());
                    h_py_RomanPots->Fill(p_recoRP.Py());
                    h_pt_RomanPots->Fill(p_recoRP.Pt());
                    h_pz_RomanPots->Fill(p_recoRP.Pz());
                    h_e_RomanPots->Fill(p_recoRP.E());
                    
                    // Hit map (for spatial occupancy studies)
                    h_rp_occupancy_map->Fill(rp_ref_x[iRPPart], rp_ref_y[iRPPart]);
                    
                    iflagproton = 1;
                    
                    // Calculate |t| using different methods
                    
                    // --- Acceptance-corrected method ---
                    TLorentzVector diff_accept = incbeam - vec1;
                    t_accept1 = -diff_accept.Mag2();
                    
                    if (t_accept1 == 0) return; // consider using an epsilon threshold instead
                    
                    mtgg_accept->Fill(t_accept1);
                    
                    // --- Total method with RP+B0 correction ---
                    TLorentzVector diff_tot = incbeam - vec5;
                    t_Tot = - (t3 / t_accept1) * diff_tot.Mag2();
                    mtgTot->Fill(t_Tot);
                    mtgTot1->Fill(t_Tot);
                    
                    // --- RP-only method ---
                    TLorentzVector diff_rp = incbeam - vec5;
                    t1 = - (t3 / t_accept1) * diff_rp.Mag2();
                    mtg->Fill(t1);
                    
                    // --- Comparisons ---
                    difft->Fill((t3 - t1) / t3);
                    difftTot->Fill((t3 - t_Tot) / t3);
                    
                    // --- 2D Plot: true vs. RP-based t ---
                    t_2D->Fill(t3, t1);
                }
            }
            
            //reconstructed tracks with ACTS -- used for B0
            // Loop over reconstructed B0 tracker tracks
            for (int iRecoTrk = 0; iRecoTrk < reco_track_px.GetSize(); ++iRecoTrk) {
                
                // Create 4-vector for the reconstructed track
                TLorentzVector p_recoB0(reco_track_px[iRecoTrk], reco_track_py[iRecoTrk], reco_track_pz[iRecoTrk], reco_track_energy[iRecoTrk]);
                
                // Basic selection for neutral, forward-going B0 proton candidate
                iflagproton2 = 0;
                bool isNeutral = (reco_track_pdg[iRecoTrk] == 0);
                bool isForward = (p_recoB0.Eta() > 4.0);
                bool isChargedTrack = (reco_track_charge[iRecoTrk] == 1);
                
                if (isNeutral && isForward && isChargedTrack) {
                    
                    // Set mass-corrected 4-vector and rotate to remove crossing angle
                    vec4.SetXYZM(reco_track_px[iRecoTrk], reco_track_py[iRecoTrk], reco_track_pz[iRecoTrk], reco_track_mass[iRecoTrk]);
                    vec4.RotateY(0.025); // Remove crossing angle for B0
                    
                    // Fill histograms
                    B0fProton->Fill(vec4.Eta());
                    B0RPfProton->Fill(vec4.Eta());
                    
                    h_px_reco_track->Fill(vec4.Px());
                    h_py_reco_track->Fill(vec4.Py());
                    h_pt_reco_track->Fill(vec4.Pt());
                    h_pt_B0RomanPots->Fill(vec4.Pt());
                    h_pz_reco_track->Fill(vec4.Pz());
                    h_e_reco_track->Fill(vec4.E());
                    
                    iflagproton2 = 1;
                    
                    // --- Kinematic calculations ---
                    
                    // Acceptance-corrected t from MC
                    TLorentzVector diff_accept = incbeam - vec1;
                    t_accept2 = -diff_accept.Mag2();
                    if (t_accept2 == 0) return; // Consider replacing with small epsilon check
                    mtgg_accept->Fill(t_accept2);
                    
                    // Total |t| with RP+B0 correction
                    TLorentzVector diff_tot = incbeam - vec4;
                    t_Tot = - (t3 / t_accept2) * diff_tot.Mag2();
                    mtgTot->Fill(t_Tot);
                    mtgTot1->Fill(t_Tot);
                    
                    // B0-only |t|
                    TLorentzVector diff_b0 = incbeam - vec4;
                    t2 = - (t3 / t_accept2) * diff_b0.Mag2();
                    mtgB0->Fill(t2);
                    
                    // Comparison
                    difft->Fill((t3 - t2) / t3);
                    difftTot->Fill((t3 - t_Tot) / t3);
                    
                    // 2D correlation plot
                    t_2D->Fill(t3, t2);
                }
            }
            
            // Variables to hold info for highest-energy EM cluster
            double maxEnergy = -1e9;
            double xpos = -999., ypos = -999., zpos = -999.;
            double ecal_theta = -999., ecal_phi = -999., findtheta = -999.;
            
            // Loop over EM clusters to find the one with highest energy
            for (int iclus = 0; iclus < ecalN_energy.GetSize(); ++iclus) {
                
                double clusterEnergy = ecalN_energy[iclus] * 1.03; // Apply energy scale factor
                
                if (clusterEnergy > maxEnergy) {
                    maxEnergy = clusterEnergy;
                    
                    xpos = ecalN_x[iclus];
                    ypos = ecalN_y[iclus];
                    zpos = ecalN_z[iclus];
                    ecal_phi = ecalN_phi[iclus];
                    
                    // Compute polar angle θ
                    findtheta = TMath::ATan2(TMath::Sqrt(xpos * xpos + ypos * ypos), zpos);
                    ecal_theta = findtheta;
                    
                    // Fill η histogram
                    double eta = -TMath::Log(TMath::Tan(findtheta / 2.0));
                    ecaleta->Fill(eta);
                }
            }
            
            // Shift theta to center-of-mass frame (if needed)
            double thetaLab = ecal_theta;
            double thetaShifted = thetaLab - TMath::Pi();  // Shift by π if scattering angle is defined backward
            
            // Calculate Q² from shifted angle and max cluster energy (scattered e)
            double sinHalfTheta = TMath::Sin(thetaShifted / 2.0);
            q2_2 = 4.0 * incbeame.E() * maxEnergy * sinHalfTheta * sinHalfTheta;
            
            // Fill histograms
            mtqq_2->Fill(q2_2);
            
            // Avoid division by zero in Q² difference plot
            if (q2 != 0) {
                diffq2_2->Fill((q2 - q2_2) / q2);
            }
            
            //RECO electron selection
            TLorentzVector ixva, JPsia;
            double ff_energy = 0;
            if (reco_track_px.GetSize() == 3) {
                
                // Identify particle indices based on charges: 2 negatives (-1), 1 positive (+1)
                int neg_indices[2], pos_index = -1;
                int n_neg = 0;
                
                for (int i = 0; i < 3; ++i) {
                    if (reco_track_charge[i] == -1) {
                        if (n_neg < 2) neg_indices[n_neg++] = i;
                    } else if (reco_track_charge[i] == 1) {
                        pos_index = i;
                    }
                }
                
                // Proceed only if we found exactly 2 negative and 1 positive
                if (n_neg == 2 && pos_index >= 0) {
                    
                    // Build Lorentz vectors
                    TLorentzVector neg1, neg2, pos;
                    neg1.SetXYZM(reco_track_px[neg_indices[0]], reco_track_py[neg_indices[0]], reco_track_pz[neg_indices[0]], reco_track_mass[neg_indices[0]]);
                    neg2.SetXYZM(reco_track_px[neg_indices[1]], reco_track_py[neg_indices[1]], reco_track_pz[neg_indices[1]], reco_track_mass[neg_indices[1]]);
                    pos.SetXYZM(reco_track_px[pos_index], reco_track_py[pos_index], reco_track_pz[pos_index], reco_track_mass[pos_index]);
                    
                    // Identify which negative is the scattered electron
                    rcpv2 = pos;
                    if (neg1.Pz() < neg2.Pz()) {
                        eq1ck = neg1;
                        rcpv1ck = neg2;
                    } else if (neg2.Pz() < neg1.Pz()) {
                        eq1ck = neg2;
                        rcpv1ck = neg1;
                    } else {
                        // Ambiguous, skip this event
                        return;
                    }
                    
                    // Rapidity cut
                    raprc = (incbeam.Dot(incbeame - eq1ck)) / (incbeam.Dot(incbeame));
                    if ((eq1ck.Pz() < rcpv1ck.Pz()) && raprc > 0.1 && raprc < 1.0) {
                        mcrapidrc->Fill(raprc);
                        
                        // Store chosen tracks
                        eq1 = eq1ck;
                        rcpv1 = rcpv1ck;
                        
                        // Compute rapidity from truth scattered e
                        double rapa = (incbeam.Dot(incbeame - vec3)) / (incbeam.Dot(incbeame));
                        mcrapida->Fill(rapa);
                        JB_y_MC_a->Fill(y_JB);
                        eSig_y_MC_a->Fill(y_eSig);
                        
                        // Assign scattered electron
                        ixva = vec3;
                        ixba = q2 / (2.0 * incbeam.Dot(incbeame - ixva));
                        xba->Fill(ixba);
                        ffElectron->Fill(vec3.Eta());
                        
                        // Save energy and compute J/ψ reco mass
                        ff_energy = vec3.E();
                        JPsia = vec2 + vecjpsi;
                        rcJPsiMassa->Fill(JPsia.M());
                        
                        // Compute xV from 4-momenta (acceptance)
                        ixv2 = (q2 + JPsia.M() * JPsia.M()) / (2.0 * incbeam.Dot(incbeame - vec3));
                        xv2->Fill(ixv2);
                        
                        // Tag ECal-end energy
                        scat_e_EcalEnd->Fill(maxEnergy);
                    }
                    else{
                        continue;
                    }
                }
            }
            
            // Fill reconstructed electron and positron kinematics
            rcElectron->Fill(eq1.Eta());
            rcElectronpt->Fill(eq1.Pt());
            rcPositron->Fill(rcpv2.Eta());
            rcPositronpt->Fill(rcpv2.Pt());
            
            // Fill J/ψ daughter electron kinematics
            rcElectronjpsieta->Fill(rcpv1.Eta());
            rcElectronjpsipt->Fill(rcpv1.Pt());
            
            // 2D η distributions
            eEta2D->Fill(vecjpsi.Eta(), vec2.Eta());
            pEta2D->Fill(rcpv1.Eta(), rcpv2.Eta());
            
            // Scattered electron energy
            scat_e_RC->Fill(eq1.E());
            
            // Calculate and fill Ecal/pz
            tot_p = sqrt(eq1.Pt()*eq1.Pt() + eq1.Pz()*eq1.Pz());
            Ecal_vs_pz->Fill(maxEnergy / tot_p);
            
            // Compute Q² using scattered electron angle
            thetaChPct = eq1.Theta() - TMath::Pi();
            q2_1 = 4 * incbeame.E() * eq1.E() * TMath::Power(TMath::Sin(thetaChPct / 2.0), 2);
            mtqq->Fill(q2_1);
            q2_2D->Fill(q2, q2_1); // 2D Q² comparison
            
            // --- Jacquet-Blondel reconstructed kinematics ---
            if (iflagproton == 1 || iflagproton2 == 1) {
                TLorentzVector jb_vec = (iflagproton == 1) ? (vec5 + rcpv1 + rcpv2) : (vec4 + rcpv1 + rcpv2);
                
                jbpt2_rc = jb_vec.Px()*jb_vec.Px() + jb_vec.Py()*jb_vec.Py();
                jbsig_rc = jb_vec.E() - jb_vec.Pz();
                y_JB_rc = jbsig_rc / (2 * incbeame.E());
                q2_JB_rc = jbpt2_rc / (1 - y_JB_rc);
                x_JB_rc = q2_JB_rc / (4 * incbeame.E() * incbeam.E() * y_JB_rc);
                
                // Fill 1D histograms
                JB_y_RC->Fill(y_JB_rc);
                JB_q2_RC->Fill(q2_JB_rc);
                JB_x_RC->Fill(x_JB_rc);
                
                // Fill differences
                diff_JB_q2->Fill((q2_JB - q2_JB_rc) / q2_JB);
                diff_JB_y->Fill((y_JB - y_JB_rc) / y_JB);
                diff_JB_xbjk->Fill((x_JB - x_JB_rc) / x_JB);
                
                // Fill 2D histograms
                JB_q2_2D->Fill(q2_JB, q2_JB_rc);
                dJB_q2_2D->Fill(q2_JB, (q2_JB - q2_JB_rc) / q2_JB);
                JB_y_2D->Fill(y_JB, y_JB_rc);
                dJB_y_2D->Fill(y_JB, (y_JB - y_JB_rc) / y_JB);
                JB_xbjk_2D->Fill(x_JB, x_JB_rc);
                dJB_xbjk_2D->Fill(x_JB, (x_JB - x_JB_rc) / x_JB);
            }
            
            // --- eSigma Method ---
            Sig_e_rc = eq1.E() - eq1.Pz();
            y_e_rc = 1 - (Sig_e_rc / (2 * incbeame.E()));
            Sig_tot_rc = Sig_e_rc + jbsig_rc;
            y_Sig_rc = jbsig_rc / Sig_tot_rc;
            
            q2_Sig_rc = (eq1.Px()*eq1.Px() + eq1.Py()*eq1.Py()) / (1 - y_Sig_rc);
            q2_e_rc = (eq1.Px()*eq1.Px() + eq1.Py()*eq1.Py()) / (1 - y_e_rc);
            x_Sig_rc = q2_Sig_rc / (4 * incbeame.E() * incbeam.E() * y_Sig_rc);
            y_eSig_rc = q2_e_rc / (4 * incbeame.E() * incbeam.E() * x_Sig_rc);
            
            // Fill 1D histograms
            eSig_y_RC->Fill(y_eSig_rc);
            eSig_q2_RC->Fill(q2_e_rc);
            eSig_x_RC->Fill(x_Sig_rc);
            
            // Fill differences
            diff_eSig_q2->Fill((q2_e - q2_e_rc) / q2_e);
            diff_eSig_y->Fill((y_eSig - y_eSig_rc) / y_eSig);
            diff_eSig_xbjk->Fill((x_Sig - x_Sig_rc) / x_Sig);
            
            // Fill 2D histograms
            eSig_q2_2D->Fill(q2_e, q2_e_rc);
            deSig_q2_2D->Fill(q2_e, (q2_e - q2_e_rc) / q2_e);
            eSig_y_2D->Fill(y_eSig, y_eSig_rc);
            deSig_y_2D->Fill(y_eSig, (y_eSig - y_eSig_rc) / y_eSig);
            eSig_xbjk_2D->Fill(x_Sig, x_Sig_rc);
            deSig_xbjk_2D->Fill(x_Sig, (x_Sig - x_Sig_rc) / x_Sig);
            
            // --- J/ψ Invariant Mass Checks ---
            JPsi22 = rcpv1 + rcpv2;
            JPsi222 = eq1 + rcpv2;
            
            if (JPsi22.M() > 2 && JPsi22.M() < 3.6 && JPsi222.M() > 2 && JPsi222.M() < 3.6)
                rcJPsiMass11->Fill(JPsi22.M());
            
            if (JPsi222.M() > 2 && JPsi222.M() < 3.6 && JPsi22.M() > 2 && JPsi22.M() < 3.6)
                rcJPsiMass111->Fill(JPsi222.M());
            
            if (iflagproton == 1) {
                auto totalVec = rcpv1 + rcpv2 + eq1 + vec5;
                double eMinusPz = totalVec.E() - totalVec.Pz();
                if (eMinusPz > 15 && eMinusPz < 25) {
                    rcjpsie_e_minus_pz->Fill(eMinusPz);
                }
            }
            
            if (iflagproton2 == 1) {
                auto totalVec = rcpv1 + rcpv2 + eq1 + vec4;
                double eMinusPz = totalVec.E() - totalVec.Pz();
                if (eMinusPz > 15 && eMinusPz < 25) {
                    rcjpsie_e_minus_pz->Fill(eMinusPz);
                }
            }
            
            // Reconstructed J/ψ from two positron candidates
            JPsi2 = rcpv1 + rcpv2;
            rcJPsiMass1->Fill(JPsi2.M());
            diffjpsimass->Fill((JPsi1.M() - JPsi2.M()) / JPsi1.M());
            
            // Reconstructed kinematics (x_bj and x_v)
            double denom = 2.0 * incbeam.Dot(incbeame - eq1);
            ixb1 = q2_1 / denom;
            ixv1 = (q2_1 + JPsi2.M() * JPsi2.M()) / denom;
            
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
            dxbjk_2D->Fill(q2, q2 - q2_1);
            
        }
        inputRootFile->Close();
    }
    cout << "Check integrals: " << endl;
    cout << "pt6_mc integral = " << h6_pt_MC->Integral() << endl;
    cout << "pt_RP_reco integral = " << h_pt_RomanPots->Integral() << endl;
    
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
    
    outputFile->Close();
    
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
    RPfProton->SetFillStyle(3002);
    RPfProton->SetFillColor(kRed);
    RPfProton->SetMarkerStyle(8);
    RPfProton->SetMarkerColor(kRed);
    RPfProton->SetLineWidth(0);
    RPfProton->Draw("CP SAME");
    
    // Overlay RECO proton tracks from B0
    B0fProton->SetFillStyle(3002);
    B0fProton->SetFillColor(kBlue);
    B0fProton->SetMarkerStyle(41);
    B0fProton->SetMarkerColor(kBlue);
    B0fProton->SetLineWidth(0);
    B0fProton->Draw("CP SAME");
    
    // Add legend
    TLegend *leg = new TLegend(0.55, 0.5, 0.85, 0.65);
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);
    leg->AddEntry(fProton, "p' MC", "l");
    leg->AddEntry(RPfProton, "p' RECO track - RP", "p kFullDotLarge");
    leg->AddEntry(B0fProton, "p' RECO track - B0", "p kFullFourTrianglesX");
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
    B0RPfProton->SetFillStyle(3002);
    B0RPfProton->SetFillColor(kMagenta);
    B0RPfProton->SetMarkerStyle(43);
    B0RPfProton->SetMarkerColor(kMagenta);
    B0RPfProton->SetLineWidth(0);
    B0RPfProton->Draw("CP SAME");
    
    // Main legend
    TLegend *legt = new TLegend(0.55, 0.5, 0.85, 0.65);
    legt->SetBorderSize(0);
    legt->SetFillStyle(0);
    legt->AddEntry(fProton, "p' MC", "l");
    legt->AddEntry(B0RPfProton, "p' RECO track - RP + B0", "p KFullDoubleDiamond");
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
    rcJPsiMass11->GetXaxis()->SetRangeUser(2, 3.6);
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
    rcJPsiMass1->GetXaxis()->SetRangeUser(2, 4);
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
    rcJPsiMassa->SetFillStyle(3002);
    rcJPsiMassa->SetFillColor(kBlue);
    rcJPsiMassa->SetMarkerStyle(45);
    rcJPsiMassa->SetMarkerColor(kBlue);
    rcJPsiMassa->SetLineWidth(0);
    rcJPsiMassa->Draw("CP SAME");

    // Reco track
    rcJPsiMass1->SetFillStyle(3002);
    rcJPsiMass1->SetFillColor(kRed);
    rcJPsiMass1->SetMarkerStyle(8);
    rcJPsiMass1->SetMarkerColor(kRed);
    rcJPsiMass1->SetLineWidth(0);
    rcJPsiMass1->Draw("CP SAME");

    // Legend for MC vs RECO
    TLegend *jpsi_MCReco = new TLegend(0.2, 0.6, 0.6, 0.68);
    jpsi_MCReco->SetBorderSize(0);
    jpsi_MCReco->SetFillStyle(0);
    jpsi_MCReco->AddEntry(JPsiMass1, "MC", "l");
    jpsi_MCReco->AddEntry(rcJPsiMassa, "Acceptance", "p f");
    jpsi_MCReco->AddEntry(rcJPsiMass1, "RECO track", "p f");
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
    TRatioPlot* rp4 = new TRatioPlot(mtg, mtgg, "divsym");
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
    mtg->SetFillStyle(3002);
    mtg->SetFillColor(kRed + 1);
    mtg->SetMarkerStyle(8);
    mtg->SetMarkerColor(kRed + 1);
    mtg->SetLineWidth(0);
    mtg->Draw("CP SAME");

    // RECO B0
    mtgB0->SetFillStyle(3002);
    mtgB0->SetFillColor(kBlue + 1);
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

    mtgTot->SetFillStyle(3002);
    mtgTot->SetFillColor(kMagenta);
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

    mtgTot->SetFillStyle(3002);
    mtgTot->SetFillColor(kMagenta);
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

    h_pt_B0RomanPots->SetFillStyle(3002);
    h_pt_B0RomanPots->SetFillColor(kMagenta);
    h_pt_B0RomanPots->SetMarkerStyle(43);
    h_pt_B0RomanPots->SetMarkerColor(kMagenta);
    h_pt_B0RomanPots->SetLineWidth(0);
    h_pt_B0RomanPots->Draw("CP SAME");

    TLegend *leg5t = new TLegend(0.55, 0.55, 0.87, 0.68);
    leg5t->SetBorderSize(0);
    leg5t->SetFillStyle(0);
    leg5t->AddEntry(h6_pt_MC, "p' MC", "l");
    leg5t->AddEntry(h_pt_B0RomanPots, "p' RECO - RP + B0", "p f");
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
    rcPositronpt->SetFillStyle(3002);
    rcPositronpt->SetFillColor(46);
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
    rcElectronjpsipt->SetFillStyle(3002);
    rcElectronjpsipt->SetFillColor(46);
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
    rcElectronpt->SetFillStyle(3002);
    rcElectronpt->SetFillColor(46);
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
    rcPositron->SetFillStyle(3002);
    rcPositron->SetFillColor(46);
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
    rcElectronjpsieta->SetFillStyle(3002);
    rcElectronjpsieta->SetFillColor(46);
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
    rcElectron->SetFillStyle(3002);
    rcElectron->SetFillColor(46);
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
    TRatioPlot* rp3 = new TRatioPlot(mtqq, mtq, "pois");
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
    rp3->GetUpperPad()->cd();

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
    EM_Q2->SetLineColor(6);
    EM_Q2->SetLineWidth(4);
    EM_Q2->Draw("CP SAME");

    // RECO track (primary)
    mtqq->SetStats(0);
    mtqq->SetFillStyle(3002);
    mtqq->SetFillColor(46);
    mtqq->SetMarkerStyle(8);
    mtqq->SetMarkerColor(2);
    mtqq->SetLineWidth(0);
    mtqq->Draw("CP SAME");

    // RECO from EEMC
    mtqq_2->SetStats(0);
    mtqq_2->SetMarkerStyle(45);
    mtqq_2->SetMarkerColor(4);
    mtqq_2->SetLineWidth(0);
    mtqq_2->Draw("CP SAME");

    // Add legend for plot elements
    TLegend *qq2 = new TLegend(0.55, 0.55, 0.8, 0.65);
    qq2->SetBorderSize(0);
    qq2->AddEntry(mtq, "MC", "l");
    qq2->AddEntry(mtqq, "RECO track", "pf ");
    qq2->AddEntry(mtqq_2, "EEMC", "p");
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
    TRatioPlot* rp1 = new TRatioPlot(xb1, xb, "pois");
    rp1->Draw();

    // Configure lower pad (ratio)
    rp1->SetSeparationMargin(0.0);
    rp1->GetLowerRefYaxis()->SetTitle("Ratio");
    rp1->GetLowerRefGraph()->SetMinimum(0);
    rp1->GetLowerRefGraph()->SetMaximum(1.5);
    rp1->GetLowerRefGraph()->SetMarkerStyle(8);
    rp1->GetLowerRefGraph()->SetMarkerColor(2);

    // Configure upper pad (main plot)
    rp1->GetUpperPad()->cd();

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
    EM_x->Draw("CP SAME");

    // RECO curve
    xb1->SetStats(0);
    xb1->SetFillStyle(3002);
    xb1->SetFillColor(46);
    xb1->SetMarkerStyle(8);
    xb1->SetMarkerColor(2);
    xb1->SetLineWidth(0);
    xb1->Draw("CP SAME");

    // Legend for plot components
    TLegend *xbjkk = new TLegend(0.55, 0.55, 0.8, 0.65);
    xbjkk->SetBorderSize(0);
    xbjkk->AddEntry(xb, "MC", "l");
    xbjkk->AddEntry(xb1, "RECO track", "p f");
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
    xv1->SetFillStyle(3002);
    xv1->SetFillColor(46);
    xv1->SetMarkerStyle(8);
    xv1->SetMarkerColor(2);
    xv1->SetLineWidth(0);
    xv1->Draw("CP SAME");

    /*
    // Optional: Add second RECO source (e.g., EEMC or acceptance)
    xv2->SetStats(0);
    xv2->SetMarkerStyle(45);
    xv2->SetMarkerColor(4);
    xv2->SetLineWidth(0);
    xv2->Draw("CP SAME");
    */

    // Legend for components
    TLegend *xvv = new TLegend(0.55, 0.55, 0.8, 0.65);
    xvv->SetBorderSize(0);
    xvv->AddEntry(xv, "MC", "l");
    xvv->AddEntry(xv1, "RECO track", "p f");
    // xvv->AddEntry(xv2, "J/#psi Acceptance", "kFullFourTrianglesPlus");
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
    diffjpsimass->SetMarkerStyle(8);
    diffjpsimass->SetMarkerColor(2);
    diffjpsimass->SetFillStyle(3002);
    diffjpsimass->SetFillColor(46);
    diffjpsimass->SetStats(0);
    diffjpsimass->SetLineWidth(0);
    diffjpsimass->Draw("CP");

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
    diffq2->Draw("CP");

    diff_eSig_q2->SetLineColor(2);
    diff_eSig_q2->SetLineWidth(4);
    diff_eSig_q2->SetStats(0);
    diff_eSig_q2->Draw("CP SAME");

    diff_JB_q2->SetLineColor(4);
    diff_JB_q2->SetLineWidth(4);
    diff_JB_q2->SetStats(0);
    diff_JB_q2->Draw("CP SAME");

    // Legend for Q²
    TLegend *legend_q2 = new TLegend(0.6, 0.5, 0.85, 0.6);
    legend_q2->SetBorderSize(0);
    legend_q2->AddEntry(diffq2, "E Method", "l");
    legend_q2->AddEntry(diff_eSig_q2, "e#Sigma Method", "l");
    legend_q2->AddEntry(diff_JB_q2, "JB Method", "l");
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

    diff_eSig_y->SetLineColor(2);
    diff_eSig_y->SetLineWidth(4);
    diff_eSig_y->SetStats(0);
    diff_eSig_y->Draw("CP SAME");

    diff_JB_y->SetLineColor(4);
    diff_JB_y->SetLineWidth(4);
    diff_JB_y->SetStats(0);
    diff_JB_y->Draw("CP SAME");

    // Legend for y
    TLegend *legend_y = new TLegend(0.6, 0.5, 0.85, 0.6);
    legend_y->SetBorderSize(0);
    legend_y->AddEntry(diffy, "E Method", "l");
    legend_y->AddEntry(diff_eSig_y, "e#Sigma Method", "l");
    legend_y->AddEntry(diff_JB_y, "JB Method", "l");
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

    diff_eSig_xbjk->SetLineColor(2);
    diff_eSig_xbjk->SetLineWidth(4);
    diff_eSig_xbjk->SetStats(0);
    diff_eSig_xbjk->Draw("CP SAME");

    diff_JB_xbjk->SetLineColor(4);
    diff_JB_xbjk->SetLineWidth(4);
    diff_JB_xbjk->SetStats(0);
    diff_JB_xbjk->Draw("CP SAME");

    // Legend for x_{Bj}
    TLegend *legend_xbj = new TLegend(0.6, 0.5, 0.85, 0.6);
    legend_xbj->SetBorderSize(0);
    legend_xbj->AddEntry(diffxbjk, "E Method", "l");
    legend_xbj->AddEntry(diff_eSig_xbjk, "e#Sigma Method", "l");
    legend_xbj->AddEntry(diff_JB_xbjk, "JB Method", "l");
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
    mcjpsie_e_minus_pzmc->SetLineWidth(6);
    mcjpsie_e_minus_pzmc->Draw();

    rcjpsie_e_minus_pz->SetMarkerStyle(8);
    rcjpsie_e_minus_pz->SetMarkerColor(2);
    rcjpsie_e_minus_pz->SetFillStyle(3002);
    rcjpsie_e_minus_pz->SetFillColor(46);
    rcjpsie_e_minus_pz->SetLineWidth(0);
    rcjpsie_e_minus_pz->Draw("CP SAME");

    // Legend for E - Pz
    TLegend *mceminuspz = new TLegend(0.6, 0.5, 0.87, 0.6);
    mceminuspz->SetBorderSize(0);
    mceminuspz->AddEntry(mcjpsie_e_minus_pzmc, "MC J/#psi e", "l");
    mceminuspz->AddEntry(rcjpsie_e_minus_pz, "RECO J/#psi e", "p");
    mceminuspz->Draw();

    // Info label
    TLegend *labelemp = new TLegend(0.52, 0.62, 0.89, 0.87);
    labelemp->SetBorderSize(0);
    labelemp->AddEntry("", "lAger, 1 < Q^{2} < 10, 10 fb^{-1}", "");
    labelemp->AddEntry("", "10x100 GeV", "");
    labelemp->AddEntry("", "", "");
    labelemp->AddEntry("", "#sum_{h}E_{h} - p_{z,h}", "");
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
    h2_e_MC->GetXaxis()->SetRangeUser(0, 12);
    h2_e_MC->GetYaxis()->SetTitleSize(0.03);
    h2_e_MC->SetStats(0);
    h2_e_MC->SetLineColor(1);
    h2_e_MC->SetLineWidth(4);
    h2_e_MC->Draw();

    // RECO track electron energy
    scat_e_RC->SetFillStyle(3002);
    scat_e_RC->SetFillColor(46);
    scat_e_RC->SetMarkerStyle(8);
    scat_e_RC->SetMarkerColor(2);
    scat_e_RC->SetLineWidth(0);
    scat_e_RC->Draw("CP SAME");

    // EEMC-detected electron energy
    scat_e_EcalEnd->SetMarkerStyle(33); // diamond shape
    scat_e_EcalEnd->SetMarkerColor(4);
    scat_e_EcalEnd->SetLineWidth(2);
    scat_e_EcalEnd->Draw("CP SAME");

    // Legend for energy sources
    TLegend *e_MC_RC_EcalEnd = new TLegend(0.2, 0.55, 0.6, 0.68);
    e_MC_RC_EcalEnd->SetBorderSize(0);
    e_MC_RC_EcalEnd->AddEntry(h2_e_MC, "e' MC", "l");
    e_MC_RC_EcalEnd->AddEntry(scat_e_RC, "e' RECO track", "p");
    e_MC_RC_EcalEnd->AddEntry(scat_e_EcalEnd, "e' EEMC", "p");
    e_MC_RC_EcalEnd->Draw();

    // Label for configuration
    TLegend *label4 = new TLegend(0.2, 0.7, 0.59, 0.8);
    label4->SetBorderSize(0);
    // label4->AddEntry("", "ePIC Preliminary", "");
    label4->AddEntry("", "lAger, 1 < Q^{2} < 10, 10 fb^{-1}", "");
    label4->AddEntry("", "10x100 GeV", "");
    label4->Draw();

    TCanvas *Ecal_vs_tot_p = new TCanvas("Ecal_vs_tot_p","Ecal_vs_tot_p",800,800);
    Ecal_vs_pz->GetXaxis()->SetTitle("E/|P|");
    Ecal_vs_pz->GetXaxis()->SetRangeUser(-1,3);
    Ecal_vs_pz->SetTitle("Tracking_Energy/Calorimeter_Energy");
    Ecal_vs_pz->SetStats(0);
    Ecal_vs_pz->SetLineColor(1);
    Ecal_vs_pz->SetLineWidth(6);
    Ecal_vs_pz->Draw();
    
    TCanvas *JB_MC_plot = new TCanvas("JB_MC_plot", "JB_MC_plot", 1400, 1000);
    JB_MC_plot->Divide(4, 3);

    // Panel 1: JB pt2 MC
    JB_MC_plot->cd(1);
    JB_pt2_MC->SetLineColor(kBlack);
    JB_pt2_MC->SetLineWidth(4);
    JB_pt2_MC->SetStats(0);
    JB_pt2_MC->Draw();

    TLegend *leg_pt2 = new TLegend(0.4, 0.65, 0.89, 0.87);
    leg_pt2->SetBorderSize(0);
    leg_pt2->AddEntry("", "p_{T,h}^{2} = (#sum p_{x})^{2} + (#sum p_{y})^{2}", "");
    leg_pt2->Draw();

    // Panel 2: JB HFS MC
    JB_MC_plot->cd(2);
    JB_HFS_MC->SetLineColor(kBlack);
    JB_HFS_MC->SetLineWidth(4);
    JB_HFS_MC->SetStats(0);
    JB_HFS_MC->Draw();

    TLegend *leg_HFS = new TLegend(0.4, 0.65, 0.89, 0.87);
    leg_HFS->SetBorderSize(0);
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
    TRatioPlot* ratio_Q2 = new TRatioPlot(JB_q2_RC, JB_q2_MC);
    ratio_Q2->Draw();

    // Lower pad (ratio)
    ratio_Q2->GetLowerPad()->cd();
    ratio_Q2->GetLowerRefYaxis()->SetTitle("ratio");
    ratio_Q2->GetLowerRefGraph()->SetMinimum(0.4);
    ratio_Q2->GetLowerRefGraph()->SetMaximum(1.2);
    ratio_Q2->SetSeparationMargin(0);
    ratio_Q2->GetLowerRefGraph()->SetMarkerStyle(8);
    ratio_Q2->GetLowerRefGraph()->SetMarkerColor(kRed);

    // Upper pad (distributions)
    ratio_Q2->GetUpperPad()->cd();
    JB_q2_MC->SetLineColor(kBlack);
    JB_q2_MC->SetLineWidth(4);
    JB_q2_MC->SetStats(0);
    JB_q2_MC->GetXaxis()->SetTitle("Q^{2}");
    JB_q2_MC->Draw();

    JB_Q2_in->SetLineColor(kMagenta + 2);
    JB_Q2_in->SetLineWidth(4);
    JB_Q2_in->SetStats(0);
    JB_Q2_in->Draw("SAME");

    JB_q2_RC->SetLineColor(kRed);
    JB_q2_RC->SetLineWidth(4);
    JB_q2_RC->SetStats(0);
    JB_q2_RC->Draw("SAME");

    TLegend *leg_q2 = new TLegend(0.4, 0.65, 0.89, 0.87);
    leg_q2->SetBorderSize(0);
    leg_q2->AddEntry(JB_q2_MC, "Q^{2}_{JB} MC", "l");
    leg_q2->AddEntry(JB_q2_RC, "Q^{2}_{JB} Reco", "l");
    leg_q2->AddEntry(JB_Q2_in, "Inclusive", "l");
    leg_q2->AddEntry("", "", "");
    leg_q2->AddEntry("", "Q^{2} = #frac{p_{T,h}^{2}}{1 - y}", "");
    leg_q2->Draw();

    // Panel 5: JB x ratio
    JB_MC_plot->cd(5)->SetLogy();
    TRatioPlot* ratio_x = new TRatioPlot(JB_x_RC, JB_x_MC);
    ratio_x->Draw();

    // Lower pad (ratio)
    ratio_x->GetLowerPad()->cd();
    ratio_x->GetLowerRefYaxis()->SetTitle("ratio");
    ratio_x->GetLowerRefGraph()->SetMinimum(0.4);
    ratio_x->GetLowerRefGraph()->SetMaximum(1.2);
    ratio_x->SetSeparationMargin(0);
    ratio_x->GetLowerRefGraph()->SetMarkerStyle(8);
    ratio_x->GetLowerRefGraph()->SetMarkerColor(kRed);

    // Upper pad (distributions)
    ratio_x->GetUpperPad()->cd();
    JB_x_MC->SetLineColor(kBlack);
    JB_x_MC->SetLineWidth(4);
    JB_x_MC->SetStats(0);
    JB_x_MC->GetXaxis()->SetTitle("x");
    JB_x_MC->Draw();

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

    TCanvas *x_eSigma = new TCanvas("x_eSigma", "x_eSigma", 1000, 1000);
    x_eSigma->Divide(3, 3);

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

    TRatioPlot *ratio_q2 = new TRatioPlot(eSig_q2_RC, eSig_q2_MC);
    ratio_q2->Draw();

    ratio_q2->GetLowerPad()->cd();
    ratio_q2->GetLowerRefYaxis()->SetTitle("ratio");
    ratio_q2->GetLowerRefGraph()->SetMinimum(0.4);
    ratio_q2->GetLowerRefGraph()->SetMaximum(1.2);
    ratio_q2->SetSeparationMargin(0);
    ratio_q2->GetLowerRefGraph()->SetMarkerStyle(8);
    ratio_q2->GetLowerRefGraph()->SetMarkerColor(2);

    ratio_q2->GetUpperPad()->cd();
    eSig_q2_MC->SetLineColor(1);
    eSig_q2_MC->SetLineWidth(4);
    eSig_q2_MC->SetStats(0);
    eSig_q2_MC->GetXaxis()->SetTitle("Q_{e}^{2}");
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

    TLegend *leg_q2es = new TLegend(0.4, 0.65, 0.89, 0.87);
    leg_q2es->SetBorderSize(0);
    leg_q2es->AddEntry(eSig_q2_MC, "Q_{e}^{2} MC", "l");
    leg_q2es->AddEntry(eSig_q2_RC, "Q_{e}^{2} Reco", "l");
    leg_q2es->AddEntry(ESig_Q2_in, "Inclusive", "l");
    leg_q2es->AddEntry("", "", "");
    leg_q2es->AddEntry("", "Q^{2}_{e} = #frac{p^{2}_{T,e}}{1 - y_{e}}", "");
    leg_q2es->Draw();

    // === Canvas 3: x_Sigma with ratio plot ===
    x_eSigma->cd(3)->SetLogy();

    TRatioPlot *ratio_xes = new TRatioPlot(eSig_x_RC, eSig_x_MC);
    ratio_xes->Draw();

    ratio_xes->GetLowerPad()->cd();
    ratio_xes->GetLowerRefYaxis()->SetTitle("ratio");
    ratio_xes->GetLowerRefGraph()->SetMinimum(0.4);
    ratio_xes->GetLowerRefGraph()->SetMaximum(1.2);
    ratio_xes->SetSeparationMargin(0);
    ratio_xes->GetLowerRefGraph()->SetMarkerStyle(8);
    ratio_xes->GetLowerRefGraph()->SetMarkerColor(2);

    ratio_xes->GetUpperPad()->cd();
    eSig_x_MC->SetLineColor(1);
    eSig_x_MC->SetLineWidth(4);
    eSig_x_MC->SetStats(0);
    eSig_x_MC->GetXaxis()->SetTitle("x_{#Sigma}");
    eSig_x_MC->GetYaxis()->SetNdivisions(0);
    eSig_x_MC->Draw();

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
    
    // 2D Resolution Plots: MC vs RECO
    auto q2_2D_plot = new TCanvas("q2_2D_plot", "Q^{2} Resolution", 800, 800);
    q2_2D->SetStats(0);
    q2_2D->GetXaxis()->SetTitle("Q^{2} (MC) [GeV^{2}]");
    q2_2D->GetYaxis()->SetTitle("Q^{2} (RECO) [GeV^{2}]");
    q2_2D->Draw("COLZ");

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

    auto dxbjk_plot = new TCanvas("dxbjk_plot", "x_{bj} 2D", 800, 800);
    dxbjk_2D->SetStats(0);
    dxbjk_2D->GetXaxis()->SetTitle("x_{bj}");
    dxbjk_2D->GetYaxis()->SetTitle("Δx_{bj}");
    dxbjk_2D->Draw("COLZ");

    // J/ψ Electron η Distribution
    auto ElectronEta = new TCanvas("ElectronEta", "J/psi Electron eta", 800, 800);
    hjpsi_eta_MC->SetStats(0);
    hjpsi_eta_MC->GetXaxis()->SetTitle("#eta");
    hjpsi_eta_MC->GetXaxis()->SetRangeUser(-4.5, 5);
    hjpsi_eta_MC->GetYaxis()->SetTitleSize(0.03);
    hjpsi_eta_MC->SetLineColor(1);
    hjpsi_eta_MC->SetLineWidth(6);
    hjpsi_eta_MC->Draw();

    rcElectronjpsieta->SetFillStyle(0);
    rcElectronjpsieta->SetLineColor(2);
    rcElectronjpsieta->SetLineWidth(6);
    rcElectronjpsieta->Draw("SAME");

    fPositron->SetLineColor(4);
    fPositron->SetLineWidth(6);
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

    // Differential Cross Section dσ/dt
    auto h_dsigma_dt = (TH1D*)mtgg->Clone("h_dsigma_dt");
    auto h_dsigma_dt_rc = (TH1D*)mtgTot1->Clone("h_dsigma_dt_rc");

    for (int i = 1; i <= h_dsigma_dt->GetNbinsX(); ++i) {
        double dsdt = h_dsigma_dt->GetBinContent(i) / (lumi_gen * h_dsigma_dt->GetBinWidth(i));
        h_dsigma_dt->SetBinContent(i, dsdt);
    }

    for (int i = 1; i <= h_dsigma_dt_rc->GetNbinsX(); ++i) {
        double dsdt_rc = h_dsigma_dt_rc->GetBinContent(i) / (lumi_gen * h_dsigma_dt_rc->GetBinWidth(i));
        h_dsigma_dt_rc->SetBinContent(i, dsdt_rc);
    }

    // Compute statistical uncertainties
    for (int i = 1; i <= h_dsigma_dt->GetNbinsX(); ++i) {
        double counts = mtgg->GetBinContent(i);
        double binWidth = h_dsigma_dt->GetBinWidth(i);
        double dsdt = counts / (lumi_gen * binWidth);
        double err = sqrt(counts) / (lumi_gen * binWidth);  // Error propagation assuming Poisson stats
        h_dsigma_dt->SetBinContent(i, dsdt);
        h_dsigma_dt->SetBinError(i, err);
    }

    for (int i = 1; i <= h_dsigma_dt_rc->GetNbinsX(); ++i) {
        double counts = mtgTot1->GetBinContent(i);
        double binWidth = h_dsigma_dt_rc->GetBinWidth(i);
        double dsdt_rc = counts / (lumi_gen * binWidth);
        double err = sqrt(counts) / (lumi_gen * binWidth);  // Same here
        h_dsigma_dt_rc->SetBinContent(i, dsdt_rc);
        h_dsigma_dt_rc->SetBinError(i, err);
    }

    // Plot with error bars
    auto t_distribution_weight = new TCanvas("t_distribution_weight", "d#sigma/dt vs -t", 800, 800);
    t_distribution_weight->SetLogy();

    h_dsigma_dt->SetStats(0);
    h_dsigma_dt->SetMarkerStyle(8);
    h_dsigma_dt->SetMarkerColor(1);
    h_dsigma_dt->SetLineWidth(4);
    h_dsigma_dt->GetXaxis()->SetRangeUser(0, 2);
    h_dsigma_dt->GetXaxis()->SetTitle("-t [GeV^{2}]");
    h_dsigma_dt->GetYaxis()->SetTitle("d#sigma/dt [nb/GeV^{2}]");
    h_dsigma_dt->Draw("E1");  // Draw with error bars

    h_dsigma_dt_rc->SetMarkerStyle(8);
    h_dsigma_dt_rc->SetMarkerColor(2);
    h_dsigma_dt_rc->SetLineColor(2);
    h_dsigma_dt_rc->SetLineWidth(4);
    h_dsigma_dt_rc->Draw("E1 SAME");  // Also draw with error bars

    auto new_t_dist = new TLegend(0.4, 0.65, 0.89, 0.87);
    new_t_dist->SetBorderSize(0);
    new_t_dist->AddEntry("", "lAger, 1<Q^{2}<10, 10 fb^{-1}", "");
    new_t_dist->AddEntry("", "10x100 GeV, J/#psi #rightarrow e^{+}e^{-}", "");
    new_t_dist->AddEntry(h_dsigma_dt, "p' MC", "lep");
    new_t_dist->AddEntry(h_dsigma_dt_rc, "p' RECO (B0+RP)", "lep");
    new_t_dist->Draw();


    return;

}
