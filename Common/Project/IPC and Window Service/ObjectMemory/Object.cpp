//---------------------------------------------------------------------------


#pragma hdrstop

#include "Object.h"

//---------------------------------------------------------------------------
Test::Test(int i , double d, char c,string s1, string s2)
{
	a = i;
	f = d;
	sex = c;
	m_name = s1;
	m_address = s2;
}

Test::Test()
{
}
#pragma package(smart_init)
