//---------------------------------------------------------------------------

#ifndef MutexImplH
#define MutexImplH
#include <Windows.h>
//---------------------------------------------------------------------------
namespace IPC
{

class MutexImpl
{
protected:
	MutexImpl();
	~MutexImpl();
	void lockImpl();
	bool tryLockImpl();
	bool tryLockImpl(long milliseconds);
	void unlockImpl();

private:
	HANDLE _mutex;
};


}
#endif
