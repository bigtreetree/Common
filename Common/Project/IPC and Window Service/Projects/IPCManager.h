#ifndef IPC_H
#define IPC_H

#include <map>
#include "ipc_interface.h"
#include "Pipe.h"
#include <boost/smart_ptr.hpp>


namespace IPC
{
	class IpcManager: public IIPCService
	{ 
	public:
		 ~IpcManager();
		 static  IpcManager* GetInstance();
		//做为服务器运行
		  bool AddListen(string PipeName, HANDLE );
		//做为客户端运行
		  bool Connect(string PipeName, HANDLE);
		//向另一端发送消息
		  void SendMessageTo(string PipeName,unsigned short int type, string Msg);
		  //判断连接状态 
		  bool ConnectedState(string pipe_name);
		  //断开某个连接
		  void CloseIpc(string pipe_name);

		  void CloseAll();
	private:
		typedef std::map<string,PipeSharePtr> DefinePipeMap;
		DefinePipeMap m_PipeMap;
		static IpcManager* ipc_manager;
	};
	
}

#endif