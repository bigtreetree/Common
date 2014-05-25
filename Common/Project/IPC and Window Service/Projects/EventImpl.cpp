//---------------------------------------------------------------------------


#pragma hdrstop

#include "EventImpl.h"

namespace IPC
{
//---------------------------------------------------------------------------
EventImpl::EventImpl(bool autoReset )
{
	m_event = CreateEvent(NULL,autoReset?false:true, false,NULL);
}
//---------------------------------------------------------------------------
EventImpl::~EventImpl()
{
	CloseHandle(m_event);
}

void EventImpl::waitImpl()
{
	switch (WaitForSingleObject(m_event, INFINITE))
	{
	case WAIT_OBJECT_0:
		return;
	default:
		return;
	}
}

void EventImpl::setImpl()
{
	SetEvent(m_event);
}


bool EventImpl::waitImpl(long milliseconds)
{
	switch (WaitForSingleObject(m_event, milliseconds + 1))
	{
	case WAIT_TIMEOUT:
		return false;
	case WAIT_OBJECT_0:
		return true;
	default:
		return false;
	}
}

void EventImpl::resetImpl()
{
	ResetEvent(m_event);
}


const HANDLE EventImpl::GetHandleImpl() const
{
    return m_event;
}
}
#pragma package(smart_init)
