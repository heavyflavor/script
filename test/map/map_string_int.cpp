#include <iostream>
#include <utility>// make_pair()
#include <cstring>
#include <map>
using namespace std;

int map_string_int()
//int main()
{
	map<string,int> StuScore;
	map<string,int>::iterator iters;


	StuScore.insert(pair<string,int>("stuA",99));
	StuScore.insert(pair<string,int>("stuB",98));
	StuScore.insert(pair<string,int>("stuC",97));
	StuScore.insert(pair<string,int>("stuD",96));

	cout<<StuScore.size()<<endl;

	cout<<"=================================="<<endl;

//	for( iters = StuScore.begin() ; iters != StuScore.end() ; ++iters )
//	{
//	//	cout<< StuScore["stuA"] <<endl;
//		cout<< iters->first <<" : "<< iters->second <<endl;
//	}

	iters = StuScore.find("stuA"); cout<<iters->second<<endl;
		
	cout<<"=================================="<<endl;
	return 0;
}
