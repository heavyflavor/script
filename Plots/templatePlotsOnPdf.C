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
#define TxDefSize   0.05
using namespace std;
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
//-----------------------------------------------------
void templatePlotsOnPdf(TString input1,TString input2,TString input3,TString output)
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
  gStyle->SetOptTitle(1);
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
  output += ".pdf";
  char *dir="pic";
  TPDF *pdf = new TPDF(output);  
  input1 += ".out.root";
  TFile *f1 = new TFile(input1);

  //------------------------------------------------------------
  //First page dirscribution, put important information in here.
  
  char *Trigger="PedAdPhys_tcd_only";
  char *TriggerRate="100hz";
  char *Workdir="/star/u/zhoulong/data02/DEV14/SST/PowerEff";
  char *InternalBusy = "15000";
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
  tx.DrawLatex(0.05,0.3,"Low Luminosity Run");

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
  tx.DrawLatex(0.03, 0.85,Form("Physcis Run : %s(2.4V)",input1.Data()));
  tx.DrawLatex(0.03, 0.75,Form("File Name : %s(2.5V)",input2.Data()));
  tx.DrawLatex(0.03, 0.65,Form("File Name : %s(2.6V)",input3.Data()));
  tx.DrawLatex(0.03, 0.55,Form("Trigger : %s(All)",Trigger));
  tx.DrawLatex(0.03, 0.45,"Power supply: 2.4V,2.5V,2.6 V");
  //  tx.SetTextSize(0.05);
  tx.DrawLatex(0.03, 0.35,"Power setting: new 50 meter setting(All)");
  tx.DrawLatex(0.03, 0.25,Form("Trigger Rate : %s(All)",TriggerRate));
  tx.DrawLatex(0.03,0.15,Form("Internal Busy : %s(All)",InternalBusy));
  tx.DrawLatex(0.03,0.05,Form("Work dir: %s",Workdir));
  
  float ty1 = 0.58;
  float ty2 = 0.44;
  float dy  = 0.06;
  //-----------------------------------
  //Start draw your plots in pdf files
  cc->cd();
  cc->Update();
  pdf->NewPage();
  cc->Divide(3,1);
  cc->cd(1);
  //h1->Draw()
  drawLatex(0.6,0.95,"2.4V",62,0.06,2);
  cc->cd(2);
  //h2->Draw();
  drawLatex(0.6,0.95,"2.5V",62,0.06,2);
  cc->cd(3);
  //h3->Draw();
  drawLatex(0.6,0.95,"2.6V",62,0.06,2);
  cc->cd();
  cc->Update();
  pdf->Close();

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
