#ifndef DVMP_JPSI_H
#define DVMP_JPSI_H

#include <TChain.h>
#include <TLorentzVector.h>
#include <TTreeReaderArray.h>
#include <limits>
#include <TMath.h>
#include <TVector2.h>
#include <vector>
#include <cmath>
#include <TH1F.h>

//---------------------------------------------
// Generator variables
//---------------------------------------------
Float_t t;
Float_t x;
Float_t Q2;
Float_t y;


//fix parameters
double q2_low = 1.0;
double q2_high = 10.0;
double y_low = 0.001;
double y_high = 0.95;
double x_low = 0.0;
double x_high = 1.0;
const double M_JPSI = 3.0969;

double reco_x_EM = -999.0;
double reco_Q2_EM = -999.0;
double reco_y_EM = -999.0;

double reco_x_JB = -999.0;
double reco_Q2_JB = -999.0;
double reco_y_JB = -999.0;

double reco_x_eSigma = -999.0;
double reco_Q2_eSigma = -999.0;
double reco_y_eSigma = -999.0;

double reco_x_DA = -999.0;
double reco_Q2_DA = -999.0;
double reco_y_DA = -999.0;
//---------------------------------------------
// Setup lAger branches
//---------------------------------------------
void SetGeneratorBranches(TChain *T)
{
    T->SetBranchAddress("t",  &t);
    T->SetBranchAddress("x",  &x);
    T->SetBranchAddress("Q2", &Q2);
    T->SetBranchAddress("y",  &y);
}

//some global vvectors
TLorentzVector RP_proton_rc;
TLorentzVector B0_proton_rc;


//for RP detector
int iflagRPpproton = 0;
void ProcessRomanPots(
                      TTreeReaderArray<float> &rp_px,
                      TTreeReaderArray<float> &rp_py,
                      TTreeReaderArray<float> &rp_pz,
                      TTreeReaderArray<int>   &rp_pdg,
                      TTreeReaderArray<float> &rp_mass,
                      TTreeReaderArray<float> &rp_energy,
                      TTreeReaderArray<float> &rp_charge,
                      TTreeReaderArray<float> &rp_goodness,
                      TTreeReaderArray<float> &rp_ref_x,
                      TTreeReaderArray<float> &rp_ref_y,
                      TTreeReaderArray<float> &rp_ref_z
                      )
{
    //-----------------------------------------
    // Roman Pot proton reconstruction
    //-----------------------------------------
    
    TLorentzVector RP_proton(0,0,0,0);
    RP_proton_rc.SetPxPyPzE(0,0,0,0);
    
    iflagRPpproton = 0;
    
    for (int iRPPart = 0;
         iRPPart < rp_px.GetSize();
         ++iRPPart)
    {
        //-------------------------------------
        // Set proton vector
        //-------------------------------------
        RP_proton.SetXYZM(rp_px[iRPPart],
                          rp_py[iRPPart],
                          rp_pz[iRPPart],
                          rp_mass[iRPPart]
                          );
        
        //save in the global vector
        RP_proton_rc = RP_proton;
        
        //-------------------------------------
        // Proton selection
        //-------------------------------------
        bool isProton =
        (rp_charge[iRPPart] == 1) &&
        (rp_pdg[iRPPart] == 2212) &&
        (rp_goodness[iRPPart] > 0.7);
        
        bool passPtCut =
        (RP_proton.Pt() > 0.2);
        
        if (!(isProton && passPtCut))
            continue;
        
        
        //-------------------------------------
        // Fill histograms
        //-------------------------------------
        RPfinalProton->Fill(RP_proton.Eta());
        B0RPfinalProton->Fill(RP_proton.Eta());
        h_pt_RomanPots->Fill(RP_proton.Pt());
        h_pt_B0RomanPots->Fill(RP_proton.Pt());
        h_px_RomanPots->Fill(RP_proton.Px());
        h_py_RomanPots->Fill(RP_proton.Py());
        h_pz_RomanPots->Fill(RP_proton.Pz());
        h_energ_RomanPots->Fill(RP_proton.E());
        
        //-------------------------------------
        // Angular studies
        //-------------------------------------
        ThetaRP->Fill(RP_proton.Theta());
        //ThetaAC->Fill(vec1.Theta());
        ThetaRPB0->Fill(RP_proton.Theta());
        
        //Theta2D->Fill(vec1.Theta(),vec5.Theta());
        
        //-------------------------------------
        // RP occupancy
        //-------------------------------------
        h_rp_occupancy_map->Fill(rp_ref_x[iRPPart],rp_ref_y[iRPPart]);
        
        //-------------------------------------
        // Acceptance
        //-------------------------------------
        //diff_forRP_acc = incbeam - vec1;
        
        //-------------------------------------
        // Proton flag
        //-------------------------------------
        iflagRPpproton = 1;
    }
}


//for BO detector
int iflagB0proton = 0;
void ProcessB0(
    TTreeReaderArray<float> &reco_track_px,
    TTreeReaderArray<float> &reco_track_py,
    TTreeReaderArray<float> &reco_track_pz,
    TTreeReaderArray<float> &reco_track_energy,
    TTreeReaderArray<float> &reco_track_mass,
    TTreeReaderArray<float> &reco_track_charge,
    TTreeReaderArray<int>   &reco_track_pdg,
    TTreeReaderArray<float> &reco_track_goodness,
    TTreeReaderArray<float> &reco_track_ref_z,
    TTreeReaderArray<unsigned int> &track_meas_begin,
    TTreeReaderArray<unsigned int> &track_meas_end
)
{
    TLorentzVector B0_proton(0,0,0,0);
    B0_proton_rc.SetPxPyPzE(0,0,0,0);
    
    iflagB0proton = 0;
    double bestPz = -999.0;
    
    for (int iRecoTrk = 0;
         iRecoTrk < reco_track_px.GetSize();
         ++iRecoTrk)
    {
        B0_proton.SetXYZM(
                          reco_track_px[iRecoTrk],
                          reco_track_py[iRecoTrk],
                          reco_track_pz[iRecoTrk],
                          reco_track_mass[iRecoTrk]
                          );
        
        B0_proton.RotateY(0.025);
        
        //nhits condition===================================
        int nHits = 0;
        if (iRecoTrk < track_meas_begin.GetSize())
        {
            nHits = track_meas_end[iRecoTrk] - track_meas_begin[iRecoTrk];
        }

        bool passNHits = (nHits >= 4);//seriously reduced the number of event
        //===================================
        
        bool isProtonCandidate = (reco_track_pdg[iRecoTrk] == 0);
        
        bool isCharged = (reco_track_charge[iRecoTrk] > 0);
        
        bool isForward = (B0_proton.Eta() > 4.0);
        
        //bool passPt = (B0_proton.Pt() > 0.2);
        
        bool passPz = (B0_proton.Pz() > 0.0);
        
        bool passPID = (reco_track_goodness[iRecoTrk] >= 0.0);
        
        //bool passTheta = (B0_proton.Theta() < 0.02);
        
        /*if (!(isProtonCandidate &&
              isCharged &&
              isForward &&
              passPt &&
              passPz &&
              passPID &&
              passNHits &&
              passTheta))
        {
            continue;
        }*/
        if (!(isProtonCandidate &&
              isCharged &&
              isForward &&
              passPz &&
              passPID )) continue;
        
        if (B0_proton.Pz() > bestPz)
        {
            bestPz = B0_proton.Pz();
            B0_proton_rc = B0_proton;
            iflagB0proton = 1;
        }
        
        B0finalProton->Fill(B0_proton.Eta());
        B0RPfinalProton->Fill(B0_proton.Eta());
        
        h_px_B0_track->Fill(B0_proton.Px());
        h_py_B0_track->Fill(B0_proton.Py());
        h_pt_B0_track->Fill(B0_proton.Pt());
        h_pt_B0RomanPots->Fill(B0_proton.Pt());
        h_pz_B0_track->Fill(B0_proton.Pz());
        h_energ_B0_track->Fill(B0_proton.E());
        
        ThetaB0->Fill(B0_proton.Theta());
        ThetaRPB0->Fill(B0_proton.Theta());

        h_B0_ref_z->Fill(reco_track_ref_z[iRecoTrk]);
        
    }
}

void ProcessB0DetectorHits(
    TTreeReaderArray<float> &b0_cluster_x,
    TTreeReaderArray<float> &b0_cluster_y,
    TTreeReaderArray<float> &b0_cluster_z,
    TTreeReaderArray<float> &b0_cluster_energy,

    TTreeReaderArray<float> &b0_hits_x,
    TTreeReaderArray<float> &b0_hits_y,
    TTreeReaderArray<float> &b0_hits_z,
    TTreeReaderArray<float> &b0_hits_eDep
)
{
    double hit_x = -9999.0;
    double hit_y = -9999.0;
    double hit_z = -9999.0;
    double hit_deposited_energy = -9999.0;

    //---------------------------------------------
    // B0 EMCAL clusters
    //---------------------------------------------
    for (int b0cluster = 0; b0cluster < b0_cluster_x.GetSize(); ++b0cluster)
    {
        hit_x = b0_cluster_x[b0cluster];
        hit_y = b0_cluster_y[b0cluster];
        hit_z = b0_cluster_z[b0cluster];

        hit_deposited_energy =
            b0_cluster_energy[b0cluster] * 1.246;

        h_B0_emcal_occupancy_map->Fill(hit_x, hit_y);
        h_B0_emcal_cluster_energy->Fill(hit_deposited_energy);
    }

    //---------------------------------------------
    // B0 tracker hits
    //---------------------------------------------
    for (int b0hit = 0; b0hit < b0_hits_x.GetSize(); ++b0hit)
    {
        hit_x = b0_hits_x[b0hit];
        hit_y = b0_hits_y[b0hit];
        hit_z = b0_hits_z[b0hit];

        hit_deposited_energy =
            b0_hits_eDep[b0hit] * 1e6; // GeV -> keV

        h_B0_hit_energy_deposit->Fill(hit_deposited_energy);

        if (hit_deposited_energy < 10.0)
            continue;

        if (hit_z > 5700 && hit_z < 5990)
            h_B0_occupancy_map_layer_0->Fill(hit_x, hit_y);

        if (hit_z > 6100 && hit_z < 6200)
            h_B0_occupancy_map_layer_1->Fill(hit_x, hit_y);

        if (hit_z > 6400 && hit_z < 6500)
            h_B0_occupancy_map_layer_2->Fill(hit_x, hit_y);

        if (hit_z > 6700 && hit_z < 6750)
            h_B0_occupancy_map_layer_3->Fill(hit_x, hit_y);
    }
}



//for RECO electron
TLorentzVector positron_rc;
TLorentzVector decayElectron_rc;
TLorentzVector scatteredElectron_rc;
TLorentzVector JPsi_rc;

int iflagRCelectron = 0;

bool ProcessScatteredElectron(
    TTreeReaderArray<float> &reco_part_px,
    TTreeReaderArray<float> &reco_part_py,
    TTreeReaderArray<float> &reco_part_pz,
    TTreeReaderArray<float> &reco_part_energy,
    TTreeReaderArray<float> &reco_part_mass,
    TTreeReaderArray<float> &reco_part_charge,
    TTreeReaderArray<int>   &reco_part_pdg
)
{
    iflagRCelectron = 0;

    positron_rc.SetPxPyPzE(0,0,0,0);
    decayElectron_rc.SetPxPyPzE(0,0,0,0);
    scatteredElectron_rc.SetPxPyPzE(0,0,0,0);
    JPsi_rc.SetPxPyPzE(0,0,0,0);

    if (reco_part_px.GetSize() < 3)
        return false;

    std::vector<int> ele_indices;
    std::vector<int> pos_indices;

    /*for (int i = 0; i < reco_part_px.GetSize(); ++i)
    {
        int charge = reco_part_charge[i];

        if (charge == -1)
            ele_indices.push_back(i);

        if (charge == 1)
            pos_indices.push_back(i);
    }*/
    for (int i = 0; i < reco_part_px.GetSize(); ++i)
    {
        int charge = reco_part_charge[i];
        double mass = reco_part_mass[i];

        bool electronLikeMass =
            (mass >= 0.0 && mass < 0.02);

        if (charge == -1 && electronLikeMass)
            ele_indices.push_back(i);

        if (charge == 1 && electronLikeMass)
            pos_indices.push_back(i);
    }

    if (ele_indices.size() < 2 || pos_indices.size() < 1)
        return false;

    //---------------------------------------------
    // Find best J/psi = e+ e- combination
    //---------------------------------------------
    double bestDM = 999.0;

    int bestPosIndex = -1;
    int bestDecayEleIndex = -1;

    TLorentzVector bestPos;
    TLorentzVector bestDecayEle;
    TLorentzVector bestJPsi;

    for (int ipos : pos_indices)
    {
        TLorentzVector pos;
        pos.SetXYZM(
            reco_part_px[ipos],
            reco_part_py[ipos],
            reco_part_pz[ipos],
            reco_part_mass[ipos]
        );

        for (int iele : ele_indices)
        {
            TLorentzVector ele;
            ele.SetXYZM(
                reco_part_px[iele],
                reco_part_py[iele],
                reco_part_pz[iele],
                reco_part_mass[iele]
            );

            TLorentzVector candJPsi = pos + ele;

            double mass = candJPsi.M();
            double dM = fabs(mass - M_JPSI);

            bool inWindow =
                (mass > 2.9 && mass < 3.2);

            if (inWindow && dM < bestDM)
            {
                bestDM = dM;
                bestPosIndex = ipos;
                bestDecayEleIndex = iele;
                bestPos = pos;
                bestDecayEle = ele;
                bestJPsi = candJPsi;
            }
        }
    }

    if (bestPosIndex < 0 || bestDecayEleIndex < 0)
        return false;

    //---------------------------------------------
    // Select scattered electron from remaining e-
    //---------------------------------------------
    int bestScatteredIndex = -1;
    double bestScatteredEnergy = -999.0;

    TLorentzVector bestScatteredEle;

    for (int iele : ele_indices)
    {
        if (iele == bestDecayEleIndex)
            continue;

        TLorentzVector ele;
        ele.SetXYZM(
            reco_part_px[iele],
            reco_part_py[iele],
            reco_part_pz[iele],
            reco_part_mass[iele]
        );

        // Choose highest-energy remaining electron as scattered electron
        if (ele.E() > bestScatteredEnergy)
        {
            bestScatteredEnergy = ele.E();
            bestScatteredIndex = iele;
            bestScatteredEle = ele;
        }
    }

    if (bestScatteredIndex < 0)
        return false;

    //---------------------------------------------
    // Save selected particles globally
    //---------------------------------------------
    positron_rc = bestPos;
    decayElectron_rc = bestDecayEle;
    scatteredElectron_rc = bestScatteredEle;
    JPsi_rc = bestJPsi;

    //---------------------------------------------
    // Fill histograms
    //---------------------------------------------
    h_jpsi_mass_RC->Fill(JPsi_rc.M());
    h_scattered_electron_energy_RC->Fill(scatteredElectron_rc.E());
    h_scattered_electron_eta_RC->Fill(scatteredElectron_rc.Eta());
    h_scattered_electron_pt_RC->Fill(scatteredElectron_rc.Pt());
    h_scattered_positron_eta_RC->Fill(positron_rc.Eta());
    h_decay_electron_eta_RC->Fill(decayElectron_rc.Eta());

    iflagRCelectron = 1;

    return true;
}


//for calorimeter
double matchedEcalEnergy = -999.0;
double matchedEcalDR     = 999.0;
double matchedEoverP     = -999.0;

bool MatchScatteredElectronToEcal(
    TTreeReaderArray<float> &ecalN_energy,
    TTreeReaderArray<float> &ecalN_x,
    TTreeReaderArray<float> &ecalN_y,
    TTreeReaderArray<float> &ecalN_z,
    TTreeReaderArray<unsigned int> &ecalN_nHits
)
{
    matchedEcalEnergy = -999.0;
    matchedEcalDR     = 999.0;
    matchedEoverP     = -999.0;

    double bestDR = 999.0;
    double bestE  = -999.0;

    for (int iclus = 0; iclus < ecalN_energy.GetSize(); ++iclus)
    {
        if (ecalN_nHits[iclus] < 4)
            continue;

        double x = ecalN_x[iclus];
        double y = ecalN_y[iclus];
        double z = ecalN_z[iclus];

        double theta = TMath::ATan2(TMath::Sqrt(x*x + y*y), z);

        double eta = -TMath::Log(TMath::Tan(theta / 2.0));

        double phi = TMath::ATan2(y, x);

        double deta = eta - scatteredElectron_rc.Eta();

        double dphi = TVector2::Phi_mpi_pi(phi - scatteredElectron_rc.Phi());

        double dR = TMath::Sqrt(deta*deta + dphi*dphi);

        if (dR < bestDR)
        {
            bestDR = dR;
            bestE  = ecalN_energy[iclus];
        }
    }

    if (bestE < 0)
        return false;

    matchedEcalEnergy = bestE;
    matchedEcalDR     = bestDR;
    matchedEoverP     = bestE / scatteredElectron_rc.P();

    bool passDR =
        (matchedEcalDR < 0.4);

    bool passEoverP =
        (matchedEoverP > 0.9 &&
         matchedEoverP < 1.2);

    if (!(passDR && passEoverP))
        return false;

    h_matched_ecal_energy->Fill(matchedEcalEnergy);
    h_matched_ecal_dr->Fill(matchedEcalDR);
    h_matched_eop->Fill(matchedEoverP);

    return true;
}

//for inclusive variables
void ProcessInclusiveKinematics(
    TTreeReaderArray<float> &ik_electron_x,
    TTreeReaderArray<float> &ik_electron_Q2,
    TTreeReaderArray<float> &ik_electron_y,

    TTreeReaderArray<float> &ik_esigma_x,
    TTreeReaderArray<float> &ik_esigma_Q2,
    TTreeReaderArray<float> &ik_esigma_y,

    TTreeReaderArray<float> &ik_jb_x,
    TTreeReaderArray<float> &ik_jb_Q2,
    TTreeReaderArray<float> &ik_jb_y,

    TTreeReaderArray<float> &ik_DA_x,
    TTreeReaderArray<float> &ik_DA_Q2,
    TTreeReaderArray<float> &ik_DA_y
)
{
    for (int i = 0; i < ik_electron_x.GetSize(); ++i)
    {
        double xVal  = ik_electron_x[i];
        double q2Val = ik_electron_Q2[i];
        double yVal  = ik_electron_y[i];

        if (xVal > x_low && xVal < x_high &&
            q2Val > q2_low && q2Val < q2_high &&
            yVal > y_low && yVal < y_high)
        {
            EM_x->Fill(xVal);
            EM_Q2->Fill(q2Val);
            EM_y->Fill(yVal);
        }
    }

    for (int i = 0; i < ik_esigma_x.GetSize(); ++i)
    {
        double xVal  = ik_esigma_x[i];
        double q2Val = ik_esigma_Q2[i];
        double yVal  = ik_esigma_y[i];

        if (xVal > x_low && xVal < x_high &&
            q2Val > q2_low && q2Val < q2_high &&
            yVal > y_low && yVal < y_high)
        {
            ESig_x_in->Fill(xVal);
            ESig_Q2_in->Fill(q2Val);
            ESig_y_in->Fill(yVal);
        }
    }

    for (int i = 0; i < ik_jb_x.GetSize(); ++i)
    {
        double xVal  = ik_jb_x[i];
        double q2Val = ik_jb_Q2[i];
        double yVal  = ik_jb_y[i];

        if (xVal > x_low && xVal < x_high &&
            q2Val > q2_low && q2Val < q2_high &&
            yVal > y_low && yVal < y_high)
        {
            JB_x_in->Fill(xVal);
            JB_Q2_in->Fill(q2Val);
            JB_y_in->Fill(yVal);
        }
    }

    for (int i = 0; i < ik_DA_x.GetSize(); ++i)
    {
        double xVal  = ik_DA_x[i];
        double q2Val = ik_DA_Q2[i];
        double yVal  = ik_DA_y[i];

        if (xVal > x_low && xVal < x_high &&
            q2Val > q2_low && q2Val < q2_high &&
            yVal > y_low && yVal < y_high)
        {
            DA_x_in->Fill(xVal);
            DA_Q2_in->Fill(q2Val);
            DA_y_in->Fill(yVal);
        }
    }
}

TLorentzVector incbeame;
TLorentzVector incbeam;

TLorentzVector mc_positron;
TLorentzVector mc_decayElectron;
TLorentzVector mc_scatteredElectron;
TLorentzVector mc_recoilProton;
TLorentzVector mc_photon;
TLorentzVector mc_jpsi;

int iflagMC = 0;

bool ProcessMCParticles(
    TTreeReaderArray<double> &mc_px,
    TTreeReaderArray<double> &mc_py,
    TTreeReaderArray<double> &mc_pz,
    TTreeReaderArray<double> &mc_mass,
    TTreeReaderArray<float>  &mc_charge,
    TTreeReaderArray<int>    &mc_pdg,
    TTreeReaderArray<int>    &mc_gen_status
)
{
    iflagMC = 0;

    incbeame.SetPxPyPzE(0,0,0,0);
    incbeam.SetPxPyPzE(0,0,0,0);

    mc_positron.SetPxPyPzE(0,0,0,0);
    mc_decayElectron.SetPxPyPzE(0,0,0,0);
    mc_scatteredElectron.SetPxPyPzE(0,0,0,0);
    mc_recoilProton.SetPxPyPzE(0,0,0,0);
    mc_photon.SetPxPyPzE(0,0,0,0);
    mc_jpsi.SetPxPyPzE(0,0,0,0);

    bool has_incbeame = false;
    bool has_incbeam = false;
    bool has_positron = false;
    bool has_recoilProton = false;
    bool has_electron1 = false;
    bool has_electron2 = false;
    bool has_decay = false;
    bool has_scattered = false;

    TLorentzVector electron1;
    TLorentzVector electron2;

    for (int imc = 0; imc < mc_px.GetSize(); ++imc)
    {
        int pdg = mc_pdg[imc];
        int status = mc_gen_status[imc];
        int charge = mc_charge[imc];

        double px = mc_px[imc];
        double py = mc_py[imc];
        double pz = mc_pz[imc];
        double m  = mc_mass[imc];

        //for selecting the incoming particles beam
        if (status == 4)
        {
            if (pdg == 11 && charge == -1)
            {
                incbeame.SetXYZM(px, py, pz, m);
                has_incbeame = true;
            }
            else if (pdg == 2212 && charge == 1)
            {
                incbeam.SetXYZM(px, py, pz, m);
                //incbeam.RotateY(0.025);
                has_incbeam = true;
            }
        } //for selecting the scattered particles
        else if (status == 1)
        {
            if (pdg == 11 && charge == -1)
            {
                if (!has_electron1)
                {
                    electron1.SetXYZM(px, py, pz, m);
                    has_electron1 = true;
                }
                else if (!has_electron2)
                {
                    electron2.SetXYZM(px, py, pz, m);
                    has_electron2 = true;
                }
            }
            else if (pdg == -11 && charge == 1)
            {
                mc_positron.SetXYZM(px, py, pz, m);
                has_positron = true;
            }
            else if (pdg == 2212 && charge == 1)
            {
                mc_recoilProton.SetXYZM(px, py, pz, m);
                //mc_recoilProton.RotateY(0.025);
                has_recoilProton = true;
            }
            else if (pdg == 22 && charge == 0)
            {
                mc_photon.SetXYZM(px, py, pz, m);
            }
        }
        else if (status == 2 && pdg == 443)
        {
            mc_jpsi.SetXYZM(px, py, pz, m);
        }
    }

    if (!has_incbeam || !has_incbeame || !has_positron || !has_recoilProton)
        return false;

    if (!has_electron1 && !has_electron2)
        return false;

    if (has_electron1 && has_electron2)
    {
        TLorentzVector cand1 = mc_positron + electron1;
        TLorentzVector cand2 = mc_positron + electron2;

        bool cand1Good = (cand1.M() > 2.9 && cand1.M() < 3.2);
        bool cand2Good = (cand2.M() > 2.9 && cand2.M() < 3.2);

        double dM1 = fabs(cand1.M() - M_JPSI);
        double dM2 = fabs(cand2.M() - M_JPSI);

        if (cand1Good && (!cand2Good || dM1 < dM2))
        {
            mc_decayElectron = electron1;
            mc_scatteredElectron = electron2;
            mc_jpsi = cand1;
            has_decay = true;
            has_scattered = true;
        }
        else if (cand2Good)
        {
            mc_decayElectron = electron2;
            mc_scatteredElectron = electron1;
            mc_jpsi = cand2;
            has_decay = true;
            has_scattered = true;
        }
        else
        {
            if (electron1.Pz() > electron2.Pz())
            {
                mc_decayElectron = electron1;
                mc_scatteredElectron = electron2;
            }
            else
            {
                mc_decayElectron = electron2;
                mc_scatteredElectron = electron1;
            }

            has_decay = true;
            has_scattered = true;
        }
    }
    else if (has_electron1)
    {
        TLorentzVector cand = mc_positron + electron1;

        if (cand.M() > 2.9 && cand.M() < 3.2)
        {
            mc_decayElectron = electron1;
            mc_jpsi = cand;
            has_decay = true;
        }
        else
        {
            mc_scatteredElectron = electron1;
            has_scattered = true;
        }
    }
    else if (has_electron2)
    {
        TLorentzVector cand = mc_positron + electron2;

        if (cand.M() > 2.9 && cand.M() < 3.2)
        {
            mc_decayElectron = electron2;
            mc_jpsi = cand;
            has_decay = true;
        }
        else
        {
            mc_scatteredElectron = electron2;
            has_scattered = true;
        }
    }

    if (!has_scattered) return false;

    if (has_decay) h_mc_jpsi_mass->Fill(mc_jpsi.M());
    
    // MC positron
    h_mc_positron_px->Fill(mc_positron.Px());
    h_mc_positron_py->Fill(mc_positron.Py());
    h_mc_positron_pz->Fill(mc_positron.Pz());
    h_mc_positron_pt->Fill(mc_positron.Pt());
    h_mc_positron_E->Fill(mc_positron.E());
    h_mc_positron_eta->Fill(mc_positron.Eta());

    // MC decay electron
    if (has_decay)
    {
        h_mc_decay_electron_px->Fill(mc_decayElectron.Px());
        h_mc_decay_electron_py->Fill(mc_decayElectron.Py());
        h_mc_decay_electron_pz->Fill(mc_decayElectron.Pz());
        h_mc_decay_electron_pt->Fill(mc_decayElectron.Pt());
        h_mc_decay_electron_E->Fill(mc_decayElectron.E());
        h_mc_decay_electron_eta->Fill(mc_decayElectron.Eta());
    }

    // MC scattered electron
    h_mc_scattered_electron_px->Fill(mc_scatteredElectron.Px());
    h_mc_scattered_electron_py->Fill(mc_scatteredElectron.Py());
    h_mc_scattered_electron_pz->Fill(mc_scatteredElectron.Pz());
    h_mc_scattered_electron_pt->Fill(mc_scatteredElectron.Pt());
    h_mc_scattered_electron_E->Fill(mc_scatteredElectron.E());
    h_mc_scattered_electron_eta->Fill(mc_scatteredElectron.Eta());

    // MC recoil proton
    h_mc_recoil_proton_px->Fill(mc_recoilProton.Px());
    h_mc_recoil_proton_py->Fill(mc_recoilProton.Py());
    h_mc_recoil_proton_pz->Fill(mc_recoilProton.Pz());
    h_mc_recoil_proton_pt->Fill(mc_recoilProton.Pt());
    h_mc_recoil_proton_E->Fill(mc_recoilProton.E());
    h_mc_recoil_proton_eta->Fill(mc_recoilProton.Eta());

    iflagMC = 1;

    return true;
}

bool PassMissingMassCut(
    const TLorentzVector &scatElectron,
    const TLorentzVector &recoilProton,
    const TLorentzVector &jpsi,
    TH1F *h_mx,
    TH1F *h_mx2
)
{
    TLorentzVector total_in  = incbeame + incbeam;
    TLorentzVector total_out = scatElectron + recoilProton + jpsi;
    TLorentzVector missing   = total_in - total_out;

    double Mx2 = missing.M2();
    double Mx  = missing.M();

    h_mx2->Fill(Mx2);
    h_mx->Fill(Mx);

    if (fabs(Mx) > 3.0)
        return false;

    return true;
}


//reconstructing RECO level kinematics
double reco_y_electron = -999.0;
double reco_q2_electron = -999.0;
double reco_x_electron = -999.0;

bool ProcessRecoElectronKinematics()
{
    reco_x_EM  = -999.0;
    reco_Q2_EM = -999.0;
    reco_y_EM  = -999.0;

    //---------------------------------------------
    // Electron method using 4-vector definitions
    //---------------------------------------------
    TLorentzVector q = incbeame - scatteredElectron_rc;

    double q2_EM = -q.M2();

    double denom_y = incbeam.Dot(incbeame);

    if (TMath::Abs(denom_y) < 1e-12)
        return false;

    double y_EM = incbeam.Dot(q) / denom_y;

    double denom_x = 2.0 * incbeam.Dot(q);

    if (TMath::Abs(denom_x) < 1e-12)
        return false;

    double x_EM = q2_EM / denom_x;

    //---------------------------------------------
    // Fill histograms
    //---------------------------------------------
    bool passEMKinematics =
    (
        x_EM  > x_low  &&
        x_EM  < x_high &&

        q2_EM > q2_low &&
        q2_EM < q2_high &&

        y_EM  > y_low  &&
        y_EM  < y_high
    );

    if (passEMKinematics)
    {
        h_reco_EM_x->Fill(x_EM);
        h_reco_EM_Q2->Fill(q2_EM);
        h_reco_EM_y->Fill(y_EM);

        reco_x_EM  = x_EM;
        reco_Q2_EM = q2_EM;
        reco_y_EM  = y_EM;
    }

    return true;
}

bool ProcessRecoHadronicKinematics(
    const TLorentzVector &recoilProton
)
{
    reco_x_JB = reco_Q2_JB = reco_y_JB = -999.0;
    reco_x_eSigma = reco_Q2_eSigma = reco_y_eSigma = -999.0;
    reco_x_DA = reco_Q2_DA = reco_y_DA = -999.0;

    //option1
    /*TLorentzVector HFS = recoilProton + decayElectron_rc + positron_rc;

    double hfs_pt2 = HFS.Px()*HFS.Px() + HFS.Py()*HFS.Py();
    double hfs_pt = TMath::Sqrt(hfs_pt2);
    double hfs_sigma = HFS.E() - HFS.Pz();*/
    
    //option2
    //---------------------------------------------
    // Build HFS manually for exclusive J/psi event
    //---------------------------------------------
    double pxsum = 0.0;
    double pysum = 0.0;
    double pzsum = 0.0;
    double Esum  = 0.0;

    auto AddToHFS = [&](const TLorentzVector &p)
    {
        pxsum += p.Px();
        pysum += p.Py();
        pzsum += p.Pz();
        Esum  += p.E();
    };

    AddToHFS(recoilProton);
    AddToHFS(decayElectron_rc);
    AddToHFS(positron_rc);

    double hfs_pt2 = pxsum*pxsum + pysum*pysum;
    double hfs_pt = TMath::Sqrt(hfs_pt2);
    double hfs_sigma = Esum - pzsum;

    //---------------------------------------------
    // JB method
    //---------------------------------------------
    double y_JB = hfs_sigma / (2.0 * incbeame.E());

    double q2_JB = (y_JB < 0.999999) ? hfs_pt2 / (1.0 - y_JB) : -999.0;

    double x_JB = (TMath::Abs(4.0 * incbeame.E() * incbeam.E() * y_JB) > 1e-12) ? q2_JB / (4.0 * incbeame.E() * incbeam.E() * y_JB) : -999.0;

    h_reco_JB_HFS_sigma->Fill(hfs_sigma);
    h_reco_JB_HFS_pt->Fill(hfs_pt);

    //---------------------------------------------
    // Fill histograms
    //---------------------------------------------
    bool passJBKinematics =
    (
        x_JB  > x_low  &&
        x_JB  < x_high &&

        q2_JB > q2_low &&
        q2_JB < q2_high &&

        y_JB  > y_low  &&
        y_JB  < y_high
    );

    if (passJBKinematics)
    {
        h_reco_JB_x->Fill(x_JB);
        h_reco_JB_Q2->Fill(q2_JB);
        h_reco_JB_y->Fill(y_JB);

        reco_x_JB  = x_JB;
        reco_Q2_JB = q2_JB;
        reco_y_JB  = y_JB;
    }

    //---------------------------------------------
    // eSigma method
    //---------------------------------------------
    double sig_e = scatteredElectron_rc.E() - scatteredElectron_rc.Pz();

    double y_e = 1.0 - sig_e / (2.0 * incbeame.E());

    double sig_tot = sig_e + hfs_sigma;

    double y_Sig = (TMath::Abs(sig_tot) > 1e-12) ? hfs_sigma / sig_tot : -999.0;

    double q2_Sig = (TMath::Abs(1.0 - y_Sig) > 1e-12) ? scatteredElectron_rc.Pt()*scatteredElectron_rc.Pt() / (1.0 - y_Sig) : -999.0;

    double q2_e = (TMath::Abs(1.0 - y_e) > 1e-12) ? scatteredElectron_rc.Pt()*scatteredElectron_rc.Pt() / (1.0 - y_e) : -999.0;

    double x_Sig = (TMath::Abs(4.0 * incbeame.E() * incbeam.E() * y_Sig) > 1e-12)
        ? q2_Sig / (4.0 * incbeame.E() * incbeam.E() * y_Sig) : -999.0;

    double y_eSig =  (TMath::Abs(4.0 * incbeame.E() * incbeam.E() * x_Sig) > 1e-12)
        ? q2_e / (4.0 * incbeame.E() * incbeam.E() * x_Sig) : -999.0;

    h_reco_eSigma_HFS_sigma->Fill(sig_tot);

    //---------------------------------------------
    // eSigma phase-space selection, Fill Histograms
    //---------------------------------------------
    bool passESigmaKinematics =
    (
        x_Sig  > x_low  &&
        x_Sig  < x_high &&

        q2_Sig > q2_low &&
        q2_Sig < q2_high &&

        y_eSig > y_low  &&
        y_eSig < y_high
    );

    if (passESigmaKinematics)
    {
        h_reco_eSigma_x->Fill(x_Sig);
        h_reco_eSigma_Q2->Fill(q2_Sig);
        h_reco_eSigma_y->Fill(y_eSig);

        reco_x_eSigma  = x_Sig;
        reco_Q2_eSigma = q2_Sig;
        reco_y_eSigma  = y_eSig;
    }

    //---------------------------------------------
    // DA method
    //---------------------------------------------
    if (hfs_pt2 < 1e-12)
        return true;

    double tanGammaHalf =  hfs_sigma / TMath::Sqrt(hfs_pt2);

    double tanElectronHalf = TMath::Tan(scatteredElectron_rc.Theta() / 2.0);

    double denomY_DA = tanElectronHalf + tanGammaHalf;

    double y_DA = -999.0;

    if (TMath::Abs(denomY_DA) > 1e-12) y_DA = tanGammaHalf / denomY_DA;

    double denomQ2_DA = tanElectronHalf * (tanElectronHalf + tanGammaHalf);

    double q2_DA = -999.0;

    if (TMath::Abs(denomQ2_DA) > 1e-12)
    {
        q2_DA = 4.0 * incbeame.E() * incbeame.E() / denomQ2_DA;
    }

    double denomX_DA = 4.0 * incbeame.E() * incbeam.E() * y_DA;

    double x_DA = -999.0;

    if (TMath::Abs(denomX_DA) > 1e-12) x_DA = q2_DA / denomX_DA;

    //---------------------------------------------
    // Double-Angle phase-space selection, Fill Histograms
    //---------------------------------------------
    bool passDAKinematics =
    (
        x_DA  > x_low  &&
        x_DA  < x_high &&

        q2_DA > q2_low &&
        q2_DA < q2_high &&

        y_DA  > y_low  &&
        y_DA  < y_high
    );

    if (passDAKinematics)
    {
        h_reco_DA_x->Fill(x_DA);
        h_reco_DA_Q2->Fill(q2_DA);
        h_reco_DA_y->Fill(y_DA);

        reco_x_DA  = x_DA;
        reco_Q2_DA = q2_DA;
        reco_y_DA  = y_DA;
    }
    
    return true;
}



void FillKinematicResolutionPlots()
{
    double lager_x  = x;
    double lager_Q2 = Q2;
    double lager_y  = y;
    
    //might be restored later
    /*if (!(lager_x  > x_low  && lager_x  < x_high &&
     lager_Q2 > q2_low && lager_Q2 < q2_high &&
     lager_y  > y_low  && lager_y  < y_high))
     {
     return;
     }*/
    
    auto fillOne = [](
                      double truth,
                      double reco,
                      TH2F *h2_frac,
                      TH1F *h1_frac,
                      TH2F *h2_res
                      )
    {
        if (truth <= 0.0) return;
        if (reco <= -900.0) return;
        
        double frac = (truth - reco) / truth;
        
        h2_frac->Fill(truth, frac);
        h1_frac->Fill(frac);
        h2_res->Fill(truth, reco);
    };
    
    // Electron method
    fillOne(lager_x,  reco_x_EM,  h2_dx_EM,  h_dx_EM,  h2_x_EM_res);
    fillOne(lager_Q2, reco_Q2_EM, h2_dQ2_EM, h_dQ2_EM, h2_Q2_EM_res);
    fillOne(lager_y,  reco_y_EM,  h2_dy_EM,  h_dy_EM,  h2_y_EM_res);
    
    // JB method
    fillOne(lager_x,  reco_x_JB,  h2_dx_JB,  h_dx_JB,  h2_x_JB_res);
    fillOne(lager_Q2, reco_Q2_JB, h2_dQ2_JB, h_dQ2_JB, h2_Q2_JB_res);
    fillOne(lager_y,  reco_y_JB,  h2_dy_JB,  h_dy_JB,  h2_y_JB_res);
    
    // eSigma method
    fillOne(lager_x,  reco_x_eSigma,  h2_dx_eSigma,  h_dx_eSigma,  h2_x_eSigma_res);
    fillOne(lager_Q2, reco_Q2_eSigma, h2_dQ2_eSigma, h_dQ2_eSigma, h2_Q2_eSigma_res);
    fillOne(lager_y,  reco_y_eSigma,  h2_dy_eSigma,  h_dy_eSigma,  h2_y_eSigma_res);
    
    // DA method
    fillOne(lager_x,  reco_x_DA,  h2_dx_DA,  h_dx_DA,  h2_x_DA_res);
    fillOne(lager_Q2, reco_Q2_DA, h2_dQ2_DA, h_dQ2_DA, h2_Q2_DA_res);
    fillOne(lager_y,  reco_y_DA,  h2_dy_DA,  h_dy_DA,  h2_y_DA_res);
}

void FillElectronResolutionPlots()
{
    //-----------------------------------------
    // Truth electron energy
    //-----------------------------------------
    double mcEnergy = mc_scatteredElectron.E();

    if (mcEnergy < 1e-12)
        return;

    //-----------------------------------------
    // Calorimeter resolution
    //-----------------------------------------
    double cal_dE_over_E = (mcEnergy - matchedEcalEnergy) / mcEnergy;

    h_CalEnergy_minus_SCElecEnergy->Fill(cal_dE_over_E);
    h_Cal_DE_over_E_vs_Eta->Fill(mc_scatteredElectron.Eta(),cal_dE_over_E);

    //-----------------------------------------
    // Track resolution
    //-----------------------------------------
    double track_dE_over_E = (mcEnergy - scatteredElectron_rc.E())/ mcEnergy;

    h_TrackEnergy_minus_SCElecEnergy->Fill(track_dE_over_E);
    h_Track_DE_over_E_vs_Eta->Fill(mc_scatteredElectron.Eta(), track_dE_over_E);
}

//do I need this applied to the final condition? if yes, then I need a bool like in reco
void FillMCEminusPz()
{
    TLorentzVector finalState = mc_decayElectron + mc_recoilProton + mc_positron + mc_scatteredElectron;

    double eMinusPz = finalState.E() - finalState.Pz();

    if (eMinusPz > 15 && eMinusPz < 25) h_mc_e_minus_pz->Fill(eMinusPz);
}

bool FillRecoEminusPz()
{
    TLorentzVector finalState = decayElectron_rc + positron_rc + scatteredElectron_rc;

    if (iflagRPpproton) finalState += RP_proton_rc;

    else if (iflagB0proton) finalState += B0_proton_rc;

    else
        return false;

    double eMinusPz = finalState.E() - finalState.Pz();

    h_reco_e_minus_pz->Fill(eMinusPz);

    if (eMinusPz > 15 && eMinusPz < 25)
        return true;

    return false;
}

double ComputeMinusT(
    const TLorentzVector &beamProton,
    const TLorentzVector &recoilProton
)
{
    TLorentzVector diff = beamProton - recoilProton;

    double tValue = diff.Dot(diff);

    return -tValue;
}

double ComputeMethodLT()
{
    TLorentzVector jpsiReco =
        decayElectron_rc + positron_rc;

    TLorentzVector protonReco =
        incbeam - (jpsiReco + scatteredElectron_rc - incbeame);

    double pAplus =
        protonReco.E() + protonReco.Pz();

    if (TMath::Abs(pAplus) < 1e-12)
        return -999.0;

    double pT2 =
        protonReco.Pt() * protonReco.Pt();

    double pAminus =
        (incbeam.M() * incbeam.M() + pT2) / pAplus;

    TLorentzVector protonCorr;

    protonCorr.SetPxPyPzE(
        protonReco.Px(),
        protonReco.Py(),
        (pAplus - pAminus) / 2.0,
        (pAplus + pAminus) / 2.0
    );

    double minusT =
        -(incbeam - protonCorr).Dot(incbeam - protonCorr);

    return minusT;
}

void FillTDistributions()
{
    double minusT_truth = -t;

    if (minusT_truth <= 0 || minusT_truth >= 5)
        return;

    if (iflagMC)
    {
        double minusT_MC = ComputeMinusT(incbeam, mc_recoilProton);

        if (minusT_MC > 0 && minusT_MC < 5)
            h_t_MC->Fill(minusT_MC);
    }

    if (iflagRPpproton)
    {
        double minusT_RP = ComputeMinusT(incbeam, RP_proton_rc);

        if (minusT_RP > 0 && minusT_RP < 5)
        {
            h_t_RECO_RP->Fill(minusT_RP);
            h_t_RECO_RPB0->Fill(minusT_RP);
            h_t2D_RP->Fill(minusT_truth, minusT_RP);
            h_t2D_RPB0->Fill(minusT_truth, minusT_RP);
        }
    }

    if (iflagB0proton)
    {
        double minusT_B0 = ComputeMinusT(incbeam, B0_proton_rc);

        if (minusT_B0 > 0 && minusT_B0 < 5)
        {
            h_t_RECO_B0->Fill(minusT_B0);
            h_t_RECO_RPB0->Fill(minusT_B0);
            h_t2D_B0->Fill(minusT_truth, minusT_B0);
            h_t2D_RPB0->Fill(minusT_truth, minusT_B0);
        }
    }
}

void FillMethodLT()
{
    if (!iflagRCelectron)
        return;

    double minusT_truth = -t;
    double minusT_MethodL = ComputeMethodLT();

    if (minusT_truth <= 0 || minusT_truth >= 5)
        return;

    if (minusT_MethodL > 0 && minusT_MethodL < 5)
    {
        h_t_MethodL_RECO->Fill(minusT_MethodL);
        h_t2D_MethodL->Fill(minusT_truth, minusT_MethodL);
    }
}

void FillCombinedTWithMethodL()
{
    double minusT_truth = -t;

    if (minusT_truth <= 0 || minusT_truth >= 5)
        return;

    //-----------------------------------------
    // Acceptance definition
    //-----------------------------------------
    bool accepted =
        iflagRPpproton ||
        iflagB0proton ||
        iflagRCelectron;

    if (accepted)
    {
        h_t_lAger_accept->Fill(minusT_truth);
    }

    //-----------------------------------------
    // RP
    //-----------------------------------------
    if (iflagRPpproton)
    {
        double minusT_RP =
            ComputeMinusT(incbeam, RP_proton_rc);

        if (minusT_RP > 0 && minusT_RP < 5)
        {
            h_t_RECO_RPB0_MethodL->Fill(minusT_RP);
            h_t2D_RPB0_MethodL->Fill(
                minusT_truth,
                minusT_RP
            );

            return;
        }
    }

    //-----------------------------------------
    // B0
    //-----------------------------------------
    else if (iflagB0proton)
    {
        double minusT_B0 =
            ComputeMinusT(incbeam, B0_proton_rc);

        if (minusT_B0 > 0 && minusT_B0 < 5)
        {
            h_t_RECO_RPB0_MethodL->Fill(minusT_B0);
            h_t2D_RPB0_MethodL->Fill(
                minusT_truth,
                minusT_B0
            );

            return;
        }
    }

    //-----------------------------------------
    // Method L
    //-----------------------------------------
    else if (iflagRCelectron)
    {
        double minusT_MethodL =
            ComputeMethodLT();

        if (minusT_MethodL > 0 && minusT_MethodL < 5)
        {
            h_t_RECO_RPB0_MethodL->Fill(
                minusT_MethodL
            );

            h_t2D_RPB0_MethodL->Fill(
                minusT_truth,
                minusT_MethodL
            );
        }
    }
}

void BuildTCorrection()
{
    h_CorrectionFactor =
        (TH1F*)h_t_lAger->Clone("h_CorrectionFactor");

    h_CorrectionFactor->SetTitle(
        "Correction Factor c_{f} = Truth / Accepted Truth;-t [GeV^{2}];c_{f}"
    );

    h_CorrectionFactor->Divide(h_t_lAger_accept);

    h_t_RPB0_MethodL_corrected = (TH1F*)h_t_RECO_RPB0_MethodL->Clone("h_t_RPB0_MethodL_corrected");

    h_t_RPB0_MethodL_corrected->SetTitle("Corrected RP+B0+MethodL -t;-t [GeV^{2}];Counts");

    h_t_RPB0_MethodL_corrected->Multiply(h_CorrectionFactor);
}

#endif
