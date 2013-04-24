// HttpDownloader.cpp : 定义应用程序的入口点。
//

#include "MainHead.h"

#include "DlgModule.h"
 


BOOL IfAppExist()
{
	//创建进程互斥体Sample07
	HANDLE hMutex = CreateMutex(NULL, TRUE, _T("OfficeVol"));
	if (hMutex == NULL)
	{
		return TRUE;
	}
	//如果程序已经存在并且正在运行
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		HWND hProgramWnd = ::FindWindow(NULL, _T("OfficeVol"));
		if (hProgramWnd)
		{
			WINDOWPLACEMENT* pWndpl = NULL;
			WINDOWPLACEMENT   wpm; 
			pWndpl =&wpm;
			GetWindowPlacement(hProgramWnd,&wpm);
			if (pWndpl)
			{
				//将运行的程序窗口还原成正常状态
				pWndpl->showCmd = SW_SHOWNORMAL;
				::SetWindowPlacement(hProgramWnd,pWndpl);
				SetWindowPos(hProgramWnd,HWND_NOTOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
			}
		}
		//关闭进程互斥体
		CloseHandle(hMutex);
		hMutex = NULL;
		return TRUE;
	}

	return FALSE;
}


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR lpCmdLine, int nCmdShow)
{
	if(IfAppExist())
		return 0;
	
	if(lpCmdLine)
		SetCurrentDirectory(lpCmdLine);

	CPaintManagerUI::SetInstance(hInstance);

	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + "res\\OfficeVol");

	HRESULT Hr = ::CoInitialize(NULL);
	if( FAILED(Hr) ) 
		return 0;

	VolMainForm* pFrame = new VolMainForm();
	if( pFrame == NULL ) 
		return 0;
	pFrame->Create(NULL, _T("OfficeVol"),  WS_POPUP, WS_EX_TOOLWINDOW, 0, 0, 160, 160);
	pFrame->CenterWindow();
	pFrame->SetIcon(IDI_ICON1);
	//::ShowWindow(*pFrame, SW_SHOW);

	if(*lpCmdLine)
		pFrame->ShowPad();
	else	
		pFrame->ShowOption();
	
	CPaintManagerUI::MessageLoop();

	::CoUninitialize();

	return 0;
}
