#include <iostream>
#include <utility>
#include <cstring>
#include <map>
using namespace std;

int multimap()
{
//	gROOT->ProcessLine(".L loader.C+");

	multimap<string,int> mymap;
	multimap<string,int>::iterator it;

	mymap.insert(make_pair("xuyifei",1));
	mymap.insert(make_pair("xuyifei",2));

//	mymap.insert(make_pair(1,1));//"xu"));
//	mymap.insert(make_pair(1,2));//"yifei"));
//	mymap.insert(make_pair(2,1));//"li"));
//	mymap.insert(make_pair(2,2));//"shu"));
//	mymap.insert(make_pair(2,3));//"lishu"));

	it = mymap.begin();
	cout<<(*it).first<<" "<<(*it).second<<endl;

//	for(it = mymap.begin(); it != mymap.end(); it++)
//	{
//		cout<<(*it)->first<<" "<<(*it)->second<<endl;
//	}

//	for(it=mymap.equal_range(2)->first; it!=mymap.equal_range(2)->second; it++)
//	{
//		cout<<it.first<<" "<<it.second<<endl;
//	}

	return 0;
}
