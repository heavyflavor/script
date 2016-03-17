//*****************************************************************************
//
//  Author:
//		Yifei Xu, woshixuyifei@gmail.com
//
//	Description:
//
//		This macro will combine root files in path :
//			$WORKPATH/rootfile_raw/*.root
//		Output files will be in path:
//			$WORKPATH/rootfile_temp/*.root
//
//		Jobs will be submitted by specified job tags and root file types.
//
//*****************************************************************************
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
using namespace std;

int myhadd()
{
	bool	TESTMODE	= false;	// 
	int		NJobTag		= 8;		// 4,8
//*******************************************************************
	bool Skip_picodst	= false;
	bool Skip_histo		= false;
//*******************************************************************
	TString BasicJobTag	= "LambH3";	//
//*******************************************************************
	TString Run	        = "Run11";
	TString Energy		= "27GeV";
	TString Centrality	= "MB";
	TString WorkPath	= "/star/data05/scratch/xyf/hypertriton/Data_Cent/Run11_27GeV_Apr_23/output_27GeV"; // no slash at end
	TString JobTag 		= 0;
	TString Type 		= 0;
	TString CMD = 0;
//*******************************************************************
	const int Nfilesperjob	= 100;
	if(TESTMODE)Int_t NIndex= 2;
	else		Int_t NIndex= NJobTag;
//*******************************************************************
	for(int Index=1;Index<=NIndex;Index++)
	{
		switch (Index)
		{
			case 1:
				JobTag	= BasicJobTag;
				Type	= "picodst";
				break;
			case 2:
				JobTag	= BasicJobTag;
				Type	= "histo";
				break;
			case 3:
				JobTag	= Form("%srot",BasicJobTag.Data());
				Type	= "picodst";
				break;
			case 4:
				JobTag	= Form("%srot",BasicJobTag.Data());
				Type	= "histo";
				break;
			case 5:
				JobTag	= Form("Anti%s",BasicJobTag.Data());
				Type	= "picodst";
				break;
			case 6:
				JobTag	= Form("Anti%s",BasicJobTag.Data());
				Type	= "histo";
				break;
			case 7:
				JobTag	= Form("Anti%srot",BasicJobTag.Data());
				Type	= "picodst";
				break;
			case 8:
				JobTag	= Form("Anti%srot",BasicJobTag.Data());
				Type	= "histo";
				break;
			default:
				cout<<"======================================="<<endl;
				cout<<"    Invalid Index -- Should be 1 - 8 "<<endl;
				cout<<"======================================="<<endl;
				return 0;
				break;
		}
		if(Skip_picodst && Type.CompareTo("picodst")==0) continue;
		if(Skip_histo   && Type.CompareTo("histo")==0) continue;
//*******************************************************************
		TString ListFileName=0;
		ListFileName.Append(Run+"_"+Energy+"_"+Centrality+"_"+JobTag+"_"+Type+".list");
		CMD = Form("ls %s/rootfile_raw/*.%s.%s.root > %s",WorkPath.Data(),JobTag.Data(),Type.Data(),ListFileName.Data());gSystem->Exec(CMD.Data());

		cout<<"================================================================================"<<endl;
		cout<<"             Getting File List   : "<<ListFileName<<endl;
		cout<<"================================================================================"<<endl;
//*******************************************************************
		TString InputFile  = ListFileName;// File list
		TString outFileDir = 0;
		TString errFileDir = 0;
		TString infFileDir = 0;
		TString RootFileDir= 0;

		outFileDir = Form("%s/haddinfo/out",	WorkPath.Data());
		errFileDir = Form("%s/haddinfo/err",	WorkPath.Data());
		infFileDir = Form("%s/haddinfo",		WorkPath.Data());
		RootFileDir= Form("%s/rootfile_temp",	WorkPath.Data());
//*******************************************************************
		CMD = Form("mkdir -p %s",outFileDir.Data());gSystem->Exec(CMD.Data());
		CMD = Form("mkdir -p %s",errFileDir.Data());gSystem->Exec(CMD.Data());
		CMD = Form("mkdir -p %s",infFileDir.Data());gSystem->Exec(CMD.Data());
		CMD = Form("mkdir -p %s",RootFileDir.Data());gSystem->Exec(CMD.Data());

		ifstream ifile;ifile.open(InputFile,ios::in);if(!ifile){cerr<<"ifile.open failed!"<<endl;return 0;}
		
		TString ofilename=0;	ofilename.Append("myhadd."+JobTag+"."+Type+".xml");
		CMD = Form("touch %s",ofilename.Data());gSystem->Exec(CMD.Data());

		TString CMDcommand = Form("root4star -l -b -q myhadd.C\\\(\\\"%s.$JOBID.%s.root\\\",\\\"$FILELIST\\\",$INPUTFILECOUNT\\\)",JobTag.Data(),Type.Data());

		ofstream ofile;ofile.open(ofilename.Data(),ios::out);if(!ofile){cerr<<"ofile.open failed!"<<endl;return 0;}
		cout<<"================================================================================"<<endl;
		cout<<"             Creating XML File   : "<<ofilename<<endl;
		cout<<"================================================================================"<<endl;

		ofile<<"<?xml version=\"1.0\" encoding=\"utf-8\" ?>"<<endl;
		ofile<<"<job  name=\"hadd\" maxFilesPerProcess=\""<<Nfilesperjob<<"\" filesPerHour=\"100\" fileListSyntax=\"paths\">"<<endl;
		ofile<<"<stdout URL=\"file:"<<outFileDir.Data()<<"/$JOBID.out\"/>"<<endl;
		ofile<<"<stderr URL=\"file:"<<errFileDir.Data()<<"/$JOBID.err\"/>"<<endl;
		ofile<<"<Generator>"<<endl;
		ofile<<"<Location>"<<infFileDir.Data()<<"</Location>"<<endl;
		ofile<<"</Generator>"<<endl;
		ofile<<"<output fromScratch=\"*.root\"    toURL=\""<<RootFileDir.Data()<<"/\" />"<<endl;
		ofile<<"<command>"<<endl;
		ofile<<"ln -s "<<WorkPath.Data()<<"/myhadd.C ."<<endl;
		ofile<<CMDcommand<<endl;
		ofile<<"</command>"<<endl;

		for(int j=0;!(ifile.eof());j++)
		{
			TString temp;	ifile>>temp;
			if(ifile.eof())break;
			ofile<<"<input URL=\"file:"<<temp.Data()<<"\"/>"<<endl;
		}

		ofile<<"</job>"<<endl;
		ofile.close();
		CMD = Form("rm -f %s",ListFileName.Data());gSystem->Exec(CMD.Data());

		if(!TESTMODE) // submit
		{
			cout<<"================================================================================"<<endl;
			cout<<"             Submitting XML File : "<<ofilename<<endl;
			cout<<"================================================================================"<<endl;
			CMD = Form("star-submit %s",ofilename.Data());gSystem->Exec(CMD.Data());
		}
	}
	return 0;
}
