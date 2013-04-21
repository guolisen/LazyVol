#ifndef KEY_EDIT_UI_H
#define KEY_EDIT_UI_H
#include <string>
#include <UIlib.h>
using namespace DuiLib;


class CKeyEditWnd;

class CKeyEditUI : public CLabelUI
{
	friend class CKeyEditWnd;
public:
	CKeyEditUI();

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);
	UINT GetControlFlags() const;

	void SetEnabled(bool bEnable = true);
	void SetText(LPCTSTR pstrText);
	void SetMaxChar(UINT uMax);
	UINT GetMaxChar();
	void SetReadOnly(bool bReadOnly);
	bool IsReadOnly() const;
	void SetPasswordMode(bool bPasswordMode);
	bool IsPasswordMode() const;
	void SetPasswordChar(TCHAR cPasswordChar);
	TCHAR GetPasswordChar() const;
	void SetNumberOnly(bool bNumberOnly);
	bool IsNumberOnly() const;
	int GetWindowStyls() const;

	LPCTSTR GetNormalImage();
	void SetNormalImage(LPCTSTR pStrImage);
	LPCTSTR GetHotImage();
	void SetHotImage(LPCTSTR pStrImage);
	LPCTSTR GetFocusedImage();
	void SetFocusedImage(LPCTSTR pStrImage);
	LPCTSTR GetDisabledImage();
	void SetDisabledImage(LPCTSTR pStrImage);
	void SetNativeEditBkColor(DWORD dwBkColor);
	DWORD GetNativeEditBkColor() const;

	void SetSel(long nStartChar, long nEndChar);
	void SetSelAll();
	void SetReplaceSel(LPCTSTR lpszReplace);

	void SetPos(RECT rc);
	void SetVisible(bool bVisible = true);
	void SetInternVisible(bool bVisible = true);
	SIZE EstimateSize(SIZE szAvailable);
	void DoEvent(TEventUI& event);
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	void PaintStatusImage(HDC hDC);
	void PaintText(HDC hDC);
	void SetKey(LPCTSTR pstrText);
	std::string& GetKey();

protected:
	CKeyEditWnd* m_pWindow;

	UINT m_uMaxChar;
	bool m_bReadOnly;
	bool m_bPasswordMode;
	TCHAR m_cPasswordChar;
	UINT m_uButtonState;
	CDuiString m_sNormalImage;
	CDuiString m_sHotImage;
	CDuiString m_sFocusedImage;
	CDuiString m_sDisabledImage;
	DWORD m_dwEditbkColor;
	int m_iWindowStyls;
	std::string mKeyStr;
};

#endif
