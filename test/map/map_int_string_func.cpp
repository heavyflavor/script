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
//
//
//	This works with :	g++  ...cpp
//						root ...cpp+
//
//				Need Compile Before Running.
//
//=========================================================
map<int,string>::iterator FindKey(map<int,string>::iterator &iter, map<int,string> &StuScore, const string &Stu);
//=========================================================

int map_int_string_func()
//int main()
{
	map<int,string> StuScore;
	map<int,string>::iterator iter;

	cout<<"=================================="<<endl;
	cout<<FontCyan<<" Start Insert ... "<<FontEnd<<endl;

	StuScore.insert(pair<int,string>(1,"stuA"));
	StuScore.insert(pair<int,string>(2,"stuB"));
	StuScore.insert(pair<int,string>(3,"stuC"));
	StuScore.insert(pair<int,string>(4,"stuD"));

//-----------------------------------------
	cout<<FontCyan<<" Middle Stage Info : "<<FontEnd<<endl;
	cout<<" Size = "<<StuScore.size()<<endl;

//-----------------------------------------
	cout<<FontCyan<<" Output : "<<FontEnd<<endl;

	iter = StuScore.find(2); 
	if(iter != StuScore.end() )cout<<iter->second<<endl;
	else cout<<" Not found "<<endl;

	iter = FindKey(iter,StuScore,"stuB");
	if(iter != StuScore.end() )cout<<iter->first<<" "<<iter->second<<endl;
	else cout<<" Not found "<<endl;

		
	cout<<"=================================="<<endl;
	return 0;
}
//=========================================================
map<int,string>::iterator FindKey(map<int,string>::iterator &iter, map<int,string> &StuScore, const string &Stu)
{
	for(iter = StuScore.begin() ; iter != StuScore.end() ; ++iter)
	{
		if( iter->second == Stu ) return iter;
	}
	return StuScore.end();
}
//=========================================================
