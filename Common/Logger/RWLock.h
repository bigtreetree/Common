#ifndef RWLOCK_H
#define RWLOCK_H
#include <windows.h>
#include "Resource.h"
class TRWLock
{
public:
	typedef TResource<TRWLock> TRWLockResource;//����Դ���й����,��֤��Դһ���ͷ� 
public:
	TRWLock();
	~TRWLock();
public:
	void AcquireRWLockExclusive();//�Զ�ռ�ķ�ʽռ�ö�д��
	void ReleaseRWLockExclusive();//�Զ�ռ�ķ�ʽ�ͷ���

	void AcquireRWLockShared();//�Թ���ķ�ʽռ����

	void ReleaseRWLockShared();//�Թ���ķ�ʽ�ͷ���

	SRWLOCK* GetRWLock() const//��ȡ�����ڲ��Ķ�д���ľ�� 
	{
		return (SRWLOCK*)&m_rw_lock;
	}
private:
	//��ֹ���ƺ͸�ֵ
	TRWLock(const TRWLock& rhc);
	TRWLock& operator = (const TRWLock& rhc);

private:
	SRWLOCK m_rw_lock;
};

#endif