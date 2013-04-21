#ifndef _OPTIONDLGMODULE_H
#define _OPTIONDLGMODULE_H
//#include "MainHead.h"
#include "OfficeVolModule.h"
#include <UIlib.h>
#include "KeyEditUI.h"
using namespace DuiLib;

#define MSG_DISMISS_OPTION   WM_USER + 122     // ¼üÅÌÏûÏ¢

const TCHAR* const kMainTabCtrl      = _T("tab_main");
const TCHAR* const kGeneralTableCtrl = _T("tab_main_general");

const TCHAR* const kTabGeneral   = _T("tab_general");
const TCHAR* const kTabAbout   	 = _T("tab_about");
const TCHAR* const kGTabKeyboard = _T("gtab_key");
const TCHAR* const kGTabCommon   = _T("gtab_common");

class OptionDlgModule: public CWindowWnd, public INotifyUI, public IDialogBuilderCallback
{
public:
	OptionDlgModule(HWND parent_hwnd);
	virtual ~OptionDlgModule(void);

    LPCTSTR GetWindowClassName() const; 

    void OnFinalMessage(HWND hWnd);

    void Init();
    void Notify(TNotifyUI& msg);

    LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	CControlUI* CreateControl(LPCTSTR pstrClass);
    LRESULT UpdateConfig();
	LRESULT ReadConfig();

public:
    CPaintManagerUI m_pm;

private:
	HWND mParentHwnd;
	std::vector<CKeyEditUI*> mKeyEditVec;
};

#endif
