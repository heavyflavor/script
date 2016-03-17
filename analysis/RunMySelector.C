#include "iostream"
#include "TString.h"
#include "TChain.h"
void RunMySelector(TString filename="st_physics_15064020_raw_0500001",TString outdir)
{
  TChain *ch = new TChain("PedestalNTuple","data run from Luis");
  //system("ls -l out/");
  //int nfile = ch->Add("../st_physics_15037070_raw_0500001.Ped.root");
  TString output = outdir;
  TString input = filename;
  output += "/";
  output += filename;
  output += ".out.root";
  input += ".Ped.root";
  cout<<"Input file name is  :"<<input<<endl;
  cout<<"Output file name is :"<<output<<endl;
  int nfile = ch->Add(input);  
  cout<<"Add File number is : "<<nfile<<endl;
  //ch->Process("mTree.C+",output,99000000,0);
  //ch->Process("myselector.C+",output,5e9,0);
  ch->Process("myselector.C+",output,5e9,0);
}
