//////////////////////////////////////////////////////////////////////////
// This is a part of the Tiny Window Library(TWL).
// Copyright (C) 2003-2009 Dandy Cheung
// dandycheung@hotmail.com
// All rights reserved.
//

#ifndef __HANDLE_H__
#define __HANDLE_H__

#pragma once

#ifndef __cplusplus
	#error TWL requires C++ compilation (use a .cpp suffix)
#endif

namespace TWL
{

class CHandle
{
public:
	CHandle() throw()
		: m_handle(NULL)
	{
	}
	
	CHandle(CHandle& rh) throw()
		: m_handle(NULL)
	{
		Attach(rh.Detach());
	}

	explicit CHandle(HANDLE handle) throw()
		: m_handle(handle)
	{
	}
	
	~CHandle() throw()
	{
		if(m_handle)
			Close();
	}

	CHandle& operator=(CHandle& rh) throw()
	{
		if(this != &rh)
		{
			if(m_handle)
				Close();

			Attach(rh.Detach());
		}

		return *this;
	}

	operator HANDLE() const throw()
	{
		return m_handle;
	}

	// Attach to an existing handle (takes ownership).
	void Attach(HANDLE handle) throw()
	{
		m_handle = handle;  // Take ownership
	}

	// Detach the handle from the object (releases ownership).
	HANDLE Detach() throw()
	{
		HANDLE handle = m_handle;  // Release ownership
		m_handle = NULL;

		return handle;
	}

	// Close the handle.
	void Close() throw()
	{
		if(m_handle)
		{
			::CloseHandle(m_handle);
			m_handle = NULL;
		}
	}

public:
	HANDLE m_handle;
};

}; // namespace TWL

#endif // __HANDLE_H__
