//---------------------------------------------------------------------------

#ifndef ScopedLockH
#define ScopedLockH
//---------------------------------------------------------------------------
namespace IPC
{

template <class M>
class ScopedLock
	/// A class that simplifies thread synchronization
	/// with a mutex.
	/// The constructor accepts a Mutex (and optionally
	/// a timeout value in milliseconds) and locks it.
	/// The destructor unlocks the mutex.
{
public:
	explicit ScopedLock(M& mutex): _mutex(mutex)
	{
		_mutex.lock();
	}

	ScopedLock(M& mutex, long milliseconds): _mutex(mutex)
	{
		_mutex.lock(milliseconds);
	}

	~ScopedLock()
	{
		_mutex.unlock();
	}

private:
	M& _mutex;

	ScopedLock();
	ScopedLock(const ScopedLock&);
	ScopedLock& operator = (const ScopedLock&);
};

template<class E>
class ScopedEvent
{
public:
	explicit ScopedEvent(E& event):m_event(event)
	{
	}
	~ScopedEvent()
	{
        CloseHandle(m_event);
    }
private:
	E& m_event;
	ScopedEvent();
	ScopedEvent(const ScopedEvent&);
	ScopedEvent& operator = (const ScopedEvent&);
};

}

#endif
