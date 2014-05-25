#include "Pipe.h"
namespace IPC
{

	Pipe::Pipe(std::string PipeName, HANDLE hand):impl(new PipeImpl(PipeName, hand))
	{

	}

	void Pipe::RunAsServer()
	{
		impl->RunAsServer();
	}

	void Pipe::RunAsClient()
	{
		impl->RunAsClient();
	}

	void Pipe::SendMess(unsigned short int type,std::string msg)
	{
		impl->SendMessageA(type, msg);
	}
	
	bool Pipe::IsConnected()
	{
		return impl->IsConnected();
	}
	Pipe::~Pipe()
	{

	}
	void Pipe::CloseIPC()
	{
		impl->CloseIPC();
	}
}