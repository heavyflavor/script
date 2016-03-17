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
#define TxDefSize   0.05
using namespace std;
TLatex* drawLatex(Double_t x, Double_t y, char* text, Int_t textFont, Double_t texSize, Int_t colorIndex);
TLine* drawLine(Double_t xlow,Double_t ylow, Double_t xup, Double_t yup, Double_t lineWidth, Int_t lineColor,Int_t linestyle);
void savepic(TCanvas *c1,char *dir,char *name);
//-----------------------------------------------------
void plots(TString input,int runnumber,char *RdoVersion="0023",char *Trigger="PedAdPhys_tcd_only",char *A128Set="#3",char *TcdDelay="0",char *Workdir="data02/DEV14/SST/offline_02_24")
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
  cc->SetLeftMargin(0.12);
  cc->SetBottomMargin(0.15);
  cc->SetTopMargin(0.025);
  cc->SetRightMargin(0.025); 
  cc->SetLogz(1);
  //cc->Close();
  //-----------------------------  
  float correct=0;
  //ofstream outdata(Form("%s.txt",filename));
  //filename += ".adc.root";
  TString output = input;
  output += ".pdf";
  char dir="pic";
  TPDF *pdf = new TPDF(output);  
  input += ".out.root";
  TFile *f1 = new TFile(input);
  //------------------------------------
  //Run argument 
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
  tx.DrawLatex(0.05,0.3,Form("Run %d",runnumber));

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
  tx.DrawLatex(0.1, 0.8,Form("File Name : %s",input.Data()));
  tx.DrawLatex(0.1, 0.7,Form("Trigger : %s",Trigger));
  sprintf(tlName, "RDO Version: %s",RdoVersion);
  tx.DrawLatex(0.1, 0.6, tlName);
  //  tx.SetTextSize(0.05);
  tx.DrawLatex(0.1, 0.5,Form("Alice 128 setting : %s",A128Set));
  tx.DrawLatex(0.1, 0.4,Form("Tcd Delay : %s #mus",TcdDelay));
  tx.DrawLatex(0.1,0.3,Form("Work dir: %s",Workdir));
  tx.DrawLatex(0.1,0.2,"Wafer No.: 1");
  tx.DrawLatex(0.1,0.1,"Strip No.: 4");
  float ty1 = 0.58;
  float ty2 = 0.44;
  float dy  = 0.06;
  /*
  tx.SetTextSize(0.05);
  tx.DrawLatex(0.15, ty1, "Event Cuts:");
  tx.SetTextSize(0.045);
  tx.DrawLatex(0.2, ty1-dy, "|V_{Z}| < 50 cm, |#Delta V_{Z}| < 3 cm");
  tx.SetTextSize(0.05);
  tx.DrawLatex(0.15, ty2, "Track Cuts:");
  tx.SetTextSize(0.045);
  tx.DrawLatex(0.2, ty2-dy, "Primary tracks");
  tx.DrawLatex(0.2, ty2-dy*2, "p_{T} > 0.2 GeV, |#eta| < 1");
  tx.DrawLatex(0.2, ty2-dy*3, "nHits > 15, nHits/nPoss > 0.52");
  tx.DrawLatex(0.2, ty2-dy*4, "gDca < 2.0 cm");

  tx.SetTextColor(kBlack);
  tx.SetTextSize(0.04);
//  tx.DrawLatex(0.1, 0.93, "/eliza15/star/starprod/embedding/2010ProductionMinBias/PiPlus_100_2011601");
//  tx.DrawLatex(0.1,0.93,
  tx.DrawLatex(0.1,0.94,"/eliza15/star/starprod/hpss/staging/embedding/AuAu200_production/");
  tx.DrawLatex(0.1,0.89,"PiPlus_AuAu200GeV_NFit_Test");
  */

  cc->cd();
  cc->Update();
  pdf->NewPage();

  //------------------------------------ 
  
  TH2F  *AdcTime[2][20];
  TH2F  *AdcTime1[2][20];
  for(int i=0;i<2;i++)
    {
      for(int j=0;j<20;j++)
	{
	  AdcTime[i][j] = (TH2F *)f1->Get(Form("AdcTime_%d_%d",i,j));
	  AdcTime[i][j]->SetName(Form("AdcTime_%d_%d",i,j));
	  AdcTime1[i][j] = (TH2F *)f1->Get(Form("AdcTime1_%d_%d",i,j));
	  AdcTime1[i][j]->SetName(Form("AdcTime1_%d_%d",i,j));
	}
    }
  for(int j=0;j<20;j++)
    {
      cc->Divide(2,2);
      for(int i=0;i<2;i++)
	{
	 
	  if(i==0) 
	    {
	      cc->cd(1);
	      //AdcTime[i][j]->SetMarkerStyle(20);
	      //AdcTime[i][j]->SetMarkerColor(2);
	      AdcTime[i][j]->Draw("COLZ");
	      //AdcTime[i][j]->Draw();
      
	      cc->cd(2);
	      //AdcTime1[i][j]->SetMarkerStyle(20);
	      //AdcTime1[i][j]->SetMarkerColor(4);
	      AdcTime1[i][j]->Draw("COLZ");
	      //AdcTime[i][j]->Draw();
	    }
	  else 
	    {
	      cc->cd(3);
	      //AdcTime[i][j]->SetMarkerStyle(20);
	      //AdcTime[i][j]->SetMarkerColor(2);
	      AdcTime[i][j]->Draw("COLZ");
	      //AdcTime[i][j]->Draw();
      
	      cc->cd(4);
	      //AdcTime1[i][j]->SetMarkerStyle(20);
	      //AdcTime1[i][j]->SetMarkerColor(4);
	      AdcTime1[i][j]->Draw("COLZ");
	      //AdcTime[i][j]->Draw();
	    }
	 
	  if(i==0) drawLatex(0.6,0.95,Form("Ladder #%d P-side",j),62,0.06,2);
	  if(i==1) drawLatex(0.6,0.95,Form("Ladder #%d N-side",j),62,0.06,4);
        }
      cc->cd();
      cc->Update();
      if(j<19)
	pdf->NewPage();
      else pdf->Close();
      //savepic(cc,dir,Form("AdcTime_%d",j));
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

