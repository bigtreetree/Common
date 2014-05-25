#ifndef auto_releaseH
#define auto_releaseH

namespace VadeApp
{
	// 自动释放资源
	class CAutoRelease
	{
	public: 
		template<typename T>
		CAutoRelease(const T & value) : m_pHelder(new Helder<T>(value)) {}
		~CAutoRelease() { delete m_pHelder; }

	private: 
		struct IHelder
		{
			virtual ~IHelder() {}
		};

		template<typename T>
		struct Helder : public IHelder
		{
			T held;
			Helder(const T & value) : held(value) {}
			virtual ~Helder() { held(); }
		};

		IHelder* m_pHelder;
	};
}

#endif

