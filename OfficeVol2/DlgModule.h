#ifndef DLG_MODULE_H
#define DLG_MODULE_H

//#include "MainHead.h"
#include "OfficeVolModule.h"
#include <UIlib.h>
using namespace DuiLib;



typedef std::map<ATOM, std::string> HOTKEY_MAP;

class VolMainForm : public CWindowWnd, public INotifyUI, public OfficeVolDelegate
{
public:
    VolMainForm():mIsShow(false),mIsOptionShow(false) {};
	virtual ~VolMainForm();

    LPCTSTR GetWindowClassName() const; 

	
    void OnFinalMessage(HWND hWnd);

    void Init();
    void OnPrepare(TNotifyUI& msg);
    void Notify(TNotifyUI& msg);

    LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOptDismiss(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnHotKey(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	LRESULT SetVolText(DWORD vol_level);
	LRESULT SetVolLogo(DWORD vol_level);
	LRESULT ShowPad();
	LRESULT HidePad();
	LRESULT ShowOption();

	LRESULT RegisterHotkey(const std::string& hotkey);
	void    UnRegisterHotkey(const std::string& hotkey);	
	
public:
	virtual int UiUpdate(const std::string& action);
	virtual int ReConfig(OFF_CONFIG_CLASS config_class, 
			                const std::string& value, 
			                const std::string& old_value);	
	virtual void UnInitOffVol();
public:
    CPaintManagerUI m_pm;

private:
	CProgressUI* mProgress;
	CLabelUI*    mVolLogo;
	CTextUI*     mVolName;
	BOOL  		 mIsShow;
	HOTKEY_MAP   mHotkeyMap; 
	bool	     mIsOptionShow;
    //...
};



#endif
