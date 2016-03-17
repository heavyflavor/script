#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <vector>
#include <TSystem>
#include "jobStatus/readPriority.h"
#include "jobStatus/dateDiff.h"
#include "color.h"
using namespace std;
TString TimeFormat(long long int NSeconds);

// **********************************************
	const TString UserNameDefault= "zhoulong";// User Name
// **********************************************
	const int NodeADefault = 6005;	// Start Node;
	const int NodeBDefault = 6010;	// End   Node;
	bool UpdatePriority	= false;
	bool UpdateDate		= false;
// **********************************************
int jobStatus(TString UserName=UserNameDefault,int NodeA=NodeADefault,int NodeB=NodeBDefault,
			bool UpdatePriority,bool UpdateDate)
{
	gSystem->Exec("clear");
	cout<<endl;
	cout<<"=========================================================="<<endl;
	cout<<"    Checking Job Status for User : "<<UserName<<endl;
	cout<<"=========================================================="<<endl;
	cout<<endl;

	const int NodeN = NodeB-NodeA+1;
	int Crjob = 0; // Running Job in Current Node
	int Cijob = 0; // Idle    Job in Current Node
	int Chjob = 0; // Held    Job in Current Node
	int Cxjob = 0; // Removed Job in Current Node

	int Nrjob = 0; // Running Job in Total 
	int Nijob = 0; // Idle    Job in Total 
	int Nhjob = 0; // Held    Job in Total 
	int Nxjob = 0; // Removed Job in Total 

	int Orjob = 0; // Running Job in Total OLD
	int Oijob = 0; // Idle    Job in Total OLD
	int Ohjob = 0; // Held    Job in Total OLD
	int Oxjob = 0; // Removed Job in Total OLD

	int nSpace = 0;

	float UserPrio  = 0;
	float OUserPrio = 0;

	TString TimePast = 0;

	TString DatFile = 0; DatFile.Append(".jobStatus."+UserName+".dat");
	TString BakFile = 0; BakFile.Append(".jobStatus."+UserName+".bak.temp");
	TString TmpFile = 0; TmpFile.Append(".jobStatus."+UserName+".tmp.temp");

	TString CMD = 0;
	TString SIZE= 0;
	CMD = Form("rm -f %s",BakFile.Data()); gSystem->Exec(CMD.Data());
	
//	cout<<DatFile<<endl;
//	cout<<BakFile<<endl;

	ifstream ifileDat;ifileDat.open(DatFile.Data(),ios::in);
	if(!ifileDat)
	{
		CMD = Form("touch %s",DatFile.Data());gSystem->Exec(CMD.Data());
		ifileDat.open(DatFile.Data(),ios::in);
		if(!ifileDat)
		{
			cerr<<"ifileDat.open Failed !"<<endl;
			return 0;
		}
	}

	vector<int> Rjob;Rjob.clear();
	vector<int> Ijob;Ijob.clear();
	vector<int> Hjob;Hjob.clear();
	vector<int> Xjob;Xjob.clear();
	for(int i=0;i<NodeN;i++)
	{
		int temp1,temp2,temp3,temp4;
		ifileDat>>temp1>>temp2>>temp3>>temp4;
		Rjob.push_back(temp1);Orjob+=temp1;
		Ijob.push_back(temp2);Oijob+=temp2;
		Hjob.push_back(temp3);Ohjob+=temp3;
		Xjob.push_back(temp4);Oxjob+=temp3;
	}
	ifileDat>>OUserPrio;
	ifileDat.close();
	cout<<"  Node | Running |    Idle | Removed |    Held | "<<endl;
	cout<<"----------------------------------------------------------"<<endl;

	for(int i=0;i<NodeN;i++)
	{
		CMD = Form("rm -f %s",TmpFile.Data()); gSystem->Exec(CMD.Data());
		CMD = Form("touch %s",TmpFile.Data()); gSystem->Exec(CMD.Data());
		CMD.Form("condor_q %s -name rcas%d.rcf.bnl.gov | grep \"%s\" > %s",UserName.Data(),i+NodeA,UserName.Data(),TmpFile.Data());
		gSystem->Exec(CMD.Data());

		CMD.Form("cat %s | grep \" R \" | wc -l >> %s",TmpFile.Data(),BakFile.Data());
		gSystem->Exec(CMD.Data());
		CMD.Form("cat %s | grep \" I \" | wc -l >> %s",TmpFile.Data(),BakFile.Data());
		gSystem->Exec(CMD.Data());
		CMD.Form("cat %s | grep \" H \" | wc -l >> %s",TmpFile.Data(),BakFile.Data());
		gSystem->Exec(CMD.Data());
		CMD.Form("cat %s | grep \" X \" | wc -l >> %s",TmpFile.Data(),BakFile.Data());
		gSystem->Exec(CMD.Data());
		CMD = Form("rm -f %s",TmpFile.Data()); gSystem->Exec(CMD.Data());

		ifstream ifileBak;ifileBak.open(BakFile.Data(),ios::in);if(!ifileBak){cout<<"ifileBak Failed"<<endl;return 0;}
		for(int j=0;j<=i;j++)
		{
			ifileBak>>Crjob>>Cijob>>Chjob>>Cxjob;
		//	gSystem->Exec("more jobStatus.cpp.temp.bak");
		//	cout<<"i="<<i<<" j="<<j<<" Crjob="<<Crjob<<" Cijob="<<Cijob<<endl;
		}
		Nrjob += Crjob;
		Nijob += Cijob;
		Nhjob += Chjob;
		Nxjob += Cxjob;
		ifileBak.close();

		if(!(Rjob[i]==0 && Ijob[i]==0 && Hjob[i]==0 && Xjob[i]==0 && Crjob==0 && Cijob==0 && Chjob==0 && Cxjob==0))
		{
			cout<<setw(6)<<i+NodeA<<" | ";
			cout<<setw(7)<<Crjob<<" | ";
			cout<<setw(7)<<Cijob<<" | ";
			cout<<setw(7)<<Cxjob<<" | ";
			cout<<setw(7)<<Chjob<<" | ";
			if(Chjob!=0 || Cxjob!=0) cout<<"ATTENTION"<<endl;
			else if(Crjob==0 && Cijob==0 && Chjob==0 && Cxjob==0 && !(Rjob[i]==0 && Ijob[i]==0 && Hjob[i]==0 && Xjob[i]==0))
			{
				cout<<" Complete"<<endl;
			}
			else cout<<endl;

			CMD = Form("echo \"       | ");
			//************************************************************
			if(Crjob-Rjob[i]==0) CMD += Form("%s      0",ColorBlue.Data());
			SIZE = Form("%d",Crjob-Rjob[i]); nSpace = 8-SIZE.Length();
			if(Crjob-Rjob[i]> 0)
			{
				CMD += Form("%s",ColorRed.Data());
				for(int k=0;k<nSpace-2;k++)	CMD += Form(" ");
				CMD += Form("+%d",Crjob-Rjob[i]);
			}
			if(Crjob-Rjob[i]< 0)
			{
				CMD += Form("%s",ColorGreen.Data());
				for(int k=0;k<nSpace-1;k++)	CMD += Form(" ");
				CMD += Form("%d",Crjob-Rjob[i]);
			}
			CMD += Form("%s | ",ColorEnd.Data());
			//************************************************************
			if(Cijob-Ijob[i]==0) CMD += Form("%s      0",ColorBlue.Data());
			SIZE = Form("%d",Cijob-Ijob[i]); nSpace = 8-SIZE.Length();
			if(Cijob-Ijob[i]> 0)
			{
				CMD += Form("%s",ColorRed.Data());
				for(int k=0;k<nSpace-2;k++)	CMD += Form(" ");
				CMD += Form("+%d",Cijob-Ijob[i]);
			}
			if(Cijob-Ijob[i]< 0)
			{
				CMD += Form("%s",ColorGreen.Data());
				for(int k=0;k<nSpace-1;k++)	CMD += Form(" ");
				CMD += Form("%d",Cijob-Ijob[i]);
			}
			CMD += Form("%s | ",ColorEnd.Data());
			//************************************************************
			if(Cxjob-Xjob[i]==0) CMD += Form("%s      0",ColorBlue.Data());
			SIZE = Form("%d",Cxjob-Xjob[i]); nSpace = 8-SIZE.Length();
			if(Cxjob-Xjob[i]> 0)
			{
				CMD += Form("%s",ColorRed.Data());
				for(int k=0;k<nSpace-2;k++)	CMD += Form(" ");
				CMD += Form("+%d",Cxjob-Xjob[i]);
			}
			if(Cxjob-Xjob[i]< 0)
			{
				CMD += Form("%s",ColorGreen.Data());
				for(int k=0;k<nSpace-1;k++)	CMD += Form(" ");
				CMD += Form("%d",Cxjob-Xjob[i]);
			}
			CMD += Form("%s | ",ColorEnd.Data());
			//************************************************************
			if(Chjob-Hjob[i]==0) CMD += Form("%s      0",ColorBlue.Data());
			SIZE = Form("%d",Chjob-Hjob[i]); nSpace = 8-SIZE.Length();
			if(Chjob-Hjob[i]> 0)
			{
				CMD += Form("%s",ColorRed.Data());
				for(int k=0;k<nSpace-2;k++)	CMD += Form(" ");
				CMD += Form("+%d",Chjob-Hjob[i]);
			}
			if(Chjob-Hjob[i]< 0)
			{
				CMD += Form("%s",ColorGreen.Data());
				for(int k=0;k<nSpace-1;k++)	CMD += Form(" ");
				CMD += Form("%d",Chjob-Hjob[i]);
			}
			CMD += Form("%s | ",ColorEnd.Data());
			//************************************************************
		//	if(Chjob!=0 || Cxjob!=0) CMD += Form("%s   ATTENTION %s\"",ColorRed.Data(),ColorEnd.Data());
		//	if(Crjob==0 && Cijob==0 && Chjob==0 && Cxjob==0 && !(Rjob[i]==0 && Ijob[i]==0 && Hjob[i]==0 && Xjob[i]==0))
		//	{
		//		CMD += Form("%s   Complete %s\"",ColorCyan.Data(),ColorEnd.Data());
		//	}
		//	else CMD += Form("\"");
			CMD += Form("\"");
			gSystem->Exec(CMD.Data());
		}
	}
//	cout<<endl;
	cout<<"----------------------------------------------------------"<<endl;
//	cout<<endl;
	cout<<setw(6)<<" Total | ";
	cout<<setw(7)<<Nrjob<<" | ";
	cout<<setw(7)<<Nijob<<" | ";
	cout<<setw(7)<<Nxjob<<" | ";
	cout<<setw(7)<<Nhjob<<" | ";
	if(Nhjob!=0 || Nxjob!=0) cout<<"ATTENTION"<<endl;
	else if(Nrjob==0 && Nijob==0 && Nhjob==0 && Nxjob==0 && !(Orjob==0 && Oijob==0 && Ohjob==0 && Oxjob==0))
	{
		cout<<" Complete"<<endl;
	}
	else cout<<endl;
	CMD = Form("echo \"       | ");
	//************************************************************
	if(Nrjob-Orjob==0) CMD += Form("%s      0",ColorBlue.Data());
	SIZE = Form("%d",Nrjob-Orjob); nSpace = 8-SIZE.Length();
	if(Nrjob-Orjob> 0)
	{
		CMD += Form("%s",ColorRed.Data());
		for(int k=0;k<nSpace-2;k++)	CMD += Form(" ");
		CMD += Form("+%d",Nrjob-Orjob);
	}
	if(Nrjob-Orjob< 0)
	{
		CMD += Form("%s",ColorGreen.Data());
		for(int k=0;k<nSpace-1;k++)	CMD += Form(" ");
		CMD += Form("%d",Nrjob-Orjob);
	}
	CMD += Form("%s | ",ColorEnd.Data());
	//************************************************************
	if(Nijob-Oijob==0) CMD += Form("%s      0",ColorBlue.Data());
	SIZE = Form("%d",Nijob-Oijob); nSpace = 8-SIZE.Length();
	if(Nijob-Oijob> 0)
	{
		CMD += Form("%s",ColorRed.Data());
		for(int k=0;k<nSpace-2;k++)	CMD += Form(" ");
		CMD += Form("+%d",Nijob-Oijob);
	}
	if(Nijob-Oijob< 0)
	{
		CMD += Form("%s",ColorGreen.Data());
		for(int k=0;k<nSpace-1;k++)	CMD += Form(" ");
		CMD += Form("%d",Nijob-Oijob);
	}
	CMD += Form("%s | ",ColorEnd.Data());
	//************************************************************
	if(Nxjob-Oxjob==0) CMD += Form("%s      0",ColorBlue.Data());
	SIZE = Form("%d",Nxjob-Oxjob); nSpace = 8-SIZE.Length();
	if(Nxjob-Oxjob> 0)
	{
		CMD += Form("%s",ColorRed.Data());
		for(int k=0;k<nSpace-2;k++)	CMD += Form(" ");
		CMD += Form("+%d",Nxjob-Oxjob);
	}
	if(Nxjob-Oxjob< 0)
	{
		CMD += Form("%s",ColorGreen.Data());
		for(int k=0;k<nSpace-1;k++)	CMD += Form(" ");
		CMD += Form("%d",Nxjob-Oxjob);
	}
	CMD += Form("%s | ",ColorEnd.Data());
	//************************************************************
	if(Nhjob-Ohjob==0) CMD += Form("%s      0",ColorBlue.Data());
	SIZE = Form("%d",Nhjob-Ohjob); nSpace = 8-SIZE.Length();
	if(Nhjob-Ohjob> 0)
	{
		CMD += Form("%s",ColorRed.Data());
		for(int k=0;k<nSpace-2;k++)	CMD += Form(" ");
		CMD += Form("+%d",Nhjob-Ohjob);
	}
	if(Nhjob-Ohjob< 0)
	{
		CMD += Form("%s",ColorGreen.Data());
		for(int k=0;k<nSpace-1;k++)	CMD += Form(" ");
		CMD += Form("%d",Nhjob-Ohjob);
	}
	CMD += Form("%s | ",ColorEnd.Data());
	//************************************************************
	CMD += Form("\"");
	gSystem->Exec(CMD.Data());
	cout<<endl;
	cout<<"=========================================================="<<endl;

	if(UpdatePriority)
	{
		UserPrio = readPriority(UserName.Data());
		cout<<" Current User Priority   :";
		cout<<setiosflags(ios::fixed)<<setprecision(2)<<setw(10)<<UserPrio;
		cout<<setiosflags(ios::showpos)<<setw(10)<<UserPrio-OUserPrio;
		cout<<resetiosflags(ios::fixed)<<resetiosflags(ios::showpos)<<endl;
		cout<<endl;
	}
	else UserPrio = OUserPrio;
	if(UpdateDate)
	{
		TimePast = TimeFormat(dateDiff(UserName.Data()));//TString
		cout<<" Time Since Last Update  : "<<TimePast<<endl;
	}
	cout<<"=========================================================="<<endl;
//	cout<<" Current Time : "<<flush;gSystem->Exec("date");

	ofstream ofileBak;ofileBak.open(BakFile.Data(),ios::app);if(!ofileBak){cerr<<"ofileBak.open failed"<<endl;return 0;}
	ofileBak<<UserPrio;
	ofileBak.close();
	CMD = Form("mv %s %s",BakFile.Data(),DatFile.Data());gSystem->Exec(CMD.Data()); 
	return 0;
}
TString TimeFormat(long long int NSeconds)
{
	int NDays = NSeconds/(3600*24);
	int NHours= (NSeconds%(3600*24))/3600;
	int NMins = ((NSeconds%(3600*24))%3600)/60;
	int NSecs = (((NSeconds%(3600*24))%3600)%60);
	TString RETRUN=0;
					RETRUN  = Form("%d+",NDays);
	if(NHours<10)	RETRUN += Form("0");
					RETRUN += Form("%d:",NHours);
	if(NMins<10)	RETRUN += Form("0");
					RETRUN += Form("%d:",NMins);
	if(NSecs<10)	RETRUN += Form("0");
					RETRUN += Form("%d",NSecs);
	return RETRUN;
}
