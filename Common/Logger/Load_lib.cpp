#include "Load_lib.h"

TLoadLib::TLoadLib()
{
	m_hDll = NULL;
}

TLoadLib::~TLoadLib()
{
	if(m_hDll)
	{
		::FreeLibrary(m_hDll);
		m_hDll = NULL;
	}
}

bool TLoadLib::LoadDll(const std::string& str)
{
	bool bre = true;
	m_hDll = ::LoadLibrary(str.c_str());
	if(NULL == m_hDll)
		bre = false;
	return bre;
}

bool TLoadLib::LoadFunction()
{
	bool bre = false;
	m_test = (Test)::GetProcAddress(m_hDll, "functionName");
	if(m_test)
	{
		bre = true;
	}
	else
	{
		bre = false;
	}
	return false;
}