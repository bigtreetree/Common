//---------------------------------------------------------------------------

#ifndef ObjectH
#define ObjectH
#include <string>
#include <vector>
using std::string;
//---------------------------------------------------------------------------
class Test
{
	public:
	Test(int , double, char ,string, string );
	Test();
	int a;
	double f;
	char sex;
	string m_name;
	string m_address;
	char ch[10];
	char ch2[10];
};

#endif
