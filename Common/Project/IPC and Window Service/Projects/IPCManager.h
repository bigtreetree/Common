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
		//��Ϊ����������
		  bool AddListen(string PipeName, HANDLE );
		//��Ϊ�ͻ�������
		  bool Connect(string PipeName, HANDLE);
		//����һ�˷�����Ϣ
		  void SendMessageTo(string PipeName,unsigned short int type, string Msg);
		  //�ж�����״̬ 
		  bool ConnectedState(string pipe_name);
		  //�Ͽ�ĳ������
		  void CloseIpc(string pipe_name);

		  void CloseAll();
	private:
		typedef std::map<string,PipeSharePtr> DefinePipeMap;
		DefinePipeMap m_PipeMap;
		static IpcManager* ipc_manager;
	};
	
}

#endif