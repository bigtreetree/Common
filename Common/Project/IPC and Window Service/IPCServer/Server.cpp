//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Server.h"
#include "IPCManager.h"
#include "ipc_interface.h"
using namespace IPC;
int i =0;
String str;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
	IpcManager::GetInstance()->AddListen("Address", this->Handle);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::btn1Click(TObject *Sender)
{
	
	String str = edt1->Text;

	IpcManager::GetInstance()->SendMessageTo("Address", 1, AnsiString(str.c_str()).c_str());
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TForm1::WndProc(TMessage& message)
{
	switch(message.Msg)
	{
		case WM_IPC:
			lbl1->Caption = String((char*)message.WParam);

			i++;
			 str = IntToStr(i);
			lbl2->Caption = str;
			break;
		default:
			TForm::WndProc(message);
			break;
    }
}
void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
	//IpcManager::GetInstance()->CloseAll();
	delete IpcManager::GetInstance();
}
//---------------------------------------------------------------------------

