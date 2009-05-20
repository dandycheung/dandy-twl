#ifndef __CMDLN_H__
#define __CMDLN_H__

class CArguments
{
public:
	CArguments(LPCTSTR pszCmdLine)
		: _argc(0), _argv(NULL)
	{
		TCHAR szProgramName[MAX_PATH];

		// Get the program name pointer from Windows
		GetModuleFileName(NULL, szProgramName, sizeof(szProgramName) / sizeof(TCHAR));

		// if there's no command line at all (won't happen from cmd.exe, but
		// possibly another program), then we use szProgramName as the command line
		// to parse, so that argv[0] is initialized to the program name
		LPTSTR pszCmdStart = (*pszCmdLine == TEXT('\0')) ? szProgramName : (LPTSTR) pszCmdLine;

		INT cbBytes;

		// first find out how much space is needed to store args
		Parse(pszCmdStart, NULL, NULL, &_argc, &cbBytes);

		// allocate space for argv[] vector and strings
		_argv = (LPTSTR*)LocalAlloc(LMEM_ZEROINIT, (_argc + 1) * sizeof(LPTSTR) + cbBytes);
		if(!_argv)
			return;

		// store args and argv ptrs in just allocated block
		Parse(pszCmdStart, _argv, (LPTSTR)(((LPBYTE)_argv) + _argc * sizeof(LPTSTR)), &_argc, &cbBytes);
	}

	~CArguments()
	{
		Reset();
	}

	int GetArgc() const
	{
		return _argc;
	}

	LPTSTR* GetArgv() const
	{
		return _argv;
	}

protected:
	/*
	 void Parse(cmdstart, argv, psz, numargs, numbytes)

	 Purpose:
	     Parses the command line and sets up the argv[] array.
	     On entry, cmdstart should point to the command line, argv should point to memory
	     for the argv array, psz points to memory to place the text of the arguments.
	     If these are NULL, then no storing (only counting) is done.  On exit, *numargs has
	     the number of arguments (plus one for a final NULL argument), and *numbytes has the
	     number of bytes used in the buffer pointed to by args.

	 Entry:
	     LPTSTR cmdstart - pointer to command line of the form: <progname><nul><args><nul>
	     LPTSTR* argv - where to build argv array; NULL means don't build array
	     LPTSTR psz - where to place argument text; NULL means don't store text

	 Exit:
	        no return value
	        INT* numargs - returns number of argv entries created
	        INT* numbytes - number of bytes used in args buffer
	*/
	static void Parse(
		LPTSTR cmdstart,
		LPTSTR* argv,
		LPTSTR psz,
		INT* numargs,
		INT* numbytes
		)
	{
		*numbytes = 0;
		*numargs = 1;                   // the program name at least

		// first scan the program name, copy it, and count the bytes
		LPTSTR p = cmdstart;
		if(argv)
			*argv++ = psz;

		// A quoted program name is handled here. The handling is much
		// simpler than for other arguments. Basically, whatever lies
		// between the leading double-quote and next one, or a terminal null
		// character is simply accepted. Fancier handling is not required
		// because the program name must be a legal NTFS/HPFS file name.
		// Note that the double-quote characters are not copied, nor do they
		// contribute to numbytes.
		if(*p == TEXT('\"'))
		{
			// scan from just past the first double-quote through the next
			// double-quote, or up to a null, whichever comes first
			while((*(++p) != TEXT('\"')) && (*p != TEXT('\0')))
			{
				*numbytes += sizeof(TCHAR);
				if(psz)
					*psz++ = *p;
			}

			// append the terminating null
			*numbytes += sizeof(TCHAR);
			if(psz)
				*psz++ = TEXT('\0');

			// if we stopped on a double-quote (usual case), skip over it
			if(*p == TEXT('\"'))
				p++;
		}
		else	// Not a quoted program name
		{
			TCHAR c;
		
			do
			{
				*numbytes += sizeof(TCHAR);
				if(psz)
					*psz++ = *p;

				c = (TCHAR)*p++;

			} while(c > TEXT(' '));

			if(c == TEXT('\0'))
				p--;
			else if(psz)
				*(psz - 1) = TEXT('\0');
		}

		BOOL bInQuote = FALSE;	// 1 = inside quotes

		// loop on each argument
		for(;;)
		{
			if(*p)
			{
				while(*p == TEXT(' ') || *p == TEXT('\t'))
					++p;
			}

			if(*p == TEXT('\0'))
				break;                  // end of args

			// scan an argument
			if(argv)
				*argv++ = psz;          // store ptr to arg

			++*numargs;

			// loop through scanning one argument
			for(;;)
			{
				BOOL bCopyChar = TRUE;	// TRUE = copy char to *args

				/*
				Rules:
					2N backslashes + " ==> N backslashes and begin/end quote
					2N+1 backslashes + " ==> N backslashes + literal "
					N backslashes ==> N backslashes
				*/
				WORD numslash = 0;
				while(*p == TEXT('\\'))
				{
					// count number of backslashes for use below
					++p;
					++numslash;
				}

				if(*p == TEXT('\"'))
				{
					// if 2N backslashes before, start/end quote, otherwise copy literally
					if(numslash % 2 == 0)
					{
						if(bInQuote)
							if(p[1] == TEXT('\"'))
								p++;    // Double quote inside quoted string
							else        // skip first quote char and copy second
								bCopyChar = 0;
						else
							bCopyChar = 0;  // don't copy quote

						bInQuote = !bInQuote;
					}

					numslash /= 2;          // divide numslash by two
				}

				// copy slashes
				while(numslash--)
				{
					if(psz)
						*psz++ = TEXT('\\');

					*numbytes += sizeof(TCHAR);
				}

				// if at end of arg, break loop
				if(*p == TEXT('\0') || (!bInQuote && (*p == TEXT(' ') || *p == TEXT('\t'))))
					break;

				// copy character into argument
				if(bCopyChar)
				{
					if(psz)
						*psz++ = *p;

					*numbytes += sizeof(TCHAR);
				}

				++p;
			}

			// null-terminate the argument
			if(psz)
				*psz++ = TEXT('\0');

			*numbytes += sizeof(TCHAR);
		}
	}

	void Reset()
	{
		if(_argv)
		{
			LocalFree(_argv);
			_argv = NULL;
		}

		_argc = 0;
	}

protected:
	int _argc;
	LPTSTR* _argv;
};


#endif // __CMDLN_H__
