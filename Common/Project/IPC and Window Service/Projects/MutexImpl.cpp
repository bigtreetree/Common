//---------------------------------------------------------------------------


#pragma hdrstop

#include "MutexImpl.h"


//---------------------------------------------------------------------------
namespace IPC
{


MutexImpl::MutexImpl()
{
	_mutex = CreateMutex(NULL, FALSE, NULL);
}
MutexImpl::~MutexImpl()
{
   CloseHandle(_mutex);
}
void MutexImpl::lockImpl()
{
    switch (WaitForSingleObject(_mutex, INFINITE))
	{
	case WAIT_OBJECT_0:
		return;
	default:
		return;
	}
}

bool MutexImpl::tryLockImpl()
{
	switch (WaitForSingleObject(_mutex, 0))
	{
	case WAIT_TIMEOUT:
		return false;
	case WAIT_OBJECT_0:
		return true;
	default:
		return false;
	}
}


bool MutexImpl::tryLockImpl(long milliseconds)
{
	switch (WaitForSingleObject(_mutex, milliseconds + 1))
	{
	case WAIT_TIMEOUT:
		return false;
	case WAIT_OBJECT_0:
		return true;
	default:
		return false;
	}
}

void MutexImpl::unlockImpl()
{
	ReleaseMutex(_mutex);
}



}

#pragma package(smart_init)
