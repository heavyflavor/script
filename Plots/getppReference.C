//===============================
//Version: 1.2 
//Time: Mon Jun 16 13:33:56 PDT 2014 
//Author: Long Zhou 
//Discribe: Add Data/FONLL in pp500 

#include <stdio>
#include <stdlib>
#include <iostream>
#include <fstream>
#include <math.h>
#include "iomanip.h"
#include "TH1.h"
#include "TMath.h"
#include "TF1.h"
#include "TLegend.h"
#include <stdlib.h>
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
#include "TVector2.h"
#include "TVector3.h"
#include "TLorentzVector.h"
#include "TSystem.h"
#include "TUnixSystem.h"
#include "TRandom3.h"
#include "TStopwatch.h"
#include "TDirectoryFile.h"
#include "TDirectory.h"
TLine* drawLine(Double_t xlow,Double_t ylow, Double_t xup, Double_t yup, Double_t lineWidth, Int_t lineColor,Int_t linestyle);
TLatex* drawLatex(Double_t x, Double_t y, char* text, Int_t textFont, Double_t textSize, Int_t colorIndex);
void setpad(TPad *pad,float left, float right, float top, float bottom);
void savepic(TCanvas *c1,char *dir,char *name);
void getppReference(TString input="ppReference.root")
{
  TCanvas *cc = new TCanvas("cc", "cc",0,0,1600,800);
  gStyle->SetOptFit(0);
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  gStyle->SetEndErrorSize(0.01);
  cc->SetFillColor(10);
  cc->SetBorderMode(0);
  cc->SetBorderSize(2);
  cc->SetFrameFillColor(0);
  cc->SetFrameBorderMode(0);
  //cc->SetLogy();
  cc->SetGridx(0);
  cc->SetGridy(0);
  cc->SetTicks(1,1);
  cc->SetLeftMargin(0.18);
  cc->SetBottomMargin(0.15);
  cc->SetTopMargin(0.025);
  cc->SetRightMargin(0.025);

  char latex[1024];
  char line[101];
  TFile *f = new TFile(input);
  TDirectory *pp200 = (TDirectory *)f->Get("run12");
  TDirectory *pp500 = (TDirectory *)f->Get("pp500");
  //-----------------
  //pp500GeV
  TGraphErrors *gHT_500     = (TGraphErrors *)pp500->Get("gHT"); 
  TGraphErrors *gMB_500     = (TGraphErrors *)pp500->Get("gMB");
  TGraphErrors *gD0_500     = (TGraphErrors *)pp500->Get("gD0");
  TF1 *flevy500             = (TF1 *)pp500->Get("func2");
  TGraph *grshade           = (TGraph *)pp500->Get("grshade");
  TGraph *grshade127        = (TGraph *)pp500->Get("grshade127");
  TGraph *gr_FONLL500_mid   = (TGraph *)pp500->Get("gr_FONLL500_mid");
  TGraph *gr_FONLL500_upper = (TGraph *)pp500->Get("gr_FONLL500_upper");
  TGraph *gr_FONLL500_lower = (TGraph *)pp500->Get("gr_FONLL500_lower");
  TGraph *gr_FONLL500m_mid   = (TGraph *)pp500->Get("gr_FONLL500m_mid");
  TGraph *gr_FONLL500m_upper = (TGraph *)pp500->Get("gr_FONLL500m_upper");
  TGraph *gr_FONLL500m_lower = (TGraph *)pp500->Get("gr_FONLL500m_lower");
  gr_FONLL500_mid->SetMarkerStyle(7);
  gr_FONLL500_mid->SetMarkerColor(kGray);
  gr_FONLL500_lower->SetMarkerStyle(7);
  gr_FONLL500_mid->SetMarkerColor(kGray);
  gr_FONLL500_upper->SetMarkerStyle(7);
  gr_FONLL500_mid->SetMarkerColor(kGray);
  gr_FONLL500m_mid->SetMarkerStyle(7);
  gr_FONLL500m_mid->SetMarkerColor(kAzure);
  gr_FONLL500m_lower->SetMarkerStyle(7);
  gr_FONLL500m_lower->SetMarkerColor(kAzure);
  gr_FONLL500m_upper->SetMarkerStyle(7);
  gr_FONLL500m_upper->SetMarkerColor(kAzure);
  //-----------------
  //pp200GeV
  TGraphErrors *gD0_200         = (TGraphErrors *)pp200->Get("grD0"); 
  TGraphErrors *gD0SE_200       = (TGraphErrors *)pp200->Get("grD0Sys");
  TGraphErrors *gDstar_200      = (TGraphErrors *)pp200->Get("grDstar");
  TGraphErrors *gDstarSE_200    = (TGraphErrors *)pp200->Get("grDstarSys");
  TGraphErrors *gDstarHT2_200   = (TGraphErrors *)pp200->Get("grDstarHT2");
  TGraphErrors *gDstarHT2SE_200 = (TGraphErrors *)pp200->Get("grDstarHT2Sys");
  TF1 *flevy200                 = (TF1 *)pp200->Get("f1Levy");
  TGraph *gr_FONLL_u            = (TGraph *)f->Get("gr_FONLL_u");
  TGraph *gr_FONLL_d            = (TGraph *)f->Get("gr_FONLL_d");
  gr_FONLL_u->SetMarkerStyle(7);
  gr_FONLL_d->SetMarkerStyle(7);
  Int_t NPoint = gr_FONLL_u->GetN();
  TGraph *pp200shade = new TGraph(2*NPoint);

  for(int i=0;i<NPoint;i++)
    { 
      float xu = gr_FONLL_u->GetX()[i];
      float yu = gr_FONLL_u->GetY()[i];      
      float xd = gr_FONLL_d->GetX()[NPoint-i-1];
      float yd = gr_FONLL_d->GetY()[NPoint-i-1];
      pp200shade->SetPoint(i,xu,yu);
      pp200shade->SetPoint(NPoint+i,xd,yd);
    }
  pp200shade->SetFillColor(16);
  //----------------
  cc->cd();
  cc->Divide(2,1);
  cc_1->cd();
  cc_1->SetLogy(1);
  cc_1->SetGridx(0);
  cc_1->SetGridy(0);
  //cc_1->SetTicks(1,1);
  setpad(cc_1,0.125,0.02,0.02,0.10);
  TH2D *h1 = new TH2D("h1","pp Reference;p_{T} [GeV/c];d^{2}#sigma^{pp}_{c#bar{c}})/(2#pip_{T}dp_{T}dy) [mb(GeV/c)^{-2}]",100,0,10,100,1e-8,1e-1);
  h1->GetYaxis()->SetTitleOffset(1.5);
  h1->Draw();
  flevy200->SetLineStyle(7);
  flevy200->SetLineWidth(2);
  //flevy200->GetYaxis()->SetTitle("d^{2}#sigma^{pp}_{c#bar{c}})/(2#pip_{T}dp_{T}dy) [mb(GeV/c)^{-2}]");
  //flevy200->GetXaxis()->SetTitle("p_{T} [GeV/c]");

  //-------------------------------------
  //pp200 combine sys error and sta error
  for(int i=0;i<gD0_200->GetN();i++)
    { 
       float x = gD0_200->GetX()[i];
      float y = gD0_200->GetY()[i];      
      float ex = gD0_200->GetEX()[i];
      float ey = gD0_200->GetEY()[i];
      float sey = gD0SE_200->GetEY()[i];
      float errorY = sqrt(pow(ey,2)+pow(sey,2));
      gD0_200->SetPoint(i,x,y);
      gD0_200->SetPointError(i,ex,errorY);
    }

  for(int i=0;i<gDstar_200->GetN();i++)
    { 
       float x = gDstar_200->GetX()[i];
      float y = gDstar_200->GetY()[i];      
      float ex = gDstar_200->GetEX()[i];
      float ey = gDstar_200->GetEY()[i];
      float sey = gDstarSE_200->GetEY()[i];
      float errorY = sqrt(pow(ey,2)+pow(sey,2));
      gDstar_200->SetPoint(i,x,y);
      gDstar_200->SetPointError(i,ex,errorY);
    }

  for(int i=0;i<gDstarHT2_200->GetN();i++)
    { 
      float x = gDstarHT2_200->GetX()[i];
      float y = gDstarHT2_200->GetY()[i];      
      float ex = gDstarHT2_200->GetEX()[i];
      float ey = gDstarHT2_200->GetEY()[i];
      float sey = gDstarHT2SE_200->GetEY()[i];
      float errorY = sqrt(pow(ey,2)+pow(sey,2));
      gDstarHT2_200->SetPoint(i,x,y);
      gDstarHT2_200->SetPointError(i,ex,errorY);
    }

  //-----------------------
  //Start Draw those graphs
  //-----------------------
 
  flevy200->Draw("same");
  gD0_200->Draw("psame");
  gD0SE_200->Draw("psame");
  gDstar_200->Draw("psame");
  gDstarSE_200->Draw("psame");
  gDstarHT2_200->SetMarkerStyle(30);
  gDstarHT2_200->SetMarkerColor(2);
  gDstarHT2_200->SetLineColor(2);
  //gDstarHT2_200->SetFillColor(0);
  gDstarHT2_200->Draw("psame");
  gDstarHT2SE_200->SetMarkerStyle(30);
  gDstarHT2SE_200->SetMarkerColor(2);
  gDstarHT2SE_200->SetLineColor(2);
  gDstarHT2SE_200->Draw("psame");

  flevy500->SetLineColor(4);
  flevy500->SetLineStyle(7);
  flevy500->Draw("same");
  gHT_500->SetMarkerColor(4);
  gHT_500->SetLineColor(4);
  gHT_500->Draw("psame");
  gMB_500->SetMarkerColor(4);
  gMB_500->SetLineColor(4);
  gMB_500->Draw("psame");
  gD0_500->SetMarkerColor(4);
  gD0_500->SetLineColor(4);
  gD0_500->Draw("psame");

  TLegend *leg200 = new TLegend(0.14,0.2,0.45,0.35);
  leg200->SetFillColor(10);
  leg200->SetTextFont(42);
  leg200->AddEntry(gDstarHT2_200,"d#sigma_{D*} / 0.224 - BHT2","pl");
  leg200->AddEntry(gDstar_200,"d#sigma_{D*} / 0.224 - MB","pl");
  leg200->AddEntry(gD0_200,"d#sigma_{D^{0}} / 0.565 - MB","pl");
  leg200->AddEntry(flevy200,"Levi-fit to All Data", "l");
  leg200->SetTextSize(0.03);
  leg200->SetBorderSize(0);
  leg200->SetFillStyle(0);
  leg200->Draw();
  sprintf(latex,"Run12 pp200GeV");
  drawLatex(0.14,0.36,latex,62,0.04,1); 
  TLegend *leg500 = new TLegend(0.65,0.77,0.95,0.93);
  leg500->SetFillColor(10);
  leg500->SetTextFont(42);
  leg500->AddEntry(gHT_500,"d#sigma_{D*} / 0.224 - BHT1","pl");
  leg500->AddEntry(gMB_500,"d#sigma_{D*} / 0.224 - MB","pl");
  leg500->AddEntry(gD0_500,"d#sigma_{D^{0}} / 0.565 - MB","pl");
  leg500->AddEntry(flevy500,"Levi-fit to All Data", "l");
  leg500->SetTextSize(0.03);
  leg500->SetBorderSize(0);
  leg500->SetFillStyle(0);
  leg500->Draw();
  sprintf(latex,"Run12 pp500GeV");
  drawLatex(0.65,0.94,latex,62,0.04,1);

  //--------------------
  //Scale pp500 to pp200
  //--------------------

  cc_2->cd();
  cc_2->SetLogy(1);
  cc_2->SetGridx(0);
  cc_2->SetGridy(0);
  //cc_2->SetTicks(1,1);
  setpad(cc_2,0.125,0.02,0.02,0.10);
  TH2D *h2 = new TH2D("h2","pp Reference;p_{T} [GeV/c];d^{2}#sigma^{pp}_{c#bar{c}})/(2#pip_{T}dp_{T}dy) [mb(GeV/c)^{-2}]",100,0,10,100,1e-8,1e-1);
  h2->GetYaxis()->SetTitleOffset(1.5);
  h2->Draw();
  ////flevy200->Draw("same");
  gD0_200->Draw("psame");
  gD0SE_200->Draw("psame");
  gDstar_200->Draw("psame");
  gDstarSE_200->Draw("psame");
  gDstarHT2_200->Draw("psame");
  gDstarHT2SE_200->Draw("psame");
  //flevy500->Draw("same");
  //gHT_500->Draw("psame");
  //gMB_500->Draw("psame");
  //gD0_500->Draw("psame");
  TGraphErrors *gD0_500_1 = (TGraphErrors *)gD0_500->Clone();
  TGraphErrors *gMB_500_1 = (TGraphErrors *)gMB_500->Clone();
  TGraphErrors *gHT_500_1 = (TGraphErrors *)gHT_500->Clone();
  for(int i=0;i<gD0_500_1->GetN();i++)
    { 
       float x = gD0_500_1->GetX()[i];
      float y = gD0_500_1->GetY()[i];      
      float  ex = gD0_500_1->GetEX()[i];
      float ey = gD0_500_1->GetEY()[i];
      float scalef  =flevy200(x)/flevy500(x);
      y = scalef*y;
      gD0_500_1->SetPoint(i,x,y);
      gD0_500_1->SetPointError(i,ex,ey*scalef);
    }
  gD0_500_1->Draw("psame");

  for(int i=0;i<5;i++)
    { 
      float x = gHT_500_1->GetX()[i];
      float y = gHT_500_1->GetY()[i];      
      float  ex = gHT_500_1->GetEX()[i];
      float ey = gHT_500_1->GetEY()[i];
      float scalef = flevy200(x)/flevy500(x);
      y = scalef*y;
      gHT_500_1->SetPoint(i,x,y);
      gHT_500_1->SetPointError(i,ex,ey*scalef);
    }
  gHT_500_1->Draw("psame");

  for(int i=0;i<gMB_500->GetN();i++)
    { 
      float x = gMB_500_1->GetX()[i];
      float y = gMB_500_1->GetY()[i];      
      float  ex = gMB_500_1->GetEX()[i];
      float ey = gMB_500_1->GetEY()[i];
      float scalef = flevy200(x)/flevy500(x);
      y = scalef*y;
      gMB_500_1->SetPoint(i,x,y);
      gMB_500_1->SetPointError(i,ex,ey*scalef);
    }
  gMB_500_1->Draw("psame");
  
  TLegend *leg200_1 = new TLegend(0.14,0.2,0.45,0.35);
  leg200_1->SetFillColor(10);
  leg200_1->SetTextFont(42);
  leg200_1->AddEntry(gDstarHT2_200,"d#sigma_{D*} / 0.224 - BHT2","pl");
  leg200_1->AddEntry(gDstar_200,"d#sigma_{D*} / 0.224 - MB","pl");
  leg200_1->AddEntry(gD0_200,"d#sigma_{D^{0}} / 0.565 - MB","pl");
  leg200_1->AddEntry(flevy200,"Levi-fit to pp 200GeV Data", "l");
  leg200_1->SetTextSize(0.03);
  leg200_1->SetBorderSize(0);
  leg200_1->SetFillStyle(0);
  leg200_1->Draw();
  sprintf(latex,"Run12 pp200GeV");
  drawLatex(0.14,0.36,latex,62,0.04,1); 
  TLegend *leg500_1 = new TLegend(0.65,0.77,0.95,0.93);
  leg500_1->SetFillColor(10);
  leg500_1->SetTextFont(42);
  leg500_1->AddEntry(gHT_500_1,"d#sigma_{D*} / 0.224 - BHT1","pl");
  leg500_1->AddEntry(gMB_500_1,"d#sigma_{D*} / 0.224 - MB","pl");
  leg500_1->AddEntry(gD0_500_1,"d#sigma_{D^{0}} / 0.565 - MB","pl");
  //leg500_1->AddEntry(flevy500,"Levi-fit to All Data", "l");
  leg500_1->SetTextSize(0.03);
  leg500_1->SetBorderSize(0);
  leg500_1->SetFillStyle(0);
  leg500_1->Draw();
  sprintf(latex,"Run12 pp500GeV");
  drawLatex(0.65,0.94,latex,62,0.04,1);
  savepic(cc,"pic","pp200_pp500_levyScale");
  
  //------------------------------------------------------
  //Compare Data with FONLL(include pp200GeV and pp500GeV)
  //------------------------------------------------------

  //pp 500 GeV

  cout<<"compare data/FONLL..."<<endl;
  cc->Divide(2,1);
  cc_1->cd();
  cc_1->SetLogy(1);
  cc_1->SetGridx(0);
  cc_1->SetGridy(0);
  setpad(cc_1,0.125,0.02,0.02,0.10); 
  TH2D *h3 = new TH2D("h3","Data vs FONLL;p_{T} [GeV/c];d^{2}#sigma^{pp}_{c#bar{\c}})/(2#pip_{T}dp_{T}dy) [mb(GeV/c)^{-2}]",200,0,20,100,1e-8,1e-1);
  h3->GetYaxis()->SetTitleOffset(1.5);
  h3->Draw();
  grshade->Draw("f");
  grshade127->Draw("f");
  flevy500->Draw("same");
  gHT_500->Draw("psame");
  gMB_500->Draw("psame");
  gD0_500->Draw("psame");
  TLegend *leg500_2 = new TLegend(0.50,0.67,0.95,0.93);
  leg500_2->SetFillColor(10);
  leg500_2->SetTextFont(42);
  leg500_2->AddEntry(gHT_500,"d#sigma_{D*} / 0.224 - BHT1","pl");
  leg500_2->AddEntry(gMB_500,"d#sigma_{D*} / 0.224 - MB","pl");
  leg500_2->AddEntry(gD0_500,"d#sigma_{D^{0}} / 0.565 - MB","pl");
  leg500_2->AddEntry(flevy500,"Levi-fit to All Data", "l");
  leg500_2->AddEntry(grshade,"FONLL #mu_{R} = #mu_{F} = 1.5 GeV/c^{2}","f");
  leg500_2->AddEntry(grshade127,"FONLL #mu_{R} = #mu_{F} = 1.27 GeV/c^{2}","f");
  leg500_2->SetTextSize(0.03);
  leg500_2->SetBorderSize(0);
  leg500_2->SetFillStyle(0);
  leg500_2->Draw();
  sprintf(latex,"Run12 pp500GeV");
  drawLatex(0.51,0.94,latex,62,0.04,1);
  //savepic(cc,"pic","pp200_pp500_levyScale");
  cc_2->cd();
  cc_2->Divide(1,2);
  cc_2_1->cd();
  cc_2_1->SetLogy(0);
  cc_2_1->SetGridx(0);
  cc_2_1->SetGridy(0);
  setpad(cc_2_1,0.125,0.02,0.02,0.10);
  TH2D *h4 = new TH2D("h4","Data vs FONLL;p_{T} [GeV/c];Data/FONLL",200,0,20,100,0,4);
  h4->GetYaxis()->SetTitleOffset(1.0);
  h4->Draw();

  TGraphErrors *gD0_500_2      = (TGraphErrors *)gD0_500->Clone();
  TGraphErrors *gMB_500_2      = (TGraphErrors *)gMB_500->Clone();
  TGraphErrors *gHT_500_2      = (TGraphErrors *)gHT_500->Clone();
  TGraph *gr_FONLL500_mid_1    = (TGraph *)gr_FONLL500_mid->Clone();
  TGraph *gr_FONLL500_lower_1  = (TGraph *)gr_FONLL500_lower->Clone();
  TGraph *gr_FONLL500_upper_1  = (TGraph *)gr_FONLL500_upper->Clone();
  
  for(int i=0;i<gMB_500->GetN();i++)
    { 
      float x = gMB_500_2->GetX()[i];
      float y = gMB_500_2->GetY()[i];      
      float  ex = gMB_500_2->GetEX()[i];
      float ey = gMB_500_2->GetEY()[i];
      float scalef = 1/gr_FONLL500_mid->Eval(x,0,"s");
      y = scalef*y;
      gMB_500_2->SetPoint(i,x,y);
      gMB_500_2->SetPointError(i,ex,ey*scalef);
    }
  gMB_500_2->Draw("psame");
  for(int i=0;i<gD0_500_2->GetN();i++)
    { 
      float x = gD0_500_2->GetX()[i];
      float y = gD0_500_2->GetY()[i];      
      float  ex = gD0_500_2->GetEX()[i];
      float ey = gD0_500_2->GetEY()[i];
      float scalef  =1/gr_FONLL500_mid->Eval(x,0,"s");
      y = scalef*y;
      gD0_500_2->SetPoint(i,x,y);
      gD0_500_2->SetPointError(i,ex,ey*scalef);
    }
  gD0_500_2->Draw("psame");

  for(int i=0;i<gHT_500->GetN();i++)
    { 
      float x = gHT_500_2->GetX()[i];
      float y = gHT_500_2->GetY()[i];      
      float  ex = gHT_500_2->GetEX()[i];
      float ey = gHT_500_2->GetEY()[i];
      float scalef = 1/gr_FONLL500_mid->Eval(x,0,"s");
      y = scalef*y;
      gHT_500_2->SetPoint(i,x,y);
      gHT_500_2->SetPointError(i,ex,ey*scalef);
    }
  gHT_500_2->Draw("psame");

  for(int i=0;i<gr_FONLL500_upper_1->GetN();i++)
    { 
      float x = gr_FONLL500_upper_1->GetX()[i];
      float y = gr_FONLL500_upper_1->GetY()[i];      
      float scalef = 1/gr_FONLL500_mid->Eval(x,0,"s");
      y = scalef*y;
      gr_FONLL500_upper_1->SetPoint(i,x,y);
    }
  gr_FONLL500_upper_1->Draw("psame");

  for(int i=0;i<gr_FONLL500_lower_1->GetN();i++)
    { 
      float x = gr_FONLL500_lower_1->GetX()[i];
      float y = gr_FONLL500_lower_1->GetY()[i];      
      float scalef = 1/gr_FONLL500_mid->Eval(x,0,"s");
      y = scalef*y;
      gr_FONLL500_lower_1->SetPoint(i,x,y);
    }
  gr_FONLL500_lower_1->Draw("psame");

  for(int i=0;i<gr_FONLL500_mid_1->GetN();i++)
    { 
      float x = gr_FONLL500_mid_1->GetX()[i];
      float y = gr_FONLL500_mid_1->GetY()[i];      
      float scalef = 1/gr_FONLL500_mid->Eval(x,0,"s");
      y = scalef*y;
      gr_FONLL500_mid_1->SetPoint(i,x,y);
    }
  gr_FONLL500_mid_1->SetMarkerColor(2);
  gr_FONLL500_mid_1->SetLineColor(2);
  gr_FONLL500_mid_1->Draw("psame");
  TLegend *leg500_2_1 = new TLegend(0.65,0.57,0.95,0.93);
  leg500_2_1->SetFillColor(10);
  leg500_2_1->SetTextFont(42);
  leg500_2_1->AddEntry(gHT_500_2,"d#sigma_{D*} / 0.224 - BHT1","pl");
  leg500_2_1->AddEntry(gMB_500_2,"d#sigma_{D*} / 0.224 - MB","pl");
  leg500_2_1->AddEntry(gD0_500_2,"d#sigma_{D^{0}} / 0.565 - MB","pl");
  leg500_2_1->AddEntry(gr_FONLL500_mid_1,"FONLL[4]","pl");
  leg500_2_1->AddEntry(gr_FONLL500_upper_1,"FONLL Uncertainty","pl");
  leg500_2_1->SetTextSize(0.03);
  leg500_2_1->SetBorderSize(0);
  leg500_2_1->SetFillStyle(0);
  leg500_2_1->Draw();
  sprintf(latex,"FONLL #mu_{R} = #mu_{F} = 1.5 GeV/c^{2}");
  drawLatex(0.66,0.94,latex,62,0.04,1);
  //---------------------
  cc_2_2->cd();
  cc_2_2->SetLogy(0);
  cc_2_2->SetGridx(0);
  cc_2_2->SetGridy(0);
  setpad(cc_2_2,0.125,0.02,0.02,0.10);
  TH2D *h5 = new TH2D("h5","Data vs FONLL;p_{T} [GeV/c];Data/FONLL",200,0,20,100,0,4);
  h5->GetYaxis()->SetTitleOffset(1.0);
  h5->Draw();

  TGraphErrors *gD0_500_3      = (TGraphErrors *)gD0_500->Clone();
  TGraphErrors *gMB_500_3      = (TGraphErrors *)gMB_500->Clone();
  TGraphErrors *gHT_500_3      = (TGraphErrors *)gHT_500->Clone();
  TGraph *gr_FONLL500m_mid_1   = (TGraph *)gr_FONLL500m_mid->Clone();
  TGraph *gr_FONLL500m_lower_1 = (TGraph *)gr_FONLL500m_lower->Clone();
  TGraph *gr_FONLL500m_upper_1 = (TGraph *)gr_FONLL500m_upper->Clone();

  for(int i=0;i<gMB_500->GetN();i++)
    { 
      float x = gMB_500_3->GetX()[i];
      float y = gMB_500_3->GetY()[i];      
      float  ex = gMB_500_3->GetEX()[i];
      float ey = gMB_500_3->GetEY()[i];
      float scalef = 1/gr_FONLL500m_mid->Eval(x,0,"s");
      y = scalef*y;
      gMB_500_3->SetPoint(i,x,y);
      gMB_500_3->SetPointError(i,ex,ey*scalef);
    }
  gMB_500_3->Draw("psame");
  for(int i=0;i<gD0_500_3->GetN();i++)
    { 
      float x = gD0_500_3->GetX()[i];
      float y = gD0_500_3->GetY()[i];      
      float  ex = gD0_500_3->GetEX()[i];
      float ey = gD0_500_3->GetEY()[i];
      float scalef  =1/gr_FONLL500m_mid->Eval(x,0,"s");
      y = scalef*y;
      gD0_500_3->SetPoint(i,x,y);
      gD0_500_3->SetPointError(i,ex,ey*scalef);
    }
  gD0_500_3->Draw("psame");

  for(int i=0;i<gHT_500->GetN();i++)
    { 
      float x = gHT_500_3->GetX()[i];
      float y = gHT_500_3->GetY()[i];      
      float  ex = gHT_500_3->GetEX()[i];
      float ey = gHT_500_3->GetEY()[i];
      float scalef = 1/gr_FONLL500m_mid->Eval(x,0,"s");
      y = scalef*y;
      gHT_500_3->SetPoint(i,x,y);
      gHT_500_3->SetPointError(i,ex,ey*scalef);
    }
  gHT_500_3->Draw("psame");

  for(int i=0;i<gr_FONLL500m_upper_1->GetN();i++)
    { 
      float x = gr_FONLL500m_upper_1->GetX()[i];
      float y = gr_FONLL500m_upper_1->GetY()[i];      
      float scalef = 1/gr_FONLL500m_mid->Eval(x,0,"s");
      y = scalef*y;
      gr_FONLL500m_upper_1->SetPoint(i,x,y);
    }
  gr_FONLL500m_upper_1->SetMarkerColor(1);
  gr_FONLL500m_upper_1->Draw("psame");

  for(int i=0;i<gr_FONLL500m_lower_1->GetN();i++)
    { 
      float x = gr_FONLL500m_lower_1->GetX()[i];
      float y = gr_FONLL500m_lower_1->GetY()[i];      
      float scalef = 1/gr_FONLL500m_mid->Eval(x,0,"s");
      y = scalef*y;
      gr_FONLL500m_lower_1->SetPoint(i,x,y);
    }
  gr_FONLL500m_lower_1->SetMarkerColor(1);
  gr_FONLL500m_lower_1->Draw("psame");

  for(int i=0;i<gr_FONLL500m_mid_1->GetN();i++)
    { 
      float x = gr_FONLL500m_mid_1->GetX()[i];
      float y = gr_FONLL500m_mid_1->GetY()[i];      
      float scalef = 1/gr_FONLL500m_mid->Eval(x,0,"s");
      y = scalef*y;
      gr_FONLL500m_mid_1->SetPoint(i,x,y);
    }
  gr_FONLL500m_mid_1->SetMarkerColor(2);
  gr_FONLL500m_mid_1->SetLineColor(2);
  gr_FONLL500m_mid_1->Draw("psame");
  TLegend *leg500_2_2 = new TLegend(0.65,0.57,0.95,0.93);
  leg500_2_2->SetFillColor(10);
  leg500_2_2->SetTextFont(42);
  leg500_2_2->AddEntry(gHT_500_3,"d#sigma_{D*} / 0.224 - BHT1","pl");
  leg500_2_2->AddEntry(gMB_500_3,"d#sigma_{D*} / 0.224 - MB","pl");
  leg500_2_2->AddEntry(gD0_500_3,"d#sigma_{D^{0}} / 0.565 - MB","pl");
  leg500_2_2->AddEntry(gr_FONLL500m_mid_1,"FONLL[4]","pl");
  leg500_2_2->AddEntry(gr_FONLL500m_upper_1,"FONLL Uncertainty","pl");
  leg500_2_2->SetTextSize(0.03);
  leg500_2_2->SetBorderSize(0);
  leg500_2_2->SetFillStyle(0);
  leg500_2_2->Draw();
  sprintf(latex,"FONLL #mu_{R} = #mu_{F} = 1.27 GeV/c^{2}");
  drawLatex(0.66,0.94,latex,62,0.04,1);
  savepic(cc,"pic","pp500_DataOfFONLL");
  
  //----------------------------------------------------
  //compare Data/FONLL upper limit(both pp500 and pp200)
  //----------------------------------------------------
  //pp200 GeV 
  cc->Divide(2,1);
  cc_1->cd();
  cc_1->SetLogy(1);
  cc_1->SetGridx(0);
  cc_1->SetGridy(0);
  setpad(cc_1,0.125,0.02,0.02,0.10); 
  TH2D *h3_1 = new TH2D("h3_1","Data vs FONLL;p_{T} [GeV/c];d^{2}#sigma^{pp}_{c#bar{\c}})/(2#pip_{T}dp_{T}dy) [mb(GeV/c)^{-2}]",100,0,10,100,1e-8,1e-1);
  h3_1->GetYaxis()->SetTitleOffset(1.5);
  h3_1->Draw();
  pp200shade->Draw("f"); 
  flevy200->Draw("same");
  gD0_200->Draw("psame");
  gDstar_200->Draw("psame");
  gDstarHT2_200->Draw("psame");

  TLegend *leg200_3_1 = new TLegend(0.63,0.77,0.95,0.93);
  leg200_3_1->SetFillColor(10);
  leg200_3_1->SetTextFont(42);
  leg200_3_1->AddEntry(gDstarHT2_200,"d#sigma_{D*} / 0.224 - BHT2","pl");
  leg200_3_1->AddEntry(gDstar_200,"d#sigma_{D*} / 0.224 - MB","pl");
  leg200_3_1->AddEntry(gD0_200,"d#sigma_{D^{0}} / 0.565 - MB","pl");
  leg200_3_1->AddEntry(flevy200,"Levi-fit to All Data", "l");
  leg200_3_1->SetTextSize(0.03);
  leg200_3_1->SetBorderSize(0);
  leg200_3_1->SetFillStyle(0);
  leg200_3_1->Draw();
  sprintf(latex,"Run12 pp200GeV");
  drawLatex(0.63,0.94,latex,62,0.04,1); 

  cc_2->cd();
  cc_2->SetLogy(0);
  cc_2->SetGridx(0);
  cc_2->SetGridy(0);
  setpad(cc_2,0.125,0.02,0.02,0.10);
  TH2D *h3_2 = new TH2D("h3_2","Data vs FONLL;p_{T} [GeV/c];Data/FONLL",100,0,10,100,0,4);
  h3_2->GetYaxis()->SetTitleOffset(1.0);
  h3_2->Draw();

  TGraphErrors *gD0_200_32       = (TGraphErrors *)gD0_200->Clone();
  TGraphErrors *gDstar_200_32    = (TGraphErrors *)gDstar_200->Clone();
  TGraphErrors *gDstarHT2_200_32 = (TGraphErrors *)gDstarHT2_200->Clone();
  TGraph *gr_FONLL_u_32   = (TGraph *)gr_FONLL_u->Clone();
  TGraph *gr_FONLL_d_32 = (TGraph *)gr_FONLL_d->Clone();

  for(int i=0;i<gD0_200_32->GetN();i++)
    { 
      float x = gD0_200_32->GetX()[i];
      float y = gD0_200_32->GetY()[i];      
      float  ex = gD0_200_32->GetEX()[i];
      float ey = gD0_200_32->GetEY()[i];
      float scalef = 1/gr_FONLL_u_32->Eval(x,0,"s");
      y = scalef*y;
      gD0_200_32->SetPoint(i,x,y);
      gD0_200_32->SetPointError(i,ex,ey*scalef);
    }
  gD0_200_32->Draw("psame");
  for(int i=0;i<gDstar_200_32->GetN();i++)
    { 
      float x = gDstar_200_32->GetX()[i];
      float y = gDstar_200_32->GetY()[i];      
      float  ex = gDstar_200_32->GetEX()[i];
      float ey = gDstar_200_32->GetEY()[i];
      float scalef  =1/gr_FONLL_u->Eval(x,0,"s");
      y = scalef*y;
      gDstar_200_32->SetPoint(i,x,y);
      gDstar_200_32->SetPointError(i,ex,ey*scalef);
    }
  gDstar_200_32->Draw("psame");

  for(int i=0;i<gDstarHT2_200_32->GetN();i++)
    { 
      float x = gDstarHT2_200_32->GetX()[i];
      float y = gDstarHT2_200_32->GetY()[i];      
      float  ex = gDstarHT2_200_32->GetEX()[i];
      float ey = gDstarHT2_200_32->GetEY()[i];
      float scalef = 1/gr_FONLL_u->Eval(x,0,"s");
      y = scalef*y;
      gDstarHT2_200_32->SetPoint(i,x,y);
      gDstarHT2_200_32->SetPointError(i,ex,ey*scalef);
    }
  gDstarHT2_200_32->Draw("psame");

  for(int i=0;i<gr_FONLL_u_32->GetN();i++)
    { 
      float x = gr_FONLL_u_32->GetX()[i];
      float y = gr_FONLL_u_32->GetY()[i];      
      float scalef = 1/gr_FONLL_u->Eval(x,0,"s");
      y = scalef*y;
      gr_FONLL_u_32->SetPoint(i,x,y);
    }
  gr_FONLL_u_32->SetMarkerColor(2);
  gr_FONLL_u_32->SetLineColor(2);
  //gr_FONLL_u_32->Draw("psame");
  drawLine(0,1,10,1,0.08,4,2);
  TLegend *leg200_32 = new TLegend(0.63,0.77,0.95,0.93);
  leg200_32->SetFillColor(10);
  leg200_32->SetTextFont(42);
  leg200_32->AddEntry(gDstarHT2_200_32,"d#sigma_{D*} / 0.224 - BHT2","pl");
  leg200_32->AddEntry(gDstar_200_32,"d#sigma_{D*} / 0.224 - MB","pl");
  leg200_32->AddEntry(gD0_200_32,"d#sigma_{D^{0}} / 0.565 - MB","pl");
  leg200_32->SetTextSize(0.03);
  leg200_32->SetBorderSize(0);
  leg200_32->SetFillStyle(0);
  leg200_32->Draw();
  sprintf(latex,"Data of FONLL upper Limit");
  drawLatex(0.63,0.95,latex,62,0.03,1); 
  savepic(cc,"pic","pp200_DataOfFONLL_upper");

  //pp 500 GeV

  cout<<"Compare Data/FONLL upper limits..."<<endl;
  cc->Divide(2,1);
  cc_1->cd();
  cc_1->SetLogy(1);
  cc_1->SetGridx(0);
  cc_1->SetGridy(0);
  setpad(cc_1,0.125,0.02,0.02,0.10); 
  TH2D *h41 = new TH2D("h41","Data vs FONLL;p_{T} [GeV/c];d^{2}#sigma^{pp}_{c#bar{\c}})/(2#pip_{T}dp_{T}dy) [mb(GeV/c)^{-2}]",200,0,20,100,1e-8,1e-1);
  h41->GetYaxis()->SetTitleOffset(1.5);
  h41->Draw();
  grshade->Draw("f");
  grshade127->Draw("f");
  flevy500->Draw("same");
  gHT_500->Draw("psame");
  gMB_500->Draw("psame");
  gD0_500->Draw("psame");
  TLegend *leg500_41 = new TLegend(0.50,0.67,0.95,0.93);
  leg500_41->SetFillColor(10);
  leg500_41->SetTextFont(42);
  leg500_41->AddEntry(gHT_500,"d#sigma_{D*} / 0.224 - BHT1","pl");
  leg500_41->AddEntry(gMB_500,"d#sigma_{D*} / 0.224 - MB","pl");
  leg500_41->AddEntry(gD0_500,"d#sigma_{D^{0}} / 0.565 - MB","pl");
  leg500_41->AddEntry(flevy500,"Levi-fit to All Data", "l");
  leg500_41->AddEntry(grshade,"FONLL #mu_{R} = #mu_{F} = 1.5 GeV/c^{2}","f");
  leg500_41->AddEntry(grshade127,"FONLL #mu_{R} = #mu_{F} = 1.27 GeV/c^{2}","f");
  leg500_41->SetTextSize(0.03);
  leg500_41->SetBorderSize(0);
  leg500_41->SetFillStyle(0);
  leg500_41->Draw();
  sprintf(latex,"Run12 pp500GeV");
  drawLatex(0.51,0.94,latex,62,0.04,1);
  //savepic(cc,"pic","pp200_pp500_levyScale");
  cc_2->cd();
  cc_2->Divide(1,2);
  cc_2_1->cd();
  cc_2_1->SetLogy(0);
  cc_2_1->SetGridx(0);
  cc_2_1->SetGridy(0);
  setpad(cc_2_1,0.125,0.02,0.02,0.10);
  TH2D *h42 = new TH2D("h42","Data vs FONLL;p_{T} [GeV/c];Data/FONLL",200,0,20,100,0,4);
  h42->GetYaxis()->SetTitleOffset(1.0);
  h42->Draw();

  TGraphErrors *gD0_500_42      = (TGraphErrors *)gD0_500->Clone();
  TGraphErrors *gMB_500_42      = (TGraphErrors *)gMB_500->Clone();
  TGraphErrors *gHT_500_42      = (TGraphErrors *)gHT_500->Clone();
  TGraph *gr_FONLL500_upper_42  = (TGraph *)gr_FONLL500_upper->Clone();
  
  for(int i=0;i<gMB_500_42->GetN();i++)
    { 
      float x = gMB_500_42->GetX()[i];
      float y = gMB_500_42->GetY()[i];      
      float  ex = gMB_500_42->GetEX()[i];
      float ey = gMB_500_42->GetEY()[i];
      float scalef = 1/gr_FONLL500_upper->Eval(x,0,"s");
      y = scalef*y;
      gMB_500_42->SetPoint(i,x,y);
      gMB_500_42->SetPointError(i,ex,ey*scalef);
    }
  gMB_500_42->Draw("psame");
  for(int i=0;i<gD0_500_42->GetN();i++)
    { 
      float x = gD0_500_42->GetX()[i];
      float y = gD0_500_42->GetY()[i];      
      float  ex = gD0_500_42->GetEX()[i];
      float ey = gD0_500_42->GetEY()[i];
      float scalef  =1/gr_FONLL500_upper->Eval(x,0,"s");
      y = scalef*y;
      gD0_500_42->SetPoint(i,x,y);
      gD0_500_42->SetPointError(i,ex,ey*scalef);
    }
  gD0_500_42->Draw("psame");

  for(int i=0;i<gHT_500->GetN();i++)
    { 
      float x = gHT_500_42->GetX()[i];
      float y = gHT_500_42->GetY()[i];      
      float  ex = gHT_500_42->GetEX()[i];
      float ey = gHT_500_42->GetEY()[i];
      float scalef = 1/gr_FONLL500_upper->Eval(x,0,"s");
      y = scalef*y;
      gHT_500_42->SetPoint(i,x,y);
      gHT_500_42->SetPointError(i,ex,ey*scalef);
    }
  gHT_500_42->Draw("psame");

  for(int i=0;i<gr_FONLL500_upper_42->GetN();i++)
    { 
      float x = gr_FONLL500_upper_42->GetX()[i];
      float y = gr_FONLL500_upper_42->GetY()[i];      
      float scalef = 1/gr_FONLL500_upper->Eval(x,0,"s");
      y = scalef*y;
      gr_FONLL500_upper_42->SetPoint(i,x,y);
    }
  gr_FONLL500_upper_42->SetMarkerColor(2);
  gr_FONLL500_upper_42->SetLineColor(2);
  gr_FONLL500_upper_42->Draw("psame");
  TLegend *leg500_42 = new TLegend(0.65,0.57,0.95,0.93);
  leg500_42->SetFillColor(10);
  leg500_42->SetTextFont(42);
  leg500_42->AddEntry(gHT_500_42,"d#sigma_{D*} / 0.224 - BHT1","pl");
  leg500_42->AddEntry(gMB_500_42,"d#sigma_{D*} / 0.224 - MB","pl");
  leg500_42->AddEntry(gD0_500_42,"d#sigma_{D^{0}} / 0.565 - MB","pl");
  leg500_42->SetTextSize(0.03);
  leg500_42->SetBorderSize(0);
  leg500_42->SetFillStyle(0);
  leg500_42->Draw();
  sprintf(latex,"FONLL #mu_{R} = #mu_{F} = 1.5 GeV/c^{2}");
  drawLatex(0.66,0.94,latex,62,0.04,1);
  //---------------------
  cc_2_2->cd();
  cc_2_2->SetLogy(0);
  cc_2_2->SetGridx(0);
  cc_2_2->SetGridy(0);
  setpad(cc_2_2,0.125,0.02,0.02,0.10);
  TH2D *h43 = new TH2D("h43","Data vs FONLL;p_{T} [GeV/c];Data/FONLL",200,0,20,100,0,4);
  h43->GetYaxis()->SetTitleOffset(1.0);
  h43->Draw();

  TGraphErrors *gD0500_43      = (TGraphErrors *)gD0_500->Clone();
  TGraphErrors *gMB500_43      = (TGraphErrors *)gMB_500->Clone();
  TGraphErrors *gHT500_43      = (TGraphErrors *)gHT_500->Clone();
  TGraph *gr_FONLL500m_upper_43 = (TGraph *)gr_FONLL500m_upper->Clone();

  for(int i=0;i<gMB500_43->GetN();i++)
    { 
      float x = gMB500_43->GetX()[i];
      float y = gMB500_43->GetY()[i];      
      float  ex = gMB500_43->GetEX()[i];
      float ey = gMB500_43->GetEY()[i];
      float scalef = 1/gr_FONLL500m_upper->Eval(x,0,"s");
      y = scalef*y;
      gMB500_43->SetPoint(i,x,y);
      gMB500_43->SetPointError(i,ex,ey*scalef);
    }
  gMB500_43->Draw("psame");
  for(int i=0;i<gD0500_43->GetN();i++)
    { 
      float x = gD0500_43->GetX()[i];
      float y = gD0500_43->GetY()[i];      
      float  ex = gD0500_43->GetEX()[i];
      float ey = gD0500_43->GetEY()[i];
      float scalef  =1/gr_FONLL500m_upper->Eval(x,0,"s");
      y = scalef*y;
      gD0500_43->SetPoint(i,x,y);
      gD0500_43->SetPointError(i,ex,ey*scalef);
    }
  gD0500_43->Draw("psame");

  for(int i=0;i<gHT500_43->GetN();i++)
    { 
      float x = gHT500_43->GetX()[i];
      float y = gHT500_43->GetY()[i];      
      float  ex = gHT500_43->GetEX()[i];
      float ey = gHT500_43->GetEY()[i];
      float scalef = 1/gr_FONLL500m_upper->Eval(x,0,"s");
      y = scalef*y;
      gHT500_43->SetPoint(i,x,y);
      gHT500_43->SetPointError(i,ex,ey*scalef);
    }
  gHT500_43->Draw("psame");

  for(int i=0;i<gr_FONLL500m_upper_43->GetN();i++)
    { 
      float x = gr_FONLL500m_upper_43->GetX()[i];
      float y = gr_FONLL500m_upper_43->GetY()[i];      
      float scalef = 1/gr_FONLL500m_upper->Eval(x,0,"s");
      y = scalef*y;
      gr_FONLL500m_upper_43->SetPoint(i,x,y);
    }
  gr_FONLL500m_upper_43->SetMarkerColor(2);
  gr_FONLL500m_upper_43->SetLineColor(2);
  gr_FONLL500m_upper_43->Draw("psame");
  TLegend *leg500_43 = new TLegend(0.65,0.57,0.95,0.93);
  leg500_43->SetFillColor(10);
  leg500_43->SetTextFont(42);
  leg500_43->AddEntry(gHT500_43,"d#sigma_{D*} / 0.224 - BHT1","pl");
  leg500_43->AddEntry(gMB500_43,"d#sigma_{D*} / 0.224 - MB","pl");
  leg500_43->AddEntry(gD0500_43,"d#sigma_{D^{0}} / 0.565 - MB","pl");
  leg500_43->SetTextSize(0.03);
  leg500_43->SetBorderSize(0);
  leg500_43->SetFillStyle(0);
  leg500_43->Draw();
  sprintf(latex,"FONLL #mu_{R} = #mu_{F} = 1.27 GeV/c^{2}");
  drawLatex(0.66,0.94,latex,62,0.04,1);
  savepic(cc,"pic","pp500_DataOfFONLL_Upper");

  /*
  htmp = new TH1D("htmp","",1,0., 0.19);
  htmp->GetYaxis()->SetTitle("(UL #times LS) / ME^{2}");
  htmp->GetYaxis()->SetTitleSize(0.065);
  htmp->GetYaxis()->SetTitleOffset(1.3);
  htmp->GetYaxis()->SetLabelSize(0.047);
  htmp->SetMinimum(ymin);
  htmp->SetMaximum(ymax);
  htmp->GetXaxis()->SetTitle("M_{#mup} - m_{#mu} - m_{p} (GeV/c^{2})");
  htmp->GetXaxis()->SetTitleOffset(1.1);
  htmp->GetXaxis()->SetTitleSize(0.055);
  htmp->GetXaxis()->SetLabelOffset(0.01);
  htmp->GetXaxis()->SetLabelSize(0.047);
  htmp->GetXaxis()->SetLabelFont(42);
  htmp->GetXaxis()->SetTitleFont(42);
  htmp->Draw();

  h2->SetLineColor(4);
  h2->Draw("same");

  TF1 *fun1 = new TF1("fun1","gaus(0)+[3]*x+[4]",0.,0.05);
  fun1->SetParameters(0.5,0.002,0.001,1.,1.);

  fun1->SetParLimits(2,0.001,0.002);
  fun1->SetLineColor(2);
  h2->Fit(fun1,"NOR");
  fun1->Draw("same");
  Double_t par[5],parerr[5];
  fun1->GetParameters(par);
  parerr[1] = fun1->GetParError(1);

  TF1 *fg = new TF1("fg","gaus",0,0.05);
  fg->SetParameters(par);
  fg->SetLineColor(2);

  TF1 *fe = new TF1("fe","[0]*x+[1]",0,0.05);
  fe->SetParameters(&par[3]);
  fe->SetLineColor(6);

  drawLine(0,1,0.19,1,0.08,1,2);

  sprintf(latex,"Au+Au 0%%-20%%");
  drawLatex(0.6,0.7,latex,62,0.04,1);
  sprintf(latex,"m_{#mup} = %3.4f#pm%3.4f",par[1],parerr[1]);
  drawLatex(0.6,0.6,latex,62,0.04,1);

  cc->SaveAs("./pic/pmu-signal.gif");
  cc->SaveAs("./pic/pmu-signal.C");
  */
}

TLine* drawLine(Double_t xlow,Double_t ylow, Double_t xup, Double_t yup, Double_t lineWidth, Int_t lineColor,Int_t linestyle){
  TLine *l1 = new TLine(xlow,ylow,xup,yup);
  l1->SetLineWidth(lineWidth);
  l1->SetLineColor(lineColor);
  l1->SetLineStyle(linestyle);
  l1->Draw("same");
  return l1;
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

void savepic(TCanvas *c1,char *dir,char *name)
{
  c1->SaveAs(Form("%s/%s.gif",dir,name));
  c1->SaveAs(Form("%s/%s.C",dir,name));
  c1->Clear();
  c1->cd();
}
