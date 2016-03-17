#include <iostream>
#include <utility>// make_pair()
#include <cstring>
#include <map>
using namespace std;

int mapint()
{
	map<int,int> Stu;
	map<int,int>::iterator iter;
//	map<string,int> StuScore;
//	map<string,int>::iterator iters;

//	pair<map<int,int>::iterator,bool> Insert;

	Stu.insert(pair<int,int>(1,101));
	Stu.insert(pair<int,int>(2,102));
	Stu.insert(pair<int,int>(3,103));
	Stu.insert(pair<int,int>(4,104));
//	StuScore.insert(make_pair("xuyifei",99));

//	Stu.insert(pair<int,int>(4,105));// will NOT change the value
//	Stu[4] = 105;                    // will     change the value

// To test if new value is added : 
//
//	Insert.second = True : added
//	Insert.second = False: Not added
//
//	Insert = Stu.insert(pair<int,int>(4,104));
//	cout<<"Insert.second = "<<Insert.second<<endl;

//=======================================================
// Get access to the Value

// Way 1
//	cout<<Stu[1]<<endl;

// Way 2
	iter = Stu.find(5);
	if( iter != Stu.end() )
	cout<<iter->first<<" "<<iter->second<<endl;
	else
		cout<<" Not Found"<<endl;

// Way3 TEST

//	for( iter = Stu.begin() ; iter != Stu.end() ; ++iter )
//	{
//		cout<<iter->first<<" "<<iter->second<<endl;
//	}

//	iters = StuScore.find("xuyifei");
//	cout<<iters->first<<" "<<iters->second<<endl;
//=======================================================
	return 0;
}
