void padRefresh(TPad *pad,int flag=0);
void TestProfile()
{
  TCanvas *c1 = new TCanvas("c1","Profile histogram example",200,10,700,500);
  //TProfile *hprof  = new TProfile("hprof","Profile of pz versus px",100,-4,4,0,20);
  //hprof->Draw();  
  TH2I *hprof  = new TH2I("hprof","Profile of pz versus px",100,-4,4,20,0,20);
  hprof->Draw("COLZ");

  Float_t px, py, pz;
  for ( Int_t i=0; i<10000; i++) {
    gRandom->Rannor(px,py);
    pz = px*px + py*py;
    hprof->Fill(px,pz,1);
    if(i && (i%100 == 0))
      	padRefresh(c1);
  }
}

void padRefresh(TPad *pad,int flag)
{
  if (!pad) return;
  pad->Modified();
  pad->Update();
  TList *tl = pad->GetListOfPrimitives();
  if (!tl) return;
  TListIter next(tl);
  TObject *to;
  while ((to=next())) {
    if (to->InheritsFrom(TPad::Class())) padRefresh((TPad*)to,1);}
  if (flag) return;
  gSystem->ProcessEvents();
}
