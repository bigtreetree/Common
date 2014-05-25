#include "IPCManager.h"
namespace IPC
{
	IpcManager* IpcManager::ipc_manager = 0;
	IpcManager* IpcManager::GetInstance()
	{
		if(ipc_manager == NULL)
			return new IpcManager();
		else
		{
			return ipc_manager;
		}
	}

	bool IpcManager::AddListen(string PipeName ,HANDLE wnd_hand)
	{
		DefinePipeMap::iterator iter = m_PipeMap.find(PipeName);
		if(iter != m_PipeMap.end())
		{
			return false;
		}
		else
		{
			m_PipeMap[PipeName] = PipeSharePtr((new Pipe(PipeName, wnd_hand)));
			m_PipeMap[PipeName]->RunAsServer();
			return true;
		}
	}
	bool IpcManager::Connect(string PipeName, HANDLE hand)
	{
		DefinePipeMap::iterator iter = m_PipeMap.find(PipeName);
		if(iter != m_PipeMap.end())
		{
			return false;
		}
		else
		{
			m_PipeMap[PipeName] = PipeSharePtr((new Pipe(PipeName, hand)));
			m_PipeMap[PipeName]->RunAsClient();
			return true;
		}
	}

	void IpcManager::SendMessageTo(string PipeName, unsigned short int type, string msg)
	{
		DefinePipeMap::iterator iter = m_PipeMap.find(PipeName);
		if(iter != m_PipeMap.end())
		{
			m_PipeMap[PipeName]->SendMess(type, msg);
		}
		else
		{
			 return;
		}
	}

	bool IpcManager::ConnectedState(string pipe_name)
	{
		DefinePipeMap::iterator iter = m_PipeMap.find(pipe_name);
		if(iter != m_PipeMap.end())
		{
			return m_PipeMap[pipe_name]->IsConnected();
		}	
		return false;
	}
	void IpcManager::CloseIpc(string pipe_name)
	{
		DefinePipeMap::iterator iter = m_PipeMap.find(pipe_name);
		if(iter != m_PipeMap.end())
		{
			 m_PipeMap[pipe_name]->CloseIPC();
			 m_PipeMap.erase(iter);
		}	
	}

	void IpcManager::CloseAll()
	{
		DefinePipeMap::iterator iter = m_PipeMap.begin();
		for(;iter != m_PipeMap.end(); iter++)
		{
			CloseIpc(iter->first);
		}
	}

	IpcManager::~IpcManager()
	{
		//if(ipc_manager != 0)
		//{
		  //	delete ipc_manager;
		   //	ipc_manager = 0;
		//}
		CloseAll();
	}

	/*
	Ipc* _stdcall GetIPCInstance()
	{
		return Ipc::GetInstance();
	}
	void _stdcall CloseIPC()
	{
		delete Ipc::GetInstance();
	}
	*/

}