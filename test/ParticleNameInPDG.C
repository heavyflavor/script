#include "TDatabasePDG.h"
#include <iomanip>
#include <iostream>
using namespace std;

void ParticleNameInPDG()
{
  // Code are modified from StarParticleData.cxx
  // http://www.star.bnl.gov/webdata/dox/html/StarParticleData_8cxx_source.html

  // Particle list will own the particles added to it, and is responsible for
  // cleaning up their memory.

  //
  // Intiailze the particle data from TDatabasePDG
  //
  TDatabasePDG *pdg = TDatabasePDG::Instance();
  pdg -> ReadPDGTable(); // because it's too much to expect from a singleton


  //
  // Iterate over all particles in the PDG database, and add them to the local list.
  // Set the tracking code according to the G3 standard.  TODO:  Allow user to select
  // differnt tracking code standard.
  //
  TIter Next( pdg->ParticleList() );

  TParticlePDG *particle = 0;
  int i=0;
  Int_t g3new = 100;
  while(  (particle=(TParticlePDG *)Next())  )
    {

      //
      // Clone the particle and add in the tracking code, which maps the PDG ID to the ID 
      // used in the simulation package.
      //
      TString   name   = particle->GetName();
      TString   title  = particle->GetTitle();
      Double_t  mass   = particle->Mass();
      Bool_t    stable = particle->Stable();
      Double_t  width  = particle->Width();
      Double_t  charge = particle->Charge();
      TString   class_ = particle->ParticleClass();

      Int_t   code = particle->PdgCode();
      Int_t   anti = 0; if ( particle->AntiParticle() == particle ) anti = -code;

      //cout << setfill('0') << setw(10) << 45698 << endl;
      if(i==0) {
	cout<<setw(20)<<"Particle Namer "<<"  |  "<<setw(20)<<"PDG Code : "<<endl;
	cout<<"-------------------------------------------------"<<endl;
      }
      cout<<setw(20)<<name.Data()<<"  |   "<<setw(20)<<code<<endl;
      i++;
    }
  cout<<"-------------------------------------------------"<<endl;
}
