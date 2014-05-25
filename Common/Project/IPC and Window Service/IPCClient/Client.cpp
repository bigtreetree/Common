//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Client.h"
#include "IPCManager.h"
#include "ipc_interface.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm2 *Form2;
int i = 0;
String str;
//---------------------------------------------------------------------------
__fastcall TForm2::TForm2(TComponent* Owner)
	: TForm(Owner)
{
	IpcManager::GetInstance()->Connect("Address", this->Handle);
}
//---------------------------------------------------------------------------
void __fastcall TForm2::btn1Click(TObject *Sender)
{
	String str = edt1->Text;
	IpcManager::GetInstance()->SendMessageTo("Address", 1, AnsiString(str.c_str()).c_str());
}
//---------------------------------------------------------------------------
void __fastcall TForm2::WndProc(TMessage& message)
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
void __fastcall TForm2::FormClose(TObject *Sender, TCloseAction &Action)
{
	//IpcManager::GetInstance()->CloseAll();
	delete IpcManager::GetInstance();
}
//---------------------------------------------------------------------------

