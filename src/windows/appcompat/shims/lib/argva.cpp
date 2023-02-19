/***
*argvw.c - create Unicode version of argv arguments
*
*       Copyright (c) 1989-1993, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       processes program command line
*
*Revision History:
    08/14/2001  robkenny    Moved code inside the ShimLib namespace.
*
*******************************************************************************/

// This routine actually lives in shell32.dll,
// we have a private copy so we don't have to link to the library.

#include <windows.h>


namespace ShimLib
{

/***
*void Parse_Cmdline(cmdstart, argv, lpstr, numargs, numbytes)
*
*Purpose:
*       Parses the command line and sets up the Unicode argv[] array.
*       On entry, cmdstart should point to the command line,
*       argv should point to memory for the argv array, lpstr
*       points to memory to place the text of the arguments.
*       If these are NULL, then no storing (only counting)
*       is done.  On exit, *numargs has the number of
*       arguments (plus one for a final NULL argument),
*       and *numbytes has the number of bytes used in the buffer
*       pointed to by args.
*
*Entry:
*       LPSTR cmdstart - pointer to command line of the form
*           <progname><nul><args><nul>
*       TCHAR **argv - where to build argv array; NULL means don't
*                      build array
*       LPSTR lpstr - where to place argument text; NULL means don't
*                      store text
*
*Exit:
*       no return value
*       INT *numargs - returns number of argv entries created
*       INT *numbytes - number of bytes used in args buffer
*
*Exceptions:
*
*******************************************************************************/

void Parse_CmdlineA (
    LPCSTR cmdstart,
    LPSTR* argv,
    LPSTR  lpstr,
    INT *  numargs,
    INT *  numbytes
    )
{
    LPCSTR p;
    char c;
    INT inquote;                    /* 1 = inside quotes */
    INT copychar;                   /* 1 = copy char to *args */
    WORD numslash;                  /* num of backslashes seen */

    *numbytes = 0;
    *numargs = 1;                   /* the program name at least */

    /* first scan the program name, copy it, and count the bytes */
    p = cmdstart;
    if (argv)
        *argv++ = lpstr;

    /* A quoted program name is handled here. The handling is much
       simpler than for other arguments. Basically, whatever lies
       between the leading double-quote and next one, or a terminal null
       character is simply accepted. Fancier handling is not required
       because the program name must be a legal NTFS/HPFS file name.
       Note that the double-quote characters are not copied, nor do they
       contribute to numbytes. */
    if (*p == TEXT('\"'))
    {
        /* scan from just past the first double-quote through the next
           double-quote, or up to a null, whichever comes first */
        while ((*(++p) != TEXT('\"')) && (*p != TEXT('\0')))
        {
            *numbytes += sizeof(char);
            if (lpstr)
                *lpstr++ = *p;
        }
        /* append the terminating null */
        *numbytes += sizeof(char);
        if (lpstr)
            *lpstr++ = TEXT('\0');

        /* if we stopped on a double-quote (usual case), skip over it */
        if (*p == TEXT('\"'))
            p++;
    }
    else
    {
        /* Not a quoted program name */
        do {
            *numbytes += sizeof(char);
            if (lpstr)
                *lpstr++ = *p;

            c = (char) *p++;

        } while (c > TEXT(' '));

        if (c == TEXT('\0'))
        {
            p--;
        }
        else
        {
            if (lpstr)
                *(lpstr - 1) = TEXT('\0');
        }
    }

    inquote = 0;

    /* loop on each argument */
    for ( ; ; )
    {
        if (*p)
        {
            while (*p == TEXT(' ') || *p == TEXT('\t'))
                ++p;
        }

        if (*p == TEXT('\0'))
            break;                  /* end of args */

        /* scan an argument */
        if (argv)
            *argv++ = lpstr;         /* store ptr to arg */
        ++*numargs;

        /* loop through scanning one argument */
        for ( ; ; )
        {
            copychar = 1;
            /* Rules: 2N backslashes + " ==> N backslashes and begin/end quote
                      2N+1 backslashes + " ==> N backslashes + literal "
                      N backslashes ==> N backslashes */
            numslash = 0;
            while (*p == TEXT('\\'))
            {
                /* count number of backslashes for use below */
                ++p;
                ++numslash;
            }
            if (*p == TEXT('\"'))
            {
                /* if 2N backslashes before, start/end quote, otherwise
                   copy literally */
                if (numslash % 2 == 0)
                {
                    if (inquote)
                        if (p[1] == TEXT('\"'))
                            p++;    /* Double quote inside quoted string */
                        else        /* skip first quote char and copy second */
                            copychar = 0;
                    else
                        copychar = 0;       /* don't copy quote */

                    inquote = !inquote;
                }
                numslash /= 2;          /* divide numslash by two */
            }

            /* copy slashes */
            while (numslash--)
            {
                if (lpstr)
                    *lpstr++ = TEXT('\\');
                *numbytes += sizeof(char);
            }

            /* if at end of arg, break loop */
            if (*p == TEXT('\0') || (!inquote && (*p == TEXT(' ') || *p == TEXT('\t'))))
                break;

            /* copy character into argument */
            if (copychar)
            {
                if (lpstr)
                        *lpstr++ = *p;
                *numbytes += sizeof(char);
            }
            ++p;
        }

        /* null-terminate the argument */

        if (lpstr)
            *lpstr++ = TEXT('\0');         /* terminate string */
        *numbytes += sizeof(char);
    }

}


/***
*CommandLineToArgvW - set up Unicode "argv" for C programs
*
*Purpose:
*       Read the command line and create the argv array for C
*       programs.
*
*Entry:
*       Arguments are retrieved from the program command line
*
*Exit:
*       "argv" points to a null-terminated list of pointers to UNICODE
*       strings, each of which is an argument from the command line.
*       The list of pointers is also located on the heap or stack.
*
*Exceptions:
*       Terminates with out of memory error if no memory to allocate.
*
*******************************************************************************/

LPSTR * _CommandLineToArgvA (LPCSTR lpCmdLine, int*pNumArgs)
{
    LPSTR*argv_U;
    LPCSTR cmdstart;                 /* start of command line to parse */
    INT    numbytes;

    if (pNumArgs == NULL) {
        SetLastError(ERROR_INVALID_PARAMETER);
        return NULL;
    }

    cmdstart = lpCmdLine;

    /* first find out how much space is needed to store args */
    Parse_CmdlineA(cmdstart, NULL, NULL, pNumArgs, &numbytes);

    /* allocate space for argv[] vector and strings */
    argv_U = (LPSTR*)LocalAlloc(LMEM_ZEROINIT,
                                (*pNumArgs + 1) * sizeof(LPSTR) + numbytes);
    if (!argv_U) {
        SetLastError(ERROR_NOT_ENOUGH_MEMORY);
        return (NULL);
    }

    /* store args and argv ptrs in just allocated block */
    Parse_CmdlineA(cmdstart,
                   argv_U,
                   (LPSTR)(((LPBYTE)argv_U) + *pNumArgs * sizeof(LPSTR)),
                   pNumArgs,
                   &numbytes);

    return argv_U;
}


};  // end of namespace ShimLib
