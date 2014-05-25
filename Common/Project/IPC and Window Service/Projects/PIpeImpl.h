#ifndef PIPEIMPL_H
#define PIPEIMPL_H
#include <queue>
#include <Windows.h>
#include <string>
#include <iostream>
#include <memory>
#include "Event.h"
using namespace std;



namespace IPC
{
	const int BUFSIZE = 4096;
	class PipeImpl
	{
	public:
		PipeImpl(string pipe, HANDLE hand);
		string GetPipeName();
		void SendMessage(unsigned short int type, string msg);
		bool IsConnected();
		void RunAsServer();
		void RunAsClient();
		void CloseIPC();
		~PipeImpl();

	private:
		static void _cdecl ServerThread(void* ptr);
		static void _cdecl ClientThread(void* ptr);

		static void _cdecl SendThread(void* ptr);
		static void _cdecl ReceiveThread(void* ptr);

	private:
		string m_PipeName;   //管道名称
		bool   m_bConnect;   //连接状态

		typedef std::tr1::shared_ptr<char> CharPtr;
		queue<CharPtr> m_MesQueue;

		HANDLE m_PipeHandle_write;//管道句柄
		HANDLE m_PipeHandle_read;

		HANDLE m_window_handle;//图形界面句柄

		//作为服务器还是客户端，服务器，即管道创建者


		CRITICAL_SECTION  m_Critical;//一会考虑把队列封装一下




		HANDLE m_queue_not_empty;
		HANDLE m_close_thread_event;

	};
	typedef std::tr1::shared_ptr<PipeImpl> ImplSharePtr;


	struct Header
	{
		unsigned short int length;
		unsigned short int type;
	};
}
#endif