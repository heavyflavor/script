#include <iostream>
using namespace std;

TString UserNameDefault = "xyf";

float readPriority(TString UserName=UserNameDefault)
{
	TString CMD;
	TString TempFile;TempFile = Form(" jobStatus/jobStatus.priority.%s.temp",UserName.Data());

	CMD = Form("rm -f %s",TempFile.Data());gSystem->Exec(CMD.Data());
	CMD = Form("condor_userprio -allusers | grep %s > %s",UserName.Data(),TempFile.Data());gSystem->Exec(CMD.Data());

	ifstream ifile;ifile.open(TempFile.Data(),ios::in);

	TString UserMail = 0;
	float UserPrio = 0;

	ifile>>UserMail;	cout<<flush;//UserMail<<endl;
	ifile>>UserPrio;	cout<<flush;//UserPrio<<endl;

	ifile.close();
	CMD = Form("rm -f %s",TempFile.Data());gSystem->Exec(CMD.Data());

//	cout<<"UserPrio = "<<UserPrio<<endl;

	return UserPrio;
}
