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
		// 建立一个空的 ACL
		PACL OldDacl = NULL;
		if(bMergeOldDacl && SetEntriesInAcl(0, NULL, NULL, &OldDacl) != ERROR_SUCCESS)
			return FALSE;

		CAclCleaner ac1(OldDacl);

		// 获取现有的 ACL 列表到 OldDacl:
		PSECURITY_DESCRIPTOR psd = NULL;
		if(bMergeOldDacl && GetNamedSecurityInfo((LPTSTR)pszObjectName, ObjectType, DACL_SECURITY_INFORMATION,
			NULL, NULL, &OldDacl, NULL, &psd) != ERROR_SUCCESS)
			return FALSE;

		CSdCleaner sc1(psd);

		PACL NewDacl = NULL;
		if(SetEntriesInAcl(0, NULL, NULL, &NewDacl) != ERROR_SUCCESS)
			return FALSE;

		CAclCleaner ac2(NewDacl);

		// 设置用户 pszAccesser 对指定的键具有操作权 AccessMode 到结构 ea
		EXPLICIT_ACCESS ea;
		ZeroMemory(&ea, sizeof(EXPLICIT_ACCESS));

		BuildExplicitAccessWithName(&ea,
			(LPTSTR)pszAccesser,	// name of trustee
			AccessMode,				// type of access
			SET_ACCESS,				// access mode
			SUB_CONTAINERS_AND_OBJECTS_INHERIT); // 子键继承它的权限
    
		// 合并结构 ea 和 OldDacl 的权限列表到 NewDacl
		if(SetEntriesInAcl(1, &ea, OldDacl, &NewDacl) != ERROR_SUCCESS)
			return FALSE;

		// 把新的 ACL 写入到指定的键
		// 特别注意：PROTECTED_DACL_SECURITY_INFORMATION 这个参数的必要性，如果不指定，则无法保证所
		// 要设定的权限的唯一性。事实上，如果你不指定该参数，而希望顺序调用下面的两个辅助宏
		// SetSamKeyAccessForEveryone 和 SetSamKeyAccessForSystem 来帮助你搞定 MACHINE\SAM\SAM 键的
		// 权限问题的话，几乎是不可能的，因为该键的父对象具有其他的权限设定，会被继承过来。
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
