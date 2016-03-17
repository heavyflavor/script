#include <vector>
#include "TH1.h"
#include "TH2.h"
#include "TList.h"
#include "TRandom3.h"
using std::vector;
using namespace std;
void VectorTest()
{
  vector<Int_t> vth1;
  vth1.clear();
  vth1.resize(96);
  TRandom3 rd3(0);
  for(int i=0;i<16;i++)
    {
      for(int j=0;j<768;j++)
	{
	  Int_t index = i*6 + j/128;
	  //cout<<"index is : "<<index<<endl;
	  Float_t counts = rd3.Gaus(0,1);
	  if(counts>3)
	    vth1[index]++;
	}
    }
  cout<<"The Side of Th1 vector is : "<<vth1.size()<<endl;
  for(int i=0;i<96;i++) cout<<vth1[i]<<endl;
}
