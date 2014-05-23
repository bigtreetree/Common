#ifndef CRITICALSECTION_H
#define CRITICALSECTION_H
#include <windows.h>
//临界区
class TCriticalSection
{
public:
	TCriticalSection();
	TCriticalSection(DWORD num);//旋转次数
	~TCriticalSection();


	void Lock();//获取临界区

	void UnLock();//释放临界区

private:
	CRITICALSECTION_H m_critical;
};

#endif