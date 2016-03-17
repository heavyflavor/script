//#include "Minuit.h"
#include "TH1.h"
#include "TF1.h"
#include "TLegend.h"
#include "TCanvas.h"
#include <stdlib.h>
#include "iostream.h"
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
TH1D *hfcn;
TH1D *HPtPiMs;
TH1D *HPtPMs;
TH1D *HPtKMs;
TH1D *HPtnSig;
TH1D *HPt;
TH1D *HPtMs;
//------------------------------------------------------
Double_t fpi(double* x, double* par)
{
  hfcn = (TH1D *)HPtPiMs->Clone();
  hfcn->SetName("hfcn");
  Double_t un = hfcn->Integral();

  int   ptbin  = hfcn->GetXaxis()->FindBin(x[0]);
  float ptlw   = hfcn->GetBinCenter(ptbin);
  float ptup   = hfcn->GetBinCenter(ptbin+1);
  float a1 = hfcn->GetBinContent(ptbin);
  float a2 = hfcn->GetBinContent(ptbin+1);
  float p0 = (a1-a2)/(ptlw-ptup);
  float p1 = (a2*ptlw-a1*ptup)/(ptlw-ptup);
  
  return par[0]*(p0*x[0]+p1)/un;
}
//----------------------------------------------------
Double_t fp(double* x, double* par)
{
  hfcn = (TH1D *)HPtPMs->Clone();
  hfcn->SetName("hfcn");
  Double_t un = hfcn->Integral();

  int   ptbin  = hfcn->GetXaxis()->FindBin(x[0]);
  float ptlw   = hfcn->GetBinCenter(ptbin);
  float ptup   = hfcn->GetBinCenter(ptbin+1);
  float a1 = hfcn->GetBinContent(ptbin);
  float a2 = hfcn->GetBinContent(ptbin+1);
  float p0 = (a1-a2)/(ptlw-ptup);
  float p1 = (a2*ptlw-a1*ptup)/(ptlw-ptup);
  
  return par[0]*(p0*x[0]+p1)/un;
}
//----------------------------------------------------
Double_t fk(double* x, double* par)
{
  hfcn = (TH1D *)HPtKMs->Clone();
  hfcn->SetName("hfcn");
  Double_t un = hfcn->Integral();

  int   ptbin  = hfcn->GetXaxis()->FindBin(x[0]);
  float ptlw   = hfcn->GetBinCenter(ptbin);
  float ptup   = hfcn->GetBinCenter(ptbin+1);
  float a1 = hfcn->GetBinContent(ptbin);
  float a2 = hfcn->GetBinContent(ptbin+1);
  float p0 = (a1-a2)/(ptlw-ptup);
  float p1 = (a2*ptlw-a1*ptup)/(ptlw-ptup);
  
  return par[0]*(p0*x[0]+p1)/un;
}
//----------------------------------------------------
Double_t fms(Double_t *x,Double_t *par)
{
  Double_t piy[1],py[1],ky[1];
  piy[0] = par[0];
  py[0] = par[1];
  ky[0] = par[2];
  return fpi(x,piy)+fp(x,py)+fk(x,ky);
}
//-----------------------------------------------------
Double_t piGaus(Double_t *x, Double_t *par)
{
  return par[0]*TMath::Gaus(x[0], par[1], par[2], kTRUE);
}

Double_t pGaus(Double_t *x,Double_t *par)
{
  return par[0]*TMath::Gaus(x[0],par[1],par[2],kTRUE);
}

Double_t kGaus(Double_t *x,Double_t *par)
{
  return par[0]*TMath::Gaus(x[0],par[1],par[2],kTRUE);
}

Double_t fsig(Double_t *x,Double_t *par)
{
  Double_t piy[3],py[3],ky[3];
  for(int i=0;i<3;i++)
    {
      piy[i] = par[i];
      py[i] = par[i+3];
      ky[i] = par[i+6];
    }
  return piGaus(x,piy)+pGaus(x,py)+kGaus(x,ky);
}
//-----------------------------------------------------
void pid_new()
{
  TStopwatch SW;
  SW.Start();
  TH1::SetDefaultSumw2();
  const int N  = 1000;
  const int RB = 10;
  Double_t low,hig;
  TFile *f = new TFile("msquare.root");
  TH2D *HMs    = (TH2D *)f->Get("HMs");
  TH2D *HPiMs  = (TH2D *)f->Get("HPiMs");
  TH2D *HPMs   = (TH2D *)f->Get("HPMs");
  TH2D *HKMs   = (TH2D *)f->Get("HKMs");
  TH2D *nSigPi = (TH2D *)f->Get("nSigPi");
  TCanvas *pt = new TCanvas("pt","fit pi,k,p yield in different pt bin",0,0,800,600);
  pt->Divide(4,2);
  char buf0[1024];
  char buf1[1024];
  char buf2[1024];
  char buf3[1024];
  char buf01[1024];
  char buf11[1024];
  char buf21[1024];
  char buf31[1024];
  TH1D *hbuf[4][10];
  HPt = HMs->ProjectionX("HPt");

  for(int i=8;i<=8;i++)
    {
      sprintf(buf0,"ms%d",i);
      sprintf(buf1,"pims%d",i);
      sprintf(buf2,"pms%d",i);
      sprintf(buf3,"kms%d",i);
      sprintf(buf01,"ms%d",i);
      sprintf(buf11,"pims%d",i);
      sprintf(buf21,"pms%d",i);
      sprintf(buf31,"kms%d",i);

      hbuf[0][i] = new TH1D(buf0,"pt vs mass square;m^{2};Entries",1600,-0.3,1.3);
      hbuf[1][i] = new TH1D(buf1,"pt vs Pi mass square;m^{2};Entries",1600,-0.3,1.3);
      hbuf[2][i] = new TH1D(buf2,"pt vs P mass square;m^{2};Entries",1600,-0.3,1.3);
      hbuf[3][i] = new TH1D(buf3,"pt vs K mass square;m^{2};Entries",1600,-0.3,1.3);
      cout<<" begin i = "<<i<<"th projection......."<<endl;
      pt->cd(i);
      if(i<=2)
	{
	  low = HMs->GetXaxis()->FindBin(0.1*i+0.9);
	  cout<<"low bin is "<<low<<endl;
	  hig = HMs->GetXaxis()->FindBin(0.1*(i+1)+0.9);
	  cout<<"hig bin is "<<hig<<endl;
        }
      else
	if(i<=7)
	  {
	    low = HMs->GetXaxis()->FindBin(0.2*i+0.6);
	    cout<<"low bin is "<<low<<endl;
	    hig = HMs->GetXaxis()->FindBin(0.2*(i+1)+0.6);
	    cout<<"hig bin is "<<hig<<endl;
          }
	else
	  {
	    low = HMs->GetXaxis()->FindBin(2.2);
	    cout<<"low bin is "<<low<<endl;
	    hig = HMs->GetXaxis()->FindBin(5.0);
	    cout<<"hig bin is "<<hig<<endl;
          }
      hbuf[0][i] =(TH1D *)HMs->ProjectionY(buf01,low,hig);
      hbuf[0][i]->Rebin();
      HPtMs = (TH1D *)hbuf[0][i]->Clone();
      hbuf[1][i] =(TH1D *)HPiMs->ProjectionY(buf11,low,hig);
      hbuf[1][i]->Rebin();
      HPtPiMs = (TH1D *)hbuf[1][i]->Clone();
      hbuf[2][i] =(TH1D *)HPMs->ProjectionY(buf21,low,hig);
      hbuf[2][i]->Rebin();
      HPtPMs = (TH1D *)hbuf[2][i]->Clone();
      hbuf[3][i] =(TH1D *)HKMs->ProjectionY(buf31,low,hig);
      hbuf[3][i]->Rebin();
      HPtKMs = (TH1D *)hbuf[3][i]->Clone();
      
      TF1 *fms = new TF1("fms",fms,-0.3,1.3,3);
      //fms->SetNpx(500);
      fms->SetLineWidth(2);
      fms->SetLineColor(2);
      //fms->SetParameters(1e6,5e5,4e5);
      cout<<"step 1 beging--------------->"<<endl;
      HPtMs->Fit("fms","R");
      cout<<"step 2 beging--------------->"<<endl;
      Double_t par[3],parerr[3];
      for(int j=0;j<3;j++)
	{
	  par[j] = fms->GetParameter(j);
	  parerr[j] = fms->GetParError(j);
        }
      TF1 *fpi = new TF1("fpi",fpi,-0.3,1.3,1);
      fpi->SetParameters(par);
      fpi->SetLineColor(3);
      fpi->SetLineStyle(2);//dash
      fpi->SetLineWidth(2);
      fpi->Draw("same");

      TF1 *fp = new TF1("fp",fp,-0.3,1.3,1);
      fp->SetParameters(&par[1]);
      fp->SetLineColor(4);
      fp->SetLineStyle(3);//dash-dot
      fp->SetLineWidth(2);
      fp->Draw("same");

      TF1 *fk = new TF1("fk",fk,-0.3,1.3,1);
      fk->SetParameters(&par[2]);
      fk->SetLineColor(6);
      fk->SetLineStyle(4);//dot-dot
      fk->SetLineWidth(2);
      fk->Draw("same");

      // hbuf[1][i] =(TH1D *)nSigPi->ProjectionY(buf11,low,hig);
      cout<<"the "<<i<<"th projection complate!"<<endl;
    }
  SW.Stop();
  SW.Print();
}
  /*
  int pTLw = HPt->FindBin(pT1);//pT1 = 0.2
  int pTHi = HPt->FindBin(pT2) - 1;// pT2 = 0.4&&very important,remenber findbin() - 1;

  cout << pTLw << "  " << pTHi << endl;

  TH1D *HPtMs   = HMs->ProjectionY("HPtMs",pTLw,pTHi);
  HPtPiMs = HPiMs->ProjectionY("HPtPiMs",pTLw,pTHi);
  HPtPMs  = HPMs->ProjectionY("HPtPMs",pTLw,pTHi);
  HPtKMs  = HKMs->ProjectionY("HPtKMs",pTLw,pTHi);
  HPtnSig = nSigPi->ProjectionY("HPtnSig",pTLw,pTHi);

  HPtnSig->Rebin(RB);
  HPtMs->Rebin(RB/5);

  cout << HPtnSig->GetNbinsX() << "  " << HPtMs->GetNbinsX() << endl;

  TCanvas *cc = new TCanvas("cc", "cc",0,0,800,600);
  gStyle->SetOptFit(0);
  gStyle->SetOptStat(0);
  gStyle->SetEndErrorSize(0.01);
  cc->SetFillColor(10);
  cc->SetBorderMode(0);
  cc->SetBorderSize(2);
  cc->SetFrameFillColor(0);
  cc->SetFrameBorderMode(0);
  cc->SetLogy();
  cc->SetGridx();
  cc->SetGridy();
  cc->SetLeftMargin(0.12);
  cc->SetBottomMargin(0.15);
  cc->SetTopMargin(0.025);
  cc->SetRightMargin(0.025);

  HPtnSig->GetYaxis()->SetTitle("Counts");
  HPtnSig->GetXaxis()->SetNdivisions(208);
  HPtnSig->GetXaxis()->CenterTitle();
  HPtnSig->GetXaxis()->SetTitle("m^{2} (GeV^{2}/c^{4})");
  HPtnSig->GetXaxis()->SetTitleOffset(1.1);
  HPtnSig->GetXaxis()->SetTitleSize(0.06);
  HPtnSig->GetXaxis()->SetLabelOffset(0.01);
  HPtnSig->GetXaxis()->SetLabelSize(0.05);
  HPtnSig->GetXaxis()->SetLabelFont(42);
  HPtnSig->GetXaxis()->SetTitleFont(42);
  HPtnSig->GetYaxis()->SetNdivisions(204);

  //HPtnSig->SetAxisRange(-0.3,1.3,"X");
  HPtnSig->Draw();
  HPtMs->Draw("same");
  HPtPiMs->SetLineColor(2);
  HPtPiMs->Draw("same");
  HPtPMs->SetLineColor(4);
  HPtPMs->Draw("same");
  HPtKMs->SetLineColor(kGreen+2);
  HPtKMs->Draw("same");

  TGraphErrors *gr = new TGraphErrors(900);

  for(int i=0; i<500; i++) {
    float x  = HPtMs->GetBinCenter(i+1);
    float y  = HPtMs->GetBinContent(i+1);
    float ey = HPtMs->GetBinError(i+1);
    gr->SetPoint(i,x,y);
    gr->SetPointError(i,0.,ey);
  }
  for(int i=500; i<900; i++) {
    gr->SetPoint(i,HPtnSig->GetBinCenter(i-500+1),HPtnSig->GetBinContent(i-500+1));
    gr->SetPointError(i,0.,HPtnSig->GetBinError(i-500+1));  
  }

  gr->SetMarkerColor(6);
  gr->SetLineColor(6);
  gr->Draw("HISTsame");
}
*/
