//---------------------------------------------------------------------------

#ifndef MutexImplCSH
#define MutexImplCSH
#include <Windows.h>
//---------------------------------------------------------------------------
namespace IPC
{
class  MutexImpl
{
protected:
	MutexImpl();
	~MutexImpl();
	void lockImpl();
	bool tryLockImpl();
	bool tryLockImpl(long milliseconds);
	void unlockImpl();

private:
	CRITICAL_SECTION _cs;
};

typedef MutexImpl FastMutexImpl;

}

#endif
