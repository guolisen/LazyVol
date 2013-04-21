#include "MainHead.h"
#include "OfficeVolModule.h"
#include "OptionDlgModule.h"


OptionDlgModule::OptionDlgModule(HWND parent_hwnd):mParentHwnd(parent_hwnd)
{

}

OptionDlgModule::~OptionDlgModule(void)
{
}


LPCTSTR OptionDlgModule::GetWindowClassName() const 
{ 
    return _T("ScanMainForm"); 
};


void OptionDlgModule::OnFinalMessage(HWND /*hWnd*/) 
{ 
    delete this;
};

void OptionDlgModule::Init() 
{
    CKeyEditUI* tmpEdit = static_cast<CKeyEditUI*>(m_pm.FindControl(_T("OFF_VOLUP")));
	assert(tmpEdit);
	std::string configStr;
    OfficeVolModule::GetInstance()->GetConfigStr("OFF_VOLUP", configStr);
	tmpEdit->SetKey(configStr.c_str());
	tmpEdit->SetText(configStr.c_str());
	mKeyEditVec.push_back(tmpEdit);

    tmpEdit = static_cast<CKeyEditUI*>(m_pm.FindControl(_T("OFF_VOLDOWN")));
	assert(tmpEdit);
	OfficeVolModule::GetInstance()->GetConfigStr("OFF_VOLDOWN", configStr);
	tmpEdit->SetKey(configStr.c_str());
	tmpEdit->SetText(configStr.c_str());
	mKeyEditVec.push_back(tmpEdit);	

    tmpEdit = static_cast<CKeyEditUI*>(m_pm.FindControl(_T("OFF_VOLON")));
	assert(tmpEdit);
	OfficeVolModule::GetInstance()->GetConfigStr("OFF_VOLON", configStr);
	tmpEdit->SetKey(configStr.c_str());
	tmpEdit->SetText(configStr.c_str());
	mKeyEditVec.push_back(tmpEdit);	

    tmpEdit = static_cast<CKeyEditUI*>(m_pm.FindControl(_T("OFF_VOLOFF")));
	assert(tmpEdit);
	OfficeVolModule::GetInstance()->GetConfigStr("OFF_VOLOFF", configStr);
	tmpEdit->SetKey(configStr.c_str());
	tmpEdit->SetText(configStr.c_str());
	mKeyEditVec.push_back(tmpEdit);	

    tmpEdit = static_cast<CKeyEditUI*>(m_pm.FindControl(_T("OFF_VOLSELECT_MASTER")));
	assert(tmpEdit);
	OfficeVolModule::GetInstance()->GetConfigStr("OFF_VOLSELECT_MASTER", configStr);
	tmpEdit->SetKey(configStr.c_str());
	tmpEdit->SetText(configStr.c_str());
	mKeyEditVec.push_back(tmpEdit);	

    tmpEdit = static_cast<CKeyEditUI*>(m_pm.FindControl(_T("OFF_VOLSELECT_WAVE")));
	assert(tmpEdit);
	OfficeVolModule::GetInstance()->GetConfigStr("OFF_VOLSELECT_WAVE", configStr);
	tmpEdit->SetKey(configStr.c_str());
	tmpEdit->SetText(configStr.c_str());
	mKeyEditVec.push_back(tmpEdit);	

    tmpEdit = static_cast<CKeyEditUI*>(m_pm.FindControl(_T("OFF_VOLSELECT_MIC")));
	assert(tmpEdit);
    OfficeVolModule::GetInstance()->GetConfigStr("OFF_VOLSELECT_MIC", configStr);
	tmpEdit->SetKey(configStr.c_str());
	tmpEdit->SetText(configStr.c_str());
	mKeyEditVec.push_back(tmpEdit);	

    tmpEdit = static_cast<CKeyEditUI*>(m_pm.FindControl(_T("OFF_OPTION")));
	assert(tmpEdit);
    OfficeVolModule::GetInstance()->GetConfigStr("OFF_OPTION", configStr);
	tmpEdit->SetKey(configStr.c_str());
	tmpEdit->SetText(configStr.c_str());
	mKeyEditVec.push_back(tmpEdit);	

    tmpEdit = static_cast<CKeyEditUI*>(m_pm.FindControl(_T("OFF_QUIT")));
	assert(tmpEdit);
    OfficeVolModule::GetInstance()->GetConfigStr("OFF_QUIT", configStr);
	tmpEdit->SetKey(configStr.c_str());
	tmpEdit->SetText(configStr.c_str());
	mKeyEditVec.push_back(tmpEdit);		

    mWinStartOpt = static_cast<COptionUI*>(m_pm.FindControl(_T("OFF_WINSTART")));
	assert(mWinStartOpt);
    OfficeVolModule::GetInstance()->GetConfigStr("OFF_WINSTART", configStr);
	if(configStr == "ON")
	{
		mWinStartOpt->Selected(true);
	}
	else
	{
		mWinStartOpt->Selected(false);
	}
}



void OptionDlgModule::Notify(TNotifyUI& msg)
{

    if(msg.sType == _T("click")) 
    {
        if( msg.pSender->GetName() == _T("cancelbtn") || 
			msg.pSender->GetName() == _T("exitbtn")) 
        {
            Close();
            return; 
        }	
		if( msg.pSender->GetName() == _T("okBtn") ) 
        {
            UpdateConfig();
			Close();
            return; 
        }
		if( msg.pSender->GetName() == _T("applyBtn") ) 
        {
            UpdateConfig();
            return; 
        }
    }
	else if (_tcsicmp(msg.sType, _T("selectchanged")) == 0)
	{
		CTabLayoutUI* pTabControl  = static_cast<CTabLayoutUI*>(m_pm.FindControl(kMainTabCtrl));
		CTabLayoutUI* pGTabControl = static_cast<CTabLayoutUI*>(m_pm.FindControl(kGeneralTableCtrl));

		if (_tcsicmp(msg.pSender->GetName(), kTabGeneral) == 0)
		{
			if (pTabControl && pTabControl->GetCurSel() != 0)
			{
				pTabControl->SelectItem(0);
			}
		}
		else if (_tcsicmp(msg.pSender->GetName(), kTabAbout) == 0)
		{
			if (pTabControl && pTabControl->GetCurSel() != 1)
			{
				pTabControl->SelectItem(1);
			}
		}
		else if (_tcsicmp(msg.pSender->GetName(), kGTabKeyboard) == 0)
		{
			if (pGTabControl && pGTabControl->GetCurSel() != 0)
			{
				pGTabControl->SelectItem(0);
			}
		}
		else if (_tcsicmp(msg.pSender->GetName(), kGTabCommon) == 0)
		{
			if (pGTabControl && pGTabControl->GetCurSel() != 1)
			{
				pGTabControl->SelectItem(1);
			}
		}		
	}

}

LRESULT OptionDlgModule::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    //LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
    //styleValue &= ~WS_CAPTION;
	 
    //::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
    m_pm.Init(m_hWnd);
    //m_pm.SetTransparent(100);
    CDialogBuilder builder;
    CControlUI* pRoot = builder.Create(_T("OpSkin.xml"), (UINT)0, this, &m_pm);
    ASSERT(pRoot && "Failed to parse XML");
    m_pm.AttachDialog(pRoot);
    m_pm.AddNotifier(this);
    Init();

    return 0;
}

LRESULT OptionDlgModule::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    bHandled = FALSE;
    return 0;
}

LRESULT OptionDlgModule::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(mParentHwnd)
		::PostMessage(mParentHwnd, MSG_DISMISS_OPTION, wParam, lParam);	
    bHandled = FALSE;
    return 0;
}

LRESULT OptionDlgModule::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    if( ::IsIconic(*this) ) bHandled = FALSE;
    return (wParam == 0) ? TRUE : FALSE;
}

LRESULT OptionDlgModule::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    return 0;
}

LRESULT OptionDlgModule::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    return 0;
}

LRESULT OptionDlgModule::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
#if 1
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

LRESULT OptionDlgModule::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{	
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
	return 0;
}
LRESULT OptionDlgModule::OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    RECT rcWnd = { 0 };
    ::GetWindowRect(m_hWnd, &rcWnd );
    ::SetWindowPos(m_hWnd, HWND_TOPMOST, rcWnd.left, rcWnd.top, rcWnd.right -rcWnd.left,
        rcWnd.bottom -rcWnd.top, SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOZORDER);

    bHandled = FALSE;
    return 0;
}
LRESULT OptionDlgModule::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT OptionDlgModule::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT OptionDlgModule::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT lRes = 0;
    BOOL bHandled = TRUE;
    switch( uMsg ) {
case WM_MOVE:
	Off_DegMsg("sdfsdfddddddddd");
	break;
        case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
        case WM_CLOSE:         lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
        case WM_DESTROY:       lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
        //case WM_NCACTIVATE:    lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
        //case WM_NCCALCSIZE:    lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
        //case WM_NCPAINT:       lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
        //case WM_NCHITTEST:     lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
        case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
        //case WM_MOVE:          lRes = OnMove(uMsg, wParam, lParam, bHandled); break;

		//case WM_GETMINMAXINFO: lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled); break;
        case WM_SYSCOMMAND:    lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); break;
		default:
            bHandled = FALSE;
    }
    if( bHandled ) return lRes;
    if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
    return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

CControlUI* OptionDlgModule::CreateControl(LPCTSTR pstrClass)
{
	std::string strname(pstrClass);
	if(strname == "KEdit")
		return new CKeyEditUI;
	return NULL;
}


LRESULT OptionDlgModule::UpdateConfig()
{
	std::vector<CKeyEditUI*>::iterator iter;
	LRESULT ret = 0;
	std::string old_config;
	for(iter=mKeyEditVec.begin(); iter != mKeyEditVec.end(); ++iter)
	{
		CKeyEditUI* tmpCtrl = *iter;
		assert(tmpCtrl);

		ret = OfficeVolModule::GetInstance()->GetConfigStr((LPCTSTR)tmpCtrl->GetName(), old_config);
		if(ret)
		{
			char str[1024] = {0};
			_snprintf(str, 1024, "获取配置错误 %s", (LPCTSTR)tmpCtrl->GetName());
			Off_Msg(str);
		}

		if(old_config != tmpCtrl->GetKey())
		{
		    ret = OfficeVolModule::GetInstance()->SetConfigStr(OFF_KEY_UPDATE, 
				                                               (LPCTSTR)tmpCtrl->GetName(), 
				                                               tmpCtrl->GetKey());
			if(ret)
			{
				char str[1024] = {0};
				_snprintf(str, 1024, "配置错误 %s:%s", (LPCTSTR)tmpCtrl->GetName(),
					                                    tmpCtrl->GetKey().c_str());
				Off_Msg(str);
			}
		}
	}

	ret = OfficeVolModule::GetInstance()->GetConfigStr((LPCTSTR)mWinStartOpt->GetName(), old_config);
	if(ret)
	{
		char str[1024] = {0};
		_snprintf(str, 1024, "获取配置错误 %s", (LPCTSTR)mWinStartOpt->GetName());
		Off_Msg(str);
		ReadConfig();
		return OFF_ERROR;
	}

	if(old_config == "OFF" && mWinStartOpt->IsSelected())
	{
	    ret = OfficeVolModule::GetInstance()->SetConfigStr(OFF_WIN_START, 
			                                               (LPCTSTR)mWinStartOpt->GetName(), 
			                                               "ON");
		if(ret)
		{
			char str[1024] = {0};
			_snprintf(str, 1024, "配置错误 %s:%s", (LPCTSTR)mWinStartOpt->GetName(),
				                                    "ON");
			Off_Msg(str);
		}
	}
	else if(old_config == "ON" && !mWinStartOpt->IsSelected())
	{
	    ret = OfficeVolModule::GetInstance()->SetConfigStr(OFF_WIN_START, 
			                                               (LPCTSTR)mWinStartOpt->GetName(), 
			                                               "OFF");
		if(ret)
		{
			char str[1024] = {0};
			_snprintf(str, 1024, "配置错误 %s:%s", (LPCTSTR)mWinStartOpt->GetName(),
				                                    "OFF");
			Off_Msg(str);
		}
	}
	
	ReadConfig();
	
	return 0;
}

LRESULT OptionDlgModule::ReadConfig()
{
	std::vector<CKeyEditUI*>::iterator iter;
	LRESULT ret = 0;
	std::string configStr;
	for(iter=mKeyEditVec.begin(); iter != mKeyEditVec.end(); ++iter)
	{
		CKeyEditUI* tmpCtrl = *iter;
		assert(tmpCtrl);
	    ret = OfficeVolModule::GetInstance()->GetConfigStr((LPCTSTR)tmpCtrl->GetName(), configStr);
		if(ret)
		{
			char str[1024] = {0};
			_snprintf(str, 1024, "获取配置错误 %s", (LPCTSTR)tmpCtrl->GetName());
			Off_Msg(str);
		}
		tmpCtrl->SetKey(configStr.c_str());
		tmpCtrl->SetText(configStr.c_str());
	}

	return 0;
}


