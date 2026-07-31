#ifndef DVMP_PLOT_H
#define DVMP_PLOT_H

#include <TH1F.h>
#include <TH2F.h>
#include <TFile.h>

//---------------------------------------------
// Generator level (lAger)
//---------------------------------------------
TH1F *h_t_lAger;
TH1F *h_xb_lAger;
TH1F *h_Q2_lAger;
TH1F *h_y_lAger;

//---------------------------------------------
// Roman Pots
//---------------------------------------------
TH1F *RPfinalProton;
TH1F *B0RPfinalProton;

TH1F *h_pt_RomanPots;
TH1F *h_pt_RPB0combined;   // renamed from h_pt_B0RomanPots — filled by both RP and B0

TH1F *h_px_RomanPots;
TH1F *h_py_RomanPots;
TH1F *h_pz_RomanPots;
TH1F *h_energ_RomanPots;

TH1F *ThetaRP;
TH1F *ThetaRPB0;

TH2F *h_rp_occupancy_map;

//---------------------------------------------
// B0 tracker
//---------------------------------------------
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

//---------------------------------------------
// RECO electron / J/psi
//---------------------------------------------
TH1F *h_jpsi_mass_RC;
TH1F *h_scattered_electron_energy_RC;
TH1F *h_scattered_electron_eta_RC;
TH1F *h_scattered_electron_pt_RC;
TH1F *h_scattered_positron_eta_RC;
TH1F *h_decay_electron_eta_RC;

//---------------------------------------------
// Calorimeter matching
//---------------------------------------------
TH1F *h_matched_ecal_energy;
TH1F *h_matched_ecal_dr;
TH1F *h_matched_eop;

//---------------------------------------------
// Inclusive kinematics (EICRECON collections)
//---------------------------------------------
TH1F *EM_x,      *EM_Q2,      *EM_y;
TH1F *ESig_x_in, *ESig_Q2_in, *ESig_y_in;
TH1F *JB_x_in,   *JB_Q2_in,   *JB_y_in;
TH1F *DA_x_in,   *DA_Q2_in,   *DA_y_in;

//---------------------------------------------
// MC truth
//---------------------------------------------
TH1F *h_mc_jpsi_mass;

TH1F *h_mc_positron_px, *h_mc_positron_py, *h_mc_positron_pz;
TH1F *h_mc_positron_pt, *h_mc_positron_E,  *h_mc_positron_eta;

TH1F *h_mc_decay_electron_px, *h_mc_decay_electron_py, *h_mc_decay_electron_pz;
TH1F *h_mc_decay_electron_pt, *h_mc_decay_electron_E,  *h_mc_decay_electron_eta;

TH1F *h_mc_scattered_electron_px, *h_mc_scattered_electron_py, *h_mc_scattered_electron_pz;
TH1F *h_mc_scattered_electron_pt, *h_mc_scattered_electron_E,  *h_mc_scattered_electron_eta;

TH1F *h_mc_recoil_proton_px, *h_mc_recoil_proton_py, *h_mc_recoil_proton_pz;
TH1F *h_mc_recoil_proton_pt, *h_mc_recoil_proton_E,  *h_mc_recoil_proton_eta;

//---------------------------------------------
// Missing mass / exclusivity
//---------------------------------------------
TH1F *h_missingMass_MC,  *h_missingMass2_MC;
TH1F *h_missingMass_RP,  *h_missingMass2_RP;
TH1F *h_missingMass_B0,  *h_missingMass2_B0;

//---------------------------------------------
// RECO kinematics (hand-computed)
//---------------------------------------------
TH1F *h_reco_EM_x,     *h_reco_EM_Q2,     *h_reco_EM_y;

TH1F *h_reco_JB_x,     *h_reco_JB_Q2,     *h_reco_JB_y;
TH1F *h_reco_JB_HFS_sigma, *h_reco_JB_HFS_pt;

TH1F *h_reco_eSigma_x, *h_reco_eSigma_Q2, *h_reco_eSigma_y;
TH1F *h_reco_eSigma_HFS_sigma;

TH1F *h_reco_DA_x,     *h_reco_DA_Q2,     *h_reco_DA_y;

//---------------------------------------------
// Kinematic resolution: 2D fractional residual (truth vs frac)
//---------------------------------------------
TH2F *h2_dx_EM,  *h2_dx_JB,  *h2_dx_eSigma,  *h2_dx_DA;
TH2F *h2_dQ2_EM, *h2_dQ2_JB, *h2_dQ2_eSigma, *h2_dQ2_DA;
TH2F *h2_dy_EM,  *h2_dy_JB,  *h2_dy_eSigma,  *h2_dy_DA;

// 1D fractional residuals
TH1F *h_dx_EM,  *h_dx_JB,  *h_dx_eSigma,  *h_dx_DA;
TH1F *h_dQ2_EM, *h_dQ2_JB, *h_dQ2_eSigma, *h_dQ2_DA;
TH1F *h_dy_EM,  *h_dy_JB,  *h_dy_eSigma,  *h_dy_DA;

// Truth vs reco 2D migration
TH2F *h2_x_EM_res,  *h2_x_JB_res,  *h2_x_eSigma_res,  *h2_x_DA_res;
TH2F *h2_Q2_EM_res, *h2_Q2_JB_res, *h2_Q2_eSigma_res, *h2_Q2_DA_res;
TH2F *h2_y_EM_res,  *h2_y_JB_res,  *h2_y_eSigma_res,  *h2_y_DA_res;

//---------------------------------------------
// -t resolution histograms (new — required by FillKinematicResolutionPlots)
//---------------------------------------------
TH2F *h2_dt_RP,      *h2_dt_B0,      *h2_dt_MethodL;   // frac residual vs truth
TH1F *h_dt_RP,       *h_dt_B0,       *h_dt_MethodL;    // 1D frac residuals
TH2F *h2_t_RP_res,   *h2_t_B0_res,   *h2_t_MethodL_res; // truth vs reco migration

//---------------------------------------------
// Electron energy resolution
//---------------------------------------------
TH1F *h_CalEnergy_minus_SCElecEnergy;
TH1F *h_TrackEnergy_minus_SCElecEnergy;
TH2F *h_Cal_DE_over_E_vs_Eta;
TH2F *h_Track_DE_over_E_vs_Eta;

TH2F *h2_reco_Q2_vs_x;

//---------------------------------------------
// E - Pz conservation
//---------------------------------------------
TH1F *h_mc_e_minus_pz;
TH1F *h_reco_e_minus_pz;

//---------------------------------------------
// t-distributions
//---------------------------------------------
TH1F *h_t_MC;
TH1F *h_t_RECO_RP, *h_t_RECO_B0, *h_t_RECO_RPB0;

TH2F *h_t2D_RP, *h_t2D_B0, *h_t2D_RPB0;

TH1F *h_t_MethodL_RECO;
TH2F *h_t2D_MethodL;

TH1F *h_t_RPB0_accept;

TH1F *h_t_RECO_RPB0_MethodL;
TH2F *h_t2D_RPB0_MethodL;

TH1F *h_methodL_RP_MethodL_B0;
TH2F *h2_methodL_RP_MethodL_B0;

// Acceptance correction
TH1F *h_t_lAger_accept;
TH1F *h_t_hybrid_accept;
TH1F *h_CorrectionFactor         = nullptr;
TH1F *h_eff_RP_B0         = nullptr;
TH1F *h_eff_MethodL_RP_MethodL_B0         = nullptr;
TH1F *h_t_RPB0_MethodL_corrected = nullptr;
TH1F *h_t_RPB0_eff_applied = nullptr;
TH1F *h_methodL_RP_MethodL_B0_eff_applied = nullptr;
TH1F *h_HybridCorrectionFactor = nullptr;
TH1F *h_methodL_RP_MethodL_B0_corrected = nullptr;

TH1F *h_sanity_methL_miss_cut;
TH1F *h_sanity_methL_miss_all;

TH1F *h_EXCLU_jpsi_mass;

//---------------------------------------------
// InitHistograms
//---------------------------------------------
void InitHistograms()
{
    const double xmax  = 0.02;
    const double tmax  = 5.0;
    const int    tBins = 200;
    
    //--------------------------------------------------
    // Generator level
    //--------------------------------------------------
    h_t_lAger  = new TH1F("h_t_lAger",  "-t distribution;-t [GeV^{2}];Counts",        tBins, 0, tmax);
    h_xb_lAger = new TH1F("h_xb_lAger", "x_{B} distribution;x_{B};Counts",             100,  0, xmax);
    h_Q2_lAger = new TH1F("h_Q2_lAger", "Q^{2} distribution;Q^{2} [GeV^{2}];Counts",  100,  0, 11);
    h_y_lAger  = new TH1F("h_y_lAger",  "y distribution;y;Counts",                     100,  0, 1);
    
    //--------------------------------------------------
    // Roman Pots
    //--------------------------------------------------
    RPfinalProton    = new TH1F("RPfinalProton",    ";#eta;Counts",           100, -5,  10);
    B0RPfinalProton  = new TH1F("B0RPfinalProton",  ";#eta;Counts",           100, -5,  10);
    h_pt_RomanPots   = new TH1F("h_pt_RomanPots",   ";p_{T} [GeV/c];Counts", 100, -1,   2);
    // Combined RP+B0 pT — filled by both ProcessRomanPots and ProcessB0
    h_pt_RPB0combined = new TH1F("h_pt_RPB0combined", ";p_{T} [GeV/c];Counts", 100, -1, 2);
    h_px_RomanPots   = new TH1F("h_px_RomanPots",   ";P_{x} [GeV/c];Counts", 100, -4,   4);
    h_py_RomanPots   = new TH1F("h_py_RomanPots",   ";P_{y} [GeV/c];Counts", 100, -4,   4);
    h_pz_RomanPots   = new TH1F("h_pz_RomanPots",   ";P_{z} [GeV/c];Counts", 100, 80, 150);
    h_energ_RomanPots= new TH1F("h_energ_RomanPots", ";Energy [GeV];Counts",  100, 80, 150);
    
    ThetaRP   = new TH1F("ThetaRP",   ";#theta_{RP} [rad];Counts",     100, -0.001, 0.025);
    ThetaRPB0 = new TH1F("ThetaRPB0", ";#theta_{RP+B0} [rad];Counts",  100, -0.001, 0.025);
    
    h_rp_occupancy_map = new TH2F("h_rp_occupancy_map",
                                  "Roman Pots occupancy;hit x [mm];hit y [mm]",
                                  100, -1300, -1000, 100, -40, 40);
    
    //--------------------------------------------------
    // B0 tracker
    //--------------------------------------------------
    B0finalProton    = new TH1F("B0finalProton",    ";#eta;Counts",           100, -5,  10);
    h_px_B0_track    = new TH1F("h_px_B0_track",    ";P_{x} [GeV/c];Counts", 100, -4,   4);
    h_py_B0_track    = new TH1F("h_py_B0_track",    ";P_{y} [GeV/c];Counts", 100, -4,   4);
    h_pt_B0_track    = new TH1F("h_pt_B0_track",    ";p_{T} [GeV/c];Counts", 100, -1,   2);
    h_pz_B0_track    = new TH1F("h_pz_B0_track",    ";P_{z} [GeV/c];Counts", 100, 80, 150);
    h_energ_B0_track = new TH1F("h_energ_B0_track", ";Energy [GeV];Counts",  100, 80, 150);
    
    ThetaB0   = new TH1F("ThetaB0", ";#theta_{B0} [rad];Counts", 100, -0.001, 0.025);
    
    // ref_z of the selected B0 proton track (filled after selection, not in loop)
    h_B0_ref_z = new TH1F("h_B0_ref_z",
                          "B0 selected proton reference z;z [mm];Counts", 200, -5000, 5000);
    
    h_B0_occupancy_map_layer_0 = new TH2F("h_B0_occupancy_map_layer_0",
                                          "B0 tracker layer 0;x [mm];y [mm]", 200, -350, 0, 200, -200, 200);
    h_B0_occupancy_map_layer_1 = new TH2F("h_B0_occupancy_map_layer_1",
                                          "B0 tracker layer 1;x [mm];y [mm]", 200, -350, 0, 200, -200, 200);
    h_B0_occupancy_map_layer_2 = new TH2F("h_B0_occupancy_map_layer_2",
                                          "B0 tracker layer 2;x [mm];y [mm]", 200, -350, 0, 200, -200, 200);
    h_B0_occupancy_map_layer_3 = new TH2F("h_B0_occupancy_map_layer_3",
                                          "B0 tracker layer 3;x [mm];y [mm]", 200, -350, 0, 200, -200, 200);
    
    h_B0_hit_energy_deposit = new TH1F("h_B0_hit_energy_deposit",
                                       "B0 tracker hit energy deposit;E_{dep} [keV];Counts", 200, 0, 200);
    
    h_B0_emcal_occupancy_map = new TH2F("h_B0_emcal_occupancy_map",
                                        "B0 EMCAL occupancy;x [mm];y [mm]", 200, -350, 0, 200, -200, 200);
    h_B0_emcal_cluster_energy = new TH1F("h_B0_emcal_cluster_energy",
                                         "B0 EMCAL cluster energy;E [GeV];Counts", 200, 0, 50);
    
    //--------------------------------------------------
    // RECO electron / J/psi
    //--------------------------------------------------
    h_jpsi_mass_RC = new TH1F("h_jpsi_mass_RC",
                              "RECO J/#psi mass;M_{e^{+}e^{-}} [GeV];Counts", 100, 2.0, 4.0);
    h_scattered_electron_energy_RC = new TH1F("h_scattered_electron_energy_RC",
                                              "RECO scattered e^{-};E [GeV];Counts", 100, 0, 11);
    h_scattered_electron_eta_RC = new TH1F("h_scattered_electron_eta_RC",
                                           "RECO scattered e^{-};#eta;Counts", 100, -4, 4);
    h_scattered_electron_pt_RC = new TH1F("h_scattered_electron_pt_RC",
                                          "RECO scattered e^{-};p_{T} [GeV];Counts", 100, 0, 5);
    h_scattered_positron_eta_RC = new TH1F("h_scattered_positron_eta_RC",
                                           "RECO J/#psi decay e^{+};#eta;Counts", 100, -5, 5);
    h_decay_electron_eta_RC = new TH1F("h_decay_electron_eta_RC",
                                       "RECO J/#psi decay e^{-};#eta;Counts", 100, -5, 5);
    
    //--------------------------------------------------
    // Calorimeter matching
    //--------------------------------------------------
    h_matched_ecal_energy = new TH1F("h_matched_ecal_energy",
                                     "Matched ECAL cluster;E [GeV];Counts", 100, 0, 11);
    h_matched_ecal_dr = new TH1F("h_matched_ecal_dr",
                                 "Matched ECAL cluster;#DeltaR;Counts", 100, 0, 1);
    h_matched_eop = new TH1F("h_matched_eop",
                             "Matched ECAL cluster;E/p;Counts", 100, 0, 2);
    
    //--------------------------------------------------
    // Inclusive kinematics (EICRECON)
    //--------------------------------------------------
    EM_x   = new TH1F("EM_x",   "Electron Method;x;Counts",             100, 1e-5, xmax);
    EM_Q2  = new TH1F("EM_Q2",  "Electron Method;Q^{2} [GeV^{2}];Counts", 100, 0, 11);
    EM_y   = new TH1F("EM_y",   "Electron Method;y;Counts",             100, 0, 1);
    
    ESig_x_in  = new TH1F("ESig_x_in",  "eSigma Method;x;Counts",              100, 1e-5, xmax);
    ESig_Q2_in = new TH1F("ESig_Q2_in", "eSigma Method;Q^{2} [GeV^{2}];Counts", 100, 0, 11);
    ESig_y_in  = new TH1F("ESig_y_in",  "eSigma Method;y;Counts",              100, 0, 1);
    
    JB_x_in  = new TH1F("JB_x_in",  "JB Method;x;Counts",              100, 1e-5, xmax);
    JB_Q2_in = new TH1F("JB_Q2_in", "JB Method;Q^{2} [GeV^{2}];Counts", 100, 0, 11);
    JB_y_in  = new TH1F("JB_y_in",  "JB Method;y;Counts",              100, 0, 1);
    
    DA_x_in  = new TH1F("DA_x_in",  "DA Method;x;Counts",              100, 1e-5, xmax);
    DA_Q2_in = new TH1F("DA_Q2_in", "DA Method;Q^{2} [GeV^{2}];Counts", 100, 0, 11);
    DA_y_in  = new TH1F("DA_y_in",  "DA Method;y;Counts",              100, 0, 1);
    
    //--------------------------------------------------
    // MC truth
    //--------------------------------------------------
    h_mc_jpsi_mass = new TH1F("h_mc_jpsi_mass",
                              "MC J/#psi mass;M_{e^{+}e^{-}} [GeV];Counts", 100, 2.0, 4.0);
    
    h_mc_positron_px  = new TH1F("h_mc_positron_px",  "MC e^{+};P_{x} [GeV/c];Counts", 100, -10, 10);
    h_mc_positron_py  = new TH1F("h_mc_positron_py",  "MC e^{+};P_{y} [GeV/c];Counts", 100, -10, 10);
    h_mc_positron_pz  = new TH1F("h_mc_positron_pz",  "MC e^{+};P_{z} [GeV/c];Counts", 100,   0, 11);
    h_mc_positron_pt  = new TH1F("h_mc_positron_pt",  "MC e^{+};p_{T} [GeV/c];Counts", 100,   0,  5);
    h_mc_positron_E   = new TH1F("h_mc_positron_E",   "MC e^{+};E [GeV];Counts",        100,   0, 11);
    h_mc_positron_eta = new TH1F("h_mc_positron_eta", "MC e^{+};#eta;Counts",            100,  -5,  5);
    
    h_mc_decay_electron_px  = new TH1F("h_mc_decay_electron_px",  "MC decay e^{-};P_{x} [GeV/c];Counts", 100, -10, 10);
    h_mc_decay_electron_py  = new TH1F("h_mc_decay_electron_py",  "MC decay e^{-};P_{y} [GeV/c];Counts", 100, -10, 10);
    h_mc_decay_electron_pz  = new TH1F("h_mc_decay_electron_pz",  "MC decay e^{-};P_{z} [GeV/c];Counts", 100,   0, 11);
    h_mc_decay_electron_pt  = new TH1F("h_mc_decay_electron_pt",  "MC decay e^{-};p_{T} [GeV/c];Counts", 100,   0,  5);
    h_mc_decay_electron_E   = new TH1F("h_mc_decay_electron_E",   "MC decay e^{-};E [GeV];Counts",        100,   0, 11);
    h_mc_decay_electron_eta = new TH1F("h_mc_decay_electron_eta", "MC decay e^{-};#eta;Counts",            100,  -5,  5);
    
    h_mc_scattered_electron_px  = new TH1F("h_mc_scattered_electron_px",  "MC scattered e^{-};P_{x} [GeV/c];Counts", 100, -10, 10);
    h_mc_scattered_electron_py  = new TH1F("h_mc_scattered_electron_py",  "MC scattered e^{-};P_{y} [GeV/c];Counts", 100, -10, 10);
    h_mc_scattered_electron_pz  = new TH1F("h_mc_scattered_electron_pz",  "MC scattered e^{-};P_{z} [GeV/c];Counts", 100,   0, 11);
    h_mc_scattered_electron_pt  = new TH1F("h_mc_scattered_electron_pt",  "MC scattered e^{-};p_{T} [GeV/c];Counts", 100,   0,  5);
    h_mc_scattered_electron_E   = new TH1F("h_mc_scattered_electron_E",   "MC scattered e^{-};E [GeV];Counts",        100,   0, 11);
    h_mc_scattered_electron_eta = new TH1F("h_mc_scattered_electron_eta", "MC scattered e^{-};#eta;Counts",            100,  -4,  4);
    
    h_mc_recoil_proton_px  = new TH1F("h_mc_recoil_proton_px",  "MC recoil p;P_{x} [GeV/c];Counts", 100,  -4,   4);
    h_mc_recoil_proton_py  = new TH1F("h_mc_recoil_proton_py",  "MC recoil p;P_{y} [GeV/c];Counts", 100,  -4,   4);
    h_mc_recoil_proton_pz  = new TH1F("h_mc_recoil_proton_pz",  "MC recoil p;P_{z} [GeV/c];Counts", 100,  80, 150);
    h_mc_recoil_proton_pt  = new TH1F("h_mc_recoil_proton_pt",  "MC recoil p;p_{T} [GeV/c];Counts", 100,   0,   2);
    h_mc_recoil_proton_E   = new TH1F("h_mc_recoil_proton_E",   "MC recoil p;E [GeV];Counts",        100,  80, 150);
    h_mc_recoil_proton_eta = new TH1F("h_mc_recoil_proton_eta", "MC recoil p;#eta;Counts",            100,  -5,  10);
    
    //--------------------------------------------------
    // Missing mass
    //--------------------------------------------------
    h_missingMass_MC  = new TH1F("h_missingMass_MC",  "MC M_{X};M_{X} [GeV];Counts",          100, -10, 10);
    h_missingMass2_MC = new TH1F("h_missingMass2_MC", "MC M_{X}^{2};M_{X}^{2} [GeV^{2}];Counts", 100, -20, 20);
    h_missingMass_RP  = new TH1F("h_missingMass_RP",  "RP RECO M_{X};M_{X} [GeV];Counts",     100, -10, 10);
    h_missingMass2_RP = new TH1F("h_missingMass2_RP", "RP RECO M_{X}^{2};M_{X}^{2} [GeV^{2}];Counts", 100, -20, 20);
    h_missingMass_B0  = new TH1F("h_missingMass_B0",  "B0 RECO M_{X};M_{X} [GeV];Counts",     100, -10, 10);
    h_missingMass2_B0 = new TH1F("h_missingMass2_B0", "B0 RECO M_{X}^{2};M_{X}^{2} [GeV^{2}];Counts", 100, -20, 20);
    
    //--------------------------------------------------
    // RECO kinematics (hand-computed)
    //--------------------------------------------------
    h_reco_EM_x  = new TH1F("h_reco_EM_x",  "RECO EM;x_{B};Counts",              100, 0, xmax);
    h_reco_EM_Q2 = new TH1F("h_reco_EM_Q2", "RECO EM;Q^{2} [GeV^{2}];Counts",   100, 0, 11);
    h_reco_EM_y  = new TH1F("h_reco_EM_y",  "RECO EM;y;Counts",                  100, 0, 1);
    
    h_reco_JB_x        = new TH1F("h_reco_JB_x",        "RECO JB;x;Counts",                100, 0, xmax);
    h_reco_JB_Q2       = new TH1F("h_reco_JB_Q2",       "RECO JB;Q^{2} [GeV^{2}];Counts", 100, 0, 11);
    h_reco_JB_y        = new TH1F("h_reco_JB_y",        "RECO JB;y;Counts",                100, 0, 1);
    h_reco_JB_HFS_sigma= new TH1F("h_reco_JB_HFS_sigma","RECO JB HFS;E-p_{z} [GeV];Counts",100, 0, 30);
    h_reco_JB_HFS_pt   = new TH1F("h_reco_JB_HFS_pt",  "RECO JB HFS;p_{T} [GeV];Counts",  100, 0, 10);
    
    h_reco_eSigma_x        = new TH1F("h_reco_eSigma_x",       "RECO eSigma;x;Counts",                 100, 0, xmax);
    h_reco_eSigma_Q2       = new TH1F("h_reco_eSigma_Q2",      "RECO eSigma;Q^{2} [GeV^{2}];Counts",  100, 0, 11);
    h_reco_eSigma_y        = new TH1F("h_reco_eSigma_y",       "RECO eSigma;y;Counts",                 100, 0, 1);
    h_reco_eSigma_HFS_sigma= new TH1F("h_reco_eSigma_HFS_sigma","RECO eSigma;#Sigma_{tot} [GeV];Counts",100, 0, 30);
    
    h_reco_DA_x  = new TH1F("h_reco_DA_x",  "RECO DA;x;Counts",                100, 0, xmax);
    h_reco_DA_Q2 = new TH1F("h_reco_DA_Q2", "RECO DA;Q^{2} [GeV^{2}];Counts", 100, 0, 11);
    h_reco_DA_y  = new TH1F("h_reco_DA_y",  "RECO DA;y;Counts",                100, 0, 1);
    
    //--------------------------------------------------
    // x, Q2, y resolution: 2D fractional residual
    //--------------------------------------------------
    h2_dx_EM     = new TH2F("h2_dx_EM",     "EM;x_{truth};(x_{t}-x_{r})/x_{t}",     100, 0, xmax, 100, -1, 1);
    h2_dx_JB     = new TH2F("h2_dx_JB",     "JB;x_{truth};(x_{t}-x_{r})/x_{t}",     100, 0, xmax, 100, -1, 1);
    h2_dx_eSigma = new TH2F("h2_dx_eSigma", "eSigma;x_{truth};(x_{t}-x_{r})/x_{t}", 100, 0, xmax, 100, -1, 1);
    h2_dx_DA     = new TH2F("h2_dx_DA",     "DA;x_{truth};(x_{t}-x_{r})/x_{t}",     100, 0, xmax, 100, -1, 1);
    
    h2_dQ2_EM     = new TH2F("h2_dQ2_EM",     "EM;Q^{2}_{t} [GeV^{2}];(Q^{2}_{t}-Q^{2}_{r})/Q^{2}_{t}",     100, 0, 10, 100, -1, 1);
    h2_dQ2_JB     = new TH2F("h2_dQ2_JB",     "JB;Q^{2}_{t} [GeV^{2}];(Q^{2}_{t}-Q^{2}_{r})/Q^{2}_{t}",     100, 0, 10, 100, -1, 1);
    h2_dQ2_eSigma = new TH2F("h2_dQ2_eSigma", "eSigma;Q^{2}_{t} [GeV^{2}];(Q^{2}_{t}-Q^{2}_{r})/Q^{2}_{t}", 100, 0, 10, 100, -1, 1);
    h2_dQ2_DA     = new TH2F("h2_dQ2_DA",     "DA;Q^{2}_{t} [GeV^{2}];(Q^{2}_{t}-Q^{2}_{r})/Q^{2}_{t}",     100, 0, 10, 100, -1, 1);
    
    h2_dy_EM     = new TH2F("h2_dy_EM",     "EM;y_{truth};(y_{t}-y_{r})/y_{t}",     100, 0, 1, 100, -1, 1);
    h2_dy_JB     = new TH2F("h2_dy_JB",     "JB;y_{truth};(y_{t}-y_{r})/y_{t}",     100, 0, 1, 100, -1, 1);
    h2_dy_eSigma = new TH2F("h2_dy_eSigma", "eSigma;y_{truth};(y_{t}-y_{r})/y_{t}", 100, 0, 1, 100, -1, 1);
    h2_dy_DA     = new TH2F("h2_dy_DA",     "DA;y_{truth};(y_{t}-y_{r})/y_{t}",     100, 0, 1, 100, -1, 1);
    
    //--------------------------------------------------
    // x, Q2, y resolution: 1D fractional residuals
    //--------------------------------------------------
    h_dx_EM     = new TH1F("h_dx_EM",     "EM;(x_{t}-x_{r})/x_{t};Counts",                         100, -0.4, 0.4);
    h_dx_JB     = new TH1F("h_dx_JB",     "JB;(x_{t}-x_{r})/x_{t};Counts",                         100, -0.4, 0.4);
    h_dx_eSigma = new TH1F("h_dx_eSigma", "eSigma;(x_{t}-x_{r})/x_{t};Counts",                     100, -0.4, 0.4);
    h_dx_DA     = new TH1F("h_dx_DA",     "DA;(x_{t}-x_{r})/x_{t};Counts",                         100, -0.4, 0.4);
    
    h_dQ2_EM     = new TH1F("h_dQ2_EM",     "EM;(Q^{2}_{t}-Q^{2}_{r})/Q^{2}_{t};Counts",         100, -0.4, 0.4);
    h_dQ2_JB     = new TH1F("h_dQ2_JB",     "JB;(Q^{2}_{t}-Q^{2}_{r})/Q^{2}_{t};Counts",         100, -0.4, 0.4);
    h_dQ2_eSigma = new TH1F("h_dQ2_eSigma", "eSigma;(Q^{2}_{t}-Q^{2}_{r})/Q^{2}_{t};Counts",     100, -0.4, 0.4);
    h_dQ2_DA     = new TH1F("h_dQ2_DA",     "DA;(Q^{2}_{t}-Q^{2}_{r})/Q^{2}_{t};Counts",         100, -0.4, 0.4);
    
    h_dy_EM     = new TH1F("h_dy_EM",     "EM;(y_{t}-y_{r})/y_{t};Counts",                         100, -0.4, 0.4);
    h_dy_JB     = new TH1F("h_dy_JB",     "JB;(y_{t}-y_{r})/y_{t};Counts",                         100, -0.4, 0.4);
    h_dy_eSigma = new TH1F("h_dy_eSigma", "eSigma;(y_{t}-y_{r})/y_{t};Counts",                     100, -0.4, 0.4);
    h_dy_DA     = new TH1F("h_dy_DA",     "DA;(y_{t}-y_{r})/y_{t};Counts",                         100, -0.4, 0.4);
    
    //--------------------------------------------------
    // x, Q2, y resolution: truth vs reco 2D migration
    //--------------------------------------------------
    h2_x_EM_res     = new TH2F("h2_x_EM_res",     "EM;x_{truth};x_{reco}",             100, 0, xmax, 100, 0, xmax);
    h2_x_JB_res     = new TH2F("h2_x_JB_res",     "JB;x_{truth};x_{reco}",             100, 0, xmax, 100, 0, xmax);
    h2_x_eSigma_res = new TH2F("h2_x_eSigma_res", "eSigma;x_{truth};x_{reco}",         100, 0, xmax, 100, 0, xmax);
    h2_x_DA_res     = new TH2F("h2_x_DA_res",     "DA;x_{truth};x_{reco}",             100, 0, xmax, 100, 0, xmax);
    
    h2_Q2_EM_res     = new TH2F("h2_Q2_EM_res",     "EM;Q^{2}_{truth};Q^{2}_{reco}",     100, 0, 10, 100, 0, 10);
    h2_Q2_JB_res     = new TH2F("h2_Q2_JB_res",     "JB;Q^{2}_{truth};Q^{2}_{reco}",     100, 0, 10, 100, 0, 10);
    h2_Q2_eSigma_res = new TH2F("h2_Q2_eSigma_res", "eSigma;Q^{2}_{truth};Q^{2}_{reco}", 100, 0, 10, 100, 0, 10);
    h2_Q2_DA_res     = new TH2F("h2_Q2_DA_res",     "DA;Q^{2}_{truth};Q^{2}_{reco}",     100, 0, 10, 100, 0, 10);
    
    h2_y_EM_res     = new TH2F("h2_y_EM_res",     "EM;y_{truth};y_{reco}",             100, 0, 1, 100, 0, 1);
    h2_y_JB_res     = new TH2F("h2_y_JB_res",     "JB;y_{truth};y_{reco}",             100, 0, 1, 100, 0, 1);
    h2_y_eSigma_res = new TH2F("h2_y_eSigma_res", "eSigma;y_{truth};y_{reco}",         100, 0, 1, 100, 0, 1);
    h2_y_DA_res     = new TH2F("h2_y_DA_res",     "DA;y_{truth};y_{reco}",             100, 0, 1, 100, 0, 1);
    
    
    h2_reco_Q2_vs_x     = new TH2F("h2_reco_Q2_vs_x",     " ;x;Q^{2} [GeV^{2}]",             100, 0, xmax, 100, 1, 10);
    
    //--------------------------------------------------
    // -t resolution (new — RP, B0, Method L)
    // Consistent 200-bin, (0,5) range across all t histograms
    //--------------------------------------------------
    h2_dt_RP      = new TH2F("h2_dt_RP",
                             "RP;-t_{truth} [GeV^{2}];(-t_{t}-(-t_{r}))/(-t_{t})", 200, 0, 2, 100, -1, 1);
    h2_dt_B0      = new TH2F("h2_dt_B0",
                             "B0;-t_{truth} [GeV^{2}];(-t_{t}-(-t_{r}))/(-t_{t})", 200, 0, 2, 100, -1, 1);
    h2_dt_MethodL = new TH2F("h2_dt_MethodL",
                             "MethodL;-t_{truth} [GeV^{2}];(-t_{t}-(-t_{r}))/(-t_{t})", 200, 0, 2, 100, -1, 1);
    
    h_dt_RP      = new TH1F("h_dt_RP",
                            "RP;(-t_{t}-(-t_{r}))/(-t_{t});Counts", 100, -1, 1);
    h_dt_B0      = new TH1F("h_dt_B0",
                            "B0;(-t_{t}-(-t_{r}))/(-t_{t});Counts", 100, -1, 1);
    h_dt_MethodL = new TH1F("h_dt_MethodL",
                            "MethodL;(-t_{t}-(-t_{r}))/(-t_{t});Counts", 100, -1, 1);
    
    h2_t_RP_res      = new TH2F("h2_t_RP_res",
                                "RP;-t_{truth} [GeV^{2}];-t_{reco} [GeV^{2}]",      200, 0, tmax, 200, 0, tmax);
    h2_t_B0_res      = new TH2F("h2_t_B0_res",
                                "B0;-t_{truth} [GeV^{2}];-t_{reco} [GeV^{2}]",      200, 0, tmax, 200, 0, tmax);
    h2_t_MethodL_res = new TH2F("h2_t_MethodL_res",
                                "MethodL;-t_{truth} [GeV^{2}];-t_{reco} [GeV^{2}]", 200, 0, tmax, 200, 0, tmax);
    
    //--------------------------------------------------
    // Electron energy resolution
    //--------------------------------------------------
    h_CalEnergy_minus_SCElecEnergy = new TH1F("h_CalEnergy_minus_SCElecEnergy",
                                              ";(E_{MC}-E_{cal})/E_{MC};Counts", 200, -1, 1);
    h_TrackEnergy_minus_SCElecEnergy = new TH1F("h_TrackEnergy_minus_SCElecEnergy",
                                                ";(E_{MC}-E_{track})/E_{MC};Counts", 200, -1, 1);
    h_Cal_DE_over_E_vs_Eta = new TH2F("h_Cal_DE_over_E_vs_Eta",
                                      ";#eta;(E_{MC}-E_{cal})/E_{MC}", 100, -4, 0, 200, -1, 1);
    h_Track_DE_over_E_vs_Eta = new TH2F("h_Track_DE_over_E_vs_Eta",
                                        ";#eta;(E_{MC}-E_{track})/E_{MC}", 100, -4, 0, 200, -1, 1);
    
    //--------------------------------------------------
    // E - Pz
    //--------------------------------------------------
    h_mc_e_minus_pz   = new TH1F("h_mc_e_minus_pz",   ";E-P_{z} [GeV];Counts", 200, 0, 40);
    h_reco_e_minus_pz = new TH1F("h_reco_e_minus_pz", ";E-P_{z} [GeV];Counts", 200, 0, 40);
    
    //--------------------------------------------------
    // t-distributions
    //--------------------------------------------------
    h_t_MC        = new TH1F("h_t_MC",        "MC;-t [GeV^{2}];Counts",          tBins, 0, tmax);
    h_t_RECO_RP   = new TH1F("h_t_RECO_RP",   "RECO RP;-t [GeV^{2}];Counts",    tBins, 0, tmax);
    h_t_RECO_B0   = new TH1F("h_t_RECO_B0",   "RECO B0;-t [GeV^{2}];Counts",    tBins, 0, tmax);
    h_t_RECO_RPB0 = new TH1F("h_t_RECO_RPB0", "RECO RP+B0;-t [GeV^{2}];Counts", tBins, 0, tmax);
    
    h_t2D_RP   = new TH2F("h_t2D_RP",   "RP;Truth -t [GeV^{2}];Reco -t [GeV^{2}]",    tBins, 0, tmax, tBins, 0, tmax);
    h_t2D_B0   = new TH2F("h_t2D_B0",   "B0;Truth -t [GeV^{2}];Reco -t [GeV^{2}]",    tBins, 0, tmax, tBins, 0, tmax);
    h_t2D_RPB0 = new TH2F("h_t2D_RPB0", "RP+B0;Truth -t [GeV^{2}];Reco -t [GeV^{2}]", tBins, 0, tmax, tBins, 0, tmax);
    
    h_t_MethodL_RECO = new TH1F("h_t_MethodL_RECO",
                                "Method L;-t [GeV^{2}];Counts", tBins, 0, tmax);
    h_t2D_MethodL    = new TH2F("h_t2D_MethodL",
                                "Method L;Truth -t [GeV^{2}];Reco -t [GeV^{2}]", tBins, 0, tmax, tBins, 0, tmax);
    
    h_t_RECO_RPB0_MethodL = new TH1F("h_t_RECO_RPB0_MethodL",
                                     "RECO RP+B0+MethodL;-t [GeV^{2}];Counts", tBins, 0, tmax);
    
    h_methodL_RP_MethodL_B0 = new TH1F("h_methodL_RP_MethodL_B0",
                                       "RECO MethodL+RP+B0+MethodL;-t [GeV^{2}];Counts", tBins, 0, tmax);
    
    // BUG FIX: y-axis was 100 bins in original — now consistent 200 bins
    h_t2D_RPB0_MethodL = new TH2F("h_t2D_RPB0_MethodL",
                                  "RP+B0+MethodL;Truth -t [GeV^{2}];Reco -t [GeV^{2}]",
                                  tBins, 0, tmax, tBins, 0, tmax);
    
    h2_methodL_RP_MethodL_B0 = new TH2F("h2_methodL_RP_MethodL_B0",
                                        "MethodL+RP+B0+MethodL;Truth -t [GeV^{2}];Reco -t [GeV^{2}]",
                                        tBins, 0, tmax, tBins, 0, tmax);
    
    h_t_lAger_accept = new TH1F("h_t_lAger_accept",
                                "Accepted truth;-t [GeV^{2}];Counts", tBins, 0, tmax);
    
    h_t_hybrid_accept = new TH1F("h_t_hybrid_accept",
                                 "Accepted truth;-t [GeV^{2}];Counts", tBins, 0, tmax);
    
    h_t_RPB0_accept = new TH1F("h_t_RPB0_accept",
                               "Accepted RPB0;-t [GeV^{2}];Counts", tBins, 0, tmax);
    
    h_sanity_methL_miss_cut = new TH1F("h_sanity_methL_miss_cut",
                                       ";MethL_Miss_Mass;Counts", 100, -10, 10);
    h_sanity_methL_miss_all = new TH1F("h_sanity_methL_miss_all",
                                       ";MethL_Miss_Mass;Counts", 100, -10, 10);
    
    h_EXCLU_jpsi_mass = new TH1F("h_EXCLU_jpsi_mass",
                                 "J/#psi mass;M_{e^{+}e^{-}} [GeV];Counts", 100, 2.0, 4.0);
}

//---------------------------------------------
// WriteHistograms
//---------------------------------------------
void WriteHistograms(TFile *outFile)
{
    outFile->cd();

    // Generator
    h_t_lAger->Write();  h_xb_lAger->Write();
    h_Q2_lAger->Write(); h_y_lAger->Write();

    // Roman Pots
    RPfinalProton->Write();   B0RPfinalProton->Write();
    h_pt_RomanPots->Write();  h_pt_RPB0combined->Write();
    h_px_RomanPots->Write();  h_py_RomanPots->Write();
    h_pz_RomanPots->Write();  h_energ_RomanPots->Write();
    ThetaRP->Write();         ThetaRPB0->Write();
    h_rp_occupancy_map->Write();

    // B0
    B0finalProton->Write();
    h_px_B0_track->Write();   h_py_B0_track->Write();
    h_pt_B0_track->Write();   h_pz_B0_track->Write();
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

    // RECO electron / J/psi
    h_jpsi_mass_RC->Write();
    h_scattered_electron_energy_RC->Write();
    h_scattered_electron_eta_RC->Write();
    h_scattered_electron_pt_RC->Write();
    h_scattered_positron_eta_RC->Write();
    h_decay_electron_eta_RC->Write();

    // Calorimeter
    h_matched_ecal_energy->Write();
    h_matched_ecal_dr->Write();
    h_matched_eop->Write();

    // Inclusive kinematics
    EM_x->Write();   EM_Q2->Write();   EM_y->Write();
    ESig_x_in->Write(); ESig_Q2_in->Write(); ESig_y_in->Write();
    JB_x_in->Write();   JB_Q2_in->Write();   JB_y_in->Write();
    DA_x_in->Write();   DA_Q2_in->Write();   DA_y_in->Write();

    // MC truth
    h_mc_jpsi_mass->Write();
    h_mc_positron_px->Write();  h_mc_positron_py->Write();
    h_mc_positron_pz->Write();  h_mc_positron_pt->Write();
    h_mc_positron_E->Write();   h_mc_positron_eta->Write();

    h_mc_decay_electron_px->Write();  h_mc_decay_electron_py->Write();
    h_mc_decay_electron_pz->Write();  h_mc_decay_electron_pt->Write();
    h_mc_decay_electron_E->Write();   h_mc_decay_electron_eta->Write();

    h_mc_scattered_electron_px->Write();  h_mc_scattered_electron_py->Write();
    h_mc_scattered_electron_pz->Write();  h_mc_scattered_electron_pt->Write();
    h_mc_scattered_electron_E->Write();   h_mc_scattered_electron_eta->Write();

    h_mc_recoil_proton_px->Write();  h_mc_recoil_proton_py->Write();
    h_mc_recoil_proton_pz->Write();  h_mc_recoil_proton_pt->Write();
    h_mc_recoil_proton_E->Write();   h_mc_recoil_proton_eta->Write();

    // Missing mass
    h_missingMass_MC->Write();  h_missingMass2_MC->Write();
    h_missingMass_RP->Write();  h_missingMass2_RP->Write();
    h_missingMass_B0->Write();  h_missingMass2_B0->Write();

    // RECO kinematics
    h_reco_EM_x->Write();  h_reco_EM_Q2->Write();  h_reco_EM_y->Write();

    h_reco_JB_x->Write();        h_reco_JB_Q2->Write();      h_reco_JB_y->Write();
    h_reco_JB_HFS_sigma->Write(); h_reco_JB_HFS_pt->Write();

    h_reco_eSigma_x->Write();        h_reco_eSigma_Q2->Write();  h_reco_eSigma_y->Write();
    h_reco_eSigma_HFS_sigma->Write();

    h_reco_DA_x->Write();  h_reco_DA_Q2->Write();  h_reco_DA_y->Write();

    // x, Q2, y resolution
    h2_dx_EM->Write();  h2_dx_JB->Write();  h2_dx_eSigma->Write();  h2_dx_DA->Write();
    h2_dQ2_EM->Write(); h2_dQ2_JB->Write(); h2_dQ2_eSigma->Write(); h2_dQ2_DA->Write();
    h2_dy_EM->Write();  h2_dy_JB->Write();  h2_dy_eSigma->Write();  h2_dy_DA->Write();

    h_dx_EM->Write();  h_dx_JB->Write();  h_dx_eSigma->Write();  h_dx_DA->Write();
    h_dQ2_EM->Write(); h_dQ2_JB->Write(); h_dQ2_eSigma->Write(); h_dQ2_DA->Write();
    h_dy_EM->Write();  h_dy_JB->Write();  h_dy_eSigma->Write();  h_dy_DA->Write();

    h2_x_EM_res->Write();  h2_x_JB_res->Write();  h2_x_eSigma_res->Write();  h2_x_DA_res->Write();
    h2_Q2_EM_res->Write(); h2_Q2_JB_res->Write(); h2_Q2_eSigma_res->Write(); h2_Q2_DA_res->Write();
    h2_y_EM_res->Write();  h2_y_JB_res->Write();  h2_y_eSigma_res->Write();  h2_y_DA_res->Write();
    
    h2_reco_Q2_vs_x->Write();

    // -t resolution (new)
    h2_dt_RP->Write();      h_dt_RP->Write();      h2_t_RP_res->Write();
    h2_dt_B0->Write();      h_dt_B0->Write();      h2_t_B0_res->Write();
    h2_dt_MethodL->Write(); h_dt_MethodL->Write(); h2_t_MethodL_res->Write();

    // Electron resolution
    h_CalEnergy_minus_SCElecEnergy->Write();
    h_TrackEnergy_minus_SCElecEnergy->Write();
    h_Cal_DE_over_E_vs_Eta->Write();
    h_Track_DE_over_E_vs_Eta->Write();

    // E - Pz
    h_mc_e_minus_pz->Write();
    h_reco_e_minus_pz->Write();

    // t-distributions
    h_t_MC->Write();
    h_t_RECO_RP->Write();  h_t_RECO_B0->Write();  h_t_RECO_RPB0->Write();
    h_t2D_RP->Write();     h_t2D_B0->Write();     h_t2D_RPB0->Write();
    h_t_MethodL_RECO->Write();  h_t2D_MethodL->Write();
    h_t_RECO_RPB0_MethodL->Write();
    h_t2D_RPB0_MethodL->Write();
    h_methodL_RP_MethodL_B0->Write();
    h2_methodL_RP_MethodL_B0->Write();

    // Corrections
    h_t_lAger_accept->Write();
    h_t_hybrid_accept->Write();
    h_t_RPB0_accept->Write();
    
    if (h_CorrectionFactor)         h_CorrectionFactor->Write();
    if (h_t_RPB0_MethodL_corrected) h_t_RPB0_MethodL_corrected->Write();
    
    if (h_HybridCorrectionFactor)         h_HybridCorrectionFactor->Write();
    if (h_methodL_RP_MethodL_B0_corrected) h_methodL_RP_MethodL_B0_corrected->Write();
    
    if (h_t_RPB0_eff_applied)         h_t_RPB0_eff_applied->Write();
    if (h_eff_RP_B0) h_eff_RP_B0->Write();
    if (h_eff_MethodL_RP_MethodL_B0) h_eff_MethodL_RP_MethodL_B0->Write();
    if (h_methodL_RP_MethodL_B0_eff_applied) h_methodL_RP_MethodL_B0_eff_applied->Write();
    
    
    h_sanity_methL_miss_cut->Write();
    h_sanity_methL_miss_all->Write();
    
    h_EXCLU_jpsi_mass->Write();
}

#endif
