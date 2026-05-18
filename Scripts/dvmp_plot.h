#ifndef DVMP_PLOT_H
#define DVMP_PLOT_H

#include <TH1F.h>
#include <TH2F.h>
#include <TFile.h>

//---------------------------------------------
// Histograms
//---------------------------------------------
//laAger
TH1F *h_t_lAger;
TH1F *h_xb_lAger;
TH1F *h_Q2_lAger;
TH1F *h_y_lAger;

//Roman Pots
TH1F *RPfinalProton;
TH1F *B0RPfinalProton;

TH1F *h_pt_RomanPots;
TH1F *h_pt_B0RomanPots;

TH1F *h_px_RomanPots;
TH1F *h_py_RomanPots;
TH1F *h_pz_RomanPots;
TH1F *h_energ_RomanPots;

TH1F *ThetaRP;
TH1F *ThetaRPB0;

TH2F *h_rp_occupancy_map;


//B0 track
TH1F *B0finalProton;

TH1F *h_px_B0_track;
TH1F *h_py_B0_track;
TH1F *h_pt_B0_track;
TH1F *h_pz_B0_track;
TH1F *h_energ_B0_track;

TH1F *ThetaB0;

TH1F *h_B0_ref_z;
TH2F *h_B0_occupancy_map_layer_0;
TH2F *h_B0_occupancy_map_layer_1;
TH2F *h_B0_occupancy_map_layer_2;
TH2F *h_B0_occupancy_map_layer_3;

TH1F *h_B0_hit_energy_deposit;

TH2F *h_B0_emcal_occupancy_map;
TH1F *h_B0_emcal_cluster_energy;


//RECO electron
TH1F *h_jpsi_mass_RC;
TH1F *h_scattered_electron_energy_RC;
TH1F *h_scattered_electron_eta_RC;
TH1F *h_scattered_electron_pt_RC;
TH1F *h_scattered_positron_eta_RC;
TH1F *h_decay_electron_eta_RC;


//for Calorimeter
TH1F *h_matched_ecal_energy;
TH1F *h_matched_ecal_dr;
TH1F *h_matched_eop;


//inclusive variables
// Electron method
TH1F *EM_x;
TH1F *EM_Q2;
TH1F *EM_y;

// eSigma method
TH1F *ESig_x_in;
TH1F *ESig_Q2_in;
TH1F *ESig_y_in;

// Jacquet-Blondel
TH1F *JB_x_in;
TH1F *JB_Q2_in;
TH1F *JB_y_in;

// Double Angle
TH1F *DA_x_in;
TH1F *DA_Q2_in;
TH1F *DA_y_in;

//for MC
TH1F *h_mc_jpsi_mass;

// MC positron
TH1F *h_mc_positron_px;
TH1F *h_mc_positron_py;
TH1F *h_mc_positron_pz;
TH1F *h_mc_positron_pt;
TH1F *h_mc_positron_E;
TH1F *h_mc_positron_eta;

// MC decay electron
TH1F *h_mc_decay_electron_px;
TH1F *h_mc_decay_electron_py;
TH1F *h_mc_decay_electron_pz;
TH1F *h_mc_decay_electron_pt;
TH1F *h_mc_decay_electron_E;
TH1F *h_mc_decay_electron_eta;

// MC scattered electron
TH1F *h_mc_scattered_electron_px;
TH1F *h_mc_scattered_electron_py;
TH1F *h_mc_scattered_electron_pz;
TH1F *h_mc_scattered_electron_pt;
TH1F *h_mc_scattered_electron_E;
TH1F *h_mc_scattered_electron_eta;

// MC recoil proton
TH1F *h_mc_recoil_proton_px;
TH1F *h_mc_recoil_proton_py;
TH1F *h_mc_recoil_proton_pz;
TH1F *h_mc_recoil_proton_pt;
TH1F *h_mc_recoil_proton_E;
TH1F *h_mc_recoil_proton_eta;

//missing mass
TH1F *h_missingMass_MC;
TH1F *h_missingMass2_MC;
TH1F *h_missingMass_RP;
TH1F *h_missingMass2_RP;
TH1F *h_missingMass_B0;
TH1F *h_missingMass2_B0;

//RECO kinematics electron method
TH1F *h_reco_EM_x;
TH1F *h_reco_EM_Q2;
TH1F *h_reco_EM_y;

TH1F *h_reco_JB_x;
TH1F *h_reco_JB_Q2;
TH1F *h_reco_JB_y;
TH1F *h_reco_JB_HFS_sigma;
TH1F *h_reco_JB_HFS_pt;

TH1F *h_reco_eSigma_x;
TH1F *h_reco_eSigma_Q2;
TH1F *h_reco_eSigma_y;
TH1F *h_reco_eSigma_HFS_sigma;

TH1F *h_reco_DA_x;
TH1F *h_reco_DA_Q2;
TH1F *h_reco_DA_y;


// Truth-vs-RECO fractional residual 2D
TH2F *h2_dx_EM, *h2_dx_JB, *h2_dx_eSigma, *h2_dx_DA;
TH2F *h2_dQ2_EM, *h2_dQ2_JB, *h2_dQ2_eSigma, *h2_dQ2_DA;
TH2F *h2_dy_EM, *h2_dy_JB, *h2_dy_eSigma, *h2_dy_DA;

// 1D fractional residuals
TH1F *h_dx_EM, *h_dx_JB, *h_dx_eSigma, *h_dx_DA;
TH1F *h_dQ2_EM, *h_dQ2_JB, *h_dQ2_eSigma, *h_dQ2_DA;
TH1F *h_dy_EM, *h_dy_JB, *h_dy_eSigma, *h_dy_DA;

// Truth vs RECO migration/resolution
TH2F *h2_x_EM_res, *h2_x_JB_res, *h2_x_eSigma_res, *h2_x_DA_res;
TH2F *h2_Q2_EM_res, *h2_Q2_JB_res, *h2_Q2_eSigma_res, *h2_Q2_DA_res;
TH2F *h2_y_EM_res, *h2_y_JB_res, *h2_y_eSigma_res, *h2_y_DA_res;

// Electron energy resolution
TH1F *h_CalEnergy_minus_SCElecEnergy;
TH1F *h_TrackEnergy_minus_SCElecEnergy;
TH2F *h_Cal_DE_over_E_vs_Eta;
TH2F *h_Track_DE_over_E_vs_Eta;

//for the mc and reco hfs
TH1F *h_mc_e_minus_pz;
TH1F *h_reco_e_minus_pz;

// t-distributions
TH1F *h_t_MC;
TH1F *h_t_RECO_RP;
TH1F *h_t_RECO_B0;
TH1F *h_t_RECO_RPB0;

TH2F *h_t2D_RP;
TH2F *h_t2D_B0;
TH2F *h_t2D_RPB0;

TH1F *h_t_MethodL_RECO;
TH2F *h_t2D_MethodL;

//combinig RP, B0 and Method L
TH1F *h_t_RECO_RPB0_MethodL;
TH2F *h_t2D_RPB0_MethodL;

//working on the corrected t distribution
TH1F *h_t_lAger_accept;
TH1F *h_CorrectionFactor = nullptr;
TH1F *h_t_RPB0_MethodL_corrected = nullptr;
//---------------------------------------------
// Initialize histograms
//---------------------------------------------
void InitHistograms()
{
    //generator level histograms
    h_t_lAger = new TH1F("h_t_lAger", "-t distribution; -t[GeV^{2}]; Counts", 200, 0, 5 );
    h_xb_lAger = new TH1F("h_xb_lAger", "xb distribution;x_{B};Counts", 100, 0, 1);
    h_Q2_lAger = new TH1F("h_Q2_lAger", "Q2 distribution;Q^{2} [GeV^{2}];Counts", 100, 0, 11 );
    h_y_lAger = new TH1F("h_y_lAger", "y distribution;y;Counts", 100, 0, 1);
    
    //romanpots histograms
    RPfinalProton = new TH1F("RPfinalProton", " ;#eta;Counts", 100, -5, 10 );
    B0RPfinalProton = new TH1F("B0RPfinalProton", " ;#eta;Counts", 100, -5, 10 );
    h_pt_RomanPots = new TH1F("h_pt_RomanPots", " ;P_{t} [GeV/c];Counts", 100, -1, 2 );
    h_pt_B0RomanPots = new TH1F("h_pt_B0RomanPots", " ;p_{T} [GeV/c];Counts", 100, -1, 2 );
    h_px_RomanPots = new TH1F("h_px_RomanPots", " ;P_{x} [GeV/c];Counts", 100, -4, 4 );
    h_py_RomanPots = new TH1F("h_py_RomanPots", " ;P_{y} [GeV/c];Counts", 100, -4, 4 );
    h_pz_RomanPots = new TH1F("h_pz_RomanPots", " ;P_{z} [GeV/c];Counts", 100, 80, 150 );
    h_energ_RomanPots = new TH1F("h_energ_RomanPots", " ;Energy [GeV];Counts", 100, 80, 150 );
    
    ThetaRP = new TH1F("ThetaRP", " ;#theta_{RP} [mrad];Counts", 100, -0.001, 0.025);
    ThetaRPB0 = new TH1F("ThetaRPB0", " ;#theta_{RP+B0} [mrad];Counts", 100, -0.001, 0.025);
    
    h_rp_occupancy_map = new TH2F("Roman_pots_occupancy_map", ";hit x [mm]", 100, -1300, -1000, 100, -40, 40);
    
    
    //B0 histograms
    B0finalProton = new TH1F("B0finalProton", ";#eta;Counts", 100, -5, 10);
    h_px_B0_track = new TH1F("h_px_B0_track", ";P_{x} [GeV/c];Counts", 100, -4, 4);
    h_py_B0_track = new TH1F("h_py_B0_track", ";P_{y} [GeV/c];Counts", 100, -4, 4);
    h_pt_B0_track = new TH1F("h_pt_B0_track", ";p_{T} [GeV/c];Counts", 100, -1, 2);
    h_pz_B0_track = new TH1F("h_pz_B0_track", ";P_{z} [GeV/c];Counts", 100, 80, 150);
    h_energ_B0_track = new TH1F("h_energ_B0_track", ";Energy [GeV];Counts", 100, 80, 150);

    ThetaB0 = new TH1F("ThetaB0", ";#theta_{B0};Counts", 100, -0.001, 0.025);
    h_B0_ref_z = new TH1F( "h_B0_ref_z", "B0 reference z;z [mm];Counts", 200, -5000, 5000);
    h_B0_occupancy_map_layer_0 = new TH2F("h_B0_occupancy_map_layer_0", "B0 tracker hit map layer 0;x [mm];y [mm]", 200, -350, 0, 200, -200, 200);
    h_B0_occupancy_map_layer_1 = new TH2F("h_B0_occupancy_map_layer_1", "B0 tracker hit map layer 1;x [mm];y [mm]", 200, -350, 0, 200, -200, 200);
    h_B0_occupancy_map_layer_2 = new TH2F("h_B0_occupancy_map_layer_2", "B0 tracker hit map layer 2;x [mm];y [mm]", 200, -350, 0, 200, -200, 200);
    h_B0_occupancy_map_layer_3 = new TH2F("h_B0_occupancy_map_layer_3", "B0 tracker hit map layer 3;x [mm];y [mm]", 200, -350, 0, 200, -200, 200);

    h_B0_hit_energy_deposit = new TH1F("h_B0_hit_energy_deposit", "B0 tracker hit energy deposit;E_{dep} [keV];Counts", 200, 0, 200);

    h_B0_emcal_occupancy_map = new TH2F("h_B0_emcal_occupancy_map", "B0 EMCAL occupancy map;x [mm];y [mm]", 200, -350, 0, 200, -200, 200);
    h_B0_emcal_cluster_energy = new TH1F("h_B0_emcal_cluster_energy", "B0 EMCAL cluster energy;E [GeV];Counts", 200, 0, 50);
    
    
    //for the RECO electron
    h_jpsi_mass_RC = new TH1F("h_jpsi_mass_RC", ";M_{e^{+}e^{-}} [GeV];Counts", 100, 2.0, 4.0);
    h_scattered_electron_energy_RC = new TH1F("h_scattered_electron_energy_RC", ";E [GeV];Counts", 100, 0, 11);
    h_scattered_electron_eta_RC = new TH1F("h_scattered_electron_eta_RC", ";#eta;Counts", 100, -4, 4);
    h_scattered_electron_pt_RC = new TH1F("h_scattered_electron_pt_RC", ";p_{T} [GeV];Counts", 100, -1, 5);
    h_scattered_positron_eta_RC = new TH1F("h_scattered_positron_eta_RC", "RECO Positron;#eta;Counts", 100, -5, 5);
    h_decay_electron_eta_RC = new TH1F("h_decay_electron_eta_RC", "RECO Electron;#eta;Counts", 100, -5, 5);
    
    
    //for calorimeter
    h_matched_ecal_energy = new TH1F("h_matched_ecal_energy", " ;E [GeV];Counts", 100, 0, 11);
    h_matched_ecal_dr = new TH1F("h_matched_ecal_dr", " ;#DeltaR;Counts", 100, 0, 1);
    h_matched_eop = new TH1F("h_matched_eop", " ;E/p;Counts", 100, 0, 2);
    
    
    //for inclusive

    // Electron method
    EM_x = new TH1F("EM_x", "Electron Method;x;Counts", 100, 1e-5, 1.0);
    EM_Q2 = new TH1F("EM_Q2", "Electron Method;Q^{2} [GeV^{2}];Counts", 100, 0, 11);
    EM_y = new TH1F("EM_y", "Electron Method;y;Counts", 100, 0, 1);

    // eSigma
    ESig_x_in = new TH1F("ESig_x_in", "eSigma Method;x;Counts", 100, 1e-5, 1.0);
    ESig_Q2_in = new TH1F("ESig_Q2_in", "eSigma Method;Q^{2} [GeV^{2}];Counts", 100, 0, 11);
    ESig_y_in = new TH1F("ESig_y_in", "eSigma Method;y;Counts", 100, 0, 1);

    // JB
    JB_x_in = new TH1F("JB_x_in", "JB Method;x;Counts", 100, 1e-5, 1.0);
    JB_Q2_in = new TH1F("JB_Q2_in", "JB Method;Q^{2} [GeV^{2}];Counts", 100, 0, 11);
    JB_y_in = new TH1F("JB_y_in", "JB Method;y;Counts", 100, 0, 1);

    // Double Angle
    DA_x_in = new TH1F("DA_x_in", "DA Method;x;Counts", 100, 1e-5, 1.0);
    DA_Q2_in = new TH1F("DA_Q2_in", "DA Method;Q^{2} [GeV^{2}];Counts", 100, 0, 11);
    DA_y_in = new TH1F("DA_y_in", "DA Method;y;Counts", 100, 0, 1);
    
    //for MC
    h_mc_jpsi_mass = new TH1F("h_mc_jpsi_mass", " ;M_{e^{+}e^{-}} [GeV];Counts", 100, 2.0, 4.0);
    
    // MC positron
    h_mc_positron_px = new TH1F("h_mc_positron_px", "MC positron;P_{x} [GeV/c];Counts", 100, -10, 10);
    h_mc_positron_py = new TH1F("h_mc_positron_py", "MC positron;P_{y} [GeV/c];Counts", 100, -10, 10);
    h_mc_positron_pz = new TH1F("h_mc_positron_pz", "MC positron;P_{z} [GeV/c];Counts", 100, 0, 11);
    h_mc_positron_pt = new TH1F("h_mc_positron_pt", "MC positron;p_{T} [GeV/c];Counts", 100, -1, 5);
    h_mc_positron_E  = new TH1F("h_mc_positron_E",  "MC positron;E [GeV];Counts", 100, 0, 11);
    h_mc_positron_eta =new TH1F("h_mc_positron_eta", "MC positron;#eta;Counts", 100, -5, 5);

    // MC decay electron
    h_mc_decay_electron_px = new TH1F("h_mc_decay_electron_px", "MC decay electron;P_{x} [GeV/c];Counts", 100, -10, 10);
    h_mc_decay_electron_py = new TH1F("h_mc_decay_electron_py", "MC decay electron;P_{y} [GeV/c];Counts", 100, -10, 10);
    h_mc_decay_electron_pz = new TH1F("h_mc_decay_electron_pz", "MC decay electron;P_{z} [GeV/c];Counts", 100, 0, 11);
    h_mc_decay_electron_pt = new TH1F("h_mc_decay_electron_pt", "MC decay electron;p_{T} [GeV/c];Counts", 100, -1, 5);
    h_mc_decay_electron_E  = new TH1F("h_mc_decay_electron_E",  "MC decay electron;E [GeV];Counts", 100, 0, 11);
    h_mc_decay_electron_eta = new TH1F("h_mc_decay_electron_eta", "MC decay electron;#eta;Counts", 100, -5, 5);

    // MC scattered electron
    h_mc_scattered_electron_px = new TH1F("h_mc_scattered_electron_px", "MC scattered electron;P_{x} [GeV/c];Counts", 100, -10, 10);
    h_mc_scattered_electron_py = new TH1F("h_mc_scattered_electron_py", "MC scattered electron;P_{y} [GeV/c];Counts", 100, -10, 10);
    h_mc_scattered_electron_pz = new TH1F("h_mc_scattered_electron_pz", "MC scattered electron;P_{z} [GeV/c];Counts", 100, 0, 11);
    h_mc_scattered_electron_pt = new TH1F("h_mc_scattered_electron_pt", "MC scattered electron;p_{T} [GeV/c];Counts", 100, -1, 5);
    h_mc_scattered_electron_E  = new TH1F("h_mc_scattered_electron_E",  "MC scattered electron;E [GeV];Counts", 100, 0, 11);
    h_mc_scattered_electron_eta = new TH1F("h_mc_scattered_electron_eta", " ;#eta;Counts", 100, -4, 4);

    // MC recoil proton
    h_mc_recoil_proton_px = new TH1F("h_mc_recoil_proton_px", "MC recoil proton;P_{x} [GeV/c];Counts", 100, -4, 4);
    h_mc_recoil_proton_py = new TH1F("h_mc_recoil_proton_py", "MC recoil proton;P_{y} [GeV/c];Counts", 100, -4, 4);
    h_mc_recoil_proton_pz = new TH1F("h_mc_recoil_proton_pz", "MC recoil proton;P_{z} [GeV/c];Counts", 100, 80, 150);
    h_mc_recoil_proton_pt = new TH1F("h_mc_recoil_proton_pt", "MC recoil proton;p_{T} [GeV/c];Counts", 100, -1, 2);
    h_mc_recoil_proton_E  = new TH1F("h_mc_recoil_proton_E",  "MC recoil proton;E [GeV];Counts", 100, 80, 150);
    h_mc_recoil_proton_eta = new TH1F("h_mc_recoil_proton_eta", " ;#eta;Counts", 100, -5, 10);
    
    //for the missing mass
    h_missingMass_MC  = new TH1F("h_missingMass_MC", "MC missing mass;M_{X} [GeV];Counts", 100, -10, 10);
    h_missingMass2_MC = new TH1F("h_missingMass2_MC", "MC missing mass squared;M_{X}^{2} [GeV^{2}];Counts", 100, -20, 20);
    h_missingMass_RP  = new TH1F("h_missingMass_RP", "RP RECO missing mass;M_{X} [GeV];Counts", 100, -10, 10);
    h_missingMass2_RP = new TH1F("h_missingMass2_RP", "RP RECO missing mass squared;M_{X}^{2} [GeV^{2}];Counts", 100, -20, 20);
    h_missingMass_B0  = new TH1F("h_missingMass_B0", "B0 RECO missing mass;M_{X} [GeV];Counts", 100, -10, 10);
    h_missingMass2_B0 = new TH1F("h_missingMass2_B0", "B0 RECO missing mass squared;M_{X}^{2} [GeV^{2}];Counts", 100, -20, 20);
    
    //for RECO level electron method
    h_reco_EM_x = new TH1F("h_reco_EM_x", "RECO Electron Method;x_{B};Counts", 100, 0, 1);
    h_reco_EM_Q2 = new TH1F("h_reco_EM_Q2", "RECO Electron Method;Q^{2} [GeV^{2}];Counts", 100, 0, 11);
    h_reco_EM_y = new TH1F("h_reco_EM_y", "RECO Electron Method;y;Counts", 100, 0, 1);
    
    //JB
    h_reco_JB_x = new TH1F("h_reco_JB_x", "RECO JB;x;Counts", 100, 0, 1);
    h_reco_JB_Q2 = new TH1F("h_reco_JB_Q2", "RECO JB;Q^{2} [GeV^{2}];Counts", 100, 0, 11);
    h_reco_JB_y = new TH1F("h_reco_JB_y", "RECO JB;y;Counts", 100, 0, 1);
    h_reco_JB_HFS_sigma = new TH1F("h_reco_JB_HFS_sigma", "RECO JB HFS;E-p_{z} [GeV];Counts", 100, 0, 30);
    h_reco_JB_HFS_pt = new TH1F("h_reco_JB_HFS_pt", "RECO JB HFS;p_{T} [GeV];Counts", 100, 0, 10);

    //eSigma
    h_reco_eSigma_x = new TH1F("h_reco_eSigma_x", "RECO eSigma;x;Counts", 100, 0, 1);
    h_reco_eSigma_Q2 = new TH1F("h_reco_eSigma_Q2", "RECO eSigma;Q^{2} [GeV^{2}];Counts", 100, 0, 11);
    h_reco_eSigma_y = new TH1F("h_reco_eSigma_y", "RECO eSigma;y;Counts", 100, 0, 1);
    h_reco_eSigma_HFS_sigma = new TH1F("h_reco_eSigma_HFS_sigma", "RECO eSigma;#Sigma_{tot} [GeV];Counts", 100, 0, 30);

    //DA
    h_reco_DA_x = new TH1F("h_reco_DA_x", "RECO DA;x;Counts", 100, 0, 1);
    h_reco_DA_Q2 = new TH1F("h_reco_DA_Q2", "RECO DA;Q^{2} [GeV^{2}];Counts", 100, 0, 11);
    h_reco_DA_y = new TH1F("h_reco_DA_y", "RECO DA;y;Counts", 100, 0, 1);
    
    // fractional residual vs truth: x
    h2_dx_EM     = new TH2F("h2_dx_EM", "Electron Method;x_{truth};(x_{truth}-x_{reco})/x_{truth}", 100, 0, 1, 100, -1, 1);
    h2_dx_JB     = new TH2F("h2_dx_JB", "JB Method;x_{truth};(x_{truth}-x_{reco})/x_{truth}", 100, 0, 1, 100, -1, 1);
    h2_dx_eSigma = new TH2F("h2_dx_eSigma", "eSigma Method;x_{truth};(x_{truth}-x_{reco})/x_{truth}", 100, 0, 1, 100, -1, 1);
    h2_dx_DA     = new TH2F("h2_dx_DA", "DA Method;x_{truth};(x_{truth}-x_{reco})/x_{truth}", 100, 0, 1, 100, -1, 1);

    // fractional residual vs truth: Q2
    h2_dQ2_EM     = new TH2F("h2_dQ2_EM", "Electron Method;Q^{2}_{truth};(Q^{2}_{truth}-Q^{2}_{reco})/Q^{2}_{truth}", 100, 0, 10, 100, -1, 1);
    h2_dQ2_JB     = new TH2F("h2_dQ2_JB", "JB Method;Q^{2}_{truth};(Q^{2}_{truth}-Q^{2}_{reco})/Q^{2}_{truth}", 100, 0, 10, 100, -1, 1);
    h2_dQ2_eSigma = new TH2F("h2_dQ2_eSigma", "eSigma Method;Q^{2}_{truth};(Q^{2}_{truth}-Q^{2}_{reco})/Q^{2}_{truth}", 100, 0, 10, 100, -1, 1);
    h2_dQ2_DA     = new TH2F("h2_dQ2_DA", "DA Method;Q^{2}_{truth};(Q^{2}_{truth}-Q^{2}_{reco})/Q^{2}_{truth}", 100, 0, 10, 100, -1, 1);

    // fractional residual vs truth: y
    h2_dy_EM     = new TH2F("h2_dy_EM", "Electron Method;y_{truth};(y_{truth}-y_{reco})/y_{truth}", 100, 0, 1, 100, -1, 1);
    h2_dy_JB     = new TH2F("h2_dy_JB", "JB Method;y_{truth};(y_{truth}-y_{reco})/y_{truth}", 100, 0, 1, 100, -1, 1);
    h2_dy_eSigma = new TH2F("h2_dy_eSigma", "eSigma Method;y_{truth};(y_{truth}-y_{reco})/y_{truth}", 100, 0, 1, 100, -1, 1);
    h2_dy_DA     = new TH2F("h2_dy_DA", "DA Method;y_{truth};(y_{truth}-y_{reco})/y_{truth}", 100, 0, 1, 100, -1, 1);
    
    // 1D fractional residuals: x
    h_dx_EM     = new TH1F("h_dx_EM", "Electron Method;(x_{truth}-x_{reco})/x_{truth};Counts", 100, -0.4, 0.4);
    h_dx_JB     = new TH1F("h_dx_JB", "JB Method;(x_{truth}-x_{reco})/x_{truth};Counts", 100, -0.4, 0.4);
    h_dx_eSigma = new TH1F("h_dx_eSigma", "eSigma Method;(x_{truth}-x_{reco})/x_{truth};Counts", 100, -0.4, 0.4);
    h_dx_DA     = new TH1F("h_dx_DA", "DA Method;(x_{truth}-x_{reco})/x_{truth};Counts", 100, -0.4, 0.4);

    // 1D fractional residuals: Q2
    h_dQ2_EM     = new TH1F("h_dQ2_EM", "Electron Method;(Q^{2}_{truth}-Q^{2}_{reco})/Q^{2}_{truth};Counts", 100, -0.4, 0.4);
    h_dQ2_JB     = new TH1F("h_dQ2_JB", "JB Method;(Q^{2}_{truth}-Q^{2}_{reco})/Q^{2}_{truth};Counts", 100, -0.4, 0.4);
    h_dQ2_eSigma = new TH1F("h_dQ2_eSigma", "eSigma Method;(Q^{2}_{truth}-Q^{2}_{reco})/Q^{2}_{truth};Counts", 100, -0.4, 0.4);
    h_dQ2_DA     = new TH1F("h_dQ2_DA", "DA Method;(Q^{2}_{truth}-Q^{2}_{reco})/Q^{2}_{truth};Counts", 100, -0.4, 0.4);

    // 1D fractional residuals: y
    h_dy_EM     = new TH1F("h_dy_EM", "Electron Method;(y_{truth}-y_{reco})/y_{truth};Counts", 100, -0.4, 0.4);
    h_dy_JB     = new TH1F("h_dy_JB", "JB Method;(y_{truth}-y_{reco})/y_{truth};Counts", 100, -0.4, 0.4);
    h_dy_eSigma = new TH1F("h_dy_eSigma", "eSigma Method;(y_{truth}-y_{reco})/y_{truth};Counts", 100, -0.4, 0.4);
    h_dy_DA     = new TH1F("h_dy_DA", "DA Method;(y_{truth}-y_{reco})/y_{truth};Counts", 100, -0.4, 0.4);
    
    // truth vs reco: x
    h2_x_EM_res     = new TH2F("h2_x_EM_res", "Electron Method;x_{truth};x_{reco}", 100, 0, 1, 100, 0, 1);
    h2_x_JB_res     = new TH2F("h2_x_JB_res", "JB Method;x_{truth};x_{reco}", 100, 0, 1, 100, 0, 1);
    h2_x_eSigma_res = new TH2F("h2_x_eSigma_res", "eSigma Method;x_{truth};x_{reco}", 100, 0, 1, 100, 0, 1);
    h2_x_DA_res     = new TH2F("h2_x_DA_res", "DA Method;x_{truth};x_{reco}", 100, 0, 1, 100, 0, 1);

    // truth vs reco: Q2
    h2_Q2_EM_res     = new TH2F("h2_Q2_EM_res", "Electron Method;Q^{2}_{truth};Q^{2}_{reco}", 100, 0, 10, 100, 0, 10);
    h2_Q2_JB_res     = new TH2F("h2_Q2_JB_res", "JB Method;Q^{2}_{truth};Q^{2}_{reco}", 100, 0, 10, 100, 0, 10);
    h2_Q2_eSigma_res = new TH2F("h2_Q2_eSigma_res", "eSigma Method;Q^{2}_{truth};Q^{2}_{reco}", 100, 0, 10, 100, 0, 10);
    h2_Q2_DA_res     = new TH2F("h2_Q2_DA_res", "DA Method;Q^{2}_{truth};Q^{2}_{reco}", 100, 0, 10, 100, 0, 10);

    // truth vs reco: y
    h2_y_EM_res     = new TH2F("h2_y_EM_res", "Electron Method;y_{truth};y_{reco}", 100, 0, 1, 100, 0, 1);
    h2_y_JB_res     = new TH2F("h2_y_JB_res", "JB Method;y_{truth};y_{reco}", 100, 0, 1, 100, 0, 1);
    h2_y_eSigma_res = new TH2F("h2_y_eSigma_res", "eSigma Method;y_{truth};y_{reco}", 100, 0, 1, 100, 0, 1);
    h2_y_DA_res     = new TH2F("h2_y_DA_res", "DA Method;y_{truth};y_{reco}", 100, 0, 1, 100, 0, 1);
    
    //---------------------------------------------
    // Electron energy resolution
    //---------------------------------------------
    h_CalEnergy_minus_SCElecEnergy = new TH1F("h_CalEnergy_minus_SCElecEnergy", ";(E_{MC}-E_{cal})/E_{MC};Counts", 200, -1, 1);
    h_TrackEnergy_minus_SCElecEnergy = new TH1F("h_TrackEnergy_minus_SCElecEnergy", ";(E_{MC}-E_{track})/E_{MC};Counts", 200, -1, 1);
    h_Cal_DE_over_E_vs_Eta = new TH2F("h_Cal_DE_over_E_vs_Eta", ";#eta;(E_{MC}-E_{cal})/E_{MC}", 100, -4, 0, 200, -1, 1);
    h_Track_DE_over_E_vs_Eta = new TH2F( "h_Track_DE_over_E_vs_Eta", ";#eta;(E_{MC}-E_{track})/E_{MC}", 100, -4, 0, 200, -1, 1);
    
    //for the mc and reco hfs
    h_mc_e_minus_pz = new TH1F("h_mc_e_minus_pz", ";E-P_{z} [GeV];Counts", 200, 0, 40);
    h_reco_e_minus_pz = new TH1F("h_reco_e_minus_pz", ";E-P_{z} [GeV];Counts", 200, 0, 40);
    
    //for t-distribution
    h_t_MC       = new TH1F("h_t_MC", "MC -t distribution;-t [GeV^{2}];Counts", 200, 0, 5);
    h_t_RECO_RP = new TH1F("h_t_RECO_RP", "RECO RP -t distribution;-t [GeV^{2}];Counts", 200, 0, 5);
    h_t_RECO_B0 = new TH1F("h_t_RECO_B0", "RECO B0 -t distribution;-t [GeV^{2}];Counts", 200, 0, 5);
    h_t_RECO_RPB0 = new TH1F("h_t_RECO_RPB0", "RECO RP+B0 combined -t distribution;-t [GeV^{2}];Counts", 200, 0, 5);
    
    h_t2D_RP = new TH2F("h_t2D_RP", "Truth vs RP reconstructed -t;Truth -t [GeV^{2}];Reco -t [GeV^{2}]", 200, 0, 5, 200, 0, 5);
    h_t2D_B0 = new TH2F("h_t2D_B0", "Truth vs B0 reconstructed -t;Truth -t [GeV^{2}];Reco -t [GeV^{2}]", 200, 0, 5, 200, 0, 5);
    h_t2D_RPB0 = new TH2F("h_t2D_RPB0", "Truth vs RP+B0 reconstructed -t;Truth -t [GeV^{2}];Reco -t [GeV^{2}]", 200, 0, 5, 200, 0, 5);
    
    h_t_MethodL_RECO = new TH1F("h_t_MethodL_RECO", "Method L reconstructed -t;-t [GeV^{2}];Counts", 200, 0, 5);
    h_t2D_MethodL = new TH2F("h_t2D_MethodL", "Truth vs Method L reconstructed -t;Truth -t [GeV^{2}];Reco -t [GeV^{2}]", 200, 0, 5, 200, 0, 5);
    
    //combining t for RP, B0 and Method L
    h_t_RECO_RPB0_MethodL = new TH1F("h_t_RECO_RPB0_MethodL", "RECO RP+B0+MethodL -t;-t [GeV^{2}];Counts", 200, 0, 5);
    h_t2D_RPB0_MethodL = new TH2F("h_t2D_RPB0_MethodL", "Truth vs RECO RP+B0+MethodL -t;Truth -t [GeV^{2}];Reco -t [GeV^{2}]", 200, 0, 5, 100, 0, 5);
    
    h_t_lAger_accept = new TH1F("h_t_lAger_accept", "Accepted truth RP+B0+MethodL -t;-t [GeV^{2}];Counts", 200, 0, 5);
    
}

//---------------------------------------------
// Write histograms
//---------------------------------------------
void WriteHistograms(TFile *outFile)
{
    outFile->cd();
    
    //for the generator
    h_t_lAger->Write();
    h_xb_lAger->Write();
    h_Q2_lAger->Write();
    h_y_lAger->Write();
    
    //for the RP
    RPfinalProton->Write();
    B0RPfinalProton->Write();
    h_pt_RomanPots->Write();
    h_pt_B0RomanPots->Write();
    h_px_RomanPots->Write();
    h_py_RomanPots->Write();
    h_pz_RomanPots->Write();
    h_energ_RomanPots->Write();
    ThetaRP->Write();
    ThetaRPB0->Write();
    h_rp_occupancy_map->Write();
    
    //for the B0
    B0finalProton->Write();
    h_px_B0_track->Write();
    h_py_B0_track->Write();
    h_pt_B0_track->Write();
    h_pz_B0_track->Write();
    h_energ_B0_track->Write();
    ThetaB0->Write();
    h_B0_ref_z->Write();
    h_B0_occupancy_map_layer_0->Write();
    h_B0_occupancy_map_layer_1->Write();
    h_B0_occupancy_map_layer_2->Write();
    h_B0_occupancy_map_layer_3->Write();

    h_B0_hit_energy_deposit->Write();

    h_B0_emcal_occupancy_map->Write();
    h_B0_emcal_cluster_energy->Write();
    
    //for the RECO electron
    h_jpsi_mass_RC->Write();
    h_scattered_electron_energy_RC->Write();
    h_scattered_electron_eta_RC->Write();
    h_scattered_electron_pt_RC->Write();
    h_scattered_positron_eta_RC->Write();
    h_decay_electron_eta_RC->Write();
    
    //for calorimeter
    h_matched_ecal_energy->Write();
    h_matched_ecal_dr->Write();
    h_matched_eop->Write();
    
    //for inclusive variables
    EM_x->Write();
    EM_Q2->Write();
    EM_y->Write();

    ESig_x_in->Write();
    ESig_Q2_in->Write();
    ESig_y_in->Write();

    JB_x_in->Write();
    JB_Q2_in->Write();
    JB_y_in->Write();

    DA_x_in->Write();
    DA_Q2_in->Write();
    DA_y_in->Write();
    
    //for MC
    h_mc_jpsi_mass->Write();
    
    h_mc_positron_px->Write();
    h_mc_positron_py->Write();
    h_mc_positron_pz->Write();
    h_mc_positron_pt->Write();
    h_mc_positron_E->Write();
    h_mc_positron_eta->Write();

    h_mc_decay_electron_px->Write();
    h_mc_decay_electron_py->Write();
    h_mc_decay_electron_pz->Write();
    h_mc_decay_electron_pt->Write();
    h_mc_decay_electron_E->Write();
    h_mc_decay_electron_eta->Write();

    h_mc_scattered_electron_px->Write();
    h_mc_scattered_electron_py->Write();
    h_mc_scattered_electron_pz->Write();
    h_mc_scattered_electron_pt->Write();
    h_mc_scattered_electron_E->Write();
    h_mc_scattered_electron_eta->Write();

    h_mc_recoil_proton_px->Write();
    h_mc_recoil_proton_py->Write();
    h_mc_recoil_proton_pz->Write();
    h_mc_recoil_proton_pt->Write();
    h_mc_recoil_proton_E->Write();
    h_mc_recoil_proton_eta->Write();
    
    //for the missing mass
    h_missingMass_MC->Write();
    h_missingMass2_MC->Write();
    h_missingMass_RP->Write();
    h_missingMass2_RP->Write();
    h_missingMass_B0->Write();
    h_missingMass2_B0->Write();
    
    //for RECO level electron method
    h_reco_EM_x->Write();
    h_reco_EM_Q2->Write();
    h_reco_EM_y->Write();
    
    //JB
    h_reco_JB_x->Write();
    h_reco_JB_Q2->Write();
    h_reco_JB_y->Write();
    h_reco_JB_HFS_sigma->Write();
    h_reco_JB_HFS_pt->Write();

    //eSigma
    h_reco_eSigma_x->Write();
    h_reco_eSigma_Q2->Write();
    h_reco_eSigma_y->Write();
    h_reco_eSigma_HFS_sigma->Write();

    //DA
    h_reco_DA_x->Write();
    h_reco_DA_Q2->Write();
    h_reco_DA_y->Write();
    
    h2_dx_EM->Write();      h2_dx_JB->Write();      h2_dx_eSigma->Write();      h2_dx_DA->Write();
    h2_dQ2_EM->Write();     h2_dQ2_JB->Write();     h2_dQ2_eSigma->Write();     h2_dQ2_DA->Write();
    h2_dy_EM->Write();      h2_dy_JB->Write();      h2_dy_eSigma->Write();      h2_dy_DA->Write();

    h_dx_EM->Write();       h_dx_JB->Write();       h_dx_eSigma->Write();       h_dx_DA->Write();
    h_dQ2_EM->Write();      h_dQ2_JB->Write();      h_dQ2_eSigma->Write();      h_dQ2_DA->Write();
    h_dy_EM->Write();       h_dy_JB->Write();       h_dy_eSigma->Write();       h_dy_DA->Write();

    h2_x_EM_res->Write();   h2_x_JB_res->Write();   h2_x_eSigma_res->Write();   h2_x_DA_res->Write();
    h2_Q2_EM_res->Write();  h2_Q2_JB_res->Write();  h2_Q2_eSigma_res->Write();  h2_Q2_DA_res->Write();
    h2_y_EM_res->Write();   h2_y_JB_res->Write();   h2_y_eSigma_res->Write();   h2_y_DA_res->Write();
    
    //tack and calorimeter resolution
    h_CalEnergy_minus_SCElecEnergy->Write();
    h_TrackEnergy_minus_SCElecEnergy->Write();
    h_Cal_DE_over_E_vs_Eta->Write();
    h_Track_DE_over_E_vs_Eta->Write();
    
    //for the mc and reco hfs
    h_mc_e_minus_pz->Write();
    h_reco_e_minus_pz->Write();
    
    //for the t-distribution
    h_t_MC->Write();
    h_t_RECO_RP->Write();
    h_t_RECO_B0->Write();
    h_t_RECO_RPB0->Write();
    
    h_t2D_RP->Write();
    h_t2D_B0->Write();
    h_t2D_RPB0->Write();
    
    h_t_MethodL_RECO->Write();
    h_t2D_MethodL->Write();
    
    h_t_RECO_RPB0_MethodL->Write();
    h_t2D_RPB0_MethodL->Write();
    
    //corrections
    h_t_lAger_accept->Write();
    if (h_CorrectionFactor) h_CorrectionFactor->Write();
    if (h_t_RPB0_MethodL_corrected) h_t_RPB0_MethodL_corrected->Write();
}




#endif
