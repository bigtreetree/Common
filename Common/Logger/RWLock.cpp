#include "RWLock.h"

TRWLock::TRWLock()
{
	InitializeSRWLock(&m_rw_lock);
}

TRWLock::~TRWLock()
{
	
}

void TRWLock::AcquireRWLockExclusive()
{
	AcquireSRWLockExclusive(&m_rw_lock);//������ɾ����д���ĺ�����������Ҫ�Զ�д������ɾ��
}

void TRWLock::ReleaseRWLockExclusive()
{
	ReleaseSRWLockExclusive(&m_rw_lock);
}

void TRWLock::AcquireRWLockShared()
{
	AcquireSRWLockExclusive(&m_rw_lock);
}

void TRWLock::ReleaseRWLockShared()
{
	ReleaseSRWLockShared(&m_rw_lock);
}