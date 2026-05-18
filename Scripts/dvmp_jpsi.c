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
    
    // Attach generator branches
    SetGeneratorBranches(T);
    
    //---------------------------------------------
    // Initialize histograms
    //---------------------------------------------
    InitHistograms();
    
    //---------------------------------------------
    // File list
    //---------------------------------------------
    //TString fileList = "dvmp_March_2026_1.list";
    TString fileList = "March_2026_1.list";
    
    ifstream fileListStream(fileList.Data());
    
    if (!fileListStream.is_open())
    {
        cerr << "ERROR: Cannot open file list: "
        << fileList << endl;
        return;
    }
    
    //---------------------------------------------
    // Output file
    //---------------------------------------------
    TString outputFileName =
    "dvmp_benchmark_data_10x130_May_2026_run_0.root";
    
    cout << "Input FileList : " << fileList << endl;
    cout << "Output File    : " << outputFileName << endl;
    
    //---------------------------------------------
    // Loop over input files
    //---------------------------------------------
    string fileName;
    int fileCounter = 0;
    int genIndex = 0;
    
    //Sanity
    int nB0 = 0;
    int nElectron = 0;
    int nB0Electron = 0;
    int nB0ElectronHFS = 0;
    
    while (getline(fileListStream, fileName))
    {
        if (fileName.empty()) continue;
        
        TString tmpFileName = fileName;
        
        cout << "\n====================================\n";
        cout << "Processing file " << fileCounter
        << " : " << tmpFileName << endl;
        
        //-----------------------------------------
        // Open reconstructed ROOT file
        //-----------------------------------------
        unique_ptr<TFile> inputRootFile(
                                        TFile::Open(tmpFileName)
                                        );
        
        if (!inputRootFile || inputRootFile->IsZombie())
        {
            cerr << "ERROR: Cannot open file: "
            << tmpFileName << endl;
            continue;
        }
        
        //-----------------------------------------
        // Get event tree
        //-----------------------------------------
        TTree *evtTree =
        (TTree*)inputRootFile->Get("events");
        
        if (!evtTree)
        {
            cerr << "ERROR: 'events' tree not found in "
            << tmpFileName << endl;
            continue;
        }
        
        //-----------------------------------------
        // Event count
        //-----------------------------------------
        Long64_t numEvents = evtTree->GetEntries();
        
        cout << "Events in file: "
        << numEvents << endl;
        
        //-----------------------------------------
        // treeReader for reconstructed tree
        //-----------------------------------------
        TTreeReader treeReader(evtTree);
        
        treeReader.SetEntriesRange(0, numEvents);
        
        TString RPBranch = "ForwardRomanPotRecParticles";
        TString B0Branch = "ReconstructedTruthSeededChargedParticles";
        TString B0ClusterBranch = "B0ECalClusters";
        TString B0HitBranch = "B0TrackerRecHits";
        TString CentralBranch = "ReconstructedChargedParticles";//you can use ReconstructedTruthSeededChargedParticles here
        TString EEMCBranch = "EcalEndcapNClusters";
        TString MCBranch = "MCParticlesHeadOnFrameNoBeamFX";
        //TString MCBranch = "MCParticles";//if you used this, then you need to for the crossing angle rotateY(0.025)
        
        TTreeReaderArray<unsigned int> track_meas_begin(treeReader,"CentralCKFTracks.measurements_begin");
        TTreeReaderArray<unsigned int> track_meas_end(treeReader,"CentralCKFTracks.measurements_end");
        
        //---------------------------------------------
        // Roman Pot branches
        //---------------------------------------------
        TTreeReaderArray<float>rp_px(treeReader, RPBranch + ".momentum.x");
        TTreeReaderArray<float> rp_py(treeReader, RPBranch + ".momentum.y");
        TTreeReaderArray<float> rp_pz(treeReader, RPBranch + ".momentum.z");
        TTreeReaderArray<int> rp_pdg(treeReader, RPBranch + ".PDG");
        TTreeReaderArray<float> rp_mass(treeReader, RPBranch + ".mass");
        TTreeReaderArray<float> rp_energy(treeReader, RPBranch + ".energy");
        TTreeReaderArray<float> rp_charge(treeReader, RPBranch + ".charge");
        TTreeReaderArray<float> rp_goodness(treeReader, RPBranch + ".goodnessOfPID");
        
        // RomanPots Reference points (hit locations)
        TTreeReaderArray<float> rp_ref_x(treeReader, RPBranch + ".referencePoint.x");
        TTreeReaderArray<float> rp_ref_y(treeReader, RPBranch + ".referencePoint.y");
        TTreeReaderArray<float> rp_ref_z(treeReader, RPBranch + ".referencePoint.z");
        
        // B0 reconstructed tracks
        TTreeReaderArray<float> reco_track_px(treeReader, B0Branch + ".momentum.x");
        TTreeReaderArray<float> reco_track_py(treeReader, B0Branch + ".momentum.y");
        TTreeReaderArray<float> reco_track_pz(treeReader, B0Branch + ".momentum.z");
        TTreeReaderArray<float> reco_track_energy(treeReader, B0Branch + ".energy");
        TTreeReaderArray<float> reco_track_mass(treeReader, B0Branch + ".mass");
        TTreeReaderArray<float> reco_track_charge(treeReader, B0Branch + ".charge");
        TTreeReaderArray<int>   reco_track_pdg(treeReader, B0Branch + ".PDG");
        TTreeReaderArray<float> reco_track_goodness(treeReader, B0Branch + ".goodnessOfPID");
        
        TTreeReaderArray<float> reco_track_ref_z(treeReader, B0Branch + ".referencePoint.z");
        
        TTreeReaderArray<float> b0_cluster_x(treeReader, B0ClusterBranch + ".position.x");
        TTreeReaderArray<float> b0_cluster_y(treeReader, B0ClusterBranch + ".position.y");
        TTreeReaderArray<float> b0_cluster_z(treeReader, B0ClusterBranch + ".position.z");
        TTreeReaderArray<float> b0_cluster_energy(treeReader, B0ClusterBranch + ".energy");

        TTreeReaderArray<float> b0_hits_x(treeReader, B0HitBranch + ".position.x");
        TTreeReaderArray<float> b0_hits_y(treeReader, B0HitBranch + ".position.y");
        TTreeReaderArray<float> b0_hits_z(treeReader, B0HitBranch + ".position.z");
        TTreeReaderArray<float> b0_hits_eDep(treeReader, B0HitBranch + ".edep");
        
        //reconstructing the scatered electron
        TTreeReaderArray<float> reco_part_px(treeReader, CentralBranch + ".momentum.x");
        TTreeReaderArray<float> reco_part_py(treeReader, CentralBranch + ".momentum.y");
        TTreeReaderArray<float> reco_part_pz(treeReader, CentralBranch + ".momentum.z");
        TTreeReaderArray<float> reco_part_energy(treeReader, CentralBranch + ".energy");
        TTreeReaderArray<float> reco_part_mass(treeReader, CentralBranch + ".mass");
        TTreeReaderArray<float> reco_part_charge(treeReader, CentralBranch + ".charge");
        TTreeReaderArray<int>   reco_part_pdg(treeReader, CentralBranch + ".PDG");
        
        //for the calorimter electrons
        TTreeReaderArray<float> ecalN_energy(treeReader, EEMCBranch + ".energy");
        TTreeReaderArray<float> ecalN_x(treeReader, EEMCBranch + ".position.x");
        TTreeReaderArray<float> ecalN_y(treeReader, EEMCBranch + ".position.y");
        TTreeReaderArray<float> ecalN_z(treeReader, EEMCBranch + ".position.z");
        TTreeReaderArray<unsigned int> ecalN_nHits(treeReader,EEMCBranch + ".nhits");
        
        //---------------------------------------------
        // Inclusive kinematics
        //---------------------------------------------
        
        // Electron method
        TTreeReaderArray<float> ik_electron_x(treeReader, "InclusiveKinematicsElectron.x");
        TTreeReaderArray<float> ik_electron_Q2(treeReader, "InclusiveKinematicsElectron.Q2");
        TTreeReaderArray<float> ik_electron_y(treeReader, "InclusiveKinematicsElectron.y");
        
        // eSigma
        TTreeReaderArray<float> ik_esigma_x(treeReader, "InclusiveKinematicsESigma.x");
        TTreeReaderArray<float> ik_esigma_Q2(treeReader, "InclusiveKinematicsESigma.Q2");
        TTreeReaderArray<float> ik_esigma_y(treeReader, "InclusiveKinematicsESigma.y");
        
        // Jacquet-Blondel
        TTreeReaderArray<float> ik_jb_x(treeReader, "InclusiveKinematicsJB.x");
        TTreeReaderArray<float> ik_jb_Q2(treeReader, "InclusiveKinematicsJB.Q2");
        TTreeReaderArray<float> ik_jb_y(treeReader, "InclusiveKinematicsJB.y");
        
        // Double Angle
        TTreeReaderArray<float> ik_DA_x(treeReader, "InclusiveKinematicsDA.x");
        TTreeReaderArray<float> ik_DA_Q2(treeReader, "InclusiveKinematicsDA.Q2");
        TTreeReaderArray<float> ik_DA_y(treeReader, "InclusiveKinematicsDA.y");
        
        //for MC
        //---------------------------------------------
        // MC particles
        //---------------------------------------------
        TTreeReaderArray<double> mc_px(treeReader, MCBranch + ".momentum.x");
        TTreeReaderArray<double> mc_py(treeReader, MCBranch + ".momentum.y");
        TTreeReaderArray<double> mc_pz(treeReader, MCBranch + ".momentum.z");
        TTreeReaderArray<double> mc_mass(treeReader, MCBranch + ".mass");
        TTreeReaderArray<float> mc_charge(treeReader, MCBranch + ".charge");
        TTreeReaderArray<int>   mc_pdg(treeReader, MCBranch + ".PDG");
        TTreeReaderArray<int>   mc_gen_status(treeReader, MCBranch + ".generatorStatus");
        
        //-----------------------------------------
        // Event loop
        //-----------------------------------------
        while (treeReader.Next())
        {
            T->GetEntry(genIndex);
            genIndex++;
            
            //might be removed later =========================
            double lager_x  = x;
            double lager_Q2 = Q2;
            double lager_y  = y;
            
            bool passTruthKinematics =
            (lager_x  > x_low  && lager_x  < x_high &&
             lager_Q2 > q2_low && lager_Q2 < q2_high &&
             lager_y  > y_low  && lager_y  < y_high);
            
            if (!passTruthKinematics)
                continue;
            //==============================================
            
            h_t_lAger->Fill(-t);
            h_xb_lAger->Fill(x);
            h_Q2_lAger->Fill(Q2);
            h_y_lAger->Fill(y);
            
            bool foundMC = ProcessMCParticles(
                                              mc_px,
                                              mc_py,
                                              mc_pz,
                                              mc_mass,
                                              mc_charge,
                                              mc_pdg,
                                              mc_gen_status
                                              );
            
            ProcessInclusiveKinematics(
                                       ik_electron_x,
                                       ik_electron_Q2,
                                       ik_electron_y,
                                       
                                       ik_esigma_x,
                                       ik_esigma_Q2,
                                       ik_esigma_y,
                                       
                                       ik_jb_x,
                                       ik_jb_Q2,
                                       ik_jb_y,
                                       
                                       ik_DA_x,
                                       ik_DA_Q2,
                                       ik_DA_y
                                       );
            
            ProcessRomanPots(
                             rp_px,
                             rp_py,
                             rp_pz,
                             rp_pdg,
                             rp_mass,
                             rp_energy,
                             rp_charge,
                             rp_goodness,
                             rp_ref_x,
                             rp_ref_y,
                             rp_ref_z
                             );
            
            ProcessB0(
                      reco_track_px,
                      reco_track_py,
                      reco_track_pz,
                      reco_track_energy,
                      reco_track_mass,
                      reco_track_charge,
                      reco_track_pdg,
                      reco_track_goodness,
                      reco_track_ref_z,
                      track_meas_begin,
                      track_meas_end
                      );
            
            // FillTDistributions(); must be exclusive, not exclusive here
            //FillTDistributions();
            
            ProcessB0DetectorHits(
                b0_cluster_x,
                b0_cluster_y,
                b0_cluster_z,
                b0_cluster_energy,

                b0_hits_x,
                b0_hits_y,
                b0_hits_z,
                b0_hits_eDep
            );
            
            bool foundElectron = ProcessScatteredElectron(
                                                          reco_part_px,
                                                          reco_part_py,
                                                          reco_part_pz,
                                                          reco_part_energy,
                                                          reco_part_mass,
                                                          reco_part_charge,
                                                          reco_part_pdg
                                                          );
            
            bool foundEcalElectron = false;
            bool foundRecoHadronicKinematics = false;

            if (foundElectron)
            {
                foundEcalElectron = MatchScatteredElectronToEcal(
                    ecalN_energy,
                    ecalN_x,
                    ecalN_y,
                    ecalN_z,
                    ecalN_nHits
                );

                ProcessRecoElectronKinematics();

                if (iflagRPpproton)
                {
                    foundRecoHadronicKinematics =
                        ProcessRecoHadronicKinematics(RP_proton_rc);
                }
                else if (iflagB0proton)
                {
                    foundRecoHadronicKinematics =
                        ProcessRecoHadronicKinematics(B0_proton_rc);
                }

                if (foundRecoHadronicKinematics)
                {
                    FillKinematicResolutionPlots();
                }
            }
            
            if (foundElectron && foundMC && foundEcalElectron)
            {
                FillElectronResolutionPlots();
            }
            
            bool passMC_MissingMass = false;
            bool passRP_MissingMass = false;
            bool passB0_MissingMass = false;
            
            if (foundMC)
            {
                passMC_MissingMass =
                PassMissingMassCut(
                                   mc_scatteredElectron,
                                   mc_recoilProton,
                                   mc_jpsi,
                                   h_missingMass_MC,
                                   h_missingMass2_MC
                                   );
            }
            
            if (foundElectron && iflagRPpproton)
            {
                passRP_MissingMass =
                PassMissingMassCut(
                                   scatteredElectron_rc,
                                   RP_proton_rc,
                                   JPsi_rc,
                                   h_missingMass_RP,
                                   h_missingMass2_RP
                                   );
            }
            
            if (foundElectron && iflagB0proton)
            {
                passB0_MissingMass =
                PassMissingMassCut(
                                   scatteredElectron_rc,
                                   B0_proton_rc,
                                   JPsi_rc,
                                   h_missingMass_B0,
                                   h_missingMass2_B0
                                   );
            }
            
            if (foundMC)
                FillMCEminusPz();
            
            bool foundHFS = false;
            
            if (foundElectron && foundEcalElectron &&
                (iflagRPpproton || iflagB0proton))
            {
                foundHFS = FillRecoEminusPz();
            }
            
            //sanity==================
            if (iflagB0proton) nB0++;
            if (foundElectron) nElectron++;
            if (iflagB0proton && foundElectron) nB0Electron++;
            if (iflagB0proton && foundElectron && foundHFS) nB0ElectronHFS++;
            //===============
            
            bool passExclusive =
            foundEcalElectron &&
            (passRP_MissingMass || passB0_MissingMass || foundMC);
            
            if (passExclusive)
            {
                FillTDistributions();
            }
            
            //FillTDistributions();
            
            if (foundElectron)
            {
                FillMethodLT();
                FillCombinedTWithMethodL();
            }
            
            // Fill final selected-event histograms here later
        }
        
        fileCounter++;
        
    } // end file loop
    
    BuildTCorrection();
    
    //---------------------------------------------
    // Output file writing
    //---------------------------------------------
    TFile *outFile =
    new TFile(outputFileName, "RECREATE");
    
    WriteHistograms(outFile);
    
    outFile->Close();
    
    //sanity==================
    cout << "\n===== DEBUG COUNTS =====" << endl;
    cout << "B0 events              : " << nB0 << endl;
    cout << "Electron events        : " << nElectron << endl;
    cout << "B0 + Electron events   : " << nB0Electron << endl;
    cout << "B0 + Electron + HFS    : " << nB0ElectronHFS << endl;
    cout << "========================\n" << endl;
    //======================
    
    cout << "\nFinished successfully.\n" << endl;
}
