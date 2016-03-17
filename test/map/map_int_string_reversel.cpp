#include <iostream>
#include <utility>// make_pair()
#include <cstring>
#include <map>
//#include "TString.h"
//#include "/star/u/xyf/myHeader/color.h"
using namespace std;

string FontRed      = "\033[31m";
string FontCyan     = "\033[36m";
string FontEnd      = "\033[0m";
//=========================================================
bool ReverseMap(const map<int,string> &mapIn, map<string,int> &mapOut);
//=========================================================

//int map_int_string_reverse()
int main()
{
	map<int,string> StuScore;
	map<int,string>::iterator iter;
	map<string,int> StuScoreR;
	map<string,int>::iterator iterR;

	cout<<"=================================="<<endl;
	cout<<FontCyan<<" Start Insert ... "<<FontEnd<<endl;

	StuScore.insert(pair<int,string>(1,"stuA"));
	StuScore.insert(pair<int,string>(2,"stuB"));
	StuScore.insert(pair<int,string>(3,"stuC"));
	StuScore.insert(pair<int,string>(4,"stuD"));
//
//	if( ReverseMap(StuScore,StuScoreR) ){cerr<<FontRed<<" Error : In Function ReverseMap"<<endl;return 1;}
//
//	ReverseMap(StuScore,StuScoreR); // FIXME

	cout<<FontCyan<<" Middle Stage Info : "<<FontEnd<<endl;
	cout<<" Size = "<<StuScore.size()<<endl;
	cout<<" SizeR= "<<StuScoreR.size()<<endl;

	cout<<FontCyan<<" Output : "<<FontEnd<<endl;

	iter = StuScore.find(2); 
	if(iter != StuScore.end() )cout<<iter->second<<endl;
	else cout<<" Not found "<<endl;
		
	cout<<"=================================="<<endl;
	return 0;
}
//=========================================================
bool ReverseMap(const map<int,string> &mapIn, map<string,int> &mapOut)
{
	if( mapIn.size() == 0 ){cout<<" mapIn is Empty "<<endl;return 1;}
	if( mapOut.size()!= 0 ){cout<<" mapOut not Empty"<<endl;return 1;}

	map<int,string>::iterator iter;
	for(iter = mapIn.begin() ; iter != mapIn.end() ; ++iter)
	{
		cout<<iter->first<<" "<<iter->second<<endl;
		mapOut.insert(pair<string,int>( iter->second , iter->first ));
	}
	return 0;
}
//=========================================================
