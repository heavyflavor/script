#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include "TString.h"
#include "TSystem.h"
using namespace std;

TString RetrunLite(int Day,int Hour,int Min,int Sec);
void showLite(int Day,int Hour,int Min,int Sec);
void showDate(TString Week,TString Month,int Day,int Hour,int Min,int Sec,TString TimeZone,int Year);
void showDateSysFormat(TString Week,TString Month,int Day,int Hour,int Min,int Sec,TString TimeZone,int Year);
bool leapYear(int Year);
int  dayDiff(int OYear,TString OMonth,int ODay,int Year,TString Month,int Day);
long long int  timeDiff(int OYear,TString OTMonth,int ODay,int OHour,int OMin,int OSec,int Year,TString TMonth,int Day,int Hour,int Min,int Sec);
int  monthTran(TString TMonth);

long long int dateDiff(TString UserName="xyf")
{
	TString Week="",		OWeek="";
	TString Month="",	OMonth="";
	TString	TimeZone="",	OTimeZone="";
	int	Day=0,	ODay=0;
	int	Hour=0,	OHour=0;
	int	Min=0,	OMin=0;
	int	Sec=0,	OSec=0;
	int	Year=0,	OYear=0;

	TString BakFile = "";
	TString DatFile = "";
	TString CMD = "";

	BakFile = Form(" jobStatus/.dateDiff.%s.bak",UserName.Data());
	DatFile = Form(" jobStatus/.dateDiff.%s.dat",UserName.Data());
	CMD = Form("rm -f %s",BakFile.Data());gSystem->Exec(CMD.Data());
	CMD = Form("date > %s",BakFile.Data());gSystem->Exec(CMD.Data());
	CMD = Form("sed -i 's/:/ /g' %s",BakFile.Data());gSystem->Exec(CMD.Data());

	ifstream ifileBak;ifileBak.open(BakFile.Data(),ios::in);if(!ifileBak){cerr<<"ifileBak.open failed!"<<endl;return 0;}
	ifileBak>>Week>>Month>>Day>>Hour>>Min>>Sec>>TimeZone>>Year;
	ifileBak.close();

	ifstream ifileDat;ifileDat.open(DatFile.Data(),ios::in);
	if(!ifileDat)
	{
		cout<<"ifileDat.open Failed! Trying to recover..."<<endl;
		CMD = Form("touch %s",DatFile.Data());gSystem->Exec(CMD.Data());
		CMD = Form("echo \"Wed Nov 20 01 01 01 EST 2013\" > %s",DatFile.Data());gSystem->Exec(CMD.Data());
		cout<<"DatFile has been created with default time."<<endl;
		ifileDat.open(DatFile.Data(),ios::in);
		if(!ifileDat){cerr<<"ifile.open Failed Again! Please check the code..."<<endl;return 0;}
	}
	ifileDat>>OWeek>>OMonth>>ODay>>OHour>>OMin>>OSec>>OTimeZone>>OYear;
	ifileDat.close();
	CMD = Form("mv %s %s",BakFile.Data(),DatFile.Data());gSystem->Exec(CMD.Data());

//	cout<<OYear<<" "<<OMonth<<" "<<ODay<<" "<<OHour<<":"<<OMin<<":"<<OSec<<endl;
//	cout<< Year<<" "<< Month<<" "<< Day<<" "<< Hour<<":"<< Min<<":"<< Sec<<endl;
//	cout<<"======================================================"<<endl;
//	showDate(OWeek,OMonth,ODay,OHour,OMin,OSec,OTimeZone,OYear);
//	showDate( Week, Month, Day, Hour, Min, Sec, TimeZone, Year);
//	showDateSysFormat(OWeek,OMonth,ODay,OHour,OMin,OSec,OTimeZone,OYear);
//	cout<<"======================================================"<<endl;
//	cout<<dayDiff(OYear,OMonth,ODay,Year,Month,Day)<<endl;
//	cout<<timeDiff(OYear,OMonth,ODay,OHour,OMin,OSec,Year,Month,Day,Hour,Min,Sec)<<endl;
	long long int NSeconds = timeDiff(OYear,OMonth,ODay,OHour,OMin,OSec,Year,Month,Day,Hour,Min,Sec);

	int NDays = NSeconds/(3600*24);
	int NHours= (NSeconds%(3600*24))/3600;
	int NMins = ((NSeconds%(3600*24))%3600)/60;
	int NSecs = (((NSeconds%(3600*24))%3600)%60);

//	showLite(NDays,NHours,NMins,NSecs);
//	cout<<RetrunLite(NDays,NHours,NMins,NSecs)<<endl;
//	return RetrunLite(NDays,NHours,NMins,NSecs);
	return NSeconds;
}

void showDate(TString Week,TString Month,int Day,int Hour,int Min,int Sec,TString TimeZone,int Year)
{
	cout<< Year<<" "<< Month<<" "<< Day<<" ";
	if(Hour<10) cout<<"0";
	cout<< Hour<<":";
	if(Min<10) cout<<"0";
	cout<< Min<<":";
	if(Sec<10) cout<<"0";
	cout<< Sec<<endl;
}
void showDateSysFormat(TString Week,TString Month,int Day,int Hour,int Min,int Sec,TString TimeZone,int Year)
{
	cout<<Week<<" "<<Month<<" "<<Day<<" ";
	if(Hour<10) cout<<"0";
	cout<< Hour<<":";
	if(Min<10) cout<<"0";
	cout<< Min<<":";
	if(Sec<10) cout<<"0";
	cout<< Sec<<" ";
	cout<<TimeZone<<" "<<Year<<endl;
}
bool leapYear(int Year)
{
	if(Year%100==0)
	{
		if(Year%400==0) return true;
		else return false;
	}
	else
	{
		if(Year%4==0) return true;
		else return false;
	}
}
//int dayDiff(int OYear=2013,int OMonth=11,int ODay=29,int Year=2013,int Month=11,int Day=29)
int dayDiff(int OYear,TString OTMonth,int ODay,int Year,TString TMonth,int Day)
{
	int OMonth = monthTran(OTMonth);
	int Month = monthTran(TMonth);
	int DayInMon[12]={31,28,31,30,31,30,31,31,30,31,30,31};
	int YearDiff = Year - OYear; 
	int NleapYear=0;
	for(int i=0;i<YearDiff;i++)
	{
		if(leapYear(OYear+i)) NleapYear++;
	}
	int Nday = YearDiff*365 + NleapYear;
//*******************
	int NdayO;
	for(int i=0;i<(OMonth-1);i++)
	{
		NdayO += DayInMon[i];
		if(i==1 && leapYear(OYear)) NdayO++;
	}
	NdayO += ODay;
	int NdayC;
	for(int i=0;i<(Month-1);i++)
	{
		NdayC += DayInMon[i];
		if(i==1 && leapYear(Year)) NdayC++;
	}
	NdayC += Day;
//*******************
	return Nday-NdayO+NdayC;
}
int  monthTran(TString TMonth)
{
	if     (TMonth.CompareTo("Jan",TString::kIgnoreCase)==0)return 1;
	else if(TMonth.CompareTo("Feb",TString::kIgnoreCase)==0)return 2;
	else if(TMonth.CompareTo("Mar",TString::kIgnoreCase)==0)return 3;
	else if(TMonth.CompareTo("Apr",TString::kIgnoreCase)==0)return 4;
	else if(TMonth.CompareTo("May",TString::kIgnoreCase)==0)return 5;
	else if(TMonth.CompareTo("Jun",TString::kIgnoreCase)==0)return 6;
	else if(TMonth.CompareTo("Jul",TString::kIgnoreCase)==0)return 7;
	else if(TMonth.CompareTo("Aug",TString::kIgnoreCase)==0)return 8;
	else if(TMonth.CompareTo("Sep",TString::kIgnoreCase)==0)return 9;
	else if(TMonth.CompareTo("Oct",TString::kIgnoreCase)==0)return 10;
	else if(TMonth.CompareTo("Nov",TString::kIgnoreCase)==0)return 11;
	else if(TMonth.CompareTo("Dec",TString::kIgnoreCase)==0)return 12;
	else return 0;
}
long long int  timeDiff(int OYear,TString OTMonth,int ODay,int OHour,int OMin,int OSec,int Year,TString TMonth,int Day,int Hour,int Min,int Sec)
{
	int OMonth = monthTran(OTMonth);
	int Month = monthTran(TMonth);
	int DayInMon[12]={31,28,31,30,31,30,31,31,30,31,30,31};
	int YearDiff = Year - OYear; 
	int NleapYear=0;
	for(int i=0;i<YearDiff;i++)
	{
		if(leapYear(OYear+i)) NleapYear++;
	}
	int Nday = YearDiff*365 + NleapYear;
//*******************
	int NdayO;
	for(int i=0;i<(OMonth-1);i++)
	{
		NdayO += DayInMon[i];
		if(i==1 && leapYear(OYear)) NdayO++;
	}
	NdayO += ODay;
	int NdayC;
	for(int i=0;i<(Month-1);i++)
	{
		NdayC += DayInMon[i];
		if(i==1 && leapYear(Year)) NdayC++;
	}
	NdayC += Day;
//*******************
	int Ndays =  Nday-NdayO+NdayC;
	long long int Nsecond = Ndays *24*3600;
	Nsecond -= OHour*3600 + OMin*60 + OSec;
	Nsecond +=  Hour*3600 +  Min*60 +  Sec;
	return Nsecond;
}
void showLite(int Day,int Hour,int Min,int Sec)
{
	cout<<Day<<"+";
	if(Hour<10) cout<<"0";
	cout<<Hour<<":";
	if(Min<10) cout<<"0";
	cout<<Min<<":";
	if(Sec<10) cout<<"0";
	cout<<Sec<<endl;
}
TString RetrunLite(int Day,int Hour,int Min,int Sec)
{
	TString RETRUN="";
				RETRUN  = Form("%d+",Day);
	if(Hour<10) RETRUN += Form("0");
				RETRUN += Form("%d:",Hour);
	if(Min<10)	RETRUN += Form("0");
				RETRUN += Form("%d:",Min);
	if(Sec<10)	RETRUN += Form("0");
				RETRUN += Form("%d",Sec);

	return RETRUN;
}
