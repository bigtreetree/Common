//---------------------------------------------------------------------------
#include "service.h"
#include <stdio.h>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
#define LOGFILE "C:\\MyServices\\memstatus.txt"
int WriteToLog(const char* str)
{
	FILE* log;
	log = fopen(LOGFILE, "a+");
	if(log == NULL)
		return -1;
	fprintf(log, "%s\n",str);
	fclose(log);
	return 0;
}
TService1 *Service1;
//---------------------------------------------------------------------------
__fastcall TService1::TService1(TComponent* Owner)
	: TService(Owner)
{
}

TServiceController __fastcall TService1::GetServiceController(void)
{
	return (TServiceController) ServiceController;
}

void __stdcall ServiceController(unsigned CtrlCode)
{
	Service1->Controller(CtrlCode);
}
//---------------------------------------------------------------------------
void __fastcall TService1::ServiceExecute(TService *Sender)
{
	WriteToLog("test");

}
//---------------------------------------------------------------------------
