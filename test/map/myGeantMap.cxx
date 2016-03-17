#include <iostream>
#include "/star/u/xyf/myHeader/GeantMap/GeantMap.h"
#include "/star/u/xyf/myHeader/GeantMap/GeantMap.cpp"
#include "TString.h"
using namespace std;

int main()
//int myGeantMap()
{
	GeantMap Gmap;

	int ID=0;

	cout<<" Input ID : "<<flush;
	cin>> ID;


//	int ID     = Gmap.Find("hyper");
	char* Name = (char*)Gmap.Find(ID);

	cout<<" ID   : "<<ID<<endl;
	cout<<" Name : "<<Name<<endl;

	return 0;
}
