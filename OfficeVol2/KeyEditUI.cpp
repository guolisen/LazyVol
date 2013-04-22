
#include "MainHead.h"
#include "OfficeVolModule.h"
#include "KeyEditUI.h"


class CKeyEditWnd : public CWindowWnd
{
public:
	CKeyEditWnd();

	void Init(CKeyEditUI* pOwner);
	RECT CalPos();

	LPCTSTR GetWindowClassName() const;
	LPCTSTR GetSuperClassName() const;
	void    OnFinalMessage(HWND hWnd);

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnEditChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

protected:
	CKeyEditUI* m_pOwner;
	HBRUSH m_hBkBrush;
	bool m_bInit;
};


CKeyEditWnd::CKeyEditWnd() : m_pOwner(NULL), m_hBkBrush(NULL), m_bInit(false)
{
}

void CKeyEditWnd::Init(CKeyEditUI* pOwner)
{
	m_pOwner = pOwner;
	RECT rcPos = CalPos();
	UINT uStyle = WS_CHILD | ES_AUTOHSCROLL;
	if( m_pOwner->IsPasswordMode() ) uStyle |= ES_PASSWORD;
	Create(m_pOwner->GetManager()->GetPaintWindow(), NULL, uStyle, 0, rcPos);
	SetWindowFont(m_hWnd, m_pOwner->GetManager()->GetFontInfo(m_pOwner->GetFont())->hFont, TRUE);
	Edit_LimitText(m_hWnd, m_pOwner->GetMaxChar());
	if( m_pOwner->IsPasswordMode() ) Edit_SetPasswordChar(m_hWnd, m_pOwner->GetPasswordChar());
	Edit_SetText(m_hWnd, m_pOwner->GetText());
	Edit_SetModify(m_hWnd, FALSE);
	SendMessage(EM_SETMARGINS, EC_LEFTMARGIN | EC_RIGHTMARGIN, MAKELPARAM(0, 0));
	Edit_Enable(m_hWnd, m_pOwner->IsEnabled() == true);
	Edit_SetReadOnly(m_hWnd, m_pOwner->IsReadOnly() == true);
	//Styls
	LONG styleValue = ::GetWindowLong(m_hWnd, GWL_STYLE);
	styleValue |= pOwner->GetWindowStyls();
	::SetWindowLong(GetHWND(), GWL_STYLE, styleValue);
	::ShowWindow(m_hWnd, SW_SHOWNOACTIVATE);
	::SetFocus(m_hWnd);
	m_bInit = true;    
}

RECT CKeyEditWnd::CalPos()
{
	CDuiRect rcPos = m_pOwner->GetPos();
	RECT rcInset = m_pOwner->GetTextPadding();
	rcPos.left += rcInset.left;
	rcPos.top += rcInset.top;
	rcPos.right -= rcInset.right;
	rcPos.bottom -= rcInset.bottom;
	LONG lEditHeight = m_pOwner->GetManager()->GetFontInfo(m_pOwner->GetFont())->tm.tmHeight;
	if( lEditHeight < rcPos.GetHeight() ) {
		rcPos.top += (rcPos.GetHeight() - lEditHeight) / 2;
		rcPos.bottom = rcPos.top + lEditHeight;
	}
	return rcPos;
}

LPCTSTR CKeyEditWnd::GetWindowClassName() const
{
	return _T("EditWnd");
}

LPCTSTR CKeyEditWnd::GetSuperClassName() const
{
	return WC_EDIT;
}

void CKeyEditWnd::OnFinalMessage(HWND /*hWnd*/)
{
	m_pOwner->Invalidate();
	// Clear reference and die
	if( m_hBkBrush != NULL ) ::DeleteObject(m_hBkBrush);
	m_pOwner->m_pWindow = NULL;
	delete this;
}

static UINT MapKeyState()
{
    UINT uState = 0;
    if( ::GetKeyState(VK_CONTROL) < 0 ) uState |= MK_CONTROL;
    if( ::GetKeyState(VK_RBUTTON) < 0 ) uState |= MK_LBUTTON;
    if( ::GetKeyState(VK_LBUTTON) < 0 ) uState |= MK_RBUTTON;
    if( ::GetKeyState(VK_SHIFT) < 0 ) uState |= MK_SHIFT;
    if( ::GetKeyState(VK_MENU) < 0 ) uState |= MK_ALT;
    return uState;
}

LRESULT CKeyEditWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	if( uMsg == WM_KILLFOCUS ) lRes = OnKillFocus(uMsg, wParam, lParam, bHandled);
	else if( uMsg == OCM_COMMAND ) {
		if( GET_WM_COMMAND_CMD(wParam, lParam) == EN_CHANGE ) lRes = OnEditChanged(uMsg, wParam, lParam, bHandled);
		else if( GET_WM_COMMAND_CMD(wParam, lParam) == EN_UPDATE ) {
			RECT rcClient;
			::GetClientRect(m_hWnd, &rcClient);
			::InvalidateRect(m_hWnd, &rcClient, FALSE);
		}
	}
	else if( uMsg == WM_KEYDOWN && TCHAR(wParam) == VK_RETURN ) {
		m_pOwner->GetManager()->SendNotify(m_pOwner, DUI_MSGTYPE_RETURN);
	}
	else if( uMsg == WM_KEYDOWN || uMsg == WM_SYSKEYDOWN) {
        TEventUI event = { 0 };
        event.Type = UIEVENT_KEYDOWN;
        event.chKey = (TCHAR)wParam;
        event.wKeyState = MapKeyState();
        event.dwTimestamp = ::GetTickCount();
        m_pOwner->Event(event);
		//m_pOwner->GetManager()->MessageHandler(uMsg, wParam, lParam, lRes);
	}	
	else if( uMsg == WM_KEYUP || uMsg == WM_SYSKEYUP) {
        TEventUI event = { 0 };
        event.Type = UIEVENT_KEYUP;
        event.chKey = (TCHAR)wParam;
        event.wKeyState = MapKeyState();
        event.dwTimestamp = ::GetTickCount();
        m_pOwner->Event(event);
		//m_pOwner->GetManager()->MessageHandler(uMsg, wParam, lParam, lRes);
	}		
	else if( uMsg == OCM__BASE + WM_CTLCOLOREDIT  || uMsg == OCM__BASE + WM_CTLCOLORSTATIC ) {
		if( m_pOwner->GetNativeEditBkColor() == 0xFFFFFFFF ) return NULL;
		::SetBkMode((HDC)wParam, TRANSPARENT);
		DWORD dwTextColor = m_pOwner->GetTextColor();
		::SetTextColor((HDC)wParam, RGB(GetBValue(dwTextColor),GetGValue(dwTextColor),GetRValue(dwTextColor)));
		if( m_hBkBrush == NULL ) {
			DWORD clrColor = m_pOwner->GetNativeEditBkColor();
			m_hBkBrush = ::CreateSolidBrush(RGB(GetBValue(clrColor), GetGValue(clrColor), GetRValue(clrColor)));
		}
		return (LRESULT)m_hBkBrush;
	}
	else bHandled = FALSE;
	if( !bHandled ) return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	return lRes;
}

LRESULT CKeyEditWnd::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT lRes = ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	PostMessage(WM_CLOSE);
	return lRes;
}

LRESULT CKeyEditWnd::OnEditChanged(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	if( !m_bInit ) return 0;
	if( m_pOwner == NULL ) return 0;
	// Copy text back
	int cchLen = ::GetWindowTextLength(m_hWnd) + 1;
	LPTSTR pstr = static_cast<LPTSTR>(_alloca(cchLen * sizeof(TCHAR)));
	ASSERT(pstr);
	if( pstr == NULL ) return 0;
	::GetWindowText(m_hWnd, pstr, cchLen);
	m_pOwner->m_sText = pstr;
	m_pOwner->GetManager()->SendNotify(m_pOwner, DUI_MSGTYPE_TEXTCHANGED);
	return 0;
}


/////////////////////////////////////////////////////////////////////////////////////
//
//

CKeyEditUI::CKeyEditUI():m_pWindow(NULL), m_uMaxChar(255), m_bReadOnly(false), 
	m_bPasswordMode(false), m_cPasswordChar(_T('*')), m_uButtonState(0), 
	m_dwEditbkColor(0xFFFFFFFF), m_iWindowStyls(0)
{
	SetTextPadding(CDuiRect(4, 3, 4, 3));
	SetBkColor(0xFFFFFFFF);
}

LPCTSTR CKeyEditUI::GetClass() const
{
	return _T("CKeyEditUI");
}

LPVOID CKeyEditUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcscmp(pstrName, DUI_CTR_EDIT) == 0 ) return static_cast<CKeyEditUI*>(this);
	return CLabelUI::GetInterface(pstrName);
}

UINT CKeyEditUI::GetControlFlags() const
{
	if( !IsEnabled() ) return CControlUI::GetControlFlags();

	return UIFLAG_SETCURSOR | UIFLAG_TABSTOP;
}


void CKeyEditUI::DoEvent(TEventUI& event)
{
	if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
		if( m_pParent != NULL ) m_pParent->DoEvent(event);
		else CLabelUI::DoEvent(event);
		return;
	}
	if( event.Type == UIEVENT_KEYDOWN )
	{
		if(!((event.wKeyState & MK_CONTROL) | 
			 (event.wKeyState & MK_ALT) |
			 (event.wKeyState & MK_SHIFT)))
		{
			return;
		}	

		std::string keyStr;
		if(event.wKeyState & MK_CONTROL)
		{
			keyStr += "Ctl+";
		}
		if(event.wKeyState & MK_ALT)	
		{
			keyStr += "Al+";
		}		
		if(event.wKeyState & MK_SHIFT)
		{
			keyStr += "Sht+";
		}
		if(event.chKey == VK_CONTROL||
			event.chKey == VK_SHIFT||
			event.chKey == VK_MENU)
		{
			SetText((LPCTSTR)keyStr.c_str());
			return;
		}		

		std::string tmpKey = ModifierKeyParser::KeyValToStr((int)event.chKey);
		if(!tmpKey.empty())
		{
			keyStr += tmpKey;
			SetKey((LPCTSTR)keyStr.c_str());
			SetText((LPCTSTR)keyStr.c_str());
		}
		return;
	}
	if( event.Type == UIEVENT_KEYUP )
	{
		if(!((event.wKeyState & MK_CONTROL) | 
			 (event.wKeyState & MK_ALT) |
			 (event.wKeyState & MK_SHIFT)))
		{
			SetText((LPCTSTR)mKeyStr.c_str());
			return;
		}		
		std::string keyStr;
		if(event.wKeyState & MK_CONTROL)
		{
			keyStr += "Ctl+";
		}
		if(event.wKeyState & MK_ALT)	
		{
			keyStr += "Al+";
		}		
		if(event.wKeyState & MK_SHIFT)
		{
			keyStr += "Sht+";
		}
		if(event.chKey == VK_CONTROL||
			event.chKey == VK_SHIFT||
			event.chKey == VK_MENU)
		{
			SetText((LPCTSTR)keyStr.c_str());
			return;
		}		

		std::string tmpKey = ModifierKeyParser::KeyValToStr(event.chKey);
		if(!tmpKey.empty())
		{
			keyStr += tmpKey;
			SetKey((LPCTSTR)keyStr.c_str());
			SetText((LPCTSTR)keyStr.c_str());
		}
		return;
	}	
	if( event.Type == UIEVENT_SETCURSOR && IsEnabled() )
	{
		::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_IBEAM)));
		return;
	}
	if( event.Type == UIEVENT_WINDOWSIZE )
	{
		if( m_pWindow != NULL ) m_pManager->SetFocusNeeded(this);
	}
	if( event.Type == UIEVENT_SCROLLWHEEL )
	{
		if( m_pWindow != NULL ) return;
	}
	if( event.Type == UIEVENT_SETFOCUS && IsEnabled() ) 
	{
		if( m_pWindow ) return;
		m_pWindow = new CKeyEditWnd();
		ASSERT(m_pWindow);
		m_pWindow->Init(this);
		Invalidate();
	}
	if( event.Type == UIEVENT_KILLFOCUS && IsEnabled() ) 
	{
		Invalidate();
	}
	if( event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK || event.Type == UIEVENT_RBUTTONDOWN) 
	{
		if( IsEnabled() ) {
			GetManager()->ReleaseCapture();
			if( IsFocused() && m_pWindow == NULL )
			{
				m_pWindow = new CKeyEditWnd();
				ASSERT(m_pWindow);
				m_pWindow->Init(this);

				if( PtInRect(&m_rcItem, event.ptMouse) )
				{
					int nSize = GetWindowTextLength(*m_pWindow);
					if( nSize == 0 )
						nSize = 1;

					Edit_SetSel(*m_pWindow, 0, nSize);
				}
			}
			else if( m_pWindow != NULL )
			{
#if 1
				int nSize = GetWindowTextLength(*m_pWindow);
				if( nSize == 0 )
					nSize = 1;

				Edit_SetSel(*m_pWindow, 0, nSize);
#else
				POINT pt = event.ptMouse;
				pt.x -= m_rcItem.left + m_rcTextPadding.left;
				pt.y -= m_rcItem.top + m_rcTextPadding.top;
				::SendMessage(*m_pWindow, WM_LBUTTONDOWN, event.wParam, MAKELPARAM(pt.x, pt.y));
#endif
			}
		}
		return;
	}
	if( event.Type == UIEVENT_MOUSEMOVE ) 
	{
		return;
	}
	if( event.Type == UIEVENT_BUTTONUP ) 
	{
		return;
	}
	if( event.Type == UIEVENT_CONTEXTMENU )
	{
		return;
	}
	if( event.Type == UIEVENT_MOUSEENTER )
	{
		if( IsEnabled() ) {
			m_uButtonState |= UISTATE_HOT;
			Invalidate();
		}
		return;
	}
	if( event.Type == UIEVENT_MOUSELEAVE )
	{
		if( IsEnabled() ) {
			m_uButtonState &= ~UISTATE_HOT;
			Invalidate();
		}
		return;
	}
	CLabelUI::DoEvent(event);
}

void CKeyEditUI::SetEnabled(bool bEnable)
{
	CControlUI::SetEnabled(bEnable);
	if( !IsEnabled() ) {
		m_uButtonState = 0;
	}
}

void CKeyEditUI::SetKey(LPCTSTR pstrText)
{
	mKeyStr = static_cast<std::string>(pstrText);
}
std::string& CKeyEditUI::GetKey()
{
	return mKeyStr;
}
void CKeyEditUI::SetText(LPCTSTR pstrText)
{
	m_sText = pstrText;
	if( m_pWindow != NULL ) Edit_SetText(*m_pWindow, m_sText);
	Invalidate();
}

void CKeyEditUI::SetMaxChar(UINT uMax)
{
	m_uMaxChar = uMax;
	if( m_pWindow != NULL ) Edit_LimitText(*m_pWindow, m_uMaxChar);
}

UINT CKeyEditUI::GetMaxChar()
{
	return m_uMaxChar;
}

void CKeyEditUI::SetReadOnly(bool bReadOnly)
{
	if( m_bReadOnly == bReadOnly ) return;

	m_bReadOnly = bReadOnly;
	if( m_pWindow != NULL ) Edit_SetReadOnly(*m_pWindow, m_bReadOnly);
	Invalidate();
}

bool CKeyEditUI::IsReadOnly() const
{
	return m_bReadOnly;
}

void CKeyEditUI::SetNumberOnly(bool bNumberOnly)
{
	if( bNumberOnly )
	{
		m_iWindowStyls |= ES_NUMBER;
	}
	else
	{
		m_iWindowStyls |= ~ES_NUMBER;
	}
}

bool CKeyEditUI::IsNumberOnly() const
{
	return m_iWindowStyls&ES_NUMBER ? true:false;
}

int CKeyEditUI::GetWindowStyls() const 
{
	return m_iWindowStyls;
}

void CKeyEditUI::SetPasswordMode(bool bPasswordMode)
{
	if( m_bPasswordMode == bPasswordMode ) return;
	m_bPasswordMode = bPasswordMode;
	Invalidate();
}

bool CKeyEditUI::IsPasswordMode() const
{
	return m_bPasswordMode;
}

void CKeyEditUI::SetPasswordChar(TCHAR cPasswordChar)
{
	if( m_cPasswordChar == cPasswordChar ) return;
	m_cPasswordChar = cPasswordChar;
	if( m_pWindow != NULL ) Edit_SetPasswordChar(*m_pWindow, m_cPasswordChar);
	Invalidate();
}

TCHAR CKeyEditUI::GetPasswordChar() const
{
	return m_cPasswordChar;
}

LPCTSTR CKeyEditUI::GetNormalImage()
{
	return m_sNormalImage;
}

void CKeyEditUI::SetNormalImage(LPCTSTR pStrImage)
{
	m_sNormalImage = pStrImage;
	Invalidate();
}

LPCTSTR CKeyEditUI::GetHotImage()
{
	return m_sHotImage;
}

void CKeyEditUI::SetHotImage(LPCTSTR pStrImage)
{
	m_sHotImage = pStrImage;
	Invalidate();
}

LPCTSTR CKeyEditUI::GetFocusedImage()
{
	return m_sFocusedImage;
}

void CKeyEditUI::SetFocusedImage(LPCTSTR pStrImage)
{
	m_sFocusedImage = pStrImage;
	Invalidate();
}

LPCTSTR CKeyEditUI::GetDisabledImage()
{
	return m_sDisabledImage;
}

void CKeyEditUI::SetDisabledImage(LPCTSTR pStrImage)
{
	m_sDisabledImage = pStrImage;
	Invalidate();
}

void CKeyEditUI::SetNativeEditBkColor(DWORD dwBkColor)
{
	m_dwEditbkColor = dwBkColor;
}

DWORD CKeyEditUI::GetNativeEditBkColor() const
{
	return m_dwEditbkColor;
}

void CKeyEditUI::SetSel(long nStartChar, long nEndChar)
{
	if( m_pWindow != NULL ) Edit_SetSel(*m_pWindow, nStartChar,nEndChar);
}

void CKeyEditUI::SetSelAll()
{
	SetSel(0,-1);
}

void CKeyEditUI::SetReplaceSel(LPCTSTR lpszReplace)
{
	if( m_pWindow != NULL ) Edit_ReplaceSel(*m_pWindow, lpszReplace);
}

void CKeyEditUI::SetPos(RECT rc)
{
	CControlUI::SetPos(rc);
	if( m_pWindow != NULL ) {
		RECT rcPos = m_pWindow->CalPos();
		::SetWindowPos(m_pWindow->GetHWND(), NULL, rcPos.left, rcPos.top, rcPos.right - rcPos.left, 
			rcPos.bottom - rcPos.top, SWP_NOZORDER | SWP_NOACTIVATE);        
	}
}

void CKeyEditUI::SetVisible(bool bVisible)
{
	CControlUI::SetVisible(bVisible);
	if( !IsVisible() && m_pWindow != NULL ) m_pManager->SetFocus(NULL);
}

void CKeyEditUI::SetInternVisible(bool bVisible)
{
	if( !IsVisible() && m_pWindow != NULL ) m_pManager->SetFocus(NULL);
}

SIZE CKeyEditUI::EstimateSize(SIZE szAvailable)
{
	if( m_cxyFixed.cy == 0 ) return CSize(m_cxyFixed.cx, m_pManager->GetFontInfo(GetFont())->tm.tmHeight + 6);
	return CControlUI::EstimateSize(szAvailable);
}

void CKeyEditUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if( _tcscmp(pstrName, _T("readonly")) == 0 ) SetReadOnly(_tcscmp(pstrValue, _T("true")) == 0);
	else if( _tcscmp(pstrName, _T("numberonly")) == 0 ) SetNumberOnly(_tcscmp(pstrValue, _T("true")) == 0);
	else if( _tcscmp(pstrName, _T("password")) == 0 ) SetPasswordMode(_tcscmp(pstrValue, _T("true")) == 0);
	else if( _tcscmp(pstrName, _T("maxchar")) == 0 ) SetMaxChar(_ttoi(pstrValue));
	else if( _tcscmp(pstrName, _T("normalimage")) == 0 ) SetNormalImage(pstrValue);
	else if( _tcscmp(pstrName, _T("hotimage")) == 0 ) SetHotImage(pstrValue);
	else if( _tcscmp(pstrName, _T("focusedimage")) == 0 ) SetFocusedImage(pstrValue);
	else if( _tcscmp(pstrName, _T("disabledimage")) == 0 ) SetDisabledImage(pstrValue);
	else if( _tcscmp(pstrName, _T("nativebkcolor")) == 0 ) {
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetNativeEditBkColor(clrColor);
	}
	else CLabelUI::SetAttribute(pstrName, pstrValue);
}

void CKeyEditUI::PaintStatusImage(HDC hDC)
{
	if( IsFocused() ) m_uButtonState |= UISTATE_FOCUSED;
	else m_uButtonState &= ~ UISTATE_FOCUSED;
	if( !IsEnabled() ) m_uButtonState |= UISTATE_DISABLED;
	else m_uButtonState &= ~ UISTATE_DISABLED;

	if( (m_uButtonState & UISTATE_DISABLED) != 0 ) {
		if( !m_sDisabledImage.IsEmpty() ) {
			if( !DrawImage(hDC, (LPCTSTR)m_sDisabledImage) ) m_sDisabledImage.Empty();
			else return;
		}
	}
	else if( (m_uButtonState & UISTATE_FOCUSED) != 0 ) {
		if( !m_sFocusedImage.IsEmpty() ) {
			if( !DrawImage(hDC, (LPCTSTR)m_sFocusedImage) ) m_sFocusedImage.Empty();
			else return;
		}
	}
	else if( (m_uButtonState & UISTATE_HOT) != 0 ) {
		if( !m_sHotImage.IsEmpty() ) {
			if( !DrawImage(hDC, (LPCTSTR)m_sHotImage) ) m_sHotImage.Empty();
			else return;
		}
	}

	if( !m_sNormalImage.IsEmpty() ) {
		if( !DrawImage(hDC, (LPCTSTR)m_sNormalImage) ) m_sNormalImage.Empty();
		else return;
	}
}

void CKeyEditUI::PaintText(HDC hDC)
{
	if( m_dwTextColor == 0 ) m_dwTextColor = m_pManager->GetDefaultFontColor();
	if( m_dwDisabledTextColor == 0 ) m_dwDisabledTextColor = m_pManager->GetDefaultDisabledColor();

	if( m_sText.IsEmpty() ) return;

	CDuiString sText = m_sText;
	if( m_bPasswordMode ) {
		sText.Empty();
		LPCTSTR p = m_sText.GetData();
		while( *p != _T('\0') ) {
			sText += m_cPasswordChar;
			p = ::CharNext(p);
		}
	}

	RECT rc = m_rcItem;
	rc.left += m_rcTextPadding.left;
	rc.right -= m_rcTextPadding.right;
	rc.top += m_rcTextPadding.top;
	rc.bottom -= m_rcTextPadding.bottom;
	if( IsEnabled() ) {
		CRenderEngine::DrawText(hDC, m_pManager, rc, sText, m_dwTextColor, \
			m_iFont, DT_SINGLELINE | m_uTextStyle);
	}
	else {
		CRenderEngine::DrawText(hDC, m_pManager, rc, sText, m_dwDisabledTextColor, \
			m_iFont, DT_SINGLELINE | m_uTextStyle);

	}
}
