//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ObjectTest.h"
#include "Object.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
	Test te(3, 3.5, 's', "chengshaoyuanchengshaoyuanchengshaoyuan", "address");
	te.ch[0] = 'a';
	te.ch[1] = 'b';
	te.ch[2] = 'c';
	te.ch[3] = 'd';
	te.ch2[0] = 'e';
	te.ch2[1] = 'f';
	int ClassSize = sizeof(Test);
	int ObjectSize = sizeof(te);
	int stringsize = sizeof(string);
	char * buf = new char[ObjectSize];
	memcpy(buf, &te, ObjectSize);
	Test tt;
	memcpy(&tt, &te, ObjectSize);
	string name = tt.m_name;
	string address = tt.m_address;
	tt.ch2

}
//---------------------------------------------------------------------------
