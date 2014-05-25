#include "CriticalSection.h"

TCriticalSection::TCriticalSection()
{
	InitializeCriticalSection(&m_critical);
	
}

TCriticalSection::TCriticalSection(DWORD num)
{
	InitializeCriticalSectionAndSpinCount(&m_critical, num);
}

TCriticalSection::~TCriticalSection()
{
	DeleteCriticalSection(&m_critical);
}


void TCriticalSection::Lock()
{
	EnterCriticalSection(&m_critical);
}


void TCriticalSection::UnLock()
{
	LeaveCriticalSection(&m_critical);
}
