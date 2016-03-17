#ifndef _COLORH_
#define _COLORH_
#include "TString.h"
using namespace std;

TString FontRed      = "\033[31m";
TString FontGreen    = "\033[32m";
TString FontYellow   = "\033[33m";
TString FontBlue     = "\033[34m";
TString FontPink     = "\033[35m";
TString FontCyan     = "\033[36m";

TString FontBGRed    = "\033[41m";
TString FontBGGreen  = "\033[42m";
TString FontBGYellow = "\033[43m";
TString FontBGBlue   = "\033[44m";
TString FontBGPink   = "\033[45m";
TString FontBGCyan   = "\033[46m";

TString FontEnd      = "\033[0m";

TString FontBold     = "\033[1m";
TString FontUnderScore="\033[4m";
TString FontBlink    = "\033[5m";
TString FontInvert   = "\033[7m";
TString FontBlank    = "\033[8m";

TString PointerHide	 = "\033[?25l";
TString PointerShow	 = "\033[?25h";
#endif
