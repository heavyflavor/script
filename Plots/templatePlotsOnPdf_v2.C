//===============================
//Version: 1.2 
//Time: Wed Sep 10 09:43:26 EDT 2014 
//Author: Long Zhou 
//Discribe: Use Landau and Gaus function to fit raw data and tring to seperate noise and signal. 

//===============================
//Version: 1.0 
//Time: Tue Sep  2 10:38:19 EDT 2014 
//Author: Long Zhou 
//Discribe: Use mean+3sigma to reject noise 

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
#include "TPDF.h"
#include "TLatex.h"
#include "TColor.h"
#include "TPad.h"
#define nSigmaCut 10
TLatex* drawLatex(Double_t x, Double_t y, char* text, Int_t textFont, Double_t texSize, Int_t colorIndex);
TLine* drawLine(Double_t xlow,Double_t ylow, Double_t xup, Double_t yup, Double_t lineWidth, Int_t lineColor,Int_t linestyle);
void savepic(TCanvas *c1,char *dir,char *name);
//-------------------------------------------------------------
Double_t myGaus(Double_t *x, Double_t *par)
{
  return par[0]*TMath::Gaus(x[0], par[1], par[2], kTRUE);
}

Double_t myLandau(Double_t *x, Double_t *par)
{
  return par[0]*TMath::Landau(x[0],par[1],par[2], kTRUE);
}

Double_t SPEFun(Double_t *x, Double_t *par) {
  Double_t parlan[3], pargaus[3];
  for(int i=0;i<3;i++) {
    parlan[i] = par[i];
    pargaus[i] = par[i+3];
  }
  return myLandau(x,parlan) + myGaus(x,pargaus);
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
void PedCorrection(char *filename="run_15163001",Int_t ladder)
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
  gStyle->SetOptLogy(0);
  gStyle->SetOptTitle(1);
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
  //------------------------------------
  float correct=0;
  TFile *f1 = new TFile(Form("%s.root",filename));
  ///TFile *outfile = new TFile(Form("%s.cor.root",filename),"RECREATE");
  ///TPDF *pdf = new TPDF(Form("%s.pdf",filename));

  ////TTree *mTree = new TTree("mTree","correction factor");
  ////mTree->Branch("correct",&correct,"correct/F");
  TH2I *AdcStrip;
  Int_t iside = ladder/20;
  Int_t iladder = ladder%20;
  AdcStrip = (TH2I *)f1->Get(Form("ADCStrip_%d_%d",iside,iladder));
  AdcStrip->SetName(Form("AdcStrip_%d_%d",iside,iladder));
  TH1F *AdcStrip1;//mean histogram
  TH1F *AdcStrip2;//sigma histogram
  TH2I *temp;

  //START DAW HISTOGRAMS.
  const float Footx1 = 0.05; //Date     
  const float Footx2 = 0.43; //Author/Aff
  const float Footx3 = 0.96; //Pagenumber 
  const float Footy  = 0.3;
  const float Footy2 = 0.2;
  TLatex   tx;
  TPad *pad1 = new TPad("pad1","",0.00,0.9,1.00,1.0);
  pad1->SetFillColor(TColor::GetColor("#3399CC"));
  pad1->Draw();  

  TPad *pad = new TPad("pad","",0.00,0.08,1.00,0.9);
  pad->Draw();

  TPad *padFoot = new TPad("padFoot","",0.00,0.00,1.00,0.05);
  padFoot->SetFillColor(TColor::GetColor("#3399CC"));
  padFoot->Draw();

  pad1->cd();
  tx.SetTextColor(kWhite);
  tx.SetTextSize(0.65);
  //tx.DrawLatex(0.05,0.3,Form("Run %d",runnumber));
  tx.DrawLatex(0.05,0.3,"Run 15163001");

  padFoot->cd();
  tx.DrawLatex(Footx2, Footy, "Long Zhou/BNL&USTC");
  TString datime  = (new TDatime())->AsSQLString();
  TString date = datime(0,10);
  tx.DrawLatex(Footx1, Footy, date.Data());

  pad->cd();
  tx.SetTextColor(1);

  tx.SetTextSize(0.06);
  char tlName[100];
  tx.DrawLatex(0.1, 0.75,Form("File Name : %s",filename));
  tx.DrawLatex(0.1, 0.65,"Noise : Gaus");
  tx.DrawLatex(0.1, 0.55,"Signal: Landau");
  tx.DrawLatex(0.1,0.45,"Fit Function: Gaus+Landau");
  tx.DrawLatex(0.1,0.35,"Work dir: /star/u/zhoulong/SSD/QA/ReZS");
  float ty1 = 0.58;
  float ty2 = 0.44;
  float dy  = 0.06;
  cc->Print(Form("%s_%d.pdf(",filename,ladder));
  cc->Clear();
  //---------------------------------------------
  TF1 *fun = new TF1("fun",SPEFun,300,700,6);
  //fun->SetLineWidth(0.02);
  fun->SetParameters(600,384,10,1e5,375,3);
  fun->SetParNames("Constant","MPV","Sigma","NNoise","mean","sigma");
  fun->SetParLimits(0,0,1e6);
  fun->SetParLimits(1,380,500);
  fun->SetParLimits(2,1,50);
  fun->SetParLimits(3,0,1e6);
  fun->SetParLimits(4,360,400);
  fun->SetParLimits(5,1,10);
  TF1 *lanfun = new TF1("lanfun",myLandau,300,700,4);
  lanfun->SetLineColor(2);
  //lanfun->SetLineWidth(0.02);
  TF1 *gausfun = new TF1("gausfun",myGaus,300,700,4);
  gausfun->SetLineColor(4);
  //gausfun->SetLineWidth(0.02);
  Float_t binw = AdcStrip->GetXaxis()->GetBinWidth(1);
  //fun->FixParameter(6,binw);
  double par[7];
  cout<<"Processing Side #"<<iside<<" Ladder #"<<iladder<<"......"<<endl;
  Int_t binx = AdcStrip->GetNbinsX();

  for(int b=1;b<=binx;b++)
    {
      if(b%4==1) {cc->cd(); cc->Clear();cc->Divide(2,2);cc->cd(1)->SetLogy(1);cout<<"Divide to 2x2. and cc->cd(1)"<<endl;}
      else {cc->cd((b-1)%4+1)->SetLogy(1); cout<<"cd "<<(b-1)%4+1<<endl;}
      //cc->SetLogx(1);
      AdcStrip1 = (TH1F *)AdcStrip->ProjectionY(Form("AdcStrip_%d_%d_%d",iside,iladder,b),b,b);
      AdcStrip1->SetName(Form("S%d_L%d_A%d",iside,iladder,b));
      AdcStrip1->GetXaxis()->SetRangeUser(300,800);
      AdcStrip1->SetFillColor(38);
      AdcStrip1->LabelsDeflate();
      //AdcStrip1->SetMarkerSize(0.2);
      Float_t rms = AdcStrip->GetRMS();
      if(AdcStrip1->GetEntries()==0) AdcStrip1->Draw("HIST");
      else  
	{
	  fun->SetParameter(2,20.);
	  fun->SetParameter(5,7.);
	  AdcStrip1->Fit(fun,"Q");
	  AdcStrip1->Draw("HIST");
	  fun->GetParameters(par);
	  lanfun->SetParameters(par);
	  gausfun->SetParameters(&par[3]);
	  fun->DrawCopy("same");
	  lanfun->DrawCopy("same");
	  gausfun->DrawCopy("same");
	}
      if(b%4==0)
	{
	  if(b!=96)
	    cc->Print(Form("%s_%d.pdf",filename,ladder));
	  else 
	    {cc->Print(Form("%s_%d.pdf)",filename,ladder)); cout<<"Draw canvas to pdf .."<<endl;}
	}
    }
	 
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
