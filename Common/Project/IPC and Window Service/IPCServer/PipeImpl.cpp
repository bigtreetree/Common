#include "PIpeImpl.h"
#include <Windows.h>
#include <process.h>
#include <sstream>
#include "ScopedLock.h"
#include "ipc_interface.h"
//#include "log_info.h"
namespace IPC
{


PipeImpl::PipeImpl(string name, HANDLE hand):m_PipeName(name),m_window_handle(hand)
{


	m_bConnect = false;
	InitializeCriticalSection(&m_Critical);
	m_close_thread_event = CreateEvent(NULL,false, false,NULL);
	m_queue_not_empty = CreateEvent(NULL, false,true,NULL);//���в�Ϊ���¼�������ʹ���������������¼�

}
PipeImpl::~PipeImpl()
{
	DeleteCriticalSection(&m_Critical);
	CloseHandle(m_close_thread_event);
}

string PipeImpl::GetPipeName()
{
	return m_PipeName;
}


//��Ϊ�����������У�����������߳�
void PipeImpl::RunAsServer()
{
	#ifdef _DEBUG
	cout<<"RunAsServer"<<endl;
	OutputDebugString("RunAsServer");
	#endif

	HANDLE server_thread = NULL;
	server_thread = (HANDLE)_beginthread(&PipeImpl::ServerThread,0,this);
	if(NULL == server_thread)
	{
		#ifdef _DEBUG
		char buf[256] = {0};
		sprintf(buf,"���������߳�ʧ��!%d\n",GetLastError());
		OutputDebugString(buf);
		cout<<"���������߳�ʧ��"<<endl;
		#endif
		return;
	}
	#ifdef _DEBUG
	OutputDebugString("���������̳߳ɹ�!");
	#endif
}

//��Ϊ�ͻ������У������ͻ����߳�
void PipeImpl::RunAsClient()
{
	cout<<"RunAsClient"<<endl;
	//�����ͻ����¼�,�ͻ�д����ʱ���Ѹ��¼���Ϊ���ź� ״̬
	//m_EventHandle = CreateEvent(NULL,false,false,("client"+m_PipeName).c_str());
	HANDLE client_thread = 0;
	client_thread = (HANDLE)_beginthread(&PipeImpl::ClientThread,0,this);
	Sleep(1000);
}


//�����̣߳����𴴽������ܵ����ȴ��ͻ������ӣ��ͻ�������
//�ɹ�֮�󣬴��������̺߳ͽ����̣߳��ȴ������߳̽���
//�����߳̽���֮�����½����ܵ�������ȴ��ͻ�������״̬
void _cdecl PipeImpl::ServerThread(void* ptr)
{
	PipeImpl* impl = (PipeImpl*)ptr;

	HANDLE thread_handle[2] = {0};
	
	while(true)
	{
		//�����ܵ� ���ȴ��ͻ��˵�����
		ResetEvent(impl->m_close_thread_event);
		string PipeNameWrite = "\\\\.\\pipe\\" + impl->m_PipeName + "FIRST";//�ܵ�1����

		HANDLE pipe_handle_write = CreateNamedPipe(PipeNameWrite.c_str(),
			 PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,
			PIPE_TYPE_MESSAGE | PIPE_READMODE_BYTE ,
			1,
			BUFSIZE,
			BUFSIZE,
			0,
			NULL);
		if(pipe_handle_write == NULL)
		{
			#ifdef _DEBUG
			OutputDebugString("�����ܵ�ʧ��");
			std::cout<<"�����ܵ�ʧ��"<<endl;
			#endif // DEBUG
			return;
		}
		//�Ծ��������Դ�����Ա�֤�����������CloseHandle��
		ScopedEvent<HANDLE> write_connected_pipe_handle(pipe_handle_write);

		impl->m_PipeHandle_write = pipe_handle_write;


        //�����ڶ����ܵ�
		string PipeNameRead = "\\\\.\\pipe\\" + impl->m_PipeName + "Second";//�ܵ�2����
		HANDLE pipe_handle_read = CreateNamedPipe(PipeNameRead.c_str(),
			PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,
			PIPE_TYPE_MESSAGE | PIPE_READMODE_BYTE ,
			1,
			BUFSIZE,
			BUFSIZE,
			0,
			NULL);
		if(pipe_handle_read== NULL )
		{
			#ifdef _DEBUG
			OutputDebugString("�����ܵ�ʧ��");
			std::cout<<"�����ܵ�ʧ��"<<endl;
			#endif // DEBUG
			return;
		}
        //�Ѵ����õĹܵ������ֵ����Ա����
		ScopedEvent<HANDLE> read_connected_pipe_handle(pipe_handle_read);
		impl->m_PipeHandle_read = pipe_handle_read;


		HANDLE hWriteEvents[2] = {0};
		HANDLE hReadEvents[2] = {0};
		HANDLE hWriteConnectEvent = CreateEvent(NULL,false,false,NULL);
		ScopedEvent<HANDLE> WriteScopeEvent(hWriteConnectEvent);
		HANDLE hReadConnectEvent = CreateEvent(NULL,false,false,NULL);
		ScopedEvent<HANDLE> ReadScopeEvent(hReadConnectEvent);
		hWriteEvents[0]= impl->m_close_thread_event;
		hWriteEvents[1] = hWriteConnectEvent;
		hReadEvents[0] = impl->m_close_thread_event;
		hReadEvents[1] = hReadConnectEvent;

		OVERLAPPED olps[2] = {0};
		olps[0].hEvent =  hWriteConnectEvent;
		olps[1].hEvent = hReadConnectEvent;

		//�ȴ��Է�����
		ConnectNamedPipe(pipe_handle_write,&olps[0]);
		ConnectNamedPipe(pipe_handle_read, &olps[1]);

		switch(WaitForMultipleObjects(2,hWriteEvents,false,INFINITE))
		{
			case WAIT_OBJECT_0:
				{
					return;//�ر��߳��¼�����
				}
			case WAIT_OBJECT_0+1://һ���ܵ����ӳɹ�
				{
					 int a = 0;
					 a++;
					 break;
				}
			default://������
				{
					int b = 0;
					b++;
					return;
				}
		}

		//���ӵڶ����ܵ�

		switch(WaitForMultipleObjects(2,hReadEvents,false,INFINITE))
		{
			case WAIT_OBJECT_0:
				{
					return;//�ر��߳��¼�����
				}
			case WAIT_OBJECT_0+1://һ���ܵ����ӳɹ�
				{
					break;
				}
			default://������
				{
					return;
				}
		}


		//���ӳɹ�

		EnterCriticalSection(&impl->m_Critical);
		impl->m_MesQueue.clear();
		LeaveCriticalSection(&impl->m_Critical);
		//��Ϊ����״̬
		impl->m_bConnect = true;
		#ifdef _DEBUG
		OutputDebugString("�ͻ������ӵ���������\n");
		std::cout<<"�пͻ�������"<<endl;
		#endif


		//�������ͽ����߳�
		thread_handle[0] = (HANDLE)_beginthread(&PipeImpl::SendThread,0,impl);
		thread_handle[1] = (HANDLE)_beginthread(&PipeImpl::ReceiveThread,0, impl);
		if(thread_handle[0] == NULL  || thread_handle[1] == NULL)
		{
			#ifdef _DEBUG
			OutputDebugString("�����߳�ʧ��\n");
			cout<<"�����߳�ʧ��"<<endl;
			#endif
			return;
		}

		//�ȴ������߳̽���
		WaitForMultipleObjects(2, thread_handle,true, INFINITE);
		#ifdef _DEBUG
		OutputDebugString("�����߳̽���\n");
		#endif
		thread_handle[0] = NULL;
		thread_handle[1] = NULL;
		impl->m_bConnect = false;
	//���￼����η���˲��ٿ�����һ��ѭ��
	}
}
void _cdecl PipeImpl::ClientThread(void* ptr)
{
	PipeImpl* impl = (PipeImpl*)ptr;
	HANDLE thread_handle[2] = {0};
	//���ӵ��Է��Ĺܵ���
	string Pipename_read = "\\\\.\\pipe\\" + impl->m_PipeName + "FIRST";//�ͻ��˴����Ĺܵ�����
	string Pipename_write = "\\\\.\\pipe\\" + impl->m_PipeName + "SECOND";

	if(!WaitNamedPipe(Pipename_read.c_str(),NMPWAIT_WAIT_FOREVER))
	{
		#ifdef _DEBUG
		OutputDebugString("���ӵ��Է�ʧ��\n");
		cout<<"���ӵ��Է�ʧ��"<<endl;
		#endif
		return;

	}
	impl->m_PipeHandle_read  = NULL;
	impl->m_PipeHandle_read = CreateFile(Pipename_read.c_str(),GENERIC_READ | GENERIC_WRITE ,
			0,NULL,OPEN_EXISTING,FILE_FLAG_OVERLAPPED,NULL);


	if(!WaitNamedPipe(Pipename_write.c_str(),NMPWAIT_WAIT_FOREVER))
	{
		#ifdef _DEBUG
		OutputDebugString("���ӵ��Է�ʧ��\n");
		cout<<"���ӵ��Է�ʧ��"<<endl;
		#endif
		return;
	}
	impl->m_PipeHandle_write  = NULL;
	impl->m_PipeHandle_write = CreateFile(Pipename_write.c_str(),GENERIC_WRITE | GENERIC_READ  ,
			0,NULL,OPEN_EXISTING,FILE_FLAG_OVERLAPPED,NULL);
	//ScopedEvent<HANDLE> write_connect_pipe_hand(impl->m_PipeHandle_write);//����д�ܵ�
	//���������߳�
	if(INVALID_HANDLE_VALUE == impl->m_PipeHandle_write || INVALID_HANDLE_VALUE == impl->m_PipeHandle_read )
	{
		#ifdef _DEBUG
		OutputDebugString("�򿪶Է��ܵ�ʧ��\n");
		cout<<"�򿪶Է��ܵ�ʧ��"<<endl;
		#endif
		return;
	}
	else
	{
		#ifdef _DEBUG
		OutputDebugString("�򿪶Է��ܵ��ɹ�\n");
		cout<<"�򿪶Է��ܵ��ɹ�"<<endl;
		#endif
		//���ӳɹ�
		EnterCriticalSection(&impl->m_Critical);
		impl->m_MesQueue.clear();
		LeaveCriticalSection(&impl->m_Critical);
		impl->m_bConnect = true;

		//���������߳�
		thread_handle[0] = (HANDLE)_beginthread(&PipeImpl::ReceiveThread,0,impl);
		thread_handle[1] = (HANDLE)_beginthread(&PipeImpl::SendThread,0,impl);
		if(thread_handle[0] == NULL || thread_handle[1] == NULL )
		{
			#ifdef _DEBUG
			OutputDebugString("�����߳�ʧ��\n");
			cout<<"�����߳�ʧ��"<<endl;
			#endif
			return;
		}

		WaitForMultipleObjects(2, thread_handle,true, INFINITE);
		#ifdef _DEBUG
		OutputDebugString("�����߳̽���\n");
		#endif
		CloseHandle(impl->m_PipeHandle_read);
		CloseHandle(impl->m_PipeHandle_write);
		thread_handle[0] = NULL;
		thread_handle[1] = NULL;
		impl->m_bConnect = false;
	}

}

void _cdecl PipeImpl::SendThread(void* ptr)
{

	PipeImpl* impl = (PipeImpl*)ptr;

	while(true)
	{
		//�Ƿ����
		EnterCriticalSection(&impl->m_Critical);
		//VadeApp::WriteLog(VadeApp::TLogInfo("�ս���sendthread�ٽ���","1"));
		if(!impl->m_MesQueue.empty())
		{
			//VadeApp::WriteLog(VadeApp::TLogInfo("sendthread������","1"));
			int len;
			memcpy(&len,impl->m_MesQueue.front().get(),sizeof(int));
			char * buf = new char[len];
			memcpy(buf,impl->m_MesQueue.front().get(), len);
            impl->m_MesQueue.pop_front();
			//VadeApp::WriteLog(VadeApp::TLogInfo("����������׼���뿪sendthread�ٽ���","1"));
			LeaveCriticalSection(&impl->m_Critical);

			DWORD dwWrite;
			HANDLE send_handle =  impl->m_PipeHandle_write;
			HANDLE hEvents[2] = {0};
			hEvents[0] = impl->m_close_thread_event;
			HANDLE  write_event = CreateEvent(NULL,false,false,NULL);
			ScopedEvent<HANDLE> scope_write_event(write_event);
			hEvents[1] = write_event;
			OVERLAPPED olp = {0};
			olp.hEvent = hEvents[1];
			//char Testbuf[4] = {'a','b','c'};
			if(!WriteFile(send_handle,buf,len,&dwWrite,&olp))
			{
				if(GetLastError() != ERROR_IO_PENDING)
				{
					#ifdef _DEBUG
					OutputDebugString("���д����ʧ��\n");
					cout<<"���д����ʧ��"<<endl;
					#endif
					delete buf;
					break;
                }
			}
			switch(WaitForMultipleObjects(2,hEvents,false,INFINITE))
			{
				case WAIT_OBJECT_0://�߳̽����¼�����
					{
						//int a =0;
						//a++;
						delete buf;
						return;
					}
				case WAIT_OBJECT_0+1: //д�¼����
					{
						//int b = 0;
						//b++;
						//VadeApp::WriteLog(VadeApp::TLogInfo("д�¼����","1"));
						delete buf;
						break;
					}
				case WAIT_FAILED:
					{

						int error = GetLastError();
						delete buf;
						return;
                    }
				default://���������ֱ�ӷ���
					{
						int error = GetLastError();
						delete buf;
						return;
					}
			}

		}
		else
		{
			//VadeApp::WriteLog(VadeApp::TLogInfo("û������׼���뿪sendthread�ٽ���","1"));
			LeaveCriticalSection(&impl->m_Critical);
			ResetEvent(impl->m_queue_not_empty);
			HANDLE hEvents[2] = {0};
			hEvents[1] = impl->m_queue_not_empty;
			hEvents[0] = impl->m_close_thread_event;
			switch(WaitForMultipleObjects(2,hEvents,false,INFINITE))
			{
				case WAIT_OBJECT_0://�߳̽�����
					{
						int a = 0;
						a++;
						return;
					}
				case WAIT_OBJECT_0+1://���е��������µ�����   ʲô������������ִ��
					{
						int b = 0;
						b++;
						break;
					}
				default://���������ֱ�ӷ���,�߳̽���
					{
						int a = 0;
						a++;
						return;
					}
			}

		}
	}
}

void _cdecl PipeImpl::ReceiveThread(void* ptr)
{

	PipeImpl* impl = (PipeImpl*)ptr;
	while(true)
	{
		char recvbuf[4] = {0};
		DWORD dwWrite;
		HANDLE receive_hand = impl->m_PipeHandle_read;
		OVERLAPPED olp = {0};
		HANDLE read_finish_event = CreateEvent(NULL, false, false, NULL);
		ScopedEvent<HANDLE> scope_read_finish_event(read_finish_event);
		HANDLE hEvents[2] = {0};
		hEvents[0]= impl->m_close_thread_event;
		hEvents[1] = read_finish_event;
		olp.hEvent = hEvents[1];

		if(!ReadFile(receive_hand,recvbuf,4,&dwWrite,&olp))
		{
			if(GetLastError() != ERROR_IO_PENDING)
			{
				#ifdef _DEBUG
				OutputDebugString("��������ʧ��");
				cout<<"��������ʧ��"<<endl;
				#endif
				impl->CloseIPC();
				break;
            }
		}
		switch(WaitForMultipleObjects(2,hEvents,false,INFINITE))
		{
			case WAIT_OBJECT_0://�߳̽����¼�����
				{
					int a = 0;
					a++;
					return;
				}
			case WAIT_OBJECT_0+1://���¼����
				{
					int a = 0;
					a++;
					//VadeApp::WriteLog(VadeApp::TLogInfo("���¼����","1"));
					break;
				}
        }
		

		int* len = (int*)recvbuf;
		int length = *len;
		if(length > 4)
		{
			CharPtr Chptr(new char[length - 4]);

			if(!ReadFile(receive_hand,Chptr.get(),length - 4,&dwWrite,&olp))
			{
				if(GetLastError() != ERROR_IO_PENDING)
				{
					#ifdef _DEBUG
					OutputDebugString("��ʧ��");
					cout<<"��ʧ��"<<endl;
					#endif
					//VadeApp::WriteLog(VadeApp::TLogInfo("��ʧ��","1"));
					return;
				}
			}
			WaitForSingleObject(hEvents[1], INFINITE);
			unsigned short int ty;
			memcpy(&ty,Chptr.get(),sizeof(ty) );
			cout<<ty<<endl;
			char buf[BUFSIZE] = {0};
			memcpy(buf,Chptr.get() + sizeof(ty), length - sizeof(ty) - 4 );
			cout<<buf<<endl;
			//VadeApp::WriteLog(VadeApp::TLogInfo("�������ݣ�׼�����������Ϣ",buf));
			PostMessage((HWND)impl->m_window_handle, WM_IPC, (WPARAM)buf, (LPARAM)ty);
		}
	}
}


void PipeImpl::SendMessage(unsigned short int type, string msg)
{
	if(m_bConnect)
	{
		for(int i = 0; i < 1000; i++)
		{
			int length = sizeof(type) + msg.size() + sizeof(length);
			CharPtr ChPtr(new char[length]);
			memcpy(ChPtr.get(),&length, sizeof(length));
			memcpy(ChPtr.get()+sizeof(length), &type, sizeof(type));
			memcpy(ChPtr.get() + sizeof(type) + sizeof(length), msg.c_str(),msg.size());
			EnterCriticalSection(&m_Critical);
			//VadeApp::WriteLog(VadeApp::TLogInfo("�ս���sendmessage�ٽ���","1"));
			m_MesQueue.push_back(ChPtr);

			LeaveCriticalSection(&m_Critical);
			//VadeApp::WriteLog(VadeApp::TLogInfo("׼���뿪sendmessage�ٽ���","1"));
			SetEvent(m_queue_not_empty);
		}
	}
}
bool PipeImpl::IsConnected()
{
	return m_bConnect;
}



void PipeImpl::CloseIPC()
{
	SetEvent(m_close_thread_event);
}

}