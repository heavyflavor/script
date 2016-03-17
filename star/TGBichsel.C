class Bichsel;
Bichsel *m_Bichsel = 0;
const Int_t NMasses = 1;
//const Double_t Masses[NMasses] = {0.493677};//{0.13956995}; pi, kaon
const Double_t Masses[NMasses] = {2.8094318};//p: 0.938272; d: 1.875612763; t: 2.809431836; He3:2.80923;
const Char_t *Names[NMasses] = {"pi"};
const Int_t NF = 4;
const Char_t *FNames[5] = {"Girrf","Sirrf","Bz","B70","B60"};
const Int_t Nlog2dx = 3;
const Double_t log2dx[Nlog2dx] = {0,1,2};
Double_t bichsel70(Double_t *x,Double_t *par) {
  Double_t pove   = pow(10.,x[0]);
  Double_t poverm = pove/par[0];
  return m_Bichsel->GetI70M(TMath::Log10(poverm),1.0); //1.0 = Log2(dX), 2.322 = Log2(5), 2.585 = Log2(6), 3=Log2(8)
  //return m_Bichsel->GetI70M(TMath::Log10(poverm),2.322); //suggest by Bichsel and Yuri, need to used dX=8 cm instead of 2 cm for proton in He3 calculation
}
void bichselG10() {
  if (gClassTable->GetID("BetheBloch") < 0 || !m_Bichsel) {
    gSystem->Load("libTable");
    gSystem->Load("St_base");
    gSystem->Load("StarClassLibrary");
    gSystem->Load("StBichsel");
    m_Bichsel = new Bichsel();
  }
  for (int h = 0; h < NMasses; h++) { // Masses
    Int_t f = 3;
    Int_t dx = 1;
    Char_t *FunName = Form("%s%s%i",FNames[f],Names[h],(int)log2dx[dx]);
    cout << "Make " << FunName << endl;
    TF1 *func = new TF1(FunName,bichsel70,-1.5., 2.2,5);
    func->SetParameter(0,Masses[h]);
        func->SetLineColor(h+2);
        func->Draw();
    // Xin Dong
    double ptot[990], dedx[990];
    double ptot1[27] = {0.1,0.15,0.2,0.25,0.3,0.4,0.5,0.6,0.7,0.8,
                        1.0,1.2,1.4,1.6,1.8,
                        2.0,2.4,2.8,
                        3.2,3.6,4.0,4.5,5.0,5.5,
                        6.0,7.0,8.0};
    double dedx1[26];

    Double_t mflag=1.0;
	//if(Masses[h]==2.808391326)mflag=2.0;
	if(Masses[h]==2.80923)mflag=2.0;
	
	TFile fa("TGBichselt_kev.root","RECREATE");
	TGraph *TGBichselP = new TGraph(11901);
    ofstream outdata;
    outdata.open("dedx_mean_test.dat"); 
	float value;    
    for(int ipt=100;ipt<=12000;ipt++) {
      value= mflag*mflag*func->Eval(log10(ipt/1000.)); //chrage*charge!
      outdata<< ipt/1000. << "       " << value << endl;
	  //TGBichselP->SetPoint(ipt-100,ipt/1000./mflag,value*1.0e-6); //Fill in dedx vs. p/q
	  TGBichselP->SetPoint(ipt-100,ipt/1000./mflag,value); //Fill in dedx (kev) vs. p/q
//      cout << " p=" << ptot[ip] << "   dEdx=" << dedx[ip] << endl;
    }
    outdata<<" p(pt)(GeV/c)                dEdx(*1.0e-6 GeV/cm)"<< endl;
    outdata.close();

	TGBichselP->SetLineColor(4);
	TGBichselP->SetLineWidth(2);
	TGBichselP->Draw("LSame");
	TGBichselP->Write();
	fa.Close();

/*
    ofstream outdata;
    outdata.open("dedxmeanpi.dat");
    for(int ip1=0;ip1<26;ip1++) {
      float p = 0.5*(ptot1[ip1]+ptot1[ip1+1]);
      dedx1[ip1] = func->Eval(log10(p));
      cout << " p=" << p << "   dEdx=" << dedx1[ip1] << endl;
      outdata << p << "  " << dedx1[ip1] << endl;
    }
    outdata.close();

    float dedx2[22],ptot2[22];
    for(int i2=0; i2<22; i2++) {
      dedx2[i2] = dedx1[i2+1];
      ptot2[i2] = ptot1[i2+1];
    }

    TGraph *gr = new TGraph(26, ptot1, dedx1);
    gr->SetLineColor(4);
    gr->SetLineWidth(4);
    gr->Draw("c");
*/

  }
}

void TGBichsel(){
  /*
  TNtuple* nt = (TNtuple* ) gDirectory->Get("nt");
  TCanvas *c1 = new TCanvas();
  TH1F *frame = c1->DrawFrame(-1.1,0,1,29);
  frame->SetTitle("dE/dx selection for nSigma < 1.0 and nHit > 20");
  frame->SetXTitle("log_{10} p (GeV/c)                   ");
  frame->SetYTitle("dE/dx (keV/cm)");
  nt->SetMarkerColor(1);
  nt->Draw("1.e6*dedx:log10(p)","nh>20&&p>0.050","same");
  nt->SetMarkerColor(4);	
  nt->Draw("1.e6*dedx:log10(p)","nh>20&&p>0.050 && abs(npr)<1.0","same"); 
  nt->SetMarkerColor(2);	  
  nt->Draw("1.e6*dedx:log10(p)","nh>20&&p>0.050 && abs(npi)<1.0","same"); 
  nt->SetMarkerColor(3);	  
  nt->Draw("1.e6*dedx:log10(p)","nh>20&&p>0.050 && abs(nk)<1.0","same"); 
  nt->SetMarkerColor(6);	  
  nt->Draw("1.e6*dedx:log10(p)","nh>20&&p>0.050 && abs(ne)<1.0","same"); 
  */
  bichselG10();
}
