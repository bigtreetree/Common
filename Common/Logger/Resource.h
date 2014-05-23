#ifndef RESOURCE_H
#define RESOURCE_H
#include <functional>
using std::function;

//TResource 是用来对资源进行管理的
template<typename T>
class TResource
{
private:
	typedef std::function<void (T*)> fun;
public:
	explicit TResource(fun construct, fun destroy, T*):m_resource(NULL),m_destroy(destroy)
	{
		construct(m_resource);//对资源进行初始化
	}

	~TResource()
	{
		m_destroy(m_resource);//释放资源 
	}
private:
	T* m_resource;
	fun  m_destroy;
};

#endif