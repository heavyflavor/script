#include "iostream"
#include "TString.h"
#include "TChain.h"
void RunMySelector_list(TString listname="test.list",TString outdir,TString outname)
{
  TChain *ch = new TChain("mTree","Ds v0 Tree ");
  TString output = outdir;
  output += "/";
  output += outname;
  output += ".mini.root";
  cout<<"Output file name is :"<<output<<endl;

  Int_t fileNumber = 0;
  ifstream inlist(listname);
  if (!(inlist)) {
    cout << "can not open list file" << endl;
    return 0;
  }

  cout << "reading filelist: " << listname << endl;

  char line[512];
  while (inlist.getline(line,512)) {
    TFile *ftmp = new TFile(line); 
    if(!ftmp||!(ftmp->IsOpen())||!(ftmp->GetNkeys())||ftmp->IsZombie()) {
      printf(" file %s error in opening!!!\n",line);
    } else { 
      printf(" read in file %s\n",line);
      ch->Add(line); 
      fileNumber++;
    } 
    delete ftmp; 
  }

  // int nfile = ch->Add(listname);  
  cout<<"Add File number is : "<<fileNumber<<endl;
  ch->Process("mySelector.C+",output,5e9,0);
}
