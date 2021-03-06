#include <iostream>
#include <fstream>

#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TCanvas.h"
#include "TStyle.h"

#include "Lsidis.h"


using namespace std;


//Acceptance
TFile * file_e = new TFile("Acceptance/acceptance_solid_SIDIS_NH3_electron_201710_1e7_output_final.root", "r");
TFile * file_pip = new TFile("Acceptance/acceptance_solid_SIDIS_NH3_pip_201710_1e7_output_final.root", "r");
TFile * file_pim = new TFile("Acceptance/acceptance_solid_SIDIS_NH3_pim_201710_1e7_output_final.root", "r");
TH3F * acc_FA_e = (TH3F *) file_e->Get("acceptance_ThetaPhiP_forwardangle");
TH3F * acc_LA_e = (TH3F *) file_e->Get("acceptance_ThetaPhiP_largeangle");
TH3F * acc_FA_pip = (TH3F *) file_pip->Get("acceptance_ThetaPhiP_forwardangle");
TH3F * acc_LA_pip = (TH3F *) file_pip->Get("acceptance_ThetaPhiP_largeangle");
TH3F * acc_FA_pim = (TH3F *) file_pim->Get("acceptance_ThetaPhiP_forwardangle");
TH3F * acc_LA_pim = (TH3F *) file_pim->Get("acceptance_ThetaPhiP_largeangle");

double GetAcceptance_e(const TLorentzVector p, const char * detector = "all"){//Get electron acceptance
  double theta = p.Theta() / M_PI * 180.0;
  double phi = p.Phi() / M_PI * 180.0;
  if (theta > 50.0) return 0;
  double mom = p.P();
  double acc = 0;
  if (strcmp(detector, "FA") == 0 || strcmp(detector, "all") == 0)
    acc += acc_FA_e->GetBinContent(acc_FA_e->GetXaxis()->FindBin(theta), acc_FA_e->GetYaxis()->FindBin(phi), acc_FA_e->GetZaxis()->FindBin(mom));
  if (mom > 3.5 && (strcmp(detector, "LA") == 0 || strcmp(detector, "all") == 0))
    acc += acc_LA_e->GetBinContent(acc_LA_e->GetXaxis()->FindBin(theta), acc_LA_e->GetYaxis()->FindBin(phi), acc_LA_e->GetZaxis()->FindBin(mom));
  return acc;
}

double GetAcceptance_pip(const TLorentzVector p){//Get pi+ acceptance
  double theta = p.Theta() / M_PI * 180.0;
  double phi = p.Phi() / M_PI * 180.0;
  if (theta > 45.0) return 0;
  double mom = p.P();
  if (mom > 7.5) return 0;
  double acc = 0;
  acc += acc_FA_pip->GetBinContent(acc_FA_pip->GetXaxis()->FindBin(theta), acc_FA_pip->GetYaxis()->FindBin(phi), acc_FA_pip->GetZaxis()->FindBin(mom));
  return acc;
}

double GetAcceptance_pim(const TLorentzVector p){//Get pi+ acceptance
  double theta = p.Theta() / M_PI * 180.0;
  double phi = p.Phi() / M_PI * 180.0;
  if (theta > 45.0) return 0;
  double mom = p.P();
  if (mom > 7.5) return 0;
  double acc = 0;
  acc += acc_FA_pim->GetBinContent(acc_FA_pim->GetXaxis()->FindBin(theta), acc_FA_pim->GetYaxis()->FindBin(phi), acc_FA_pim->GetZaxis()->FindBin(mom));
  return acc;
}

double GetAcceptance_hadron(const TLorentzVector p, const char * hadron = "pi+"){//
  if (strcmp(hadron, "pi+") == 0) return GetAcceptance_pip(p);
  else if (strcmp(hadron, "pi-") == 0) return GetAcceptance_pim(p);
  else return 0;
}

int main(const int argc, const char * argv[]){

  if (argc < 3) {
    cout << "./analysis_region <Ebeam> <Hadron> <filename>" << endl;
    return 0;
  }
	       
  double Ebeam = atof(argv[1]);
  
  Lsidis sidis;
  TLorentzVector l(0, 0, Ebeam, Ebeam);
  TLorentzVector P(0, 0, 0, 0.938272);
  sidis.SetNucleus(0.334*10.0, 0.334*7.0);
  sidis.SetHadron(argv[2]);

  sidis.ChangeTMDpars(0.604, 0.131);

  sidis.SetInitialState(l, P);
  sidis.SetPDFset("CJ15lo");
  sidis.SetFFset("DSSFFlo");

  double lumi = 1.0e+9 * pow(0.197327, 2);
  double Xmin[6] = {0.03, 1.0, 0.3, 0.0, -M_PI, -M_PI};
  double Xmax[6] = {0.7, 10.0, 0.7, 2.0, M_PI, M_PI};
  sidis.SetRange(Xmin, Xmax);

  Long64_t Nsim = 10000000;
  double weight = 0;
  double acc = 0;
  TLorentzVector lp(0, 0, 0, 0);
  TLorentzVector Ph(0, 0, 0, 0);

  TFile * fs = new TFile(argv[3], "RECREATE");
  TH2D * hxQ2 = new TH2D("hxQ2", "", 100, 0.0, 1.0, 100, 0.0, 10.0);
  TH2D * h0xQ2 = new TH2D("h0xQ2", "", 100, 0.0, 1.0, 100, 0.0, 10.0);
  TH2D * h1xQ2 = new TH2D("h1xQ2", "", 100, 0.0, 1.0, 100, 0.0, 10.0);
  TH2D * h2xQ2 = new TH2D("h2xQ2", "", 100, 0.0, 1.0, 100, 0.0, 10.0);
  TH2D * h3xQ2 = new TH2D("h3xQ2", "", 100, 0.0, 1.0, 100, 0.0, 10.0);
  TH2D * hzPt = new TH2D("hzPt", "", 100, 0.0, 1.0, 100, 0.0, 2.0);
  TH2D * h0zPt = new TH2D("h0zPt", "", 100, 0.0, 1.0, 100, 0.0, 2.0);
  TH2D * h1zPt = new TH2D("h1zPt", "", 100, 0.0, 1.0, 100, 0.0, 2.0);
  TH2D * h2zPt = new TH2D("h2zPt", "", 100, 0.0, 1.0, 100, 0.0, 2.0);
  TH2D * h3zPt = new TH2D("h3zPt", "", 100, 0.0, 1.0, 100, 0.0, 2.0);
  hxQ2->SetDirectory(fs);
  h0xQ2->SetDirectory(fs);
  h1xQ2->SetDirectory(fs);
  h2xQ2->SetDirectory(fs);
  h3xQ2->SetDirectory(fs);
  hzPt->SetDirectory(fs);
  h0zPt->SetDirectory(fs);
  h1zPt->SetDirectory(fs);
  h2zPt->SetDirectory(fs);
  h3zPt->SetDirectory(fs);
 
  for (Long64_t i = 0; i < Nsim; i++){
    if (i % (Nsim/10) == 0) cout << i / (Nsim/100) << "%" << endl;
    weight = sidis.GenerateEvent(0, 1);
    if (weight > 0){
      if (sidis.GetVariable("W") < 2.3) continue;
      if (sidis.GetVariable("Wp") < 1.6) continue;
      lp = sidis.GetLorentzVector("lp");
      Ph = sidis.GetLorentzVector("Ph");
      acc = GetAcceptance_e(lp) * GetAcceptance_hadron(Ph, argv[2]);
      if (acc > 0){
	hxQ2->Fill(sidis.GetVariable("x"), sidis.GetVariable("Q2"), weight * acc);
	hzPt->Fill(sidis.GetVariable("z"), sidis.GetVariable("Pt"), weight * acc);
	if (Ph.P() > 2.5 && Ph.P() < 7.5){
	  h0xQ2->Fill(sidis.GetVariable("x"), sidis.GetVariable("Q2"), weight * acc);
	  h0zPt->Fill(sidis.GetVariable("z"), sidis.GetVariable("Pt"), weight * acc);
	}
	if (Ph.P() > 1.6 && Ph.P() < 4.8){
	  h1xQ2->Fill(sidis.GetVariable("x"), sidis.GetVariable("Q2"), weight * acc);
	  h1zPt->Fill(sidis.GetVariable("z"), sidis.GetVariable("Pt"), weight * acc);
	}
	if (Ph.P() > 1.3 && Ph.P() < 3.8){
	  h2xQ2->Fill(sidis.GetVariable("x"), sidis.GetVariable("Q2"), weight * acc);
	  h2zPt->Fill(sidis.GetVariable("z"), sidis.GetVariable("Pt"), weight * acc);
	}
	if (Ph.P() > 1.2 && Ph.P() < 3.3){
	  h3xQ2->Fill(sidis.GetVariable("x"), sidis.GetVariable("Q2"), weight * acc);
	  h3zPt->Fill(sidis.GetVariable("z"), sidis.GetVariable("Pt"), weight * acc);
	}
      }
    }
  }
  hxQ2->Scale(lumi / Nsim);
  h0xQ2->Scale(lumi / Nsim);
  h1xQ2->Scale(lumi / Nsim);
  h2xQ2->Scale(lumi / Nsim);
  h3xQ2->Scale(lumi / Nsim);
  hzPt->Scale(lumi / Nsim);
  h0zPt->Scale(lumi / Nsim);
  h1zPt->Scale(lumi / Nsim);
  h2zPt->Scale(lumi / Nsim);
  h3zPt->Scale(lumi / Nsim);
 
  fs->Write();
  fs->Close();

  return 0;
}

