//===============================
//Version: 1.2 
//Time: Thu May  1 14:00:32 EDT 2014 
//Author: Long Zhou 
//Discribe: Create PDF file use update method  

//===============================
//Version: 1.1 
//Time: Thu Apr 24 23:54:39 EDT 2014 
//Author: Long Zhou 
//Discribe: 
//<1>This code is depend on Power Eff code.
//<2>removed third root file 

//---------------
// 2014-2-8
// version 1.0
//<1>copy pid_standard.C
//<2>used to plots adc histograms 
#include "TH1.h"
#include "TF1.h"
#include "TH3.h"
#include "TLegend.h"
#include "TCanvas.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <iomanip>
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
#include "TGraphErrors.h"
#include "TString.h"
#include "TPDF.h"
#include "TColor.h"
#define TxDefSize   0.05
using namespace std;
TLatex* drawLatex(Double_t x, Double_t y, char* text, Int_t textFont, Double_t texSize, Int_t colorIndexx,Int_t textAngle);
TLine* drawLine(Double_t xlow,Double_t ylow, Double_t xup, Double_t yup, Double_t lineWidth, Int_t lineColor,Int_t linestyle);
void savepic(TCanvas *c1,char *dir,char *name);
//-----------------------------------------------------
void plots(Int_t lowday=114,Int_t higday=121,char *Workdir="")
{
  TStopwatch SW;
  SW.Start();
  TH1::SetDefaultSumw2();
  const float Footx1 = 0.05; //Date                                                                                                                                    
  const float Footx2 = 0.43; //Author/Aff                                                                                                                              
  const float Footx3 = 0.96; //Pagenumber                                                                                                                              
  const float Footy  = 0.3;
  const float Footy2 = 0.2;
  TCanvas *cc = new TCanvas("cc","",0,0,800,600);
  gStyle->SetOptFit(1111);
  gStyle->SetOptStat(0);
  gStyle->SetEndErrorSize(0.01);
  gStyle->SetPalette(1,0);
  gStyle->SetOptLogz(1);
  gStyle->SetOptTitle(0);
  gStyle->SetPalette(1,0);
  cc->SetFillColor(10);
  cc->SetBorderMode(0);
  cc->SetBorderSize(2);
  cc->SetFrameFillColor(0);
  cc->SetFrameBorderMode(0);
  //cc->SetLogy();                                                                                          
  cc->SetGrid(1);
  cc->SetLeftMargin(0.089);
  cc->SetBottomMargin(0.15);
  cc->SetTopMargin(0.025);
  //cc->SetRightMargin(0.025);
  cc->SetRightMargin(0.1); 
  cc->SetLogz(1);
  //cc->Close();
  //-----------------------------  
  float correct=0;
  char *dir="pic";
  TPDF *pdf = new TPDF("pedestal.time.pdf");  
  TFile *f1 = new TFile("pedestal.time.root");
  
  //------------------------------------
  //Run argument 
  char *Trigger="Pedestal_tcd_only";
  char *TriggerRate="100hz";
  char *InternalBusy="14000";
  //char *Workdir="/star/u/zhoulong/data02/DEV14/SST/Firmware";
  //------------------------------------
  
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
  tx.DrawLatex(0.05,0.3,"Pedestal stability");

  padFoot->cd();
  tx.DrawLatex(Footx2, Footy, "Long Zhou/BNL&USTC");
  TString datime  = (new TDatime())->AsSQLString();
  TString date = datime(0,10);
  tx.DrawLatex(Footx1, Footy, date.Data());

  pad->cd();
  tx.SetTextColor(1);
  //tx.SetTextSize(TxDefSize);

  tx.SetTextSize(0.06);
  char tlName[100];
  tx.DrawLatex(0.03, 0.85,"Data : Regular pedestal run");
  tx.DrawLatex(0.03, 0.75,"Base line : Day 114");
  tx.DrawLatex(0.03, 0.65,Form("Trigger : %s(All)",Trigger));
  tx.DrawLatex(0.03, 0.55,"Power supply: 2.5V");
  //  tx.SetTextSize(0.05);

  tx.DrawLatex(0.03, 0.45,"Power setting: New 50 meter setting(All)");
  tx.DrawLatex(0.03, 0.35,"Trigger Rate : Default(All)");
  tx.DrawLatex(0.03,0.25,Form("Internal Busy : %s(All)",InternalBusy));
  tx.SetTextSize(0.05);
  tx.DrawLatex(0.03,0.15,Form("Work dir: %s",Workdir));
  float ty1 = 0.58;
  float ty2 = 0.44;
  float dy  = 0.06;
  cc->cd();
  cc->Update();
  
  //------------------------------------ 
  TH2I  *hPedStable[2][20];
  //----------------------------
  for(int j=0;j<20;j++)
    {
      cout<<"ladder "<<j<<endl;
      for(int i=0;i<2;i++)
	{
	  //pdf->NewPage();
	  hPedStable[i][j] = (TH2I *)f1->Get(Form("hPedStable_%d_%d",i,j));
	  hPedStable[i][j]->SetName(Form("hPedStable1_%d_%d",i,j));
	  hPedStable[i][j]->GetXaxis()->SetRangeUser(lowday-5,higday+5);
	  hPedStable[i][j]->DrawCopy("COLZ");
	  drawLatex(0.7,0.975,Form("Side %d Ladder %d",i,j),62,0.03,2,0);
	  ////drawLine(114,0,114,96,0.04,2,1);
	  ////drawLatex(0.435,0.7,"Base line",62,0.03,2,270);
          cc->cd();
	  cc->Update();
	  if(j==19&&i==1)
	    pdf->Close();
	  //else 
	  //  pdf->NewPage();
	  //cc->Clear();
	  hPedStable[i][j]->Delete();
	}
    }
 
  SW.Stop();
  SW.Print();
}
void savepic(TCanvas *c1,char *dir,char *name)
{
  char temp[1024];
  sprintf(temp,"%s/%s.gif",dir,name);
  c1->SaveAs(temp);
  c1->Clear();
  c1->cd();
}

TLatex* drawLatex(Double_t x, Double_t y, char* text, Int_t textFont, Double_t textSize, Int_t colorIndex,Int_t textAngle){
  TLatex *latex = new TLatex(x,y,text);
  latex->SetNDC();
  latex->SetTextFont(textFont);
  latex->SetTextSize(textSize);
  latex->SetTextColor(colorIndex);
  latex->SetTextAngle(textAngle);
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

