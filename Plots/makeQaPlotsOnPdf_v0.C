//===============================
//Version: 2.0 
//Time: Fri Mar 13 13:47:41 EDT 2015 
//Author: Long Zhou 
//Discribe: General make QA plots to PDF. Removed All PXL related Plots  

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "iomanip.h"
#include <math.h>
#include <TH1.h>
#include <TH2.h>
#include <TFile.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TText.h>
#include <TLine.h>
#include <TPDF.h>
#include <TBox.h>
#include <TKey.h>
#include <TPad.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TString.h>
#include <TROOT.h>

void setpad(TVirtualPad *pad,float left, float right, float top, float bottom, int logy = 0, int logz = 0);
TLegend* myLeg(Double_t xlow, Double_t ylow, Double_t xup, Double_t yup,Int_t textFont=42,Double_t textSize=0.05);

void makeQaPlotsOnPdf(const char* indir="out",const char*outdir="PDF",const char* FileName="Test",Int_t nPlots = 4)
{
  gROOT->Reset();
  setcolz();
  setstyle();   
  TGaxis::SetMaxDigits(3);

  char name[100];
  sprintf(name, "%s.root", FileName);
  TFile *f = new TFile(name);

  TCanvas *c1 = new TCanvas("c1", "c1",0,0,800,800);
  setpad(c1,0.001,0.001,0.001,0.001, 0, 0);
  TPad *pad = new TPad("pad","",0.00,0.00,1.00,1.00);
  pad->Draw();

  TIter nextkey(f->GetListOfKeys());
  TObject *t;

  sprintf(name, "%s.pdf", FileName);
  TPDF *pdf = new TPDF(name);
  //Set front page
  c1->cd();
  pad->cd();
  setpad(gPad,0.1,0.1,0.05,0.12, 0, 0);
  TBox *bLabel = new TBox(0.01, 0.88, 0.99, 0.99);
  bLabel->SetFillColor(kBlack);
  bLabel->Draw();
  TLatex tl;
  tl.SetNDC();
  tl.SetTextColor(kWhite);
  tl.SetTextSize(0.063);
  char tlName[100];
  char tlName2[100];

  TString titlename = FileName;
  int found = titlename.Last('/');
  if(found >= 0){
    titlename.Replace(0, found+1, "");
  } 
  sprintf(tlName, "Universer QA");
  tl.SetTextSize(0.075);
  tl.SetTextColor(kBlack);

  TBox *bFoot = new TBox(0.01, 0.01, 0.99, 0.12);
  bFoot->SetFillColor(kBlack);
  bFoot->Draw();
  tl.SetTextColor(kWhite);
  tl.SetTextSize(0.05);
  tl.DrawLatex(0.05, 0.05, (new TDatime())->AsString());
  tl.SetTextColor(kBlack);
  tl.SetTextSize(0.1);
  tl.DrawLatex(0.15, 0.55, titlename);
  c1->cd();
  c1->Update();
  pdf->NewPage();

  c1->cd();
  pad->Divide(1,2,0.001,0.001);
  int nCount2 = 0;
  int nFig = 0;
  TKey *key;
  TLine line;
  line.SetLineStyle(7);
  line.SetLineWidth(1);
  TString tName;
  tl.SetTextSize(0.035);
  //Loop through TH1 and TH2 histos and place them on PDF
  while ((key = (TKey*)nextkey())) {
    t = dynamic_cast<TObject*>(key->ReadObj());
    if(t){
      sprintf(name,"%s.gif",t->GetName());
      if(!strcmp(t->ClassName(),"TH1D")||!strcmp(t->ClassName(),"TH1I")||!strcmp(t->ClassName(),"TH1S")||!strcmp(t->ClassName(),"TH1F")){
	TH1F *h1 = (TH1F*)t;
	//if(h1->GetEntries()>0){
	if(nCount2==0){
	  if(nFig>0)pdf->NewPage();
	  pad->cd();
	  pad->Clear();
	  c1->cd();
	  pad->cd();
	  pad->Divide(2,2,0.001,0.001);
	  pad->cd(1);
	  setpad(gPad,0.1,0.2,0.1,0.15, 0, 0);
	}
	else if(nCount2==1){
	  pad->cd(2);
	  setpad(gPad,0.1,0.2,0.1,0.15, 0, 0);
	}
	else if(nCount2==2){
	  pad->cd(3);
	  setpad(gPad,0.1,0.2,0.1,0.15, 0, 0);
	}
	else if(nCount2==3){
	  pad->cd(4);
	  setpad(gPad,0.1,0.2,0.1,0.15, 0, 0);
	}
	h1->GetYaxis()->SetTitleOffset(1.3);
	tName = t->GetName();
	t->Draw();
	nFig++;
	nCount2++;
	if(nCount2==4){
	  sprintf(tlName, "Page %i", nFig/4+1);
	  tl.DrawLatex(0.02, 0.02, tlName);
	  c1->cd();
	  c1->Update();
	  nCount2=0;    	
	}
      }	
      else if(!strcmp(t->ClassName(),"TProfile")){
	TProfile *hp = (TProfile*)t;
	//if(hp->GetEntries()>0){
	if(nCount2==0){
	  if(nFig>0)pdf->NewPage();
	  pad->cd();
	  pad->Clear();
	  c1->cd();
	  pad->cd();
	  pad->Divide(2,2,0.001,0.001);
	  pad->cd(1);
	  setpad(gPad,0.1,0.2,0.1,0.15, 1, 0);
	}
	else if(nCount2==1){
	  pad->cd(2);
	  setpad(gPad,0.1,0.2,0.1,0.15, 1, 0);
	}
	else if(nCount2==2){
	  pad->cd(3);
	  setpad(gPad,0.1,0.2,0.1,0.15, 1, 0);
	}
	else if(nCount2==3){
	  pad->cd(4);
	  setpad(gPad,0.1,0.2,0.1,0.15, 1, 0);
	}
	tName = t->GetName();
	hp->GetYaxis()->SetTitleOffset(1.3);
	t->Draw("hist");
	nFig++;
	nCount2++;
	if(nCount2==4){
	  sprintf(tlName, "Page %i", nFig/4+1);
	  tl.DrawLatex(0.02, 0.02, tlName);
	  c1->cd();
	  c1->Update();
	  nCount2=0;
	}
	//}
      }   
      else if(!strcmp(t->ClassName(),"TH2D")||!strcmp(t->ClassName(),"TH2I")||!strcmp(t->ClassName(),"TH2F")||!strcmp(t->ClassName(),"TH2S"))
	{
	  TH2F *h2 = (TH2F*)t;
	  tName = t->GetName();
	  h2->GetYaxis()->SetTitleOffset(1.3);

	  if(nCount2==0){
	    if(nFig>0)pdf->NewPage();
	    pad->cd();
	    pad->Clear();
	    c1->cd();
	    pad->cd();
	    pad->Divide(2,2,0.001,0.001);
	    pad->cd(1);
	    setpad(gPad,0.1,0.2,0.1,0.15, 0, 1);
	  }
	  else if(nCount2==1){
	    pad->cd(2);
	    setpad(gPad,0.1,0.2,0.1,0.15, 0, 1);
	  }
	  else if(nCount2==2){
	    pad->cd(3);
	    setpad(gPad,0.1,0.2,0.1,0.15, 0, 1);
	  }
	  else if(nCount2==3){
	    pad->cd(4);
	    setpad(gPad,0.1,0.2,0.1,0.15, 0, 1);
	  }

	  t->Draw("COLZ");
	  nFig++;
	  nCount2++;
	  if(nCount2==4){
	  sprintf(tlName, "Page %i", nFig/4+1);
	  tl.DrawLatex(0.02, 0.02, tlName);
	  c1->cd();
	  c1->Update();
	  nCount2=0;    	
	}

      }
    }
  }


  //-----------
  pdf->Close();
  f->Close(); 
  return;
}
void setpad(TVirtualPad *pad,float left, float right, float top, float bottom, int logy, int logz){
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
  pad->SetLogy(logy);
  pad->SetLogz(logz);
  return;
}
TLegend* myLeg(Double_t xlow, Double_t ylow, Double_t xup, Double_t yup, Int_t textFont, Double_t textSize)
{
  TLegend *leg = new TLegend(xlow,ylow,xup,yup);
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);
  leg->SetFillColor(10);
  leg->SetTextFont(textFont);
  leg->SetTextSize(textSize);
  return leg;
}
void setcolz()
{
  const Int_t NRGBs=5;
  const Int_t NCont=32;

  Double_t stops[NRGBs]={0.00,0.34,0.61,0.84,1.00};
  Double_t red[NRGBs]={0.00,0.00,0.87,1.00,0.51};
  Double_t green[NRGBs]={0.00,0.81,1.00,0.20,0.00};
  Double_t blue[NRGBs]={0.51,1.00,0.12,0.00,0.00};
  TColor::CreateGradientColorTable(NRGBs,stops,red,green,blue,NCont);
  gStyle->SetNumberContours(NCont);
}
setstyle()
{
  TStyle* myStyle = new TStyle("myStyle","Styles");
  myStyle->SetPalette(1,0); // avoid horrible default color scheme
  myStyle->SetOptStat("e");
  myStyle->SetOptTitle(1);
  myStyle->SetOptDate(0);
  myStyle->SetLabelSize(0.045,"xyz"); // size of axis value font
  myStyle->SetTitleX(0.2f);
  myStyle->SetTitleY(0.96f);
  myStyle->SetTitleW(0.5f);
  myStyle->SetTickLength(0.01,"xyz");
  myStyle->SetTitleFont(62,"xyz"); // font option 
  myStyle->SetLabelFont(62,"xyz");
  myStyle->SetTitleOffset(0.8,"z");
  myStyle->SetTitleOffset(1.2,"y");
  myStyle->SetTitleFillColor(10);
  myStyle->SetLineWidth(2);
  myStyle->SetCanvasDefW(700);
  myStyle->SetCanvasDefH(600);
  myStyle->SetCanvasColor(0);// canvas...
  myStyle->SetCanvasBorderMode(0);
  myStyle->SetCanvasBorderSize(0);
  myStyle->SetPadColor(0);
  myStyle->SetPadBorderSize(1);
  myStyle->SetPadBorderMode(-1);
  myStyle->SetPadBottomMargin(0.14); //margins...
  myStyle->SetPadTopMargin(0.06);
  myStyle->SetPadLeftMargin(0.14);
  myStyle->SetPadRightMargin(0.04);
  myStyle->SetPadGridX(0); // grids, tickmarks
  myStyle->SetPadGridY(0);
  myStyle->SetPadTickX(1);
  myStyle->SetPadTickY(1);
  myStyle->SetFrameBorderSize(1);
  myStyle->SetFrameBorderMode(-1);
  myStyle->SetFrameFillColor(0);
  myStyle->SetFrameLineWidth(1.2);
  myStyle->SetPaperSize(20,24); // US letter size
  gROOT->SetStyle("myStyle");
  cout << "Styles are Set!" << endl;

}

