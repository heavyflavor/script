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
#include "readstatus.h"
#include "TStopwatch.h"
#endif

using namespace std;
int main(int argc,char **argv)
{
  if(argc !=2) exit(1);
  TStopwatch SW;
  SW.Start();
  TString output;
  output = "Strips.status.root";
  TString input;
  input = argv[1]; 
  cout<<"Input file is :"<<input<<endl;
  cout<<"Output file is :"<<output<<endl;
  
  FILE *fp = fopen(input,"r");
  char line[101];
  TFile *f = new TFile(output,"RECREATE");
  TTree *tree = new TTree("mTree","pedestal data from Tonko's text file");

  tree->Branch("fiber",&fiber,"fiber/I");
  tree->Branch("hybrid",&hybrid,"hybrid/I");
  tree->Branch("strip",&strip,"strip/I");
  tree->Branch("status0",&status0,"status0/I");
  tree->Branch("status1",&status1,"status1/I");
  tree->Branch("status2",&status2,"status2/I");
  tree->Branch("status3",&status3,"status3/I");
  tree->Branch("status4",&status4,"status4/I");
  for(int i=0;i<5;i++)
    cout<<fgets(&line[0],100,fp)<<endl;
  
  while (fgets(&line[0],100,fp)) {
    sscanf(&line[0],"%d%d%d%d%d%d%d%d",&fiber,&hybrid,&strip,&status0,&status1,&status2,&status3,&status4);
    tree->Fill();
  }
  tree->Print();
  fclose(fp);
  f->Write();
  return(0);
  exit(0);
}
