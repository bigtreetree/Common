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
	AcquireSRWLockExclusive(&m_rw_lock);//不存在删除读写锁的函数，即不需要对读写锁进行删除
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