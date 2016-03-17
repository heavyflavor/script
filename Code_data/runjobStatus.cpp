#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <unistd.h>
#include "/star/u/xyf/code_functional/jobStatus/dateDiff.h"
using namespace std;
// **********************************************
	TString UserNameDefault= "xyf";// User Name
// **********************************************
	const int NodeADefault = 6005;	// Start Node;
	const int NodeBDefault = 6010;	// End   Node;
// **********************************************
	int sleepTime = 30;// seconds
	int updateTime= 1;// seconds
	bool AutoUpdate		= true;
	bool UpdatePriority = true;
	bool UpdateDate		= true;
// **********************************************
int runjobStatus(TString UserName=UserNameDefault,int NodeA=NodeADefault,int NodeB=NodeBDefault)
{
	TString CMDcls = "clear";
	TString CMDrqb = "root4star -l -b -q /star/u/xyf/code_functional/jobStatus/jobStatus.cpp";
			CMDrqb+= Form("\\\(\\\"%s\\\",%d,%d,%d,%d\\\)",UserName.Data(),NodeA,NodeB,UpdatePriority,UpdateDate);
	TString Exit = "X";
	TString temp = 0;
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
