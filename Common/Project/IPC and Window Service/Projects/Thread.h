//---------------------------------------------------------------------------

#ifndef ThreadH
#define ThreadH
#include <string>
#include "Mutex.h"

//---------------------------------------------------------------------------
namespace IPC
{
class  Thread: private ThreadImpl
{
	Thread();
		/// Creates a thread. Call start() to start it.
	int id() const;
		/// Returns the unique thread ID of the thread.

	TID tid() const;
		/// Returns the native thread ID of the thread.

	std::string name() const;
		/// Returns the name of the thread.

	std::string getName() const;
		/// Returns the name of the thread.

	void setName(const std::string& name);
		/// Sets the name of the thread.
	void setStackSize(int size);
		/// Sets the thread's stack size in bytes.
		/// Setting the stack size to 0 will use the default stack size.
		/// Typically, the real stack size is rounded up to the nearest
		/// page size multiple.

	int getStackSize() const;
		/// Returns the thread's stack size in bytes.
		/// If the default stack size is used, 0 is returned.
	void join();
		/// Waits until the thread completes execution.
		/// If multiple threads try to join the same
		/// thread, the result is undefined.

	void join(long milliseconds);
		/// Waits for at most the given interval for the thread
		/// to complete. Throws a TimeoutException if the thread
		/// does not complete within the specified time interval.

	bool tryJoin(long milliseconds);
		/// Waits for at most the given interval for the thread
		/// to complete. Returns true if the thread has finished,
		/// false otherwise.

	bool isRunning() const;
		/// Returns true if the thread is running.

	static void sleep(long milliseconds);
		/// Suspends the current thread for the specified
		/// amount of time.
private:
	Thread(const Thread&);
	Thread& operator = (const Thread&);

	int                 _id;
	std::string         _name;
	mutable FastMutex   _mutex;

};
}
#endif
