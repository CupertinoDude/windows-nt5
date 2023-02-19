/*++

Copyright (c) 1995  Microsoft Corporation

Module Name:

    parmlist.cxx

Abstract:

    Simple class for parsing and storing parameter list pairs

Author:

    John Ludeman (johnl)   22-Feb-1995

Revision History:

--*/

//
//  System include files.
//

#include "precomp.hxx"

inline
HRESULT
UnescapeStr( 
    STRA * pstr 
    )
{
    CHAR * pch;

    pch = pstr->QueryStr();
    while ( pch = strchr( pch, '+' ) )
    {
        *pch = ' ';
    }

    return pstr->Unescape();
}

PARAM_LIST::~PARAM_LIST(
    VOID
    )
/*++

Routine Description:

    Param list destructor

--*/
{
    FIELD_VALUE_PAIR * pFVP;

    while ( !IsListEmpty( &_FieldListHead ) )
    {
        pFVP = CONTAINING_RECORD( _FieldListHead.Flink,
                                  FIELD_VALUE_PAIR,
                                  ListEntry );

        DBG_ASSERT( pFVP != NULL );

        RemoveEntryList( &pFVP->ListEntry );

        delete( pFVP );
        pFVP = NULL;
    }

    while ( !IsListEmpty( &_FreeHead ))
    {
        pFVP = CONTAINING_RECORD( _FreeHead.Flink,
                                  FIELD_VALUE_PAIR,
                                  ListEntry );

        DBG_ASSERT( pFVP != NULL );

        RemoveEntryList( &pFVP->ListEntry );

        delete( pFVP );
        pFVP = NULL;
    }
}

VOID
PARAM_LIST::Reset(
    VOID
    )
/*++

Routine Description:

    Resets the parameter list back to its initially constructed state

--*/
{
    FIELD_VALUE_PAIR * pFVP;

    while ( !IsListEmpty( &_FieldListHead ) )
    {
        pFVP = CONTAINING_RECORD( _FieldListHead.Flink,
                                  FIELD_VALUE_PAIR,
                                  ListEntry );

        DBG_ASSERT( pFVP != NULL );

        RemoveEntryList( &pFVP->ListEntry );

        //
        //  Put the removed item on the end so the same entry will 
        //  tend to be used for the same purpose on the next use
        //

        InsertTailList( &_FreeHead, &pFVP->ListEntry );
    }

    _fCanonicalized = FALSE;
}

HRESULT
PARAM_LIST::ParsePairs(
    const CHAR * pszList,
    BOOL         fDefaultParams,
    BOOL         fAddBlankValues,
    BOOL         fCommaIsDelim
    )
/*++

Routine Description:

    Puts the text list into a linked list of field/value pairs

    This can be used to parse lists in the form of:

    "a=b,c=d,e=f" (with fCommaIsDelim = TRUE)
    "name=Joe, Billy\nSearch=tom, sue, avery"(with fCommaIsDelim = FALSE)

    Duplicates will be appended and tab separated

Arguments:

    pszList - list of comma separated field/value pairs
    fDefaultParams - If TRUE, means these parameters are only defaults 
                     and shouldn't be added to the list if the field 
                     name is already in the list and the value is 
                     non-empty.
    fAddBlankValues - if TRUE, allow fields with empty values to be 
                      added to the list, else ignore empty values.
    fCommaIsDelim - if TRUE, a comma acts as a separator between two 
                    sets of fields values, otherwise the comma is 
                    ignored

Return Value:

    TRUE if successful, FALSE on error

--*/
{
    CHAR *             pch;
    DWORD              cParams = 0;
    DWORD              i;
    BOOL               fRet;
    STRA               strParams;
    STRA               strField;
    HRESULT            hr;

    //
    //  Make a copy we can muck with
    //

    hr = strParams.Copy( pszList );
    if ( FAILED( hr ) )
    {
        DBGPRINTF(( DBG_CONTEXT,
                   "Error copying field/value pairs list, hr = 0x%x.\n",
                   hr ));

        return hr;
    }

    //
    //  Replace all of the equal signs and commas with '\n's for 
    //  easier parsing
    //

    pch = strParams.QueryStr();
    while ( pch = strchr( pch, '=' ) )
    {
        *pch = '\n';
        cParams++;
    }

    if ( fCommaIsDelim )
    {
        pch = strParams.QueryStr();
        while ( pch = strchr( pch, ',' ) )
        {
            *pch = '\n';
            cParams++;
        }
    }

    //
    //  Pick out the fields and values and build the associative list
    //

    ODBC_PARSER Parser( strParams.QueryStr() );

    for ( i = 0; i < cParams; i++ )
    {
        hr = strField.Copy( Parser.QueryToken() );
        if ( FAILED( hr ) )
        {
            DBGPRINTF(( DBG_CONTEXT,
                       "Error copying token, hr = 0x%x.\n",
                       hr ));
            return hr;
        }

        Parser.NextLine();

        pch = Parser.QueryToken();

        //
        //  If we aren't supposed to add blanks, then just go to the next
        //  line
        //

        if ( !fAddBlankValues && !*pch )
        {
            Parser.NextLine();
            continue;
        }

        if ( !fDefaultParams )
        {
            FIELD_VALUE_PAIR * pFVP;
            LIST_ENTRY *       ple;

            //
            //  Look for an existing field with this name and append
            //  the value there if we find it, otherwise add a new entry
            //

            for ( ple  = _FieldListHead.Flink;
                  ple != &_FieldListHead;
                  ple  = ple->Flink )
            {
                pFVP = CONTAINING_RECORD( ple, 
                                          FIELD_VALUE_PAIR, 
                                          ListEntry );

                if ( !_stricmp( pFVP->QueryField(),
                               strField.QueryStr() ))
                {
                    //
                    //  CODEWORK - Remove this allocation
                    //

                    STRA strtmp;

                    hr = strtmp.Copy( pch );
                    if( FAILED( hr ) )
                    {
                        DBGPRINTF(( DBG_CONTEXT,
                                    "Error copying, hr = 0x%x.\n",
                                    hr ));
                        return hr;
                    }

                    //
                    //  Found this header, append the new value
                    //

                    pFVP->_cValues++;

                    hr = UnescapeStr( &strtmp );
                    if( FAILED( hr ) )
                    {
                        DBGPRINTF(( DBG_CONTEXT,
                            "Error unescaping string, hr = 0x%x.\n",
                            hr ));

                        return hr;
                    }       
                    
                    hr = pFVP->_strValue.Append( "\t" );
                    if( FAILED( hr ) )
                    {
                        DBGPRINTF(( DBG_CONTEXT,
                            "Error appending '\t', hr = 0x%x.\n",
                            hr ));
                        return hr;
                    } 
                         
                    hr = pFVP->_strValue.Append( strtmp );

                    goto Found;
                }
            }

            hr = AddEntry( strField.QueryStr(),
                           pch,
                           TRUE );
Found:
            ;
        }
        else
        {
            //
            //  Don't add if already in list
            //

            hr = AddParam( strField.QueryStr(),
                             pch );
        }

        if ( FAILED( hr ) )
        {
            DBGPRINTF(( DBG_CONTEXT,
                   "Error parsing pairs list, hr = 0x%x.\n",
                   hr ));

            return hr;
        }

        Parser.NextLine();
    }

    return hr;
}

HRESULT
PARAM_LIST::AddEntryUsingConcat(
    const CHAR * pszField,
    const CHAR * pszValue,
    BOOL  fPossibleFastMap
)
/*++

Routine Description:

    Concatenate value with existing entry of same name
    or call AddEntry if none exists

Arguments:

    pszField         - Field to add
    pszValue         - Value to add
    fPossibleFastMap - TRUE if entry is not known not to be
                       in the fast map


Return Value:

    TRUE if successful, FALSE on error

--*/
{
    //
    //  Look for an existing field with this name
    //  and add the value there
    //

    FIELD_VALUE_PAIR * pFVP;
    LIST_ENTRY *       ple;
    HRESULT            hr;

    //
    //  Find the field
    //

    for ( ple  = _FieldListHead.Flink;
          ple != &_FieldListHead;
          ple  = ple->Flink )
    {
        pFVP = CONTAINING_RECORD( ple, FIELD_VALUE_PAIR, ListEntry );

        if ( !_stricmp( pFVP->QueryField(),
                       pszField ))
        {
            //
            //  Found this header, append the new value
            //

            pFVP->_cValues++;

            hr = pFVP->_strValue.Append( "," );
            if( FAILED( hr ) )
            {
                DBGPRINTF(( DBG_CONTEXT,
                   "Error appending ',', hr = 0x%x.\n",
                   hr ));

                return hr;
            } 
            
            hr = pFVP->_strValue.Append( pszValue );

            goto Found;
        }
    }

    hr = AddEntry( pszField,
                   pszValue,
                   FALSE,
                   fPossibleFastMap );

Found:

    return hr;
}



HRESULT
PARAM_LIST::ParseSimpleList(
    const CHAR * pszList
    )
/*++

Routine Description:

    Puts the comma separated list into a linked list of field/value pairs
    where the value is always NULL

Arguments:

    pszList - list of comma separated fields

Return Value:

    TRUE if successful, FALSE on error

--*/
{
    CHAR *             pch;
    HRESULT            hr = S_OK;

    //
    //  Pick out the fields and values and build the associative list
    //

    ODBC_PARSER Parser( (CHAR *) pszList );

    Parser.SetListMode( TRUE );

    while ( *(pch = Parser.QueryToken()) )
    {
        hr = AddEntry( pch,
                       NULL,
                       TRUE );

        if ( FAILED( hr ) )
        {
            DBGPRINTF(( DBG_CONTEXT,
                   "Error AddEntry, hr = 0x%x.\n",
                   hr ));

            return hr;
        }

        Parser.NextItem();
    }

    return hr;
}

CHAR *
PARAM_LIST::FindValue(
    const CHAR * pszField,
    BOOL *       pfIsMultiValue  OPTIONAL,
    DWORD *      pcbValue OPTIONAL
    )
/*++

Routine Description:

    Returns the value associated with pszField or NULL of no value was
    found

Arguments:

    pszField - field to search for value for
    pfIsMultiValue - Set to TRUE if this value is a composite of multiple fields
    pcbValue - Set to size of value (excluding nul terminator)


Return Value:

    Pointer to value or NULL if not found

--*/
{
    FIELD_VALUE_PAIR * pFVP;
    LIST_ENTRY *       ple;

    //
    // Do we need to canon?
    //

    if ( !IsCanonicalized() )
    {
        CanonList( );
    }

    //
    //  Find the field
    //

    for ( ple  = _FieldListHead.Flink;
          ple != &_FieldListHead;
          ple  = ple->Flink )
    {
        pFVP = CONTAINING_RECORD( ple, FIELD_VALUE_PAIR, ListEntry );

        if ( !_stricmp( pFVP->QueryField(),
                       pszField ))
        {
            if ( pfIsMultiValue )
            {
                *pfIsMultiValue = pFVP->IsMultiValued();
            }

            if ( pcbValue )
            {
                *pcbValue = pFVP->_strValue.QueryCB();
            }

            return pFVP->QueryValue();
        }
    }


    return NULL;
}

HRESULT
PARAM_LIST::AddEntry(
    const CHAR * pszField,
    const CHAR * pszValue,
    BOOL         fUnescape,
    BOOL         fPossibleFastMap
    )
/*++

Routine Description:

    Unconditionally adds the field/value pair to the end of the list

Arguments:

    pszField         - Field to add
    pszValue         - Value to add
    fPossibleFastMap - TRUE if entry is not known not to be
                   in the fast map


Return Value:

    TRUE if successful, FALSE on error

--*/
{
    FIELD_VALUE_PAIR * pFVP;
    CHAR *             pch;
    HRESULT            hr;

    if ( !IsListEmpty( &_FreeHead ) )
    {
        LIST_ENTRY * pEntry;

        pEntry = _FreeHead.Flink;

        RemoveEntryList( _FreeHead.Flink );

        pFVP = CONTAINING_RECORD( pEntry, FIELD_VALUE_PAIR, ListEntry );

        pFVP->_strField.Reset();
        pFVP->_strValue.Reset();
    }
    else
    {
        pFVP = new FIELD_VALUE_PAIR;

        if ( !pFVP )
        {
            hr = HRESULT_FROM_WIN32( ERROR_NOT_ENOUGH_MEMORY );
            DBGPRINTF(( DBG_CONTEXT,
                   "Error creating new FIELD_VALUE_PAIR, hr = 0x%x.\n",
                   hr ));

            return hr;
        }
    }

    pFVP->_cValues = 1;

    //
    //  Add it to the list now so we don't have to worry about deleting it
    //  if one of the below copies fail
    //

    InsertTailList( &_FieldListHead, &pFVP->ListEntry );

    hr = pFVP->_strField.Copy( pszField );
    if ( FAILED( hr ) )
    {
        DBGPRINTF(( DBG_CONTEXT,
                   "Error copying field, hr = 0x%x.\n",
                   hr ));
        return hr;
    }

    hr = pFVP->_strValue.Copy( pszValue );
    if ( FAILED( hr ) )
    {
        DBGPRINTF(( DBG_CONTEXT,
                   "Error copying value, hr = 0x%x.\n",
                   hr ));
        return hr;
    }

    if ( fUnescape )
    {
        //
        //  Normalize the fields and values (unescape and replace
        //  '+' with ' ')
        //

        hr = UnescapeStr( &pFVP->_strField );
        if( FAILED( hr ) )
        {
            DBGPRINTF(( DBG_CONTEXT,
                   "Error unescaping field, hr = 0x%x.\n",
                   hr ));
            return hr;
        }

        hr = UnescapeStr( &pFVP->_strValue );
        if( FAILED( hr ) )
        {
            DBGPRINTF(( DBG_CONTEXT,
                   "Error unescaping value, hr = 0x%x.\n",
                   hr ));
            return hr;
        }
    }

    return hr;
}

HRESULT
PARAM_LIST::AddEntry(
    const CHAR * pszField,
    DWORD        cbField,
    const CHAR * pszValue,
    DWORD        cbValue
    )
/*++

Routine Description:

    Unconditionally adds the field/value pair to the end of the list

    The fast map is not used and the fields are not unescaped

Arguments:

    pszField         - Field to add
    cbField          - Number of bytes in pszField
    pszValue         - Value to add
    cbValue          - Number of bytes in pszValue

Return Value:

    TRUE if successful, FALSE on error

--*/
{
    FIELD_VALUE_PAIR *  pFVP;
    CHAR *              pch;
    PLIST_ENTRY         listEntry;
    HRESULT             hr;

    listEntry = RemoveHeadList( &_FreeHead );

    if ( listEntry != &_FreeHead )
    {
        pFVP = CONTAINING_RECORD(
                            listEntry,
                            FIELD_VALUE_PAIR,
                            ListEntry );

        pFVP->_strField.Reset();
        pFVP->_strValue.Reset();
    }
    else
    {
        pFVP = new FIELD_VALUE_PAIR;

        if ( !pFVP )
        {
            hr = HRESULT_FROM_WIN32( ERROR_NOT_ENOUGH_MEMORY );
            DBGPRINTF(( DBG_CONTEXT,
                   "Error creating new FIELD_VALUE_PAIR, hr = 0x%x.\n",
                   hr ));

            return hr;
        }
    }

    pFVP->_cValues = 1;

    //
    //  Add it to the list now so we don't have to worry about deleting it
    //  if one of the below copies fail
    //

    InsertTailList( &_FieldListHead, &pFVP->ListEntry );

    hr = pFVP->_strField.Copy( pszField, cbField );
    if ( FAILED( hr ) )
    {
        DBGPRINTF(( DBG_CONTEXT,
                   "Error copying field, hr = 0x%x.\n",
                   hr ));
        return hr;
    }

    hr = pFVP->_strValue.Copy( pszValue, cbValue );
    if ( FAILED( hr ) )
    {
        DBGPRINTF(( DBG_CONTEXT,
                   "Error copying value, hr = 0x%x.\n",
                   hr ));
        return hr;
    }

    return hr;
}

HRESULT 
PARAM_LIST::AddParam(
    const CHAR * pszField,
    const CHAR * pszValue
    )
/*++

Routine Description:

    Adds a field/value pair to the list if the field isn't already in the list
    or the value is empty

    The fields added through this method will be escaped

Arguments:

    pszField - Field to add
    pszValue - Value to add


Return Value:

    TRUE if successful, FALSE on error

--*/
{
    FIELD_VALUE_PAIR * pFVP;
    LIST_ENTRY *       ple;

    //
    //  Find the field
    //

    for ( ple  = _FieldListHead.Flink;
          ple != &_FieldListHead;
          ple  = ple->Flink )
    {
        pFVP = CONTAINING_RECORD( ple, FIELD_VALUE_PAIR, ListEntry );

        if ( !_stricmp( pFVP->QueryField(),
                        pszField ) )
        {
            //
            //  We found the field, replace the value if it is empty
            //

            if ( !*pFVP->QueryValue() )
            {
                return pFVP->_strValue.Copy( pszValue );
            }

            return S_OK;
        }
    }

    //
    //  The field name wasn't found, add it
    //

    return AddEntry( pszField,
                     pszValue,
                     TRUE );
}

BOOL
PARAM_LIST::RemoveEntry(
    const CHAR * pszFieldName
    )
/*++

Routine Description:

    Removes all occurrences of the specified fieldname from the list

Arguments:

    pszField - Field to remove

Return Value:

    TRUE if successful, FALSE on error

--*/
{
    FIELD_VALUE_PAIR * pFVP;
    LIST_ENTRY *       ple;
    LIST_ENTRY *       pleNext;
    BOOL               fFound = FALSE;

    //
    // Do we need to canon?
    //

    if ( !IsCanonicalized() )
    {
        CanonList( );
    }

    //
    //  Find the field
    //

    for ( ple  = _FieldListHead.Flink;
          ple != &_FieldListHead;
          ple  = pleNext )
    {
        pleNext = ple->Flink;

        pFVP = CONTAINING_RECORD( ple, FIELD_VALUE_PAIR, ListEntry );

        if ( !_stricmp( pFVP->QueryField(),
                       pszFieldName ))
        {
            //
            //  We found a matching field, remove it
            //

            RemoveEntryList( ple );

            InsertHeadList( &_FreeHead, ple );

            fFound = TRUE;
        }
    }

    return (fFound);
}


VOID *
PARAM_LIST::NextPair(
    VOID *   pCookie,
    CHAR * * ppszField,
    CHAR * * ppszValue
    )
/*++

Routine Description:

    Enumerates the field and values in this parameter list

Arguments:

    pCookie - Stores location in enumeration, set to NULL for new enumeration
    ppszField - Receives field
    ppszValue - Receives corresponding value

Return Value:

    NULL when the enumeration is complete

--*/
{
    FIELD_VALUE_PAIR * pFVP;

    //
    // Do we need to canon?
    //

    if ( !IsCanonicalized() )
    {
        CanonList( );
    }

    //
    //  pCookie points to the ListEntry in the FIELD_VALUE_PAIR class
    //
    if ( pCookie == NULL )
    {

        if ( IsListEmpty( &_FieldListHead ))
        {
            return NULL;
        }

        //
        //  Start a new enumeration
        //

        pCookie = (VOID *) _FieldListHead.Flink;
    }
    else
    {
        //
        //  Have we finished the current enumeration?
        //

        if ( pCookie == (VOID *) &_FieldListHead )
        {
            return NULL;
        }
    }

    pFVP = CONTAINING_RECORD( pCookie, FIELD_VALUE_PAIR, ListEntry );

    *ppszField = pFVP->QueryField();
    *ppszValue = pFVP->QueryValue();

    pCookie = pFVP->ListEntry.Flink;

    return pCookie;
}

VOID *
PARAM_LIST::NextPair(
    VOID *   pCookie,
    CHAR * * ppszField,
    DWORD *  pcbField,
    CHAR * * ppszValue,
    DWORD *  pcbValue
    )
/*++

Routine Description:

    Enumerates the field and values in this parameter list

Arguments:

    pCookie - Stores location in enumeration, set to NULL for new enumeration
    ppszField - Receives field
    pcbField -  Receives pointer to length of field
    ppszValue - Receives corresponding value
    pcbValue -  Receives pointer to length of value

Return Value:

    NULL when the enumeration is complete

--*/
{
    FIELD_VALUE_PAIR * pFVP;

    //
    // Do we need to canon?
    //

    if ( !IsCanonicalized() )
    {
        CanonList( );
    }

    //
    //  pCookie points to the ListEntry in the FIELD_VALUE_PAIR class
    //

    if ( pCookie == NULL )
    {

        if ( IsListEmpty( &_FieldListHead ))
        {
            return NULL;
        }

        //
        //  Start a new enumeration
        //

        pCookie = (VOID *) _FieldListHead.Flink;
    }
    else
    {
        //
        //  Have we finished the current enumeration?
        //

        if ( pCookie == (VOID *) &_FieldListHead )
        {
            return NULL;
        }
    }

    pFVP = CONTAINING_RECORD( pCookie, FIELD_VALUE_PAIR, ListEntry );

    *ppszField = pFVP->QueryField();
    *pcbField  = pFVP->_strField.QueryCB();

    *ppszValue = pFVP->QueryValue();
    *pcbValue  = pFVP->_strValue.QueryCB();

    pCookie = pFVP->ListEntry.Flink;

    return pCookie;
}

DWORD
PARAM_LIST::GetCount(
    VOID
    )
{
    LIST_ENTRY * pEntry;
    DWORD        cParams = 0;

    //
    // Do we need to canon?
    //

    if ( !IsCanonicalized() )
    {
        CanonList( );
    }

    for ( pEntry = _FieldListHead.Flink;
          pEntry != &_FieldListHead;
          pEntry = pEntry->Flink )
    {
        cParams++;
    }

    return cParams;
}


VOID
PARAM_LIST::CanonList(
    VOID
    )
{
    FIELD_VALUE_PAIR * pFVP;
    PLIST_ENTRY        listEntry;
    PLIST_ENTRY        nextEntry;
    PLIST_ENTRY        tmpEntry;

    DBG_ASSERT(!_fCanonicalized);

    //
    // Go through the list and make sure that there are no dups.
    // if there are, convert them into comma separated lists.
    //

    for ( listEntry  = _FieldListHead.Flink;
          listEntry != &_FieldListHead;
          listEntry  = nextEntry
          )
    {
        DWORD fieldLen;
        PCHAR fieldName;

        nextEntry = listEntry->Flink;

        pFVP = CONTAINING_RECORD( listEntry, FIELD_VALUE_PAIR, ListEntry );


        //
        // if field or value is empty, zap it
        //

        if ( (*pFVP->QueryField() == '\0') ||
             (*pFVP->QueryValue() == '\0') )
        {

            RemoveEntryList( listEntry );
            InsertHeadList( &_FreeHead, listEntry );
            continue;
        }

        fieldName = pFVP->QueryField();
        fieldLen = pFVP->_strField.QueryCB();

        //
        // Walk the rest of the list and look for dup fields
        //

        tmpEntry = nextEntry;

        while ( tmpEntry != &_FieldListHead )
        {

            FIELD_VALUE_PAIR * pTmpFVP;
            pTmpFVP = CONTAINING_RECORD(
                                tmpEntry,
                                FIELD_VALUE_PAIR,
                                ListEntry );

            //
            // combine the two fields
            //

            if ( (pTmpFVP->_strField.QueryCB() == fieldLen) &&
                 (_stricmp(pTmpFVP->QueryField(), fieldName) == 0) &&
                 (*pTmpFVP->QueryValue() != '\0') )
            {

                pFVP->_cValues++;

                pFVP->_strValue.Append( "," );
                pFVP->_strValue.Append( pTmpFVP->QueryValue() );

                pTmpFVP->_strField.Reset();
            }

            tmpEntry = tmpEntry->Flink;
        }
    }

    _fCanonicalized = TRUE;
    return;

} // PARAM_LIST::CanonList


