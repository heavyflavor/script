#include <iostream>
#include <iomanip>
#include <fstream>
using namespace std;

int checkoutfilelist()
{
	TString outfilelist = "outfilelist.list";
//*************************************************************************************************
	TString infilename = 0;
	TString outfilename1 = 0; // good file
	TString outfilename2 = 0; // bad  file
	TString goodbye	="Goodbye";
	TString current="NULL";
	int i=0;

	cout<<"========================================================="<<endl;
	cout<<"    Checking : "<<endl;
	cout<<"========================================================="<<endl;

	TString rmBad  = Form("rm -r Badfilelist.log");	system(rmBad.Data());
	TString rmGood = Form("rm -r Goodfilelist.log");system(rmGood.Data());

	ifstream inlist;inlist.open(outfilelist.Data(),ios::in);
	if(!inlist){cerr<<"outfilelist.open failed!"<<endl;return 0;}

	for(int i=0;;i++)
	{
		inlist>>infilename;
		if(inlist.eof())break;
		if(i%100==0) cout<<"Processing No."<<i<<endl;
		outfilename1	= Form("Goodfilelist.log"); // good file
		outfilename2	= Form("Badfilelist.log"); // bad  file

		ifstream infile;	infile.open(infilename.Data(),ios::in);	if(!infile) cerr<<"infile.open failed!"<<endl;
		ofstream outfile1;	outfile1.open(outfilename1.Data(),ios::app);if(!outfile1)cerr<<"outfile1.open failed!"<<endl;
		ofstream outfile2;	outfile2.open(outfilename2.Data(),ios::app);if(!outfile2)cerr<<"outfile2.open failed!"<<endl;

		infile.seekg(-9,ios::end);
//		char c;
//		c=infile.peek();
//		cout<<"peek="<<c<<endl;
		infile>>current;
//		cout<<current<<endl;
//		cout<<goodbye<<endl;

		if(goodbye.CompareTo(current)!=0) outfile2<<infilename<<endl;
		else outfile1<<infilename<<endl;
		infile.close();
		outfile1.close();
		outfile2.close();
	}
	return 0;
}
