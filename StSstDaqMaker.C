/*!
 * \class StSstDaqMaker
 * \author Long Zhou, Dec 2013, according codes from PXL
 * \Initial Revision.
 */
/***************************************************************************
 *
 * $Id$
 *
 * Author: Long Zhou, Nov 2013, according codes from PXL
 ***************************************************************************
 *
 * Description:
 * Read ssd strip info from daq format.
 * In this code ,where here found 2 line of "@".it's means here need add or update something!
 *
 ***************************************************************************

$Log$
version 1.1
<1> change daqmaker .C code and try to use root run this code directly
<2> Each event's length can not read from the header directly,because header info is not right.

version 1.11
2013-12-06
<1>this version is depend on a new DAQ data format,
<2>this code only used to decode T_0001_All.txt,this file do not have any TC * D info,and also have some mistakers ,for example, too much header token 0xaa * aaaaaa
<3>because do not have TCD info ,so the TAIL_LENGTH=4.

version 1.2
2013-12-09
<1>this version code only used to test T_0001h_TCD.txt,
<2>this file have TCD info ,I just add check TCD info code.

version 1.3
2013-12-11
<1>Add some code to avoid the  data have mistakes.
<2>  
 **************************************************%************************/
#include "StSstDaqMaker.h"
#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "TNtuple.h"
#include <stdio.h>
#include <exception>
#include <stdexcept>
#include "iostream"
using namespace std;

#define HEADER_LENGTH        8
#define TAIL_LENGTH          2  //2013-12-10,this is not include TCD info
#define FIBER_HEADER_LENGTH  10

#define HEADER_TOKEN         0xAAAAAAAA
#define END_TOKEN            0xBBBBBBBB
#define TCD_TOKEN            0xCCCCCCCC
#define FIBER_LINK_TOKEN     0xDDDDDDDD
#define TCD_END_TOKEN        0xEEEEEEEE

#define RDO_START            24//RDO Number Start bit
#define RDO_END              28//RDO Number End bit
#define FIBER_START          28//Fiber Input Start bit
#define FIBER_END            31//Fiber Input End bit
#define HYBRID_ONE_START     0 //Hyirid 0 start bit
#define HYBRID_ONE_END       10//Hyirid 0 end bit
#define HYBRID_TWO_START     10
#define HYBRID_TWO_END       20
#define HYBRID_THREE_START   20
#define HYBRID_THREE_END     30
#define HYBRID_START         10
#define HYBRID_END           14
#define STRIP_START          14
#define STRIP_END            24   
#define ADC_START            4    //adc length start bit
#define ADC_END              20   //adc length end bit
#define DATAMODE_START       0    //adc mode start bit
#define DATAMODE_END         4    //adc mode end bit
#define RAWMODE              0  //0x0 this is data mode,raw or compressed
#define COMPRESSEDMODE       1  //0x1 here 0x0 is RAW mode,and 0x1 is COMPRESSED mode
#define FLAG_START           20     //flag start bit
#define FLAG_END             32     //flag end bit
#define NORMAL               0  //0x000  //normal flag
#define NODATA               1  //0x001  //no data
#define OVERFLOW             2  //0x002  //over flow
#define EARLY_ABORT          3  //0x003  //early abort 
#define WRONG_PIPE_MODE      4  //0x004  //wrong pipe mode
#define ADC_LENGTH_LIMIT     4106   //adc length limit,include fiber header

//-----------------------------------------------
void Init()
{
  cout<<"StSstDaqMaker initialization..."<<endl;
  //initination ...
  occupancy_wafer = new TH3I("occupancy_wafer","occupancy per wafer",20,0,20,768,0,768,1024,0,1024);
  occupancy_wafer->SetXTitle("Wafer");
  occupancy_wafer->SetYTitle("Strip");  
  occupancy_wafer->SetZTitle("ADC");
  occupancy_chip = new TH2I("occupancy_chip","Fiber Vs Wafer",8,0,8,20,0,20);
  occupancy_chip->SetXTitle("Fiber/Ladder");
  occupancy_chip->SetYTitle("Wafer");  

 
  for(int j=0;j<8;j++)
    {
      AdcStrip[j] = new TH2I(Form("AdcStrip%d",j),"Adc vs strip for each ladder;(Strip #)#times(Wafer #));Adc Value",12288,0,12288,1024,0,1024);	  
    }
 
  if(mPedOut)
    {
      DeclareNTuple();
    }
  cout << "Init() - Done " << endl;
  //return kStOk;
}
//--------------
void WriteHist()
{
  cout<<"start write histogram"<<endl;
  pfile->cd();
  occupancy_wafer->Write();
  occupancy_chip->Write();

  for(int j=0;j<8;j++)
    {
      AdcStrip[j]->Write();
    }

  pfile->Close();
  occupancy_wafer->Delete();
  occupancy_chip->Delete();
  
  for(int j=0;j<8;j++)
    {
      AdcStrip[j]->Delete();
    }
    
 
  cout<<"Done!"<<endl;
}
//--------------
void SetDaqFileName(TString inputfile)
{
  TString name=inputfile;
  fDaqName=name;
}
//--------------
void SetOutFileName(TString outputfile)
{
  fOutName=outputfile;
}
//--------------
Int_t LoadDaqFile(TString daqFileName)
{
cout<<"load DAQ file.."<<endl;
Int_t file_size = FileSize(daqFileName);

if(Debug()>1)
{
cout<<"File Size in byte is "<<file_size<<endl;
}
if(pFile!=0) fclose(pFile);
cout<<"Open data file.."<<endl;
pFile=fopen(daqFileName,"rb");
if(NULL==pFile) return 0;
cout<<"Load DAQ File done.."<<endl;
 return 1;
}
//--------------
Int_t Debug()
{
  return m_Debug;
}
//---------------
Int_t SetDebug(Int_t debug)
{
  m_Debug = debug;
  return m_Debug;
}
//---------------
Int_t SetPedOut(int pedout)
{
  mPedOut = pedout;
  return mPedOut;
}
//---------------
Int_t FileSize(TString fn)
{
  FILE *p = fopen(fn,"rb");
  fseek(p,0L,SEEK_END);
  Int_t fz=ftell(p);
  fclose(p);
  return fz;
}
//-------------------------------------------------
void DeclareNTuple(){
  pfile = new TFile(fOutName,"RECREATE");
  string varlist = "side:ladder:wafer:strip:adc:eventno:readout:RDO:channel";
  pTuple     = new TNtuple("PedestalNTuple","Pedestal Ntuple",varlist.c_str());
  cout << "StSsdDaqMaker::DeclareNtuple - Done - "<<endl;
}
//-----------------
Int_t CheckHeader(UInt_t words[],int length,int &m)
{
  int kError = 1;
  if(words[0]!=HEADER_TOKEN)
    {
      cout<<"ERROR:Bad HEADER TOKEN, Stop!"<<endl;
      cout<<"Wrong Header : "<<hex<<words[0]<<dec<<endl;
      kError=0;
    }
  cout<<"INFO:New Event!"<<endl;
  m_RDO = Mid(RDO_START,RDO_END,words[3]);
  m = m+length;
  eventnumber++;
  return kError;
}
//----------------
Int_t CheckFiberHeader(UInt_t words[],int length,int &m)
{
  int kError = 1;
  //for(int i=0;i<length;i++) cout<<"INFO: 0x"<<hex<<words[i]<<dec<<endl;
  if(words[0]!=FIBER_LINK_TOKEN)
    {
      cout<<"ERROR:Bad FIBER LINK TOKEN, Stop!"<<endl;
      cout<<"INFO:Print bad Fiber token words "<<endl;
      //for(int i=0;i<length;i++) cout<<"INFO: 0x"<<hex<<words[i]<<dec<<endl;
      kError=0;
    }
  m_mode = Mid(DATAMODE_START,DATAMODE_END,words[1]);
  
  if(m_mode==RAWMODE) cout<<"INFO:Current data Mode is RAW DATA mode."<<endl;\
  else if(m_mode==COMPRESSEDMODE) cout<<"INFO:Current data Mode is COMPRESSED DATA mode. "<<endl;
  else 
    {
      cout<<"ERROR:No such kind of DATA MODE ! STOP !!!"<<endl;
      kError=0;
    }
 
  m_adcLength = Mid(ADC_START,ADC_END,words[1]);
  cout<<"INFO:Current Adc length is m_adclength = "<<m_adcLength<<endl;
  if(m_adcLength>ADC_LENGTH_LIMIT) {cout<<"ERROR:ADC Length is large than limit, Stop !"<<endl; kError=0;}
  m_flag = Mid(FLAG_START,FLAG_END,words[1]);
  if(m_flag==NODATA)
    {
      cout<<"INFO: FLAG is NO DATA,let's check the ADC length"<<endl;
      if(m_adcLength==FIBER_HEADER_LENGTH)
	{
	  cout<<"INFO:flag and adc length is consistent! let's look at next fiber.."<<endl;
	}
      else 
	{
	  cout<<"ERROR:flag and adc length is not consistent,Stop !"<<endl;
	  kError=0;
	}
    }
  if(m_flag==OVERFLOW) {cout<<"ERROR:This Fiber Flag is :Over Flow"<<endl; kError=0;}
  if(m_flag==EARLY_ABORT) {cout<<"ERROR:Abort Early!"<<endl; kError=1;}//thorsten request
  if(m_flag==WRONG_PIPE_MODE) {cout<<"ERROR:Wrong pipe mode,Stop!"<<endl; kError=0;}

  m_channel = Mid(FIBER_START,FIBER_END,words[2]);
  m = m+length;
  return kError;
}
//-----------------
Int_t CheckTailHeader(UInt_t words[],int length,int &m)
{
  int kError=1;
  if(words[1]!=END_TOKEN) {cout<<"ERROR:Bad End Token, Stop"<<endl; kError=0;}
  m = m+length;
  return kError;
}

//-------------------------------------------------
void StSstDaqMaker(TString inputdaq="testV3.bin",TString outputroot="testV3.root")
{
  cout<<"StSstDaqMaker Start Make..."<<endl;
  SetDaqFileName(inputdaq);
  SetOutFileName(outputroot);
  SetDebug(0);
  SetPedOut(1);  
  Init();
  //DeclareNTuple();
  
  int m;
  Int_t ok = LoadDaqFile(fDaqName);
  if(ok) {cout<<"INFO:load data correct.."<<endl;}
  else {cout<<"ERROR:can not load data,Stop"<<endl; return;}
  m_filelength=FileSize(fDaqName);
  while(fread(m_sectorData,sizeof(UInt_t),HEADER_LENGTH,pFile))
    {
      ok =  CheckHeader(m_sectorData,HEADER_LENGTH,m);
      if(ok) {cout<<"INFO:header ok"<<endl;}     
      else 
	{
	  cout<<"ERROR:header wrong, Skip this event !"<<endl; 
	  while(1) 
	    {
	      fread(m_sectorData,sizeof(UInt_t),1,pFile);
	      if(m_sectorData[0]==HEADER_TOKEN)
		{
		  cout<<"INFO:Found next event!"<<endl;
		  fseek(pFile,-4,SEEK_CUR);
		  break;
		}
	    }
	  continue;
	}     
      for(int i=0;i<8;i++)
	{
	  cout<<"INFO:-----------------Decoding Fiber["<<i<<"]--------------------"<<endl;
	  fread(m_sectorData,sizeof(UInt_t),FIBER_HEADER_LENGTH,pFile);
	  ok = CheckFiberHeader(m_sectorData,FIBER_HEADER_LENGTH,m);
	  if(ok) {cout<<"INFO:Fiber Token ok"<<endl;}
	  else 
	    {
	      while(1)
		{
		  /*
		  fpos_t pos; 
		  fgetpos(pFile,&pos); // get currecnt read point position
		  printf("Befor read :current position is %d\n",pos);
		  */
		  fread(m_sectorData,sizeof(UInt_t),1,pFile);
		  m_channel = i;
		  /*
		  fpos_t pos1;
		  fgetpos(pFile,&pos1);
		  printf("After read :current position is %d\n",pos1);
		  */
		  if(i==7)
		    {
		      if(m_sectorData[0]==TCD_TOKEN) 
			{

			  cout<<"Found TCD Token :0x"<<hex<<m_sectorData[0]<<dec<<endl;
			  /*
			  fpos_t pos;
			  fgetpos(pFile,&pos);
			  printf("current position is %d\n",pos);
			  cout<<"after shift back one word"<<endl;
			  */
			  fseek(pFile,-4,SEEK_CUR);//shift back 4 words to the 0xdddddddd
			  //cout<<"offset="<<ftell(pFile)<<endl;
			  break;
			}
		    }
		  else 
		    {
		      if(m_sectorData[0]==FIBER_LINK_TOKEN) 
			{
			  cout<<"Found next Fiber Token :0x"<<hex<<m_sectorData[0]<<dec<<endl;
			  /*
                          fpos_t pos;
			  fgetpos(pFile,&pos);
			  printf("current position is %d\n",pos);
			  cout<<"after shift back one word"<<endl;
			  */
			  fseek(pFile,-4,SEEK_CUR);//shift back 4 words to the 0xdddddddd

			  //cout<<"offset="<<ftell(pFile)<<endl;
			  break;
			}
		    }
		  m = m + 1;
		}
	      continue;
	    }
	  fread(m_sectorData,sizeof(UInt_t),m_adcLength-FIBER_HEADER_LENGTH,pFile);
	  ok = DecodeHitsData();
	  if(ok) {cout<<"INFO:DecodeHitsData,Good to go"<<endl;}
	  else {cout<<"ERROR:Decode hit data failure, Stop!"<<endl; return;}

	  cout<<"INFO:Current adc_Length = "<<m_adcLength<<endl;
	}
	 
      fread(m_sectorData,sizeof(UInt_t),1,pFile);//TCD token
      cout<<"INFO:Check TCD Token"<<endl;
      if(m_sectorData[0]!=TCD_TOKEN) 
	{
	  cout<<"ERROR:Bad TCD Token,Stop!"<<endl;
	  cout<<"INFO:BAD TCD:"<<m_sectorData[0]<<endl; 
	  //return;
	}
      while(1)
	{
	  fread(m_sectorData,sizeof(UInt_t),1,pFile);
	  if(m_sectorData[0]==TCD_END_TOKEN)  break;
	  m_TCDLength++; 
	}
      cout<<"INFO:Current TCD words length is "<<m_TCDLength++<<endl;

      fread(m_sectorData,sizeof(UInt_t),TAIL_LENGTH,pFile);
      ok =  CheckTailHeader(m_sectorData,TAIL_LENGTH,m);//check tail ...
      if(ok) cout<<"CheckTailHeader:Good to go"<<endl;
      else return;
      cout<<"INFO:Event Number = "<<eventnumber<<endl;
      cout<<"*********************Process Event "<<eventnumber<<" Done!***********************"<<endl;
      m_TCDLength = 0;
      //if(eventnumber==100) break;
    }
  cout<<"INFO:READE DONE, LET'S WRITE HISTOGRAM!!!"<<endl;
  WriteHist();
}

//-------------------------------------------------
Int_t GetHitsDataLength(int FiberNumber)
{
  return m_adcLength;
  //get the data length
}
//------------------------------------------------    
Int_t DecodeHitsData()
{
  int temp = m_adcLength - FIBER_HEADER_LENGTH;
  Int_t kError = 1;
  if(temp==0) 
    {
      //if one fiber do not have any data, just continue
      cout<<"Fiber ["<<m_channel<<"] do not have any data..."<<endl;
	  return 1;
    }
	if(m_mode==RAWMODE) {
	  kError = DecodeRawWords(m_sectorData,temp,m_channel);
	}
    if(m_mode==COMPRESSEDMODE) DecodeCompressedWords(m_sectorData,temp,m_channel);
    return kError;
}
//-------------------------------------------------
int DecodeRawWords(UInt_t val[],int vallength,int channel)
{
  int strip_number[3],id_wafer[3],id_side,count,my_channel[3]={-9999,-9999,-9999};
  int ladderCountN[20]={0};
  int ladderCountP[20]={0};
  int data[3],pedestal[3],wafer[3],noise[3],strip[3],ladder,laddercard;
  for(int i=0;i<3;i++)
    {
    data[i]     = 0;
	pedestal[i] = 0;
	wafer[i]    = 0;
	strip[i]    = 0;
	noise[i]    = 0;
    }

 count = 1;
 cout<<"Decode Raw Data, Adc Length = "<<m_adcLength<<endl;
 //----
for(int i=0;i<vallength;i++)
{
  if(Debug()>1)
    {
      if(i%1000==0) cout<<"INFO:PROCESSING "<<(float)i/(float)vallength*100.<<"%"<<endl;
    }
  if(i==0) wafer[0] = 0;
  else  wafer[0] = wafer[2] + 1; 
  if(wafer[0]>=16) wafer[0] = wafer[0]%16;
  wafer[1] = wafer[0] + 1;
  if(wafer[1]>=16) wafer[1] = wafer[1]%16;
  wafer[2] = wafer[0] + 2;
  if(wafer[2]>=16) wafer[2] = wafer[2]%16;
  if(Debug()>1)
    {
      cout<<"Three wafer number in current word is :"<<wafer[0]<<","<<wafer[1]<<","<<wafer[2]<<endl;
    }
if(i==0)
{
  
  for(int j=0;j<3;j++)
    {
      strip[j] = 0;
		//cout<<"strip "<<j<<endl;
    }
if(Debug()>1) 
  {
    cout<<"[adc_pointer,strip_0,wafer_0] = ["<<i<<","<<strip[0]<<","<<wafer[0]<<"]"<<endl;
    cout<<"[adc_pointer,strip_1,wafer_1] = ["<<i<<","<<strip[1]<<","<<wafer[1]<<"]"<<endl;
    cout<<"[adc_pointer,strip_2,wafer_2] = ["<<i<<","<<strip[2]<<","<<wafer[2]<<"]"<<endl;
  }
 }
else
{
  
if(wafer[0]==0) strip[0] = strip[0] + 1;
if(strip[2]>strip[0]) strip[0] = strip[2];
if(wafer[1]==0) strip[1] = strip[1] + 1;
if(strip[0]>strip[1]) strip[1] = strip[0];
if(wafer[2]==0) strip[2] = strip[2] + 1;
if(strip[1]>strip[2]) strip[2] = strip[1];
if(Debug()>1) 
  {
    cout<<"[adc_pointer,strip_0,wafer_0] = ["<<i<<","<<strip[0]<<","<<wafer[0]<<"]"<<endl;
    cout<<"[adc_pointer,strip_1,wafer_1] = ["<<i<<","<<strip[1]<<","<<wafer[1]<<"]"<<endl;
    cout<<"[adc_pointer,strip_2,wafer_2] = ["<<i<<","<<strip[2]<<","<<wafer[2]<<"]"<<endl;
  }

}
 
data[0] = Mid(HYBRID_ONE_START,HYBRID_ONE_END,val[i]);//number one hybrid data
data[1] = Mid(HYBRID_TWO_START,HYBRID_TWO_END,val[i]);//number two hybird data
data[2] = Mid(HYBRID_THREE_START,HYBRID_THREE_END,val[i]);//number three hybird data
for(int n=0;n<3;n++)
{
  occupancy_wafer->Fill(wafer[n],strip[n],data[n]);
  occupancy_chip->Fill(m_channel,wafer[n]);
  //cout<<"channel :"<<channel<<",strip:"<<strip[n]<<", wafer:"<<wafer[n]<<", Adc:"<<data[n]<<endl;
  AdcStrip[m_channel]->Fill((strip[n]+wafer[n]*nSstStripsPerWafer),(data[n]+375)%1024);
}

//greb ladder info and p-side or n-side 
count = count + 3;
if(strip[0]>nSstStripsPerWafer||strip[1]>nSstStripsPerWafer||strip[2]>nSstStripsPerWafer)
    {
      cout<<"Strip number is larger than 768, ERROR"<<endl;
      return 0;
    }
}
 return 1;
}


//-------------------------------------------------
void DecodeCompressedWords(UInt_t val[],int vallength,int channel)
{
  int strip_number,id_side,id_wafer,count,my_channel=-9999;
  int ladderCountN[20]={0};
  int ladderCountP[20]={0};
  int data,pedestal,wafer,noise,strip,ladder,laddercard;

  count = 1;
 //----
 for(int i=0;i<vallength;i++)
 	{
 wafer = Mid(HYBRID_START,HYBRID_END,val[i]);
 strip = Mid(STRIP_START,STRIP_END,val[i]);
 data  = Mid(ADC_START,ADC_END,val[i]);
 //greb ladder info and p-side or n-side 
 
occupancy_wafer->Fill(wafer,strip,data);
 laddercard = RDO2LC[m_RDO][channel];//convert RDO and channel to LadderCards No
 for(int i=0;i<20;i++)
 {
 for(int j=0;j<2;j++)
	 {
	 if(laddercard==LADDER2SIDE[i][j]) 
		 {
		 ladder = i;//geographic address
		 id_side = j;//p-side or n-side
		 }
	 }
 }
 //coding wafer number and strip number
 if(id_side == 0)
 {
	 id_wafer=7000+100*(nSstWaferPerLadder-wafer)+ladder+1;
 }
 else
 {
	 id_wafer=7000+100*((wafer)+1)+ladder+1;
 }
//coding strip_number ,just keep the same algoithm with old ssd code 
strip_number = strip + 1;//strip--[0-767],but strip_number--[1,768]
 
 if(id_side ==0) ladderCountP[ladder]++;
 else            ladderCountN[ladder]++;
 count = count + 1;
 	}
}
//-------------------------------------------------
UInt_t Mid(Int_t start,Int_t end, UInt_t input)
{
//this function used to grab input bit words between start to end. 
  UInt_t buf;
  buf=input<<(32-end);//erasure the high bit words. 
  buf=buf>>(32-end);//grab low bit words
  return buf>>start;
}

//--------------------------------------------------
void FindLadderSide(int RDO,int channel,int &ladder,int &side)
{
	int laddercard = RDO2LC[RDO][channel];//convert RDO and channel to LadderCards No
	for(int i=0;i<20;i++)
	{
	for(int j=0;j<2;j++)
		{
		if(laddercard==LADDER2SIDE[i][j]) 
			{
			ladder = i;//geographic address
			side = j;//p-side or n-side
			}
		}
	}

}
//--------------------------------------------------
void shift(FILE* filename,int length)
{
//this function used to shift Filepointer a constont value position.
char temp[200];
for(int i=0;i<length;i++)
  {
   fgets(&temp[0],120,filename);
  }
}
//--------------------------------------------------
void reset(FILE* val)
{
//set the file pointer to 0
  rewind(val);
}
//--------------------------------------------------
