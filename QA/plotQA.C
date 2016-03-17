void plotQA()
{

  const float Footx1 = 0.05; //Date
  const float Footx2 = 0.43; //Author/Aff
  const float Footx3 = 0.96; //Pagenumber
  const float Footy  = 0.3;

  TFile *f = new TFile("PiPlus_hist.root");

  TCanvas *cc = new TCanvas("cc", "cc",0,0,1024,768);
  gStyle->SetOptFit(0);
  gStyle->SetOptStat(0);
  gStyle->SetEndErrorSize(0.01);
  cc->SetFillColor(10);
  cc->SetBorderMode(0);
  cc->SetBorderSize(2);
  cc->SetFrameFillColor(0);
  cc->SetFrameBorderMode(0);
  cc->SetLogy();
  cc->SetGridx();
  cc->SetGridy();
  cc->SetLeftMargin(0.12);
  cc->SetBottomMargin(0.15);
  cc->SetTopMargin(0.025);
  cc->SetRightMargin(0.025);

  TPDF *pdf = new TPDF("PiPlus_Run10AuAu_Qa.pdf");

  TPad *pad1 = new TPad("pad1","",0.00,0.9,1.00,1.0);
  pad1->SetFillColor(TColor::GetColor("#3399CC"));
  pad1->Draw();  

  TPad *pad = new TPad("pad","",0.00,0.08,1.00,0.9);
  pad->Draw();

  TPad *padFoot = new TPad("padFoot","",0.00,0.00,1.00,0.05);
  padFoot->SetFillColor(TColor::GetColor("#3399CC"));
  padFoot->Draw();

  pad1->cd();
  tx.SetTextColor(kWhite);
  tx.SetTextSize(0.65);
  tx.DrawLatex(0.05,0.3,"#pi^{+} embedding QA 200 GeV Au+Au Run 10");

  padFoot->cd();
  tx.DrawLatex(Footx2, Footy, "Yifei Zhang / LBNL");
  TString datime  = (new TDatime())->AsSQLString();
  TString date = datime(0,10);
  tx.DrawLatex(Footx1, Footy, date.Data());

  pad->cd();
  tx.SetTextColor(1);
  tx.SetTextSize(TxDefSize);

  tx.SetTextSize(0.06);
  TH1F *hevCounter = (TH1F *)f->Get("eventCounter");
  int   nEvents    = hevCounter->GetBinContent(1);
  int   nEventsCut = hevCounter->GetBinContent(2);
  char tlName[100];
  sprintf(tlName, "Number of events: ");
  sprintf(tlName, "%s %d / %d", tlName, nEvents, nEventsCut);
  tx.DrawLatex(0.15, 0.8, tlName);
  //  tx.SetTextSize(0.05);
  tx.DrawLatex(0.15, 0.72, "10% of mult #pi^{+} per event");

  float ty1 = 0.58;
  float ty2 = 0.44;
  float dy  = 0.06;
  tx.SetTextSize(0.05);
  tx.DrawLatex(0.15, ty1, "Event Cuts:");
  tx.SetTextSize(0.045);
  tx.DrawLatex(0.2, ty1-dy, "|V_{Z}| < 50 cm, |#Delta V_{Z}| < 3 cm");
  tx.SetTextSize(0.05);
  tx.DrawLatex(0.15, ty2, "Track Cuts:");
  tx.SetTextSize(0.045);
  tx.DrawLatex(0.2, ty2-dy, "Primary tracks");
  tx.DrawLatex(0.2, ty2-dy*2, "p_{T} > 0.2 GeV, |#eta| < 1");
  tx.DrawLatex(0.2, ty2-dy*3, "nHits > 15, nHits/nPoss > 0.52");
  tx.DrawLatex(0.2, ty2-dy*4, "gDca < 2.0 cm");

  tx.SetTextColor(kBlack);
  tx.SetTextSize(0.04);
//  tx.DrawLatex(0.1, 0.93, "/eliza15/star/starprod/embedding/2010ProductionMinBias/PiPlus_100_2011601");
//  tx.DrawLatex(0.1,0.93,
  tx.DrawLatex(0.1,0.94,"/eliza15/star/starprod/hpss/staging/embedding/AuAu200_production/");
  tx.DrawLatex(0.1,0.89,"PiPlus_AuAu200GeV_NFit_Test");


  cc->cd();
  cc->Update();
  
  ////////////////////////

  pdf->NewPage();

  cc->cd();
  cc->Divide(2,2);
  
  cc_1->cd();
  TH1F *hrunId = (TH1F *)f->Get("runId");
  hrunId->SetMinimum(1.);
  hrunId->GetXaxis()->SetTitle("RunId");
  hrunId->Draw();

  cc_2->cd();
  TH1F *hbbcX = (TH1F *)f->Get("bbcX");
  hbbcX->SetAxisRange(4.,24.,"X");
  hbbcX->SetMinimum(1.);
  hbbcX->GetXaxis()->SetTitle("BBC Coincidence rate (kHz)");
  hbbcX->Draw();

  cc_3->cd();
  cc_3->SetLogy();
  TH1F *hrefMult = (TH1F *)f->Get("refMult");
  hrefMult->Rebin(5);
  hrefMult->SetAxisRange(0.,680,"X");
  hrefMult->SetMinimum(1.);
  hrefMult->GetYaxis()->SetTitle("");
  hrefMult->GetXaxis()->SetTitle("RefMult");
  hrefMult->Draw();

  cc_4->cd();
  cc_4->SetLogz();

  TH2F *hvxyMc = (TH2F *)f->Get("vxyMc");
  hvxyMc->SetMinimum(1.);
  hvxyMc->GetYaxis()->SetTitle("VyMc (cm)");
  hvxyMc->GetXaxis()->SetTitle("VxMc (cm)");
  hvxyMc->Draw("col");

  cc->cd();
  cc->Update();

  pdf->NewPage();

  cc->cd();
  cc->Divide(2,2);

  cc_1->cd();
  TH1F *hVzMc = (TH1F *)f->Get("VzMc");
  hVzMc->SetMinimum(1.);
  hVzMc->GetXaxis()->SetTitle("VzMc (cm)");
  hVzMc->Draw();

  cc_2->cd();
  TH1F *hvzRcMcDiff = (TH1F *)f->Get("vzRcMcDiff");
  hvzRcMcDiff->SetMinimum(1.);
  hvzRcMcDiff->GetXaxis()->SetTitle("VzRc - VzMc (cm)");
  hvzRcMcDiff->Draw();

  cc_3->cd();
  cc_3->SetLogy(0);
  cc_3->SetLogz();
  TH2F *hvzRcCorr = (TH2F *)f->Get("vzRcCorr");
  hvzRcCorr->GetYaxis()->SetTitle("vpdVz (cm)");
  hvzRcCorr->GetXaxis()->SetTitle("Vz (cm)");
  hvzRcCorr->Draw("col");

  cc_4->cd();
  TH1F *hVzRcDiffZoom = (TH1F *)f->Get("VzRcDiffZoom");
  hVzRcDiffZoom->SetMinimum(1.);
  hVzRcDiffZoom->GetXaxis()->SetTitle("Vz - vpdVz (cm)");
  hVzRcDiffZoom->Draw();

  TF1 *fun = new TF1("fun","gaus",-2.,2.);
  hVzRcDiffZoom->Fit(fun,"NOR");
  fun->SetRange(-5.,5.);
  fun->Draw("same");

  char chh1[50],chh2[50],chh3[50],chh4[50];
  sprintf(chh1,"#chi^{2} / ndf = %6.2f / %d",fun->GetChisquare(),fun->GetNDF());
  sprintf(chh2,"p0 = %4.2e #pm %4.2e",fun->GetParameter(0),fun->GetParError(0));
  sprintf(chh3,"mean = %5.3f #pm %5.3f",fun->GetParameter(1),fun->GetParError(1));
  sprintf(chh4,"#sigma = %5.3f #pm %5.3f",fun->GetParameter(2),fun->GetParError(2));
  
  TPaveStats *psts = new TPaveStats(0.65,0.65,0.93,0.95,"brNDC");
  psts->SetName("stats");
  psts->SetBorderSize(2);
  psts->SetFillColor(0);
  psts->SetTextAlign(12);
  psts->SetTextSize(0.03);
  TText *text = psts->AddText(chh1);
  text = psts->AddText(chh2);
  text = psts->AddText(chh3);
  text = psts->AddText(chh4);
  psts->Draw();
  
  cc->cd();
  cc->Update();
  
  pdf->NewPage();

  cc->cd();
  cc->Divide(2,2);

  cc_1->cd();
  TH2F *hPtY_Mc = (TH2F *)f->Get("PtY_Mc");
  hPtY_Mc->SetAxisRange(0.,5.5,"X");
  hPtY_Mc->SetAxisRange(-1.3,1.3,"Y");
  hPtY_Mc->GetXaxis()->SetTitle("McPt (GeV/c)");
  hPtY_Mc->GetYaxis()->SetTitle("McEta");
  hPtY_Mc->Draw("col");

  cc_2->cd();
  TH2F *hPtPhi_Mc = (TH2F *)f->Get("PtPhi_Mc");
  hPtPhi_Mc->SetAxisRange(0.,5.5,"X");
  hPtPhi_Mc->SetAxisRange(-1.3,1.3,"Y");
  hPtPhi_Mc->GetXaxis()->SetTitle("McPt (GeV/c)");
  hPtPhi_Mc->GetYaxis()->SetTitle("McPhi (rad)");
  hPtPhi_Mc->Draw("col");

  cc_3->cd();
  cc_3->SetLogz(0);
  TH2F *hPtEta = (TH2F *)f->Get("PtEta");
  hPtEta->SetAxisRange(0.,5.5,"X");
  hPtEta->SetAxisRange(-1.3,1.3,"Y");
  hPtEta->GetXaxis()->SetTitle("RcPt (GeV/c)");
  hPtEta->GetYaxis()->SetTitle("RcEta");
  hPtEta->Draw("col");

  cc_4->cd();
  cc_4->SetLogz(0);
  TH2F *hPtPhi = (TH2F *)f->Get("PtPhi");
  hPtPhi->SetAxisRange(0.,5.5,"X");
  hPtPhi->SetAxisRange(-1.3,1.3,"Y");
  hPtPhi->GetXaxis()->SetTitle("RcPt (GeV/c)");
  hPtPhi->GetYaxis()->SetTitle("RcPhi (rad)");
  hPtPhi->Draw("col");

  cc->cd();
  cc->Update();

  pdf->NewPage();

  cc->cd();
  cc->Divide(2,2);

  cc_1->cd();
  TH2F *hdEdx = (TH2F *)f->Get("dEdx");
  hdEdx->GetYaxis()->SetTitleOffset(0.8);
  hdEdx->GetXaxis()->SetTitleOffset(0.8);
  hdEdx->GetYaxis()->SetTitle("dE/dx (keV/cm)");
  hdEdx->GetXaxis()->SetTitle("Momentum (GeV/c)");
  hdEdx->Draw("col");

  cc_2->cd();
  cc_2->SetLogy(0);
  cc_2->SetLogz();
  TH2F *hPtCorr = (TH2F *)f->Get("PtCorr");
  hPtCorr->SetAxisRange(0.,6.,"X");
  hPtCorr->SetAxisRange(0.,6.,"Y");
  hPtCorr->GetYaxis()->SetTitle("RcPt (GeV/c)");
  hPtCorr->GetXaxis()->SetTitle("McPt (GeV/c)");
  hPtCorr->Draw("col");

  cc_3->cd();
  cc_3->SetLogz();
  TH2F *hPtDiff = (TH2F *)f->Get("PtDiff");
  hPtDiff->SetName("hPtDiff");
  hPtDiff->GetXaxis()->SetTitle("McPt (GeV/c)");
  hPtDiff->GetYaxis()->SetTitle("(RcPt-McPt)/McPt");
  hPtDiff->SetAxisRange(0.,6.,"X");
  //  hPtDiff->SetAxisRange(0.,6.,"Y");
  hPtDiff->Draw("col");

  cc_4->cd();
  cc_4->SetLogy();
  hPtDiff->FitSlicesY();
  hPtDiff_2->SetTitle("");
  hPtDiff_2->GetXaxis()->SetTitle("McPt (GeV/c)");
  hPtDiff_2->SetAxisRange(0.,6.,"X");
  hPtDiff_2->SetAxisRange(5e-3,0.05,"Y");
  hPtDiff_2->SetMarkerStyle(24);
  hPtDiff_2->SetMarkerSize(0.8);
  hPtDiff_2->Draw();
  
  TF1 *funptres = new TF1("funptres","sqrt( [0]*[0]*x*x + [1]*[1]*(0.13957*0.13957/x/x + 1.) )",0.,5.);
  funptres->SetParameters(2.7e-3,8e-3);
  hPtDiff_2->Fit(funptres,"NOR");
  funptres->Draw("same");

  cc->cd();
  cc->Update();

  cc->Clear();
  cc->cd();
  cc->Divide(2,2);

  cc_1->cd();
  TH2F *hnHits = (TH2F *)f->Get("nHits");
  TH1F *hnHits1 = (TH1F *)hnHits->ProjectionY("hnHits1",1,10);
  hnHits1->GetXaxis()->SetTitle("nFitPts");
  hnHits1->Draw();
  tx.DrawLatex(0.2,0.8,"0.0 < p_{T} < 0.5 GeV/c");

  cc_2->cd();
  TH2F *hnHits = (TH2F *)f->Get("nHits");
  TH1F *hnHits2 = (TH1F *)hnHits->ProjectionY("hnHits2",11,20);
  hnHits2->GetXaxis()->SetTitle("nFitPts");
  hnHits2->Draw();
  tx.DrawLatex(0.2,0.8,"0.5 < p_{T} < 1.0 GeV/c");

  cc_3->cd();
  TH2F *hnHits = (TH2F *)f->Get("nHits");
  TH1F *hnHits3 = (TH1F *)hnHits->ProjectionY("hnHits3",21,30);
  hnHits3->GetXaxis()->SetTitle("nFitPts");
  hnHits3->Draw();
  tx.DrawLatex(0.2,0.8,"1.0 < p_{T} < 1.5 GeV/c");

  cc_4->cd();
  TH2F *hnHits = (TH2F *)f->Get("nHits");
  TH1F *hnHits4 = (TH1F *)hnHits->ProjectionY("hnHits4",31,40);
  hnHits4->GetXaxis()->SetTitle("nFitPts");
  hnHits4->Draw();
  tx.DrawLatex(0.2,0.8,"1.5 < p_{T} < 2.0 GeV/c");

  cc->cd();
  cc->Update();

  cc->Clear();
  cc->cd();
  cc->Divide(2,2);

  cc_1->cd();
  TH2F *hdca = (TH2F *)f->Get("dca");
  TH1F *hdca1 = (TH1F *)hdca->ProjectionY("hdca1",1,10);
  hdca1->GetXaxis()->SetTitle("dca (cm)");
  hdca1->SetAxisRange(0.,2.5,"X");
  hdca1->Draw();
  tx.DrawLatex(0.6,0.8,"0.0 < p_{T} < 0.5 GeV/c");

  cc_2->cd();
  TH2F *hdca = (TH2F *)f->Get("dca");
  TH1F *hdca2 = (TH1F *)hdca->ProjectionY("hdca2",11,20);
  hdca2->GetXaxis()->SetTitle("dca (cm)");
  hdca2->SetAxisRange(0.,2.5,"X");
  hdca2->Draw();
  tx.DrawLatex(0.6,0.8,"0.5 < p_{T} < 1.0 GeV/c");

  cc_3->cd();
  TH2F *hdca = (TH2F *)f->Get("dca");
  TH1F *hdca3 = (TH1F *)hdca->ProjectionY("hdca3",21,30);
  hdca3->GetXaxis()->SetTitle("dca (cm)");
  hdca3->SetAxisRange(0.,2.5,"X");
  hdca3->Draw();
  tx.DrawLatex(0.6,0.8,"1.0 < p_{T} < 1.5 GeV/c");

  cc_4->cd();
  TH2F *hdca = (TH2F *)f->Get("dca");
  TH1F *hdca4 = (TH1F *)hdca->ProjectionY("hdca4",31,40);
  hdca4->GetXaxis()->SetTitle("dca (cm)");
  hdca4->SetAxisRange(0.,2.5,"X");
  hdca4->Draw();
  tx.DrawLatex(0.6,0.8,"1.5 < p_{T} < 2.0 GeV/c");

  cc->cd();
  cc->Update();

  cc->cd();
  cc->SetLogy(0);
  TH1F *heff = (TH1F *)f->Get("eff");
  heff->SetAxisRange(-0.05,1.1,"Y");
  heff->GetYaxis()->SetNdivisions(510);
  heff->GetYaxis()->SetTitleOffset(0.9);
  heff->GetXaxis()->SetTitleOffset(0.9);
  heff->GetYaxis()->SetTitle("Efficiency");
  heff->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  heff->SetMarkerStyle(24);
  heff->Draw("P");

  TF1 *funeff = new TF1("funeff","[0]", 0.8, 5.);
  heff->Fit(funeff,"NOR");
  funeff->Draw("same");

  TLine *ll = new TLine(0.02,1.,4.98,1.);
  ll->SetLineWidth(1);
  ll->SetLineStyle(2);
  ll->Draw("same");

  cc->cd();
  cc->Update();

  pdf->Close();
}

