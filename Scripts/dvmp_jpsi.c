// Code written by Olaiya Olokunboyo, University of New Hampshire
// DVMP / J/psi analysis steering macro (clean modular version)
//
// May 07, 2026

#include <iostream>
#include <fstream>
#include <string>
#include <memory>

// ROOT
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TTreeReader.h>
#include <TTreeReaderArray.h>

// Local headers
#include "dvmp_plot.h"
#include "dvmp_jpsi.h"

using namespace std;

void dvmp_jpsi()
{
    //---------------------------------------------
    // Generator chain (lAger)
    //---------------------------------------------
    TChain *T = new TChain("lAger");
    T->Add("lager-vmp-00mrad.jpsi-10on130.4pi.disp-jpsi-00-electron.run00001-lumi10.root");
    SetGeneratorBranches(T);
    
    //---------------------------------------------
    // Initialize histograms
    //---------------------------------------------
    InitHistograms();
    
    //---------------------------------------------
    // File list
    //---------------------------------------------
    //TString fileList = "dvmp_March_2026_1.list";
    //TString fileList = "March_2026_1.list";
    TString fileList = "April_2026_1.list";
    //TString fileList = "October_2025_2.list";
    //TString fileList = "../10x100_jpsi/Background_noise_3.list";
    //TString fileList = "../10x100_jpsi/Background_noise.list";
    //TString fileList = "Background_Full.list";
    
    ifstream fileListStream(fileList.Data());
    if (!fileListStream.is_open())
    {
        cerr << "ERROR: Cannot open file list: " << fileList << endl;
        return;
    }
    
    //---------------------------------------------
    // Output file
    //---------------------------------------------
    TString outputFileName = "benchmark_data_10x130_June_2026_From_April_2026_run_0.root";
    //TString outputFileName = "dvmp_benchmark_data_10x130_June_2026_run_0.root";
    //TString outputFileName = "DIS_background_data_10x130_June_2026_run_1.root";
    
    cout << "Input FileList : " << fileList << endl;
    cout << "Output File    : " << outputFileName << endl;
    
    //---------------------------------------------
    // Loop counters
    //---------------------------------------------
    string fileName;
    int fileCounter = 0;
    int genIndex    = 0;
    
    // Sanity counters
    int nB0             = 0;
    int nElectron       = 0;
    int nB0Electron     = 0;
    int nB0ElectronHFS  = 0;
    int MissingMassCounter  = 0;
    
    int nMethodL_enter = 0;
    int nMethodL_ecal  = 0;
    int nMethodL_miss    = 0;
    int nMethodL_EM    = 0;
    
    //---------------------------------------------
    // File loop
    //---------------------------------------------
    while (getline(fileListStream, fileName))
    {
        if (fileName.empty()) continue;
        
        TString tmpFileName = fileName;
        cout << "\n====================================\n";
        cout << "Processing file " << fileCounter
        << " : " << tmpFileName << endl;
        
        //---------------------------------------------
        // Open reconstructed ROOT file
        //---------------------------------------------
        unique_ptr<TFile> inputRootFile(TFile::Open(tmpFileName));
        if (!inputRootFile || inputRootFile->IsZombie())
        {
            cerr << "ERROR: Cannot open file: " << tmpFileName << endl;
            // CRITICAL: still advance genIndex by the number of events
            // we would have processed so lAger stays in sync.
            // Since we can't know numEvents, skip and warn.
            cerr << "WARNING: lAger sync may be broken after this file." << endl;
            continue;
        }
        
        //---------------------------------------------
        // Get event tree
        //---------------------------------------------
        TTree *evtTree = (TTree*)inputRootFile->Get("events");
        if (!evtTree)
        {
            cerr << "ERROR: 'events' tree not found in " << tmpFileName << endl;
            cerr << "WARNING: lAger sync may be broken after this file." << endl;
            continue;
        }
        
        Long64_t numEvents = evtTree->GetEntries();
        cout << "Events in file: " << numEvents << endl;
        
        //---------------------------------------------
        // Tree reader setup
        //---------------------------------------------
        TTreeReader treeReader(evtTree);
        treeReader.SetEntriesRange(0, numEvents);
        
        // Branch collection names
        TString RPBranch        = "ForwardRomanPotRecParticles";
        TString B0Branch        = "ReconstructedTruthSeededChargedParticles";
        // NOTE: B0Branch and CentralBranch are the same collection in this
        // simulation. B0 proton and central tracks are separated by kinematics
        // inside ProcessB0 and ProcessScatteredElectron respectively.
        TString CentralBranch   = "ReconstructedTruthSeededChargedParticles";
        TString B0ClusterBranch = "B0ECalClusters";
        TString B0HitBranch     = "B0TrackerRecHits";
        TString EEMCBranch      = "EcalEndcapNClusters";
        // Use MCParticles (head-on frame) — crossing angle correction
        // applied inside ProcessMCParticles via RotateY(0.025)
        TString MCBranch        = "MCParticles";
        
        //---------------------------------------------
        // CKF track hit-count branches (for B0 nHits)
        //---------------------------------------------
        TTreeReaderArray<unsigned int> track_meas_begin(treeReader, "CentralCKFTracks.measurements_begin");
        TTreeReaderArray<unsigned int> track_meas_end  (treeReader, "CentralCKFTracks.measurements_end");
        
        //---------------------------------------------
        // Roman Pot branches
        //---------------------------------------------
        TTreeReaderArray<float> rp_px      (treeReader, RPBranch + ".momentum.x");
        TTreeReaderArray<float> rp_py      (treeReader, RPBranch + ".momentum.y");
        TTreeReaderArray<float> rp_pz      (treeReader, RPBranch + ".momentum.z");
        TTreeReaderArray<int>   rp_pdg     (treeReader, RPBranch + ".PDG");
        TTreeReaderArray<float> rp_mass    (treeReader, RPBranch + ".mass");
        TTreeReaderArray<float> rp_energy  (treeReader, RPBranch + ".energy");
        TTreeReaderArray<float> rp_charge  (treeReader, RPBranch + ".charge");
        TTreeReaderArray<float> rp_goodness(treeReader, RPBranch + ".goodnessOfPID");
        TTreeReaderArray<float> rp_ref_x   (treeReader, RPBranch + ".referencePoint.x");
        TTreeReaderArray<float> rp_ref_y   (treeReader, RPBranch + ".referencePoint.y");
        TTreeReaderArray<float> rp_ref_z   (treeReader, RPBranch + ".referencePoint.z");
        
        //---------------------------------------------
        // B0 tracker branches
        //---------------------------------------------
        TTreeReaderArray<float> reco_track_px      (treeReader, B0Branch + ".momentum.x");
        TTreeReaderArray<float> reco_track_py      (treeReader, B0Branch + ".momentum.y");
        TTreeReaderArray<float> reco_track_pz      (treeReader, B0Branch + ".momentum.z");
        TTreeReaderArray<float> reco_track_energy  (treeReader, B0Branch + ".energy");
        TTreeReaderArray<float> reco_track_mass    (treeReader, B0Branch + ".mass");
        TTreeReaderArray<float> reco_track_charge  (treeReader, B0Branch + ".charge");
        TTreeReaderArray<int>   reco_track_pdg     (treeReader, B0Branch + ".PDG");
        TTreeReaderArray<float> reco_track_goodness(treeReader, B0Branch + ".goodnessOfPID");
        TTreeReaderArray<float> reco_track_ref_z   (treeReader, B0Branch + ".referencePoint.z");
        
        //---------------------------------------------
        // B0 EMCAL clusters
        //---------------------------------------------
        TTreeReaderArray<float> b0_cluster_x     (treeReader, B0ClusterBranch + ".position.x");
        TTreeReaderArray<float> b0_cluster_y     (treeReader, B0ClusterBranch + ".position.y");
        TTreeReaderArray<float> b0_cluster_z     (treeReader, B0ClusterBranch + ".position.z");
        TTreeReaderArray<float> b0_cluster_energy(treeReader, B0ClusterBranch + ".energy");
        
        //---------------------------------------------
        // B0 tracker hits
        //---------------------------------------------
        TTreeReaderArray<float> b0_hits_x   (treeReader, B0HitBranch + ".position.x");
        TTreeReaderArray<float> b0_hits_y   (treeReader, B0HitBranch + ".position.y");
        TTreeReaderArray<float> b0_hits_z   (treeReader, B0HitBranch + ".position.z");
        TTreeReaderArray<float> b0_hits_eDep(treeReader, B0HitBranch + ".edep");
        
        //---------------------------------------------
        // Central tracking (scattered electron + J/psi decay)
        //---------------------------------------------
        TTreeReaderArray<float> reco_part_px    (treeReader, CentralBranch + ".momentum.x");
        TTreeReaderArray<float> reco_part_py    (treeReader, CentralBranch + ".momentum.y");
        TTreeReaderArray<float> reco_part_pz    (treeReader, CentralBranch + ".momentum.z");
        TTreeReaderArray<float> reco_part_energy(treeReader, CentralBranch + ".energy");
        TTreeReaderArray<float> reco_part_mass  (treeReader, CentralBranch + ".mass");
        TTreeReaderArray<float> reco_part_charge(treeReader, CentralBranch + ".charge");
        TTreeReaderArray<int>   reco_part_pdg   (treeReader, CentralBranch + ".PDG");
        
        //---------------------------------------------
        // Endcap ECAL clusters (electron matching)
        //---------------------------------------------
        TTreeReaderArray<float>        ecalN_energy(treeReader, EEMCBranch + ".energy");
        TTreeReaderArray<float>        ecalN_x     (treeReader, EEMCBranch + ".position.x");
        TTreeReaderArray<float>        ecalN_y     (treeReader, EEMCBranch + ".position.y");
        TTreeReaderArray<float>        ecalN_z     (treeReader, EEMCBranch + ".position.z");
        TTreeReaderArray<unsigned int> ecalN_nHits (treeReader, EEMCBranch + ".nhits");
        
        //---------------------------------------------
        // Inclusive kinematics collections (EICRECON output)
        //---------------------------------------------
        TTreeReaderArray<float> ik_electron_x (treeReader, "InclusiveKinematicsElectron.x");
        TTreeReaderArray<float> ik_electron_Q2(treeReader, "InclusiveKinematicsElectron.Q2");
        TTreeReaderArray<float> ik_electron_y (treeReader, "InclusiveKinematicsElectron.y");
        
        TTreeReaderArray<float> ik_esigma_x (treeReader, "InclusiveKinematicsESigma.x");
        TTreeReaderArray<float> ik_esigma_Q2(treeReader, "InclusiveKinematicsESigma.Q2");
        TTreeReaderArray<float> ik_esigma_y (treeReader, "InclusiveKinematicsESigma.y");
        
        TTreeReaderArray<float> ik_jb_x (treeReader, "InclusiveKinematicsJB.x");
        TTreeReaderArray<float> ik_jb_Q2(treeReader, "InclusiveKinematicsJB.Q2");
        TTreeReaderArray<float> ik_jb_y (treeReader, "InclusiveKinematicsJB.y");
        
        TTreeReaderArray<float> ik_DA_x (treeReader, "InclusiveKinematicsDA.x");
        TTreeReaderArray<float> ik_DA_Q2(treeReader, "InclusiveKinematicsDA.Q2");
        TTreeReaderArray<float> ik_DA_y (treeReader, "InclusiveKinematicsDA.y");
        
        //---------------------------------------------
        // MC truth particles
        //---------------------------------------------
        TTreeReaderArray<double> mc_px        (treeReader, MCBranch + ".momentum.x");
        TTreeReaderArray<double> mc_py        (treeReader, MCBranch + ".momentum.y");
        TTreeReaderArray<double> mc_pz        (treeReader, MCBranch + ".momentum.z");
        TTreeReaderArray<double> mc_mass      (treeReader, MCBranch + ".mass");
        TTreeReaderArray<float>  mc_charge    (treeReader, MCBranch + ".charge");
        TTreeReaderArray<int>    mc_pdg       (treeReader, MCBranch + ".PDG");
        TTreeReaderArray<int>    mc_gen_status(treeReader, MCBranch + ".generatorStatus");
        TTreeReaderArray<unsigned int> mc_daughters_begin(treeReader, MCBranch + ".daughters_begin");
        TTreeReaderArray<unsigned int> mc_daughters_end  (treeReader, MCBranch + ".daughters_end");
        TTreeReaderArray<int> mc_daughter_index(treeReader, "_" + MCBranch + "_daughters.index");
        
        //=============================================
        // Event loop
        //=============================================
        while (treeReader.Next())
        {
            // CRITICAL: always advance genIndex before any early continue
            // so the lAger chain stays in sync with the reco tree.
            T->GetEntry(genIndex);
            genIndex++;
            
            // Truth-level kinematic gate
            // Uses gen_* globals set by SetGeneratorBranches / T->GetEntry
            bool passTruthKinematics =
            (gen_x  > x_low  && gen_x  < x_high  &&
             gen_Q2 > q2_low && gen_Q2 < q2_high &&
             gen_y  > y_low  && gen_y  < y_high);
            
            if (!passTruthKinematics) continue;
            
            // Fill generator-level histograms
            h_t_lAger ->Fill(-gen_t);
            h_xb_lAger->Fill(gen_x);
            h_Q2_lAger->Fill(gen_Q2);
            h_y_lAger ->Fill(gen_y);
            
            //------------------------------------------
            // MC truth reconstruction
            //------------------------------------------
            bool foundMC = ProcessMCParticles(
                                              mc_px, mc_py, mc_pz, mc_mass,
                                              mc_charge, mc_pdg, mc_gen_status);
            
            // For DIS background only:
            // remove PYTHIA events containing real generated J/psi++++++++++++
            //if (HasGeneratedJpsi(mc_pdg, mc_gen_status)) continue;//use ! for DIS
            
            /*bool isDISBackground = true; // set false for signal
             
             int jpsiMode = CheckGeneratedLeptonicJpsi(
             mc_pdg,
             mc_daughters_begin,
             mc_daughters_end,
             mc_daughter_index
             );
             
             //if (isDISBackground && jpsiMode != 0) continue; //removing only the DIS
             if (isDISBackground && (jpsiMode == 11 || jpsiMode == 13)) continue;//removing on the jpsi leption
             //+++++++++++++++++++++*/
            
            //------------------------------------------
            // Inclusive kinematics (EICRECON collections)
            // Note: reco_x_EM etc. will be overwritten by
            // ProcessRecoElectronKinematics below.
            //------------------------------------------
            ProcessInclusiveKinematics(
                                       ik_electron_x, ik_electron_Q2, ik_electron_y,
                                       ik_esigma_x,   ik_esigma_Q2,   ik_esigma_y,
                                       ik_jb_x,       ik_jb_Q2,       ik_jb_y,
                                       ik_DA_x,       ik_DA_Q2,       ik_DA_y);
            
            //------------------------------------------
            // Forward proton reconstruction
            //------------------------------------------
            ProcessRomanPots(
                             rp_px, rp_py, rp_pz, rp_pdg, rp_mass,
                             rp_energy, rp_charge, rp_goodness,
                             rp_ref_x, rp_ref_y, rp_ref_z);
            
            ProcessB0(
                      reco_track_px, reco_track_py, reco_track_pz,
                      reco_track_energy, reco_track_mass, reco_track_charge,
                      reco_track_pdg, reco_track_goodness, reco_track_ref_z,
                      track_meas_begin, track_meas_end);
            
            /*ProcessB0TruthMatchedDebug(
             reco_track_px, reco_track_py, reco_track_pz,
             reco_track_mass, reco_track_charge);
             
             cout << "B0 DEBUG: "
             << "normalB0 = " << iflagB0proton
             << ", debugB0 = " << iflagB0_truthMatched
             << ", nDebugCand = " << nB0_truthMatched_candidates
             << ", nRecoTracks = " << reco_track_px.GetSize()
             << endl;*/
            
            ProcessB0DetectorHits(
                                  b0_cluster_x, b0_cluster_y, b0_cluster_z, b0_cluster_energy,
                                  b0_hits_x, b0_hits_y, b0_hits_z, b0_hits_eDep);
            
            //------------------------------------------
            // Central tracking: J/psi + scattered electron
            //------------------------------------------
            bool foundElectron = ProcessScatteredElectron(
                                                          reco_part_px, reco_part_py, reco_part_pz,
                                                          reco_part_energy, reco_part_mass,
                                                          reco_part_charge, reco_part_pdg, mc_pdg, mc_gen_status);
            
            // Remove reconstructed exclusive-like J/psi events from DIS background
            //if (IsRecoExclusiveJpsiLike()) continue;
            //if (IsMCExclusiveJpsiLike( mc_pdg, mc_gen_status)) { continue; }
            
            bool foundEcalElectron          = false;
            bool foundRecoElectronKin       = false;
            bool foundRecoHadronicKin       = false;
            
            if (foundElectron)
            {
                foundEcalElectron = MatchScatteredElectronToEcal(
                                                                 ecalN_energy, ecalN_x, ecalN_y, ecalN_z, ecalN_nHits
                                                                 );
                
                foundRecoElectronKin = ProcessRecoElectronKinematics();
                
                if (iflagRPpproton)
                    foundRecoHadronicKin = ProcessRecoHadronicKinematics(RP_proton_rc);
                else if (iflagB0proton)
                    foundRecoHadronicKin = ProcessRecoHadronicKinematics(B0_proton_rc);
                
                if (foundRecoElectronKin)
                    FillKinematicResolutionPlots();
            }
            
            // Electron energy resolution (needs MC truth + ECAL match)
            if (foundElectron && foundMC && foundEcalElectron)
                FillElectronResolutionPlots();
            
            //------------------------------------------
            // Missing mass / exclusivity cuts
            //------------------------------------------
            bool passMC_MissingMass = false;
            bool passRP_MissingMass = false;
            bool passB0_MissingMass = false;
            
            if (foundMC)
            {
                passMC_MissingMass = PassMissingMassCut(
                                                        mc_scatteredElectron, mc_recoilProton, mc_jpsi,
                                                        h_missingMass_MC, h_missingMass2_MC);
            }
            
            if (foundElectron && iflagRPpproton)
            {
                passRP_MissingMass = PassMissingMassCut(
                                                        scatteredElectron_rc, RP_proton_rc, JPsi_rc,
                                                        h_missingMass_RP, h_missingMass2_RP);
            }
            
            if (foundElectron && iflagB0proton)
            {
                passB0_MissingMass = PassMissingMassCut(
                                                        scatteredElectron_rc, B0_proton_rc, JPsi_rc,
                                                        h_missingMass_B0, h_missingMass2_B0);
            }
            
            //------------------------------------------
            // E - Pz conservation checks
            //------------------------------------------
            if (foundMC)
                FillMCEminusPz();
            
            bool foundHFS = false;
            if (foundElectron && foundEcalElectron &&
                (iflagRPpproton || iflagB0proton))
            {
                foundHFS = FillRecoEminusPz();
            }
            
            //------------------------------------------
            // Sanity counters
            //------------------------------------------
            if (iflagB0proton)                          nB0++;
            if (foundElectron)                          nElectron++;
            if (iflagB0proton && foundElectron)         nB0Electron++;
            if (iflagB0proton && foundElectron && foundHFS) nB0ElectronHFS++;
            
            //------------------------------------------
            // Method L exclusivity balance
            //------------------------------------------
            
            double miss_MethodL2 = -999.0;
            double miss_MethodL = -999.0;
            bool passMethodLMissingMass = false;
            
            if (foundElectron && iflagRCelectron && JPsi_rc.E() > 0 && iflagRCelectron)
            {
                nMethodL_enter++;
                
                if (!(foundElectron && iflagRCelectron))
                    continue;
                
                if (JPsi_rc.E() <= 0)
                    continue;
                
                if (JPsi_rc.M() < jplow || JPsi_rc.M() > jphigh)
                    continue;
                
                if (!foundEcalElectron)
                    continue;
                
                if (!foundRecoElectronKin)
                    continue;
                
                //miss_MethodL2 = (incbeame + incbeam - mc_scatteredElectron - mc_jpsi).M2();
                //miss_MethodL = (incbeame + incbeam - scatteredElectron_rc - JPsi_rc).Pt();
                miss_MethodL2 = (incbeame + incbeam - scatteredElectron_rc - positron_rc - decayElectron_rc - protonCorr).M2();
                
                h_sanity_methL_miss_all->Fill(miss_MethodL2);
                
                passMethodLMissingMass = (miss_MethodL2 > -999 && miss_MethodL2 < 999);
                
                if (!passMethodLMissingMass)
                    continue;
                
                nMethodL_miss++;
                
                h_sanity_methL_miss_cut->Fill(miss_MethodL2);
                
                h_EXCLU_jpsi_mass->Fill(JPsi_rc.M());
                
                nMethodL_EM++;
                MissingMassCounter++;
                
                /*static int nPrint = 0;
                
                if (nPrint < 200 &&
                    JPsi_rc.M() > jplow &&
                    JPsi_rc.M() < jphigh)
                {
                    PrintMCParticlesAfterMissingMassCut(
                                                        mc_px,
                                                        mc_py,
                                                        mc_pz,
                                                        mc_mass,
                                                        mc_pdg,
                                                        mc_gen_status,
                                                        JPsi_rc.M(),
                                                        miss_MethodL2
                                                        );
                    
                    nPrint++;
                }*/
            }
            
            //------------------------------------------
            // Exclusivity flags
            //------------------------------------------
            bool passRPExclusive =
            foundElectron      &&
            foundEcalElectron  &&
            foundHFS           &&
            iflagRPpproton     &&
            passRP_MissingMass;
            
            bool passB0Exclusive =
            foundElectron      &&
            foundEcalElectron  &&
            foundHFS           &&
            iflagB0proton      &&
            passB0_MissingMass;
            
            bool passMethodL =
            foundElectron        &&
            foundEcalElectron    &&
            iflagRCelectron      &&
            passMethodLMissingMass;
            
            //------------------------------------------
            // t-distributions (exclusive events only)
            //------------------------------------------
            if (iflagMC || passRPExclusive || passB0Exclusive)
            {
                FillTDistributions(passRPExclusive, passB0Exclusive);
            }
            
            if (passMethodL)
                FillMethodLT();
            
            FillCombinedTWithMethodL(
                                     passRPExclusive,
                                     passB0Exclusive,
                                     passMethodL);
            FillHybridTDistribution(
                                    passRPExclusive,
                                    passB0Exclusive,
                                    passMethodLMissingMass
                                    );
            
        } // end event loop
        
        fileCounter++;
        
    } // end file loop
    
    //---------------------------------------------
    // Acceptance correction
    //---------------------------------------------
    BuildTCorrection();
    BuildTEff();
    BuildHybridTCorrection();
    BuildHybridTEff();
    
    //---------------------------------------------
    // Write output
    //---------------------------------------------
    TFile *outFile = new TFile(outputFileName, "RECREATE");
    WriteHistograms(outFile);
    outFile->Close();
    
    //---------------------------------------------
    // Sanity summary
    //---------------------------------------------
    cout << "\n===== DEBUG COUNTS =====" << endl;
    cout << " " << endl;
    
    cout << "MethodL enter e+e-e-     : " << nMethodL_enter << endl;
    cout << "MethodL after ECAL       : " << nMethodL_ecal  << endl;
    cout << "MethodL after ptmiss     : " << nMethodL_miss    << endl;
    cout << "MethodL after EM kin     : " << nMethodL_EM    << endl;
    
    cout << " " << endl;
    cout << "B0 events              : " << nB0            << endl;
    cout << "Electron events        : " << nElectron       << endl;
    cout << "B0 + Electron events   : " << nB0Electron     << endl;
    cout << "B0 + Electron + HFS    : " << nB0ElectronHFS  << endl;
    cout << "Method L Missing Mass Counter    : " << MissingMassCounter  << endl;
    cout << "========================\n" << endl;
    cout << "\nFinished successfully Selection Script.\n" << endl;
}
