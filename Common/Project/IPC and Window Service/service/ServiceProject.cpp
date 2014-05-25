#include <SysUtils.hpp>
#include <SvcMgr.hpp>
#include <Registry.hpp>
#pragma hdrstop
#define Application Svcmgr::Application
USEFORM("service.cpp", Service1); /* TService: File Type */
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	try
	{
		// Windows 2003 Server requires StartServiceCtrlDispatcher to be
		// called before CoRegisterClassObject, which can be called indirectly
		// by Application.Initialize. TServiceApplication->DelayInitialize allows
		// Application->Initialize to be called from TService->Main (after
		// StartServiceCtrlDispatcher has been called).
		//
		// Delayed initialization of the Application object may affect
		// events which then occur prior to initialization, such as
		// TService->OnCreate. It is only recommended if the ServiceApplication
		// registers a class object with OLE and is intended for use with
		// Windows 2003 Server.
		//
		// Application->DelayInitialize = true;
		//
		if ((!Application->DelayInitialize) || (Application->Installing()))
		{
			Application->Initialize();
		}
		bool isInstall = FindCmdLineSwitch("install", true);    //��װ�����־
		bool isUninstall = FindCmdLineSwitch("uninstall", true); //ж�ط����־
		AnsiString serviceName;//��������
		AnsiString serviceDispName;//������ʾ����
		if(ParamCount() > 1 && (isInstall || isUninstall))
		{
			Application->CreateForm(__classid(TService1), &Service1);
			if(isInstall || isUninstall)//���ڰ�װ��ж�����
			{
				for(int i = 1; i <= ParamCount(); i++)
				{
					AnsiString param = ParamStr(i).LowerCase();//Ϊ���ڱȽϣ���ת����Сд
					int pos;//
					if (param.Pos("/name:")) // ƥ��������ƣ� "/name:xxxxx"
					{
						pos = param.Pos(":");
						serviceName = param.SubString(pos + 1, param.Length() - pos);
						if ((pos = serviceName.Pos(" ")) > 0) // ɾ���ո������ʵ����Ҫ��ϵͳ��ָ�����ƷǷ�
						{
							serviceName.Delete(pos, 1);
						}
					}
					else
					if (param.Pos("/dispname:")) // ƥ�������ʾ���ƣ� "/serviceDispName:xxxxx"
					{
						pos = param.Pos(":");
						serviceDispName = param.SubString(pos + 1, param.Length() - pos);
					}
				}
				if(serviceName.Length() > 0)
				Application->Components[0]->Name = serviceName; // ʹ��ָ���ķ�������
				else
				serviceName = Application->Components[0]->Name; // δָ���������ƣ���ʹ��TMyService��Ĭ�����ƣ�MyService
				if (serviceDispName.Length() == 0)
				serviceDispName = serviceName;  // ���δָ����ʾ���ƣ�ʹ�÷�������������

				((TService *)Application->Components[0])->DisplayName = serviceDispName; // ������ʾ����
			}
			else // �ǰ�װ��������
			{
				// ����������
				if (ParamCount() > 0)
				{
					AnsiString extraParam = ParamStr(1).LowerCase(); // ���������ת��ΪСд
					AnsiString specifiedServiceName;
					if (extraParam.Pos("-")) // ��λ"-"
					{
						int pos = extraParam.Pos("-");
						specifiedServiceName = extraParam.SubString(pos + 1, extraParam.Length() - pos); // ������������
					}
					if (!specifiedServiceName.Length()) // ���������Ч����ʹ��Ĭ�����ƣ�MyService
					{
						Application->CreateForm(__classid(TService1), &Service1); // ʹ��Ĭ������
					}
					else
					{
						Application->CreateForm(__classid(TService1), &Service1);
						//Service1->Name = specifiedServiceName; // ʹ��ָ��������
					}
				} //
			} // if ... else

			Application->Run();
			// ��Ҫ��Run�Ժ󣬴�ʱ�����Ѿ��ɹ���װ
			if (isInstall)
			{
				// ��򵥵ķ�����Hackע���
				TRegistry* reg = new TRegistry();
				AnsiString key = "//System//CurrentControlSet//Services//" + serviceName; // ע���·��
				reg->RootKey = HKEY_LOCAL_MACHINE;
				if (reg->OpenKey(key, false)) // �򿪼�
				{
					AnsiString imagePath = reg->ReadString("ImagePath"); // ���ϵ����ӳ���·��
					reg->WriteString("ImagePath", imagePath + " -" + serviceName); // ����Ҫ��������Ӷ���Ĳ�������������
				}
				reg->CloseKey();
				delete reg;
			} // if isInstall then hack registry
		}
	}
	catch (Exception &exception)
	{
		Sysutils::ShowException(&exception, System::ExceptAddr());
	}
	catch(...)
	{
	try
	{
			throw Exception("");
	}
	catch(Exception &exception)
	{
		Sysutils::ShowException(&exception, System::ExceptAddr());
	}
	}
	return 0;
}
