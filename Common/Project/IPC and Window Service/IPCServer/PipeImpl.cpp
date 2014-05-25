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
	m_queue_not_empty = CreateEvent(NULL, false,true,NULL);//队列不为空事件，可以使用条件变量代替事件

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


//作为服务器端运行，创建服务端线程
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
		sprintf(buf,"创建服务线程失败!%d\n",GetLastError());
		OutputDebugString(buf);
		cout<<"创建服务线程失败"<<endl;
		#endif
		return;
	}
	#ifdef _DEBUG
	OutputDebugString("创建服务线程成功!");
	#endif
}

//作为客户端运行，开启客户端线程
void PipeImpl::RunAsClient()
{
	cout<<"RunAsClient"<<endl;
	//创建客户端事件,客户写数据时，把该事件置为有信号 状态
	//m_EventHandle = CreateEvent(NULL,false,false,("client"+m_PipeName).c_str());
	HANDLE client_thread = 0;
	client_thread = (HANDLE)_beginthread(&PipeImpl::ClientThread,0,this);
	Sleep(1000);
}


//服务线程，负责创建两条管道，等待客户端连接，客户端连接
//成功之后，创建发送线程和接收线程，等待两个线程结束
//两个线程结束之后，重新建立管道，进入等待客户端连接状态
void _cdecl PipeImpl::ServerThread(void* ptr)
{
	PipeImpl* impl = (PipeImpl*)ptr;

	HANDLE thread_handle[2] = {0};
	
	while(true)
	{
		//创建管道 ，等待客户端的连接
		ResetEvent(impl->m_close_thread_event);
		string PipeNameWrite = "\\\\.\\pipe\\" + impl->m_PipeName + "FIRST";//管道1名称

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
			OutputDebugString("创建管道失败");
			std::cout<<"创建管道失败"<<endl;
			#endif // DEBUG
			return;
		}
		//对句柄进行资源管理，以保证最后能正常的CloseHandle掉
		ScopedEvent<HANDLE> write_connected_pipe_handle(pipe_handle_write);

		impl->m_PipeHandle_write = pipe_handle_write;


        //创建第二条管道
		string PipeNameRead = "\\\\.\\pipe\\" + impl->m_PipeName + "Second";//管道2名称
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
			OutputDebugString("创建管道失败");
			std::cout<<"创建管道失败"<<endl;
			#endif // DEBUG
			return;
		}
        //把创建好的管道句柄赋值给成员变量
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

		//等待对方连接
		ConnectNamedPipe(pipe_handle_write,&olps[0]);
		ConnectNamedPipe(pipe_handle_read, &olps[1]);

		switch(WaitForMultipleObjects(2,hWriteEvents,false,INFINITE))
		{
			case WAIT_OBJECT_0:
				{
					return;//关闭线程事件发生
				}
			case WAIT_OBJECT_0+1://一条管道连接成功
				{
					 int a = 0;
					 a++;
					 break;
				}
			default://出问题
				{
					int b = 0;
					b++;
					return;
				}
		}

		//连接第二条管道

		switch(WaitForMultipleObjects(2,hReadEvents,false,INFINITE))
		{
			case WAIT_OBJECT_0:
				{
					return;//关闭线程事件发生
				}
			case WAIT_OBJECT_0+1://一条管道连接成功
				{
					break;
				}
			default://出问题
				{
					return;
				}
		}


		//连接成功

		EnterCriticalSection(&impl->m_Critical);
		impl->m_MesQueue.clear();
		LeaveCriticalSection(&impl->m_Critical);
		//置为连接状态
		impl->m_bConnect = true;
		#ifdef _DEBUG
		OutputDebugString("客户端连接到服务器上\n");
		std::cout<<"有客户端连接"<<endl;
		#endif


		//创建发送接收线程
		thread_handle[0] = (HANDLE)_beginthread(&PipeImpl::SendThread,0,impl);
		thread_handle[1] = (HANDLE)_beginthread(&PipeImpl::ReceiveThread,0, impl);
		if(thread_handle[0] == NULL  || thread_handle[1] == NULL)
		{
			#ifdef _DEBUG
			OutputDebugString("创建线程失败\n");
			cout<<"创建线程失败"<<endl;
			#endif
			return;
		}

		//等待两个线程结束
		WaitForMultipleObjects(2, thread_handle,true, INFINITE);
		#ifdef _DEBUG
		OutputDebugString("工作线程结束\n");
		#endif
		thread_handle[0] = NULL;
		thread_handle[1] = NULL;
		impl->m_bConnect = false;
	//这里考虑如何服务端不再开启下一次循环
	}
}
void _cdecl PipeImpl::ClientThread(void* ptr)
{
	PipeImpl* impl = (PipeImpl*)ptr;
	HANDLE thread_handle[2] = {0};
	//连接到对方的管道上
	string Pipename_read = "\\\\.\\pipe\\" + impl->m_PipeName + "FIRST";//客户端创建的管道名称
	string Pipename_write = "\\\\.\\pipe\\" + impl->m_PipeName + "SECOND";

	if(!WaitNamedPipe(Pipename_read.c_str(),NMPWAIT_WAIT_FOREVER))
	{
		#ifdef _DEBUG
		OutputDebugString("连接到对方失败\n");
		cout<<"连接到对方失败"<<endl;
		#endif
		return;

	}
	impl->m_PipeHandle_read  = NULL;
	impl->m_PipeHandle_read = CreateFile(Pipename_read.c_str(),GENERIC_READ | GENERIC_WRITE ,
			0,NULL,OPEN_EXISTING,FILE_FLAG_OVERLAPPED,NULL);


	if(!WaitNamedPipe(Pipename_write.c_str(),NMPWAIT_WAIT_FOREVER))
	{
		#ifdef _DEBUG
		OutputDebugString("连接到对方失败\n");
		cout<<"连接到对方失败"<<endl;
		#endif
		return;
	}
	impl->m_PipeHandle_write  = NULL;
	impl->m_PipeHandle_write = CreateFile(Pipename_write.c_str(),GENERIC_WRITE | GENERIC_READ  ,
			0,NULL,OPEN_EXISTING,FILE_FLAG_OVERLAPPED,NULL);
	//ScopedEvent<HANDLE> write_connect_pipe_hand(impl->m_PipeHandle_write);//管理写管道
	//创建接收线程
	if(INVALID_HANDLE_VALUE == impl->m_PipeHandle_write || INVALID_HANDLE_VALUE == impl->m_PipeHandle_read )
	{
		#ifdef _DEBUG
		OutputDebugString("打开对方管道失败\n");
		cout<<"打开对方管道失败"<<endl;
		#endif
		return;
	}
	else
	{
		#ifdef _DEBUG
		OutputDebugString("打开对方管道成功\n");
		cout<<"打开对方管道成功"<<endl;
		#endif
		//连接成功
		EnterCriticalSection(&impl->m_Critical);
		impl->m_MesQueue.clear();
		LeaveCriticalSection(&impl->m_Critical);
		impl->m_bConnect = true;

		//创建发送线程
		thread_handle[0] = (HANDLE)_beginthread(&PipeImpl::ReceiveThread,0,impl);
		thread_handle[1] = (HANDLE)_beginthread(&PipeImpl::SendThread,0,impl);
		if(thread_handle[0] == NULL || thread_handle[1] == NULL )
		{
			#ifdef _DEBUG
			OutputDebugString("创建线程失败\n");
			cout<<"创建线程失败"<<endl;
			#endif
			return;
		}

		WaitForMultipleObjects(2, thread_handle,true, INFINITE);
		#ifdef _DEBUG
		OutputDebugString("工作线程结束\n");
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
		//是否结束
		EnterCriticalSection(&impl->m_Critical);
		//VadeApp::WriteLog(VadeApp::TLogInfo("刚进入sendthread临界区","1"));
		if(!impl->m_MesQueue.empty())
		{
			//VadeApp::WriteLog(VadeApp::TLogInfo("sendthread有数据","1"));
			int len;
			memcpy(&len,impl->m_MesQueue.front().get(),sizeof(int));
			char * buf = new char[len];
			memcpy(buf,impl->m_MesQueue.front().get(), len);
            impl->m_MesQueue.pop_front();
			//VadeApp::WriteLog(VadeApp::TLogInfo("复制完数据准备离开sendthread临界区","1"));
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
					OutputDebugString("添加写请求失败\n");
					cout<<"添加写请求失败"<<endl;
					#endif
					delete buf;
					break;
                }
			}
			switch(WaitForMultipleObjects(2,hEvents,false,INFINITE))
			{
				case WAIT_OBJECT_0://线程结束事件发生
					{
						//int a =0;
						//a++;
						delete buf;
						return;
					}
				case WAIT_OBJECT_0+1: //写事件完成
					{
						//int b = 0;
						//b++;
						//VadeApp::WriteLog(VadeApp::TLogInfo("写事件完成","1"));
						delete buf;
						break;
					}
				case WAIT_FAILED:
					{

						int error = GetLastError();
						delete buf;
						return;
                    }
				default://其它情况，直接返回
					{
						int error = GetLastError();
						delete buf;
						return;
					}
			}

		}
		else
		{
			//VadeApp::WriteLog(VadeApp::TLogInfo("没有数据准备离开sendthread临界区","1"));
			LeaveCriticalSection(&impl->m_Critical);
			ResetEvent(impl->m_queue_not_empty);
			HANDLE hEvents[2] = {0};
			hEvents[1] = impl->m_queue_not_empty;
			hEvents[0] = impl->m_close_thread_event;
			switch(WaitForMultipleObjects(2,hEvents,false,INFINITE))
			{
				case WAIT_OBJECT_0://线程结束了
					{
						int a = 0;
						a++;
						return;
					}
				case WAIT_OBJECT_0+1://队列当中有了新的数据   什么都不做，接着执行
					{
						int b = 0;
						b++;
						break;
					}
				default://其它情况，直接返回,线程结束
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
				OutputDebugString("接收数据失败");
				cout<<"接收数据失败"<<endl;
				#endif
				impl->CloseIPC();
				break;
            }
		}
		switch(WaitForMultipleObjects(2,hEvents,false,INFINITE))
		{
			case WAIT_OBJECT_0://线程结束事件发生
				{
					int a = 0;
					a++;
					return;
				}
			case WAIT_OBJECT_0+1://读事件完成
				{
					int a = 0;
					a++;
					//VadeApp::WriteLog(VadeApp::TLogInfo("读事件完成","1"));
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
					OutputDebugString("读失败");
					cout<<"读失败"<<endl;
					#endif
					//VadeApp::WriteLog(VadeApp::TLogInfo("读失败","1"));
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
			//VadeApp::WriteLog(VadeApp::TLogInfo("读出数据，准备向界面抛消息",buf));
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
			//VadeApp::WriteLog(VadeApp::TLogInfo("刚进入sendmessage临界区","1"));
			m_MesQueue.push_back(ChPtr);

			LeaveCriticalSection(&m_Critical);
			//VadeApp::WriteLog(VadeApp::TLogInfo("准备离开sendmessage临界区","1"));
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