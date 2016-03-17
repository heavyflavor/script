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
void TStringTest()
{
  TString input = "run_15167001.cor.root";
  TString dig = "0123456789";
  Ssiz_t first = input.First(dig);
  //Ssiz_t last  = input.Last(".");
  if(first != kNPOS) cout<<"Found it ! "<<endl;
  cout<<"First "<<first<<endl;
  TRegexp runid("[0-9]*[0-9]");
  TRegexp runid2("\\\..*");
  TRegexp runid3(".*"); //if you want to use the "*" , you must need "." before "*"
  cout<<input(runid)<<endl;
  cout<<input(runid2)<<endl;
  cout<<input(runid3)<<endl;
  cout<<input(first,8)<<endl;
}




