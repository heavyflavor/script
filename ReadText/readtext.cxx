#ifdef __CINT__
 
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class PlotFile;
#endif
#ifndef __CINT__

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#include <iostream>
#include "Riostream.h"
//#include "stadio.h"
#include "TTree.h"
#include "TFile.h"
#include "TString.h"
#include "readtext.h"
#include "TSystem.h"
#include "TStopwatch.h"
#endif

using namespace std;
int main(int argc,char **argv)
{
  if(argc !=4) exit(1);		// filename, sector number,rdo number
  TStopwatch SW;
  SW.Start();

  TString input;
  input = argv[1]; 
  TString filename;
  filename = argv[1];
  filename.ReplaceAll(".txt","");
  Ssiz_t nfirst = filename.Last('/');
  filename.Remove(0,nfirst+1);  
  filename += ".root";
  TString output;
  output = "out/";
  output += filename;
  cout<<"Input file is :"<<input<<endl;
  cout<<"Output file is :"<<output<<endl;
  gSystem->Exec("mkdir -p out");

  TString SectorStr = argv[2];
  SectorStr.ReplaceAll("s","");
  TString RdoStr = argv[3];
  RdoStr.ReplaceAll("r","");

  Int_t Sector = SectorStr.Atoi();
  Int_t Rdo    = RdoStr.Atoi();

  FILE *fp = fopen(input,"r");
  char line[101];
  TFile *f = new TFile(output,"RECREATE");
  TTree *tree = new TTree("mTree","pedestal data from Tonko's text file");

  // tree->Branch("pedestal",&pedestal,"sector/I:rdo/I:side/I:ladder/I:fiber/I:hybrid/I:strip/I:pedestal/F:RMS/F"); // STILL WORKS
  tree->Branch("sector",&pedestal.sector,"sector/I");
  tree->Branch("rdo",&pedestal.rdo,"rdo/I");
  tree->Branch("side",&pedestal.side,"side/I");
  tree->Branch("ladder",&pedestal.ladder,"ladder/I");
  tree->Branch("fiber",&pedestal.fiber,"fiber/I");
  tree->Branch("wafer",&pedestal.hybrid,"hybrid/I");
  tree->Branch("strip",&pedestal.strip,"strip/I");
  tree->Branch("ped",&pedestal.pedestal,"pedestal/F");
  tree->Branch("rms",&pedestal.RMS,"RMS/F");
  
  for(int i=0;i<5;i++)
    cout<<fgets(&line[0],100,fp)<<endl;
  
 while (fgets(&line[0],100,fp)) {
    sscanf(&line[0],"%d%d%d%f%f",&pedestal.fiber,&pedestal.hybrid,&pedestal.strip,&pedestal.pedestal,&pedestal.RMS);
    //cout<<pedestal.fiber<<":"<<pedestal.hybrid<<":"<<pedestal.strip<<":"<<pedestal.pedestal<<":"<<pedestal.RMS<<endl;
    pedestal.sector = Sector;
    if(Sector==1) pedestal.rdo = Rdo;
    else pedestal.rdo = Rdo + 3;
    
    FindLadderSide(Rdo,pedestal.fiber,pedestal.ladder,pedestal.side);   
    tree->Fill();
  }
  // check what the tree looks like
  tree->Print();
  fclose(fp);
  f->Write();
  return(0);
  exit(0);
}

//---------------------------
void FindLadderSide(int RDO,int channel,int &ladder,int &side)
{
  ladder = RDO2LADDER[RDO-1][channel]-1;//ladder [0-19];
  if(RDO<3) side = 1;
  else if(RDO>3) side = 0;
  else if(channel<4) side = 1;
  else side = 0;
}
