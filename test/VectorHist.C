#include <vector>
#include "TH1.h"
#include "TH2.h"
#include "TList.h"
using std::vector;
using namespace std;
void VectorHist()
{
  vector<TH1D* > vth1;
  vector<TH2D* > vth2;
  vth1.clear();
  vth2.clear();

  TList list;
  TList *mHistList;
  TH1D *h1d[10];
  TH1F *h1f[10];
  TH2D *h2d[10];
  TH2F *h2f[10];
  for(int i=0;i<10;i++)
    {
      h1d[i] = new TH1D(Form("h1d_%d",i),"Test ",10,0,10);
      h1f[i] = new TH1F(Form("h1f_%d",i),"Test ",10,0,10);
      h2d[i] = new TH2D(Form("h2d_%d",i),"Test ",10,0,10,10,0,10);
      h2f[i] = new TH2F(Form("h2f_%d",i),"Test ",10,0,10,10,0,10);
      vth1.push_back(h1d[i]);
      //vth1.push_back(h1f[i]);
      vth2.push_back(h2d[i]);
      //vth2.push_back(h2f[i]);
      list.Add(h1d[i]);
      list.Add(h1f[i]);
      list.Add(h2d[i]);
      list.Add(h2f[i]);
    }
  cout<<"The Side of Th1 vector is : "<<vth1.size()<<endl;
  cout<<"The Side of Th2 vector is : "<<vth2.size()<<endl;
  list.Print();

  list.At(1)->Draw();

  mHistList = (TList *)gDirectory->GetList(); 
  mHistList->Print();
  TFile *f = new TFile("VectorHist.root","RECREATE");
  mHistList->Write();
  f->Close();
}
