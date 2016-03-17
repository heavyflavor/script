#include <iostream>

#include <TROOT.h>
#include <TChain.h>
#include <TDSet.h>

using namespace std;

int runLocal () {
	#ifndef __CINT__
	cout << "Please run as: aliroot run.C" << endl;
	return 0;
	#endif
	gROOT->LoadMacro("$ALICE_ROOT/PWG0/CreateESDChain.C");
	
	TChain * chain;
	int testRun = 0;

	if (testRun == 1) {
		chain = CreateChain("esdTree", "./../rootFiles.txt", 1);
	} else {
		chain = CreateChain("esdTree", "./../rootFiles.txt", -1);
	}

	chain->Process("physAlice.C++g");

	return (0);
}

