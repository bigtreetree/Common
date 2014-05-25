//---------------------------------------------------------------------------


#pragma hdrstop

#include "Mutex.h"

//---------------------------------------------------------------------------
namespace IPC
{
Mutex::Mutex()
{
}

Mutex::~Mutex()
{
}

void Mutex::lock()
{
    lockImpl();
}
void Mutex::lock(long milliseconds)
{
   tryLockImpl(milliseconds);
}

bool Mutex::tryLock()
{
	return tryLockImpl();
}

bool Mutex::tryLock(long milliseconds)
{
	return tryLockImpl(milliseconds);
}

void Mutex::unlock()
{
	unlockImpl();
}



FastMutex::FastMutex()
{
}


FastMutex::~FastMutex()
{
}

inline void FastMutex::lock()
{
	lockImpl();
}

inline void FastMutex::lock(long milliseconds)
{
	tryLockImpl(milliseconds);
}


inline bool FastMutex::tryLock()
{
	return tryLockImpl();
}


inline bool FastMutex::tryLock(long milliseconds)
{
	return tryLockImpl(milliseconds);
}


inline void FastMutex::unlock()
{
	unlockImpl();
}



}

#pragma package(smart_init)
