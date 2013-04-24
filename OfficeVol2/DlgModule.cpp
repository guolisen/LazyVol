
#include "MainHead.h"
#include "OfficeVolModule.h"
#include <UIlib.h>
#include "resource.h"

using namespace DuiLib;


#include "DlgModule.h"
#include "OptionDlgModule.h"


static std::string s_productName("OfficeVol");

VolMainForm::~VolMainForm()
{

}


LPCTSTR VolMainForm::GetWindowClassName() const 
{ 
    return _T("ScanMainForm"); 
};


void VolMainForm::OnFinalMessage(HWND /*hWnd*/) 
{ 
    delete this;
};

void VolMainForm::Init() 
{
	::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	if(OfficeVolModule::GetInstance()->Init(this, ""))
	{
		Off_Msg("Module Init Error!");
		::PostQuitMessage(0L);
		return;
	}
	
    mProgress = static_cast<CProgressUI*>(m_pm.FindControl(_T("progressbar")));
    mVolLogo  = static_cast<CLabelUI*>(m_pm.FindControl(_T("logolabel")));
	mVolName  = static_cast<CTextUI*>(m_pm.FindControl(_T("coltext")));
	
	mProgress->SetMaxValue(OfficeVolModule::GetInstance()->GetMaxVol());
	mProgress->SetMinValue(OfficeVolModule::GetInstance()->GetMinVol());
	mProgress->SetValue(OfficeVolModule::GetInstance()->GetCurrentVolume());

//	ShowPad();

}

void VolMainForm::OnPrepare(TNotifyUI& msg) 
{

}


void VolMainForm::Notify(TNotifyUI& msg)
{
#if 0
    if( msg.sType == _T("windowinit") ) 
        OnPrepare(msg);
    else if( msg.sType == _T("click") ) 
    {
        if( msg.pSender == m_pCloseBtn ) 
        {
            PostQuitMessage(0);
            return; 
        }
        else if( msg.pSender == m_pMinBtn ) 
        { 
            SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
            return; 
        }
        else if( msg.pSender == m_pMaxBtn ) 
        { 
            SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0); return; 
        }
        else if( msg.pSender == m_pRestoreBtn ) 
        { 
            SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0); return; 
        }

    }
    else if(msg.sType==_T("setfocus"))
    {
    }
    else if( msg.sType == _T("itemclick") ) 
    {
    }
    else if( msg.sType == _T("itemactivate") ) 
    {
        int iIndex = msg.pSender->GetTag();
        CDuiString sMessage = _T("Click: ");;
#ifdef _UNICODE		
        int iLen = domain[iIndex].length();
        LPWSTR lpText = new WCHAR[iLen + 1];
        ::ZeroMemory(lpText, (iLen + 1) * sizeof(WCHAR));
        ::MultiByteToWideChar(CP_ACP, 0, domain[iIndex].c_str(), -1, (LPWSTR)lpText, iLen) ;
        sMessage += lpText;
        delete[] lpText;
#else
       // sMessage += domain[iIndex].c_str();

#endif
        ::MessageBox(NULL, sMessage.GetData(), _T("提示(by tojen)"), MB_OK);
    }
    else if(msg.sType == _T("menu")) 
    {

    }
    else if( msg.sType == _T("menu_Delete") ) {
  
    }
#endif
}

LRESULT VolMainForm::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
    styleValue &= ~WS_CAPTION;
	 
    ::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
    m_pm.Init(m_hWnd);
    //m_pm.SetTransparent(100);
    CDialogBuilder builder;
    CControlUI* pRoot = builder.Create(_T("skin.xml"), (UINT)0, NULL, &m_pm);
    ASSERT(pRoot && "Failed to parse XML");
    m_pm.AttachDialog(pRoot);
    m_pm.AddNotifier(this);
    Init();

    return 0;
}

LRESULT VolMainForm::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    bHandled = FALSE;
    return 0;
}

LRESULT VolMainForm::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    ::PostQuitMessage(0L);
    bHandled = FALSE;
    return 0;
}

LRESULT VolMainForm::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    if( ::IsIconic(*this) ) bHandled = FALSE;
    return (wParam == 0) ? TRUE : FALSE;
}

LRESULT VolMainForm::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    return 0;
}

LRESULT VolMainForm::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    return 0;
}

LRESULT VolMainForm::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
#if 0
    POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
    ::ScreenToClient(*this, &pt);

    RECT rcClient;
    ::GetClientRect(*this, &rcClient);

    if( !::IsZoomed(*this) ) {
        RECT rcSizeBox = m_pm.GetSizeBox();
        if( pt.y < rcClient.top + rcSizeBox.top ) {
            if( pt.x < rcClient.left + rcSizeBox.left ) return HTTOPLEFT;
            if( pt.x > rcClient.right - rcSizeBox.right ) return HTTOPRIGHT;
            return HTTOP;
        }
        else if( pt.y > rcClient.bottom - rcSizeBox.bottom ) {
            if( pt.x < rcClient.left + rcSizeBox.left ) return HTBOTTOMLEFT;
            if( pt.x > rcClient.right - rcSizeBox.right ) return HTBOTTOMRIGHT;
            return HTBOTTOM;
        }
        if( pt.x < rcClient.left + rcSizeBox.left ) return HTLEFT;
        if( pt.x > rcClient.right - rcSizeBox.right ) return HTRIGHT;
    }

    RECT rcCaption = m_pm.GetCaptionRect();
    if( pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
        && pt.y >= rcCaption.top && pt.y < rcCaption.bottom ) {
            CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(pt));
            if( pControl && _tcscmp(pControl->GetClass(), _T("ButtonUI")) != 0 && 
                _tcscmp(pControl->GetClass(), _T("OptionUI")) != 0 &&
                _tcscmp(pControl->GetClass(), _T("TextUI")) != 0 )
                return HTCAPTION;
    }
#endif
    return HTCLIENT;
}

LRESULT VolMainForm::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
#if 0
    SIZE szRoundCorner = m_pm.GetRoundCorner();
    if( !::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0) ) {
        CDuiRect rcWnd;
        ::GetWindowRect(*this, &rcWnd);
        rcWnd.Offset(-rcWnd.left, -rcWnd.top);
        rcWnd.right++; rcWnd.bottom++;
        RECT rc = { rcWnd.left, rcWnd.top + szRoundCorner.cx, rcWnd.right, rcWnd.bottom };
        HRGN hRgn1 = ::CreateRectRgnIndirect( &rc );
        HRGN hRgn2 = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom - szRoundCorner.cx, szRoundCorner.cx, szRoundCorner.cy);
        ::CombineRgn( hRgn1, hRgn1, hRgn2, RGN_OR );
        ::SetWindowRgn(*this, hRgn1, TRUE);
        ::DeleteObject(hRgn1);
        ::DeleteObject(hRgn2);
    }

    bHandled = FALSE;
#endif	
    return 0;
}

LRESULT VolMainForm::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
#if 0
    MONITORINFO oMonitor = {};
    oMonitor.cbSize = sizeof(oMonitor);
    ::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
    CDuiRect rcWork = oMonitor.rcWork;
	rcWork.Offset(-oMonitor.rcMonitor.left, -oMonitor.rcMonitor.top);

    LPMINMAXINFO lpMMI = (LPMINMAXINFO) lParam;
    lpMMI->ptMaxPosition.x	= rcWork.left;
    lpMMI->ptMaxPosition.y	= rcWork.top;
    lpMMI->ptMaxSize.x		= rcWork.right;
    lpMMI->ptMaxSize.y		= rcWork.bottom;

    bHandled = FALSE;
#endif
    return 0;
}

LRESULT VolMainForm::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
#if 0
    // 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
    if( wParam == SC_CLOSE ) {
        ::PostQuitMessage(0L);
        bHandled = TRUE;
        return 0;
    }
    BOOL bZoomed = ::IsZoomed(*this);
    LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
    if( ::IsZoomed(*this) != bZoomed ) {
        if( !bZoomed ) {
            CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("maxbtn")));
            if( pControl ) pControl->SetVisible(false);
            pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("restorebtn")));
            if( pControl ) pControl->SetVisible(true);
        }
        else {
            CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("maxbtn")));
            if( pControl ) pControl->SetVisible(true);
            pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("restorebtn")));
            if( pControl ) pControl->SetVisible(false);
        }
    }

    return lRes;
#endif

	return 0;
}

LRESULT VolMainForm::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT lRes = 0;
    BOOL bHandled = TRUE;
    switch( uMsg ) {
        case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
        case WM_CLOSE:         lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
        case WM_DESTROY:       lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
        case WM_NCACTIVATE:    lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
        case WM_NCCALCSIZE:    lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
        case WM_NCPAINT:       lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
        case WM_NCHITTEST:     lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
        case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
        case WM_GETMINMAXINFO: lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled); break;
        case WM_SYSCOMMAND:    lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); break;
        case WM_TIMER:         lRes = OnTimer(uMsg, wParam, lParam, bHandled); break;
		case WM_HOTKEY:		   lRes = OnHotKey(uMsg, wParam, lParam, bHandled); break;
		case MSG_DISMISS_OPTION:	lRes = OnOptDismiss(uMsg, wParam, lParam, bHandled); break;
		case MSG_SHOW_PAD:	   lRes = OnShowPad(uMsg, wParam, lParam, bHandled); break;

		default:
            bHandled = FALSE;
    }
    if( bHandled ) return lRes;
    if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
    return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

LRESULT VolMainForm::OnOptDismiss(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	mIsOptionShow = false;
	return OFF_OK;
}

LRESULT VolMainForm::OnHotKey(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HOTKEY_MAP::iterator iter = mHotkeyMap.find(wParam);
	if (iter == mHotkeyMap.end())
		return OFF_ERROR;

	OfficeVolModule::GetInstance()->UpdateStatus(iter->second);
	
	return OFF_OK;
}

LRESULT VolMainForm::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HidePad();
	return 0;
}
LRESULT VolMainForm::OnShowPad(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	ShowPad();
	return 0;
}

LRESULT VolMainForm::SetVolLogo(DWORD vol_level)
{
	if(OfficeVolModule::GetInstance()->IsAvailable())
	{
		if(OfficeVolModule::GetInstance()->GetCurrentVolume() <= OfficeVolModule::GetInstance()->GetMinVol())
			mVolLogo->SetBkImage(_T("sound_mute.png"));
		else if(OfficeVolModule::GetInstance()->GetCurrentVolume() < (OfficeVolModule::GetInstance()->GetMaxVol()/2))
			mVolLogo->SetBkImage(_T("sound_low.png"));
		else
			mVolLogo->SetBkImage(_T("sound_high.png"));
	}
	else
	{
		mVolLogo->SetBkImage(_T("sound_mute.png"));
	}

	return 0;
}

LRESULT VolMainForm::SetVolText(DWORD vol_level)
{
	switch(vol_level)
	{
		case MASTER_VOL:
			mVolName->SetText("Master Vol");
				break;
		case WAVE_VOL:
			mVolName->SetText("Wave Vol");
				break;		
		default:
			mVolName->SetText("Unknow Vol");
				break;	
	};

	return 0;
}

LRESULT VolMainForm::ShowPad()
{
	SetVolLogo(OfficeVolModule::GetInstance()->GetCurrentVolume()); 
	SetVolText(OfficeVolModule::GetInstance()->GetVolType());
    if(mIsShow)
	{
		KillTimer(m_hWnd, 0);
		SetTimer(m_hWnd, 0, 1000, NULL);
	}
	else
	{
		m_pm.NeedUpdate();  
		m_pm.GetRoot()->NeedUpdate();
		
		::ShowWindow(m_hWnd, SW_SHOW);
		::SetWindowPos(m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE |SWP_NOACTIVATE);
		SetTimer(m_hWnd, 0, 1000, NULL);
		mIsShow = TRUE;
	}

	return 0;
}

LRESULT VolMainForm::HidePad()
{
	::ShowWindow(m_hWnd, SW_HIDE);
	mIsShow = FALSE;
	
	return 0;
}
LRESULT VolMainForm::ShowOption()
{
	OptionDlgModule* pOption = new OptionDlgModule(m_hWnd);
	assert(pOption);
	pOption->Create(NULL, _T("OfficeVolOption"), WS_VISIBLE | WS_POPUP, NULL, 0, 0, 0, 0);
	pOption->CenterWindow();
	::ShowWindow(*pOption, SW_SHOW);
	mIsOptionShow = true;

	return OFF_OK;
}

LRESULT VolMainForm::RegisterHotkey(const std::string& hotkey)
{
	ModifierKeyParser parser(hotkey);
	if (!parser.IsValid())
		return false;

	ATOM hotkeyID = GlobalAddAtomA((s_productName + hotkey).c_str());
	if (mHotkeyMap.find(hotkeyID) != mHotkeyMap.end())
		return OFF_KEY_MULTIDEFINE;

	if (!::RegisterHotKey(m_hWnd, hotkeyID, parser.GetModifier(), parser.GetKey()))
	{
		DWORD dd = GetLastError();
		return OFF_ERROR_KEY_REGISTE;
	}

	mHotkeyMap[hotkeyID] = hotkey;
	return OFF_OK;
}

void VolMainForm::UnRegisterHotkey(const std::string& hotkey)
{
	ATOM hotkeyID = GlobalAddAtomA((s_productName + hotkey).c_str());

	HOTKEY_MAP::iterator iter = mHotkeyMap.find(hotkeyID);
	if (iter != mHotkeyMap.end())
	{
		::UnregisterHotKey(m_hWnd, hotkeyID);
		mHotkeyMap.erase(iter);
	}
}


int VolMainForm::UiUpdate(const std::string& action)
{
	if(mIsOptionShow)
		return OFF_OK;
	
	if(action == "OFF_VOLUP") 
   	{
		OfficeVolModule::GetInstance()->VolUp();
		mProgress->SetValue(OfficeVolModule::GetInstance()->GetCurrentVolume());
		ShowPad();
    }
    else if(action == "OFF_VOLDOWN") 
    {
       	OfficeVolModule::GetInstance()->VolDown();
		mProgress->SetValue(OfficeVolModule::GetInstance()->GetCurrentVolume());	
		ShowPad();
    }
    else if(action == "OFF_VOLSELECT_MASTER") 
	{
		OfficeVolModule::GetInstance()->ChangeVolType(MASTER_VOL);
		mProgress->SetValue(OfficeVolModule::GetInstance()->GetCurrentVolume());	
		ShowPad();
	}
    else if(action == "OFF_VOLSELECT_WAVE") 
	{
		OfficeVolModule::GetInstance()->ChangeVolType(WAVE_VOL);
		mProgress->SetValue(OfficeVolModule::GetInstance()->GetCurrentVolume());	
		ShowPad();
	}
    else if(action == "OFF_VOLSELECT_MIC") 
	{
		//OfficeVolModule::GetInstance()->ChangeVolType(WAVE_VOL);
		//mProgress->SetValue(OfficeVolModule::GetInstance()->GetCurrentVolume());	
		//ShowPad();
	}	
    else if(action == "OFF_OPTION") 
	{
		OptionDlgModule* pOption = new OptionDlgModule(m_hWnd);
		if( pOption == NULL ) 
			return 0;
		pOption->Create(NULL, _T("OfficeVolOption"), WS_VISIBLE | WS_POPUP, NULL, 0, 0, 0, 0);
		pOption->CenterWindow();
		::ShowWindow(*pOption, SW_SHOW);
		mIsOptionShow = true;
	}	
    else if(action == "OFF_QUIT") 
	{
		::MessageBox(m_hWnd, "再见！", "Create By Guolisen", MB_OK);
		OfficeVolModule::GetInstance()->UnInit();

		::PostMessage(m_hWnd, WM_DESTROY, 0, 0);
	}
    else if(action == "OFF_VOLON") 
	{
		OfficeVolModule::GetInstance()->Enable();
		ShowPad();
	}
    else if(action == "OFF_VOLOFF") 
	{
		OfficeVolModule::GetInstance()->Disable();
		ShowPad();
	}
	
	return 0;
}	
int VolMainForm::ReConfig(OFF_CONFIG_CLASS config_class, 
		                const std::string& value, 
		                const std::string& old_value)
{
	if(value == old_value)
		return OFF_OK;
	
	if(OFF_KEY_UPDATE == config_class)
	{
		if(value.empty())
			return OFF_ERROR_PARAMTER;	
		if(!old_value.empty())
			UnRegisterHotkey(old_value);
		
		LRESULT ret = RegisterHotkey(value);
		if(OFF_KEY_MULTIDEFINE == ret || OFF_ERROR_KEY_REGISTE == ret)
		{
			char outStr[1024] = {0};
			_snprintf(outStr, 1024, "热键(%s)重复", value.c_str());
			::MessageBox(m_hWnd, "热键重复", outStr, MB_OK);
			return OFF_ERROR;
		}
	}
	else if(OFF_WIN_START == config_class)
	{
		if(value == "ON")
		{
			TCHAR modlepath[1024];
			TCHAR syspath[1024];
			HRESULT hr;
			GetModuleFileName(0, modlepath, 256);//取得程序名字
			SHGetSpecialFolderPath(NULL, syspath, CSIDL_COMMON_STARTUP, TRUE);
			strcat(syspath, "\\LazyVol.lnk");

			TCHAR szBuf[1024];
			ZeroMemory(szBuf, 1024);
			GetCurrentDirectory(1024, szBuf);

			IShellLink *pisl;

			hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&pisl);
			if (SUCCEEDED(hr))
			{
				IPersistFile* pIPF;
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////

				//这里是我们要创建快捷方式的原始文件地址
				pisl->SetPath(modlepath);
				pisl->SetArguments(szBuf);
				hr = pisl->QueryInterface(IID_IPersistFile, (void**)&pIPF);
				if (SUCCEEDED(hr))
				{   

					//这里是我们要创建快捷方式的目标地址
					WCHAR pWidePath[256];
					ZeroMemory(pWidePath, 256);
					MultiByteToWideChar(CP_ACP,0, syspath, 256, pWidePath, 256);
					pIPF->Save(pWidePath, FALSE);
					pIPF->Release();
				}
				pisl->Release();
			}

		}
		else
		{
			TCHAR modlepath[256];
			TCHAR syspath[256];
			GetModuleFileName(0, modlepath, 256);//取得程序名字
			SHGetSpecialFolderPath(NULL, syspath, CSIDL_COMMON_STARTUP,TRUE);
			strcat(syspath,"\\LazyVol.lnk");
			
			::DeleteFile(syspath);
		}
	}
	
	return OFF_OK;
}

void VolMainForm::UnInitOffVol()
{
	HOTKEY_MAP::iterator iter;
	for(iter = mHotkeyMap.begin(); iter != mHotkeyMap.end(); )
	{
		std::string key = iter->second;
		++iter;
		UnRegisterHotkey(key);
	}
}
#if 0
void CMyMCIDlg::OnMciPlay()  
{  
	isPause=true;  
	SetTimer(1,33,NULL);  
	mciSendCommand(m_wDeviceID, MCI_CLOSE, 0, NULL);  
	MCI_OPEN_PARMS mciOpen;  
	MCI_PLAY_PARMS mciPlay;  
	mciOpen.lpstrElementName=strFileName.GetBuffer(strFileName.GetLength());  
	mciSendCommand(NULL,MCI_OPEN,MCI_OPEN_ELEMENT,(DWORD)(LPVOID)&mciOpen);  
	m_wDeviceID=mciOpen.wDeviceID;  MCI_STATUS_PARMS mciStatusParms;  
	mciStatusParms.dwItem=MCI_STATUS_LENGTH;  

	mciSendCommand(m_wDeviceID,MCI_STATUS,MCI_WAIT|MCI_STATUS_ITEM,(DWORD)(LPVOID)&mciStatusParms);  
	m_dLength=mciStatusParms.dwReturn;  
	mciSendCommand(m_wDeviceID,MCI_PLAY,0,(DWORD)(LPVOID)&mciPlay);  
	m_position.SetRange(0,m_dLength);  
	m_position.SetPos(0); 
}
#endif

