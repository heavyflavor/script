#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include "TFile.h"
#include "TTree.h"
#include "TMath.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TGraph2D.h"
#include "TGraph2DErrors.h"
#include "TStyle.h"
#include "TF1.h"
#include "TH2.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TBrowser.h"
#include "TLine.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TPDF.h"

using std::cout;
using std::endl;

TH1D* histo(char *name, Double_t xlow, Double_t xup, Double_t ylow, Double_t yup, char* xTitle, char* yTitle);
TLatex* drawLatex(Double_t x, Double_t y, char* text, Int_t textFont, Double_t textSize, Int_t colorIndex);
TLine* drawLine(Double_t xlow,Double_t ylow, Double_t xup, Double_t yup, Double_t lineWidth,Int_t lineStyle , Int_t lineColor);
void drawLines(Double_t xlow,Double_t ylow, Double_t xup, Double_t yup, Double_t lineWidth,Int_t lineStyle, Int_t lineColor);
void setpad(float left, float right, float top, float bottom);

void plotsdEdx(char* inFile=""){

	char buf[1024];
	gStyle->Reset("plain");
	gStyle->SetPalette(1);

	bool save = kFALSE;
	save = kTRUE;

	gStyle->SetOptFit(1111);
	gStyle->SetOptStat(0);
	gStyle->SetTitleOffset(1.5,"y");

	TCanvas *c1 = new TCanvas("c1", "c1",0,0,600,600);
	c1->SetFillColor(10);
	c1->SetBorderMode(0);
	c1->SetBorderSize(0);
	c1->SetFrameFillColor(10);
	c1->SetFrameBorderMode(0);
	c1->SetFrameBorderSize(0);
	//c1->SetLogz(1);

	sprintf(buf,"dedx_bichsel.root");
	TFile *f = new TFile(buf);

	c1->cd();
	setpad(0.12,0.10,0.10,0.12);

	TH1D *dEdx_e = (TH1D *)f->Get("dEdx_e");
	dEdx_e->GetXaxis()->SetRangeUser(0.1,12);
	dEdx_e->GetYaxis()->SetRangeUser(0,10.5);
	dEdx_e->SetLineColor(1);
	dEdx_e->SetLineWidth(1.5);

	TH1D *dEdx_mu = (TH1D *)f->Get("dEdx_mu");
	dEdx_mu->GetXaxis()->SetRangeUser(0.1,12);
	dEdx_mu->SetLineColor(2);
	dEdx_mu->SetLineWidth(1.5);

	TH1D *dEdx_pi = (TH1D *)f->Get("dEdx_pi");
	dEdx_pi->GetXaxis()->SetRangeUser(0.1,12);
	dEdx_pi->SetLineColor(kViolet);
	dEdx_pi->SetLineWidth(1.5);

	TH1D *dEdx_k = (TH1D *)f->Get("dEdx_k");
	dEdx_k->GetXaxis()->SetRangeUser(0.23,12);
	dEdx_k->SetLineColor(kBlue);
	dEdx_k->SetLineWidth(1.5);

	TH1D *dEdx_p = (TH1D *)f->Get("dEdx_p");
	dEdx_p->GetXaxis()->SetRangeUser(0.44,12);
	dEdx_p->SetLineColor(kSpring);
	dEdx_p->SetLineWidth(1.5);

	c1->cd();
	c1->SetLogx(1);
	TH1D *d0 = (TH1D *)histo("d0",0.1,12,0,11.5,"p(GeV/c)","dEdx(KeV/cm)");
	d0->Draw("c");
	drawLines(0.1,0,12,11.5,2,1,1);
	dEdx_e->Draw("csame");
	dEdx_mu->Draw("csame");
	dEdx_pi->Draw("csame");
	dEdx_k->Draw("csame");
	dEdx_p->Draw("csame");

	TLegend *leg = new TLegend(0.70,0.60,0.80,0.80);
	leg->SetFillColor(10);
	leg->SetBorderSize(0);
	leg->AddEntry(dEdx_e,"e","pl");
	leg->AddEntry(dEdx_mu,"#mu","pl");
	leg->AddEntry(dEdx_pi,"#pi","pl");
	leg->AddEntry(dEdx_k,"k","pl");
	leg->AddEntry(dEdx_p,"p","pl");
	leg->Draw("same");

	c1->SaveAs("dEdx_bichsel.png");

	cout<<"The program has completed!!"<<endl;

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
	dd->GetYaxis()->SetTitleOffset(0.9);
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

TLine* drawLine(Double_t xlow,Double_t ylow, Double_t xup, Double_t yup, Double_t lineWidth, Int_t lineStyle,Int_t lineColor){
	TLine *l1 = new TLine(xlow,ylow,xup,yup);
	l1->SetLineWidth(lineWidth);
	l1->SetLineColor(lineColor);
	l1->SetLineStyle(lineStyle);
	l1->Draw("same");
	return l1;
}

void drawLines(Double_t xlow,Double_t ylow, Double_t xup, Double_t yup, Double_t lineWidth, Int_t lineStyle,Int_t lineColor){
	drawLine(xlow,ylow,xup,ylow,lineWidth,lineStyle,lineColor);
	drawLine(xlow,yup,xup,yup,lineWidth,lineStyle,lineColor);
	drawLine(xlow,ylow,xlow,yup,lineWidth,lineStyle,lineColor);
	drawLine(xup,ylow,xup,yup,lineWidth,lineStyle,lineColor);
}

void setpad(float left, float right, float top, float bottom){
	gPad->SetFillColor(10);
	gPad->SetBorderMode(0);
	gPad->SetBorderSize(0);
	gPad->SetFrameFillColor(10);
	gPad->SetFrameBorderMode(0);
	gPad->SetFrameBorderSize(0);
	gPad->SetLeftMargin(left);
	gPad->SetRightMargin(right);
	gPad->SetTopMargin(top);
	gPad->SetBottomMargin(bottom);
}






