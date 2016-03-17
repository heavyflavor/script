//*****************************************************************************
//
//  Author:
//		Yifei Xu, woshixuyifei@gmail.com
//
//	Description:
//
//		This macro will combine root files in path :
//			$WORKPATH/output_$PART/rootfile_raw/*.root
//		Output files will be in path:
//			$WORKPATH/
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
	bool	TESTMODE	= false;		// 
	bool	UsePart		= false;	// 
	int		NJobTag		= 8;		// 4,8
//*******************************************************************
	bool	SKIP_Histo	= false;
	bool	SKIP_PicoDst= false;
//*******************************************************************
	TString Part		= "part1";		// part1 part2
	TString BasicJobTag	= "LambH3";	//
//*******************************************************************
	TString Run		=	"Run10";
	TString Energy	=	"39GeV";
	TString Centrality=	"MB";
	TString WorkPath=	"/star/data05/scratch/xyf/hypertriton/Pico_generation_Data";
	TString JobTag = 0;
	TString Type = 0;
//*******************************************************************
	const int Nfilesperjob = 100;
	if(TESTMODE)Int_t NIndex	=	2;
	else		Int_t NIndex	=	NJobTag;
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
		if(SKIP_Histo &&	Type.CompareTo("histo")==0) continue;
		if(SKIP_PicoDst &&	Type.CompareTo("picodst")==0) continue;
//*******************************************************************
		TString ListFileName=0;
		if(UsePart)	ListFileName.Append(Run+"_"+Energy+"_"+Centrality+"_"+JobTag+"_"+Type+"_"+Part+".list");
		else		ListFileName.Append(Run+"_"+Energy+"_"+Centrality+"_"+JobTag+"_"+Type+         ".list");
		TString CMDls	= "ls ";
		if(UsePart)	CMDls.Append(WorkPath+"/output_"+Part+"/rootfile_raw/"+"*."+JobTag+"."+Type+".root > "+ListFileName);
		else		CMDls.Append(WorkPath+"/output/rootfile_raw/"+"*."+JobTag+"."+Type+".root > "+ListFileName);
		TString CMDrm = 0;
		CMDrm.Append("rm -f "+ListFileName);
		system(CMDrm.Data());
		system(CMDls.Data());
		cout<<"================================================================================"<<endl;
		cout<<"             Getting File List   : "<<ListFileName<<endl;
		cout<<"================================================================================"<<endl;
//*******************************************************************
		TString InputFile  = ListFileName;// File list
		TString outFileDir = "./haddinfo/out";
		TString errFileDir = "./haddinfo/err";
		TString infFileDir = "./haddinfo";
		TString RootFileDir= "./rootfile_temp";
//*******************************************************************
		TString CMDmkdirA = "mkdir -p ";CMDmkdirA.Append(outFileDir);
		TString CMDmkdirB = "mkdir -p ";CMDmkdirB.Append(errFileDir);
		TString CMDmkdirC = "mkdir -p ";CMDmkdirC.Append(infFileDir);
		TString CMDmkdirD = "mkdir -p ";CMDmkdirD.Append(RootFileDir);
		system(CMDmkdirA.Data());
		system(CMDmkdirB.Data());
		system(CMDmkdirC.Data());
		system(CMDmkdirD.Data());

		ifstream ifile;ifile.open(InputFile,ios::in);if(!ifile){cerr<<"ifile.open failed!"<<endl;return 0;}
		
		TString ofilename=0;	ofilename.Append("myhadd."+JobTag+"."+Type+".xml");
		TString CMDtouch=0;		CMDtouch.Append("touch "+ofilename);
		TString CMDsubmit=0;	CMDsubmit.Append("star-submit "+ofilename);
		TString CMDcommand=0;	CMDcommand.Append("root4star ");
		TString InputLine=0;

		CMDcommand += Form("myhadd.C\\\(\\\"%s/%s.$JOBID.%s.root\\\",\\\"$FILELIST\\\",$INPUTFILECOUNT\\\)",RootFileDir.Data(),JobTag.Data(),Type.Data());

		system(CMDtouch.Data());
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
		ofile<<"<command>"<<endl;
		ofile<<"cd ${SUBMITTINGDIRECTORY}"<<endl;
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
		if(!TESTMODE) // submit
		{
			cout<<"================================================================================"<<endl;
			cout<<"             Submitting XML File : "<<ofilename<<endl;
			cout<<"================================================================================"<<endl;
			system(CMDsubmit.Data());
		}
	}
	return 0;
}
