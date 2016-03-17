//===============================
//Version: 1.0 
//Time: Tue Jun 30 16:45:17 EDT 2015 
//Author: Long Zhou 
//Discribe: Run my Selector with a list input 

#include "iostream"
#include "TString.h"
#include "TChain.h"
#include "TFileCollection.h"
#include "TCollection.h"
void RunMySelector_list(TString listname="test.list",TString outname)
{
  TChain *ch = new TChain("mTree","Ds v0 Tree ");
  TString output = outname;
  output += ".mini.root";
  cout<<"Output file name is :"<<output<<endl;

  Int_t fileNumber = 0;
  ifstream inlist(listname);
  if (!(inlist)) {
    cout << "can not open list file" << endl;
    return 0;
  }

  cout << "reading filelist: " << listname << endl;


  // TFileCollection Method (more Simpler)
  TFileCollection* fc = new TFileCollection("mylist", "mylist",listname.Data());
  cout<<fc->GetList()->Print()<<endl;
  ch->AddFileInfoList((TCollection *)fc->GetList());
  cout<<"Add File number is : "<<fc->GetList()->GetSize()<<endl;

  // Tradional Method. Use iostream to read file line-by-line.
  // char line[512];
  // while (inlist.getline(line,512)) {
  //   TFile *ftmp = new TFile(line); 
  //   if(!ftmp||!(ftmp->IsOpen())||!(ftmp->GetNkeys())||ftmp->IsZombie()) {
  //     printf(" file %s error in opening!!!\n",line);
  //   } else { 
  //     printf(" read in file %s\n",line);
  //     ch->Add(line); 
  //     fileNumber++;
  //   } 
  //   delete ftmp; 
  // }
  // cout<<"Add File number is : "<<fileNumber<<endl;


  // TProof* prf = TProof::Open("");
  // ch->SetProof();
  ch->Process("mySelector.C",output,5e9,0);
  // ch->Process("mySelector.C+",output,5e9,0);
}
