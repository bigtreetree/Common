//---------------------------------------------------------------------------

#ifndef EventImplH
#define EventImplH
#include <Windows.h>
//---------------------------------------------------------------------------
namespace IPC
{
class EventImpl
{
public:
	EventImpl(bool autoReset = false);
	~EventImpl();

	void setImpl();
	void waitImpl();
	bool waitImpl(long milliseconds);

	void resetImpl();

	const HANDLE GetHandleImpl() const;
private:
	EventImpl(const EventImpl&);
	EventImpl& operator=(const EventImpl&);

	HANDLE m_event;

};



}
#endif
