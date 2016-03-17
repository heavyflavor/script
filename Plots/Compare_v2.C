#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "stdio.h"
#include <iostream>
#include "TLine.h"
#include "TLatex.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "TString.h"
#include "TLegend.h"
#include "TSystem.h"
//#include "mDs.h"
#include "TMath.h"
#include "TStopwatch.h"
using namespace std;
TLatex* drawLatex(Double_t x, Double_t y, char* text, Int_t textFont, Double_t texSize, Int_t colorIndex);
void Compare(TString setnum="ch1")
{
  // All data points are from HaoQiu  -- Many Thanks @!

  TStopwatch ST;
  ST.Start();  
  TCanvas *cc1 = new TCanvas("cc1", "cc1",0,0,800,600);
  gStyle->SetOptFit(0);
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  gStyle->SetEndErrorSize(0.01);
  gStyle->SetOptDate(0);
  cc1->SetFillColor(10);
  cc1->SetBorderMode(0);
  cc1->SetBorderSize(2);
  cc1->SetFrameFillColor(0);
  cc1->SetFrameBorderMode(0);
  //cc1->SetLogy();
  cc1->SetGridx();
  cc1->SetGridy();
  cc1->SetLeftMargin(0.12);
  cc1->SetBottomMargin(0.15);
  cc1->SetTopMargin(0.025);
  cc1->SetRightMargin(0.025);
  cc1->cd(); 
  // cc1->Divide(2,1);		// Divide(lie,hang)
  gPad->SetGrid(0,0);

  TH2D *hsigma = new TH2D("hsigma","",10,0,10,50,0,0.050) ;
  hsigma->GetXaxis()->SetTitle("p_{T} [GeV/c]");
  hsigma->GetYaxis()->SetTitle("#sigma [GeV]");
  hsigma->Draw();

  // D0 width from Data 
  // ------------------
  TFile *fin1 = new TFile("D0width.root");
  TGraphAsymmErrors *D0width = (TGraphAsymmErrors *)fin1->Get("d0ptspt_data");
  D0width->SetMarkerColor(2);
  D0width->SetLineColor(2);
  D0width->Draw("sameP");

  // Toy MC simulation 
  // ------------------
  TFile *fin2 = new TFile(Form("Dswidth_toymc_%s.root",setnum.Data()));
  TIter nextkey(fin2->GetListOfKeys());
  TObject *t;
  TKey *key;
  TString tName;

  while ((key = (TKey*)nextkey())) {
    t = dynamic_cast<TObject*>(key->ReadObj());
    if(t){
      t->Print();
      TGraphAsymmErrors *g1 = (TGraphAsymmErrors *)t;
      g1->SetMarkerStyle(24);
      g1->SetMarkerColor(4);
      g1->SetLineColor(4);
      t->Draw("sameP");
    }
  }

  // Read mean, width and yield from Data
  // -------------------------------------
  TFile *fin3 = new TFile(Form("Ds_%s_Hist.root",setnum.Data()));
  TIter nextkey3(fin3->GetListOfKeys());

  // Ds sigma from Data
  while ((key = (TKey*)nextkey3())) {
    t = dynamic_cast<TObject*>(key->ReadObj());
    if(t){
      t->Print();
      tName = t->GetName();
      if(tName.Contains("sigma")){
	TGraphAsymmErrors *g1 = (TGraphAsymmErrors *)t;
	g1->SetMarkerStyle(21);
	if(tName.Contains("20_80")) {
	  g1->SetMarkerColor(3);
	  g1->SetLineColor(3);
	  t->Draw("sameP");
	}
	else if(tName.Contains("0_80")){
	  g1->SetMarkerColor(6);
	  g1->SetLineColor(6);
	  t->Draw("sameP");
	}
	else {
	  g1->SetMarkerColor(7);
	  g1->SetLineColor(7);
	  t->Draw("sameP");
	}
      }
    }
  }

  //  Ds mean from Data 
  TH2D *hmean = new TH2D("hmean","",10,0,10,40,0,0.040) ;
  hmean->GetXaxis()->SetTitle("p_{T} [GeV/c]");
  hmean->GetYaxis()->SetTitle("<M> - PDG [MeV/c^{2}]");
  // hmean->Draw();
  while ((key = (TKey*)nextkey3())) {
    t = dynamic_cast<TObject*>(key->ReadObj());
    if(t){
      t->Print();
      tName = t->GetName();
      if(tName.Contains("mean")) {
	TGraphAsymmErrors *g1 = (TGraphAsymmErrors *)t;
	g1->SetMarkerColor(4);
	g1->SetLineColor(4);
	// t->Draw("sameP");
      }
    }
  }

  // Ds yield from Data
  TH2D *hyield = new TH2D("hyield","",10,0,10,40,0,0.040) ;
  hyield->GetXaxis()->SetTitle("p_{T} [GeV/c]");
  hyield->GetYaxis()->SetTitle("<M> - PDG [MeV/c^{2}]");
  // hyield->Draw();
  while ((key = (TKey*)nextkey3())) {
    t = dynamic_cast<TObject*>(key->ReadObj());
    if(t){
      t->Print();
      tName = t->GetName();
      if(tName.Contains("yield")) {
	TGraphAsymmErrors *g1 = (TGraphAsymmErrors *)t;
	g1->SetMarkerColor(4);
	g1->SetLineColor(4);
	// t->Draw("sameP");
      }
    }
  }

  // TFile *fin3 = new TFile(Form("Dswidth_toymc_ch1.root"));
  // TIter nextkey1(fin3->GetListOfKeys());
  // TObject *t1;
  // TKey *key1;
  // while ((key1 = (TKey*)nextkey1())) {
  //   t1 = dynamic_cast<TObject*>(key1->ReadObj());
  //   if(t1){
  //     t1->Print();
  //     TGraphAsymmErrors *g2 = (TGraphAsymmErrors *)t1;
  //     g2->SetMarkerColor(2);
  //     g2->SetLineColor(2);
  //     t1->Draw("sameP");
  //   }
  // }

  // drawLatex(0.65,0.88,Form("D_{s}#rightarrow#phi(1020)#pi"),62,0.04,4);
  // drawLatex(0.65,0.83,Form("D_{s}#rightarrowk#bar{k^{*0}}(892)"),62,0.04,2);
  drawLatex(0.65,0.88,Form("D_{0}#rightarrowk#pi[Run14]"),62,0.04,2);
  if(setnum=="ch1") {
    drawLatex(0.65,0.83,Form("D_{s}#rightarrow#phi(1020)#pi[ToyMC]"),62,0.04,4);
    drawLatex(0.65,0.78,Form("D_{s}#rightarrow#phi(1020)#pi[0-80%]"),62,0.04,6);
    drawLatex(0.65,0.73,Form("D_{s}#rightarrow#phi(1020)#pi[0-20%]"),62,0.04,7);
    drawLatex(0.65,0.68,Form("D_{s}#rightarrow#phi(1020)#pi[20-80%]"),62,0.04,3);
  }
  if(setnum=="ch2") {
    drawLatex(0.65,0.83,Form("D_{s}#rightarrowk#bar{k^{*0}}(892)[ToyMC]"),62,0.04,4);
    drawLatex(0.65,0.78,Form("D_{s}#rightarrowk#bar{k^{*0}}(892)[0-80%]"),62,0.04,6);
    drawLatex(0.65,0.73,Form("D_{s}#rightarrowk#bar{k^{*0}}(892)[0-20%]"),62,0.04,7);
    drawLatex(0.65,0.68,Form("D_{s}#rightarrowk#bar{k^{*0}}(892)[20-80%]"),62,0.04,3);
  }

  gSystem->Exec("mkdir -p pic");
  cc1->SaveAs(Form("pic/Dswidth_data_vs_toymc_%s.gif",setnum.Data()));
  cout<<"end of program"<<endl;
  ST.Stop();
  ST.Print();
  return;
}
//----------------------------------------------
TLatex* drawLatex(Double_t x, Double_t y, char* text, Int_t textFont, Double_t textSize, Int_t colorIndex){
  TLatex *latex = new TLatex(x,y,text);
  latex->SetNDC();
  latex->SetTextFont(textFont);
  latex->SetTextSize(textSize);
  latex->SetTextColor(colorIndex);
  latex->Draw("same");
  return latex;
}
