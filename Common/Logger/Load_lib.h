#ifndef LOAD_LIB_H
#define LOAD_LIB_h
#include <Windows.h>
#include <string>

class TLoadLib
{
public:
	TLoadLib();
	~TLoadLib();

	bool LoadDll(const std::string& str);//加载DLL
	bool LoadFunction();//从DLL中获取函数地址 

	typedef void(*Test)();//例子
	Test m_test;
private:
	HMODULE m_hDll;
};

#endif