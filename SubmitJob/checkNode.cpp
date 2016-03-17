#include <iostream>
#include <iomanip>
#include "/star/u/xyf/myHeader/color.h"
using namespace std;

int checkNode()
{
	int NodeS = 6005;
	int NodeE = 6010;
	TString CMD = 0;
	TString TmpSumFile = "/star/u/xyf/code_functional/checkNode/tempFile/.nodeStatusSum.tmp";
	TString TmpDatFile = "/star/u/xyf/code_functional/checkNode/tempFile/.nodeStatusDat.tmp";
	ifstream ifile;
	int Srjob = 0;int Nrjob = 0;
	int Sijob = 0;int Nijob = 0;
	int Sxjob = 0;int Nxjob = 0;
	int Shjob = 0;int Nhjob = 0;
	int Stjob = 0;int Ntjob = 0;
	int Sbjob = 0;int Nbjob = 0;

	gSystem->Exec("clear");
	cout<<endl;
	cout<<"===================================================================="<<endl;
	cout<<" Checking Job Status "<<endl;
	cout<<"===================================================================="<<endl;
	cout<<endl;
	cout<<"  Node | Running |    Idle | Removed |    Held |   Total | Bad (?) |"<<endl;
	cout<<"--------------------------------------------------------------------"<<endl;

	for(int i=NodeS; i<=NodeE; i++)
	{
		CMD = Form("rm -f %s",TmpSumFile.Data());		gSystem->Exec(CMD.Data());
		CMD = Form("rm -f %s",TmpDatFile.Data());		gSystem->Exec(CMD.Data());
		CMD = Form("condor_q -name rcas%d.rcf.bnl.gov > %s",i,TmpSumFile.Data());	gSystem->Exec(CMD.Data());
		CMD = Form("cat %s | grep \" R \" | wc -l >  %s",			TmpSumFile.Data(),TmpDatFile.Data());gSystem->Exec(CMD.Data());
		CMD = Form("cat %s | grep \" I \" | wc -l >> %s",			TmpSumFile.Data(),TmpDatFile.Data());gSystem->Exec(CMD.Data());
		CMD = Form("cat %s | grep \" X \" | wc -l >> %s",			TmpSumFile.Data(),TmpDatFile.Data());gSystem->Exec(CMD.Data());
		CMD = Form("cat %s | grep \" H \" | wc -l >> %s",			TmpSumFile.Data(),TmpDatFile.Data());gSystem->Exec(CMD.Data());
		CMD = Form("cat %s | grep \" 0+00:00:00 R \" | wc -l >> %s",TmpSumFile.Data(),TmpDatFile.Data());gSystem->Exec(CMD.Data());

		ifile.open(TmpDatFile.Data(),ios::in);if(!ifile){cerr<<"ifile.open failed "<<TmpDatFile<<endl;return 0;}
		ifile>>Srjob>>Sijob>>Sxjob>>Shjob>>Sbjob;
		Stjob = Srjob+Sijob+Sxjob+Shjob;
		Nrjob+=Srjob;
		Nijob+=Sijob;
		Nxjob+=Sxjob;
		Nhjob+=Shjob;
		Ntjob+=Stjob;
		Nbjob+=Sbjob;
		ifile.close();
		cout<<"  "<<FontRed<<setw(4)<<i<<FontEnd;
		cout<<" | "<<setw(7)<<Srjob;
		cout<<" | "<<setw(7)<<Sijob;
		cout<<" | "<<setw(7)<<Sxjob;
		cout<<" | "<<setw(7)<<Shjob;
		if		(Stjob< 200)cout<<" | "<<FontGreen<<setw(7)<<Stjob<<FontEnd;
		else if	(Stjob< 500)cout<<" | "<<FontYellow<<setw(7)<<Stjob<<FontEnd;
		else   				cout<<" | "<<FontRed<<setw(7)<<Stjob<<FontEnd;
		if		(Sbjob<  50)cout<<" | "<<FontGreen<<setw(7)<<Sbjob<<FontEnd;
		else if	(Sbjob< 100)cout<<" | "<<FontYellow<<setw(7)<<Sbjob<<FontEnd;
		else				cout<<" | "<<FontRed<<setw(7)<<Sbjob<<FontEnd;
		cout<<" | "<<endl;
	}
	cout<<"--------------------------------------------------------------------"<<endl;
	cout<<" Total";
	cout<<" | "<<setw(7)<<Nrjob;
	cout<<" | "<<setw(7)<<Nijob;
	cout<<" | "<<setw(7)<<Nxjob;
	cout<<" | "<<setw(7)<<Nhjob;
	cout<<" | "<<setw(7)<<Ntjob;
	cout<<" | "<<setw(7)<<Nbjob;
	cout<<" | "<<endl;
	cout<<endl;
	cout<<"===================================================================="<<endl;
	cout<<endl;
	gSystem->Exec("date");
	cout<<endl;
//	CMD = Form("rm -f %s",TmpSumFile.Data()); gSystem->Exec(CMD.Data());
//	CMD = Form("rm -f %s",TmpDatFile.Data()); gSystem->Exec(CMD.Data());

	return 0;
}
