#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TH1.h"
#include "TH2.h"
#include "TSystem.h"
using namespace std;

void TreeQA(TString input="ds.ntp",TString treename="mDs",Int_t nentries=1e5)
{
  TCanvas *cc = new TCanvas("cc", "cc",0,0,1800,1200);
  gStyle->SetOptFit(0);
  gStyle->SetOptStat(0);
  gStyle->SetEndErrorSize(0.01);
  gStyle->SetOptLogz(1);
  cc->SetFillColor(10);
  cc->SetBorderMode(0);
  cc->SetBorderSize(2);
  cc->SetFrameFillColor(0);
  cc->SetFrameBorderMode(0);
  cc->SetLeftMargin(0.12);
  cc->SetBottomMargin(0.15);
  cc->SetTopMargin(0.025);
  cc->SetRightMargin(0.025);

  input += ".root";
  TFile *f = new TFile(input.Data());
  TTree *t = (TTree *)f->Get(treename.Data());
  f->cd();
  t->SetMarkerStyle(20);
  t->SetMarkerSize(1);

  // cc->Divide(11,11);
  // TString branche_name[11] = {"Dca12","Dca3","sumDca","Pt12","Pt3","secSigma","dcaDaughters","dcaToPv","decayLength","cosTheta","cosOpenAngle"};
  // TString myfor;
  // for(int i=0;i<11;i++)
  //   {
  //     for(int j=0;j<11;j++)
  // 	{
  // 	  cc->cd(i*11 + j + 1);
  //      	  myfor=Form("\"%s:%s\",\"\",\"COLZ\",%d",branche_name[i].Data(),branche_name[j].Data(),nentries);
  // 	  cout<<myfor.Data()<<endl;
  // 	  t->Draw(Form("%s:%s",branche_name[i].Data(),branche_name[j].Data()),"","COLZ",nentries);
  // 	}
  //   }

  cc->Divide(5,5);
  const Int_t nvar = 5;
  TString branche_name[nvar] = {"sumDca","Pt","secSigma","dcaDaughters","dcaToPv"};
  TString myfor;
  for(int i=0;i<nvar;i++)
    {
      for(int j=0;j<nvar;j++)
  	{
  	  cc->cd(i*nvar + j + 1);
       	  myfor=Form("\"%s:%s\",\"\",\"COLZ\",%d",branche_name[i].Data(),branche_name[j].Data(),nentries);
  	  cout<<myfor.Data()<<endl;
  	  t->Draw(Form("%s:%s",branche_name[i].Data(),branche_name[j].Data()),"","COLZ",nentries);
  	}
    }

  char figname[200];
  gSystem->Exec("mkdir -p QA");
  input.ReplaceAll(".root","");
  sprintf(figname,"QA/var_corr_%s.png",input.Data());
  cc->SaveAs(figname);
  sprintf(figname,"QA/var_corr_%s.C",input.Data());
  cc->SaveAs(figname);
}
