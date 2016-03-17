//#include "StSstDaqMaker.h"
  
//#undef _FILE_OFFSET_BITS
//#define _FILE_OFFSET_BITS 64 //used to open big file >2G
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

#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "TNtuple.h"
#include "TString.h"
#include <stdio.h>
#include <iostream>
#include "Riostream.h"
#include "string.h"
#include "TStopwatch.h"
#endif
//#include <unistd.h>
//#include <dirent.h>
//#include <exception>
using namespace std;
//#include <stdexcept>
void filesize(TString filename);
int main(int argc,char **argv)
{
 
  TStopwatch SW;
  SW.Start();
  TString FileInPut,FileOutPut;
  if(argc!=3) exit(1);
  FileOutPut = argv[2];

  FileInPut = argv[1];
  cout<<"Input File is:"<<FileInPut<<endl; 
  cout<<"Output File is:"<<FileOutPut<<endl;
  //filesize(FileInPut);   
  cout<<"START RAED DATA......"<<endl;
  FILE *pFile,*pWrite;
  
  pFile=fopen(FileInPut,"r");
  cout<<"pFile = "<<pFile<<endl;
  if(pFile==NULL) return 0;
  
  pWrite=fopen(FileOutPut,"wb");
  if(NULL==pWrite)
    {
      fclose(pFile);
      return 0;
    }

  cout<<"load file done!"<<endl;
  char m_sectorData[200];//char *m_sectorData = (char *)malloc(20);
  Int_t m_sectorDataLength=0;
  Int_t j=0,flag=0,flagblock=1;
  Int_t wordslength = 0;
  cout<<"start read data"<<endl;
  int i=0,event=0;
  int x=0;
  TString Line;  
  while(fgets(&m_sectorData[0],120,pFile))
  //while(!feof(pFile))
    {
      //fgets(&m_sectorData[0],120,pFile);
      x++;
      if(x%1000000==0) cout<<"PROCESS LINE "<<x<<endl;
      Line=m_sectorData;
      if(Line.Length()==1) continue;
      Line.Strip(TString::kTrailing,'\n');
      if(Line.BeginsWith("#"))
	{
	  if(Line.Contains("Event")) 
	    {
	      event++;
	      m_sectorDataLength=0;
	      flag =1;
            }
	  else if(Line.Contains("Block")) 
	    {

	      if((flagblock)%5==0) 
		{
		  i = 0;
		  flagblock = 0;
		}

	      flagblock+=1;

              fgets(m_sectorData,120,pFile);
	      continue;
            }
	  continue;
        }

      //cout<<Line.Data();
      m_sectorDataLength++;
      sscanf(&m_sectorData[0],"%x",&j);
      fwrite(&j,sizeof(j),1,pWrite);
      wordslength = Line.Length();
      
      i++;
      flag = 0;
      //if(i>100) return;
      /*
      if((flagblock+1)%5==0) 
        {
	  i = 0;
	  flagblock = 0;
        }
      */
      //memset(m_sectorData,0,sizeof(m_sectorData));
    }
  fclose(pFile);
  fclose(pWrite);
  cout<<"Convert ASCII to BIN file done!"<<endl;
  cout<<"The OutPut File is:"<<FileOutPut<<endl;
  /*
  UInt_t array[200] = {0};
  memset(m_sectorData,0x00,sizeof(m_sectorData));
  pFile=fopen("testV3.bin","rb");
  if(NULL==pFile)
    {
      cout<<"open testV3.bin failed"<<endl;
      return;
    }
  cout<<"Open testV3.bin sucessfully,each words length is "<<wordslength<<endl;
  fread(array,wordslength,20,pFile);
  for(int m=0;m<20;m++)
    {
      cout<<hex<<array[i]<<endl;
    }
  fclose(pFile);
  */
  SW.Stop();
  SW.Print();
  return(0);
  exit(0);
}

void filesize(TString filename)
{

  FILE *p = fopen(filename,"rb");
  fseek(p,0L,2);
  Long64_t length = ftell(p);
  cout<<"DATA Length1 is "<<length<<" and int length is "<<sizeof(int)<<endl;
  fseek(p,0L,SEEK_END);
  Long64_t length1 = ftell(p);
  cout<<"DATA Length2 is "<<length1<<endl;
  fclose(p);
  //return length;
  }
  
