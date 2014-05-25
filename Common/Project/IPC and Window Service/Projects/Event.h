//---------------------------------------------------------------------------

#ifndef EventH
#define EventH
#include "EventImpl.h"
//---------------------------------------------------------------------------
namespace IPC
{
class Event: public EventImpl
{
public:
	Event(bool autoReset = false);
	~Event();

	void set();
	void wait();
	//µÈ´ýmilliseconds¾Í·µ»Ø
	void wait(long milliseconds);
	bool tryWait(long milliseconds);

	void reset();
	const HANDLE GetHandle() const;
private:
	Event(const Event&);
	Event& operator = (const Event&);
};


}

#endif
