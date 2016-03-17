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
#include <iomanip>
#include <stdio.h>
#include <unistd.h>
#include "TString.h"
#include "jobStatus/dateDiff.h"
#endif
using namespace std;
// **********************************************
	TString UserNameDefault= "zhoulong";// User Name
// **********************************************
	const int NodeADefault = 6005;	// Start Node;
	const int NodeBDefault = 6010;	// End   Node;
// **********************************************
	int sleepTime = 300;// seconds
	int updateTime= 1;// seconds
	bool AutoUpdate		= true;
	bool UpdatePriority = true;
	bool UpdateDate		= true;
// **********************************************
int main(int argc,char **argv)
{
  TString UserName;
  if(argc==1) 
    UserName=UserNameDefault;
  else if(argc==2)
    UserName=argv[1];
  else exit(1);
  
  int NodeA=NodeADefault;
  int NodeB=NodeBDefault;
  TString CMDcls = "clear";
  TString CMDrqb = "root4star -l -b -q -n jobStatus/jobStatus.cpp";
  CMDrqb+= Form("\\\(\\\"%s\\\",%d,%d,%d,%d\\\)",UserName.Data(),NodeA,NodeB,UpdatePriority,UpdateDate);
  TString Exit = "X";
  TString temp = "";
  char s;
	for(int i=0;;i++)
	{
		gSystem->Exec(CMDcls.Data());
		gSystem->Exec(CMDrqb.Data());
		if(!AutoUpdate)
		{
			cout<<"Press 'x' to Exit, any other key to show job status "<<endl;

			s = cin.get();
			if(s == 'X' || s == 'x') break;
		}
		if(AutoUpdate)
		{
			for(int i=0;i<(sleepTime/updateTime);i++)
			{
				if(i%5==0) cout<<"Status will be updated in "<<sleepTime-i*updateTime<<" seconds      \r"<<flush;
				if(i%5==1) cout<<"Status will be updated in "<<sleepTime-i*updateTime<<" seconds .    \r"<<flush;
				if(i%5==2) cout<<"Status will be updated in "<<sleepTime-i*updateTime<<" seconds ..   \r"<<flush;
				if(i%5==3) cout<<"Status will be updated in "<<sleepTime-i*updateTime<<" seconds ...  \r"<<flush;
				if(i%5==4) cout<<"Status will be updated in "<<sleepTime-i*updateTime<<" seconds .... \r"<<flush;
				sleep(updateTime);
			}
		}
	}
	return 0;
}
