#include <windows.h>
#include <aclapi.h>

class CObjAccessChanger
{
	class CLocalFreer
	{
		HLOCAL _handle;

	public:
		CLocalFreer(HANDLE handle)
			: _handle(handle)
		{
		}

		CLocalFreer(PACL pacl)
			: _handle((HLOCAL)pacl)
		{
		}

		/*
		// we do not need this because both of the HANDLE and PSECURITY_DESCRIPTOR are PVOID indeed
		CLocalFreer(PSECURITY_DESCRIPTOR psd)
			: _handle((HLOCAL)psd)
		{
		}
		// */

		~CLocalFreer()
		{
			if(_handle)
				LocalFree(_handle);
		}
	};

	typedef CLocalFreer CAclCleaner;
	typedef CLocalFreer CSdCleaner;

public:
	static BOOL AllowAccess(LPCTSTR pszObjectName, SE_OBJECT_TYPE ObjectType,
		LPCTSTR pszAccesser, DWORD AccessMode, BOOL bMergeOldDacl)	// AccessMode: ACCESS_MODE, REGSAM, and so on
	{
		// ����һ���յ� ACL
		PACL OldDacl = NULL;
		if(bMergeOldDacl && SetEntriesInAcl(0, NULL, NULL, &OldDacl) != ERROR_SUCCESS)
			return FALSE;

		CAclCleaner ac1(OldDacl);

		// ��ȡ���е� ACL �б� OldDacl:
		PSECURITY_DESCRIPTOR psd = NULL;
		if(bMergeOldDacl && GetNamedSecurityInfo((LPTSTR)pszObjectName, ObjectType, DACL_SECURITY_INFORMATION,
			NULL, NULL, &OldDacl, NULL, &psd) != ERROR_SUCCESS)
			return FALSE;

		CSdCleaner sc1(psd);

		PACL NewDacl = NULL;
		if(SetEntriesInAcl(0, NULL, NULL, &NewDacl) != ERROR_SUCCESS)
			return FALSE;

		CAclCleaner ac2(NewDacl);

		// �����û� pszAccesser ��ָ���ļ����в���Ȩ AccessMode ���ṹ ea
		EXPLICIT_ACCESS ea;
		ZeroMemory(&ea, sizeof(EXPLICIT_ACCESS));

		BuildExplicitAccessWithName(&ea,
			(LPTSTR)pszAccesser,	// name of trustee
			AccessMode,				// type of access
			SET_ACCESS,				// access mode
			SUB_CONTAINERS_AND_OBJECTS_INHERIT); // �Ӽ��̳�����Ȩ��
    
		// �ϲ��ṹ ea �� OldDacl ��Ȩ���б� NewDacl
		if(SetEntriesInAcl(1, &ea, OldDacl, &NewDacl) != ERROR_SUCCESS)
			return FALSE;

		// ���µ� ACL д�뵽ָ���ļ�
		// �ر�ע�⣺PROTECTED_DACL_SECURITY_INFORMATION ��������ı�Ҫ�ԣ������ָ�������޷���֤��
		// Ҫ�趨��Ȩ�޵�Ψһ�ԡ���ʵ�ϣ�����㲻ָ���ò�������ϣ��˳��������������������
		// SetSamKeyAccessForEveryone �� SetSamKeyAccessForSystem ��������㶨 MACHINE\SAM\SAM ����
		// Ȩ������Ļ��������ǲ����ܵģ���Ϊ�ü��ĸ��������������Ȩ���趨���ᱻ�̳й�����
		return SetNamedSecurityInfo((LPTSTR)pszObjectName, ObjectType, DACL_SECURITY_INFORMATION | PROTECTED_DACL_SECURITY_INFORMATION,
			NULL, NULL, NewDacl, NULL) == ERROR_SUCCESS;
	}
};

#define SetSamKeyAllAccessForEveryone()	\
		CObjAccessChanger::AllowAccess(TEXT("MACHINE\\SAM\\SAM"), SE_REGISTRY_KEY, \
		TEXT("Everyone"), KEY_ALL_ACCESS, FALSE)

#define SetSamKeyAllAccessForSystem()	\
		CObjAccessChanger::AllowAccess(TEXT("MACHINE\\SAM\\SAM"), SE_REGISTRY_KEY, \
		TEXT("System"), KEY_ALL_ACCESS, FALSE)

#define SetSamKeyAllAccessForAdministrators()	\
		CObjAccessChanger::AllowAccess(TEXT("MACHINE\\SAM\\SAM"), SE_REGISTRY_KEY, \
		TEXT("Administrators"), KEY_ALL_ACCESS, FALSE)

#define AddSamKeyDefaultAccessForAdministrators()	\
		CObjAccessChanger::AllowAccess(TEXT("MACHINE\\SAM\\SAM"), SE_REGISTRY_KEY, \
		TEXT("Administrators"), WRITE_DAC | READ_CONTROL, TRUE)
