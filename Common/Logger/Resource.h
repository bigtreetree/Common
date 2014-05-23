#ifndef RESOURCE_H
#define RESOURCE_H
#include <functional>
using std::function;

//TResource ����������Դ���й����
template<typename T>
class TResource
{
private:
	typedef std::function<void (T*)> fun;
public:
	explicit TResource(fun construct, fun destroy, T*):m_resource(NULL),m_destroy(destroy)
	{
		construct(m_resource);//����Դ���г�ʼ��
	}

	~TResource()
	{
		m_destroy(m_resource);//�ͷ���Դ 
	}
private:
	T* m_resource;
	fun  m_destroy;
};

#endif