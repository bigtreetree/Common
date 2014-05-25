#ifndef IIPCSERVICE_H
#define IIPCSERVICE_H
#include <string>
#include <Windows.h>
#include <boost/smart_ptr.hpp>


namespace IPC
{
    #define WM_IPC (WM_USER + 1231)
	class IIPCMangager
	{
	public:
		virtual bool AddListen(std::string, HANDLE ) = 0;
		virtual bool Connect(std::string, HANDLE) = 0;

		virtual void SendMessageTo(std::string, unsigned short int type, std::string ) = 0;
		virtual bool ConnectedState(std::string) = 0;
		virtual void CloseIpc(std::string ) = 0;
		virtual void CloseAll() = 0;
	protected:
		virtual ~IIPCMangager(){};
	};

	typedef boost::shared_ptr<IIPCMangager> ipc_service_ptr_type;

}
using namespace IPC;

#endif