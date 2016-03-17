#include <iostream>
 struct pedestal_t {
   Int_t   sector;
   Int_t   rdo;
   Int_t   side;
   Int_t   ladder;
   Int_t   fiber;
   Int_t   hybrid;
   Int_t   strip;
   Float_t pedestal;
   Float_t RMS;
    
  };
  //fiber strip hybrid pedestal RMS
  pedestal_t pedestal;

//RDO to Ladder 
const int RDO2LADDER[5][8] = { {1,2,3,4,5,6,7,8},
			       {9,10,11,12,13,14,15,16},
			       {17,18,19,20,1,2,3,4},
			       {5,6,7,8,9,10,11,12},
			       {13,14,15,16,17,18,19,20}
};//Ladder cards number in each RDO channel .

void FindLadderSide(int RDO,int channel,int &ladder,int &side);
