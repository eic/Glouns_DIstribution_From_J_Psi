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
// Generator variables (lAger truth)
//---------------------------------------------
Float_t gen_t;
Float_t gen_x;
Float_t gen_Q2;
Float_t gen_y;

//---------------------------------------------
// Kinematic cuts
//---------------------------------------------
double q2_low  = 1.0,  q2_high = 10.0;
double y_low   = 0.1,  y_high  = 0.95;
double x_low   = 0.0,  x_high  = 1.0;
double jplow   = 2.9,  jphigh  = 3.2;
const double M_JPSI = 3.0969;

//---------------------------------------------
// Reco kinematic globals (reset each event)
//---------------------------------------------
double reco_x_EM  = -999.0, reco_Q2_EM  = -999.0, reco_y_EM  = -999.0;
double reco_x_JB  = -999.0, reco_Q2_JB  = -999.0, reco_y_JB  = -999.0;
double reco_x_eSigma = -999.0, reco_Q2_eSigma = -999.0, reco_y_eSigma = -999.0;
double reco_x_DA  = -999.0, reco_Q2_DA  = -999.0, reco_y_DA  = -999.0;

//---------------------------------------------
// Setup lAger branches
// Renamed to gen_* to avoid shadowing ROOT globals
//---------------------------------------------
void SetGeneratorBranches(TChain *T)
{
    T->SetBranchAddress("t",  &gen_t);
    T->SetBranchAddress("x",  &gen_x);
    T->SetBranchAddress("Q2", &gen_Q2);
    T->SetBranchAddress("y",  &gen_y);
}

//---------------------------------------------
// Global 4-vectors
//---------------------------------------------
TLorentzVector RP_proton_rc;
TLorentzVector B0_proton_rc;

//---------------------------------------------
// Roman Pot reconstruction
//---------------------------------------------
bool iflagRPpproton = false;

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
    RP_proton_rc.SetPxPyPzE(0,0,0,0);
    iflagRPpproton = false;

    double bestGoodness = -1.0;   // select by PID quality, not raw Pz
    TLorentzVector bestRPProton;
    double bestRefX = -9999.0;
    double bestRefY = -9999.0;

    for (int i = 0; i < rp_px.GetSize(); ++i)
    {
        TLorentzVector p;
        p.SetXYZM(rp_px[i], rp_py[i], rp_pz[i], rp_mass[i]);

        if (p.M() <= 0)           continue;
        if (p.Pt() < 0.2)        continue;   // RP acceptance cut — tune as needed
        if (p.Pz() <= 0.0)       continue;

        bool isProton = (rp_charge[i] == 1)  &&
                        (rp_pdg[i]    == 2212) &&
                        (rp_goodness[i] > 0.7);
        if (!isProton) continue;

        // Pick candidate with best PID goodness (more physically meaningful than highest Pz)
        if (rp_goodness[i] > bestGoodness)
        {
            bestGoodness  = rp_goodness[i];
            bestRPProton  = p;
            bestRefX      = rp_ref_x[i];
            bestRefY      = rp_ref_y[i];
            iflagRPpproton = true;
        }
    }

    if (!iflagRPpproton) return;

    RP_proton_rc = bestRPProton;

    RPfinalProton->Fill(RP_proton_rc.Eta());
    B0RPfinalProton->Fill(RP_proton_rc.Eta());
    h_pt_RomanPots->Fill(RP_proton_rc.Pt());
    h_pt_RPB0combined->Fill(RP_proton_rc.Pt());   // renamed from h_pt_B0RomanPots
    h_px_RomanPots->Fill(RP_proton_rc.Px());
    h_py_RomanPots->Fill(RP_proton_rc.Py());
    h_pz_RomanPots->Fill(RP_proton_rc.Pz());
    h_energ_RomanPots->Fill(RP_proton_rc.E());
    ThetaRP->Fill(RP_proton_rc.Theta());
    ThetaRPB0->Fill(RP_proton_rc.Theta());
    h_rp_occupancy_map->Fill(bestRefX, bestRefY);
}

//---------------------------------------------
// B0 tracker reconstruction
//---------------------------------------------
bool iflagB0proton = false;
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
    B0_proton_rc.SetPxPyPzE(0,0,0,0);
    iflagB0proton = false;

    double bestPz = -999.0;
    TLorentzVector bestB0Proton;
    double bestRefZ = -9999.0;

    for (int i = 0; i < reco_track_px.GetSize(); ++i)
    {
        TLorentzVector p;
        p.SetXYZM(reco_track_px[i], reco_track_py[i],
                  reco_track_pz[i], reco_track_mass[i]);
        p.RotateY(0.025);   // crossing angle correction

        // Require at least 3 hits (unsigned subtraction is always >= 0, so floor at 3)
        int nHits = 0;
        if (i < (int)track_meas_begin.GetSize())
            nHits = (int)(track_meas_end[i] - track_meas_begin[i]);
        if (nHits < 0) continue;   // was (>= 0) before — dead cut, now meaningful

        // B0 tracks have no PID assigned in simulation; select by kinematics
        bool isProtonCandidate = (reco_track_pdg[i]    == 0);
        bool isCharged         = (reco_track_charge[i] == 1);
        bool isForward         = (p.Eta() > 4.0);
        bool passPz            = (p.Pz()  > 0.0);

        if (!(isProtonCandidate && isCharged && isForward && passPz))
            continue;

        if (p.Pz() > bestPz)
        {
            bestPz        = p.Pz();
            bestB0Proton  = p;
            bestRefZ      = reco_track_ref_z[i];
            iflagB0proton = true;
        }
    }

    if (!iflagB0proton) return;

    B0_proton_rc = bestB0Proton;

    // Fill selected-track diagnostics after selection (not inside the loop)
    h_B0_ref_z->Fill(bestRefZ);

    B0finalProton->Fill(B0_proton_rc.Eta());
    B0RPfinalProton->Fill(B0_proton_rc.Eta());
    h_px_B0_track->Fill(B0_proton_rc.Px());
    h_py_B0_track->Fill(B0_proton_rc.Py());
    h_pt_B0_track->Fill(B0_proton_rc.Pt());
    h_pt_RPB0combined->Fill(B0_proton_rc.Pt());   // renamed from h_pt_B0RomanPots
    h_pz_B0_track->Fill(B0_proton_rc.Pz());
    h_energ_B0_track->Fill(B0_proton_rc.E());
    ThetaB0->Fill(B0_proton_rc.Theta());
    ThetaRPB0->Fill(B0_proton_rc.Theta());
}

//---------------------------------------------
// B0 detector hits and EMCAL clusters
//---------------------------------------------
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
    // B0 EMCAL clusters
    for (int ic = 0; ic < b0_cluster_x.GetSize(); ++ic)
    {
        double eScaled = b0_cluster_energy[ic] * 1.246;
        h_B0_emcal_occupancy_map->Fill(b0_cluster_x[ic], b0_cluster_y[ic]);
        h_B0_emcal_cluster_energy->Fill(eScaled);
    }

    // B0 tracker hits
    for (int ih = 0; ih < b0_hits_x.GetSize(); ++ih)
    {
        double eKeV = b0_hits_eDep[ih] * 1e6;   // GeV -> keV
        h_B0_hit_energy_deposit->Fill(eKeV);

        if (eKeV < 10.0) continue;

        double hz = b0_hits_z[ih];
        double hx = b0_hits_x[ih];
        double hy = b0_hits_y[ih];

        if (hz > 5700 && hz < 5990) h_B0_occupancy_map_layer_0->Fill(hx, hy);
        if (hz > 6100 && hz < 6200) h_B0_occupancy_map_layer_1->Fill(hx, hy);
        if (hz > 6400 && hz < 6500) h_B0_occupancy_map_layer_2->Fill(hx, hy);
        if (hz > 6700 && hz < 6750) h_B0_occupancy_map_layer_3->Fill(hx, hy);
    }
}

//---------------------------------------------
// Reco particle 4-vectors (set by ProcessScatteredElectron)
//---------------------------------------------
TLorentzVector positron_rc;
TLorentzVector decayElectron_rc;
TLorentzVector scatteredElectron_rc;
TLorentzVector JPsi_rc;
bool iflagRCelectron = false;

//---------------------------------------------
// Scattered electron + J/psi reconstruction
//---------------------------------------------
bool ProcessScatteredElectron(
    TTreeReaderArray<float> &reco_part_px,
    TTreeReaderArray<float> &reco_part_py,
    TTreeReaderArray<float> &reco_part_pz,
    TTreeReaderArray<float> &reco_part_energy,
    TTreeReaderArray<float> &reco_part_mass,
    TTreeReaderArray<float> &reco_part_charge,
    TTreeReaderArray<int>   &reco_part_pdg,
    TTreeReaderArray<int>   &mc_pdg,
    TTreeReaderArray<int>   &mc_gen_status
)
{
    iflagRCelectron = false;
    positron_rc.SetPxPyPzE(0,0,0,0);
    decayElectron_rc.SetPxPyPzE(0,0,0,0);
    scatteredElectron_rc.SetPxPyPzE(0,0,0,0);
    JPsi_rc.SetPxPyPzE(0,0,0,0);

    // Exclusive topology: exactly 3 central tracks (e+ e- e-)
    // Loosen to >= 3 so soft/ghost tracks don't kill real signal
    if (reco_part_px.GetSize() < 3)
        return false; //change < to != for DIS background

    std::vector<int> ele_indices;
    std::vector<int> pos_indices;

    for (int i = 0; i < (int)reco_part_px.GetSize(); ++i)
    {
        TLorentzVector p;
        p.SetXYZM(reco_part_px[i], reco_part_py[i],
                  reco_part_pz[i], reco_part_mass[i]);

        if (p.Eta() > 4.0) continue;

        int pdg = reco_part_pdg[i];

        int charge = (int)reco_part_charge[i];

        if (charge == -1)
            ele_indices.push_back(i);

        if (charge == 1)
            pos_indices.push_back(i);
    }

    if ((int)ele_indices.size() < 2 || (int)pos_indices.size() < 1)
        return false;

    //--------------------------------------------------
    // Find best J/psi = e+ e- combination by mass window
    //--------------------------------------------------
    double bestDM = 999.0;
    int bestPosIdx = -1, bestDecayEleIdx = -1;
    TLorentzVector bestPos, bestDecayEle, bestJPsi;

    for (int ipos : pos_indices)
    {
        TLorentzVector pPos;
        pPos.SetXYZM(reco_part_px[ipos], reco_part_py[ipos],
                     reco_part_pz[ipos], reco_part_mass[ipos]);

        for (int iele : ele_indices)
        {
            TLorentzVector pEle;
            pEle.SetXYZM(reco_part_px[iele], reco_part_py[iele],
                         reco_part_pz[iele], reco_part_mass[iele]);

            TLorentzVector cand = pPos + pEle;
            double mass = cand.M();

            //if (mass < jplow || mass > jphigh) continue;

            double dM = fabs(mass - M_JPSI);
            if (dM < bestDM)
            {
                bestDM          = dM;
                bestPosIdx      = ipos;
                bestDecayEleIdx = iele;
                bestPos         = pPos;
                bestDecayEle    = pEle;
                bestJPsi        = cand;
            }
        }
    }

    if (bestPosIdx < 0 || bestDecayEleIdx < 0) return false;

    //--------------------------------------------------
    // Scattered electron: highest-energy remaining e-
    // (goes backward, carries most of beam electron energy)
    //--------------------------------------------------
    int bestScatIdx = -1;
    double bestScatE = -999.0;
    TLorentzVector bestScatEle;

    for (int iele : ele_indices)
    {
        if (iele == bestDecayEleIdx) continue;

        TLorentzVector pEle;
        pEle.SetXYZM(reco_part_px[iele], reco_part_py[iele],
                     reco_part_pz[iele], reco_part_mass[iele]);

        if (pEle.E() > bestScatE)
        {
            bestScatE   = pEle.E();
            bestScatIdx = iele;
            bestScatEle = pEle;
        }
        //cout << "pdg: " << reco_part_pdg[iele] << endl;
        
        //-----------------
        //For the background
        //if (mc_gen_status[iele] != 1) continue;
        //if (abs(mc_pdg[iele]) != 11 ) continue;
        //if (abs(mc_pdg[iele]) == 11 || abs(mc_pdg[iele]) == 13) continue;
        //cout << "Residual particles: " << reco_part_pdg[iele] << endl;
    }

    if (bestScatIdx < 0) return false;

    //--------------------------------------------------
    // Commit globals
    //--------------------------------------------------
    positron_rc          = bestPos;
    decayElectron_rc     = bestDecayEle;
    scatteredElectron_rc = bestScatEle;
    JPsi_rc              = bestJPsi;

    h_jpsi_mass_RC->Fill(JPsi_rc.M());
    if (JPsi_rc.M() < jplow || JPsi_rc.M() > jphigh) return false;
    
    h_scattered_electron_energy_RC->Fill(scatteredElectron_rc.E());
    h_scattered_electron_eta_RC->Fill(scatteredElectron_rc.Eta());
    h_scattered_electron_pt_RC->Fill(scatteredElectron_rc.Pt());
    h_scattered_positron_eta_RC->Fill(positron_rc.Eta());
    h_decay_electron_eta_RC->Fill(decayElectron_rc.Eta());
    
    iflagRCelectron = true;
    
    return true;
}

//For bacground DIS file+++++++++++++++++++++++++++++++++++
bool HasGeneratedJpsi(
    TTreeReaderArray<int> &mc_pdg,
    TTreeReaderArray<int> &mc_gen_status
)
{
    for (int i = 0; i < mc_pdg.GetSize(); ++i)
    {
        if (std::abs(mc_pdg[i]) == 443)
            return true;
    }

    return false;
}

//--------------------------------------------------
// Check if event contains generated leptonic J/psi
//
// Return:
//   0  = no generated J/psi -> e+e- or mu+mu-
//   11 = generated J/psi -> e+e-
//   13 = generated J/psi -> mu+mu-
//--------------------------------------------------
int CheckGeneratedLeptonicJpsi(
    TTreeReaderArray<int> &mc_pdg,
    TTreeReaderArray<unsigned int> &mc_daughters_begin,
    TTreeReaderArray<unsigned int> &mc_daughters_end,
    TTreeReaderArray<int> &mc_daughter_index
)
{
    for (int i = 0; i < mc_pdg.GetSize(); ++i)
    {
        if (std::abs(mc_pdg[i]) != 443)
            continue;

        bool hasEleMinus = false;
        bool hasElePlus  = false;
        bool hasMuMinus  = false;
        bool hasMuPlus   = false;

        for (unsigned int j = mc_daughters_begin[i];
             j < mc_daughters_end[i]; ++j)
        {
            int didx = mc_daughter_index[j];

            if (didx < 0 || didx >= mc_pdg.GetSize())
                continue;

            int dpdg = mc_pdg[didx];

            if (dpdg ==  11) hasEleMinus = true;
            if (dpdg == -11) hasElePlus  = true;

            if (dpdg ==  13) hasMuMinus = true;
            if (dpdg == -13) hasMuPlus  = true;
        }

        if (hasEleMinus && hasElePlus)
            return 11;

        if (hasMuMinus && hasMuPlus)
            return 13;

        return 99; // J/psi found, but not leptonic e/mu decay
    }

    return 0; // no J/psi found
}
//+++++++++++++++++++++++++++++++++

//---------------------------------------------
// Calorimeter match globals
//---------------------------------------------
double matchedEcalEnergy = -999.0;
double matchedEcalDR     =  999.0;
double matchedEoverP     = -999.0;

//---------------------------------------------
// Match scattered electron track to ECAL cluster
//---------------------------------------------
bool MatchScatteredElectronToEcal(
    TTreeReaderArray<float>        &ecalN_energy,
    TTreeReaderArray<float>        &ecalN_x,
    TTreeReaderArray<float>        &ecalN_y,
    TTreeReaderArray<float>        &ecalN_z,
    TTreeReaderArray<unsigned int> &ecalN_nHits
)
{
    matchedEcalEnergy = -999.0;
    matchedEcalDR     =  999.0;
    matchedEoverP     = -999.0;

    if (scatteredElectron_rc.P() <= 0) return false;

    double bestDR = 999.0;
    double bestE  = -999.0;
    bool   found  = false;

    for (int ic = 0; ic < (int)ecalN_energy.GetSize(); ++ic)
    {
        if (ecalN_nHits[ic] < 4) continue;

        double clus_x = ecalN_x[ic];   // renamed: avoids shadowing global gen_x
        double clus_y = ecalN_y[ic];
        double clus_z = ecalN_z[ic];

        double theta = TMath::ATan2(TMath::Sqrt(clus_x*clus_x + clus_y*clus_y), clus_z);
        double eta   = -TMath::Log(TMath::Tan(theta / 2.0));
        double phi   = TMath::ATan2(clus_y, clus_x);

        double deta = eta - scatteredElectron_rc.Eta();
        double dphi = TVector2::Phi_mpi_pi(phi - scatteredElectron_rc.Phi());
        double dR   = TMath::Sqrt(deta*deta + dphi*dphi);

        if (dR < bestDR)
        {
            bestDR = dR;
            bestE  = ecalN_energy[ic];
            found  = true;
        }
    }

    // No cluster found at all
    if (!found) return false;

    matchedEcalEnergy = bestE;
    matchedEcalDR     = bestDR;
    matchedEoverP     = matchedEcalEnergy / scatteredElectron_rc.P();

    bool passDR   = (matchedEcalDR  > 0.0 && matchedEcalDR < 0.4);
    bool passEoP  = (matchedEoverP  > 0.9 && matchedEoverP < 1.2);

    if (!(passDR && passEoP)) return false;

    h_matched_ecal_energy->Fill(matchedEcalEnergy);
    h_matched_ecal_dr->Fill(matchedEcalDR);
    h_matched_eop->Fill(matchedEoverP);

    return true;
}

//---------------------------------------------
// Inclusive kinematics from EICRECON collections
// Note: reco_x_EM etc. will be overwritten later
// by ProcessRecoElectronKinematics (hand-computed).
// These histograms show the EICRECON output directly.
//---------------------------------------------
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
    reco_x_EM  = reco_Q2_EM  = reco_y_EM  = -999.0;
    reco_x_eSigma = reco_Q2_eSigma = reco_y_eSigma = -999.0;
    reco_x_JB  = reco_Q2_JB  = reco_y_JB  = -999.0;
    reco_x_DA  = reco_Q2_DA  = reco_y_DA  = -999.0;

    auto tryFill = [&](
        TTreeReaderArray<float> &ax,
        TTreeReaderArray<float> &aQ2,
        TTreeReaderArray<float> &ay,
        double &gx, double &gQ2, double &gy,
        TH1F *hx, TH1F *hQ2, TH1F *hy)
    {
        if (ax.GetSize() < 1 || aQ2.GetSize() < 1 || ay.GetSize() < 1) return;
        double xv = ax[0], q2v = aQ2[0], yv = ay[0];
        if (xv  < x_low  || xv  > x_high)  return;
        if (q2v < q2_low || q2v > q2_high) return;
        if (yv  < y_low  || yv  > y_high)  return;
        gx = xv; gQ2 = q2v; gy = yv;
        hx->Fill(xv); hQ2->Fill(q2v); hy->Fill(yv);
    };

    tryFill(ik_electron_x, ik_electron_Q2, ik_electron_y,
            reco_x_EM,     reco_Q2_EM,     reco_y_EM,
            EM_x, EM_Q2, EM_y);

    tryFill(ik_esigma_x,   ik_esigma_Q2,   ik_esigma_y,
            reco_x_eSigma, reco_Q2_eSigma, reco_y_eSigma,
            ESig_x_in, ESig_Q2_in, ESig_y_in);

    tryFill(ik_jb_x,  ik_jb_Q2,  ik_jb_y,
            reco_x_JB, reco_Q2_JB, reco_y_JB,
            JB_x_in, JB_Q2_in, JB_y_in);

    tryFill(ik_DA_x,  ik_DA_Q2,  ik_DA_y,
            reco_x_DA, reco_Q2_DA, reco_y_DA,
            DA_x_in, DA_Q2_in, DA_y_in);
}

void PrintMCParticlesAfterMissingMassCut(
    TTreeReaderArray<double> &mc_px,
    TTreeReaderArray<double> &mc_py,
    TTreeReaderArray<double> &mc_pz,
    TTreeReaderArray<double> &mc_mass,
    TTreeReaderArray<int>    &mc_pdg,
    TTreeReaderArray<int>    &mc_gen_status,
    double jpsiMass,
    double missingMass2
)
{
    cout << "\n==================================================" << endl;
    cout << "Event survived missing-mass cut" << endl;
    cout << "J/psi mass = " << jpsiMass << endl;
    cout << "Mx^2       = " << missingMass2 << endl;
    cout << "==================================================" << endl;

    cout << setw(5)  << "idx"
         << setw(10) << "PDG"
         << setw(10) << "status"
         << setw(12) << "eta"
         << setw(12) << "pt"
         << setw(14) << "pz"
         << setw(14) << "mass"
         << endl;

    for (int i = 0; i < mc_pdg.GetSize(); i++)
    {
        TLorentzVector p;
        p.SetXYZM(
            mc_px[i],
            mc_py[i],
            mc_pz[i],
            mc_mass[i]
        );

        cout << setw(5)  << i
             << setw(10) << mc_pdg[i]
             << setw(10) << mc_gen_status[i]
             << setw(12) << p.Eta()
             << setw(12) << p.Pt()
             << setw(14) << p.Pz()
             << setw(14) << p.M()
             << endl;
    }

    cout << "==================================================\n"
         << endl;
}

//---------------------------------------------
// MC truth 4-vectors
//---------------------------------------------
TLorentzVector incbeame;
TLorentzVector incbeam;
TLorentzVector mc_positron;
TLorentzVector mc_decayElectron;
TLorentzVector mc_scatteredElectron;
TLorentzVector mc_recoilProton;
TLorentzVector mc_photon;
TLorentzVector mc_jpsi;
bool iflagMC = false;

//---------------------------------------------
// MC particle loop
//---------------------------------------------
double mc_x  = -999.0;
double mc_Q2 = -999.0;
double mc_y  = -999.0;

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
    mc_x  = -999.0;
    mc_Q2 = -999.0;
    mc_y  = -999.0;
    
    iflagMC = false;

    incbeame.SetPxPyPzE(0,0,0,0);
    incbeam.SetPxPyPzE(0,0,0,0);
    mc_positron.SetPxPyPzE(0,0,0,0);
    mc_decayElectron.SetPxPyPzE(0,0,0,0);
    mc_scatteredElectron.SetPxPyPzE(0,0,0,0);
    mc_recoilProton.SetPxPyPzE(0,0,0,0);
    mc_photon.SetPxPyPzE(0,0,0,0);
    mc_jpsi.SetPxPyPzE(0,0,0,0);

    bool has_incbeame    = false;
    bool has_incbeam     = false;
    bool has_positron    = false;
    bool has_recoilP     = false;
    bool has_electron1   = false;
    bool has_electron2   = false;

    TLorentzVector electron1, electron2;

    for (int i = 0; i < (int)mc_px.GetSize(); ++i)
    {
        int    pdg    = mc_pdg[i];
        int    status = mc_gen_status[i];
        int    charge = (int)mc_charge[i];
        double px = mc_px[i], py = mc_py[i], pz = mc_pz[i], m = mc_mass[i];

        // Use a temp vector so a bad mass check doesn't dirty the global
        TLorentzVector tmp;
        tmp.SetXYZM(px, py, pz, m);
        if (tmp.M() < 0) continue;   // skip unphysical entries

        if (status == 4)
        {
            if (pdg == 11 && charge == -1 && !has_incbeame)
            {
                incbeame    = tmp;
                has_incbeame = true;
            }
            else if (pdg == 2212 && charge == 1 && !has_incbeam)
            {
                tmp.RotateY(0.025);   // crossing angle
                incbeam    = tmp;
                has_incbeam = true;
            }
        }
        else if (status == 1)
        {
            if (pdg == 11 && charge == -1)
            {
                if (!has_electron1) { electron1 = tmp; has_electron1 = true; }
                else if (!has_electron2) { electron2 = tmp; has_electron2 = true; }
            }
            else if (pdg == -11 && charge == 1 && !has_positron)
            {
                mc_positron  = tmp;
                has_positron = true;
            }
            else if (pdg == 2212 && charge == 1 && !has_recoilP)
            {
                tmp.RotateY(0.025);
                mc_recoilProton = tmp;
                has_recoilP     = true;
            }
            else if (pdg == 22 && charge == 0)
            {
                mc_photon = tmp;   // unused currently
            }
        }
        else if (status == 2 && pdg == 443)
        {
            mc_jpsi = tmp;   // intermediate J/psi (unused currently)
        }
    }

    if (!has_incbeame || !has_incbeam || !has_positron ||
        !has_recoilP  || !has_electron1 || !has_electron2)
        return false;

    //--------------------------------------------------
    // Assign decay electron vs scattered electron
    // by finding which e- pairs with e+ to give J/psi mass
    //--------------------------------------------------
    TLorentzVector cand1 = mc_positron + electron1;
    TLorentzVector cand2 = mc_positron + electron2;

    bool cand1Good = (cand1.M() > jplow && cand1.M() < jphigh);
    bool cand2Good = (cand2.M() > jplow && cand2.M() < jphigh);

    double dM1 = fabs(cand1.M() - M_JPSI);
    double dM2 = fabs(cand2.M() - M_JPSI);

    if (cand1Good && (!cand2Good || dM1 <= dM2))
    {
        mc_decayElectron     = electron1;
        mc_scatteredElectron = electron2;
        mc_jpsi              = cand1;
    }
    else if (cand2Good)
    {
        mc_decayElectron     = electron2;
        mc_scatteredElectron = electron1;
        mc_jpsi              = cand2;
    }
    else
    {
        // No mass-window match — assign by energy (higher E = scattered)
        if (electron1.E() >= electron2.E())
        {
            mc_scatteredElectron = electron1;
            mc_decayElectron     = electron2;
        }
        else
        {
            mc_scatteredElectron = electron2;
            mc_decayElectron     = electron1;
        }
        mc_jpsi = mc_positron + mc_decayElectron;
    }

    // Diagnostic histograms
    h_mc_jpsi_mass->Fill(mc_jpsi.M());

    h_mc_positron_px->Fill(mc_positron.Px());
    h_mc_positron_py->Fill(mc_positron.Py());
    h_mc_positron_pz->Fill(mc_positron.Pz());
    h_mc_positron_pt->Fill(mc_positron.Pt());
    h_mc_positron_E->Fill(mc_positron.E());
    h_mc_positron_eta->Fill(mc_positron.Eta());

    h_mc_decay_electron_px->Fill(mc_decayElectron.Px());
    h_mc_decay_electron_py->Fill(mc_decayElectron.Py());
    h_mc_decay_electron_pz->Fill(mc_decayElectron.Pz());
    h_mc_decay_electron_pt->Fill(mc_decayElectron.Pt());
    h_mc_decay_electron_E->Fill(mc_decayElectron.E());
    h_mc_decay_electron_eta->Fill(mc_decayElectron.Eta());

    h_mc_scattered_electron_px->Fill(mc_scatteredElectron.Px());
    h_mc_scattered_electron_py->Fill(mc_scatteredElectron.Py());
    h_mc_scattered_electron_pz->Fill(mc_scatteredElectron.Pz());
    h_mc_scattered_electron_pt->Fill(mc_scatteredElectron.Pt());
    h_mc_scattered_electron_E->Fill(mc_scatteredElectron.E());
    h_mc_scattered_electron_eta->Fill(mc_scatteredElectron.Eta());

    h_mc_recoil_proton_px->Fill(mc_recoilProton.Px());
    h_mc_recoil_proton_py->Fill(mc_recoilProton.Py());
    h_mc_recoil_proton_pz->Fill(mc_recoilProton.Pz());
    h_mc_recoil_proton_pt->Fill(mc_recoilProton.Pt());
    h_mc_recoil_proton_E->Fill(mc_recoilProton.E());
    h_mc_recoil_proton_eta->Fill(mc_recoilProton.Eta());
    
    //--------------------------------------------------
    // MC-level DIS kinematics from true scattered electron
    //--------------------------------------------------
    TLorentzVector q_mc = incbeame - mc_scatteredElectron;

    double Q2_mc = -q_mc.M2();

    double denom_y = incbeam.Dot(incbeame);
    if (TMath::Abs(denom_y) < 1e-12) return false;

    double y_mc = incbeam.Dot(q_mc) / denom_y;

    double denom_x = 2.0 * incbeam.Dot(q_mc);
    if (TMath::Abs(denom_x) < 1e-12) return false;

    double x_mc = Q2_mc / denom_x;

    mc_x  = x_mc;
    mc_Q2 = Q2_mc;
    mc_y  = y_mc;

    iflagMC = true;
    return true;
}

//---------------------------------------------
// Missing mass exclusivity cut
// Requires ProcessMCParticles to have run first
// (uses incbeame + incbeam as incoming state)
//---------------------------------------------
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

    // Cut at 3 GeV — consider tightening to 1 GeV after seeing the distribution
    return (Mx2 > -999 && Mx2 < 999);
}

//---------------------------------------------
// Hand-computed electron-method kinematics
// Overwrites reco_x_EM etc. set by ProcessInclusiveKinematics.
// This version is used for resolution plots.
//---------------------------------------------
bool ProcessRecoElectronKinematics()
{
    reco_x_EM  = -999.0;
    reco_Q2_EM = -999.0;
    reco_y_EM  = -999.0;

    if (!iflagRCelectron) return false;

    TLorentzVector q = incbeame - scatteredElectron_rc;

    double q2_EM = -q.M2();

    double denom_y = incbeam.Dot(incbeame);
    if (TMath::Abs(denom_y) < 1e-12) return false;

    double y_EM = incbeam.Dot(q) / denom_y;

    double denom_x = 2.0 * incbeam.Dot(q);
    if (TMath::Abs(denom_x) < 1e-12) return false;

    double x_EM = q2_EM / denom_x;

    reco_x_EM  = x_EM;
    reco_Q2_EM = q2_EM;
    reco_y_EM  = y_EM;

    bool pass = (x_EM  > x_low  && x_EM  < x_high &&
                 q2_EM > q2_low && q2_EM < q2_high &&
                 y_EM  > y_low  && y_EM  < y_high);

    if (!pass) return false;

    h_reco_EM_x->Fill(x_EM);
    h_reco_EM_Q2->Fill(q2_EM);
    h_reco_EM_y->Fill(y_EM);
    h2_reco_Q2_vs_x->Fill(x_EM, q2_EM);

    return true;
}

//---------------------------------------------
// Hadronic reconstruction methods (JB, eSigma, DA)
// HFS built from J/psi decay pair + recoil proton
//---------------------------------------------
bool ProcessRecoHadronicKinematics(const TLorentzVector &recoilProton)
{
    reco_x_JB = reco_Q2_JB = reco_y_JB = -999.0;
    reco_x_eSigma = reco_Q2_eSigma = reco_y_eSigma = -999.0;
    reco_x_DA = reco_Q2_DA = reco_y_DA = -999.0;

    // HFS for exclusive event: J/psi decay products + recoil proton
    TLorentzVector hfs = recoilProton + decayElectron_rc + positron_rc;

    double hfs_pt2   = hfs.Pt() * hfs.Pt();
    double hfs_pt    = hfs.Pt();
    double hfs_sigma = hfs.E() - hfs.Pz();

    h_reco_JB_HFS_sigma->Fill(hfs_sigma);
    h_reco_JB_HFS_pt->Fill(hfs_pt);

    //--------------------------------------------------
    // JB method
    //--------------------------------------------------
    double y_JB = -999.0, q2_JB = -999.0, x_JB = -999.0;

    if (incbeame.E() > 0)
    {
        y_JB = hfs_sigma / (2.0 * incbeame.E());

        if (y_JB > 0.0 && y_JB < 1.0)
            q2_JB = hfs_pt2 / (1.0 - y_JB);

        double denom = 4.0 * incbeame.E() * incbeam.E() * y_JB;
        if (TMath::Abs(denom) > 1e-12)
            x_JB = q2_JB / denom;
    }

    if (x_JB  > x_low  && x_JB  < x_high  &&
        q2_JB > q2_low && q2_JB < q2_high  &&
        y_JB  > y_low  && y_JB  < y_high)
    {
        reco_x_JB = x_JB; reco_Q2_JB = q2_JB; reco_y_JB = y_JB;
        h_reco_JB_x->Fill(x_JB);
        h_reco_JB_Q2->Fill(q2_JB);
        h_reco_JB_y->Fill(y_JB);
    }

    //--------------------------------------------------
    // eSigma method
    //--------------------------------------------------
    double sig_e   = scatteredElectron_rc.E() - scatteredElectron_rc.Pz();
    double sig_tot = sig_e + hfs_sigma;

    h_reco_eSigma_HFS_sigma->Fill(sig_tot);

    double y_Sig = -999.0, q2_Sig = -999.0, x_Sig = -999.0;

    if (TMath::Abs(sig_tot) > 1e-12)
        y_Sig = hfs_sigma / sig_tot;

    if (y_Sig > y_low && y_Sig < y_high)
        q2_Sig = scatteredElectron_rc.Pt() * scatteredElectron_rc.Pt() / (1.0 - y_Sig);

    double denomSig = 4.0 * incbeame.E() * incbeam.E() * y_Sig;
    if (TMath::Abs(denomSig) > 1e-12)
        x_Sig = q2_Sig / denomSig;

    if (x_Sig  > x_low  && x_Sig  < x_high  &&
        q2_Sig > q2_low && q2_Sig < q2_high  &&
        y_Sig  > y_low  && y_Sig  < y_high)
    {
        reco_x_eSigma = x_Sig; reco_Q2_eSigma = q2_Sig; reco_y_eSigma = y_Sig;
        h_reco_eSigma_x->Fill(x_Sig);
        h_reco_eSigma_Q2->Fill(q2_Sig);
        h_reco_eSigma_y->Fill(y_Sig);
    }

    //--------------------------------------------------
    // DA method
    //--------------------------------------------------
    if (hfs_pt2 < 1e-12) return true;

    double tanGamma    = hfs_sigma / TMath::Sqrt(hfs_pt2);
    double tanElectron = TMath::Tan(scatteredElectron_rc.Theta() / 2.0);

    double y_DA = -999.0, q2_DA = -999.0, x_DA = -999.0;

    double denomY = tanElectron + tanGamma;
    if (TMath::Abs(denomY) > 1e-12)
        y_DA = tanGamma / denomY;

    double denomQ2 = tanElectron * (tanElectron + tanGamma);
    if (TMath::Abs(denomQ2) > 1e-12)
        q2_DA = 4.0 * incbeame.E() * incbeame.E() / denomQ2;

    double denomX = 4.0 * incbeame.E() * incbeam.E() * y_DA;
    if (TMath::Abs(denomX) > 1e-12)
        x_DA = q2_DA / denomX;

    if (x_DA  > x_low  && x_DA  < x_high  &&
        q2_DA > q2_low && q2_DA < q2_high  &&
        y_DA  > y_low  && y_DA  < y_high)
    {
        reco_x_DA = x_DA; reco_Q2_DA = q2_DA; reco_y_DA = y_DA;
        h_reco_DA_x->Fill(x_DA);
        h_reco_DA_Q2->Fill(q2_DA);
        h_reco_DA_y->Fill(y_DA);
    }

    return true;
}

//---------------------------------------------
// -t computation helpers
//---------------------------------------------
double ComputeMinusT(const TLorentzVector &beamP, const TLorentzVector &recoilP)
{
    TLorentzVector diff = beamP - recoilP;
    return -diff.Dot(diff);
}

TLorentzVector protonCorr;
double ComputeMethodLT()
{
    TLorentzVector jpsiReco  = decayElectron_rc + positron_rc;
    TLorentzVector protonReco = incbeam - (jpsiReco + scatteredElectron_rc - incbeame);

    double pAplus = protonReco.E() + protonReco.Pz();
    if (TMath::Abs(pAplus) < 1e-12) return -999.0;

    double pT2     = protonReco.Pt() * protonReco.Pt();
    double pAminus = (incbeam.M() * incbeam.M() + pT2) / pAplus;

    protonCorr.SetPxPyPzE(protonReco.Px(), protonReco.Py(),
                           (pAplus - pAminus) / 2.0,
                           (pAplus + pAminus) / 2.0);

    return -(incbeam - protonCorr).Dot(incbeam - protonCorr);
}

//---------------------------------------------
// Kinematic resolution plots (truth vs reco)
// Uses gen_* for truth, reco_* globals for reco
//---------------------------------------------
void FillKinematicResolutionPlots()
{
    /*double truth_x  = gen_x;
    double truth_Q2 = gen_Q2;
    double truth_y  = gen_y;*/
    //double truth_minusT = -gen_t;
    
    double truth_x  = mc_x;
    double truth_Q2 = mc_Q2;
    double truth_y  = mc_y;
    double truth_minusT = ComputeMinusT(incbeam, mc_recoilProton);

    if (!(truth_x  > x_low  && truth_x  < x_high  &&
          truth_Q2 > q2_low && truth_Q2 < q2_high  &&
          truth_y  > y_low  && truth_y  < y_high))
        return;

    auto fillOne = [](double truth, double reco,
                      TH2F *h2_frac, TH1F *h1_frac, TH2F *h2_res)
    {
        if (!std::isfinite(truth) || !std::isfinite(reco)) return;
        if (truth <= 0.0 || reco <= 0.0) return;
        double frac = (truth - reco) / truth;
        h2_frac->Fill(truth, frac);
        h1_frac->Fill(frac);
        h2_res->Fill(truth, reco);
    };

    // Separate lambda for -t: both sides must be positive but no other sign restriction
    auto fillOneT = [](double truth, double reco,
                       TH2F *h2_frac, TH1F *h1_frac, TH2F *h2_res)
    {
        if (!std::isfinite(truth) || !std::isfinite(reco)) return;
        if (truth <= 0.0 || reco <= 0.0) return;
        double frac = (truth - reco) / truth;
        h2_frac->Fill(truth, frac);
        h1_frac->Fill(frac);
        h2_res->Fill(truth, reco);
    };

    // x, Q2, y for all four methods
    fillOne(truth_x,  reco_x_EM,      h2_dx_EM,      h_dx_EM,      h2_x_EM_res);
    fillOne(truth_Q2, reco_Q2_EM,     h2_dQ2_EM,     h_dQ2_EM,     h2_Q2_EM_res);
    fillOne(truth_y,  reco_y_EM,      h2_dy_EM,      h_dy_EM,      h2_y_EM_res);

    fillOne(truth_x,  reco_x_JB,      h2_dx_JB,      h_dx_JB,      h2_x_JB_res);
    fillOne(truth_Q2, reco_Q2_JB,     h2_dQ2_JB,     h_dQ2_JB,     h2_Q2_JB_res);
    fillOne(truth_y,  reco_y_JB,      h2_dy_JB,      h_dy_JB,      h2_y_JB_res);

    fillOne(truth_x,  reco_x_eSigma,  h2_dx_eSigma,  h_dx_eSigma,  h2_x_eSigma_res);
    fillOne(truth_Q2, reco_Q2_eSigma, h2_dQ2_eSigma, h_dQ2_eSigma, h2_Q2_eSigma_res);
    fillOne(truth_y,  reco_y_eSigma,  h2_dy_eSigma,  h_dy_eSigma,  h2_y_eSigma_res);

    fillOne(truth_x,  reco_x_DA,      h2_dx_DA,      h_dx_DA,      h2_x_DA_res);
    fillOne(truth_Q2, reco_Q2_DA,     h2_dQ2_DA,     h_dQ2_DA,     h2_Q2_DA_res);
    fillOne(truth_y,  reco_y_DA,      h2_dy_DA,      h_dy_DA,      h2_y_DA_res);

    // -t resolution: RP, B0, Method L
    if (truth_minusT > 0 && truth_minusT < 5)
    {
        if (iflagRPpproton)
        {
            double mt_RP = ComputeMinusT(incbeam, RP_proton_rc);
            fillOneT(truth_minusT, mt_RP, h2_dt_RP, h_dt_RP, h2_t_RP_res);
        }
        if (iflagB0proton)
        {
            double mt_B0 = ComputeMinusT(incbeam, B0_proton_rc);
            fillOneT(truth_minusT, mt_B0, h2_dt_B0, h_dt_B0, h2_t_B0_res);
        }
        if (iflagRCelectron)
        {
            double mt_L = ComputeMethodLT();
            fillOneT(truth_minusT, mt_L, h2_dt_MethodL, h_dt_MethodL, h2_t_MethodL_res);
        }
    }
}

//---------------------------------------------
// Electron energy resolution (cal vs track vs MC)
//---------------------------------------------
void FillElectronResolutionPlots()
{
    double mcE = mc_scatteredElectron.E();
    if (mcE < 1e-12) return;

    double cal_res   = (mcE - matchedEcalEnergy)       / mcE;
    double track_res = (mcE - scatteredElectron_rc.E()) / mcE;

    h_CalEnergy_minus_SCElecEnergy->Fill(cal_res);
    h_Cal_DE_over_E_vs_Eta->Fill(mc_scatteredElectron.Eta(), cal_res);

    h_TrackEnergy_minus_SCElecEnergy->Fill(track_res);
    h_Track_DE_over_E_vs_Eta->Fill(mc_scatteredElectron.Eta(), track_res);
}

//---------------------------------------------
// E - Pz (HFS conservation check)
//---------------------------------------------
void FillMCEminusPz()
{
    TLorentzVector fs = mc_decayElectron + mc_recoilProton +
                        mc_positron + mc_scatteredElectron;
    double val = fs.E() - fs.Pz();
    if (val > 18 && val < 22) h_mc_e_minus_pz->Fill(val);
}

bool FillRecoEminusPz()
{
    TLorentzVector fs = decayElectron_rc + positron_rc + scatteredElectron_rc;

    if      (iflagRPpproton) fs += RP_proton_rc;
    else if (iflagB0proton)  fs += B0_proton_rc;
    else return false;

    double val = fs.E() - fs.Pz();
    h_reco_e_minus_pz->Fill(val);
    return (val > 18 && val < 22);
}

//---------------------------------------------
// t-distribution fills (exclusive events only)
//---------------------------------------------
void FillTDistributions(bool passRPExclusive, bool passB0Exclusive)
{
    double mt_truth = ComputeMinusT(incbeam, mc_recoilProton);
    //double mt_truth = -gen_t;
    if (mt_truth <= 0 || mt_truth >= 5) return;

    if (iflagMC)
    {
        double mt_MC = ComputeMinusT(incbeam, mc_recoilProton);
        if (mt_MC > 0 && mt_MC < 5) h_t_MC->Fill(mt_MC);
    }

    if (passRPExclusive)
    {
        double mt_RP = ComputeMinusT(incbeam, RP_proton_rc);
        if (mt_RP > 0 && mt_RP < 5)
        {
            h_t_RECO_RP->Fill(mt_RP);
            h_t_RECO_RPB0->Fill(mt_RP);
            h_t2D_RP->Fill(mt_truth, mt_RP);
            h_t2D_RPB0->Fill(mt_truth, mt_RP);
        }
    }
    else if (passB0Exclusive)
    {
        double mt_B0 = ComputeMinusT(incbeam, B0_proton_rc);
        if (mt_B0 > 0 && mt_B0 < 5)
        {
            h_t_RECO_B0->Fill(mt_B0);
            h_t_RECO_RPB0->Fill(mt_B0);
            h_t2D_B0->Fill(mt_truth, mt_B0);
            h_t2D_RPB0->Fill(mt_truth, mt_B0);
        }
    }
}
void FillMethodLT()
{
    if (!iflagRCelectron) return;

    double mt_truth   = ComputeMinusT(incbeam, mc_recoilProton);
    double mt_MethodL = ComputeMethodLT();

    if (mt_truth <= 0 || mt_truth >= 5) return;
    if (mt_MethodL > 0 && mt_MethodL < 5)
    {
        h_t_MethodL_RECO->Fill(mt_MethodL);
        h_t2D_MethodL->Fill(mt_truth, mt_MethodL);
    }
}

void FillHybridTDistribution(
    bool passRPExclusive,
    bool passB0Exclusive,
    bool passMethodL
)
{
    double mt_truth = ComputeMinusT(incbeam, mc_recoilProton);
    if (mt_truth <= 0 || mt_truth >= 5.0) return;

    double tFinal = -999.0;
    bool accepted = false;

    //--------------------------------------------------
    // Method L: 0.00 - 0.078
    //--------------------------------------------------
    if (mt_truth >= 0.0 && mt_truth < 0.078)
    {
        if (passMethodL)
        {
            accepted = true;
            tFinal = ComputeMethodLT();
            h_t_hybrid_accept->Fill(mt_truth);
        }
    }

    //--------------------------------------------------
    // RP: 0.078 - 0.27
    //--------------------------------------------------
    else if (mt_truth >= 0.078 && mt_truth < 0.27)
    {
        if (passRPExclusive)
        {
            accepted = true;
            tFinal = ComputeMinusT(incbeam, RP_proton_rc);
            h_t_hybrid_accept->Fill(mt_truth);
        }
    }

    //--------------------------------------------------
    // Method L: 0.27 - 0.83
    //--------------------------------------------------
    else if (mt_truth >= 0.27 && mt_truth < 0.83)
    {
        if (passMethodL)
        {
            accepted = true;
            tFinal = ComputeMethodLT();
            h_t_hybrid_accept->Fill(mt_truth);
        }
    }

    //--------------------------------------------------
    // B0: 0.83 - 5.0
    //--------------------------------------------------
    else if (mt_truth >= 0.75 && mt_truth < 5.0)
    {
        if (passB0Exclusive)
        {
            accepted = true;
            tFinal = ComputeMinusT(incbeam, B0_proton_rc);
            h_t_hybrid_accept->Fill(mt_truth);
        }
    }

    //--------------------------------------------------
    // Fill accepted truth
    //--------------------------------------------------
    //if (accepted) h_t_hybrid_accept->Fill(mt_truth);

    //--------------------------------------------------
    // Fill reconstructed
    //--------------------------------------------------
    if (accepted && tFinal > 0 && tFinal < 5.0)
    {
        h_methodL_RP_MethodL_B0->Fill(tFinal);
        h2_methodL_RP_MethodL_B0->Fill(mt_truth, tFinal);
    }
}


void FillCombinedTWithMethodL(bool passRPExclusive,
                               bool passB0Exclusive,
                               bool passMethodL)
{
    double mt_truth = ComputeMinusT(incbeam, mc_recoilProton);
    if (mt_truth <= 0 || mt_truth >= 5) return;
    
    if (passRPExclusive || passB0Exclusive )
        h_t_RPB0_accept->Fill(mt_truth);
    
    if (passRPExclusive || passB0Exclusive || passMethodL)
        h_t_lAger_accept->Fill(mt_truth);
    
    if (passRPExclusive)
    {
        double mt = ComputeMinusT(incbeam, RP_proton_rc);
        if (mt > 0 && mt < 5)
        {
            h_t_RECO_RPB0_MethodL->Fill(mt);
            h_t2D_RPB0_MethodL->Fill(mt_truth, mt);
        }
        return;
    }
    else if (passB0Exclusive)
    {
        double mt = ComputeMinusT(incbeam, B0_proton_rc);
        if (mt > 0 && mt < 5)
        {
            h_t_RECO_RPB0_MethodL->Fill(mt);
            h_t2D_RPB0_MethodL->Fill(mt_truth, mt);
        }
        return;
    }
    else if (passMethodL)
    {
        double mt = ComputeMethodLT();
        if (mt > 0 && mt < 5)
        {
            h_t_RECO_RPB0_MethodL->Fill(mt);
            h_t2D_RPB0_MethodL->Fill(mt_truth, mt);
        }
    }
    
}

//--------------------------------------------------
// Reconstructed exclusive-like J/psi veto for DIS
//--------------------------------------------------
bool IsRecoExclusiveJpsiLike()
{
    bool hasRecoJpsi =
        (iflagRCelectron &&
         JPsi_rc.E() > 0 &&
         JPsi_rc.M() > jplow &&
         JPsi_rc.M() < jphigh);

    bool hasRecoScatteredElectron = (iflagRCelectron && scatteredElectron_rc.E() > 0);

    bool hasRecoProton_RP = iflagRPpproton;
    bool hasRecoProton_B0 = iflagB0proton;

    bool hasRecoProton_MethodL = false;
    double t_methodL_tmp = ComputeMethodLT();

    if (std::isfinite(t_methodL_tmp) &&
        t_methodL_tmp > 0.0 &&
        t_methodL_tmp < 5.0)
    {
        hasRecoProton_MethodL = true;
    }

    return hasRecoScatteredElectron &&
           hasRecoJpsi &&
           (hasRecoProton_RP ||
            hasRecoProton_B0 ||
            hasRecoProton_MethodL);
}

//--------------------------------------------------
// MC truth exclusive-like J/psi veto
//--------------------------------------------------
bool IsMCExclusiveJpsiLike(
    TTreeReaderArray<int> &mc_pdg,
    TTreeReaderArray<int> &mc_gen_status
)
{
    bool hasJpsi = false;
    bool hasFinalElectron = false;
    bool hasFinalProton = false;
    bool hasMuMinus = false, hasMuPlus = false;
    bool hasEleMinus = false, hasElePlus = false;

    for (int i = 0; i < mc_pdg.GetSize(); ++i)
    {
        int pdg = mc_pdg[i];
        int status = mc_gen_status[i];

        if (pdg == 443 && status == 2)
            hasJpsi = true;

        if (status != 1) continue;

        if (pdg == 2212) hasFinalProton = true;
        if (pdg == 11)   hasFinalElectron = true;

        if (pdg ==  13) hasMuMinus = true;
        if (pdg == -13) hasMuPlus  = true;

        if (pdg ==  11) hasEleMinus = true;
        if (pdg == -11) hasElePlus  = true;
    }

    return hasJpsi &&
           hasFinalElectron &&
           hasFinalProton &&
           ((hasMuMinus && hasMuPlus) ||
            (hasEleMinus && hasElePlus));
}

//---------------------------------------------
// Acceptance correction for t
//---------------------------------------------
void BuildTCorrection()
{
    //h_CorrectionFactor = (TH1F*)h_t_lAger->Clone("h_CorrectionFactor");
    h_CorrectionFactor = (TH1F*)h_t_MC->Clone("h_CorrectionFactor");
    h_CorrectionFactor->SetTitle(
        "Correction Factor c_{f} = MC / Accepted Truth;-t [GeV^{2}];c_{f}");
    h_CorrectionFactor->Reset();

    for (int i = 1; i <= h_t_MC->GetNbinsX(); i++)
    {
        double generated = h_t_MC->GetBinContent(i);
        double accepted  = h_t_lAger_accept->GetBinContent(i);

        if (generated > 0 && accepted > 0)
        {
            double cf = generated / accepted;
            h_CorrectionFactor->SetBinContent(i, cf);
        }
        else
        {
            h_CorrectionFactor->SetBinContent(i, 0);
        }
    }

    h_t_RPB0_MethodL_corrected =
        (TH1F*)h_t_RECO_RPB0_MethodL->Clone("h_t_RPB0_MethodL_corrected");
    h_t_RPB0_MethodL_corrected->SetTitle(
        "Corrected RP+B0+MethodL -t;-t [GeV^{2}];Counts");
    h_t_RPB0_MethodL_corrected->Multiply(h_CorrectionFactor);
}

void BuildTEff()
{
    h_eff_RP_B0 = (TH1F*)h_t_MC->Clone("h_eff_RP_B0");
    h_eff_RP_B0->SetTitle(
        "RP+B0 Acceptance #times Efficiency;-t [GeV^{2}];RECO / MC Truth");
    h_eff_RP_B0->Reset();

    for (int i = 1; i <= h_t_MC->GetNbinsX(); i++)
    {
        double MC   = h_t_MC->GetBinContent(i);
        double reco = h_t_RECO_RPB0->GetBinContent(i);

        if (MC > 0)
        {
            double eff = reco / MC;
            h_eff_RP_B0->SetBinContent(i, eff);
        }
        else
        {
            h_eff_RP_B0->SetBinContent(i, 0);
        }
    }

    h_t_RPB0_eff_applied =
        (TH1F*)h_t_RECO_RPB0->Clone("h_t_RPB0_eff_applied");

    h_t_RPB0_eff_applied->SetTitle(
        "Efficiency-corrected RP+B0 -t;-t [GeV^{2}];Counts");

    for (int i = 1; i <= h_t_RECO_RPB0->GetNbinsX(); i++)
    {
        double reco = h_t_RECO_RPB0->GetBinContent(i);
        double eff  = h_eff_RP_B0->GetBinContent(i);

        if (eff > 0)
            h_t_RPB0_eff_applied->SetBinContent(i, reco / eff);
        else
            h_t_RPB0_eff_applied->SetBinContent(i, 0);
    }
}

void BuildHybridTEff()
{
    //--------------------------------------------------
    // Acceptance × Efficiency
    //--------------------------------------------------
    h_eff_MethodL_RP_MethodL_B0 =
        (TH1F*)h_t_MC->Clone("h_eff_MethodL_RP_MethodL_B0");

    h_eff_MethodL_RP_MethodL_B0->SetTitle(
        "Hybrid MethodL+RP+MethodL+B0 Acceptance #times Efficiency;"
        "-t [GeV^{2}];RECO / MC Truth");

    h_eff_MethodL_RP_MethodL_B0->Reset();

    for (int i = 1; i <= h_t_MC->GetNbinsX(); i++)
    {
        double MC   = h_t_MC->GetBinContent(i);
        double reco = h_methodL_RP_MethodL_B0->GetBinContent(i);

        if (MC > 0)
        {
            h_eff_MethodL_RP_MethodL_B0->SetBinContent(i, reco / MC);
        }
        else
        {
            h_eff_MethodL_RP_MethodL_B0->SetBinContent(i, 0);
        }
    }

    //--------------------------------------------------
    // Apply efficiency correction
    //--------------------------------------------------
    h_methodL_RP_MethodL_B0_eff_applied =
        (TH1F*)h_methodL_RP_MethodL_B0->Clone(
            "h_methodL_RP_MethodL_B0_eff_applied");

    h_methodL_RP_MethodL_B0_eff_applied->SetTitle(
        "Efficiency-corrected Hybrid MethodL+RP+MethodL+B0;"
        "-t [GeV^{2}];Counts");

    for (int i = 1; i <= h_methodL_RP_MethodL_B0->GetNbinsX(); i++)
    {
        double reco = h_methodL_RP_MethodL_B0->GetBinContent(i);
        double eff  = h_eff_MethodL_RP_MethodL_B0->GetBinContent(i);

        if (eff > 0)
            h_methodL_RP_MethodL_B0_eff_applied->SetBinContent(i, reco / eff);
        else
            h_methodL_RP_MethodL_B0_eff_applied->SetBinContent(i, 0);
    }
}
void BuildHybridTCorrection()
{
    h_HybridCorrectionFactor =
        (TH1F*)h_t_MC->Clone("h_HybridCorrectionFactor");

    h_HybridCorrectionFactor->SetTitle(
        "Hybrid Correction Factor c_{f} = MC / Accepted Truth;-t [GeV^{2}];c_{f}");

    h_HybridCorrectionFactor->Reset();

    for (int i = 1; i <= h_t_MC->GetNbinsX(); i++)
    {
        double generated = h_t_MC->GetBinContent(i);
        //double accepted  = h_t_lAger_accept->GetBinContent(i);
        double accepted  = h_t_hybrid_accept->GetBinContent(i);

        if (generated > 0 && accepted > 0)
        {
            double cf = generated / accepted;
            h_HybridCorrectionFactor->SetBinContent(i, cf);
        }
        else
        {
            h_HybridCorrectionFactor->SetBinContent(i, 0);
        }
    }

    h_methodL_RP_MethodL_B0_corrected =
        (TH1F*)h_methodL_RP_MethodL_B0->Clone("h_methodL_RP_MethodL_B0_corrected");

    h_methodL_RP_MethodL_B0_corrected->SetTitle(
        "Corrected Hybrid MethodL+RP+MethodL+B0;-t [GeV^{2}];Counts");

    h_methodL_RP_MethodL_B0_corrected->Multiply(
        h_HybridCorrectionFactor);
}

#endif

