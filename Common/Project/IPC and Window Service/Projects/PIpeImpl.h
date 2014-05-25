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
		string m_PipeName;   //�ܵ�����
		bool   m_bConnect;   //����״̬

		typedef std::tr1::shared_ptr<char> CharPtr;
		queue<CharPtr> m_MesQueue;

		HANDLE m_PipeHandle_write;//�ܵ����
		HANDLE m_PipeHandle_read;

		HANDLE m_window_handle;//ͼ�ν�����

		//��Ϊ���������ǿͻ��ˣ������������ܵ�������


		CRITICAL_SECTION  m_Critical;//һ�ῼ�ǰѶ��з�װһ��




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