#define myselector_cxx
// The class definition in myselector.h has been generated automatically
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
// Root > T->Process("myselector.C")
// Root > T->Process("myselector.C","some options")
// Root > T->Process("myselector.C+")
//

#include "myselector.h"
#include <TH2.h>
#include <TStyle.h>


#include "TDatime.h"
#include <stdio.h>
#include <iostream>
using namespace std;
int eventnumber = 0;
int per = 1;
TDatime tBegin,tNow;

void myselector::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();
   tBegin.Set();
   fNumberOfEvents = 0;
   currentEventNumber = 0;
   currentTime = 0;
   cout<<"-------Begin of job-------"<<endl;
   tBegin.Print();
}

void myselector::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

   for(int j=0;j<2;j++)
	 {
	   for(int k=0;k<20;k++)
	     {
	       if(j==0)
		 {
		   EventTime[j][k] = new TH2F(Form("EventTime_%d_%d",j,k),Form("P-side , Ladder %d;Event number;T(us)",k),500,0,500,2000,0,4000);
		   AdcTime[j][k] = new TH2F(Form("AdcTime_%d_%d",j,k),Form("P-side , Ladder %d;T(us);Adc",k),2000,0,4000,1024,0,1024);
		 }
	       else 
		 {
		   EventTime[j][k] = new TH2F(Form("EventTime_%d_%d",j,k),Form("N-side , Ladder %d;Event number;#deltaT(us)",k),500,0,500,2000,0,4000);
		   AdcTime[j][k] = new TH2F(Form("AdcTime_%d_%d",j,k),Form("N-side , Ladder %d;#deltaT(us);Adc",k),2000,0,4000,1024,0,1024);
		 }
	     }
	 }
 
   hTimeDist = new TH1F("hTimeDist","Time distribution",2000,0,4000);
   hAdcTime = new TH2F("hAdcTime","Adc vs deltatime",20,0,20, 1024,0,1024);
   fOutput->AddAll(gDirectory->GetList());
}

Bool_t myselector::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either myselector::GetEntry() or TBranch::GetEntry()
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
  if(currentEventNumber==0)
    {
      currentEventNumber = eventno;
      currentTime= time;
      cout << currentEventNumber << " " << currentTime << endl;
      return kTRUE;
    }
  else if( eventno > currentEventNumber) 
    {
      currentEventNumber = eventno;
      deltaTime   = time-currentTime;
      currentTime = time;
      hTimeDist->Fill(deltaTime/1000.);
      //cout<<deltaTime<<endl;
      
    }
  //if((int)readout%768!=0)
 
  if(readout==4&&wafer==1)
    {
      //cout<<"readout==2, and deltatime = "<<deltaTime/10.<<endl;
      EventTime[(Int_t)side][(int)ladder]->Fill(eventno,deltaTime/10.);
      AdcTime[(Int_t)side][(int)ladder]->Fill(deltaTime/10.,((Int_t)(adc+350))%1024);
    }
  if(side==0 && ladder==19 && wafer==2 &&readout>1)
    {
      int adcMod = adc;
      adcMod=(adcMod+350)%1024;
      float adcm=adcMod;
      hAdcTime->Fill( deltaTime/1000., adcm);
    }
  return kTRUE;
}

void myselector::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.
  cout<<"-------------End of Slave Job----------------"<<endl;
  tNow.Set();tNow.Print();
  cout<<"Number of Event: "<<fNumberOfEvents<<", elapsed time: "<<tNow.Convert()-tBegin.Convert()<<"seconds,rate: "<<float(fNumberOfEvents)/(tNow.Convert()-tBegin.Convert())<<endl;

}

void myselector::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.
  TString output = GetOption();
  TFile *hfile = new TFile(output,"RECREATE","event time info");
  hfile->cd();
  fOutput->Write();
  hfile->Close();
  fOutput->Delete();
  tNow.Set();cout<<"----------End of job----------"<<endl;
  tNow.Print();
}
