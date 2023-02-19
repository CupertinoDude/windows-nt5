#ifndef _STRUTIL_H_
#define _STRUTIL_H_

#include <stock.h>
#include <wincrypt.h>


INLINE BOOL
IsEmptyStringA(LPCSTR pcsz)
{
	return (NULL == pcsz) || ('\0' == *pcsz);
}

INLINE BOOL
IsEmptyStringW(LPCWSTR pcwsz)
{
	return (NULL == pcwsz) || (L'\0' == *pcwsz);
}

#if defined(UNICODE)
#define IsEmptyString IsEmptyStringW
#else // defined(UNICODE)
#define IsEmptyString IsEmptyStringA
#endif // defined(UNICODE)


#define IS_EMPTY_STRING(s)	  (IsEmptyString(s))
#define FEmptySz(psz)         (IsEmptyString(psz))

#define SetEmptySz(psz)       (*(psz) = _T('\0'))

	
// global helper functions for Unicode support in a DBCS environment

int      UnicodeCompare(PCWSTR s, PCWSTR t);
PWSTR    NewUnicodeString(PCWSTR wszText);
BOOL     UnicodeIsNumber(PCWSTR wszText);
PWSTR    DBCSToUnicode(UINT uCodePage, PCSTR szText);
PSTR     UnicodeToDBCS(UINT uCodePage, PCWSTR wszText);

INLINE PWSTR AnsiToUnicode(PCSTR szText)
{
	return DBCSToUnicode(CP_ACP, szText);
}

INLINE PWSTR OEMToUnicode(PCSTR szText)
{
	return DBCSToUnicode(CP_OEMCP, szText);
}

INLINE PSTR UnicodeToAnsi(PCWSTR wszText)
{
	return UnicodeToDBCS(CP_ACP, wszText);
}

INLINE PSTR UnicodeToOEM(PCWSTR wszText)
{
	return UnicodeToDBCS(CP_OEMCP, wszText);
}


// Functions to convert between Quad Words (expressed as ULARGE_INTEGERs)
// and ANSI strings.
BOOL  HexStringToQWordA(LPCSTR pcszString, ULARGE_INTEGER* pqw);
int  QWordToHexStringA(ULARGE_INTEGER qw, LPSTR pszString);

// Function to convert from a hex string to a DWORD.
DWORD    DwFromHex(LPCTSTR pchHex);

// CCHMAX_HEX_ULARGE_INTEGER - defines the minimum string buffer size needed
// for the second parameter of QWordToHexStringA().
#define CCHMAX_HEX_ULARGE_INTEGER 17 // 16 characters + n.t.

#define CCH_HEX_DWORD		8
#define CCH_HEX_QWORD		16
#define BITS_PER_HEX_CHAR	4


// Other Random string functions
VOID     GuidToSz(GUID * pguid, LPTSTR lpchDest);
/* sizeof(GUID)*2 + 7 (includes NULL terminator) characters (see GuidToSz) */
#define LENGTH_SZGUID_FORMATTED 39


int WINAPI RtStrToInt(LPCTSTR lpSrc);       // atoi()
#define ATOI					RtStrToInt

#ifdef __cplusplus
extern "C"{
#endif
UINT     DecimalStringToUINT(LPCTSTR pcszString);
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C"{
#endif
LPCTSTR  _StrChr(LPCTSTR psz, TCHAR c);
int      _StrCmpN(LPCTSTR psz1, LPCTSTR psz2, UINT maxChars);
#ifdef __cplusplus
}
#endif
LPCTSTR  _StrStr(LPCTSTR pcsz1, LPCTSTR pcsz2);
LPCWSTR  _StrStrW(LPCWSTR pcsz1, LPCWSTR pcsz2);

LPSTR	 _StrPbrkA(LPCSTR pcszString, LPCSTR pcszSearch);
LPWSTR	 _StrPbrkW(LPCWSTR pcszString, LPCWSTR pcszSearch);

#if defined(UNICODE)
#define _StrPbrk _StrPbrkW
#else // defined(UNICODE)
#define _StrPbrk _StrPbrkA
#endif // defined(UNICODE)

LPTSTR   SzFindLastCh(LPTSTR lpsz, TCHAR ch);
UINT     TrimSz(PTCHAR psz);

// Local LStrLenW function is unnecessary, since Windows 95 supports
// lstrlenW natively
#define LStrLenW lstrlenW

// Map LStrCpyW to its Win32 equivalent for Unicode builds
#if defined UNICODE
#define LStrCpyW lstrcpyW
#define LStrCpyNW lstrcpyn
#else // defined UNICODE
LPWSTR   LStrCpyW(LPWSTR pszDest, LPWSTR pszSrc);
LPWSTR   LStrCpyNW(LPWSTR pszDest, LPCWSTR pszSrc, INT iMaxLength);
#endif // defined UNICODE

LPWSTR  _StrLwrW(LPWSTR pwszSrc);

#ifdef __cplusplus
class CHash
{
public:
	CHash();
	~CHash();
	DWORD GetHashedData(PBYTE pbData, DWORD cbData, void ** ppvHashedData);

private:
	HCRYPTPROV      m_hProv;
	HCRYPTHASH      m_hHash;
	PBYTE			m_pbHashedData;
	DWORD			m_cbHashedData;
	BOOL			m_fReady;
};
#endif
	
DWORD  HashPasswd(PBYTE pbPasswd, DWORD cbPasswd, void **ppvData);

#endif // ndef STRUTIL_H
