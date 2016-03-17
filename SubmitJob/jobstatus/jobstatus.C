#include <stdio>
#include <stdlib>
#include <iostream>
#include <fstream>
#include <math.h>
#include "iomanip.h"

#include "TROOT.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TF1.h"
#include "TMath.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "TStyle.h"

TH1D* histo(char *name, Double_t xlow, Double_t xup, Double_t ylow, Double_t yup, char* xTitle, char* yTitle);
TLatex* drawLatex(Double_t x, Double_t y, char* text, Int_t textFont, Double_t textSize, Int_t colorIndex);
TLine* drawLine(Double_t xlow,Double_t ylow, Double_t xup, Double_t yup, Double_t lineWidth, Int_t lineColor);
void drawLines(Double_t xlow,Double_t ylow, Double_t xup, Double_t yup, Double_t lineWidth, Int_t lineColor);
void drawBoxes(Int_t n, Double_t* x1, Double_t* x2, Double_t* y1, Double_t* y2, Int_t lineWidth, Int_t lineColor);
void setpad(TPad *pad,float left, float right, float top, float bottom);
void ratio(TGraph *gr1, TGraph *gr2);
void ratio(TGraphErrors *gr1, TGraph *gr2);
void ratio(TGraphAsymmErrors *gr1, TGraph *gr2);

Double_t myExp(Double_t *x, Double_t *par)
{
  return par[0]*exp(-(x[0]-par[1])/par[2])/par[2]*par[3];
}

Double_t myGaus(Double_t *x, Double_t *par)
{
  return par[0]*TMath::Gaus(x[0], par[1], par[2], kTRUE)*par[3];
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
Double_t SPEFun2(Double_t *x, Double_t *par) {
  Double_t x2[1];
  x2[0] = par[7]-x[0];
  return SPEFun(x2,par)*TMath::Gaus(x[0],0.,par[8],1);
  //return TMath::Gaus(x[0],0.,par[8],1);
}
Double_t SPEFunGaus(Double_t *x, Double_t *par)
{
  TF1 *fun = new TF1("fun",SPEFun2,-4000,4000,9);
  fun->SetParameters(par);
  fun->SetParameter(7,x[0]);
  fun->SetParameter(8,par[7]);
  Double_t val = fun->Integral(-5,5);
  //Double_t val = fun->Eval(0.);
  delete fun;
  return val;
}

void jobstatus(TString input="jobstatus.txt",TString time="201503261640")
{
  char buf[1024];
  gStyle->Reset("plain");
  gStyle->SetPalette(0);
  gStyle->SetOptStat(00000);
  TCanvas *c1 = new TCanvas("c1","Job monitor",1200,800);
  setpad(c1,0.09,0.02,0.02,0.10);
  gPad->SetLogy(1);

  Double_t x1 = 86;
  const Double_t dx = 10;
  const Double_t x2 = 160;

  TH1D *hStatus  = new TH1D("hStatus",";Running Percent;Number of Jobs",100,0,100);

  Double_t qdc;
  sprintf(buf,"%s",input.Data());
  ifstream indata(buf);
  if(!indata) {
    cout<<buf<<"doesn't exist!"<<endl;
    return;
  }
  while(indata>>qdc) {
    hStatus->Fill(qdc);
  }
  indata.close();

  hStatus->SetFillColor(3);
  hStatus->Draw("HISTTEXT");

  sprintf(buf,"L. Zhou On RCF");
  drawLatex(0.7,0.9,buf,62,0.04,2);
  sprintf(buf,"%d jobs finished",hStatus->GetBinContent(100));
  drawLatex(0.7,0.8,buf,62,0.04,4);
  sprintf(buf,"%d jobs running",hStatus->GetEntries()-hStatus->GetBinContent(100));
  drawLatex(0.7,0.7,buf,62,0.04,3);
  //char *time=gSystem->Exec("date +%Y%m%d%H%M%S")
  sprintf(buf,Form("Jobstatus/jobstatus_%s.gif",time.Data()));
  c1->SaveAs(buf);
  sprintf(buf,Form("Jobstatus/jobstatus_%s.C",time.Data()));
  c1->SaveAs(buf);
}


//===========================================================================
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

//===========================================================================
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
TLatex* drawLatex(Double_t x, Double_t y, char* text, Int_t textFont, Double_t textSize, Int_t colorIndex){
  TLatex *latex = new TLatex(x,y,text);
  latex->SetNDC();
  latex->SetTextFont(textFont);
  latex->SetTextSize(textSize);
  latex->SetTextColor(colorIndex);
  latex->Draw("same");
  return latex;
}

TLine* drawLine(Double_t xlow,Double_t ylow, Double_t xup, Double_t yup, Double_t lineWidth, Int_t lineColor){
  TLine *l1 = new TLine(xlow,ylow,xup,yup);
  l1->SetLineWidth(lineWidth);
  l1->SetLineColor(lineColor);
  l1->Draw("same");
  return l1;
}

void drawLines(Double_t xlow,Double_t ylow, Double_t xup, Double_t yup, Double_t lineWidth, Int_t lineColor){
  drawLine(xlow,ylow,xup,ylow,lineWidth,lineColor);
  drawLine(xlow,yup,xup,yup,lineWidth,lineColor);
  drawLine(xlow,ylow,xlow,yup,lineWidth,lineColor);
  drawLine(xup,ylow,xup,yup,lineWidth,lineColor);
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






