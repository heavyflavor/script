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
#include "TStopwatch.h"
#endif

using namespace std;
int main(int argc,char **argv)
{

  if(argc !=2) exit(1);
  TStopwatch SW;
  SW.Start();
  // continued...
  // open the ASCII file
  TString output;
  output = "out/";
  output += argv[1];
  output += ".root";
  TString input;
  input = "TEXTFILES/";
  input += argv[1]; 
  input += ".txt";
  cout<<"Input file is :"<<input<<endl;
  cout<<"Output file is :"<<output<<endl;
  
  FILE *fp = fopen(input,"r");
  char line[101];
  TFile *f = new TFile(output,"RECREATE");
  TTree *tree = new TTree("mTree","pedestal data from Tonko's text file");

  tree->Branch("pedestal",&pedestal.fiber,"fiber/I:hybrid/I:strip/I:pedestal/F:RMS/F");
  
  for(int i=0;i<5;i++)
    cout<<fgets(&line[0],100,fp)<<endl;
  
  // fill the tree from the values in ASCII file
  //cout<<"fiber:hybrid:strip:pedestal:RMS"<<endl;
  while (fgets(&line[0],100,fp)) {
    sscanf(&line[0],"%d%d%d%f%f",&pedestal.fiber,&pedestal.hybrid,&pedestal.strip,&pedestal.pedestal,&pedestal.RMS);
    //cout<<pedestal.fiber<<":"<<pedestal.hybrid<<":"<<pedestal.strip<<":"<<pedestal.pedestal<<":"<<pedestal.RMS<<endl;
    tree->Fill();
  }
  // check what the tree looks like
  tree->Print();
  fclose(fp);
  f->Write();
  return(0);
  exit(0);
}
