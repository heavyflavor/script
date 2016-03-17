//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Jun  1 14:21:25 2010 by ROOT version 5.26/00
// from TChain esdTree/
//////////////////////////////////////////////////////////

#ifndef d_h
#define d_h

#include <iostream>

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TList.h>
#include <TUrl.h>

#include <TH1D.h>

#include "AliESDEvent.h"
#include "AliESDtrack.h"
#include "AliPID.h" // Particle identification

using namespace std;

class d : public TSelector {
public :
	TTree          *fChain;   //!pointer to the analyzed TTree or TChain

	AliESDEvent    *esd;
	AliPID         particleProbs;

	TH1D * invMassPrPim;
	TH1D * invMassPimPip;
	TH1D * invMassPrPimEventMix;
	TH1D * invMassPimPipEventMix;

	int numberOfEvents;

   d(TTree * /*tree*/ =0) { }
   virtual ~d() { }
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();

   ClassDef(d,0);
};

#endif

#ifdef d_cxx
void d::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set array pointer

   cout << "init, segfault? 0" << endl;

   // Set branch addresses and branch pointers
   cout << "init, segfault? 1" << endl;
   if (!tree) return;
   fChain = tree;
   fChain->SetMakeClass(0);

}

Bool_t d::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   // Just for the sake of monitoring
   TUrl * url = const_cast< TUrl * > (fChain->GetCurrentFile()->GetEndpointUrl());
   cout << url->GetUrl() << endl;

   // Need to reset the esd object when a new file is opened.
   esd->ReadFromTree(fChain->GetTree());
   numberOfEvents = fChain->GetTree()->GetEntries();

   return kTRUE;
}

#endif // #ifdef d_cxx
