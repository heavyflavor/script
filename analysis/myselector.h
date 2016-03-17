//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Mar  6 10:13:26 2014 by ROOT version 5.34/09
// from TTree PedestalNTuple/Pedestal Ntuple
// found on file: st_physics_15064020_raw_0500001.Ped.root
//////////////////////////////////////////////////////////

#ifndef myselector_h
#define myselector_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include "TH2.h"
#include "TH1.h"
// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class myselector : public TSelector {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   TH2F           *EventTime[2][20];
   TH2F           *AdcTime[2][20];
   TH1F           * hTimeDist;
   TH2F           * hAdcTime;
   // Declaration of leaf types
   Float_t         side;
   Float_t         ladder;
   Float_t         wafer;
   Float_t         strip;
   Float_t         adc;
   Float_t         eventno;
   Float_t         readout;
   Float_t         A128chip;
   Float_t         RDO;
   Float_t         time;
   Float_t         temp;
   // List of branches
   TBranch        *b_side;   //!
   TBranch        *b_ladder;   //!
   TBranch        *b_wafer;   //!
   TBranch        *b_strip;   //!
   TBranch        *b_adc;   //!
   TBranch        *b_eventno;   //!
   TBranch        *b_readout;   //!
   TBranch        *b_A128chip;   //!
   TBranch        *b_RDO;   //!
   TBranch        *b_time;   //!
   UInt_t fNumberOfEvents;

   Int_t   currentEventNumber;
   Float_t deltaTime;
   Float_t currentTime;

   myselector(TTree * /*tree*/ =0) : fChain(0) { }
   virtual ~myselector() { }
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

   ClassDef(myselector,0);
};

#endif

#ifdef myselector_cxx
void myselector::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("side", &side, &b_side);
   fChain->SetBranchAddress("ladder", &ladder, &b_ladder);
   fChain->SetBranchAddress("wafer", &wafer, &b_wafer);
   fChain->SetBranchAddress("strip", &strip, &b_strip);
   fChain->SetBranchAddress("adc", &adc, &b_adc);
   fChain->SetBranchAddress("eventno", &eventno, &b_eventno);
   fChain->SetBranchAddress("readout", &readout, &b_readout);
   fChain->SetBranchAddress("A128chip", &A128chip, &b_A128chip);
   fChain->SetBranchAddress("RDO", &RDO, &b_RDO);
   fChain->SetBranchAddress("time", &time, &b_time);
}

Bool_t myselector::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

#endif // #ifdef myselector_cxx
