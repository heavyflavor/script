#define mySelector_cxx
// The class definition in mySelector.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.

// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// Root > T->Process("mySelector.C")
// Root > T->Process("mySelector.C","some options")
// Root > T->Process("mySelector.C+")
//

#include "mySelector.h"
#include <TH2.h>
#include <TStyle.h>
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
#include "TGraph.h"
#include "TDirectoryFile.h"
#define Nevt        10100000
using namespace std;
int eventnumber = 0;
int per = 1;
TDatime tBegin,tNow;

void mySelector::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();
   tBegin.Set();
   fNumberOfEvents = 0;
   cout<<"-------Begin of job-------"<<endl;
   tBegin.Print();
}

void mySelector::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();
   for(int i=0;i<NMAX;i++)
     {
       Pt[i] = 0;
       Central[i] = 0;
       Up[i] = 0;
       Down[i] = 0;
     }
   hRatio = new TH2F("hRatio","The ratio uncertainty from FONLL calculate;p_{T} [GeV/C];Ratio",200,0,20,100,0,100);
}

Bool_t mySelector::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either mySelector::GetEntry() or TBranch::GetEntry()
   // to read either all or the required parts of the data. When processing
   // keyed objects with PROOF, the object is already loaded and is available
   // via the fObject pointer.
   //
   // This function should contain the "body" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.
   //
   // The processing can be stopped by calling Abort().
   //
   // Use fStatus to set the return value of TTree::Process().
   //
   // The return value is currently not used.
if(fNumberOfEvents==0)
    {
      eventnumber = fChain->GetEntries();
      cout<<"Entries = "<<eventnumber<<endl;
      if(eventnumber<100) per = 1;
      else per = eventnumber/100;
      cout<<"per = "<<per<<endl;
    }
 if(fNumberOfEvents%per==0) cout<<"PROCESSING "<<float(fNumberOfEvents)/eventnumber*100<<"%...."<<endl;
 GetEntry(entry);
 ++fNumberOfEvents;   
 //int ptbin = hRatio->FindBin(pt);
 Pt[entry]      = pt;
 Central[entry] = central;
 Up[entry]      = upper;
 Down[entry]    = lower;
 //hRatio->SetBinContent(ptbin,central/1.0e3);
 //hRatio->SetBinError(ptbin,0);
 //hRatio_low->SetBinContent(ptbin,lower/1.0e3);
 //hRatio_low->SetBinError(ptbin,0);
 //hRatio_up->SetBinContent(ptbin,upper/1.0e3);
 //hRatio_up->SetBinError(ptbin,0);

 return kTRUE;
}

void mySelector::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.
  cout<<"-------------End of Slave Job----------------"<<endl;
  tNow.Set();tNow.Print();
  cout<<"Number of Event: "<<fNumberOfEvents<<", elapsed time: "<<tNow.Convert()-tBegin.Convert()<<"seconds,rate: "<<float(fNumberOfEvents)/(tNow.Convert()-tBegin.Convert())<<endl;
}

void mySelector::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.
  TString option = GetOption();
  TString output = "gr_";
  output += option;
  output += ".root";
  TFile *hfile = new TFile(output,"RECREATE","FONLL CCbar cross section");
  
  hfile->cd();
  const Int_t npoint = eventnumber;
  Float_t x[npoint];
  Float_t y[npoint];
  Float_t ylow[npoint];
  Float_t yup[npoint];
  for(int i=0;i<npoint;i++)
    {
      x[i] = Pt[i];
      y[i] = Central[i];
      ylow[i] = Down[i];
      yup[i] = Up[i];
    }
  //TGraph *grFONLLD0 = new TGraph(npoint,Pt,Central);
  TDirectoryFile *ratioErr = new TDirectoryFile(option,"ratio error of scale pp500 to pp200 ");
  ratioErr->SetName(option);
  ratioErr->Add(hRatio);
  TGraph *grFONLLRatio = new TGraph(npoint,x,y);
  grFONLLRatio->SetName("grFONLLRatio");
  TGraph *grFONLLRatio_u = new TGraph(npoint,x,yup);
  grFONLLRatio_u->SetName("grFONLLRatio_u");
  TGraph *grFONLLRatio_d = new TGraph(npoint,x,ylow);
  grFONLLRatio_d->SetName("grFONLLRatio_d");
  grFONLLRatio->Print();
  ///grFONLLRatio->Write();
  ratioErr->Add(grFONLLRatio);
  grFONLLRatio_u->Print();
  //grFONLLRatio_u->Write();
  ratioErr->Add(grFONLLRatio_u);
  grFONLLRatio_d->Print();
  //grFONLLRatio_d->Write();
  ratioErr->Add(grFONLLRatio_d);
  ratioErr->Write();
  hfile->Print();
  hfile->Close();
  tNow.Set();
  cout<<"----------End of job----------"<<endl;
  tNow.Print();
}
