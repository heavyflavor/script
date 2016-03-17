#include <iostream>

#include <TROOT.h>
#include <TChain.h>
#include <TDSet.h>
#include <TProof.h>

using namespace std;

int run () {
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

	TProof * gProof = TProof::Open("");

	gProof->ShowPackages();

	gProof->UploadPackage("STEERBase");
	gProof->EnablePackage("STEERBase");

	gProof->UploadPackage("ESD");
	gProof->EnablePackage("ESD");

	TDSet * set = new TDSet((*chain), 0);
	set->Process("d.C++g");

	gProof->Close();

	return (0);
}

