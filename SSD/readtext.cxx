//===============================
//Version: 1.3 
//Time: Sat May 17 18:17:49 EDT 2014 
//Author: Long Zhou 
//Discribe: This version can used to create pedestal files automaticly 

#ifdef __CINT__
 
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class PlotFile;
#endif
#ifndef __CINT__

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#include <iostream>
#include "Riostream.h"
//#include "stadio.h"
#include "TTree.h"
#include "TFile.h"
#include "TString.h"
#include "readtext.h"
#include "TStopwatch.h"
#endif
#include "TDatime.h"
#include <stdio.h>
#include <iostream>
#include "TChain.h"
#include "TCanvas.h"
#include <stdlib.h>
using namespace std;
UInt_t  MASK      = 1023;
UInt_t  THRESHOLD = 1023;
UInt_t  OFFSET    = 375;
UInt_t  NSIGMA    = 0;
UInt_t  CONSTANT  = 0; 
TDatime tBegin,tNow;
void CalculatePed_P(float ped,float rms,UInt_t &uped);
void CalculatePed_N(float ped,float rms,UInt_t &uped);
int main(int argc,char **argv)
{
  if(argc !=3) exit(1);
  TStopwatch SW;
  SW.Start();
  cout<<"----------Start of job----------"<<endl;
  //-----------
  fiber           = 0;
  RDO             = atoi(argv[2]); //atoi(const char*nptr). used to convert char to int.
  polarity        = 0;
  status          = 0;
  CurrentRms      = 0;
  CurrentPed      = 0;
  UCurrentPed     = 0; 
  fNumberOfEvents = 8*16*768;
  //-----------
  TString input;
  input = argv[1];
  TString output;
  output = "Pedestal_rdo";
  output += argv[2];
  output += ".txt";
  outdata1.open(output,ios::out); 
  cout<<"Input file is  :"<<input<<endl;
  cout<<"Output file is :"<<output<<endl;
  cout<<"Current RDO is :"<<RDO<<endl;  
  FILE *fp = fopen(input,"r");
  char line[101];
  cout<<"=================="<<endl;
  cout<<"START PRINT HEADER"<<endl;
  for(int i=0;i<5;i++)
    cout<<fgets(&line[0],100,fp)<<endl;
  cout<<"=================="<<endl;

  while (fgets(&line[0],100,fp)) {
    sscanf(&line[0],"%d%d%d%f%f",&fiber,&hybrid,&strip,&ped,&RMS);
    PedA[fiber][hybrid][strip] = ped;
    RMSA[fiber][hybrid][strip] = RMS;
  }
  for(int f=0;f<8;f++)
    {
      if(RDO<2) polarity = 0; //RDO 0,1---n-side
      else if(RDO==2)
	{

	  if(f<4) polarity = 0; // RDO 2.fiber <4 ---n-side
	  else polarity = 1; //RDO 2,fiber >4 ----p-side
	}
      else polarity = 1; //RDO 3,4---p-side
      outdata1<<Form("0x0%x09%x%03x",(((UInt_t)f)+8),polarity,THRESHOLD)<<endl;
      outdata1<<Form("0x0020000%x",((UInt_t)f))<<endl;
      outdata1<<Form("0x00210000")<<endl;
      outdata1<<Form("0x80210000")<<endl;
      outdata1<<Form("0x80210000")<<endl;

      for(int s=0;s<768;s++)
	{
	  for(int h=0;h<16;h++)
	    {
	      CurrentPed = PedA[f][h][s];
	      CurrentRms = RMSA[f][h][s];
	      //if(f==0&&s<10) cout<<"f "<<f<<" w "<<h<<" s "<<s<<" ped"<<CurrentPed<<" rms "<<CurrentRms<<endl;
	      if(CurrentRms>10.) status = 1;

	      if(polarity==0)
		CalculatePed_N(CurrentPed,CurrentRms,UCurrentPed);
	      else 
		CalculatePed_P(CurrentPed,CurrentRms,UCurrentPed);
	      if(status==0) 
		{
		  outdata1<<Form("0x00220%03x",UCurrentPed)<<endl;
		}
	      else 
		{
		  outdata1<<Form("0x00220%03x",1023)<<endl;
		}
	      status = 0;
	    }
	}
      outdata1<<Form("0x80210000")<<endl;
      //polarity = 0;
    }

  outdata1.close();
  fclose(fp);
  tNow.Set();
  cout<<"Number of Event: "<<fNumberOfEvents<<", elapsed time: "<<tNow.Convert()-tBegin.Convert()<<" seconds,rate: "<<float(fNumberOfEvents)/(tNow.Convert()-tBegin.Convert())<<endl;
  cout<<"----------End of job----------"<<endl;
  tNow.Print();
  return(0);
  exit(0);
}

void CalculatePed_P(float ped,float rms,UInt_t &uped)
{
  uped = (UInt_t)(ped + NSIGMA*rms + 0.5);
  uped = (uped - OFFSET + CONSTANT )&MASK;
}
void CalculatePed_N(float ped,float rms,UInt_t &uped)
{
  uped = (UInt_t)(ped - NSIGMA*rms + 0.5);
  uped = (uped - OFFSET - CONSTANT )&MASK;
}
