void ConvertTaoGamma(Double_t input, Int_t Flag)
{
  // Flag = 0 ; lifttime 
  // Flag = 1 ; Energy Width
  // tao = hbar/Gamma

  const Double_t C_LIGHT     = 2.9979e8; //[m/s]
  const Double_t PlanckConst = 6.58211928e-22; // [Mev*s] 
  Double_t Tao   = 0; 
  Double_t Gamma = 0;
  Double_t CTao  = 0;

  if(Flag==0) 
    {
      Tao   = input; // [s]
      Gamma = (PlanckConst/input); // [MeV]
      CTao  = C_LIGHT*Tao*1e6; // [um]
    }
  else 
    {
      Gamma = input; // [MeV]
      Tao   = (PlanckConst/input); // [s]
      CTao  = C_LIGHT*Tao*1e6; // [um]
    }
  cout<<"================================="<<endl;
  cout<<"Life   Time  | "<<Tao*1e12<<"[ps]"<<endl;
  cout<<"Energy Width | "<<Gamma<<"[MeV]"<<endl;
  cout<<"Decay Length | "<<CTao<<"[um]"<<endl;
  cout<<"================================="<<endl;
}
