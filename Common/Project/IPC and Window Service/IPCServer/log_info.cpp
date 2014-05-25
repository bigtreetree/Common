#include <vcl.h>
#include "log_info.h"
#include "auto_release.h"
#include <boost/thread/mutex.hpp>
#include <boost/filesystem.hpp>
#include <string>
using std::string;
//#include "ReadConfigure.h"
namespace VadeApp
{
	TLogInfo::TLogLevel g_log_level;

	boost::mutex m_mtxLog;
    string GetLogDir();


	bool WriteLog(TLogInfo log)
	{
		bool bResult = true;
		//if (g_log_level >= log.m_level)
		try
		{
			m_mtxLog.lock();
			CAutoRelease _autoUnlock(boost::bind(&boost::mutex::unlock,&m_mtxLog));
			/* TODO : ��ʱ��д��־���� */

			// ������־����
		

			AnsiString sNowTime = FormatDateTime("hh:mm:ss.zzz", Now());
			string sLogInfo = string("[") + sNowTime.c_str() + "]\t|\t"
					+ log.m_info + "\t|\t" + log.m_place + "\t|\t" + log.m_otner
					+ "\t|\t" + log.m_excpType + "\t|\t";

			// ������־�ļ�·��
			AnsiString sNow = DateTimeToStr(Now());   			// ϵͳ��ǰʱ��
			TFormatSettings DateFormat;
			DateFormat.ShortDateFormat = "YYYYMMDD";
			AnsiString sToday = DateToStr(Now(), DateFormat);   // ϵͳ��ǰ����
		
			string sLogPath = "C:\\Users\\Administrator.PC-20140104IKZX\\Documents\\RAD Studio\\release\\";
		
			string sLogFile  = sLogPath + sToday.c_str() + ".txt";  	// ��־�ļ�

			// ����־�ļ�
			TStreamWriter *swLog = new TStreamWriter(sLogFile.c_str(), true);
			boost::shared_ptr<TStreamWriter> _autoDel(swLog);
			//ofstream wfLog;
			//wfLog.open(sLogFile.c_str(), ios::app);
			//CAutoRelease _autoCloseFile(boost::bind(&ofstream::close, &wfLog));

			// д����־����
			swLog->WriteLine(UnicodeString(sLogInfo.c_str()));
			//wfLog << sLogInfo.c_str() << endl;
		}
		catch(...)
		{
			bResult = false;
		}
		return bResult;
	}

	 /*
	string GetLogDir()
	{
		string file;
		IniFile read_file;
		read_file.ReadFile(g_location + "config.ini", "log", "LogFilePath", file);
		string dir = g_location + file;
		string level;
		read_file.ReadFile(g_location + "config.ini", "log", "LogFilePath", level);
		String str = String(level.c_str());
        TLogInfo::TLogLevel g_log_level = TLogInfo::TLogLevel(StrToInt(str));
		return level;
	}
	string GetLogLevel()
	{
		string level;
		IniFile read_file;
		read_file.ReadFile(g_location + "config.ini", "log", "LogFilePath", level);
		return level;
	}
	*/
}

