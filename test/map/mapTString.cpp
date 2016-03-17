#include <iostream>
#include <utility>// make_pair()
#include <cstring>
#include <map>
using namespace std;

int mapTString()
//int main()
{
	map<TString,int> StuScore;
	map<TString,int>::iterator iters;


	StuScore.insert(pair<TString,int>("stuA",99));
	StuScore.insert(pair<TString,int>("stuB",98));
	StuScore.insert(pair<TString,int>("stuC",97));
	StuScore.insert(pair<TString,int>("stuD",96));

	cout<<StuScore.size()<<endl;

	cout<<"=================================="<<endl;

	for( iters = StuScore.begin() ; iters != StuScore.end() ; ++iters )
	{

	//	cout<< StuScore["stuA"] <<endl;
		cout<< iters->first <<" : "<< iters->second <<endl;

	}
	cout<<"=================================="<<endl;
	return 0;
}
