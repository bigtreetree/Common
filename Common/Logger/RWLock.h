#ifndef RWLOCK_H
#define RWLOCK_H
#include <windows.h>
#include "Resource.h"
class TRWLock
{
public:
	typedef TResource<TRWLock> TRWLockResource;//对资源进行管理的,保证资源一定释放 
public:
	TRWLock();
	~TRWLock();
public:
	void AcquireRWLockExclusive();//以独占的方式占用读写锁
	void ReleaseRWLockExclusive();//以独占的方式释放锁

	void AcquireRWLockShared();//以共享的方式占用锁

	void ReleaseRWLockShared();//以共享的方式释放锁

	SRWLOCK* GetRWLock() const//获取对象内部的读写锁的句柄 
	{
		return (SRWLOCK*)&m_rw_lock;
	}
private:
	//禁止复制和赋值
	TRWLock(const TRWLock& rhc);
	TRWLock& operator = (const TRWLock& rhc);

private:
	SRWLOCK m_rw_lock;
};

#endif