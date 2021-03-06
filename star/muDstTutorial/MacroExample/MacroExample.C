void MacroExample( Int_t nFiles, Char_t InputFileList[256] ) 
{

  // Load Libraries

  gROOT->Macro("loadMuDst.C");

  // Create Histograms
  
  TCanvas*   nCanvas[2] ;
  TH1F*    histogram[2] ;

  const Int_t    nbins    =  100   ;

  nCanvas[0]    =  new TCanvas("C0","C0",150,150,600,600) ;
  nCanvas[1]    =  new TCanvas("C1","C1",250,250,600,600) ;

  histogram[0]  =  new TH1F( "Vertex", "Event Vertex Z Position", nbins, -25.0, 25.0 ) ; 
  histogram[1]  =  new TH1F( "Pt", "Transverse Momentum for all particles", nbins, 0.0, 10.0 ) ;

  // Run the muDstMaker

  StMuDstMaker* maker = new StMuDstMaker(0,0,"",InputFileList,"MuDst",nFiles) ;

  while ( !maker->Make() )     

    {

      StMuEvent* muEvent     = maker->muDst()->event() ;     

      // Do 'event' analysis based on event data 
      
      histogram[0] -> Fill( muEvent->primaryVertexPosition().z() ) ; // Make histogram of the vertex Z distribution
      
      // Get 'track' data, make cuts on tracks, do physics analysis, histogram results.
      
      TObjArray* tracks = maker->muDst()->primaryTracks() ;          // Create a TObject containing the primary tracks
      TObjArrayIter GetTracks(tracks) ;                              // Create an iterator to step through the tracks
      
      StMuTrack* track ;                                             // Pointer to a track
      while ( ( track = (StMuTrack*)GetTracks.Next() ) )             // Main loop for Iterating over tracks
	{
	  histogram[1] -> Fill( track->pt() ) ;
	}
 
    }
  
  // Finish using the muDSTmaker
  
  C0 -> cd() ;    
  histogram[0] -> Draw() ;
  C1 -> cd() ;    
  histogram[1] -> Draw() ;

}
