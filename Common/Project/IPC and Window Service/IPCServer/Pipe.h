#ifndef NAMEPIPE_H
#define NAMEPIPE_H


#include <string>
#include <memory>

#include "PIpeImpl.h"
namespace IPC
{

class Pipe
{
public:
	Pipe(std::string PipeName, HANDLE hand);
	~Pipe();
	void RunAsServer();
	void RunAsClient();
	void SendMess(unsigned short int type,std::string mess);
	bool IsConnected();
	void CloseIPC();
private:
	ImplSharePtr impl;
};
typedef std::tr1::shared_ptr<Pipe> PipeSharePtr;

}
#endif
