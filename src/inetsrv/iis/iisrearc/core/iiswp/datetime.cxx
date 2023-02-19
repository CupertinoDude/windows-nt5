/*++

   Copyright    (c)    1999    Microsoft Corporation

   Module  Name :

      DateTime.cxx

   Abstract:

      Date parsing routines thanx to JohnL (01/24/95)

--*/

#include "precomp.hxx"

# if !defined(dllexp)
# define dllexp __declspec( dllexport)
# endif

/************************************************************
 *   Data
 ************************************************************/
 
static const WCHAR* s_rgchDays[] =  {
    TEXT("Sun"), TEXT("Mon"), TEXT("Tue"), TEXT("Wed"),
    TEXT("Thu"), TEXT("Fri"), TEXT("Sat")
};

static const WCHAR* s_rgchMonths[] = {
    TEXT("Jan"), TEXT("Feb"), TEXT("Mar"), TEXT("Apr"),
    TEXT("May"), TEXT("Jun"), TEXT("Jul"), TEXT("Aug"),
    TEXT("Sep"), TEXT("Oct"), TEXT("Nov"), TEXT("Dec")
};

// Custom hash table for make_month() for mapping "Apr" to 4
static const CHAR MonthIndexTable[64] = {
   -1,'A',  2, 12, -1, -1, -1,  8, // A to G
   -1, -1, -1, -1,  7, -1,'N', -1, // F to O
    9, -1,'R', -1, 10, -1, 11, -1, // P to W
   -1,  5, -1, -1, -1, -1, -1, -1, // X to Z
   -1,'A',  2, 12, -1, -1, -1,  8, // a to g
   -1, -1, -1, -1,  7, -1,'N', -1, // f to o
    9, -1,'R', -1, 10, -1, 11, -1, // p to w
   -1,  5, -1, -1, -1, -1, -1, -1  // x to z
};


static const BYTE TensDigit[10] = { 0, 10, 20, 30, 40, 50, 60, 70, 80, 90 };


/************************************************************
 *    Functions
 ************************************************************/

WORD
iis_2atoi(
    PWCHAR s
    )
/*++

    Converts a 2 character string to integer

    Arguments:
        s   String to convert

    Returns:
        numeric equivalent, 0 on failure.
--*/
{

    DWORD tens = s[0] - L'0';
    DWORD ones = s[1] - L'0';

    if ( (tens <= 9) && (ones <= 9) ) {
        return((WORD)(TensDigit[tens] + ones));
    }
    return(0);
}

WORD
make_month(
    PWCHAR s
    )
{
    UCHAR monthIndex;
    UCHAR c;
    LPCTSTR monthString;

    //
    // use the third character as the index
    //

    c = ((CHAR) s[2] - 0x40) & 0x3F;

    monthIndex = MonthIndexTable[c];

    if ( monthIndex < 13 ) {
        goto verify;
    }

    //
    // ok, we need to look at the second character
    //

    if ( monthIndex == 'N' ) {

        //
        // we got an N which we need to resolve further
        //

        //
        // if s[1] is 'u' then Jun, if 'a' then Jan
        //

        if ( MonthIndexTable[(s[1]-0x40) & 0x3f] == 'A' ) {
            monthIndex = 1;
        } else {
            monthIndex = 6;
        }

    } else if ( monthIndex == 'R' ) {

        //
        // if s[1] is 'a' then March, if 'p' then April
        //

        if ( MonthIndexTable[(s[1]-0x40) & 0x3f] == 'A' ) {
            monthIndex = 3;
        } else {
            monthIndex = 4;
        }
    } else {
        goto error_exit;
    }

verify:

    monthString = s_rgchMonths[monthIndex-1];

    if ( (s[0] == monthString[0]) &&
         (s[1] == monthString[1]) &&
         (s[2] == monthString[2]) ) {

        return(monthIndex);

    } else if ( (toupper(s[0]) == monthString[0]) &&
                (tolower(s[1]) == monthString[1]) &&
                (tolower(s[2]) == monthString[2]) ) {

        return monthIndex;
    }

error_exit:
    return(0);

} // make_month

BOOL
NtSystemTimeToLargeInteger(
    IN  const SYSTEMTIME * pst,
    OUT LARGE_INTEGER *    pli
    )
{

    FILETIME ft;

    //
    // Convert to file time
    //

    if ( !SystemTimeToFileTime( pst, &ft ) ) {
        return(FALSE);
    }

    //
    // Convert file time to large integer
    //

    pli->LowPart = ft.dwLowDateTime;
    pli->HighPart = ft.dwHighDateTime;

    return(TRUE);
}

dllexp
BOOL
StringTimeToFileTime(
    IN  const WCHAR * pszTime,
    OUT __int64 * pliTime
    )
/*++

  Converts a string representation of a GMT time (three different
  varieties) to an NT representation of a file time.

  We handle the following variations:

    Sun, 06 Nov 1994 08:49:37 GMT   (RFC 822 updated by RFC 1123)
    Sunday, 06-Nov-94 08:49:37 GMT  (RFC 850)
    Sun Nov  6 08:49:37 1994        (ANSI C's asctime() format

  Arguments:
    pszTime             String representation of time field
    pliTime             large integer containing the time in NT format.

  Returns:
    TRUE on success and FALSE on failure.

  History:

    Johnl       24-Jan-1995     Modified from WWW library

--*/
{

    WCHAR *         s;
    SYSTEMTIME      st;
    LARGE_INTEGER   liTime;

    if (pszTime == NULL) {
        SetLastError( ERROR_INVALID_PARAMETER );
        return FALSE;
    }

    st.wMilliseconds = 0;

    if ((s = wcschr(pszTime, ','))) {

        DWORD len;

        //
        // Thursday, 10-Jun-93 01:29:59 GMT
        // or: Thu, 10 Jan 1993 01:29:59 GMT */
        //

        s++;

        while (*s && *s==' ') s++;
        len = wcslen(s);

        if (len < 18) {
            SetLastError( ERROR_INVALID_PARAMETER );
            return FALSE;
        }

        if ( *(s+2) == '-' ) {        /* First format */

            st.wDay = (WORD) _wtoi(s);
            st.wMonth = (WORD) make_month(s+3);
            st.wYear = (WORD) _wtoi(s+7);
            st.wHour = (WORD) _wtoi(s+10);
            st.wMinute = (WORD) _wtoi(s+13);
            st.wSecond = (WORD) _wtoi(s+16);

        } else {                /* Second format */

            if (len < 20) {
                SetLastError( ERROR_INVALID_PARAMETER );
                return FALSE;
            }

            st.wDay = iis_2atoi(s);
            st.wMonth = make_month(s+3);
            st.wYear = iis_2atoi(s+7) * 100  +  iis_2atoi(s+9);
            st.wHour = iis_2atoi(s+12);
            st.wMinute = iis_2atoi(s+15);
            st.wSecond = iis_2atoi(s+18);

        }
    } else {    /* Try the other format:  Wed Jun  9 01:29:59 1993 GMT */

        s = (WCHAR *) pszTime;
        while (*s && *s==' ') s++;

        if ((int)wcslen(s) < 24) {
            SetLastError( ERROR_INVALID_PARAMETER );
            return FALSE;
        }

        st.wDay = (WORD) _wtoi(s+8);
        st.wMonth = (WORD) make_month(s+4);
        st.wYear = (WORD) _wtoi(s+20);
        st.wHour = (WORD) _wtoi(s+11);
        st.wMinute = (WORD) _wtoi(s+14);
        st.wSecond = (WORD) _wtoi(s+17);
    }

    //
    //  Adjust for dates with only two digits
    //

    if ( st.wYear < 1000 ) {
        if ( st.wYear < 50 ) {
            st.wYear += 2000;
        } else {
            st.wYear += 1900;
        }
    }

    if ( !NtSystemTimeToLargeInteger( &st, &liTime )) {
        SetLastError( ERROR_INVALID_PARAMETER );
        return FALSE;
    }
    
    *pliTime = liTime.QuadPart;
    
    return(TRUE);
}
