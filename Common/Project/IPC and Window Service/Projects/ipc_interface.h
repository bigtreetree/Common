#ifndef IIPCSERVICE_H
#define IIPCSERVICE_H
#include <string>
#include <Windows.h>
#include <boost/smart_ptr.hpp>
using std::string;

namespace IPC
{
	class IIPCService
	{
	public:
		virtual bool AddListen(string, HANDLE ) = 0;
		virtual bool Connect(string, HANDLE) = 0;

		virtual void SendMessageTo(string, unsigned short int type, string ) = 0;
		virtual bool ConnectedState(string) = 0;
		virtual void CloseIpc(string ) = 0;
		virtual void CloseAll() = 0;
	protected:
		virtual ~IIPCService(){};
	};

	typedef boost::shared_ptr<IIPCService> ipc_service_ptr_type;

}
using namespace IPC;

#endif