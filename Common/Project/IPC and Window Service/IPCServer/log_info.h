#ifndef FRAMEWORK_LOGINFO_H
#define FRAMEWORK_LOGINFO_H

//#include "std.h"
#include <string>

using namespace std;

namespace VadeApp
{
	// 日志的数据
	struct TLogInfo
	{
	public:
		// 日志等级
		enum TLogLevel
		{
			lglvFatal = 0, 		// 致命错误
			lglvError,			// 普通错误
			lglvWarning,		// 警告信息
			lglvInfo,			// 普通信息
			lglvDebug			// 调试信息
		};

	public:
		TLogInfo(string info, string place, TLogLevel level = lglvInfo, string other = "")
			: m_info(info), m_place(place), m_level(level), m_otner(other)
			, m_excpID(0), m_LogType(0)/*, m_refID(0)*/ {}

	public:
		// 用户输入信息
		string m_info;			// 日志消息
		string m_place;			// 调用的位置
		TLogLevel m_level;		// 日志的等级
		string m_otner;			// 其他标志信息

		// 异常信息
		string m_excpType;		// 异常的类型
		int m_excpID;			// 异常编号

		// 固定的信息
		int m_LogType;			// 调用类型：0框架日志；1模块日志；2服务日志；3引用日志
		//string m_bundleName;	// 模块的名称（子目录名）
		//string m_bundleGUID;	// 模块的GUID
		//string m_serviceName;	// 服务的名称（类名）
		//string m_serviceGUID;	// 服务的GUID
		//long m_refID;			// 服务引用的编号
	};

   //	extern TLogInfo::TLogLevel g_log_level;

	bool WriteLog(TLogInfo log);

}

#endif

