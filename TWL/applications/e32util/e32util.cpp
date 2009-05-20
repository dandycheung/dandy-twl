#include "..\..\include\dlg.h"
#include "..\..\include\msgmap.h"
#include "..\..\include\stdctl.h"
#include "..\..\include\cmndlg.h"
#include "..\..\include\modutil.h"
#include "..\..\include\util\filemapping.h"
#include "..\..\include\util\numutil.h"
#include "f32image.h"
#include "e32uid.h"
#include "resource.h"

using namespace TWL;

const UINT IDC_CHK_BASE = IDC_CHK_TCB;

class CE32CustomData
{
public:
	TUint32 uid1;
	TUint32 uid2;
	TUint32 uid3;
	TUint32 secureId;
	TUint32 vonderId;
	TUint32 iCaps[SCapabilitySet::ENCapW];

	CE32CustomData()
	{
		ZeroMemory(this, sizeof(*this));
	}
	
	bool UpdateData(E32ImageHeader* phdr)
	{
		if(!phdr || !this)
			return false;

		// 1. update all specified fields
		phdr->iUid1 = uid1;
		phdr->iUid2 = uid2;
		phdr->iUid3 = uid3;

		PBYTE p = (PBYTE)phdr;
		TInt hdrLength;
		if((phdr->iFlags & KImageHdrFmtMask) == KImageHdrFmt_V)
		{
			E32ImageHeaderV* phdrv = (E32ImageHeaderV*)phdr;

			hdrLength = phdr->iCodeOffset;	// sizeof(E32ImageHeaderV);

			phdrv->iS.iSecureId = secureId;
			phdrv->iS.iVendorId = vonderId;
			
			for(int i=0; i<SCapabilitySet::ENCapW; i++)
				phdrv->iS.iCaps.iCaps[i] = iCaps[i];
		}
		else if((phdr->iFlags & KImageHdrFmtMask) == KImageHdrFmt_J)
			hdrLength = sizeof(E32ImageHeaderComp);
		else
			hdrLength = sizeof(E32ImageHeader);

		// 2. re-calculate uid-crc and header-crc
		TUint uids[3] = { uid1, uid2, uid3 };
		TCheckedUidX x(uids);
		phdr->iUidChecksum = x.CRC();

		TUint32 crc = 0;
		phdr->iHeaderCrc = KImageCrcInitialiser;
		Crc::Calculate(crc, phdr, hdrLength);
		phdr->iHeaderCrc = crc;

		return true;
	}
};

class CMainDlg : public CDialog
{
public:
	CMainDlg()
		: _pfm(NULL)
	{
	}

	~CMainDlg()
	{
		if(_pfm)
			delete _pfm;
	}

	LPCTSTR GetDialogTemplate()
	{
		return MAKEINTRESOURCE(IDD_MAIN);
	}

	BEGIN_MSG_MAP()
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDC_BTN_BROWSE, BN_CLICKED, OnBrowseClicked)
		COMMAND_HANDLER(IDC_BTN_OPEN, BN_CLICKED, OnOpenClicked)
		COMMAND_HANDLER(IDC_BTN_UPDATE, BN_CLICKED, OnUpdateClicked)
		// COMMAND_RANGE_HANDLER(IDC_CHK_TCB, IDC_CHK_USERENVIRONMENT, OnCheckClicked)
		CHAIN_MSG_MAP(CDialog)
	END_MSG_MAP()

	bool OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult)
	{
		HICON hIcon = LoadIcon(GetModuleInstance(), MAKEINTRESOURCE(IDI_MAIN));
		SetIcon(hIcon, FALSE);
		SetIcon(hIcon, TRUE);

		CenterWindow();

		return true;
	}

	bool OnBrowseClicked(WORD wNotifyCode, WORD wID, HWND hWndCtl, LRESULT& lResult)
	{
		CFileDialog fileDlg(TRUE, NULL, NULL, 0, NULL, *this);
		if(fileDlg.DoModal() == IDOK)
		{
			CWindow edt = GetDlgItem(IDC_EDT_E32FILE);
			edt.SetWindowText(fileDlg.m_szFileName);
		}

		return true;
	}

	bool OnOpenClicked(WORD wNotifyCode, WORD wID, HWND hWndCtl, LRESULT& lResult)
	{
		// 1. check if there is a file name or not, and if the target file is a valid E32 file
		TCHAR szFileName[MAX_PATH];
		CWindow edt = GetDlgItem(IDC_EDT_E32FILE);
		edt.GetWindowText(szFileName, MAX_PATH);

		CFileMapping* pfm = new CFileMapping(szFileName);
		if(!pfm->IsMapped())
		{
			MessageBox(TEXT("Please specify a valid file."));
			delete pfm;
			return true;
		}

		E32ImageHeader* phdr = (E32ImageHeader*)pfm->GetView();
		if(phdr->iSignature != 0x434f5045) // 'EPOC'
		{
			MessageBox(TEXT("Please specify a valid E32 file."));
			delete pfm;
			return true;
		}

		// 2. check if there is a opening file already, and if any changes should be saved or discarded
		if(_pfm)
		{
			delete _pfm;
			_pfm = NULL;
		}

		_pfm = pfm;

		// 3. update the view
		UpdateView(phdr);
		
		return true;
	}
	
	bool OnUpdateClicked(WORD wNotifyCode, WORD wID, HWND hWndCtl, LRESULT& lResult)
	{
		if(!_pfm)
		{
			MessageBox(TEXT("Please open a valid E32 file first."));
			return true;
		}

		E32ImageHeader* phdr = (E32ImageHeader*)_pfm->GetView();

		TCHAR szId[0x20];
		CWindow wnd;

		CE32CustomData data;
		DWORD dwUid;

		// 1. get all ids
		wnd = GetDlgItem(IDC_EDT_UID1);
		wnd.GetWindowText(szId, _countof(szId));
		if(NumUtil::StringToDWORD(szId, dwUid))
			data.uid1 = dwUid;

		wnd = GetDlgItem(IDC_EDT_UID2);
		wnd.GetWindowText(szId, _countof(szId));
		if(NumUtil::StringToDWORD(szId, dwUid))
			data.uid2 = dwUid;

		wnd = GetDlgItem(IDC_EDT_UID3);
		wnd.GetWindowText(szId, _countof(szId));
		if(NumUtil::StringToDWORD(szId, dwUid))
			data.uid3 = dwUid;

		wnd = GetDlgItem(IDC_EDT_SECUREID);
		wnd.GetWindowText(szId, _countof(szId));
		if(NumUtil::StringToDWORD(szId, dwUid))
			data.secureId = dwUid;

		wnd = GetDlgItem(IDC_EDT_VENDORID);
		wnd.GetWindowText(szId, _countof(szId));
		if(NumUtil::StringToDWORD(szId, dwUid))
			data.vonderId = dwUid;

		// 2. get all caps
		CButton btn;
		int iCheck;
		for(int i=0; i<ECapability_Limit; i++)
		{
			btn = GetDlgItem(IDC_CHK_BASE + i);
			iCheck = !!btn.GetCheck();

			data.iCaps[i / 32] |= (iCheck << (i % 32));
		}

		data.UpdateData(phdr);
		UpdateView(phdr);

		return true;
	}

protected:
	bool UpdateView(E32ImageHeader* phdr)
	{
		if(!phdr)
			return false;

		CWindow wnd;

		TCHAR szUid[0x10];

		wsprintf(szUid, TEXT("0x%08X"), phdr->iUid1);
		wnd = GetDlgItem(IDC_EDT_UID1);
		wnd.SetWindowText(szUid);
		
		wsprintf(szUid, TEXT("0x%08X"), phdr->iUid2);
		wnd = GetDlgItem(IDC_EDT_UID2);
		wnd.SetWindowText(szUid);
		
		wsprintf(szUid, TEXT("0x%08X"), phdr->iUid3);
		wnd = GetDlgItem(IDC_EDT_UID3);
		wnd.SetWindowText(szUid);

#ifdef _DEBUG
		TUint uids[3] = { phdr->iUid1, phdr->iUid2, phdr->iUid3 };
		TCheckedUidX x(uids);
		TUint crc = x.CRC();
		ASSERT(crc == phdr->iUidChecksum);
#endif

		if((phdr->iFlags & KImageHdrFmtMask) == KImageHdrFmt_V)
		{
			E32ImageHeaderV* phdrv = (E32ImageHeaderV*)phdr;

			TUint32 id;
			id = phdrv->iS.iSecureId;
			wsprintf(szUid, TEXT("0x%08X"), id);
			wnd = GetDlgItem(IDC_EDT_SECUREID);
			wnd.SetWindowText(szUid);
			wnd.EnableWindow();

			id = phdrv->iS.iVendorId;
			wsprintf(szUid, TEXT("0x%08X"), id);
			wnd = GetDlgItem(IDC_EDT_VENDORID);
			wnd.SetWindowText(szUid);
			wnd.EnableWindow();

			UpdateCapabilities(&phdrv->iS.iCaps);

			// enable all capability fields
			for(int i=0; i<ECapability_Limit; i++)
			{
				wnd = GetDlgItem(IDC_CHK_BASE + i);
				wnd.EnableWindow();
			}
		}
		else
		{
			wnd = GetDlgItem(IDC_EDT_SECUREID);
			wnd.SetWindowText(TEXT(""));
			wnd.EnableWindow(FALSE);

			wnd = GetDlgItem(IDC_EDT_VENDORID);
			wnd.SetWindowText(TEXT(""));
			wnd.EnableWindow(FALSE);

			// disable all capability fields
			CButton btn;
			for(int i=0; i<ECapability_Limit; i++)
			{
				btn = GetDlgItem(IDC_CHK_BASE + i);
				btn.SetCheck(0);
				btn.EnableWindow(FALSE);
			}
		}

		return true;
	}

	bool UpdateCapabilities(SCapabilitySet* pcaps)
	{
		if(!pcaps)
			return false;

		CButton btn;
		for(int i=0; i<ECapability_Limit; i++)
		{
			btn = GetDlgItem(IDC_CHK_BASE + i);
			if(pcaps->iCaps[i / 32] & (1 << (i % 32)))
				btn.SetCheck(1);
			else
				btn.SetCheck(0);
		}

		return true;
	}

protected:
	CFileMapping* _pfm;
};

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR pszCmdLine, int nShowCmd)
{
	/// TODO: command line support?

	CMainDlg mainDlg;
	return mainDlg.DoModal();
}
