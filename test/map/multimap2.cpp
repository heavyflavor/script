#include <iostream>
#include <map>
#include <cstring>
using namespace std;

//int multimap2()
int main()
{
//	gROOT->ProcessLine(".L loader.C+");

	std::multimap<char,int> mymultimap;
	std::multimap<char,int>::iterator iter;

	mymultimap.insert (std::pair<char,int>('a',10));
	mymultimap.insert (std::pair<char,int>('b',20));
	mymultimap.insert (std::pair<char,int>('b',150));

	iter = mymultimap.find('b');
	cout<<(*iter).first<<" "<<(*iter).second<<endl;

//	for(std::multimap<char,int>::iterator it=mymultimap.begin(); it!=mymultimap.end(); ++it)
//	{
//		std::cout << (*it).first << " => " << (*it).second << '\n';
//	}

	return 0;
}
