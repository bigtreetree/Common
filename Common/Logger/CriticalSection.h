#ifndef CRITICALSECTION_H
#define CRITICALSECTION_H
#include <windows.h>
//�ٽ���
class TCriticalSection
{
public:
	TCriticalSection();
	TCriticalSection(DWORD num);//��ת����
	~TCriticalSection();


	void Lock();//��ȡ�ٽ���

	void UnLock();//�ͷ��ٽ���

private:
	CRITICAL_SECTION m_critical;
};

#endif