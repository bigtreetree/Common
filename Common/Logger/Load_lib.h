#ifndef LOAD_LIB_H
#define LOAD_LIB_h
#include <Windows.h>
#include <string>

class TLoadLib
{
public:
	TLoadLib();
	~TLoadLib();

	bool LoadDll(const std::string& str);//����DLL
	bool LoadFunction();//��DLL�л�ȡ������ַ 

	typedef void(*Test)();//����
	Test m_test;
private:
	HMODULE m_hDll;
};

#endif