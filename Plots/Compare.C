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
void Compare_ds(TString file0="",TString file1="",TString file2="",TString file3="")
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
  gPad->SetGrid(0,0);

  TH2D *h2 = new TH2D("h2","",10,0,10,40,0,0.040) ;
  h2->GetXaxis()->SetTitle("p_{T} [GeV/c]");
  h2->GetYaxis()->SetTitle("Width [GeV]");
  h2->Draw();

  TFile *ftmp;

  if(!file0.IsNull()) {
  ftmp = new TFile(Form("%s",file0.Data()));
  if(ftmp&&(ftmp->IsOpen())&&(ftmp->GetNkeys())&&!ftmp->IsZombie())
    {
      TIter nextkey(ftmp->GetListOfKeys());
      TObject *t;
      TKey *key;
      while ((key = (TKey*)nextkey())) {
	t = dynamic_cast<TObject*>(key->ReadObj());
	if(t){
	  t->Print();
	  TGraphAsymmErrors *g1 = (TGraphAsymmErrors *)t;
	  g1->SetMarkerStyle(20);
	  g1->SetMarkerColor(4);
	  g1->SetLineColor(4);
	  t->Draw("sameP");
	}
      }
    }
  }

  ftmp->Close();
  if(!file1.IsNull()) {
  ftmp = new TFile(Form("%s",file1.Data()));
  if(ftmp&&(ftmp->IsOpen())&&(ftmp->GetNkeys())&&!ftmp->IsZombie())
    {
      TIter nextkey(ftmp->GetListOfKeys());
      TObject *t;
      TKey *key;
      while ((key = (TKey*)nextkey())) {
	t = dynamic_cast<TObject*>(key->ReadObj());
	if(t){
	  t->Print();
	  TGraphAsymmErrors *g1 = (TGraphAsymmErrors *)t;
	  g1->SetMarkerStyle(21);
	  g1->SetMarkerColor(4);
	  g1->SetLineColor(4);
	  t->Draw("sameP");
	}
      }
    }
  }

  if(!file2.IsNull()) {
  ftmp = new TFile(Form("%s",file2.Data()));
  if(ftmp&&(ftmp->IsOpen())&&(ftmp->GetNkeys())&&!ftmp->IsZombie())
    {
      TIter nextkey(ftmp->GetListOfKeys());
      TObject *t;
      TKey *key;
      while ((key = (TKey*)nextkey())) {
	t = dynamic_cast<TObject*>(key->ReadObj());
	if(t){
	  t->Print();
	  TGraphAsymmErrors *g1 = (TGraphAsymmErrors *)t;
	  g1->SetMarkerStyle(22);
	  g1->SetMarkerColor(4);
	  g1->SetLineColor(4);
	  t->Draw("sameP");
	}
      }
    }
  }

  if(!file3.IsNull()) {
  ftmp = new TFile(Form("%s",file3.Data()));
  if(ftmp&&(ftmp->IsOpen())&&(ftmp->GetNkeys())&&!ftmp->IsZombie())
    {
      TIter nextkey(ftmp->GetListOfKeys());
      TObject *t;
      TKey *key;
      while ((key = (TKey*)nextkey())) {
	t = dynamic_cast<TObject*>(key->ReadObj());
	if(t){
	  t->Print();
	  TGraphAsymmErrors *g1 = (TGraphAsymmErrors *)t;
	  g1->SetMarkerStyle(23);
	  g1->SetMarkerColor(4);
	  g1->SetLineColor(4);
	  t->Draw("sameP");
	}
      }
    }
  }
  // TFile *ftmp = new TFile(Form("Dswidth_toymc_ch1.root"));
  // TIter nextkey1(ftmp->GetListOfKeys());
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

  // if(setnum=="ch0") {
  //   drawLatex(0.65,0.88,Form("D_{s}#rightarrow#phi(1020)#pi"),62,0.04,4);
  // }
  // else drawLatex(0.65,0.88,Form("D_{s}#rightarrowk#bar{k^{*0}}(892)"),62,0.04,4);
  // cc1->SaveAs(Form("pic/Dswidth_data_vs_toymc_%s.gif",setnum.Data()));

  cc1->SaveAs(Form("pic/%s_%s.gif",file0.Data(),file1.Data()));
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
