//===============================
//Version: 1.0 
//Time: Fri Nov 14 13:50:21 EST 2014 
//Author: Long Zhou 
//Discribe: Initial release , used to fit D-E correlation. you need adjust NRebin times 

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "TSystem.h"
#include "TH1.h"
#include "TF1.h"
#include "TH3.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TString.h"
#include "TROOT.h"
#include "TFile.h"
#include "TChain.h"
#include "TH2.h"
#include "TStyle.h"
#include "TProfile.h"
#include "TTree.h"
#include "TNtuple.h"
#include "TRandom.h"
#include "TMath.h"
#include "TVector2.h"
#include "TVector3.h"
#include "TLorentzVector.h"
#include "TRandom3.h"
#include "TLatex.h"
#include "TLine.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "TString.h"
#include "TPDF.h"
#include "TColor.h"
#include "TStopwatch.h"
#include "TAxis.h"
#include "TMatrixDSym.h"
#include "TText.h"
#define TxDefSize   0.05
using namespace std;

#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "RooConstVar.h"
#include "RooPolynomial.h"
#include "RooHistPdf.h"
#include "RooPlot.h"
#include "RooAddPdf.h"
#include "RooChebychev.h"
#include "RooFitResult.h"
#include "RooExponential.h"
using namespace RooFit ;

TH1D* histo(char *name, Double_t xlow, Double_t xup, Double_t ylow, Double_t yup, char* xTitle, char* yTitle);
TLine* drawLine(Double_t xlow,Double_t ylow, Double_t xup, Double_t yup, Double_t lineWidth, Int_t lineColor,Int_t linestyle);
void drawLines(Double_t xlow,Double_t ylow, Double_t xup, Double_t yup, Double_t lineWidth, Int_t lineColor,Int_t linestyle);
void drawBoxes(Int_t n, Double_t* x1, Double_t* x2, Double_t* y1, Double_t* y2, Int_t lineWidth, Int_t lineColor);
TLatex* drawLatex(Double_t x, Double_t y, char* text, Int_t textFont, Double_t texSize, Int_t colorIndex);
void setpad(TPad *pad,float left, float right, float top, float bottom);
void ratio(TGraphAsymmErrors *gr1, TGraph *gr2);
void ratio(TGraphErrors *gr1, TGraph *gr2);
void ratio(TGraph *gr1, TGraph *gr2);
void savepic(TCanvas *c1,TString dir,TString name);
void CanvasPartition(TCanvas *C,const Int_t Nx,const Int_t Ny,
                     Float_t lMargin, Float_t rMargin,
                     Float_t bMargin, Float_t tMargin);
//---------------------------------------------------
Double_t Charm(double* x, double* par)
{
  Double_t un    = ctoeminus->Integral();
  int      ptbin = ctoeminus->GetXaxis()->FindBin(x[0]);
  float    ptlw  = ctoeminus->GetBinCenter(ptbin);
  float    ptup  = ctoeminus->GetBinCenter(ptbin+1);
  float    a1    = ctoeminus->GetBinContent(ptbin);
  float    a2    = ctoeminus->GetBinContent(ptbin+1);
  float    p0    = (a1-a2)/(ptlw-ptup);
  float    p1    = (a2*ptlw-a1*ptup)/(ptlw-ptup);
  
  return par[0]*(p0*x[0]+p1)/un;
}
//----------------------------------------------------
Double_t Bottom(double* x, double* par)
{
  Double_t un    = btoeminus->Integral();
  int      ptbin = btoeminus->GetXaxis()->FindBin(x[0]);
  float    ptlw  = btoeminus->GetBinCenter(ptbin);
  float    ptup  = btoeminus->GetBinCenter(ptbin+1);
  float    a1    = btoeminus->GetBinContent(ptbin);
  float    a2    = btoeminus->GetBinContent(ptbin+1);
  float    p0    = (a1-a2)/(ptlw-ptup);
  float    p1    = (a2*ptlw-a1*ptup)/(ptlw-ptup);
  
  return par[0]*(p0*x[0]+p1)/un;
}
//-----------------------------------------------------
Double_t Combine(double* x, double* par)
{
  double par1[1],par2[1];
  par1[0] = par[0];
  par2[0] = 1 - par[0];
  return par[1]*(Bottom(x,par1) + Charm(x,par2));
}
//-----------------------------------------------------
void pythiaCalculate(Float_t dlowpt=2,Float_t dhigpt=20,Float_t elowpt=2,Float_t ehigpt=20,Int_t roofit=1)
{
  TStopwatch SW;
  SW.Start();
  TH1::SetDefaultSumw2();
  TH2::SetDefaultSumw2();
  TH3::SetDefaultSumw2();
  //gSystem->Load("libRooFit");
  using namespace RooFit;
  //gStyle->SetOptFit(11111);
  gStyle->SetOptFit(0);
  gStyle->SetOptStat(0);
  gStyle->SetEndErrorSize(0.01);
  gStyle->SetPalette(1,0);
  gStyle->SetOptLogz(1);
  gStyle->SetOptTitle(1);
  gStyle->SetPalette(1,0);

  //Float_t dlowpt = 2;
  //Float_t dhigpt = 20;
  //Float_t elowpt = 2;
  //Float_t ehigpt = 20;

  //TCanvas *cc = new TCanvas("cc","",0,0,800,600);
  //cc->SetFillColor(10);
  //cc->SetBorderMode(0);
  //cc->SetBorderSize(2);
  //cc->SetFrameFillColor(0);
  //cc->SetFrameBorderMode(0);
  //cc->SetGrid(1);
  //cc->SetLeftMargin(0.12);
  //cc->SetBottomMargin(0.15);
  //cc->SetTopMargin(0.025);
  //cc->SetRightMargin(0.025); 
  //cc->SetLogz(1);

  TFile *fin = new TFile("./500.root");
  Int_t NRebin = 10; //Rebin pythia simulation .(100 bins)
  //X-DeltaPhi, Y-Trigger Pt, Z-Associate Pt
  TH3F *Deminus_all_correlation_pT       = (TH3F *)fin->Get("Deminus_all_correlation_pT");     	   
  TH3F *Deplus_all_correlation_pT        = (TH3F *)fin->Get("Deplus_all_correlation_pT");     	   
  TH3F *Dbareminus_all_correlation_pT    = (TH3F *)fin->Get("Dbareminus_all_correlation_pT");     	   
  TH3F *Dbareplus_all_correlation_pT     = (TH3F *)fin->Get("Dbareplus_all_correlation_pT");     	   
  TH3F *Dctoeminus_all_correlation_pT    = (TH3F *)fin->Get("Dctoeminus_all_correlation_pT");     	   
  TH3F *Dbtoeminus_all_correlation_pT	 = (TH3F *)fin->Get("Dbtoeminus_all_correlation_pT");	  
  TH3F *Dctoeplus_all_correlation_pT	 = (TH3F *)fin->Get("Dctoeplus_all_correlation_pT");	  
  TH3F *Dbtoeplus_all_correlation_pT	 = (TH3F *)fin->Get("Dbtoeplus_all_correlation_pT");	  
  TH3F *Dbarctoeminus_all_correlation_pT = (TH3F *)fin->Get("Dbarctoeminus_all_correlation_pT");  
  TH3F *Dbarbtoeminus_all_correlation_pT = (TH3F *)fin->Get("Dbarbtoeminus_all_correlation_pT");
  TH3F *Dbarctoeplus_all_correlation_pT	 = (TH3F *)fin->Get("Dbarctoeplus_all_correlation_pT");	 
  TH3F *Dbarbtoeplus_all_correlation_pT  = (TH3F *)fin->Get("Dbarbtoeplus_all_correlation_pT");

  TH1F *D0_pT    = (TH1F *)fin->Get("D0_pT");
  TH1F *D0bar_pT = (TH1F *)fin->Get("D0bar_pT");

  Deminus_all_correlation_pT->Sumw2(kTRUE); 
  Deplus_all_correlation_pT->Sumw2(kTRUE); 
  Dbareminus_all_correlation_pT->Sumw2(kTRUE); 
  Dbareplus_all_correlation_pT->Sumw2(kTRUE); 
  Dctoeminus_all_correlation_pT->Sumw2(kTRUE); 
  Dbtoeminus_all_correlation_pT->Sumw2(kTRUE); 
  Dctoeplus_all_correlation_pT->Sumw2(kTRUE); 
  Dbtoeplus_all_correlation_pT->Sumw2(kTRUE); 
  Dbarctoeminus_all_correlation_pT->Sumw2(kTRUE); 
  Dbarbtoeminus_all_correlation_pT->Sumw2(kTRUE); 
  Dbarctoeplus_all_correlation_pT->Sumw2(kTRUE); 
  Dbarbtoeplus_all_correlation_pT->Sumw2(kTRUE); 

  Int_t   dlowptbin  = Dctoeminus_all_correlation_pT->GetYaxis()->FindBin(dlowpt);
  Int_t   dhigptbin  = Dctoeminus_all_correlation_pT->GetYaxis()->FindBin(dhigpt);
  Int_t   elowptbin  = Dctoeminus_all_correlation_pT->GetZaxis()->FindBin(elowpt);
  Int_t   ehigptbin  = Dctoeminus_all_correlation_pT->GetZaxis()->FindBin(ehigpt);

  Int_t   lowbin     = D0_pT->GetXaxis()->FindBin(dlowpt);
  Int_t   higbin     = D0bar_pT->GetXaxis()->FindBin(dhigpt);
  Float_t NTrgD0     = D0_pT->Integral(lowbin,higbin);
  Float_t NTrgD0bar  = D0bar_pT->Integral(lowbin,higbin);

  TH1F *Deminus      = (TH1F *)Deminus_all_correlation_pT->ProjectionX("Deminus",dlowptbin,dhigptbin,elowptbin,ehigptbin);
  TH1F *Deplus       = (TH1F *)Deplus_all_correlation_pT->ProjectionX("Deplus",dlowptbin,dhigptbin,elowptbin,ehigptbin); 
  TH1F *Dbareminus   = (TH1F *)Dbareminus_all_correlation_pT->ProjectionX("Dbareminus",dlowptbin,dhigptbin,elowptbin,ehigptbin);
  TH1F *Dbareplus    = (TH1F *)Dbareplus_all_correlation_pT->ProjectionX("Dbareplus",dlowptbin,dhigptbin,elowptbin,ehigptbin); 
  TH1F *ctoeminus    = (TH1F *)Dctoeminus_all_correlation_pT->ProjectionX("ctoeminus",dlowptbin,dhigptbin,elowptbin,ehigptbin);
  TH1F *btoeminus    = (TH1F *)Dbtoeminus_all_correlation_pT->ProjectionX("btoeminus",dlowptbin,dhigptbin,elowptbin,ehigptbin); 
  TH1F *ctoeplus     = (TH1F *)Dctoeplus_all_correlation_pT->ProjectionX("ctoeplus",dlowptbin,dhigptbin,elowptbin,ehigptbin); 
  TH1F *btoeplus     = (TH1F *)Dbtoeplus_all_correlation_pT->ProjectionX("btoeplus",dlowptbin,dhigptbin,elowptbin,ehigptbin); 
  TH1F *cbartoeminus = (TH1F *)Dbarctoeminus_all_correlation_pT->ProjectionX("cbartoeminus",dlowptbin,dhigptbin,elowptbin,ehigptbin); 
  TH1F *bbartoeminus = (TH1F *)Dbarbtoeminus_all_correlation_pT->ProjectionX("bbartoeminus",dlowptbin,dhigptbin,elowptbin,ehigptbin); 
  TH1F *cbartoeplus  = (TH1F *)Dbarctoeplus_all_correlation_pT->ProjectionX("cbartoeplus",dlowptbin,dhigptbin,elowptbin,ehigptbin); 
  TH1F *bbartoeplus  = (TH1F *)Dbarbtoeplus_all_correlation_pT->ProjectionX("bbartoeplus",dlowptbin,dhigptbin,elowptbin,ehigptbin); 

  Deminus->Scale(1/NTrgD0);
  Deplus->Scale(1/NTrgD0);
  ctoeminus->Scale(1/NTrgD0); 
  btoeminus->Scale(1/NTrgD0); 
  ctoeplus->Scale(1/NTrgD0); 
  btoeplus->Scale(1/NTrgD0); 

  Dbareminus->Scale(1/NTrgD0bar);
  Dbareplus->Scale(1/NTrgD0bar);
  cbartoeminus->Scale(1/NTrgD0bar); 
  bbartoeminus->Scale(1/NTrgD0bar); 
  cbartoeplus->Scale(1/NTrgD0bar); 
  bbartoeplus->Scale(1/NTrgD0bar);

  Deminus->Rebin(NRebin);
  Deplus->Rebin(NRebin);
  Dbareminus->Rebin(NRebin);
  Dbareplus->Rebin(NRebin);
  ctoeminus->Rebin(NRebin);
  btoeminus->Rebin(NRebin);
  ctoeplus->Rebin(NRebin);
  btoeplus->Rebin(NRebin); 
  cbartoeminus->Rebin(NRebin);
  bbartoeminus->Rebin(NRebin);
  cbartoeplus->Rebin(NRebin);
  bbartoeplus->Rebin(NRebin);

  TCanvas* c1 = new TCanvas("c1","D-E correlation fit results",800,400) ;
  c1->SetBorderSize(0);
  c1->Divide(2);
  c1->cd(1)->SetLogy(1);
  Deminus->SetMarkerStyle(20);
  Deminus->GetYaxis()->SetRangeUser(2e-7,2e-3);
  Deminus->Draw("L");
  ctoeminus->SetLineColor(2);
  ctoeminus->SetMarkerStyle(20);
  ctoeminus->SetMarkerColor(2);
  ctoeminus->Draw("Lsame");
  btoeminus->SetLineColor(4);
  btoeminus->SetMarkerStyle(20);
  btoeminus->SetMarkerColor(4);
  btoeminus->Draw("Lsame");
  c1->cd(2)->SetLogy(1);
  Deplus->SetMarkerStyle(20);
  Deplus->GetYaxis()->SetRangeUser(2e-7,2e-3);
  Deplus->Draw("l");
  ctoeplus->SetLineColor(2);
  ctoeplus->SetMarkerStyle(20);
  ctoeplus->SetMarkerColor(2);
  ctoeplus->Draw("lsame");
  btoeplus->SetLineColor(4);
  btoeplus->SetMarkerStyle(20);
  btoeplus->SetMarkerColor(4);
  btoeplus->Draw("lsame");
  savepic(c1,"./pic","Pythia_DE");
  c1->Close();
  //-----------------------------
  Deplus->Add(Dbareminus,1);
  Deminus->Add(Dbareplus,1);
  ctoeminus->Add(cbartoeplus,1);
  btoeminus->Add(bbartoeplus,1);
  ctoeplus->Add(cbartoeminus,1);
  btoeplus->Add(bbartoeminus,1);
  //-----------------------------

  if(roofit)
    {
  RooRealVar x("x","#Delta#phi",0,TMath::Pi());
  
  RooDataHist dehist("dehist","D-E Correlation hist/LS",x,Deminus);
  RooHistPdf  depdf("depdf","D-E Correlation pdf/LS",x,dehist,2);
  
  RooDataHist pdehist("pdehist","D-E Correlation hist/UL",x,Deplus);
  RooHistPdf  pdepdf("pdepdf","D-E Correlation pdf/UL",x,pdehist,2);
  
  RooDataHist charmhist("charmhist","charm constribution hist",x,ctoeminus);
  RooHistPdf  charmpdf("charmpdf","charm contribution pdf",x,charmhist,2);
  
  RooDataHist bottomhist("bottomhist","bottom constribution hist",x,btoeminus);
  RooHistPdf  bottompdf("bottompdf","bottom contribution pdf",x,bottomhist,2);
  
  RooDataHist pcharmhist("charmhist","charm constribution hist",x,ctoeplus);
  RooHistPdf  pcharmpdf("charmpdf","charm contribution pdf",x,pcharmhist,2);
  
  RooDataHist pbottomhist("bottomhist","bottom constribution hist",x,btoeplus);
  RooHistPdf  pbottompdf("bottompdf","bottom contribution pdf",x,pbottomhist,2);
  
  x.setBins(20);
  RooPlot* frame1 = x.frame(Title("D-E correlation pdf / LS"),Bins(100)) ;
  dehist.plotOn(frame1) ;
  depdf.plotOn(frame1);
  charmhist.plotOn(frame1,LineColor(2)) ;
  charmpdf.plotOn(frame1,LineColor(2)) ;  
  bottomhist.plotOn(frame1,LineColor(2)) ;
  bottompdf.plotOn(frame1,LineColor(2),LineStyle(2)) ;  
  
  RooPlot* frame2 = x.frame(Title("D-E correlation pdf / UL"),Bins(100)) ;
  pdehist.plotOn(frame2) ;
  pdepdf.plotOn(frame2);
  pcharmhist.plotOn(frame2,LineColor(2)) ;
  pcharmpdf.plotOn(frame2,LineColor(2)) ;  
  pbottomhist.plotOn(frame2,LineColor(2)) ;
  pbottompdf.plotOn(frame2,LineColor(2),LineStyle(2)) ;
  
  TCanvas* c = new TCanvas("c","Charm and bottom contribution pdf",800,400) ;  
  c->Divide(2) ;
  c->cd(1) ; gPad->SetLeftMargin(0.15) ; frame1->GetYaxis()->SetTitleOffset(1.4) ; frame1->Draw() ;
  c->cd(2) ; gPad->SetLeftMargin(0.15) ; frame2->GetYaxis()->SetTitleOffset(1.8) ; frame2->Draw() ;
  
  RooRealVar charmfrac("charmfrac","fraction of charm contribution",0.5,0.,1.) ;
  RooRealVar bottomfrac("bottomfrac","fraction of bottom contribution",0.5,0.,1.) ;
  RooRealVar charmyield("charmyield","yield of charm contribution",0.5,0,1) ;
  RooRealVar bottomyield("bottomyield","yield of bottom contribution",0.5,0,1) ;

  RooArgList shapes;
  RooArgList yields;
  shapes.add(charmpdf);  yields.add(charmyield);
  shapes.add(bottompdf); yields.add(bottomyield);

  //RooAddPdf  charmsig("charmsig","charm contribution in D-E correlation",shapes,yields) ;
  RooAddPdf  charmsig("charmsig","charm contribution in D-E correlation",RooArgList(bottompdf,charmpdf),bottomfrac) ;
  
  dehist->Print("v");

  RooChi2Var chi2("chi2","chi2",charmsig,dehist,DataError(RooAbsData::SumW2)) ;
  RooMinuit m(chi2) ;
  m.migrad() ;
  m.hesse() ;
  RooFitResult* r_chi2_wgt = m.save() ;
  r_chi2_wgt->Print();
  
  //use likely-hoold fit 
  //RooFitResult* r = charmsig.fitTo(dehist,Save(),SumW2Error(kTRUE)) ;
  //r->Print() ;
  //r->Print("v") ;
  //
  //cout << "EDM = " << r->edm() << endl ;
  //cout << "-log(L) at minimum = " << r->minNll() << endl ;
  //cout << "final value of floating parameters" << endl ;
  //r->floatParsFinal().Print("s") ;
  //
  //// Extract covariance and correlation matrix as TMatrixDSym
  //const TMatrixDSym& cor = r->correlationMatrix() ;
  //const TMatrixDSym& cov = r->covarianceMatrix() ;
  //
  //// Print correlation, covariance matrix
  //cout << "correlation matrix" << endl ;
  //cor.Print() ;
  //cout << "covariance matrix" << endl ;
  //cov.Print() ;
  
  // P e r s i s t   f i t   r e s u l t   i n   r o o t   f i l e 
  // ------------------------------------------------------------- 
  // Open new ROOT file save save result 
  //TFile f("fitresult.root","RECREATE") ;
  //r->Write("charmcontribution") ;
  //f.Close() ;
  
  RooPlot* frame3 = x.frame(Bins(20),Title("Raw Spectra")) ;
  dehist.plotOn(frame3) ;
  charmhist.plotOn(frame3,MarkerColor(kRed),LineColor(kRed)) ;
  bottomhist.plotOn(frame3,MarkerColor(kBlue),LineColor(kBlue)) ;
  
  //TLegend *legc = new TLegend(0.14,0.2,0.45,0.35);
  //legc->SetFillColor(10);
  //legc->SetTextFont(42);
  //legc->AddEntry(charmhist,"Charmed hadron contribution","lep");
  //legc->AddEntry(bottomhist,"Bottomed hadron contribution","lep");
  //legc->SetTextSize(0.03);
  //legc->SetBorderSize(0);
  //legc->SetFillStyle(0);
  //frame3->addObject(legc);
  //sprintf(latex,"Run11 pp500GeV");
  //drawLatex(0.15,0.36,latex,62,0.04,1);
  
  TText* txtc = new TText(0.5,0.85,"c to e") ;
  txtc->SetNDC(kTRUE);
  txtc->SetTextSize(0.04) ;
  txtc->SetTextColor(kRed) ;
  frame3->addObject(txtc) ;
  TText* txtb = new TText(0.5,0.80,"b to e") ;
  txtb->SetNDC(kTRUE);
  txtb->SetTextSize(0.04) ;
  txtb->SetTextColor(kBlue) ;
  frame3->addObject(txtb) ;
  
  // Plot accept/reject efficiency on data overlay fitted efficiency curve
  RooPlot* frame4 = x.frame(Bins(20),Title("Fitted results")) ;
  dehist.plotOn(frame4) ;
  charmsig.plotOn(frame4,LineColor(kRed)) ;
  //charmpdf.plotOn(frame4,LineColor(kBlue)) ;
  //bottompdf.plotOn(frame4,LineColor(kBlue),LineStyle(2)) ;
  charmsig.plotOn(frame4,Components(charmpdf),LineColor(kBlue));
  charmsig.plotOn(frame4,Components(bottompdf),LineColor(kBlue),LineStyle(2));
  charmsig.paramOn(frame4,Format("NELU",AutoPrecision(2)),Layout(0.2,0.7,0.9));

  // Draw all frames on a canvas
  TCanvas* ca = new TCanvas("ca","D-E correlation fit results",800,400) ;
  ca->Divide(2) ;
  ca->cd(1) ; gPad->SetLeftMargin(0.15) ; frame3->GetYaxis()->SetTitleOffset(1.6) ; frame3->Draw() ;
  ca->cd(2) ; gPad->SetLeftMargin(0.15) ; frame4->GetYaxis()->SetTitleOffset(1.4) ; frame4->Draw() ;
  
  // Add text to frame
  //TText* txt = new TText(2,100,"Signal") ;
  //txt->SetTextSize(0.04) ;
  //txt->SetTextColor(kRed) ;
  //frame->addObject(txt) ;
  //
  //// Add arrow to frame
  //TArrow* arrow = new TArrow(2,100,-1,50,0.01,"|>") ;
  //arrow->SetLineColor(kRed) ;
  //arrow->SetFillColor(kRed) ;
  //arrow->SetLineWidth(3) ;
  //frame->addObject(arrow) ;
    }
  else
    {
  TF1 *fun = new TF1("fun",Combine,0,TMath::Pi(),2);
  fun->SetParameter(0.5,0.002);
  fun->SetLineColor(2);
  Deminus->Fit(fun);
  
  Double_t par[2];
  fun->GetParameters(par);
  Double_t parerr[2] = fun->GetParErrors();
  Double_t fracerr = fun->GetParError(0);
  
  TF1 *fc = new TF1("fc",Charm,0,TMath::Pi(),1);
  TF1 *fb = new TF1("fb",Bottom,0,TMath::Pi(),1);
  fc->SetLineColor(4);
  fc->SetLineStyle(2);
  fc->SetParameter(0,(1-par[0])*par[1]);
  fb->SetLineColor(4);
  fb->SetParameter(0,par[0]*par[1]);
  
  fc->Draw("same");
  fb->Draw("same");
  drawLatex(0.5,0.8,Form("r_{B}=%3.3f#pm%.3f",par[0],fracerr),62,0.04,1);
    }
  SW.Stop();
  SW.Print();
}

//-----------------
//Custom Functions 

void savepic(TCanvas *c1,TString dir,TString name)
{
  char temp[1024];
  gSystem->Exec(Form("mkdir -p %s",dir.Data()));
  sprintf(temp,"%s/%s.gif",dir.Data(),name.Data());
  c1->SaveAs(temp);
  sprintf(temp,"%s/%s.C",dir.Data(),name.Data());
  c1->SaveAs(temp);
  //sprintf(temp,"%s/%s.root",dir.Data(),name.Data());
  //c1->SaveAs(temp);
  c1->Clear();
  c1->cd();
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

void setpad(TPad *pad,float left, float right, float top, float bottom){
  pad->SetFillColor(10);
  pad->SetBorderMode(0);
  pad->SetBorderSize(0);
  pad->SetFrameFillColor(10);
  pad->SetFrameBorderMode(0);
  pad->SetFrameBorderSize(0);
  pad->SetLeftMargin(left);
  pad->SetRightMargin(right);
  pad->SetTopMargin(top);
  pad->SetBottomMargin(bottom);
}

void ratio(TGraph *gr1, TGraph *gr2)
{
  Double_t x, y;
  for(int i=0;i<gr1->GetN();i++) {
    gr1->GetPoint(i,x,y);
    y /= gr2->Eval(x);
    gr1->SetPoint(i,x,y);
  }
}

void ratio(TGraphErrors *gr1, TGraph *gr2)
{
  Double_t x, y, ex, ey;
  for(int i=0;i<gr1->GetN();i++) {
    gr1->GetPoint(i,x,y);
    ex = gr1->GetErrorX(i);
    ey = gr1->GetErrorY(i);
    y /= gr2->Eval(x);
    ey /= gr2->Eval(x);
    gr1->SetPoint(i,x,y);
    gr1->SetPointError(i, ex, ey);
  }
}

void ratio(TGraphAsymmErrors *gr1, TGraph *gr2)
{
  Double_t x, y, ex1, ex2, ey1, ey2;
  for(int i=0;i<gr1->GetN();i++) {
    gr1->GetPoint(i,x,y);
    ex1 = gr1->GetErrorXlow(i);
    ex2 = gr1->GetErrorXhigh(i);
    ey1 = gr1->GetErrorYlow(i);
    ey2 = gr1->GetErrorYhigh(i);
    y /= gr2->Eval(x);
    ey1 /= gr2->Eval(x);
    ey2 /= gr2->Eval(x);
    gr1->SetPoint(i,x,y);
    gr1->SetPointError(i, ex1, ex2, ey1, ey2);
  }
}

TH1D* histo(char *name, Double_t xlow, Double_t xup, Double_t ylow, Double_t yup, char* xTitle, char* yTitle){
  TH1D *dd = new TH1D(name,"",100,xlow,xup);
  dd->SetMinimum(ylow);
  dd->SetMaximum(yup);
  dd->GetXaxis()->SetTitle(xTitle);
  dd->GetYaxis()->SetTitle(yTitle);

  dd->GetXaxis()->SetTitleSize(0.055);
  dd->GetXaxis()->SetTitleOffset(0.9);
  dd->GetXaxis()->SetLabelSize(0.045);
  dd->GetYaxis()->SetTitleSize(0.055);
  dd->GetYaxis()->SetTitleOffset(1);
  dd->GetYaxis()->SetLabelSize(0.045);
  //dd->GetXaxis()->CenterTitle(kTRUE);
  //dd->GetYaxis()->CenterTitle(kTRUE);
  dd->GetXaxis()->SetNdivisions(512);
  return dd;
}

void drawLines(Double_t xlow,Double_t ylow, Double_t xup, Double_t yup, Double_t lineWidth, Int_t lineColor,Int_t linestyle){
  drawLine(xlow,ylow,xup,ylow,lineWidth,lineColor,linestyle);
  drawLine(xlow,yup,xup,yup,lineWidth,lineColor,linestyle);
  drawLine(xlow,ylow,xlow,yup,lineWidth,lineColor,linestyle);
  drawLine(xup,ylow,xup,yup,lineWidth,lineColor,linestyle);
}

void drawBoxes(Int_t n, Double_t* x1, Double_t* x2, Double_t* y1, Double_t* y2, Int_t lineWidth, Int_t lineColor){
  for(int i=0;i<n;i++){
    TBox *box = new TBox(x1[i],y1[i],x2[i],y2[i]);
    box->SetLineWidth(lineWidth);
    box->SetLineColor(lineColor);
    box->SetFillStyle(0);
    box->Draw("lsame");
  }
}

void CanvasPartition(TCanvas *C,const Int_t Nx,const Int_t Ny,
                     Float_t lMargin, Float_t rMargin,
                     Float_t bMargin, Float_t tMargin)
{
  if (!C) return;
  // Setup Pad layout:
  Float_t vSpacing = 0.0;
  Float_t vStep  = (1.- bMargin - tMargin - (Ny-1) * vSpacing) / Ny;
  Float_t hSpacing = 0.0;
  Float_t hStep  = (1.- lMargin - rMargin - (Nx-1) * hSpacing) / Nx;
  Float_t vposd,vposu,vmard,vmaru,vfactor;
  Float_t hposl,hposr,hmarl,hmarr,hfactor;
  for (Int_t i=0;i<Nx;i++) {
    if (i==0) {
      hposl = 0.0;
      hposr = lMargin + hStep;
      hfactor = hposr-hposl;
      hmarl = lMargin / hfactor;
      hmarr = 0.0;
    } else if (i == Nx-1) {
      hposl = hposr + hSpacing;
      hposr = hposl + hStep + rMargin;
      hfactor = hposr-hposl;
      hmarl = 0.0;
      hmarr = rMargin / (hposr-hposl);
    } else {
      hposl = hposr + hSpacing;
      hposr = hposl + hStep;
      hfactor = hposr-hposl;
      hmarl = 0.0;
      hmarr = 0.0;
    }
    for (Int_t j=0;j<Ny;j++) {
      if (j==0) {
	vposd = 0.0;
	vposu = bMargin + vStep;
	vfactor = vposu-vposd;
	vmard = bMargin / vfactor;
	vmaru = 0.0;
      } else if (j == Ny-1) {
	vposd = vposu + vSpacing;
	vposu = vposd + vStep + tMargin;
	vfactor = vposu-vposd;
	vmard = 0.0;
	vmaru = tMargin / (vposu-vposd);
      } else {
	vposd = vposu + vSpacing;
	vposu = vposd + vStep;
	vfactor = vposu-vposd;
	vmard = 0.0;
	vmaru = 0.0;
      }
      C->cd(0);
      char name[16];
      sprintf(name,"pad_%i_%i",i,j);
      TPad *pad = (TPad*) gROOT->FindObject(name);
      if (pad) delete pad;
      pad = new TPad(name,"",hposl,vposd,hposr,vposu);
      pad->SetLeftMargin(hmarl);
      pad->SetRightMargin(hmarr);
      pad->SetBottomMargin(vmard);
      pad->SetTopMargin(vmaru);
      pad->SetFrameBorderMode(0);
      pad->SetBorderMode(0);
      pad->SetBorderSize(0);
      pad->Draw();
    }
  }
}
