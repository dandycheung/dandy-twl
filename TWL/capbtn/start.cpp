// testWnd.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "CaptionButton\CaptionButton.h"
#include "CaptionButton\drawutil.h"
#include "menupin.h"
#include "hotkey.h"

#include "resource.h"

// namespace - Tiny Window Library
using namespace TWL;

#define IDM_SWITCHONTOP		7

// #pragma comment(linker, "-align:4096")
// #pragma comment(linker, "-merge:.rdata=.text")

class CAboutDlg : public CDialog
{
	BEGIN_MSG_MAP()
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		CHAIN_MSG_MAP(CDialog)
	END_MSG_MAP()

	bool OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult)
	{
		CenterWindow();
		return CDialog::ProcessMessage(uMsg, wParam, lParam, lResult);
	}

public:
	LPCTSTR GetDialogTemplate()
	{
		return MAKEINTRESOURCE(IDD_ABOUT);
	}
};

class CMainWindow : public CUserWindow
{
	DECLARE_CLASS_NAME(_T("Test"));

public:
	CMainWindow()
	{
	}

	virtual ~CMainWindow()
	{
	}

	BEGIN_MSG_MAP()
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_COMMAND, OnCommand)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_NCDESTROY, OnNcDestroy)
		MESSAGE_HANDLER(WM_SYSCOMMAND, OnSysCommand)
		MESSAGE_HANDLER(WM_MEASUREITEM, OnMeasureItem)
		MESSAGE_HANDLER(WM_DRAWITEM, OnDrawItem)
	END_MSG_MAP()

	CCaptionButton m_captionButtons;
	CMenuPin m_pin;
	bool OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult)
	{
#if 1	
		CBitmap bmpGlyph;
		bmpGlyph.LoadBitmap(IDB_DANDY);
		
		m_captionButtons.CreateCaptionFrame(*this);
		m_captionButtons.AddButton(CRect(0, 0, 100, 0), 1, CBNBKGNDSTYLE_CLOSE, TRUE);

		CCaptionButton* pButton = m_captionButtons.GetButton(1);
		if(pButton)
		{
			pButton->SetGlyphBitmap(bmpGlyph);
#ifdef USE_TOOLTIP_SUPPORT
			pButton->SetTooltipText(TEXT("Double-click to quit"));
#endif // USE_TOOLTIP_SUPPORT
		}
#endif // 0 or 1

		SetMenu(CreateMenu());
		m_pin.Install(m_hWnd, GetMenu(), IDM_SWITCHONTOP);
		m_pin.SetBitmap(GetResourceInstance(), IDB_PIN);

		CreateDandyHotKey(WS_CHILD | WS_VISIBLE, 50, 50, 200, 24, *this, 0, GetModuleInstance());

		return false;
	}
	
	bool OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult)
	{
		if(wParam == SC_NOTIFY)
		{
			int iNotifyCode = (int)(short)HIWORD(lParam);
			int iCmdid = (int)(short)LOWORD(lParam);
			
			if(iNotifyCode == NM_CLICK)
				; // MessageBox(TEXT("Clicked!"));
			else if(iNotifyCode == NM_DBLCLK)
				MessageBox(TEXT("Double Clicked!"));
			else if(iNotifyCode == NM_RCLICK)
				; // MessageBox(TEXT("Right Clicked!"));
			else if(iNotifyCode == NM_RDBLCLK)
				MessageBox(TEXT("Right Double Clicked!"));
			
			return true;
		}
		
		return false;
	}

	bool OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult)
	{
		CPaintDC dc(m_hWnd);

		lResult = 0;
		return true;
	}

	bool OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult)
	{
		MPX_WM_COMMAND(wParam, lParam, iCtrlId, hwndCtrl, iCmdId);

		return m_pin.OnCommand(uMsg, wParam, lParam, lResult);
	}

	bool OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult)
	{
		MPX_WM_SIZE(wParam, lParam, uType, cx, cy);

		return false;
	}

	bool OnNcDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult)
	{
		PostQuitMessage(0);
		lResult = 0;

		return false;
	}

	bool OnMeasureItem(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult)
	{
		return m_pin.OnMeasureItem(uMsg, wParam, lParam, lResult);
	}

	bool OnDrawItem(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult)
	{
		return m_pin.OnDrawItem(uMsg, wParam, lParam, lResult);
	}
	
	void SetWndClassEx(WNDCLASSEX& wce)
	{
		CUserWindow::SetWndClassEx(wce);
		wce.hIconSm = LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION));
	}
};

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	// InitCommonControls();
	InitHotKeyClass(hInstance);

	RECT rc = { 0, 0, 320, 240 };

	CMainWindow wndMain;
	wndMain.Create(0, _T("Hello, TWL!"), WS_OVERLAPPEDWINDOW, &rc/*CWindow::rcDefault*/, NULL, NULL, hInstance);
	wndMain.CenterWindow();
	wndMain.ShowWindow(SW_SHOW);

	CMessageLoop msgloop;
	return msgloop.Run();
}
