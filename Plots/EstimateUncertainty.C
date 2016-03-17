//===============================
//Version: 1.0 
//Time: Fri Jul 25 17:14:44 EDT 2014 
//Author: Long Zhou 
//Discribe: This macro used to do pp Reference work, the whold macro is depend on the getppReference.C version 1.4 

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include "TH1.h"
#include "TMath.h"
#include "TF1.h"
#include "TLegend.h"
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
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TLine.h"
#include "TLatex.h"
#include "TGraphAsymmErrors.h"
using namespace std;
TLine* drawLine(Double_t xlow,Double_t ylow, Double_t xup, Double_t yup, Double_t lineWidth, Int_t lineColor,Int_t linestyle);
TLatex* drawLatex(Double_t x, Double_t y, char* text, Int_t textFont, Double_t textSize, Int_t colorIndex);
void setpad(TPad *pad,float left, float right, float top, float bottom);
void savepic(TCanvas *c1,char *dir,char *name);
Int_t hist2graph(TH1F *h,TGraph *gr);
Int_t hist2graph(TH1D *h,TGraph *gr);
void scaleGraph(TGraphErrors *gr,float scale);
void scaleGraph(TGraphAsymmErrors *gr,float scale);
Int_t combineGraphs(TGraphErrors *grAll,TGraphErrors *gr1,TGraphErrors *gr2,TGraphErrors *gr3);
Int_t combineGraphs(TGraphAsymmErrors *grAll,TGraphAsymmErrors *gr1,TGraphAsymmErrors *gr2,TGraphAsymmErrors *gr3);
Int_t combineGraphs(TGraphErrors *all,TGraphErrors *gr1,TGraphErrors *gr2);
Int_t combineGraphs(TGraphAsymmErrors *grAll,TGraphAsymmErrors *gr1,TGraphAsymmErrors *gr2);
Int_t addShade(TGraph *shade,TGraph *up,TGraph *down);
Int_t GraphErrorsToGraphAsymmErrors(TGraphErrors *gr,TGraphAsymmErrors *gar);
Int_t FONLLscale(TGraphAsymmErrors *gar,TGraph *gr, TGraph *gr_u, TGraph *gr_d);
void FONLLscale(TGraphErrors *gr,TGraph *ratio);
void EstimateUncertainty(TString input="ppReference.root")
{
  TCanvas *cc = new TCanvas("cc", "cc",0,0,1600,800);
  gStyle->SetOptFit(0);
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  gStyle->SetEndErrorSize(0.01);
  gStyle->SetOptDate(0);
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
  TGraph *gr_FONLL500m_mid   = (TGraph *)pp500->Get("gr_FONLL500m_mid");//m127
  TGraph *gr_FONLL500m_upper = (TGraph *)pp500->Get("gr_FONLL500m_upper");
  TGraph *gr_FONLL500m_lower = (TGraph *)pp500->Get("gr_FONLL500m_lower");
  flevy500->SetLineColor(4);
  gHT_500->SetMarkerColor(4);
  gHT_500->SetLineColor(4);
  gMB_500->SetMarkerColor(4);
  gMB_500->SetLineColor(4);
  gD0_500->SetMarkerColor(4);
  gD0_500->SetLineColor(4);
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
  flevy200->SetLineStyle(7);
  flevy200->SetLineWidth(2);
  flevy200->SetLineColor(2);
  gDstarHT2_200->SetMarkerColor(2);
  gDstarHT2_200->SetLineColor(2);
  gDstar_200->SetMarkerColor(2);
  gDstar_200->SetLineColor(2);
  gD0_200->SetMarkerColor(2);
  gD0_200->SetLineColor(2);
  Int_t NPoint = gr_FONLL_u->GetN();
  TGraph *pp200shade = new TGraph(2*NPoint);
  if(!addShade(pp200shade,gr_FONLL_u,gr_FONLL_d)) return;
  //=====================================
  //accumulate pp500 point ot one graph.. 
  Int_t ND0 = gD0_500->GetN();
  Int_t NHT = gHT_500->GetN();
  Int_t NMB = gMB_500->GetN();
  NPoint = ND0 + NHT + NMB;
  TGraphErrors *pp500All = new TGraphErrors(NPoint);
  if(!combineGraphs(pp500All,gD0_500,gHT_500,gMB_500)) return;
  pp500All->Sort();
  //=====================================
  //accumulate pp200 point ot one graph.. 
  ND0 = gD0_200->GetN();
  NHT = gDstarHT2_200->GetN();
  NMB = gDstar_200->GetN();
  NPoint = ND0 + NHT + NMB;
  TGraphErrors *pp200All = new TGraphErrors(NPoint);
  if(!combineGraphs(pp200All,gD0_200,gDstarHT2_200,gDstar_200)) return;
  pp200All->Sort();
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

  /*=================================================
  | work number:5                                    |
  | scale pp500 to pp200 with FONLL calculate results| 
   =================================================*/   
  //scale pp500 to pp200
  TGraphErrors *gD0_500_52       = (TGraphErrors *)gD0_500->Clone();
  TGraphErrors *gHT_500_52       = (TGraphErrors *)gHT_500->Clone();
  TGraphErrors *gMB_500_52       = (TGraphErrors *)gMB_500->Clone();
  TGraphErrors *pp500All_52      = (TGraphErrors *)pp500All->Clone();
  TFile *f2 = new TFile("gr_rat500_200_m127.root");
  TDirectory *rat500_200_m127_all = (TDirectory *)f2->Get("rat500_200_m127_all");
  TDirectory *rat500_200_m127_corr = (TDirectory *)f2->Get("rat500_200_m127_corr");
  TDirectory *rat500_200_m127_uncorr = (TDirectory *)f2->Get("rat500_200_m127_uncorr"); 
  TGraph *grFONLLRatio   = (TGraph *)rat500_200_m127_all->Get("grFONLLRatio");
  TGraph *grFONLLRatio_u = (TGraph *)rat500_200_m127_all->Get("grFONLLRatio_u");
  TGraph *grFONLLRatio_d = (TGraph *)rat500_200_m127_all->Get("grFONLLRatio_d");
 
  NPoint = grFONLLRatio->GetN();
  TGraphAsymmErrors *ratio = new TGraphAsymmErrors(NPoint);
  for(int i=0;i<NPoint;i++)
    {
      float x = grFONLLRatio->GetX()[i];
      float y = grFONLLRatio->GetY()[i];      
      float eyl = grFONLLRatio_d->GetY()[i];
      float eyh = grFONLLRatio_u->GetY()[i];
      ratio->SetPoint(i,x,y);
      ratio->SetPointError(i,0,0,y-eyl,eyh-y);
    }
  /*
  FONLLscale(gD0_500_52,grFONLLRatio);
  FONLLscale(gHT_500_52,grFONLLRatio);
  FONLLscale(gMB_500_52,grFONLLRatio);
  FONLLscale(pp500All_52,grFONLLRatio);
  */
  //=========
  ND0 = gD0_500_52->GetN();
  NHT = gHT_500_52->GetN();
  NMB = gMB_500_52->GetN();
  NPoint = pp500All_52->GetN();
  TGraphAsymmErrors *gD0500 = new TGraphAsymmErrors(ND0);
  TGraphAsymmErrors *gHT500 = new TGraphAsymmErrors(NHT);
  TGraphAsymmErrors *gMB500 = new TGraphAsymmErrors(NMB);
  TGraphAsymmErrors *gpp500All = new TGraphAsymmErrors(NPoint);
  /*
  gHT500->SetMarkerStyle(20);
  gHT500->SetMarkerColor(4);
  gHT500->SetLineColor(4);
  gMB500->SetMarkerStyle(25);
  gMB500->SetMarkerColor(4);
  gMB500->SetLineColor(4);
  gD0500->SetMarkerStyle(21);
  gD0500->SetMarkerColor(4);
  gD0500->SetLineColor(4);
  gpp500All->SetMarkerColor(4);
  gpp500All->SetLineColor(4);*/

  if(!GraphErrorsToGraphAsymmErrors(gD0_500_52,gD0500)) return;
  if(!GraphErrorsToGraphAsymmErrors(gHT_500_52,gHT500)) return;
  if(!GraphErrorsToGraphAsymmErrors(gMB_500_52,gMB500)) return;
  if(!GraphErrorsToGraphAsymmErrors(pp500All_52,gpp500All)) return;
  FONLLscale(gD0500,grFONLLRatio,grFONLLRatio_u,grFONLLRatio_d);
  FONLLscale(gHT500,grFONLLRatio,grFONLLRatio_u,grFONLLRatio_d);
  FONLLscale(gMB500,grFONLLRatio,grFONLLRatio_u,grFONLLRatio_d);
  FONLLscale(gpp500All,grFONLLRatio,grFONLLRatio_u,grFONLLRatio_d);
  //=========
  cc->cd();
  cc->SetLogy(1);
  cc->SetGridx(0);
  cc->SetGridy(0);
  setpad(cc,0.125,0.02,0.02,0.10); 
  TH2D *h51 = new TH2D("h51","Data vs FONLL;p_{T} [GeV/c];d^{2}#sigma^{pp}_{c#bar{\c}})/(2#pip_{T}dp_{T}dy) [mb(GeV/c)^{-2}]",200,0,20,100,1e-10,1e-1);
  h51->GetYaxis()->SetTitleOffset(1.5);
  h51->Draw();
  //pp200shade->Draw("f"); 
  //flevy200->Draw("same");
  gD0_200->Draw("psame");
  gDstar_200->Draw("psame");
  gDstarHT2_200->Draw("psame");
  gD0500->Draw("psame");
  gHT500->Draw("psame");
  gMB500->Draw("psame");

  TLegend *leg200_51 = new TLegend(0.14,0.2,0.45,0.35);
  leg200_51->SetFillColor(10);
  leg200_51->SetTextFont(42);
  leg200_51->AddEntry(gDstarHT2_200,"d#sigma_{D*} / 0.224 - BHT2","lep");
  leg200_51->AddEntry(gDstar_200,"d#sigma_{D*} / 0.224 - MB","lep");
  leg200_51->AddEntry(gD0_200,"d#sigma_{D^{0}} / 0.565 - MB","lep");
  leg200_51->AddEntry(flevy200,"Levi-fit to All Data", "l");
  leg200_51->SetTextSize(0.03);
  leg200_51->SetBorderSize(0);
  leg200_51->SetFillStyle(0);
  leg200_51->Draw();
  sprintf(latex,"Run12 pp200GeV");
  drawLatex(0.15,0.36,latex,62,0.04,1);

  TLegend *leg500_52 = new TLegend(0.50,0.67,0.95,0.93);
  leg500_52->SetFillColor(10);
  leg500_52->SetTextFont(42);
  leg500_52->AddEntry(gHT500,"d#sigma_{D*} / 0.224 - BHT1","lep");
  leg500_52->AddEntry(gMB500,"d#sigma_{D*} / 0.224 - MB","lep");
  leg500_52->AddEntry(gD0500,"d#sigma_{D^{0}} / 0.565 - MB","lep");
  //leg500_52->AddEntry(grshade,"FONLL #mu_{R} = #mu_{F} = 1.5 GeV/c^{2}","f");
  //leg500_52->AddEntry(grshade127,"FONLL #mu_{R} = #mu_{F} = 1.27 GeV/c^{2}","f");
  leg500_52->SetTextSize(0.03);
  leg500_52->SetBorderSize(0);
  leg500_52->SetFillStyle(0);
  leg500_52->Draw();
  sprintf(latex,"Run11 pp500GeV");
  drawLatex(0.51,0.94,latex,62,0.04,1);
  savepic(cc,"pic","pp500topp200");
  /*================================================================
  | work number:6                                                   |
  | 1>scale pp500 to pp200 with FONLL contral value(work number 5)  |
  | 2>but the shape is not consistent,so ,we will use Levy function |
  | to fit pp500 data                                               |
  | 3>use Levy function result to scale pp500 to pp200              |
   ================================================================*/
  cc->cd();  
  cc->SetLogy(1);
  cc->SetGridx(0);
  cc->SetGridy(0);
  setpad(cc,0.125,0.02,0.02,0.10); 
  TH2D *h61 = new TH2D("h61","Data vs FONLL;p_{T} [GeV/c];d^{2}#sigma^{pp}_{c#bar{\c}})/(2#pip_{T}dp_{T}dy) [mb(GeV/c)^{-2}]",200,0,20,100,1e-10,1e-1);
  h61->GetYaxis()->SetTitleOffset(1.5);
  h61->Draw();
  //pp200shade->Draw("f"); 
  flevy200->Draw("same");
  gD0_200->Draw("psame");
  gDstar_200->Draw("psame");
  gDstarHT2_200->Draw("psame");

  TLegend *leg200_61 = new TLegend(0.14,0.2,0.45,0.35);
  leg200_61->SetFillColor(10);
  leg200_61->SetTextFont(42);
  leg200_61->SetTextColor(2);
  leg200_61->AddEntry(gDstarHT2_200,"d#sigma_{D*} / 0.224 - BHT2","lep");
  leg200_61->AddEntry(gDstar_200,"d#sigma_{D*} / 0.224 - MB","lep");
  leg200_61->AddEntry(gD0_200,"d#sigma_{D^{0}} / 0.565 - MB","lep");
  leg200_61->AddEntry(flevy200,"Levi-fit to All Data", "l");
  leg200_61->SetTextSize(0.03);
  leg200_61->SetBorderSize(0);
  leg200_61->SetFillStyle(0);
  leg200_61->Draw();
  sprintf(latex,"Run12 pp200GeV");
  drawLatex(0.15,0.36,latex,62,0.04,2); 

  TGraphAsymmErrors *gD0_500_62    = (TGraphAsymmErrors *)gD0500->Clone();
  TGraphAsymmErrors *gHT_500_62    = (TGraphAsymmErrors *)gHT500->Clone();
  TGraphAsymmErrors *gMB_500_62    = (TGraphAsymmErrors *)gMB500->Clone();

  TF1 *flevy500_62 = (TF1 *)flevy500->Clone("flevy500_62");
  ND0 = gD0_500->GetN();
  NHT = gHT_500->GetN();
  NMB = gMB_500->GetN();
  NPoint = ND0 + NHT + NMB;
  TGraphAsymmErrors *pp500All_62 = new TGraphAsymmErrors(NPoint);
  cout<<"=========check 1========"<<endl;
  if(!combineGraphs(pp500All_62,gD0_500_62,gHT_500_62,gMB_500_62)) return;

  /*before renormalization*/

  pp500All_62->Fit(flevy500_62);
  //float scalefactor =flevy200->Integral(0,10)/flevy500_62->Integral(0,10);
  float scalefactor =flevy200->Integral(0,20)/flevy500_62->Integral(0,20);
  cout<<"scale factor is : "<<scalefactor<<endl;
  scaleGraph(pp500All_62,scalefactor); //pp500All_62 is the final pp reference(after scaled and renormalization)
  scaleGraph(gD0_500_62,scalefactor);
  scaleGraph(gHT_500_62,scalefactor);
  scaleGraph(gMB_500_62,scalefactor);

  /*After renormalization*/

  TF1 *flevy500_63 = (TF1 *)flevy500_62->Clone("flevy500_63");
  pp500All_62->Fit(flevy500_63);
  gD0_500_62->Draw("psame");
  gHT_500_62->Draw("psame");
  gMB_500_62->Draw("psame");
  flevy500_63->Draw("same");

  TLegend *leg500_62 = new TLegend(0.50,0.67,0.95,0.93);
  leg500_62->SetFillColor(10);
  leg500_62->SetTextFont(42);
  leg500_62->SetTextColor(4);
  leg500_62->AddEntry(gHT_500_62,"d#sigma_{D*} / 0.224 - BHT1","lep");
  leg500_62->AddEntry(gMB_500_62,"d#sigma_{D*} / 0.224 - MB","lep");
  leg500_62->AddEntry(gD0_500_62,"d#sigma_{D^{0}} / 0.565 - MB","lep");
  //leg500_62->AddEntry(flevy500_62,"Levi-fit to All Data", "l");
  leg500_62->AddEntry(flevy500_63,"Levi-fit to All Data", "l");
  //leg500_62->AddEntry(grshade,"FONLL #mu_{R} = #mu_{F} = 1.5 GeV/c^{2}","f");
  //leg500_62->AddEntry(grshade127,"FONLL #mu_{R} = #mu_{F} = 1.27 GeV/c^{2}","f");
  leg500_62->SetTextSize(0.03);
  leg500_62->SetBorderSize(0);
  leg500_62->SetFillStyle(0);
  leg500_62->Draw();
  sprintf(latex,"Run11 pp500GeV");
  drawLatex(0.51,0.94,latex,62,0.04,4);
  savepic(cc,"pic","Renormalizepp500topp200");

  /*================================================================
  | work number:7                                                   |
  | 1>accumulate pp200 and pp500 data point                         |
  | 2>Use Levy function to fit all data ,and Levy function as       |
  |   the central value.                                            |
   ================================================================*/

  cc->cd();  
  cc->SetLogy(1);
  cc->SetGridx(0);
  cc->SetGridy(0);
  setpad(cc,0.125,0.02,0.02,0.10);
 
  TH2D *h71 = new TH2D("h71","Data vs FONLL;p_{T} [GeV/c];d^{2}#sigma^{pp}_{c#bar{\c}})/(2#pip_{T}dp_{T}dy) [mb(GeV/c)^{-2}]",200,0,20,100,1e-10,1e-1);
  h71->GetYaxis()->SetTitleOffset(1.5);
  h71->Draw();

  Int_t Npp200 = pp200All->GetN();
  Int_t Npp500 = pp500All_62->GetN();
  NPoint = Npp200 + Npp500;
  TGraphAsymmErrors *pp200All_62 = new TGraphAsymmErrors(Npp200);
  if(!GraphErrorsToGraphAsymmErrors(pp200All,pp200All_62)) return;
  TGraphAsymmErrors *ppAll = new TGraphAsymmErrors(NPoint);
  cout<<"==========================================="<<endl;
  cout<<"Accumulate pp500 and pp200 data point...."<<endl;
  if(!combineGraphs(ppAll,pp200All_62,pp500All_62)) return;  
  ppAll->Sort();
  cout<<"==========================================="<<endl;
  //ppAll->SetMarkerStyle(24);
  //ppAll->SetMarkerColor(4);
  //ppAll->Draw("psame");
  TF1 *flevy = (TF1 *)flevy500_63->Clone("flevy");
  ppAll->Fit(flevy);
  //==============
  //sys error band 
  TFile *f72 = new TFile("ppLevy_trigaus_sys.root");
  TH2D *hFitErr = (TH2D *)f72->Get("hFitErr");
  hFitErr->FitSlicesY();
  TH1D *hFitErr_2 = (TH1D *)gDirectory->Get("hFitErr_2");
  hFitErr_2->SetName("hFitErr_2");
  Int_t NBinsFit = hFitErr_2->GetNbinsX();
  cout<<"NBinsFit  = "<<NBinsFit<<endl;
  TGraph *grFitErr = new TGraph(NBinsFit);
  if(!hist2graph(hFitErr_2,grFitErr)) return;
  //grFitErr->Print();
  TGraph *ppAll_sys = new TGraph(2*NBinsFit);
  for(int i=0;i<NBinsFit;i++)
    { 
      float xu = grFitErr->GetX()[i];
      float yu = flevy->Eval(xu)*(1+grFitErr->GetY()[i]);
      float xd = grFitErr->GetX()[NBinsFit-i-1];
      float yd = flevy->Eval(xd)*(1-grFitErr->GetY()[NBinsFit-i-1]);
      ppAll_sys->SetPoint(i,xu,yu);
      ppAll_sys->SetPoint(NBinsFit+i,xd,yd);
    }

  ppAll_sys->SetFillColor(16);
  ppAll_sys->Draw("fsame");
  gD0_200->Draw("psame");
  gDstar_200->Draw("psame");
  gDstarHT2_200->Draw("psame");
  //TLegend *leg200_71 = new TLegend(0.14,0.2,0.45,0.35);
  TLegend *leg200_71 = new TLegend(0.7,0.67,0.85,0.92);
  leg200_71->SetFillColor(10);
  leg200_71->SetTextFont(42);
  leg200_71->SetTextColor(2);
  leg200_71->AddEntry(gDstarHT2_200,"d#sigma_{D*} / 0.224 - BHT2","lep");
  leg200_71->AddEntry(gDstar_200,"d#sigma_{D*} / 0.224 - MB","lep");
  leg200_71->AddEntry(gD0_200,"d#sigma_{D^{0}} / 0.565 - MB","lep");
  leg200_71->SetTextSize(0.03);
  leg200_71->SetBorderSize(0);
  leg200_71->SetFillStyle(0);
  leg200_71->Draw();
  sprintf(latex,"Run12 pp200GeV");
  drawLatex(0.7,0.93,latex,62,0.04,2); 

  //Draw pp500GeV data point
  gD0_500_62->Draw("PEsame");
  gHT_500_62->Draw("PEsame");
  gMB_500_62->Draw("PEsame");
  //flevy500_62->Draw("same");
  flevy->Draw("same");
  TLegend *leg50071 = new TLegend(0.50,0.67,0.65,0.92);
  leg50071->SetFillColor(10);
  leg50071->SetTextFont(42);
  leg50071->SetTextColor(4);
  leg50071->AddEntry(gHT_500_62,"d#sigma_{D*} / 0.224 - BHT1","lep");
  leg50071->AddEntry(gMB_500_62,"d#sigma_{D*} / 0.224 - MB","lep");
  leg50071->AddEntry(gD0_500_62,"d#sigma_{D^{0}} / 0.565 - MB","lep");
  //leg50071->AddEntry(flevy500_62,"Levi-fit to All Data", "l");
  leg50071->AddEntry(flevy,"Levi-fit to All Data", "l");
  leg50071->AddEntry(ppAll_sys,"Levy-fit Uncertainties","f");
  leg50071->SetTextSize(0.03);
  leg50071->SetBorderSize(0);
  leg50071->SetFillStyle(0);
  leg50071->Draw();
  sprintf(latex,"Run11 pp500GeV");
  drawLatex(0.5,0.93,latex,62,0.04,4);
  savepic(cc,"pic","fitAll");
  ofstream outdata("ppAll.dat");
  NPoint = ppAll->GetN();
  ppAll->Sort();
  for(int i=0;i<NPoint;i++)
    { 
      float x = ppAll->GetX()[i];
      float y = ppAll->GetY()[i];      
      float eyl = ppAll->GetEYlow()[i];
      float eyh = ppAll->GetEYhigh()[i];
      outdata<<x<<" "<<y<<" "<<eyl<<" "<<eyh<<" "<<0.0<<" "<<0.0<<endl;
    }
  outdata.close();
  f2->Close();  
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

Double_t hist2fun(Double_t *x,Double_t *par,TH1 *hist)
{
  int tbin = (int)(x[0]*1000/1.6+0.5);
  float tlw = hist->GetBinCenter(tbin);
  float tup = hist->GetBinCenter(tbin+1);

  float a1 = hist->GetBinContent(tbin);
  float a2 = hist->GetBinContent(tbin+1);
  float p0 = (a1-a2)/(tlw-tup);
  float p1 = (a2*tlw-a1*tup)/(tlw-tup);

  return par[0]*(p0*x[0]+p1);

}

void scaleGraph(TGraphErrors *gr,float scale)
{
  Int_t NPoint = gr->GetN();
  for(int i=0;i<NPoint;i++)
    {
      float x = gr->GetX()[i];
      float y = gr->GetY()[i];      
      float  ex = gr->GetEX()[i];
      float ey = gr->GetEY()[i];
      y = scale*y;
      gr->SetPoint(i,x,y);
      gr->SetPointError(i,ex,ey*scale);
    }
}

void scaleGraph(TGraphAsymmErrors *gr,float scale)
{
  Int_t NPoint = gr->GetN();
  for(int i=0;i<NPoint;i++)
    {
      float x = gr->GetX()[i];
      float y = gr->GetY()[i];      
      float exl = gr->GetEXlow()[i];
      float eyl = gr->GetEYlow()[i];
      float exh = gr->GetEXhigh()[i];
      float eyh = gr->GetEYhigh()[i];
      y = scale*y;
      gr->SetPoint(i,x,y);
      gr->SetPointError(i,exl,exh,eyl*scale,eyh*scale);
    }
}

Int_t hist2graph(TH1F *h,TGraph *gr)
{
  Int_t NBins = h->GetNbinsX();
  Int_t NPoint = gr->GetN();
  if(NBins!=NPoint)
    {
      cout<<"Input histogram and Tgraph do not have the same number of point."<<endl;
      return 0;
    }
  else 
    {
      for(int i=0;i<gr->GetN();i++)
	{
	  float x = h->GetBinCenter(i+1);
	  float y = h->GetBinContent(i+1);
	  gr->SetPoint(i,x,y);
	}
      return 1;
    }
}

Int_t hist2graph(TH1D *h,TGraph *gr)
{
  Int_t NBins = h->GetNbinsX();
  Int_t NPoint = gr->GetN();
  if(NBins!=NPoint)
    {
      cout<<"Input histogram and Tgraph do not have the same number of point."<<endl;
      return 0;
    }
  else 
    {
      for(int i=0;i<gr->GetN();i++)
	{
	  float x = h->GetBinCenter(i+1);
	  float y = h->GetBinContent(i+1);
	  gr->SetPoint(i,x,y);
	}
      return 1;
    }
}

Int_t combineGraphs(TGraphErrors *grAll,TGraphErrors *gr1,TGraphErrors *gr2,TGraphErrors *gr3)
{
  Int_t N1 = gr1->GetN();
  Int_t N2 = gr2->GetN();
  Int_t N3 = gr3->GetN();
  Int_t NPoint = grAll->GetN();
  if(NPoint!=(N1 + N2 + N3)) {cout<<"The number of point in final graph is not correct , please check it !"<<endl;return 0;}
  
  for(int i=0;i<N1;i++)
    { 
      float x = gr1->GetX()[i];
      float y = gr1->GetY()[i];      
      float ex = gr1->GetEX()[i];
      float ey = gr1->GetEY()[i];
      grAll->SetPoint(i,x,y);
      grAll->SetPointError(i,ex,ey);
    }
  for(int i=0;i<N2;i++)
    { 
      float x = gr2->GetX()[i];
      float y = gr2->GetY()[i];      
      float ex = gr2->GetEX()[i];
      float ey = gr2->GetEY()[i];
      grAll->SetPoint(N1+i,x,y);
      grAll->SetPointError(N1+i,ex,ey);
    }
  for(int i=0;i<N3;i++)
    { 
      float x = gr3->GetX()[i];
      float y = gr3->GetY()[i];      
      float ex = gr3->GetEX()[i];
      float ey = gr3->GetEY()[i];
      grAll->SetPoint(N1+N2+i,x,y);
      grAll->SetPointError(N1+N2+i,ex,ey);
    }
  cout<<"Print All point......."<<endl;
  //grAll->Sort();
  grAll->Print();
  return 1;
}

Int_t combineGraphs(TGraphAsymmErrors *grAll,TGraphAsymmErrors *gr1,TGraphAsymmErrors *gr2,TGraphAsymmErrors *gr3)
{
  Int_t N1 = gr1->GetN();
  Int_t N2 = gr2->GetN();
  Int_t N3 = gr3->GetN();
  Int_t NPoint = grAll->GetN();
  if(NPoint!=(N1 + N2 + N3)) {cout<<"The number of point in final graph is not correct , please check it !"<<endl;return 0;}
  
  for(int i=0;i<N1;i++)
    { 
      float x = gr1->GetX()[i];
      float y = gr1->GetY()[i];      
      float exl = gr1->GetEXlow()[i];
      float eyl = gr1->GetEYlow()[i];
      float exh = gr1->GetEXhigh()[i];
      float eyh = gr1->GetEYhigh()[i];
      grAll->SetPoint(i,x,y);
      grAll->SetPointError(i,exl,exh,eyl,eyh);
    }
  for(int i=0;i<N2;i++)
    { 
      float x = gr2->GetX()[i];
      float y = gr2->GetY()[i];      
      float exl = gr2->GetEXlow()[i];
      float eyl = gr2->GetEYlow()[i];
      float exh = gr2->GetEXhigh()[i];
      float eyh = gr2->GetEYhigh()[i];

      grAll->SetPoint(N1+i,x,y);
      grAll->SetPointError(N1+i,exl,exh,eyl,eyh);
    }
  for(int i=0;i<N3;i++)
    { 
      float x = gr3->GetX()[i];
      float y = gr3->GetY()[i];      
      float exl = gr3->GetEXlow()[i];
      float eyl = gr3->GetEYlow()[i];
      float exh = gr3->GetEXhigh()[i];
      float eyh = gr3->GetEYhigh()[i];
      grAll->SetPoint(N1+N2+i,x,y);
      grAll->SetPointError(N1+N2+i,exl,exh,eyl,eyh);
    }
  cout<<"Print All point......."<<endl;
  //grAll->Sort();
  grAll->Print();
  return 1;
}

Int_t combineGraphs(TGraphAsymmErrors *grAll,TGraphAsymmErrors *gr1,TGraphAsymmErrors *gr2)
{

  Int_t N1 = gr1->GetN();
  Int_t N2 = gr2->GetN();
  Int_t NPoint = grAll->GetN();
  if(NPoint!=(N1+N2)) {cout<<"The number of point in final graph is not correct , please check it !"<<endl;return 0;}
  for(int i=0;i<N1;i++)
    { 
      float x = gr1->GetX()[i];
      float y = gr1->GetY()[i];      
      float exl = gr1->GetEXlow()[i];
      float eyl = gr1->GetEYlow()[i];
      float exh = gr1->GetEXhigh()[i];
      float eyh = gr1->GetEYhigh()[i];
      grAll->SetPoint(i,x,y);
      grAll->SetPointError(i,exl,exh,eyl,eyh);
    }
  for(int i=0;i<N2;i++)
    { 
      float x = gr2->GetX()[i];
      float y = gr2->GetY()[i];      
      float exl = gr2->GetEXlow()[i];
      float eyl = gr2->GetEYlow()[i];
      float exh = gr2->GetEXhigh()[i];
      float eyh = gr2->GetEYhigh()[i];
      grAll->SetPoint(N1+i,x,y);
      grAll->SetPointError(N1+i,exl,exh,eyl,eyh);
    }
  cout<<"Print All point......."<<endl;
  //grAll->Sort();
  grAll->Print();
  return 1;
}


Int_t combineGraphs(TGraphErrors *grAll,TGraphErrors *gr1,TGraphErrors *gr2)
{

  Int_t N1 = gr1->GetN();
  Int_t N2 = gr2->GetN();
  Int_t NPoint = grAll->GetN();
  if(NPoint!=(N1+N2)) {cout<<"The number of point in final graph is not correct , please check it !"<<endl;return 0;}
  for(int i=0;i<N1;i++)
    { 
      float x = gr1->GetX()[i];
      float y = gr1->GetY()[i];      
      float ex = gr1->GetEX()[i];
      float ey = gr1->GetEY()[i];
      grAll->SetPoint(i,x,y);
      grAll->SetPointError(i,ex,ey);
    }
  for(int i=0;i<N2;i++)
    { 
      float x = gr2->GetX()[i];
      float y = gr2->GetY()[i];      
      float ex = gr2->GetEX()[i];
      float ey = gr2->GetEY()[i];
      grAll->SetPoint(N1+i,x,y);
      grAll->SetPointError(N1+i,ex,ey);
    }
  cout<<"Print All point......."<<endl;
  //grAll->Sort();
  grAll->Print();
  return 1;
}

Int_t addShade(TGraph *shade,TGraph *up,TGraph *down)
{
  Int_t N1 = up->GetN();
  Int_t N2 = down->GetN();
  Int_t N  = shade->GetN();
  if(N1!=N2 || N!=(N1+N2))
    {
      cout<<"Input two graph do not have equal point. please check it ! "<<endl;
      return 0;
    }
  Int_t NPoint = N1;

  for(int i=0;i<NPoint;i++)
    { 
      float xu = up->GetX()[i];
      float yu = up->GetY()[i];      
      float xd = down->GetX()[NPoint-i-1];
      float yd = down->GetY()[NPoint-i-1];
      shade->SetPoint(i,xu,yu);
      shade->SetPoint(NPoint+i,xd,yd);
    }
  shade->SetFillColor(16);
  return 1;
}

//=================
// special function
Int_t GraphErrorsToGraphAsymmErrors(TGraphErrors *gr,TGraphAsymmErrors *gar)
{
  int ngr = gr->GetN();
  int ngar = gar->GetN();
  Style_t nstyle = gr->GetMarkerStyle();
  Size_t nsize = gr->GetMarkerSize();
  Color_t ncolor = gr->GetMarkerColor();
  if(ngr!=ngar) {cout<<"Input Graph and output Graph have different point number. "<<endl; return 0;}
  for(int i=0;i<ngr;i++)
    {
      float x = gr->GetX()[i];
      float y = gr->GetY()[i];
      float ex = gr->GetEX()[i];
      float ey = gr->GetEY()[i];
      gar->SetPoint(i,x,y);
      gar->SetPointError(i,ex,ex,ey,ey);
    }
  gar->SetMarkerStyle(nstyle);
  gar->SetMarkerSize(nsize);
  gar->SetMarkerColor(ncolor);
  return 1;
}

 
Int_t FONLLscale(TGraphAsymmErrors *gar,TGraph *gr, TGraph *gr_u, TGraph *gr_d)
{
  int ngar = gar->GetN();
  int ngr = gr->GetN();
  int ngru = gr_u->GetN();
  int ngrd = gr_d->GetN();
  if(ngr!=ngru || ngr!=ngrd) {cout<<"Input Graphs have different point number"<<endl; return 0;}
  for(int i=0;i<gar->GetN();i++)
    { 
      float x = gar->GetX()[i];
      float y = gar->GetY()[i];      
      float exl = gar->GetEXlow()[i];
      float eyl = gar->GetEYlow()[i];
      float exh = gar->GetEXhigh()[i];
      float eyh = gar->GetEYhigh()[i];

      float scalef = gr->Eval(x,0,"s");
      float Y = y/scalef;
      float rexl = 0;
      float reyl = scalef - gr_d->Eval(x,0,"s");
      float rexh = 0;
      float reyh = gr_u->Eval(x,0,"s") - scalef;
      float EYL = Y*sqrt(pow(eyl/y,2)+pow(reyl/scalef,2));
      float EYH = Y*sqrt(pow(eyh/y,2)+pow(reyh/scalef,2));
      gar->SetPoint(i,x,Y);
      gar->SetPointError(i,exl,exh,EYL,EYH);
    }
  return 1;
} 

void FONLLscale(TGraphErrors *gr,TGraph *ratio)
{
  for(int i=0;i<gr->GetN();i++)
    { 
      float x = gr->GetX()[i];
      float y = gr->GetY()[i];      
      float  ex = gr->GetEX()[i];
      float ey = gr->GetEY()[i];
      float scalef = 1./ratio->Eval(x,0,"s");
      y = scalef*y;
      gr->SetPoint(i,x,y);
      gr->SetPointError(i,ex,ey*scalef);
    }
} 


Int_t graphTographerror(TGraphErrors *gr,TGraph *gr1,TGraph *gr_u, TGraph *gr_d)
{

}
//===========================================================================
//user-defined function, used to do some detail work(not an general function)

/*===================================================
| Work No.5                                          |
| Scale pp500 to pp200 with FONLL Calculate results  | 
 ===================================================*/
void ScalePP500toPP200(TGraphErrors *pp500,TGraphErrors *ratio)
{
  //FONLL Calculate using charm mass 1.27GeV/C^2


}
/*===========================================================
| Work No.6                                                  |
| 1>scale pp500 to pp200 with FONLL central value(work No.5) | 
| 2>but the shape is not consistente,so we will use Levy     |
|   function to fit scaled pp500 data point.                 |
| 3>Use fit pp500 data point Levy function to scale pp500 to |
|   pp200(over all normalnition)                             | 
 ===========================================================*/
void OverAllNormalization()
{

}
/*===============================
|          Work No.8             |
| Accumulate Ratios uncertaintys | 
 ===============================*/

void AddRatioError()
{
}
