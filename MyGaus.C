//---------------
// 2014-1-30
// version 1.1
//<1>copy code from pid_standard.C
//<2>used to calculate mean ADC value for diff delay time and diff SHARPER setting
//#include "Minuit.h"
#include "TH1.h"
#include "TF1.h"
#include "TH3.h"
#include "TLegend.h"
#include "TCanvas.h"
#include <stdlib.h>
#include "iostream"
#include "string.h"
#include "TROOT.h"
#include "TFile.h"
#include "TChain.h"
#include "TH2.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TProfile.h"
#include "TTree.h"
#include "TNtuple.h"
#include "TRandom.h"
#include "TMath.h"
#include "TVector2.h"
#include "TVector3.h"
#include "TLorentzVector.h"
#include "TRandom3.h"
#include "TStopwatch.h"
#include "TLatex.h"
#include "TLine.h"
#include "TSpectrum.h"
#define nSigmaCut 10
TLatex* drawLatex(Double_t x, Double_t y, char* text, Int_t textFont, Double_t texSize, Int_t colorIndex);
TLine* drawLine(Double_t xlow,Double_t ylow, Double_t xup, Double_t yup, Double_t lineWidth, Int_t lineColor,Int_t linestyle);
void savepic(TCanvas *c1,char *dir,char *name);
//-------------------------------------------------------------
Double_t myGaus1(Double_t *x, Double_t *par)
{
  return par[0]*TMath::Gaus(x[0], par[1], par[2], kTRUE);
}

Double_t myGaus2(Double_t *x, Double_t *par)
{
  return par[0]*TMath::Gaus(x[0], par[1], par[2]);
}

Double_t myGaus3(Double_t *x, Double_t *par)
{
  return par[0]*TMath::Gaus(x[0], par[1], par[2], kTRUE)*par[3];
}

Double_t myLandau(Double_t *x, Double_t *par)
{
  return 1/sqrt(2*TMath::Pi())*exp(-1/2);
}

Double_t SPEFun(Double_t *x, Double_t *par) {
  Double_t parexp[4], pargaus[4];
  for(int i=0;i<3;i++) {
    parexp[i] = par[i];
    pargaus[i] = par[i+3];
  }
  parexp[3] = par[6];
  pargaus[3] = par[6];
  return myExp(x,parexp) + myGaus(x,pargaus);
}

Double_t crystalFun(Double_t *x, Double_t *par)
{
  Double_t Eg = par[0];
  Double_t p0 = par[1];
  Double_t ped = par[2];
  Double_t Lc = par[3];
  Double_t Tc = par[4];
  Double_t A = par[5];
  Double_t Npe = par[6];
  Double_t Ec = Eg/(1.+2*0.511/Eg);
  Double_t xx = x[0] - ped;
  Double_t val = Lc/(1.+exp((xx-Ec*Npe*p0)/Tc));
  //val = 0;
  val += A*exp(-Eg*Npe)*pow(Eg*Npe,xx/p0)/TMath::Gamma(xx/p0+1.);
  return val;
}

//-----------------------------------------------
void MyGaus()
{
  TStopwatch SW;
  SW.Start();
  TH1::SetDefaultSumw2();
  TCanvas *cc = new TCanvas("cc","",0,0,800,600);
  gStyle->SetOptFit(1111);
  gStyle->SetOptStat(0);
  gStyle->SetEndErrorSize(0.01);
  gStyle->SetPalette(1,0);
  gStyle->SetOptLogz(1);
  gStyle->SetOptTitle(0);
  cc->SetFillColor(10);
  cc->SetBorderMode(0);
  cc->SetBorderSize(2);
  cc->SetFrameFillColor(0);
  cc->SetFrameBorderMode(0);
  //cc->SetLogy();                                                                                          
  cc->SetGrid(1);
  cc->SetLeftMargin(0.12);
  cc->SetBottomMargin(0.15);
  cc->SetTopMargin(0.025);
  cc->SetRightMargin(0.025);
  cc->SetLogz(1);
  TF1 *f1 = new TF1("f1",myGaus1,0,10,3);
  TF1 *f2 = new TF1("f2",myGaus2,0,10,3);
  TF1 *f3 = new TF1("f3",myGaus3,0,10,4);
  TRandom3 random;
  Float_t a = 0;
  TH1F *h = new TH1F("h","Gaus distribution",100,0,10);
  for(int i=0;i<1e3;i++)
    {
      a = random.Gaus(5,1);
      h->Fill(a);
    }	 
  f1->SetParameters(1e3,5,1);
  f2->SetParameters(1e3,5,1);
  Float_t binw = h->GetBinWidth(1);
  f3->SetParameters(1e3,5,1,binw);
  f3->FixParameter(3,binw);
  h->Fit(f1);
  h->Fit(f2);
  h->Fit(f3);
  SW.Stop();
  SW.Print();
}

void savepic(TCanvas *c1,char *dir,char *name)
{
  char temp[1024];
  // sprintf(temp,"%s/%s.C",dir,name);
  //c1->SaveAs(temp);
  sprintf(temp,"%s/%s.gif",dir,name);
  c1->SaveAs(temp);
  c1->Clear();
  c1->cd();
  //return 1;
}

TLatex* drawLatex(Double_t x, Double_t y, char* text, Int_t textFont, Double_t textSize, Int_t colorIndex){
  TLatex *latex = new TLatex(x,y,text);
  latex->SetNDC();
  latex->SetTextFont(textFont);
  latex->SetTextSize(textSize);
  latex->SetTextColor(colorIndex);
  latex->Draw("same");
  return latex;
}
TLine* drawLine(Double_t xlow,Double_t ylow, Double_t xup, Double_t yup, Double_t lineWidth, Int_t lineColor,Int_t linestyle){
  TLine *l1 = new TLine(xlow,ylow,xup,yup);
  l1->SetLineWidth(lineWidth);
  l1->SetLineColor(lineColor);
  l1->SetLineStyle(linestyle);
  l1->Draw("same");
  return l1;
}
