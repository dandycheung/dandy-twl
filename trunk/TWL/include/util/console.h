#ifndef __CONSOLE_H__
#define __CONSOLE_H__

class CConsole
{
public:
	CConsole() 
	{ 
		hConsole = NULL; 
	}

public:
	// create the console
	bool Create(LPCTSTR pszTitle, bool bNoClose = false);
	
	// set color for output
	void Color(WORD wColor = NULL);
	// write output to console
	void Output(LPCTSTR pszOutput = NULL, ...);
	
	// set and get title of console
	void SetTitle(LPCTSTR pszTitle);
	LPCTSTR GetTitle();

	// get HWND and/or HANDLE of console
	HWND GetHWND();
	HANDLE GetHandle();

	// show/hide the console
	void Show(bool bShow = true);
	// disable the [x] button of the console
	void DisableClose();
	// clear all output
	void Clear();

	// close the console and delete it
	void Close();

private:
	HANDLE hConsole;
};

#endif // __CONSOLE_H__
