//---------------------------------------------------------------------------


#pragma hdrstop

#include "Event.h"

//---------------------------------------------------------------------------
namespace IPC
{
Event::Event(bool autoReset):EventImpl(autoReset)
{
}

Event::~Event()
{
}


void Event::set()
{
	setImpl();
}


void Event::wait()
{
	waitImpl();
}


void Event::wait(long milliseconds)
{
	waitImpl(milliseconds);
}


bool Event::tryWait(long milliseconds)
{
	return waitImpl(milliseconds);
}

void Event::reset()
{
	resetImpl();
}

const HANDLE Event::GetHandle() const
{
    return GetHandleImpl();
}
}
#pragma package(smart_init)
