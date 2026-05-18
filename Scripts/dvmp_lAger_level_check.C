void dvmp_lAger_level_check()
{
    // ---------------- Input file ----------------
    TString inputFile =
        "lager-vmp-00mrad.jpsi-10on130.4pi.disp-jpsi-00-electron.run00001-lumi10.root";

    // ---------------- Read the lAger tree ----------------
    TChain *T = new TChain("lAger");
    T->Add(inputFile);

    Long64_t Nevents = T->GetEntries();
    std::cout << "Number of events = " << Nevents << std::endl;

    if (Nevents == 0) {
        std::cout << "ERROR: No events found. Check file name and tree name." << std::endl;
        return;
    }

    // ---------------- Extract beam energy and luminosity from filename ----------------
    TString beamLabel = "Beam unknown";
    TString lumiLabel = "lAger, luminosity unknown";

    TPRegexp beamRegex("([0-9]+)on([0-9]+)");
    TObjArray *beamMatch = beamRegex.MatchS(inputFile);

    if (beamMatch && beamMatch->GetEntries() >= 3) {
        TString eBeam = ((TObjString*)beamMatch->At(1))->GetString();
        TString pBeam = ((TObjString*)beamMatch->At(2))->GetString();
        beamLabel = Form("%s#times%s GeV^{2}", eBeam.Data(), pBeam.Data());
    }

    TPRegexp lumiRegex("lumi([0-9]+)");
    TObjArray *lumiMatch = lumiRegex.MatchS(inputFile);

    if (lumiMatch && lumiMatch->GetEntries() >= 2) {
        TString lumi = ((TObjString*)lumiMatch->At(1))->GetString();
        lumiLabel = Form("lAger, %s fb^{-1}", lumi.Data());
    }

    std::cout << "Beam label = " << beamLabel << std::endl;
    std::cout << "Luminosity label = " << lumiLabel << std::endl;

    // ---------------- Create output directory ----------------
    gSystem->mkdir("dvmp_jpsi_plots", kTRUE);

    // ---------------- Branch variables ----------------
    Int_t evgen;
    Float_t W, Q2, nu, x, y, epsilon, R, t, xv;
    Short_t n_part;

    const Int_t size1 = 20;
    Double_t fPx[size1], fPy[size1], fPz[size1], fE[size1];
    Int_t PDG[size1];

    T->SetBranchAddress("evgen", &evgen);
    T->SetBranchAddress("W", &W);
    T->SetBranchAddress("Q2", &Q2);
    T->SetBranchAddress("nu", &nu);
    T->SetBranchAddress("x", &x);
    T->SetBranchAddress("y", &y);
    T->SetBranchAddress("epsilon", &epsilon);
    T->SetBranchAddress("R", &R);
    T->SetBranchAddress("t", &t);
    T->SetBranchAddress("xv", &xv);

    T->SetBranchAddress("n_part", &n_part);

    // Important: pass the array itself, not &array[n_part]
    T->SetBranchAddress("particles.fPdgCode", PDG);
    T->SetBranchAddress("particles.fPx", fPx);
    T->SetBranchAddress("particles.fPy", fPy);
    T->SetBranchAddress("particles.fPz", fPz);
    T->SetBranchAddress("particles.fE", fE);

    // ---------------- Histograms ----------------
    TH1F *Electroneta =
        new TH1F("Electroneta", ";#eta;Counts", 200, -7, 7);

    TH1F *Protoneta =
        new TH1F("Protoneta", ";#eta;Counts", 200, -50, 50);

    TH1F *Positroneta =
        new TH1F("Positroneta", ";#eta;Counts", 200, -7, 7);

    TH1F *fElectron =
        new TH1F("fElectron", ";#eta;Counts", 200, -20, 2);

    TH1F *iProtoneta =
        new TH1F("iProtoneta", ";#eta;Counts", 200, -10, 10);

    TH1F *JPsiMass1 =
        new TH1F("JPsiMass1", ";M_{e^{+}e^{-}} [GeV];Counts",
                 200, 2.5, 3.5);

    TH1F *tElectron =
        new TH1F("tElectron", ";P_{T} [GeV];Counts",
                 200, -1, 5);

    TH1F *tPositron =
        new TH1F("tPositron", ";P_{T} [GeV];Counts",
                 200, -1, 5);

    TH1F *tProton =
        new TH1F("tProton", ";P_{T} [GeV];Counts",
                 200, -1, 3);

    TH1F *tfElectron =
        new TH1F("tfElectron", ";P_{T} [GeV];Counts",
                 200, -1, 5);

    TH1F *lElectron =
        new TH1F("lElectron", ";P_{Z} [GeV];Counts",
                 200, -250, 250);

    TH1F *lPositron =
        new TH1F("lPositron", ";P_{Z} [GeV];Counts",
                 200, -250, 250);

    TH1F *lProton =
        new TH1F("lProton", ";P_{Z} [GeV];Counts",
                 200, -275, 500);

    TH1F *lfElectron =
        new TH1F("lfElectron", ";P_{Z} [GeV];Counts",
                 200, -250, 250);

    TH1F *tt =
        new TH1F("tt", ";|t| [GeV^{2}];Counts", 200, -10, 10);

    TH1F *QQ2 =
        new TH1F("QQ2", ";Q^{2} [GeV^{2}];Counts", 100, 1, 10);

    TH1F *xxv =
        new TH1F("xxv", ";x_{v};Counts", 200, -0.1, 1);

    // ---------------- Event loop ----------------
    for (Long64_t l = 0; l < Nevents; l++) {

        T->GetEntry(l);

        TLorentzVector decayElectron;
        TLorentzVector scatteredProton;
        TLorentzVector decayPositron;
        TLorentzVector scatteredElectron;
        TLorentzVector initialProton;

        bool hasDecayElectron     = false;
        bool hasScatteredProton   = false;
        bool hasDecayPositron     = false;
        bool hasScatteredElectron = false;
        bool hasInitialProton     = false;

        for (Int_t j = 0; j < n_part && j < size1; j++) {

            TLorentzVector p;
            p.SetPxPyPzE(fPx[j], fPy[j], fPz[j], fE[j]);

            // These indices follow the current lAger event structure.
            // Verify with PDG[j] if the particle ordering changes.
            if (j == 8) {
                decayElectron = p;
                Electroneta->Fill(p.Eta());
                hasDecayElectron = true;
            }

            if (j == 6) {
                scatteredProton = p;
                Protoneta->Fill(p.Eta());
                hasScatteredProton = true;
            }

            if (j == 7) {
                decayPositron = p;
                Positroneta->Fill(p.Eta());
                hasDecayPositron = true;
            }

            if (j == 4) {
                scatteredElectron = p;
                fElectron->Fill(p.Eta());
                hasScatteredElectron = true;
            }

            if (j == 1) {
                initialProton = p;
                iProtoneta->Fill(p.Eta());
                hasInitialProton = true;
            }
        }

        if (hasDecayElectron && hasDecayPositron) {
            TLorentzVector JPsi = decayElectron + decayPositron;
            JPsiMass1->Fill(JPsi.M());
        }

        if (hasDecayElectron) {
            tElectron->Fill(decayElectron.Pt());
            lElectron->Fill(decayElectron.Pz());
        }

        if (hasDecayPositron) {
            tPositron->Fill(decayPositron.Pt());
            lPositron->Fill(decayPositron.Pz());
        }

        if (hasScatteredProton) {
            tProton->Fill(scatteredProton.Pt());
            lProton->Fill(scatteredProton.Pz());
        }

        if (hasScatteredElectron) {
            tfElectron->Fill(scatteredElectron.Pt());
            lfElectron->Fill(scatteredElectron.Pz());
        }

        tt->Fill(-t);
        QQ2->Fill(Q2);
        xxv->Fill(xv);
    }

    // ---------------- Canvas 1: eta distributions ----------------
    TCanvas *c1 = new TCanvas("c1", "Eta distributions", 800, 800);
    c1->Divide(2, 2);

    c1->cd(1);
    Electroneta->SetStats(0);
    Electroneta->Draw("hist");

    TLegend *leg_eta1 = new TLegend(0.55, 0.72, 0.88, 0.88);
    leg_eta1->SetBorderSize(0);
    leg_eta1->SetFillStyle(0);
    leg_eta1->AddEntry(Electroneta, "Truth", "l");
    leg_eta1->AddEntry((TObject*)0, lumiLabel, "");
    leg_eta1->AddEntry((TObject*)0, beamLabel, "");
    leg_eta1->Draw();

    c1->cd(2);
    Positroneta->SetStats(0);
    Positroneta->Draw("hist");

    TLegend *leg_eta2 = new TLegend(0.55, 0.72, 0.88, 0.88);
    leg_eta2->SetBorderSize(0);
    leg_eta2->SetFillStyle(0);
    leg_eta2->AddEntry(Positroneta, "Truth", "l");
    leg_eta2->AddEntry((TObject*)0, lumiLabel, "");
    leg_eta2->AddEntry((TObject*)0, beamLabel, "");
    leg_eta2->Draw();

    c1->cd(3);
    Protoneta->SetStats(0);
    Protoneta->GetXaxis()->SetRangeUser(5, 11);
    Protoneta->Draw("hist");

    TLegend *leg_eta3 = new TLegend(0.55, 0.72, 0.88, 0.88);
    leg_eta3->SetBorderSize(0);
    leg_eta3->SetFillStyle(0);
    leg_eta3->AddEntry(Protoneta, "Truth", "l");
    leg_eta3->AddEntry((TObject*)0, lumiLabel, "");
    leg_eta3->AddEntry((TObject*)0, beamLabel, "");
    leg_eta3->Draw();

    c1->cd(4);
    fElectron->SetStats(0);
    fElectron->GetXaxis()->SetRangeUser(-4, 0);
    fElectron->Draw("hist");

    TLegend *leg_eta4 = new TLegend(0.55, 0.72, 0.88, 0.88);
    leg_eta4->SetBorderSize(0);
    leg_eta4->SetFillStyle(0);
    leg_eta4->AddEntry(fElectron, "Truth", "l");
    leg_eta4->AddEntry((TObject*)0, lumiLabel, "");
    leg_eta4->AddEntry((TObject*)0, beamLabel, "");
    leg_eta4->Draw();

    c1->SaveAs("dvmp_jpsi_plots/Generator_level_eta_distributions.pdf");

    // ---------------- Canvas 2: J/Psi mass ----------------
    TCanvas *c2 = new TCanvas("c2", "J/Psi Mass", 800, 800);

    JPsiMass1->SetStats(0);
    JPsiMass1->GetXaxis()->SetRangeUser(2.8, 3.3);
    JPsiMass1->Draw("hist");

    TLegend *leg_jpsi = new TLegend(0.55, 0.72, 0.88, 0.88);
    leg_jpsi->SetBorderSize(0);
    leg_jpsi->SetFillStyle(0);
    leg_jpsi->AddEntry(JPsiMass1, "Truth", "l");
    leg_jpsi->AddEntry((TObject*)0, lumiLabel, "");
    leg_jpsi->AddEntry((TObject*)0, beamLabel, "");
    leg_jpsi->Draw();

    c2->SaveAs("dvmp_jpsi_plots/Generator_level_jpsi_mass.pdf");

    // ---------------- Canvas 3: initial proton eta ----------------
    TCanvas *c3 = new TCanvas("c3", "Initial Proton Eta", 800, 800);

    iProtoneta->SetStats(0);
    iProtoneta->GetXaxis()->SetRangeUser(0, 2);
    iProtoneta->Draw("hist");

    TLegend *leg_ip = new TLegend(0.55, 0.72, 0.88, 0.88);
    leg_ip->SetBorderSize(0);
    leg_ip->SetFillStyle(0);
    leg_ip->AddEntry(iProtoneta, "Truth", "l");
    leg_ip->AddEntry((TObject*)0, lumiLabel, "");
    leg_ip->AddEntry((TObject*)0, beamLabel, "");
    leg_ip->Draw();

    c3->SaveAs("dvmp_jpsi_plots/Generator_level_initial_proton_eta.pdf");

    // ---------------- Canvas 4: transverse momentum ----------------
    TCanvas *c4 = new TCanvas("c4", "Transverse Momentum", 800, 800);
    c4->Divide(2, 2);

    c4->cd(1);
    tElectron->SetStats(0);
    tElectron->Draw("hist");

    TLegend *leg_pt1 = new TLegend(0.55, 0.72, 0.88, 0.88);
    leg_pt1->SetBorderSize(0);
    leg_pt1->SetFillStyle(0);
    leg_pt1->AddEntry(tElectron, "Truth", "l");
    leg_pt1->AddEntry((TObject*)0, lumiLabel, "");
    leg_pt1->AddEntry((TObject*)0, beamLabel, "");
    leg_pt1->Draw();

    c4->cd(2);
    tPositron->SetStats(0);
    tPositron->Draw("hist");

    TLegend *leg_pt2 = new TLegend(0.55, 0.72, 0.88, 0.88);
    leg_pt2->SetBorderSize(0);
    leg_pt2->SetFillStyle(0);
    leg_pt2->AddEntry(tPositron, "Truth", "l");
    leg_pt2->AddEntry((TObject*)0, lumiLabel, "");
    leg_pt2->AddEntry((TObject*)0, beamLabel, "");
    leg_pt2->Draw();

    c4->cd(3);
    tProton->SetStats(0);
    tProton->Draw("hist");

    TLegend *leg_pt3 = new TLegend(0.55, 0.72, 0.88, 0.88);
    leg_pt3->SetBorderSize(0);
    leg_pt3->SetFillStyle(0);
    leg_pt3->AddEntry(tProton, "Truth", "l");
    leg_pt3->AddEntry((TObject*)0, lumiLabel, "");
    leg_pt3->AddEntry((TObject*)0, beamLabel, "");
    leg_pt3->Draw();

    c4->cd(4);
    tfElectron->SetStats(0);
    tfElectron->Draw("hist");

    TLegend *leg_pt4 = new TLegend(0.55, 0.72, 0.88, 0.88);
    leg_pt4->SetBorderSize(0);
    leg_pt4->SetFillStyle(0);
    leg_pt4->AddEntry(tfElectron, "Truth", "l");
    leg_pt4->AddEntry((TObject*)0, lumiLabel, "");
    leg_pt4->AddEntry((TObject*)0, beamLabel, "");
    leg_pt4->Draw();

    c4->SaveAs("dvmp_jpsi_plots/Generator_level_transverse_momentum.pdf");

    // ---------------- Canvas 5: longitudinal momentum ----------------
    TCanvas *c5 = new TCanvas("c5", "Longitudinal Momentum", 800, 800);
    c5->Divide(2, 2);

    c5->cd(1);
    lElectron->SetStats(0);
    lElectron->GetXaxis()->SetRangeUser(-25, 60);
    lElectron->Draw("hist");

    TLegend *leg_pz1 = new TLegend(0.55, 0.72, 0.88, 0.88);
    leg_pz1->SetBorderSize(0);
    leg_pz1->SetFillStyle(0);
    leg_pz1->AddEntry(lElectron, "Truth", "l");
    leg_pz1->AddEntry((TObject*)0, lumiLabel, "");
    leg_pz1->AddEntry((TObject*)0, beamLabel, "");
    leg_pz1->Draw();

    c5->cd(2);
    lPositron->SetStats(0);
    lPositron->GetXaxis()->SetRangeUser(-25, 60);
    lPositron->Draw("hist");

    TLegend *leg_pz2 = new TLegend(0.55, 0.72, 0.88, 0.88);
    leg_pz2->SetBorderSize(0);
    leg_pz2->SetFillStyle(0);
    leg_pz2->AddEntry(lPositron, "Truth", "l");
    leg_pz2->AddEntry((TObject*)0, lumiLabel, "");
    leg_pz2->AddEntry((TObject*)0, beamLabel, "");
    leg_pz2->Draw();

    c5->cd(3);
    lProton->SetStats(0);
    lProton->GetXaxis()->SetRangeUser(110, 150);
    lProton->Draw("hist");

    TLegend *leg_pz3 = new TLegend(0.55, 0.72, 0.88, 0.88);
    leg_pz3->SetBorderSize(0);
    leg_pz3->SetFillStyle(0);
    leg_pz3->AddEntry(lProton, "Truth", "l");
    leg_pz3->AddEntry((TObject*)0, lumiLabel, "");
    leg_pz3->AddEntry((TObject*)0, beamLabel, "");
    leg_pz3->Draw();

    c5->cd(4);
    lfElectron->SetStats(0);
    lfElectron->GetXaxis()->SetRangeUser(-25, 5);
    lfElectron->Draw("hist");

    TLegend *leg_pz4 = new TLegend(0.55, 0.72, 0.88, 0.88);
    leg_pz4->SetBorderSize(0);
    leg_pz4->SetFillStyle(0);
    leg_pz4->AddEntry(lfElectron, "Truth", "l");
    leg_pz4->AddEntry((TObject*)0, lumiLabel, "");
    leg_pz4->AddEntry((TObject*)0, beamLabel, "");
    leg_pz4->Draw();

    c5->SaveAs("dvmp_jpsi_plots/Generator_level_longitudinal_momentum.pdf");

    // ---------------- Canvas 6: generator-level summary distributions ----------------
    TCanvas *c6 = new TCanvas("c6", "Generator-level summary distributions", 800, 800);
    c6->Divide(2, 2);

    c6->cd(1);
    gPad->SetLogy();
    tt->SetStats(0);
    tt->SetLineWidth(2);
    tt->GetXaxis()->SetRangeUser(0.0, 1.7);
    tt->Draw("hist");

    TLegend *leg1 = new TLegend(0.55, 0.72, 0.88, 0.88);
    leg1->SetBorderSize(0);
    leg1->SetFillStyle(0);
    leg1->AddEntry(tt, "Truth", "l");
    leg1->AddEntry((TObject*)0, lumiLabel, "");
    leg1->AddEntry((TObject*)0, beamLabel, "");
    leg1->Draw();

    c6->cd(2);
    gPad->SetLogy();
    QQ2->SetStats(0);
    QQ2->SetLineWidth(2);
    QQ2->GetXaxis()->SetRangeUser(1.0, 10.0);
    QQ2->Draw("hist");

    TLegend *leg2 = new TLegend(0.55, 0.72, 0.88, 0.88);
    leg2->SetBorderSize(0);
    leg2->SetFillStyle(0);
    leg2->AddEntry(QQ2, "Truth", "l");
    leg2->AddEntry((TObject*)0, lumiLabel, "");
    leg2->AddEntry((TObject*)0, beamLabel, "");
    leg2->Draw();

    c6->cd(3);
    gPad->SetLogy();
    xxv->SetStats(0);
    xxv->SetLineWidth(2);
    xxv->GetXaxis()->SetRangeUser(0.0, 1.0);
    xxv->Draw("hist");

    TLegend *leg3 = new TLegend(0.55, 0.72, 0.88, 0.88);
    leg3->SetBorderSize(0);
    leg3->SetFillStyle(0);
    leg3->AddEntry(xxv, "Truth", "l");
    leg3->AddEntry((TObject*)0, lumiLabel, "");
    leg3->AddEntry((TObject*)0, beamLabel, "");
    leg3->Draw();

    c6->cd(4);
    gPad->SetLogy();
    JPsiMass1->SetStats(0);
    JPsiMass1->SetLineWidth(2);
    JPsiMass1->GetXaxis()->SetRangeUser(2.8, 3.3);
    JPsiMass1->Draw("hist");

    TLegend *leg4 = new TLegend(0.55, 0.72, 0.88, 0.88);
    leg4->SetBorderSize(0);
    leg4->SetFillStyle(0);
    leg4->AddEntry(JPsiMass1, "Truth", "l");
    leg4->AddEntry((TObject*)0, lumiLabel, "");
    leg4->AddEntry((TObject*)0, beamLabel, "");
    leg4->Draw();

    c6->SaveAs("dvmp_jpsi_plots/Generator_level_summary_distributions.pdf");

    c6->Update();
}
