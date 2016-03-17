//===========================
// Version :1.1
// Autor   : Long Zhou
// Discribution: This code is delevoped form Jonathan's code , 
#include <stdlib.h>
#include "iostream"
#include "Riostream.h"
#include "string.h"
#include "TROOT.h"
#include "TFile.h"
#include "TChain.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TProfile.h"
#include "TTree.h"
#include "TRandom.h"
#include "TMath.h"
#include "TStopwatch.h"
#include "TLatex.h"
#include "TLine.h"
#include "mTree.h"
#include "TString.h"
#include "TSystem.h"
#include "TRegexp.h"
#include "TNtuple.h"
#include "TH2.h"

void dbstability()
{
  gSystem->Load("St_base"); // needed for StDbModifier 
  gSystem->Load("StUtilities");
  
  // DB-specific libs
  gSystem->Load("libStDb_Tables.so");
  gSystem->Load("StDbLib");

  //count of the number of lines of the input files
  ifstream in;
  in.open("goodrun.list");
  int lines_count =0;
  std::string line;
  while (std::getline(in , line))
    ++lines_count;
  
  printf(" found %d files\n",lines_count);
  in.close();

  //output root file
  Float_t CalibNTuple[50];
  TFile   *MyFile  = new TFile("stability.root","RECREATE");
  string  varlist  = "side:ladder:wafer:chip:cutpos:correct:runid:counter";
  TNtuple *mNtuple = new TNtuple("CalibrationNTuple","Calibration Ntuple",varlist.c_str());

  Int_t index = 0;  
  TH1F *hcut[3840];
  TH1F *hcorr[3840];

  for(Int_t iSide=0;iSide<2;iSide++)
    {
      for(Int_t iLad=0;iLad<20;iLad++)
	{
	  for(Int_t iWaf=0;iWaf<16;iWaf++)
	    {
	      for(Int_t iChip=0;iChip<6;iChip++)
		{
		  index = iSide*20*16*6+iLad*16*6+iWaf*6+iChip;
		  hcut[index]  = new TH1F(Form("hcut_S%d_L%d_W%d_C%d",iSide,iLad,iWaf,iChip),Form("New TH S%d L%d W%d C%d",iSide,iLad,iWaf,iChip),400,0,400);
		  hcut[index]->SetFillColor(5);
		  hcorr[index] = new TH1F(Form("hcorr_S%d_L%d_W%d_C%d",iSide,iLad,iWaf,iChip),Form("CF S%d L%d W%d C%d",iSide,iLad,iWaf,iChip),400,0,40);
		  hcorr[index]->SetFillColor(5);
		}
	    }
	}
    }

  // loop over input file and open the associated root file
  in.open("goodrun.list");
  for(int ii=0;ii<lines_count-1;++ii){
    TString currentFile;
    in >> currentFile;
    std::cout <<" currentFile is : " << currentFile << std::endl;
    //std::string currentFile;
    //std::getline(in , currentFile);
    
    TRegexp number="[0-9]*[0-9]";
    //TString runid = input(number);
    TString runid = currentFile(number);
    cout<<"Current run id is "<<runid<<endl;
    
    const Int_t maxEntries = 3840; //20 ladders * 16 wafers * 6 chips * 2 sides

    Float_t chipCorrection;
    Int_t iLad,iCaf,iChip,iSide;
    //TFile *infile = new TFile(Form("%s",input.Data()));
    //const char *tempo = currentFile.c_str();
    //TFile *infile = new TFile(tempo);
    TFile *infile = new TFile(currentFile);
    TTree *m = (TTree *)infile->Get("mTree");
    Short_t side    = 0;
    Short_t ladder  = 0;
    Short_t wafer   = 0;
    Short_t chip    = 0;
    Float_t cutpos  = 0;
    Float_t correct = 0;
    m->SetBranchAddress("side",&side);
    m->SetBranchAddress("ladder",&ladder);
    m->SetBranchAddress("wafer",&wafer);
    m->SetBranchAddress("chip",&chip);
    m->SetBranchAddress("cutpos",&cutpos);
    m->SetBranchAddress("correct",&correct);
    
    Int_t add_counter = 0;
    //here I just put by hand the chip correction to 400, for the purpose to show how the macro works
    chipCorrection    = 400;

    for(Int_t iSide=0;iSide<2;iSide++)
      {
	for(Int_t iLad=0;iLad<20;iLad++)
  	  {
  	    for(Int_t iWaf=0;iWaf<16;iWaf++)
  	      {
  		for(Int_t iChip=0;iChip<6;iChip++)
  		  {
		    index = iSide*20*16*6+iLad*16*6+iWaf*6+iChip;
		    //hcut[index]  = new TH1F(Form("hcut_S%d_L%d_W%d_C%d",iSide,iLad,iWaf,iChip),Form("New TH S%d L%d W%d C%d",iSide,iLad,iWaf,iChip),400,0,400);
		    //hcorr[index] = new TH1F(Form("hcorr_S%d_L%d_W%d_C%d",iSide,iLad,iWaf,iChip),Form("CF S%d L%d W%d C%d",iSide,iLad,iWaf,iChip),400,0,40);
		    m->GetEntry(iSide*20*16*6+iLad*16*6+iWaf*6+iChip);
		    cutpos  = (int)(cutpos+0.5);
		    //correct = (int)(correct-375+0.5);
		    correct = (int)(correct+0.5);
  		    add_counter++;
		    CalibNTuple[0]=iSide;
		    CalibNTuple[1]=iLad;
		    CalibNTuple[2]=iWaf;
		    CalibNTuple[3]=iChip;
		    CalibNTuple[4]=cutpos;
		    CalibNTuple[5]=correct;
		    CalibNTuple[6]=atoi(runid.Data());
		    CalibNTuple[7]= ii;
		    mNtuple->Fill(CalibNTuple);     

		    Int_t xbin = hcut[index]->GetXaxis()->FindBin(ii);
		    hcut[index]->SetBinContent(xbin,cutpos);
		    hcorr[index]->SetBinContent(xbin,correct);
  		  }
  	      }
  	  }
      }

    cout << " Entries: " << add_counter << endl;
    
    infile->Close();
  }

  for(Int_t iSide=0;iSide<2;iSide++)
    {
      for(Int_t iLad=0;iLad<20;iLad++)
	{
	  for(Int_t iWaf=0;iWaf<16;iWaf++)
	    {
	      for(Int_t iChip=0;iChip<6;iChip++)
		{
		  index = iSide*20*16*6+iLad*16*6+iWaf*6+iChip;
		  MyFile->WriteTObject(hcut[index]);
		  MyFile->WriteTObject(hcorr[index]);
		  hcut[index]->Delete();
		  hcorr[index]->Delete();
		}
	    }
	}
    }

  if(MyFile){
    MyFile->Write();
    MyFile->Close();
  }
}



