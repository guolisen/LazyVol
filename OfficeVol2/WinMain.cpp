// HttpDownloader.cpp : 定义应用程序的入口点。
//

#include "MainHead.h"

#include "DlgModule.h"
 

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{

	CPaintManagerUI::SetInstance(hInstance);

	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + "..\\res\\OfficeVol");

	HRESULT Hr = ::CoInitialize(NULL);
	if( FAILED(Hr) ) return 0;

	VolMainForm* pFrame = new VolMainForm();
	if( pFrame == NULL ) return 0;
	pFrame->Create(NULL, _T("OfficeVol"), WS_VISIBLE | WS_POPUP, WS_EX_TOOLWINDOW, 0, 0, 160, 160);
	pFrame->CenterWindow();
	pFrame->SetIcon(IDI_ICON1);
	::ShowWindow(*pFrame, SW_SHOW);

	CPaintManagerUI::MessageLoop();

	::CoUninitialize();

	return 0;
}
