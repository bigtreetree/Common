 //---------------------------------------------------------------------------

#ifndef MutexH
#define MutexH
//#include "MutexImpl.h"
#include "MutexImplCS.h"
#include "ScopedLock.h"
//---------------------------------------------------------------------------

namespace IPC
{
class Mutex: public MutexImpl
{
public:
	Mutex();
	~Mutex();

	void lock();
	void lock(long milliseconds);
	bool tryLock();
	bool tryLock(long milliseconds);
	void unlock();
private:
	Mutex(const Mutex&);
	Mutex& operator = (const Mutex&);

};


class  FastMutex: private FastMutexImpl
	/// A FastMutex (mutual exclusion) is similar to a Mutex.
	/// Unlike a Mutex, however, a FastMutex is not recursive,
	/// which means that a deadlock will occur if the same
	/// thread tries to lock a mutex it has already locked again.
	/// Locking a FastMutex is faster than locking a recursive Mutex.
	/// Using the ScopedLock class is the preferred way to automatically
	/// lock and unlock a mutex.
{
public:
	typedef ScopedLock<FastMutex> ScopedLock;

	FastMutex();
		/// creates the Mutex.

	~FastMutex();
		/// destroys the Mutex.

	void lock();
		/// Locks the mutex. Blocks if the mutex
		/// is held by another thread.

	void lock(long milliseconds);
		/// Locks the mutex. Blocks up to the given number of milliseconds
		/// if the mutex is held by another thread. Throws a TimeoutException
		/// if the mutex can not be locked within the given timeout.
		///
		/// Performance Note: On most platforms (including Windows), this member function is
		/// implemented using a loop calling (the equivalent of) tryLock() and Thread::sleep().
		/// On POSIX platforms that support pthread_mutex_timedlock(), this is used.

	bool tryLock();
		/// Tries to lock the mutex. Returns false immediately
		/// if the mutex is already held by another thread.
		/// Returns true if the mutex was successfully locked.

	bool tryLock(long milliseconds);
		/// Locks the mutex. Blocks up to the given number of milliseconds
		/// if the mutex is held by another thread.
		/// Returns true if the mutex was successfully locked.
		///
		/// Performance Note: On most platforms (including Windows), this member function is
		/// implemented using a loop calling (the equivalent of) tryLock() and Thread::sleep().
		/// On POSIX platforms that support pthread_mutex_timedlock(), this is used.

	void unlock();
		/// Unlocks the mutex so that it can be acquired by
		/// other threads.

private:
	FastMutex(const FastMutex&);
	FastMutex& operator = (const FastMutex&);
};

}
#endif
