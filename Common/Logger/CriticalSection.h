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
	CRITICALSECTION_H m_critical;
};

#endif