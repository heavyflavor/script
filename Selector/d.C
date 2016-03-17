#define d_cxx
// The class definition in d.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.

// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// Root > T->Process("d.C")
// Root > T->Process("d.C","some options")
// Root > T->Process("d.C+")
//

#include <iostream>

#include <TCanvas.h>
#include <TGraph.h>
#include <TStyle.h>

#include <TMath.h>
#include <TLorentzVector.h>

#include "AliESDEvent.h"
#include "AliESDtrack.h"
#include "AliPID.h" // Particle identification

#include "d.h"

using namespace std;

//
// Helper functions
//
double invariantMass2 (const TLorentzVector & p1, const TLorentzVector & p2) {
	// Computes the squared invariant mass from two TLorentzVector
	// p1 * p2 = p1.E() * p2.E() - p1.Vect() * p2.Vect()
	return ( p1.Mag2() + p2.Mag2() + 2 * (p1 * p2) );
}

double linearTranslate (double from_min, double from_max, double to_min, double to_max, double from_number) {
	// "Translates" a number defined within the interval [from_min, from_max] to a number within the interval [to_min, to_max]
	// it's all bsed on the equation (to_number - to_min) / (to_max - to_min) = (from_number - from_min) / (from_max - from_min)
	return to_min + (to_max - to_min) * (from_number - from_min) / (from_max - from_min);
}

void drawAndPrint (TCanvas *c, TH1 *h, const char * filename = NULL) {
	// Prints histogram and saves it
	// Since the TH1* classes all inherits from the TH1 class, we can use that as a general
	// class for drawing and printing histograms. One should note though, that you're restricted 
	// to the methods available for the TH1 base class. Not sure how it applies to virtual functions in TH1
	h->Draw();
	string tmpName;
	if (filename == NULL) {
		tmpName = h->GetName();
		tmpName += ".png";
		c->Print(tmpName.c_str());
	} else {
		c->Print(filename);
	}
	c->Clear();
}

//
// Definitions of d class methods
//

void d::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

}

void d::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

	TString option = GetOption();
	esd = new AliESDEvent();
	cout << "Break? 0" << endl;

	int bins = 200;

	invMassPrPim = new TH1D("invMassPrPim", "Invariant mass from a p^{+} and a #pi^{-}", bins, 0., 3.);
	invMassPrPim->GetXaxis()->SetTitle("Mass in GeV/c^{2}");
	fOutput->Add(invMassPrPim);

	invMassPimPip = new TH1D("invMassPimPip", "Invariant mass from a #pi^{+} and a #pi^{-}", bins, 0., 3.);
	invMassPimPip->GetXaxis()->SetTitle("Mass in GeV/c^{2}");
	fOutput->Add(invMassPimPip);

	invMassPrPimEventMix = new TH1D("invMassPrPimEventMix", "Invariant mass from a p^{+} and a #pi^{-} from different events", bins, 0., 3.);
	invMassPrPimEventMix->GetXaxis()->SetTitle("Mass in GeV/c^{2}");
	fOutput->Add(invMassPrPimEventMix);

	invMassPimPipEventMix = new TH1D("invMassPimPipEventMix", "Invariant mass from #pi^{-} and a #pi^{+} from different events", bins, 0., 3.);
	invMassPimPipEventMix->GetXaxis()->SetTitle("Mass in GeV/c^{2}");
	fOutput->Add(invMassPimPipEventMix);

}

Bool_t d::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either d::GetEntry() or TBranch::GetEntry()
   // to read either all or the required parts of the data. When processing
   // keyed objects with PROOF, the object is already loaded and is available
   // via the fObject pointer.
   //
   // This function should contain the "body" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.
   //
   // The processing can be stopped by calling Abort().
   //
   // Use fStatus to set the return value of TTree::Process().
   //
   // The return value is currently not used.


	// Containters of diverse particles
	vector<TLorentzVector> pionMinus;
	vector<TLorentzVector> pionPlus;
	vector<TLorentzVector> proton;

	// A temporary variable used throughout identification of the particles
	TLorentzVector lorentzvector;

	// Variables related to probabilities from particle identification.
	const double * probs;
	AliPID::EParticleType mostProbable;

	// Temporary variable for the squared invariant mass
	double invMass2;

	fChain->GetTree()->GetEntry(entry);

	if (entry % 1000 == 0) {
		cout << entry << endl;
	}

	//cout << "main entry " << entry << endl;

	int ntracks = esd->GetNumberOfTracks();
	for (int t = 0; t < ntracks; t++) {
		AliESDtrack *esdt = esd->GetTrack(t);

		probs = esdt->PID();
		mostProbable = particleProbs.GetMostProbable(probs);

		// We'll try to detect and collect the needed particles
		if (mostProbable == AliPID::kPion) {
			lorentzvector.SetPxPyPzE(esdt->Px(), esdt->Py(), esdt->Pz(), esdt->E());

			if (esdt->Charge() == -1) {
				pionMinus.push_back(lorentzvector);
			} else {
				pionPlus.push_back(lorentzvector);
			}
		} else if ((mostProbable == AliPID::kProton) && (esdt->Charge() == 1)) {
			lorentzvector.SetPxPyPzE(esdt->Px(), esdt->Py(), esdt->Pz(), esdt->E());
			proton.push_back(lorentzvector);
		}
	}

	//Compute the different invariant masses
	for (unsigned int x = 0; x < pionMinus.size(); x++) {

		// Compute the invariant mass of a pion minus and a pion plus
		for (unsigned int y = 0; y < pionPlus.size(); y++) {
			invMass2 = invariantMass2(pionMinus[x], pionPlus[y]);

			if (invariantMass2 > 0) {
				invMassPimPip->Fill(TMath::Sqrt(invMass2));
			}
		}

		// Compute the invariant mass of a pion minus and a proton
		for (unsigned int y = 0; y < proton.size(); y++) {
			invMass2 = invariantMass2(pionMinus[x], proton[y]);

			if (invMass2 > 0) {
				invMassPrPim->Fill(TMath::Sqrt(invMass2));
			}
		}

	}

	//
	// Then we start to compute the possible background via event mixing. I'll keep the pionMinus collection.
	//

	pionPlus.clear();
	proton.clear();

	for (int nEntry = 1; nEntry < 2; nEntry++) {

		if ((entry + nEntry) >= numberOfEvents) {
			GetEntry((entry + nEntry) - numberOfEvents);
		} else {
			GetEntry(entry + nEntry);
		}

		ntracks = esd->GetNumberOfTracks();
		for (int t = 0; t < ntracks; t++) {
			AliESDtrack *esdt = esd->GetTrack(t);
			mostProbable = particleProbs.GetMostProbable(esdt->PID());

			if (mostProbable == AliPID::kPion) {
				if (esdt->Charge() == 1) {
					lorentzvector.SetPxPyPzE(esdt->Px(), esdt->Py(), esdt->Pz(), esdt->E());
					pionPlus.push_back(lorentzvector);
				}
			} else if ((mostProbable == AliPID::kProton) && (esdt->Charge() == 1)) {
				lorentzvector.SetPxPyPzE(esdt->Px(), esdt->Py(), esdt->Pz(), esdt->E());
				proton.push_back(lorentzvector);
			}
		}
	}

	//Compute the different invariant masses
	for (unsigned int x = 0; x < pionMinus.size(); x++) {

		// Compute the invariant mass of a pion minus and a pion plus
		for (unsigned int y = 0; y < pionPlus.size(); y++) {
			invMass2 = invariantMass2(pionMinus[x], pionPlus[y]);

			if (invMass2 > 0) {
				invMassPimPipEventMix->Fill(TMath::Sqrt(invMass2));
			}
		}

		// Compute the invariant mass of a pion minus and a proton
		for (unsigned int y = 0; y < proton.size(); y++) {
			invMass2 = invariantMass2(pionMinus[x], proton[y]);

			if (invMass2 > 0) {
				invMassPrPimEventMix->Fill(TMath::Sqrt(invMass2));
			}
		}
	}

	return kTRUE;
}

void d::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

	cout << "Starting to terminate slave" << endl;

	delete esd;

	cout << "Slave terminated" << endl;

}

void d::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

	double scale;

	//Normalized inv. mass from a proton and a pion^{-}
	TH1D * invMassPrPimBGNorm;
	//Normalized inv. mass from pion^{-} and a pion^{+}
	TH1D * invMassPimPipBGNorm;

	TH1D * invMassPrPim = dynamic_cast<TH1D*>(fOutput->FindObject("invMassPrPim"));
	TH1D * invMassPimPip = dynamic_cast<TH1D*>(fOutput->FindObject("invMassPimPip"));
	TH1D * invMassPrPimEventMix = dynamic_cast<TH1D*>(fOutput->FindObject("invMassPrPimEventMix"));
	TH1D * invMassPimPipEventMix = dynamic_cast<TH1D*>(fOutput->FindObject("invMassPimPipEventMix"));

	cout << "invMassPrPim maxbin " << linearTranslate(0., 200., 0., 3., invMassPrPim->GetMaximumBin()) << endl;
	cout << "invMassPimPip maxbin " << linearTranslate(0., 200., 0., 3., invMassPimPip->GetMaximumBin()) << endl;

	TCanvas * c1 = new TCanvas("c1", "c1", 800, 600);

	gStyle->SetOptStat("KSouRMe");

	// Initialize the histograms where the operations wrt. removal of background will take place
	invMassPrPimBGNorm = (TH1D *)invMassPrPim->Clone("invMassPrPimBGNorm");
	invMassPrPimBGNorm->SetTitle("Normalized inv. mass from a p and a #pi^{-}");
	invMassPrPim->GetXaxis()->SetTitle("Mass in GeV/c^{2}");

	invMassPimPipBGNorm = (TH1D *)invMassPimPip->Clone("invMassPimPipBGNorm");
	invMassPimPipBGNorm->SetTitle("Normalized inv. mass from #pi^{-} and a #pi^{+}");
	invMassPimPipBGNorm->GetXaxis()->SetTitle("Mass in GeV/c^{2}");

	drawAndPrint(c1, invMassPrPimEventMix);
	drawAndPrint(c1, invMassPimPipEventMix);

	//invMassPrPimBGNorm->Scale(1 / invMassPrPimBGNorm->Integral());

	//cout << invMassPrPimBGNorm->Integral() << endl;
	int binFrom = (int)linearTranslate(0., 3., 0., 200., 1.75);
	//cout << "binFrom " << binFrom << endl;
	//cout << "invMassPrPimBGNorm->Integral() " << invMassPrPimBGNorm->Integral(binFrom, 200) << endl;
	//cout << "invMassPrPimEventMix->Integral() " << invMassPrPimEventMix->Integral(binFrom, 200) << endl;
	//cout << invMassPrPimBGNorm->Integral() << endl;

	invMassPrPimEventMix->Scale(invMassPrPimBGNorm->Integral(binFrom, 200) / invMassPrPimEventMix->Integral(binFrom, 200));

	binFrom = (int)linearTranslate(0., 3., 0., 200., 1.);
	//cout << "binFrom " << binFrom << endl;
	//cout << "invMassPimPipBGNorm->Integral() " << invMassPimPipBGNorm->Integral(binFrom, 200) << endl;
	//cout << "invMassPimPipEventMix->Integral() " << invMassPimPipEventMix->Integral(binFrom, 200) << endl;

	invMassPrPimBGNorm->Add(invMassPrPimEventMix, -1);
	invMassPimPipBGNorm->Add(invMassPimPipEventMix, -1);

	//cout << "invMassPrPimBGNorm maxbin " << linearTranslate(0., 200., 0., 3., invMassPrPimBGNorm->GetMaximumBin()) << endl;
	//cout << "invMassPimPipBGNorm maxbin " << linearTranslate(0., 200., 0., 3., invMassPimPipBGNorm->GetMaximumBin()) << endl;

	drawAndPrint(c1, invMassPrPim);
	delete invMassPrPim;

	drawAndPrint(c1, invMassPimPip);
	delete invMassPimPip;

	//drawAndPrint(c1, invMassPrPimEventMix);
	delete invMassPrPimEventMix;

	//drawAndPrint(c1, invMassPimPipEventMix);
	delete invMassPimPipEventMix;
	
	drawAndPrint(c1, invMassPrPimBGNorm);
	delete invMassPrPimBGNorm;

	drawAndPrint(c1, invMassPimPipBGNorm);
	delete invMassPimPipBGNorm;

	delete c1;
}
