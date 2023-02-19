/*++

   Copyright    (c)    1994-1999    Microsoft Corporation

   Module  Name :

        strfrn.cpp

   Abstract:

        String Functions

   Author:

        Ronald Meijer (ronaldm)

   Project:

        Internet Services Manager (cluster edition)

   Revision History:

--*/

//
// Include Files
//
#include "stdafx.h"
#include "common.h"
#include <pudebug.h>


#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


#define new DEBUG_NEW


#ifdef _MT

    //
    // Thread protected stuff
    //
    #define RaiseThreadProtection() EnterCriticalSection(&_csSect)
    #define LowerThreadProtection() LeaveCriticalSection(&_csSect)

    static CRITICAL_SECTION _csSect;

#else

    #pragma message("Module is not thread-safe")

    #define RaiseThreadProtection()
    #define LowerThreadProtection()

#endif // _MT

#define MAKE_NULL(obj) { if (obj) delete obj, obj = NULL; }


//
// Text copy functions
//
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



BOOL
PCToUnixText(
    OUT LPWSTR & lpstrDestination,
    IN  const CString strSource
    )
/*++

Routine Description:

    Convert CR/LF string to LF string (T String to W String).  Destination
    string will be allocated.

Arguments:

    LPWSTR & lpstrDestination : Destination string
    const CString & strSource : Source string

Return Value:

    TRUE for success, FALSE for failure.

--*/
{
    int cch = strSource.GetLength() + 1;
    lpstrDestination = (LPWSTR)AllocMem(cch * sizeof(WCHAR));

    if (lpstrDestination != NULL)
    {
        LPCTSTR lpS = strSource;
        LPWSTR lpD = lpstrDestination;

        do
        {
            if (*lpS != _T('\r'))
            {

#ifdef UNICODE
                *lpD++ = *lpS;
#else
                ::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, lpS, 1, lpD++, 1);
#endif // UNICODE

            }
        }
        while (*lpS++);

        return TRUE;
    }

    return FALSE;
}



BOOL
UnixToPCText(
    OUT CString & strDestination,
    IN  LPCWSTR lpstrSource
    )
/*++

Routine Description:

    Expand LF to CR/LF (no allocation necessary) W String to T String.

Arguments:

    CString & strDestination : Destination string
    LPCWSTR lpstrSource      : Source string

Return Value:

    TRUE for success, FALSE for failure.

--*/
{
    BOOL fSuccess = FALSE;

    try
    {
        LPCWSTR lpS = lpstrSource;

        //
        // Since we're doubling every linefeed length, assume
        // the worst possible expansion to start with.
        //
        int cch = (::lstrlenW(lpstrSource) + 1) * 2;
        LPTSTR lpD = strDestination.GetBuffer(cch);

        do
        {
            if (*lpS == L'\n')
            {
                *lpD++ = _T('\r');
            }

#ifdef UNICODE
            *lpD++ = *lpS;
#else
            ::WideCharToMultiByte(CP_ACP, 0, lpS, 1, lpD++, 1, NULL, NULL);
#endif // UNICODE

        }
        while (*lpS++);

        strDestination.ReleaseBuffer();

        ++fSuccess;
    }
    catch(CMemoryException * e)
    {
        TRACEEOLID("Exception in UnixToPCText");
        e->ReportError();
        e->Delete();
    }

    return fSuccess;
}

/*

BOOL
TextToText(
    OUT LPWSTR & lpstrDestination,
    IN  const CString & strSource
    )
/*++

Routine Description:

    Straight copy with allocation. T String to W String.

Arguments:

    LPWSTR & lpstrDestination : Destination string
    const CString & strSource : Source string

Return Value:

    TRUE for success, FALSE for failure.

--/
{
    int cch = strSource.GetLength() + 1;
    lpstrDestination = (LPWSTR)AllocMem(cch * sizeof(WCHAR));

    if (lpstrDestination != NULL)
    {
        TWSTRCPY(lpstrDestination, strSource, cch);
        return TRUE;
    }

    return FALSE;
}



#ifndef UNICODE



#define WBUFF_SIZE  255



LPWSTR
ReferenceAsWideString(
    IN LPCTSTR str
    )
/*++

Routine Description:

    Reference a T string as a W string (non-unicode only).

Arguments:

    LPCTSTR str : Source string

Return Value:

    Wide char pointer to wide string.

Notes:

    This uses an internal wide char buffer, which will be overwritten
    by subsequent calls to this function.

--/
{
    static WCHAR wchBuff[WBUFF_SIZE + 1];

    ::MultiByteToWideChar(
        CP_ACP,
        MB_PRECOMPOSED,
        str,
        -1,
        wchBuff,
        WBUFF_SIZE + 1
        );

    return wchBuff;
}



#endif !UNICODE


*/


LPSTR
AllocAnsiString(
    IN LPCTSTR lpString
    )
/*++

Routine Description:

    Convert the wide string to an ansi (multi-byte) string, which is allocated
    by this function

Arguments:

    LPCTSTR lpString        : Input wide string

Return Value:

    Pointer to the allocated string

--*/
{
    //
    // Character counts are DBCS friendly
    //
    int cChars = lstrlen(lpString);
    int nLength = (cChars * 2) + 1;
    LPSTR lp = (LPSTR)AllocMem(nLength);

    if (lp)
    {
        ::WideCharToMultiByte(
            CP_ACP,
            0,
            lpString,
            cChars + 1,
            lp,
            nLength,
            NULL,
            NULL
            );
    }

    return lp;
}




LPTSTR
AllocString(
    IN LPCTSTR lpString,
    IN int nLen 
    )
/*++

Routine Description:

    Allocate and copy string

Arguments:

    LPCTSTR lpString        : Input string
    int nLen                : Length or -1 to autodetermine

Return Value:

    Pointer to the allocated string

--*/
{
    if (nLen < 0)
    {
        nLen = lstrlen(lpString);
    }
    
    LPTSTR lp = (LPTSTR)AllocMem((nLen + 1) * sizeof(TCHAR));

    if (lp)
    {
        lstrcpy(lp, lpString);
    }

    return lp;
}



BOOL
IsUNCName(
    IN const CString & strDirPath
    )
/*++

Routine Description:

    Determine if the given string path is a UNC path.

Arguments:

    const CString & strDirPath : Directory path string

Return Value:

    TRUE if the path is a UNC path, FALSE otherwise.

Notes:

    Any string of the form \\foo\bar\whatever is considered a UNC path,
    with the exception of \\.\device paths.  No validation for the 
    existance occurs, only for the correct format.

--*/
{
    if (strDirPath.GetLength() >= 5)  // It must be at least as long as \\x\y,
    {                                 //
        LPCTSTR lp = strDirPath;      //
        if (*lp == _T('\\')           // It must begin with \\,
         && *(lp + 1) == _T('\\')     //
         && *(lp + 2) != _T('.')      // This is a device.
         && _tcschr(lp + 3, _T('\\')) // And have at least one more \ after that
           )
        {
            //
            // Yes, it's a UNC path
            //
            return TRUE;
        }
    }

    //
    // No, it's not
    //
    return FALSE;
}



BOOL 
IsDevicePath(
    IN const CString & strDirPath
    )
/*++

Routine Description:

    Determine if the given path is of the form "\\.\foobar"

Arguments:

    const CString & strDirPath : Directory path string

Return Value:

    TRUE if the path given is a device path, 
    FALSE if it is not.

--*/
{
    LPCTSTR lpszDevice = _T("\\\\.\\");

    return _tcsnccmp(strDirPath, lpszDevice, lstrlen(lpszDevice)) == 0;
}


BOOL
PathIsValid(LPCTSTR path)
{
    LPCTSTR p = path;
    BOOL rc = TRUE;
    if (p == NULL || *p == 0)
        return FALSE;
    while (*p != 0)
    {
        switch (*p)
        {
        case TEXT('|'):
        case TEXT('>'):
        case TEXT('<'):
        case TEXT('/'):
        case TEXT('?'):
        case TEXT('*'):
//        case TEXT(';'):
//        case TEXT(','):
        case TEXT('"'):
            rc = FALSE;
            break;
        default:
            if (*p < TEXT(' '))
            {
                rc = FALSE;
            }
            break;
        }
        if (!rc)
        {
            break;
        }
        p++;
    }
    return rc;
}


BOOL
IsFullyQualifiedPath(
    IN const CString & strDirPath
    )
/*++

Routine Description:

    Determine if the given string is a fully qualified path name

Arguments:

    const CString & strDirPath : Directory path string

Return Value:

    TRUE if the path is a fully qualified path name


--*/
{
    return strDirPath.GetLength() >= 3
        && strDirPath[1] == _T(':')
        && strDirPath[2] == _T('\\');
}



BOOL
IsNetworkPath(
    IN  const CString & strDirPath,
    OUT CString * pstrDrive,        OPTIONAL
    OUT CString * pstrUNC           OPTIONAL
    )
/*++

Routine Description:

    Determine if the path exists on a network directory
    in the context of the local machine.

Arguments:

    const CString & strDirPath : Directory path string
    CString * pstrDrive        : Returns drive
    CString * pstrUNC          : Returns UNC path

Return Value:

    TRUE if the path is a network path, FALSE if it is local.

Notes:

    Only fully qualified paths with drive letters are checked.

--*/
{
    BOOL fUNC = FALSE;
    CString strDrive;

    try
    {
        if (pstrDrive == NULL)
        {
            pstrDrive = &strDrive;
        }

        ASSERT(strDirPath[1] == _T(':'));

        if (strDirPath[1] == _T(':'))
        {
            *pstrDrive = _T("?:");

            //
            // Fill in actual drive letter
            //
            pstrDrive->SetAt(0, strDirPath[0]);
            UINT nType = GetDriveType(*pstrDrive);

            //
            // DRIVE_NO_ROOT_DIR? this is a little dodgy, but
            // this is the result I get back after browsing
            // using a file open dialog.  Weird.
            //
            fUNC = (nType == DRIVE_NO_ROOT_DIR || nType == DRIVE_REMOTE);
        }

        //
        // Return UNC path if requested
        //
        if (fUNC && pstrUNC != NULL)
        {
            DWORD dwSize = _MAX_PATH;
            LPTSTR lp = pstrUNC->GetBuffer(dwSize);
            ::WNetGetConnection(*pstrDrive, lp, &dwSize);
            pstrUNC->ReleaseBuffer();
        }
    }
    catch(CException * e)
    {
        e->ReportError();
        e->Delete();
    }

    return fUNC;
}



LPCTSTR
MakeUNCPath(
    IN OUT CString & strDir,
    IN LPCTSTR lpszOwner,
    IN LPCTSTR lpszDirectory
    )
/*++

Routine Description:

    Convert the given directory to a UNC path.

Arguments:

    CString & strDir      : UNC String.
    LPCTSTR lpszOwner     : Computer name
    LPCTSTR lpszDirectory : Source string

Return Value:

    Pointer to strDir

Notes:

    The owner may or may not start with "\\".  If it doesn't, the
    backslashes are provided.

--*/
{
    //
    // Try to make make a unc path out of the directory
    //
    ASSERT(lpszDirectory[1] == _T(':'));

    strDir.Format(
        _T("\\\\%s\\%c$\\%s"),
        PURE_COMPUTER_NAME(lpszOwner),
        lpszDirectory[0],
        lpszDirectory + 3
        );

    return (LPCTSTR)strDir;
}



BOOL
IsURLName(
    IN const CString & strDirPath
    )
/*++

Routine Description:

    Determine if the given string path is an URL path.

Arguments:

    const CString & strDirPath : Directory path string

Return Value:

    TRUE if the path is an URL path, FALSE otherwise.

Notes:

    Any string of the form protocol://whatever is considered an URL path

--*/
{
    if (strDirPath.GetLength() >= 4)  // It must be at least as long as x://
    {                                 //
        if (strDirPath.Find(_T("://")) > 0) // Must contain ://
        {
            //
            // Yes, it's an URL path
            //
            return TRUE;
        }
    }

    //
    // No, it's not
    //
    return FALSE;
}



int
CStringFindNoCase(
    IN const CString & strSrc,
    IN LPCTSTR lpszSub
    )
/*++

Routine Description:

    This should be CString::FindNoCase().  Same as CString::Find(),
    but case-insensitive.

Arguments:

    const CString & strSrc  : Source string
    LPCTSTR lpszSub         : String to look for.

Return Value:

    The position of the substring, or -1 if not found.

--*/
{
    LPCTSTR lp1 = strSrc;
    LPCTSTR lp2, lp3;
    int nPos = -1;

    while (*lp1)
    {
        lp2 = lp1;
        lp3 = lpszSub;

        while(*lp2 && *lp3 && _totupper(*lp2) == _totupper(*lp3))
        {
            ++lp2;
            ++lp3;
        }

        if (!*lp3)
        {
            //
            // Found the substring
            //
            nPos = (int)(lp1 - (LPCTSTR)strSrc);
            break;
        }
    
        ++lp1;                    
    }

    return nPos;
}



DWORD
ReplaceStringInString(
    OUT IN CString & strBuffer,
    IN  CString & strTarget,
    IN  CString & strReplacement,
    IN  BOOL fCaseSensitive
    )
/*++

Routine Description:

    Replace the first occurrence of a string with a second string
    inside a third string.

Arguments:

    CString & strBuffer         : Buffer in which to replace
    CString & strTarget         : String to look for
    CString & strReplacement    : String to replace it with
    BOOL fCaseSensitive         : TRUE for case sensitive replacement.
    
Return Value:

    ERROR_SUCCESS for successful replacement.
    ERROR_INVALID_PARAMETER if any string is empty,
    ERROR_FILE_NOT_FOUND if the target string doesn't exist, or
    another win32 error code indicating failure.

--*/
{
    if (strBuffer.IsEmpty() || strTarget.IsEmpty() || strReplacement.IsEmpty())
    {
        return ERROR_INVALID_PARAMETER;
    }

    DWORD err = ERROR_FILE_NOT_FOUND;
    int nPos = fCaseSensitive 
        ? strBuffer.Find(strTarget)
        : CStringFindNoCase(strBuffer, strTarget);

    if (nPos >= 0)
    {
        try
        {
            CString str(strBuffer.Left(nPos));

            str += strReplacement;
            str += strBuffer.Mid(nPos + strTarget.GetLength());
            strBuffer = str;

            err = ERROR_SUCCESS;
        }
        catch(CMemoryException * e)
        {
            e->Delete();
            err = ERROR_NOT_ENOUGH_MEMORY;
        }
    }    

    return err;
}




DWORD
DeflateEnvironmentVariablePath(
    IN LPCTSTR lpszEnvVar,
    IN OUT CString & strTarget
    )
/*++

Routine Description:

    Take a path, and if the path represents a superset of the
    path in the environment variable given, replace the relevant
    portion of the path with the environment variable.

Arguments:

    LPCTSTR lpszEnvVar      : Environment variable
    CString & strTarget     : Path

Return Value:

    ERROR_SUCCESS if the replacement was done succesfully,
    ERROR_FILE_NOT_FOUND if the path represented by the environment
    variable is not contained within the string, or an other win32
    error for error conditions

--*/
{
    CError err;
    CString strEnv;

    if (!::GetEnvironmentVariable(
        lpszEnvVar,
        strEnv.GetBuffer(_MAX_PATH),
        _MAX_PATH
        ))
    {
        err.SetLastWinError();
    }

    strEnv.ReleaseBuffer();

    if (err.Succeeded())
    {
        try
        {
            CString strReplacement(_T("%"));
            strReplacement += lpszEnvVar;
            strReplacement += _T("%");

            err = ReplaceStringInString(
                strTarget,
                strEnv,
                strReplacement,
                FALSE 
                );
        }
        catch(CMemoryException * e)
        {
            err = ERROR_NOT_ENOUGH_MEMORY;
            e->Delete();
        }
    }

    return err;
}



LPCTSTR
GUIDToCString(
    IN  REFGUID guid,
    OUT CString & str
    )
/*++

Routine Description:

    Convert a GUID to a CString, returning pointer to the string

Arguments:

    REFGUID       : GUID to be converted
    CString & str : Output string buffer

Return Value:

    Pointer to the string

--*/
{
    LPTSTR lpGUID = str.GetBuffer(MAX_PATH);

    if (lpGUID)
    {
        ::StringFromGUID2(guid, lpGUID, MAX_PATH);
        str.ReleaseBuffer();
    }

    return str;
}



int
CountCharsToDoubleNull(
    IN LPCTSTR lp
    )
/*++

Routine Description:

    Count TCHARS up to and including the double NULL.

Arguments:

    LPCTSTR lp       : TCHAR Stream

Return Value:

    Number of chars up to and including the double NULL

--*/
{
    int cChars = 0;

    for(;;)
    {
        ++cChars;

        if (lp[0] == _T('\0') && lp[1] == _T('\0'))
        {
            return ++cChars;
        }

        ++lp;
    }
}



DWORD
ConvertDoubleNullListToStringList(
    IN  LPCTSTR lpstrSrc,
    OUT CStringList & strlDest,
    IN  int cChars                  OPTIONAL
    )
/*++

Routine Description:

    Convert a double null terminate list of null terminated strings to a more
    manageable CStringListEx

Arguments:

    LPCTSTR lpstrSrc       : Source list of strings
    CStringList & strlDest : Destination string list.
    int cChars             : Number of characters in double NULL list. if
                             -1, autodetermine length

Return Value:

    ERROR_SUCCESS           if the list was converted properly
    ERROR_INVALID_PARAMETER if the list was empty
    ERROR_NOT_ENOUGH_MEMORY if there was a mem exception

--*/
{
    DWORD err = ERROR_SUCCESS;

    if (lpstrSrc == NULL)
    {
        return ERROR_INVALID_PARAMETER;
    }

    if (cChars < 0)
    {
        //
        // Calculate our own size.  This might be off if multiple
        // blank linkes (0) appear in the multi_sz, so the character
        // size is definitely preferable
        //
        cChars = CountCharsToDoubleNull(lpstrSrc);
    }

    try
    {
        strlDest.RemoveAll();

        if (cChars == 2 && *lpstrSrc == _T('\0'))
        {
            //
            // Special case: MULTI_SZ containing only
            // a double NULL are in fact blank entirely.
            //
            // N.B. IMHO this is a metabase bug -- RonaldM
            //
            --cChars;
        }

        //
        // Grab strings until only the final NULL remains
        //
        while (cChars > 1)
        {
            CString strTmp = lpstrSrc;
            strlDest.AddTail(strTmp);
            lpstrSrc += (strTmp.GetLength() + 1);
            cChars -= (strTmp.GetLength() + 1);
        }
    }
    catch(CMemoryException * e)
    {
        TRACEEOLID("!!! exception building stringlist");
        err = ERROR_NOT_ENOUGH_MEMORY;
        e->Delete();
    }

    return err;
}



DWORD
ConvertStringListToDoubleNullList(
    IN  CStringList & strlSrc,
    OUT DWORD & cchDest,
    OUT LPTSTR & lpstrDest
    )
/*++

Routine Description:

    Flatten the string list into a double null terminated list
    of null terminated strings.

Arguments:

    CStringList & strlSrc : Source string list
    DWORD & cchDest       : Size in characters of the resultant array
                            (including terminating NULLs)
    LPTSTR & lpstrDest    : Allocated flat array.

Return Value:

    ERROR_SUCCESS           if the list was converted properly
    ERROR_INVALID_PARAMETER if the list was empty
    ERROR_NOT_ENOUGH_MEMORY if there was a mem exception

--*/
{
    cchDest = 0;
    lpstrDest = NULL;
    BOOL fNullPad = FALSE;

    //
    // Compute total size in characters
    //
    POSITION pos;

    for(pos = strlSrc.GetHeadPosition(); pos != NULL; /**/ )
    {
        CString & str = strlSrc.GetNext(pos);

        TRACEEOLID(str);

        cchDest += str.GetLength() + 1;
    }

    if (!cchDest)
    {
        //
        // Special case: A totally empty MULTI_SZ
        // in fact consists of 2 (final) NULLS, instead
        // of 1 (final) NULL.  This is required by the
        // metabase, but should be a bug.  See note
        // at reversal function above.
        //
        ++cchDest;
        fNullPad = TRUE;
    }

    //
    // Remember final NULL
    //
    cchDest += 1;

    lpstrDest = AllocTString(cchDest);

    if (lpstrDest == NULL)
    {
        return ERROR_NOT_ENOUGH_MEMORY;
    }

    LPTSTR pch = lpstrDest;

    for(pos = strlSrc.GetHeadPosition(); pos != NULL; /**/ )
    {
        CString & str = strlSrc.GetNext(pos);

        lstrcpy(pch, (LPCTSTR)str);
        pch += str.GetLength();
        *pch++ = _T('\0');
    }

    *pch++ = _T('\0');

    if (fNullPad)
    {
        *pch++ = _T('\0');
    }

    return ERROR_SUCCESS;
}



int
ConvertSepLineToStringList(
    IN  LPCTSTR lpstrIn,
    OUT CStringList & strlOut,
    IN  LPCTSTR lpstrSep
    )
/*++

Routine Description:

    Convert a line containing multiple strings separated by
    a given character to a CStringListEx

Arguments:

    LPCTSTR lpstrIn         : Input line
    CStringListEx & strlOut : Output stringlist
    LPCTSTR lpstrSep        : List of separators

Return Value:

    The number of items added

--*/
{
    int cItems = 0;
    strlOut.RemoveAll();

    try
    {
        CString strSrc(lpstrIn);
        LPTSTR lp = strSrc.GetBuffer(0);
        lp = _tcstok(lp, lpstrSep);

        while (lp)
        {
            CString str(lp);

            strlOut.AddTail(str);
            lp = _tcstok(NULL, lpstrSep);
            ++cItems;
        }
    }
    catch(CMemoryException * e)
    {
        TRACEEOLID("Exception converting CSV list to stringlist");
        e->ReportError();
        e->Delete();
    }

    return cItems;
}




LPCTSTR
ConvertStringListToSepLine(
    IN  CStringList & strlIn,
    OUT CString & strOut,
    IN  LPCTSTR lpstrSep
    )
/*++

Routine Description:

    Convert stringlist into a single CString, each entry separated by the given
    separator string.

Arguments:

    CStringListEx & strlIn  : Input stringlist
    CString & strOut        : Output string
    LPCTSTR lpstrSep        : Separator string

Return Value:

    Pointer to the output string.

--*/
{
    strOut.Empty();
    POSITION pos = strlIn.GetHeadPosition();

    while(pos)
    {
        CString & str = strlIn.GetNext(pos);

        strOut += str;
        strOut += lpstrSep;
    }

    //
    // Fix for bug #286824
    // Remove separator from the last line: it looks ugly in edit control
    //
    if (!strOut.IsEmpty())
    {
        strOut.GetBufferSetLength(strOut.GetLength() - 1);
        strOut.ReleaseBuffer();
    }

    return strOut;
}



BOOL
CStringListEx::operator ==(
    IN const CStringList & strl
    )
/*++

Routine Description:

    Compare against CStringList.  In order for two CStringLists to match,
    they must match in every element, which must be in the same order.

Arguments:

    CStringList strl       : String list to compare against.

Return Value:

    TRUE if the two string lists are identical

--*/
{
    if (strl.GetCount() != GetCount())
    {
        return FALSE;
    }

    POSITION posa = strl.GetHeadPosition();
    POSITION posb = GetHeadPosition();

    while (posa)
    {
        ASSERT(posa);
        ASSERT(posb);

        CString & strA = strl.GetNext(posa);
        CString & strB = GetNext(posb);

        if (strA != strB)
        {
            return FALSE;
        }
    }

    return TRUE;
}



CStringListEx & 
CStringListEx::operator =(
    IN const CStringList & strl
    )
/*++

Routine Description:

    Assignment operator

Arguments:

    const CStringList & strl        : Source stringlist

Return Value:

    Reference to this

--*/
{
    RemoveAll();
    POSITION pos = strl.GetHeadPosition();

    while(pos)
    {
        CString & str = strl.GetNext(pos);
        AddTail(str);
    }

    return *this;
}



BOOL
CvtStringToLong(
    IN  LPCTSTR lpNumber,
    OUT DWORD * pdwValue
    )
/*++

Routine Description:

    Helper function to convert string (hex or decimal) to a dword.

Arguments:

    LPCTSTR lpNumber        : Input number
    DWORD * pdwValue        : Returns the value

Return Value:

    TRUE for success, FALSE for failure

--*/
{
    const TCHAR rgchHex[] = _T("00112233445566778899aAbBcCdDeEfF");

    DWORD dwResult = 0L;
    DWORD dwResultPrev = 0L;

    //
    // Assume a decimal base
    //
    DWORD dwBase = 10L;

    ASSERT_READ_PTR(lpNumber);
    ASSERT_WRITE_PTR(pdwValue);

    while (*lpNumber == _T(' ') || *lpNumber == _T('0'))
    {
        ++lpNumber;
    }

    if (*lpNumber == _T('x') || *lpNumber == _T('X'))
    {
        dwBase = 16L;
        ++lpNumber;
    }

    while (*lpNumber)
    {
        DWORD dwDigit;

        //
        // Search the character in the hexadecimal string
        //
        LPCTSTR pchDigit =  _tcschr(rgchHex, *lpNumber);

        if (!pchDigit)
        {
            //
            // Character is not found
            //
            return FALSE;
        }

        dwDigit = DIFF(pchDigit - rgchHex) >> 1;

        if (dwDigit >= dwBase)
        {
            //
            // Hexadecimal character in a decimal integer
            //
            return FALSE;
        }

        dwResultPrev = dwResult;
        dwResult *= dwBase;
        dwResult += dwDigit;

        if (dwResult < dwResultPrev)
        {
            //
            // Overflow
            //
            return FALSE;
        }

        //
        // Parse the next character
        //
        ++lpNumber;
    }

    *pdwValue = dwResult;

    return TRUE;
}



const LPCTSTR g_cszMonths[] =
{
    _T("Jan"),
    _T("Feb"),
    _T("Mar"),
    _T("Apr"),
    _T("May"),
    _T("Jun"),
    _T("Jul"),
    _T("Aug"),
    _T("Sep"),
    _T("Oct"),
    _T("Nov"),
    _T("Dec"),
};



const LPCTSTR g_cszWeekDays[] =
{
    _T("Sun"),
    _T("Mon"),
    _T("Tue"),
    _T("Wed"),
    _T("Thu"),
    _T("Fri"),
    _T("Sat"),
};



inline BOOL SkipTillDigit(LPCTSTR & lp)
{
    while (lp && *lp && !_istdigit(*lp)) ++lp;

    return lp != NULL;
}



inline BOOL SkipPastDigits(LPCTSTR & lp)
{
    while (lp && *lp && _istdigit(*lp)) ++lp;

    return lp != NULL;
}



BOOL
FetchIntField(
    LPCTSTR & lp,
    int & n
    )
{
    if (SkipTillDigit(lp))
    {
        n = _ttoi(lp);
        if (n < 0)
        {
            ASSERT_MSG("Bogus string->int");
            return FALSE;
        }

        return SkipPastDigits(lp);
    }

    return FALSE;
}



BOOL
MatchString(
    LPCTSTR lpTarget,
    const LPCTSTR * rglp,
    int cElements,
    int & idx
    )
{
    for (idx = 0; idx < cElements; ++idx)
    {
        if (!_tcsnicmp(lpTarget, rglp[idx], _tcslen(rglp[idx])))
        {
            return TRUE;
        }
    }

    return FALSE;
}



static g_dwCurrentTimeZone = TIME_ZONE_ID_INVALID;
static TIME_ZONE_INFORMATION g_tzInfo;




BOOL
CvtGMTStringToInternal(
    IN  LPCTSTR lpTime,
    OUT time_t * ptValue
    )
/*++

Routine Description:

    Convert GMT string to time in the local timezone format

Arguments:

    IN  LPCSTSTR lpTime             : Input time string
    OUT time_t * ptValue            : Output time_t

Return Value:

    TRUE for success, FALSE for failure

--*/
{
    ASSERT_READ_PTR(lpTime);
    ASSERT_WRITE_PTR(ptValue);

    int year, month, day, hour, minutes, seconds;

    do
    {
        if (!lpTime || !ptValue)
        {
            break;
        }

        if (!FetchIntField(lpTime, day))
        {
            break;
        }

        ++lpTime;   // Skip separator

        if (!MatchString(lpTime, g_cszMonths, 12, month))
        {
            break;
        }

        ++month; // 0-based.

        if (!FetchIntField(lpTime, year)    ||
            !FetchIntField(lpTime, hour)    ||
            !FetchIntField(lpTime, minutes) ||
            !FetchIntField(lpTime, seconds))
        {
            break;
        }

        if (year < 100)
        {
            year += ((year < 50) ? 2000 : 1900);
        }

        CTime time(year, month, day, hour, minutes, seconds);
        if (time.GetTime() == (time_t)-1L)
        {
            break;
        }

        *ptValue = time.GetTime();

        TRACEEOLID("GMT Time is " << ctime(ptValue));

        //
        // Adjust for local time zone
        //
        RaiseThreadProtection();

        if (g_dwCurrentTimeZone == TIME_ZONE_ID_INVALID) 
        {
            //
            // GetTimeZoneInfo() is expensive, so cache its output
            //
            g_dwCurrentTimeZone = GetTimeZoneInformation(&g_tzInfo);
        }

        LowerThreadProtection();

        if (g_dwCurrentTimeZone != TIME_ZONE_ID_INVALID)
        {
            _tzset();

            struct tm * tb = localtime(ptValue);

            BOOL fDST = tb->tm_isdst;

            RaiseThreadProtection();
            *ptValue -= (g_tzInfo.Bias * 60L);

            TRACEEOLID("local time (w/o dst adjustment) is " << ctime(ptValue));

            if (fDST)
            {
                *ptValue -= (g_tzInfo.DaylightBias * 60L);
            }
            else
            {
                //
                // Almost always this is zero
                //
                *ptValue -= (g_tzInfo.StandardBias * 60L);
            }
            LowerThreadProtection();

            TRACEEOLID("Local time adjusted for dst is " << ctime(ptValue));
        }

        //
        // Time OK
        //
        return TRUE;
    }
    while(FALSE);

    //
    // Set max abs timeout value.
    //
    CTime time(2037, 12, 31, 0, 0, 0);
    *ptValue = (time_t)time.GetTime();

    return FALSE;
}



void
CvtInternalToGMTString(
    IN  time_t tm,
    OUT CString & str
    )
/*++

Routine Description:

    Reverse of the above function.  Convert internal time_t to a GMT
    time string (converting to GM time in the process)

Arguments:

    time_t tm       : Input time_t
    CString & str   : Out CString

Return Value:

    None

--*/
{
    try
    {
        _tzset();

        struct tm * tb = gmtime(&tm);

        str.Format(
            _T("%-3.3s, %02d %-3.3s %4d %02d:%02d:%02d GMT"),
            g_cszWeekDays[tb->tm_wday],
            tb->tm_mday,
            g_cszMonths[tb->tm_mon],
            tb->tm_year + 1900,
            tb->tm_hour,
            tb->tm_min,
            tb->tm_sec
            );

        TRACEEOLID("Time string is " << str);
    }
    catch(CMemoryException * e)
    {
        e->ReportError();
        e->Delete();
    }
}



//
// International numeric strings
//
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



//
// Initialize library
//
BOOL
InitIntlSettings()
{
#ifdef _MT
    INITIALIZE_CRITICAL_SECTION(&_csSect);
#endif // _MT

    return CINumber::Allocate();
}



//
// De-initialize library
//
void
TerminateIntlSettings()
{
    CINumber::DeAllocate();

#ifdef _MT
    DeleteCriticalSection(&_csSect);
#endif // _MT
}



//
// Static Member Initialization
//
// NOTE: MFC classes CANNOT be global in an AFX extension, as they
//       will not be initialized properly.  Solution is to allocate them.
//
CString * CINumber::_pstrThousandSeparator = NULL;
CString * CINumber::_pstrDecimalPoint = NULL;
CString * CINumber::_pstrBadNumber = NULL;
CString * CINumber::_pstrCurrency = NULL;
CString * CINumber::_pstr = NULL;
BOOL CINumber::_fAllocated = FALSE;
BOOL CINumber::_fCurrencyPrefix = TRUE;
BOOL CINumber::_fInitialized = FALSE;



CINumber::CINumber()
/*++

Routine Description:

    Constructor

Arguments:

    None

Return Value:

    None

--*/
{
    if (!CINumber::_fInitialized)
    {
        CINumber::Initialize();
    }
}



CINumber::~CINumber()
/*++

Routine Description:

    Destructor

Arguments:

    None

Return Value:

    None

--*/
{
}



/* static */
BOOL
CINumber::Allocate()
/*++

Routine Description:

    Allocate with US settings

Arguments:

    None

Return Value:

    TRUE if allocation was successfull, FALSE otherwise

--*/
{
    RaiseThreadProtection();

    ASSERT(!IsAllocated());

    if (!IsAllocated())
    {
        try
        {
            //
            // NOTE: Cannot use globals for these, because
            //       they will not be initialized properly
            //       when used in an extension dll.
            //
            CINumber::_pstrThousandSeparator = new CString(_T(","));
            CINumber::_pstrDecimalPoint = new CString(_T("."));
            CINumber::_pstrBadNumber = new CString(_T("--"));
            CINumber::_pstrCurrency = new CString(_T("$ "));
            CINumber::_pstr = new CString;
            _fAllocated = TRUE;
        }
        catch(CMemoryException * e)
        {
            TRACEEOLID("Initialization Failed");
            e->ReportError();
            e->Delete();
        }
    }

    LowerThreadProtection();

    return IsAllocated();
}



/* static */
void
CINumber::DeAllocate()
/*++

Routine Description:

    Clean up allocations

Arguments:

    N/A

Return Value:

    N/A

--*/
{
    RaiseThreadProtection();
    
    ASSERT(IsAllocated());

    if (IsAllocated())
    {
        MAKE_NULL(CINumber::_pstrThousandSeparator);
        MAKE_NULL(CINumber::_pstrDecimalPoint);
        MAKE_NULL(CINumber::_pstrBadNumber);
        MAKE_NULL(CINumber::_pstrCurrency);
        MAKE_NULL(CINumber::_pstr);
    }

    LowerThreadProtection();

    _fAllocated = FALSE;
}



/* static */
BOOL
CINumber::Initialize(
    IN BOOL fUserSetting /* TRUE */
    )
/*++

Routine Description:

    Initialize all the international settings, such as thousand
    separators and decimal points

Parameters:

    BOOL    fUserSetting        If TRUE, use current user settings,
                                if FALSE use system settings.
Return Value:

    TRUE for success, FALSE for failure

Notes:

    Note that this function only needs to be explicitly called
    when the country settings have changed, or when system
    settings are desired (user is default)

--*/
{
#define MAXLEN  6

    int cErrors = 0;

    TRACEEOLID("Getting locale-dependend information");

    ASSERT(IsAllocated());
    if (!IsAllocated())
    {
        Allocate();
    }

    RaiseThreadProtection();

    try
    {
        LCID lcid = fUserSetting
            ? ::GetUserDefaultLCID()
            : GetSystemDefaultLCID();

        LCTYPE lctype = fUserSetting ? 0 : LOCALE_NOUSEROVERRIDE;

        //
        // Get Decimal Point
        //
        if (!::GetLocaleInfo(
            lcid,
            LOCALE_SDECIMAL | lctype,
            CINumber::_pstrDecimalPoint->GetBuffer(MAXLEN),
            MAXLEN
            ))
        {
            TRACEEOLID("Unable to get intl decimal point");
            ++cErrors;
        }

        CINumber::_pstrDecimalPoint->ReleaseBuffer();

        //
        // Get Thousand Separator
        //
        if (!::GetLocaleInfo(
            lcid, LOCALE_STHOUSAND | lctype,
            CINumber::_pstrThousandSeparator->GetBuffer(MAXLEN),
            MAXLEN
            ))
        {
            TRACEEOLID("Unable to get 1000 separator");
            ++cErrors;
        }

        CINumber::_pstrThousandSeparator->ReleaseBuffer();

#ifndef _UNICODE
        //
        // Some countries have a space as a 1000 separator,
        // but for some reason, this is ansi 160, which
        // shows up as a space fine on windows apps,
        // looks like garbage on console apps.
        //
        if ((*CINumber::_pstrThousandSeparator)[0] == CHAR(160))
        {
            CINumber::_pstrThousandSeparator->SetAt(0, ' ');
            TRACEEOLID("Space 1000 separator substituted");
        }
#endif // _UNICODE

        //
        // Get currency symbol
        //
        if (!::GetLocaleInfo(
            lcid,
            LOCALE_SCURRENCY | lctype,
            CINumber::_pstrCurrency->GetBuffer(MAXLEN),
            MAXLEN
            ))
        {
            TRACEEOLID("Unable to get currency symbol");
            ++cErrors;
        }

        CINumber::_pstrCurrency->ReleaseBuffer();
    }

    catch(CMemoryException * e)
    {
        TRACEEOLID("!!!exception in getting intl settings:");
        e->ReportError();
        e->Delete();
        ++cErrors;
    }

    TRACEEOLID("Thousand Separator . . . . . : " << *CINumber::_pstrThousandSeparator);
    TRACEEOLID("Decimal Point  . . . . . . . : " << *CINumber::_pstrDecimalPoint);
    TRACEEOLID("Currency Symbol. . . . . . . : " << *CINumber::_pstrCurrency);
    TRACEEOLID("Bad number . . . . . . . . . : " << *CINumber::_pstrBadNumber);
    TRACEEOLID("Currency Prefix. . . . . . . : " << CINumber::_fCurrencyPrefix);

    CINumber::_fInitialized = TRUE;

    LowerThreadProtection();

    return cErrors == 0;
}



/* static */
double
CINumber::BuildFloat(
    IN const LONG lInteger,
    IN const LONG lFraction
    )
/*++

Return Value:

    Combine integer and fraction to form float

Parameters:

    const LONG lInteger       Integer portion
    const LONG lFraction      Fractional portion

Return Value:

    float value

--*/
{
    double flValue = 0.0;

    //
    // Negative fractions?
    //
    ASSERT(lFraction >= 0);

    if (lFraction >= 0)
    {
        flValue = (double)lFraction;

        while (flValue >= 1.0)
        {
            flValue /= 10.0;
        }

        //
        // Re-add (or subtract if the original number
        // was negative) the fractional part
        //
        if (lInteger > 0L)
        {
            flValue += (double)lInteger;
        }
        else
        {
            flValue -= (double)lInteger;
            flValue = -flValue;
        }
    }

    return flValue;
}



/* static */
LPCTSTR
CINumber::ConvertLongToString(
    IN  const LONG lSrc,
    OUT CString & str
    )
/*++

CINumber::ConvertLongToString

Purpose:

    Convert long number to string with 1000 separators

Parameters:

    const LONG lSrc         Source number
    CString & str           String to write to

Return Value:

    Pointer to converted string

--*/
{
    LPTSTR lpOutString = str.GetBuffer(16);

    //
    // Forget about the negative sign for now.
    //
    LONG lNum = (lSrc >= 0L) ? lSrc : -lSrc;
    int outstrlen = 0;

    do
    {
        lpOutString[outstrlen++] = _T('0') + (TCHAR)(lNum % 10L);
        lNum /= 10L;

        //
        // if more digits left and we're on a 1000 boundary (printed 3 digits,
        // or 3 digits + n*(3 digits + 1 comma), then print a 1000 separator.
        // Note: will only work if the 1000 separator is 1 character.
        //
        ASSERT(CINumber::_pstrThousandSeparator->GetLength() == 1);

        if (lNum != 0L && (outstrlen == 3 || outstrlen == 7 || outstrlen == 11))
        {
            lstrcpy(lpOutString + outstrlen, *CINumber::_pstrThousandSeparator);
            outstrlen += CINumber::_pstrThousandSeparator->GetLength();
        }

    }
    while (lNum > 0L);

    //
    // Add a negative sign if necessary.
    //
    if (lSrc < 0L)
    {
        lpOutString[outstrlen++] = _T('-');
    }

    str.ReleaseBuffer(outstrlen);
    str.MakeReverse();

    return (LPCTSTR)str;
}



/* static */
LPCTSTR
CINumber::ConvertFloatToString(
    IN const double flSrc,
    IN int nPrecision,
    OUT CString & str
    )
/*++

Routine Description:

    Convert floating point number to string represenation

Parameters:

    const double flSrc          Source floating point number
    int nPrecision              Number of decimal points
    CString & str               String to convert to

Return Value:

    Pointer to converted string.

--*/
{
    //
    // Forget about the negative sign for now,
    // and the fractional portion.
    //
    TCHAR szFraction[256];
    LPCTSTR lpFraction = NULL;

    ::_stprintf(szFraction, _T("%.*f"), nPrecision, flSrc);
    lpFraction = ::_tcschr(szFraction, _T('.') );
    ASSERT(lpFraction != NULL);
    ++lpFraction;

    CINumber::ConvertLongToString((LONG)flSrc, str);

    str += *CINumber::_pstrDecimalPoint + lpFraction;

    return (LPCTSTR)str;
}



/* static */
BOOL
CINumber::ConvertStringToLong(
    IN  LPCTSTR lpsrc,
    OUT LONG & lValue
    )
/*++

Routine Description:

    Convert string to long integer.  1000 Separators will be treated
    correctly.

Parameters:

    LPCTSTR lpsrc       Source string
    LONG & lValue       Value to convert to.  Will be 0 in case of error

Return Value:

    TRUE for success, FALSE for failure.

--*/
{
    CString strNumber(lpsrc);
    LONG lBase = 1L;
    BOOL fNegative = FALSE;

    lValue = 0L;

    //
    // Empty strings are invalid
    //
    if (strNumber.IsEmpty())
    {
        return FALSE;
    }

    //
    // Check for negative sign (at the end only)
    //
    if (strNumber[0] == _T('-'))
    {
        fNegative = TRUE;
    }

    strNumber.MakeReverse();

    //
    // Strip negative sign
    //
    if (fNegative)
    {
        strNumber.ReleaseBuffer(strNumber.GetLength()-1);
    }

    //
    // Make sure the 1000 separator is only 1 char.  See note below
    //
    ASSERT(CINumber::_pstrThousandSeparator->GetLength() == 1);

    for (int i = 0; i < strNumber.GetLength(); ++i)
    {
        if ((strNumber[i] >= _T('0')) && (strNumber[i] <= _T('9')))
        {
            LONG lDigit = (LONG)(strNumber[i] - _T('0'));
            if (lDigit != 0L)
            {
                LONG lOldValue = lValue;
                LONG lDelta = (lDigit * lBase);
                if (lDelta / lDigit != lBase)
                {
                    TRACEEOLID("Overflow!");
                    lValue = 0x7fffffff;

                    return FALSE;
                }

                lValue += lDelta;

                if (lValue - lDelta != lOldValue)
                {
                    TRACEEOLID("Overflow!");
                    lValue = 0x7fffffff;

                    return FALSE;
                }
            }

            lBase *= 10L;
        }
        //
        // It's not a digit, maybe a thousand separator?
        // CAVEAT: If a thousand separator of more than
        //         one character is used, this won't work.
        //
        else if ((strNumber[i] != (*CINumber::_pstrThousandSeparator)[0])
             || (i != 3) && (i != 7) && (i != 11))
        {
            //
            // This is just invalid, since it is not a thousand
            // separator in the proper location, nor a negative
            // sign.
            //
            TRACEEOLID("Invalid character " << (BYTE)strNumber[i] << " encountered");
            return FALSE;
        }
    }

    if (fNegative)
    {
        lValue = -lValue;
    }

    return TRUE;
}



/* static */
BOOL
CINumber::ConvertStringToFloat(
    IN  LPCTSTR lpsrc,
    OUT double & flValue
    )
/*++

Routine Description:

    Convert fully decorated floating point string to double

Parameters:

    LPCTSTR lpsrc       Source string
    double & flValue    float value generated from string

Return Value:

    TRUE for success, FALSE for failure

--*/
{
    CString strNumber(lpsrc);

    //
    // This only works if the decimal point is a
    // single character
    //
    ASSERT(CINumber::_pstrDecimalPoint->GetLength() == 1);

    //
    // Strip off the > 0 part
    //
    LONG lFraction = 0;

    int nPoint = strNumber.ReverseFind((*CINumber::_pstrDecimalPoint)[0]);

    if (nPoint >= 0)
    {
        //
        // Convert fractional part
        //
        LPCTSTR lpszFraction = (LPCTSTR)strNumber + ++nPoint;
        lFraction = ::_ttol(lpszFraction);
        strNumber.ReleaseBuffer(--nPoint);
    }

    //
    // Convert integer part
    //
    LONG lInteger;

    if (ConvertStringToLong(strNumber, lInteger))
    {
        flValue = CINumber::BuildFloat(lInteger, lFraction);
        return TRUE;
    }

    return FALSE;
}



CILong::CILong()
/*++

Routine Description:

    Constructor without arguments

Parameters:

    None.

Return Value:

    N/A

--*/
    : m_lValue(0L)
{
}



CILong::CILong(
    IN LONG lValue
    )
/*++

Routine Description:

    Constructor taking LONG argument

Parameters:

    LONG lValue     Value to be set

Return Value:

    N/A

--*/
    : m_lValue(lValue)
{
}



CILong::CILong(
    IN LPCTSTR lpszValue
    )
/*++

Routine Description:

    Constructor taking string argument

Parameters:

    LPCTSTR lpszValue       String number

Return Value:

    N/A

--*/
{
    ConvertStringToLong(lpszValue, m_lValue);
}



CILong &
CILong::operator =(
    IN LONG lValue
    )
/*++

Routine Description:

    Assignment operator taking long value

Parameters:

    LONG lValue     Value to be set

Return Value:

    this object

--*/
{
    m_lValue = lValue;

    return *this;
}



CILong &
CILong::operator =(
    IN LPCTSTR lpszValue
    )
/*++

Routine Description:

    Assignment operator taking string value

Parameters:

    LPCTSTR lpszValue       String number

Return Value:

    this object

--*/
{
    ConvertStringToLong(lpszValue, m_lValue);

    return *this;
}



//
// Arithmetic Shorthand operators
//
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



CILong &
CILong::operator +=(
    IN const LONG lValue
    )
{
    m_lValue += lValue;

    return *this;
}



//
// As above
//
CILong &
CILong::operator +=(
    IN const LPCTSTR lpszValue
    )
{
    LONG lValue;

    ConvertStringToLong(lpszValue, lValue);

    m_lValue += lValue;

    return *this;
}



//
// As above
//
CILong &
CILong::operator +=(
    IN const CILong& value
    )
{
    m_lValue += value.m_lValue;

    return *this;
}



//
// As above
//
CILong &
CILong::operator -=(
    IN const LONG lValue
    )
{
    m_lValue -= lValue;

    return *this;
}



//
// As above
//
CILong &
CILong::operator -=(
    IN const LPCTSTR lpszValue
    )
{
    LONG lValue;

    ConvertStringToLong(lpszValue, lValue);

    m_lValue -= lValue;

    return *this;
}



//
// As above
//
CILong &
CILong::operator -=(
    IN const CILong & value
    )
{
    m_lValue -= value.m_lValue;

    return *this;
}



//
// As above
//
CILong &
CILong::operator *=(
    IN const LONG lValue
    )
{
    m_lValue *= lValue;

    return *this;
}



//
// As above
//
CILong &
CILong::operator *=(
    IN const LPCTSTR lpszValue
    )
{
    LONG lValue;

    ConvertStringToLong(lpszValue, lValue);

    m_lValue *= lValue;

    return *this;
}



//
// As above
//
CILong &
CILong::operator *=(
    IN const CILong & value
    )               
{
    m_lValue *= value.m_lValue;

    return *this;
}



//
// As above
//
CILong &
CILong::operator /=(
    IN const LONG lValue
    )
{
    m_lValue /= lValue;

    return *this;
}



//
// As above
//
CILong &
CILong::operator /=(
    IN const LPCTSTR lpszValue
    )
{
    LONG lValue;

    ConvertStringToLong(lpszValue, lValue);
    if (lValue != 0)
    {
      m_lValue /= lValue;
    }

    return *this;
}



//
// As above
//
CILong &
CILong::operator /=(
    IN const CILong& value
    )
{
    m_lValue /= value.m_lValue;

    return *this;
}



