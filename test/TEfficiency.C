void TEfficiency()
 {
   TFile *pFile = new TFile("myEff.root","recreate");
   //canvas only needed for the documentation
   TCanvas* c1 = new TCanvas("c1","",600,400);
   c1->Divide(2);
   c1->SetFillStyle(1001);
   c1->SetFillColor(kWhite);

   //create one-dimensional TEfficiency object with fixed bin size
   TEfficiency* pEff = new TEfficiency("eff","different confidence levels;x;#epsilon",20,0,10);
   TRandom3 rand;

   bool bPassed;
   double x;
   for(int i=0; i<1000; ++i)
     {
       //simulate events with variable under investigation
       x = rand.Uniform(10);
       //check selection: bPassed = DoesEventPassSelection(x)
       bPassed = rand.Rndm() < TMath::Gaus(x,5,4);
       pEff->Fill(bPassed,x);
     }

   //set style attributes
   pEff->SetFillStyle(3004);
   pEff->SetFillColor(kRed);
   cout<<"The Conf level is : "<<pEff->GetConfidenceLevel()<<endl;

   //copy current TEfficiency object and set new confidence level
   TEfficiency* pCopy = new TEfficiency(*pEff);
   pCopy->SetConfidenceLevel(0.90);

   //set style attributes
   pCopy->SetFillStyle(3005);
   pCopy->SetFillColor(kBlue);

   c1->cd(1);

   //add legend
   TLegend* leg1 = new TLegend(0.3,0.1,0.7,0.5);
   leg1->AddEntry(pEff,"95%","F");
   leg1->AddEntry(pCopy,"68.3%","F");

   pEff->Draw("A4");
   pCopy->Draw("same4");
   leg1->Draw("same");

   //use same confidence level but different statistic methods
   TEfficiency* pEff2 = new TEfficiency(*pEff);
   TEfficiency* pCopy2 = new TEfficiency(*pEff);

   pEff2->SetStatisticOption(TEfficiency::kFNormal);
   pCopy2->SetStatisticOption(TEfficiency::kFAC);

   pEff2->SetTitle("different statistic options;x;#epsilon");

   //set style attributes
   pCopy2->SetFillStyle(3005);
   pCopy2->SetFillColor(kBlue);

   c1->cd(2);

   //add legend
   TLegend* leg2 = new TLegend(0.3,0.1,0.7,0.5);
   leg2->AddEntry(pEff2,"kFNormal","F");
   leg2->AddEntry(pCopy2,"kFAC","F");

   pEff2->Draw("a4");
   pCopy2->Draw("same4");
   leg2->Draw("same");

   pEff2->SetDirectory(gDirectory);
   pFile->Write();

   //only for this documentation
   c1->cd(0);
   return c1;
 }
