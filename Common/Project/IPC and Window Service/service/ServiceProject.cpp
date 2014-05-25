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
		bool isInstall = FindCmdLineSwitch("install", true);    //安装服务标志
		bool isUninstall = FindCmdLineSwitch("uninstall", true); //卸载服务标志
		AnsiString serviceName;//服务名称
		AnsiString serviceDispName;//服务显示名称
		if(ParamCount() > 1 && (isInstall || isUninstall))
		{
			Application->CreateForm(__classid(TService1), &Service1);
			if(isInstall || isUninstall)//对于安装或卸哉情况
			{
				for(int i = 1; i <= ParamCount(); i++)
				{
					AnsiString param = ParamStr(i).LowerCase();//为便于比较，先转换成小写
					int pos;//
					if (param.Pos("/name:")) // 匹配服务名称： "/name:xxxxx"
					{
						pos = param.Pos(":");
						serviceName = param.SubString(pos + 1, param.Length() - pos);
						if ((pos = serviceName.Pos(" ")) > 0) // 删除空格，这个其实不必要，系统会指出名称非法
						{
							serviceName.Delete(pos, 1);
						}
					}
					else
					if (param.Pos("/dispname:")) // 匹配服务显示名称： "/serviceDispName:xxxxx"
					{
						pos = param.Pos(":");
						serviceDispName = param.SubString(pos + 1, param.Length() - pos);
					}
				}
				if(serviceName.Length() > 0)
				Application->Components[0]->Name = serviceName; // 使用指定的服务名称
				else
				serviceName = Application->Components[0]->Name; // 未指定服务名称，会使用TMyService的默认名称：MyService
				if (serviceDispName.Length() == 0)
				serviceDispName = serviceName;  // 如果未指定显示名称，使用服务名称来代替

				((TService *)Application->Components[0])->DisplayName = serviceDispName; // 设置显示名称
			}
			else // 非安装，即启动
			{
				// 检查参数个数
				if (ParamCount() > 0)
				{
					AnsiString extraParam = ParamStr(1).LowerCase(); // 额外参数，转换为小写
					AnsiString specifiedServiceName;
					if (extraParam.Pos("-")) // 定位"-"
					{
						int pos = extraParam.Pos("-");
						specifiedServiceName = extraParam.SubString(pos + 1, extraParam.Length() - pos); // 解析服务名称
					}
					if (!specifiedServiceName.Length()) // 如果名称无效，则使用默认名称：MyService
					{
						Application->CreateForm(__classid(TService1), &Service1); // 使用默认名称
					}
					else
					{
						Application->CreateForm(__classid(TService1), &Service1);
						//Service1->Name = specifiedServiceName; // 使用指定的名称
					}
				} //
			} // if ... else

			Application->Run();
			// 需要在Run以后，此时服务已经成功安装
			if (isInstall)
			{
				// 最简单的方法是Hack注册表
				TRegistry* reg = new TRegistry();
				AnsiString key = "//System//CurrentControlSet//Services//" + serviceName; // 注册表路径
				reg->RootKey = HKEY_LOCAL_MACHINE;
				if (reg->OpenKey(key, false)) // 打开荐
				{
					AnsiString imagePath = reg->ReadString("ImagePath"); // 介就系程序映像的路径
					reg->WriteString("ImagePath", imagePath + " -" + serviceName); // 我们要做的是添加额外的参数：服务名称
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
