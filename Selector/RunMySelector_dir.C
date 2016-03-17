#include "iostream"
#include "TString.h"
#include "TChain.h"
void RunMySelector_dir(TString indir="Out",Long64_t runnumber)
{
  TChain *ch = new TChain("mDstar","v0tree analysis");
  //system("ls -l out/");
  //int nfile = ch->Add("../st_physics_15037070_raw_0500001.Ped.root");
  int     fileNumber = 0;
  void*   dir = gSystem->OpenDirectory(gSystem->ExpandPathName(indir.Data()));
  int     nruns=0;
  char    *file_name;
  TString Tname=0;
  char    file_list[2500][256];
  TString output = indir;
  output += ".hist.root";
  
  cout << "Execute : RunMySelector_dir.C " << " ; Input : " << indir.Data() << "; OutDir : "<<output.Data()<< endl;
 
  do {
    file_name = (char*)gSystem->GetDirEntry(dir);
    Tname=file_name;
    if (file_name && Tname.Contains(".root")) {
      sprintf(file_list[nruns],"%s/%s",indir.Data(),file_name);
      TFile *ftmp = new TFile(file_list[nruns]); 
      if(!ftmp||!(ftmp->IsOpen())||!(ftmp->GetNkeys())) {
        printf(" file %s error in opening!!!\n",file_list[nruns]);
      } else { 
        printf(" read in file %s\n",file_list[nruns]);
        ch->Add(file_list[nruns]); 
        fileNumber++;
      } 
      nruns++;
      delete ftmp;
    }
  } while (file_name);

  cout<<"++++++++++++++++++++++++++++++++++++++"<<endl;
  cout<<"+ Input dir  name is   : "<<indir<<endl;
  cout<<"+ Output file name is  : "<<output<<endl;
  cout<<"+ Add File number is   : "<<fileNumber<<endl;
  cout<<"+ Run Number of events : "<<runnumber<<endl;
  cout<<"++++++++++++++++++++++++++++++++++++++"<<endl;
  ch->Process("mySelector.C+",output,runnumber,0);

}
