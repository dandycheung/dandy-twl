#ifndef __E32CMN_H__
#define __E32CMN_H__

//
// NOTE: this is a fake e32cmn.h file for f32image.h!!!
//

//
// section 1: copied from e32def.h
//

typedef void TAny;
typedef signed char TInt8;
typedef unsigned char TUint8;
typedef short int TInt16;
typedef unsigned short int TUint16;
typedef long int TInt32;
typedef unsigned long int TUint32;
typedef signed int TInt;
typedef unsigned int TUint;
typedef float TReal32;
typedef double TReal64;
typedef double TReal;
typedef unsigned char TText8;
typedef unsigned short int TText16;
typedef int TBool;

#define IMPORT_C __declspec(dllexport)

//
// section 2: copied from e32const.h
//

enum TProcessPriority
{
	EPriorityLow=150,
	EPriorityBackground=250,
	EPriorityForeground=350,
	EPriorityHigh=450,
	EPriorityWindowServer=650,
	EPriorityFileServer=750,
	EPriorityRealTimeServer=850,
	EPrioritySupervisor=950
};

enum TFloatingPointType
{
	/** No hardware floating point. */
	EFpTypeNone=0,
	/** ARM VFPv2 */
	EFpTypeVFPv2=1,
	/** ARM VFPv3 */
	EFpTypeVFPv3=2
};

//
// section 3: e32capability.h
//

#include "e32capability.h"

//
// section 4: copied from e32cmn.h(orginal file)
//

class TVersion
{
public:
	// NOTE: All member functions removed for simplicity, by dandy
public:
	/**
	The major version number.
    */
	TInt8 iMajor;
	
	
    /**
    The minor version number.
    */
	TInt8 iMinor;
	
	
	/**
	The build number.
	*/
	TInt16 iBuild;
};

struct SCapabilitySet
{
	enum {ENCapW=2};
	
	inline void AddCapability(TCapability aCap1)
	{
		/// TODO:
	}
	inline void Remove(const SCapabilitySet& aCaps)
	{
		/// TODO:
	}
	inline TBool NotEmpty() const
	{
		/// TODO:
	}
	
	inline const TUint32& operator[] (TInt aIndex) const
	{
		return iCaps[aIndex];
	}
	inline TUint32& operator[] (TInt aIndex)
	{
		return iCaps[aIndex];
	}
	
	TUint32 iCaps[ENCapW];
};

struct SSecurityInfo
{
	TUint32	iSecureId;
	TUint32	iVendorId;
	SCapabilitySet iCaps;	// Capabilities re. platform security
};

#endif // __E32CMN_H__
