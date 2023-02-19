///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Microsoft WMI OLE DB Client test app
// (C) Copyright 1999 Microsoft Corporation. All Rights Reserved.
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#define INITGUID
#define DBINITCONSTANTS
#include "wmiclnt.h"
#include <headers.h>

IMalloc* g_pIMalloc = NULL;
FILE*    g_fpLogFile = NULL;    // our log file
WCHAR *  g_pwcsTable = NULL;
WCHAR *  g_pwcsNamespace = NULL;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AllocateAndConvertAnsiToUnicode(char * pstr, WCHAR *& pszW)
{
    pszW = NULL;

    int nSize = strlen(pstr);
    if (nSize != 0 ){

        // Determine number of wide characters to be allocated for the
        // Unicode string.
        nSize++;
		try{
			pszW = new WCHAR[nSize * 2];
			if (NULL != pszW){
            // Covert to Unicode.
				MultiByteToWideChar(CP_ACP, 0, pstr, nSize,pszW,nSize);
	        }	
		}
		catch(...){
			throw;
		}
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL GetDataFromCommandLine(int argc,int & i, WCHAR *& p, char * argv[])
{
    i++;
    if( i > argc ){
        return FALSE;
    }

    AllocateAndConvertAnsiToUnicode(argv[i],p);


    return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL ParseCommandLine(int argc, char * argv[])
{
    
    BOOL	fFlag = FALSE;

	if ( argc > 1 ){
        g_pwcsTable = NULL;
        g_pwcsNamespace = NULL;

        //======================================================================
        //  Now, go thru and get the ones we support
        //======================================================================
        for( int i = 0; i < argc; i++){

            //==================================================================
            //  NAMESPACE
            //==================================================================
            if(_stricmp(argv[i],"-Namespace")== 0){
                GetDataFromCommandLine(argc,i,g_pwcsNamespace,argv);
            }
            //==================================================================
            //  TABLE
            //==================================================================
            if(_stricmp(argv[i],"-Table")== 0){
                GetDataFromCommandLine(argc,i,g_pwcsTable,argv);
            }
        }
        if( g_pwcsTable && g_pwcsNamespace ) {
            fFlag = TRUE;
        }
    }

    if( !fFlag ){
		printf( "-Namespace name\n" );
		printf( "-Table tablename\n" );
	} 


    return fFlag;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void InitializeLogFile()
{
	time_t	ttime;
 
  	g_fpLogFile = fopen( "wmiclnt.out", "at");
    if (!g_fpLogFile){
    	DumpErrorMsg( "Warning: cannot open log file wmiclnt.out\n" );
    }
  
  	time(&ttime);
    
    DumpStatusMsg( "\n-------------------------\n\n");
    DumpStatusMsg( "running wmiclnt.exe\n%s\n\n", ctime(&ttime) );
}




//**********************************************************************
//  
// GetWMIOLEDBDataSource
// 
// Purpose:
//
//     Calls OLE to find and load the WMIOLEDB data provider. 
//     Returns an IDBInitialize interface pointer on WMIOLEDB's 
//     Data Source object.
//
// Parameters:
// 	
//   IDBInitialize** ppIDBInitialize_out  - out pointer through which to return
// 									        IDBInitialize pointer on data 
//								            provider's Data Source object 	
// 
// Return Value:
// 
// 	S_OK		- Success
//  E_*	    	- Failure
//     
//
// Comments:      
// 
//     The call to CoCreateInstance is hard-coded with WMIOLEDB's CLSID.
//     The pointer returned through ppIDBInitialize_out has been AddRef'ed,
//     it must be Release'd later by the caller.
//  
//**********************************************************************

HRESULT GetWMIOLEDBDataSource(IDBInitialize**	ppIDBInitialize_out	)
{
	IDBInitialize*	pIDBInit = NULL;
	IDBProperties*	pIDBProperties = NULL;
	DBPROPSET		dbPropSet[1];
	DBPROP			dbProp[1];

	HRESULT	hr;


	DumpStatusMsg( "Connecting to the WMIOLEDB data provider...\n" );

	assert(ppIDBInitialize_out != NULL);

	VariantInit(&(dbProp[0].vValue));

    //===========================================================================
	// Create an instance of the WMIOLEDB  data provider
    //===========================================================================
	hr = CoCreateInstance( CLSID_WMIOLEDB, NULL, CLSCTX_INPROC_SERVER, 	IID_IDBInitialize, (void **)&pIDBInit ); 
	if (FAILED(hr)){
        DUMP_ERROR_LINENUMBER();
        DumpErrorHResult( hr, "CoCreateInstance" );
        goto error;
    }

    //===========================================================================
	// Initialize this provider with the namespace
    //===========================================================================
	dbPropSet[0].rgProperties		= &dbProp[0];
	dbPropSet[0].cProperties		= 1;
	dbPropSet[0].guidPropertySet	= DBPROPSET_DBINIT;
	
	dbProp[0].dwPropertyID			= DBPROP_INIT_DATASOURCE;
	dbProp[0].dwOptions				= DBPROPOPTIONS_REQUIRED;
	dbProp[0].colid					= DB_NULLID;
	V_VT(&(dbProp[0].vValue))		= VT_BSTR;
	V_BSTR(&(dbProp[0].vValue))		= SysAllocString( g_pwcsNamespace );
    if ( NULL == V_BSTR(&(dbProp[0].vValue)) ){
        DUMP_ERROR_LINENUMBER();
        DumpErrorMsg( "SysAllocString failed\n" );
        goto error;
    }

    hr = pIDBInit->QueryInterface( IID_IDBProperties, (void**)&pIDBProperties);
    if (FAILED(hr)){
        DUMP_ERROR_LINENUMBER();
        DumpErrorHResult( hr, "IDBInitialize::QI for IDBProperties");
        goto error;
        }

	hr = pIDBProperties->SetProperties( 1, &dbPropSet[0]);
	if (FAILED(hr)){
        DUMP_ERROR_LINENUMBER();
        DumpErrorHResult( hr, "IDBProperties::SetProperties" );
        goto error;
    }
		
	hr = pIDBInit->Initialize();
	if (FAILED(hr)){
        DUMP_ERROR_LINENUMBER();
        DumpErrorHResult( hr, "IDBInitialize::Initialize" );
        goto error;
    }

	*ppIDBInitialize_out = pIDBInit;

	hr = ResultFromScode( S_OK );

error:    
    VariantClear( &(dbProp[0].vValue) );

	if( pIDBProperties )
		pIDBProperties->Release();

	if( FAILED(hr) )
	{
        if (pIDBInit){
    		pIDBInit->Release();
        }
		*ppIDBInitialize_out = NULL;
	}

	return hr;    
}




//  **********************************************************************
//
// GetDBSessionFromDataSource
//
// Purpose:
//      Calls the provider's Data Source object to get an IOpenRowset interface
//      pointer on a DBSession object.  
//      
// Parameters:
//      pIDBInitialize      - pointer to Data Source object
//      ppIOpenRowset_out   - out pointer through which to return 
//                            IOpenRowset pointer on DBSession object
//
// Return Value: 
//
// 	S_OK		- Success
//  E_*	    	- Failure
//
//
// Comments:
//
//      The interface pointer returned through ppIOpenRowset_out has been 
//      AddRef'ed, the caller must Release it later.
//
//**********************************************************************

HRESULT GetDBSessionFromDataSource
    (
    IDBInitialize*      pIDBInitialize,      // [in]
    IOpenRowset**       ppIOpenRowset_out    // [out]
    )
{
    IDBCreateSession*   pIDBCreateSession;
    IOpenRowset*        pIOpenRowset;
    HRESULT             hr;


    DumpStatusMsg( "Getting a DBSession object from the data source object...\n" );
    
    assert(pIDBInitialize != NULL);
    assert(ppIOpenRowset_out  != NULL );

    hr = pIDBInitialize->QueryInterface( IID_IDBCreateSession, (void**)&pIDBCreateSession);
    if (FAILED(hr))
    	{
        DUMP_ERROR_LINENUMBER();
        DumpErrorHResult( hr, "IDBInitialize::QI for IDBCreateSession");
        goto error;
        }
        
    hr = pIDBCreateSession->CreateSession( NULL, IID_IOpenRowset, (IUnknown**)&pIOpenRowset );    
    if (FAILED(hr))
    	{
        DUMP_ERROR_LINENUMBER();
        DumpErrorHResult( hr, "IDBCreateSession::CreateSession");
        goto error;
        }
     pIDBCreateSession->Release();
     pIDBCreateSession = NULL;
     
     // all went well
     *ppIOpenRowset_out = pIOpenRowset;
     return ResultFromScode( S_OK );
     
error:
    if (pIDBCreateSession)
        pIDBCreateSession->Release();
        
    *ppIOpenRowset_out = NULL;    
    return ResultFromScode( hr );
}



//**********************************************************************
//
// GetRowsetFromDBSession
//
// Purpose:
//      Calls the provider's DBSession object to get an IRowset interface
//      pointer on a Rowset object.  
//      
// Parameters:
//      pIOpenRowset        - interface pointer on DBSession object
//      pwszTableName       - name of "table" (in this case text file)
//      ppIRowset_out       - out pointer through which to return 
//                            IRowset pointer on Rowset object
//
// Return Value: 
//
// 	S_OK		- Success
//  E_*	    	- Failure
//
// Comments:
//
//      The interface pointer returned through ppIRowset_out has been 
//      AddRef'ed, the caller must Release it later.
//
///**********************************************************************

HRESULT GetRowsetFromDBSession
    (
    IOpenRowset*   pIOpenRowset,    // [in]
    LPWSTR         pwszTableName,   // [in] 
    IRowset**      ppIRowset_out    // [out]
    )
{
    DBID            dbcolid;
    IRowset*        pIRowset = NULL;
    HRESULT         hr;

    DumpStatusMsg( "Getting a rowset object from the DBSession object...\n" );
    
    assert(pIOpenRowset != NULL);
    assert(ppIRowset_out  != NULL );
    
    // tell the provider which table to open
    dbcolid.eKind           = DBKIND_NAME;
    dbcolid.uName.pwszName  = pwszTableName;
    
    hr = pIOpenRowset->OpenRowset
                        (
                        NULL,                   // pUnkOuter - we are not aggregating
                        &dbcolid,               // pTableID -  the table we want
						NULL,					// pIndexID - the index we want
                        IID_IRowset,            // riid - interface we want on the rowset object
                        0,                      // cProperties - we are niave about props for now
                        NULL,                   // prgProperties[]
                        (IUnknown**)&pIRowset   // ppRowset
                        );
    if (FAILED(hr))
    	{
        DUMP_ERROR_LINENUMBER();
        DumpErrorHResult( hr, "IOpenRowset::OpenRowset" );
        goto error;
        }
    
    // all went well
    *ppIRowset_out = pIRowset;
    return ResultFromScode( S_OK );

error:
    if (pIRowset)
        pIRowset->Release();
    *ppIRowset_out = NULL;            
    
    return ResultFromScode( hr );
}    





//**********************************************************************
// 
// GetColumnsInfo
//
// Purpose:
// 
//     Obtains information (metadata) about the columns in the rowset - the types
//     of the data and so on.
//
// 
// Parameters:
// 	IRowset*		pIRowset            - interface pointer on data provider's
//                                        Rowset object  
// 	ULONG*			pcCol_out           - out pointer through which to return
//                                        number of columns in the rowset
// 	DBCOLUMNINFO**	ppColumnInfo_out    - out pointer through which to return
//                                        pointer to structure containing
//                                        metadata for the columns in the rowset
// 	WCHAR**			ppStringsBuffer_out - out pointer through which to return
//                                        pointer to table of strings. see comments. 
// 
// Return Value:
//     S_OK        - Success
//     E_*         - Failure
//     
//     
// Comments:      
// 
//     ppColumnInfo_out and ppStringsBuffer_out are used to return pointers
//     to two buffers. These buffers are allocated by the data provider
//     (when GetColumnsInfo calls IColumnsInfo::GetColumnInfo). The data
//     provider uses IMalloc to allocate the buffers; therefore, the caller
//     of this routine must at a later point use IMalloc::Free to free
//     both of these buffers. The StringsBuffer contains strings pointed
//     to by pointers in the ColumnInfo buffer, therefore the StringsBuffer
//     should be freed *after* the ColumnInfo pointer.    
//
//     GetColumnsInfo calls DumpColumnsInfo to dump the column metadata to
//     the log file.
// 
//**********************************************************************

HRESULT GetColumnsInfo
	(
	IRowset*		pIRowset,
	ULONG*			pcCol_out,
	DBCOLUMNINFO**	ppColumnInfo_out,
	WCHAR**			ppStringsBuffer_out
	)
{
	IColumnsInfo* 	pIColumnsInfo = NULL;
    ULONG			cCol;
    DBCOLUMNINFO*	pColumnInfo;
    WCHAR*			pStringsBuffer;
	HRESULT 		hr;
    

	assert(pIRowset != NULL);
    assert(pcCol_out != NULL);
    assert(ppColumnInfo_out != NULL);
    assert(ppStringsBuffer_out != NULL);

	// get column information from the command object via IColumnsInfo 
	hr = pIRowset->QueryInterface( IID_IColumnsInfo, (void **) &pIColumnsInfo );
    if (FAILED(hr))
    	{
        DUMP_ERROR_LINENUMBER();
        DumpErrorHResult( hr, "IRowset::QI for IID_IColumnsInfo" );
        goto error;
        }
	hr = pIColumnsInfo->GetColumnInfo( 
			&cCol, 
			&pColumnInfo,
			&pStringsBuffer );
    if (FAILED(hr))
    	{
        DUMP_ERROR_LINENUMBER();
        DumpErrorHResult( hr, "pIColumnsInfo->GetColumnInfo" );
        goto error;
        }
	pIColumnsInfo->Release();
	pIColumnsInfo = NULL;

	DumpColumnsInfo( pColumnInfo, cCol );
    
    // fill out-params
    *pcCol_out = cCol;
    *ppColumnInfo_out = pColumnInfo;
    *ppStringsBuffer_out = pStringsBuffer;
    
	return ResultFromScode( S_OK );

error:
	if (pIColumnsInfo)
    	pIColumnsInfo->Release();
        
	*pcCol_out = 0;
    *ppColumnInfo_out = NULL;
    *ppStringsBuffer_out = NULL;
	
	return ResultFromScode( hr );
}


//**********************************************************************
// 
// SetupBindings
// 
// Purpose:
// 
//     Creates bindings that map the data in the rowset's columns to 
//     slots in the consumer's data buffer.
//     
// Parameters:
//
// 	ULONG 			cCol                - number of columns in rowset to bind
// 	DBCOLUMNINFO*	pColumnInfo         - pointer to column metadata
// 	DBBINDING*		rgBind_out          - out pointer through which to return
//                                        an array of binding structures, one
//                                        structure per column bound
// 	ULONG*			pcBind_out          - out pointer through which to return   
//                                        the number of columns bound (number
//                                        of valid elements in rgBind_out)              
// 	ULONG*			pcMaxRowSize_out    - out pointer through which to return
//                                        the buffer size necessary to hold
//                                        the largest row data
// 
// Return Value:
//     S_OK         - Success
//     E_*          - Failure
// 
//     
// Comments:      
// 
// 
//**********************************************************************

HRESULT SetupBindings
	(
	ULONG 			cCol,
	DBCOLUMNINFO*	pColumnInfo,
	DBBINDING*		rgBind_out,
	ULONG*			pcBind_out,
	ULONG*			pcMaxRowSize_out
	)
{
	ULONG dwOffset;
	ULONG iCol;
	ULONG iBind;
	
    
    assert(pColumnInfo != NULL);
    assert(rgBind_out != NULL);
    assert(pcBind_out != NULL);
    assert(pcMaxRowSize_out != NULL);
    
	// Create bindings.
	// Bind everything as a string just to keep things simple.
	dwOffset = 0;
	iBind=0;
	for (iCol=0; iCol < cCol; iCol++)
	{
		// Skip columns of type _VECTOR. Probably binary data.
		if (pColumnInfo[iCol].wType & DBTYPE_VECTOR)
			continue;

		rgBind_out[iBind].dwPart	= DBPART_VALUE | DBPART_LENGTH |
									  DBPART_STATUS;
        rgBind_out[iBind].eParamIO  = DBPARAMIO_NOTPARAM;                              
		rgBind_out[iBind].iOrdinal  = pColumnInfo[iCol].iOrdinal;
		rgBind_out[iBind].wType		= DBTYPE_STR;
		rgBind_out[iBind].pTypeInfo = NULL;
		rgBind_out[iBind].obValue   = dwOffset + offsetof(TMPCOLUMNDATA,bData);
 		rgBind_out[iBind].obLength  = dwOffset + offsetof(TMPCOLUMNDATA,dwLength);
        rgBind_out[iBind].obStatus  = dwOffset + offsetof(TMPCOLUMNDATA,dwStatus);
		rgBind_out[iBind].cbMaxLen  = pColumnInfo[iCol].wType == DBTYPE_STR ? 
										pColumnInfo[iCol].ulColumnSize + sizeof(char) : DEFAULT_CBMAXLENGTH;
		rgBind_out[iBind].pObject	= NULL;
		dwOffset += rgBind_out[iBind].cbMaxLen + offsetof( TMPCOLUMNDATA, bData );
		dwOffset = ROUND_UP( dwOffset, COLUMN_ALIGNVAL );
		iBind++;
	}  
    
	*pcBind_out 	  = iBind;
	*pcMaxRowSize_out = dwOffset;

	return ResultFromScode( S_OK );
}



//**********************************************************************
// 
// CreateAccessor
// 
// Purpose:
//
//     Passes a set of bindings to the data provider and recieves in return
//     an accessor handle that represents those bindings. 
//      
// Parameters:
// 	IRowset*	pIRowset        - interface pointer on data provider's Rowset
//                                object
// 	DBBINDING*	rgBind          - array of binding structures
// 	ULONG		cBind           - number of binding structures in rgBind
// 	HACCESSOR*	phAccessor_out  - out pointer through which to return an 
//                                accessor handle that represents all the bindings
//                                in rgBind
// 
// Return Value:
//     S_OK        - Success
//     E_*         - Failure
// 
//     
// Comments:      
// 
// 
//**********************************************************************

HRESULT CreateAccessor
	(
	IRowset*	pIRowset,
	DBBINDING*	rgBind,
	ULONG		cBind,
	HACCESSOR*	phAccessor_out 
	)
{
	IAccessor*	pIAccessor = NULL;
	HACCESSOR   hAccessor;
	HRESULT 	hr;


	assert(pIRowset != NULL);
    assert(rgBind != NULL);
    assert(phAccessor_out != NULL);

  	// Get an accessor for our bindings from the rowset, via IAccessor 
	hr = pIRowset->QueryInterface( IID_IAccessor, (void**)&pIAccessor );
	if (FAILED(hr))   	
    	{
        DUMP_ERROR_LINENUMBER();
        DumpErrorHResult( hr, "pIRowset->QI for IID_IAccessor" );	
        goto error;
        }
	hr = pIAccessor->CreateAccessor( DBACCESSOR_ROWDATA, cBind, rgBind, 0, 
	                                 &hAccessor, NULL );
	if (FAILED(hr))   	
    	{
        DUMP_ERROR_LINENUMBER();
        DumpErrorHResult( hr, "pIAccessor->CreateAccessor" );	
        goto error;
        }
	pIAccessor->Release();
	pIAccessor = NULL;

	*phAccessor_out = hAccessor;

	return ResultFromScode( S_OK );	

error:
	if (pIAccessor)
    	pIAccessor->Release();
	*phAccessor_out = NULL;
	
	return ResultFromScode( hr );
}


//**********************************************************************
// 
// GetData
// 
// Purpose:
// 
//     Reads the data from a rowset.
// 
// Parameters:
//
// 	   IRowset*	 pIRowset           - interface pointer on data provider's
//                                    Rowset object
// 	   ULONG   	 cMaxRowSize        - size of buffer needed to hold the data
//                                    for the largest row
//     HACCESSOR hAccessor          - accessor handle representing the set
//                                    of desired bindings
//     DBBINDING*		rgBind	  	- needed only for pretty printing	
//     ULONG			cBind	  	- for pretty printing  
//     DBCOLUMNINFO*	pColumnInfo - for pretty printing  
//     ULONG			cCol		- for pretty printing  
// 
// 
// Return Value:
//     S_OK        - Success
//     E_*         - Failure
// 
// 
//     
// Comments:      
//
//     GetData reads all the rows in the rowset, sequentially.
//     
//     GetData calls CalcPrettyPrintMaxColWidth, DumpColumnHeadings, and
//     DumpRow to dump the row data to a log file.
//  
// 
//**********************************************************************

HRESULT GetData( IRowset*	pIRowset,ULONG   	cMaxRowSize,   HACCESSOR   hAccessor,   DBBINDING*		rgBind,	  	  
                 ULONG			cBind,	  	    DBCOLUMNINFO*	pColumnInfo,    ULONG			cCol		  
	)
{
	ULONG 	cRowsObtained;
	ULONG	iRow;
	BYTE*	pRowData = NULL;
	HROW 	rghRows[NUMROWS_CHUNK];
	HROW*	pRows = &rghRows[0];
	ULONG	cMaxColWidth;	  	// needed for pretty printing
	HRESULT hr;
    

	assert(pIRowset != NULL);
    assert(rgBind != NULL);
    assert(pColumnInfo != NULL);

	// create a buffer for row data, big enough to hold the biggest row
	pRowData = (BYTE *) malloc( cMaxRowSize );
	if (!pRowData)
		{
        DUMP_ERROR_LINENUMBER();
        DumpErrorMsg("GetData: malloc failed\n");
        goto error;
		}	

	// pretty print
    cMaxColWidth = CalcPrettyPrintMaxColWidth( rgBind, cBind );
    
    // pretty print
	DumpColumnHeadings( rgBind, cBind, pColumnInfo, cCol, cMaxColWidth );
		
    // process all the rows, NUMROWS_CHUNK rows at a time
	while (1)
		{
		hr = pIRowset->GetNextRows(
			NULL,						// hChapter
			0,							// cRowsToSkip
			NUMROWS_CHUNK,				// cRowsDesired
			&cRowsObtained,             // pcRowsObtained
			&pRows );					// filled in w/ row handles
		if (FAILED(hr))
			{
            DUMP_ERROR_LINENUMBER();
            DumpErrorHResult( hr, "pIRowset->GetNextRows" );
            goto error;
			}		

		if ( cRowsObtained == 0 )			// all done, no more rows left to get
			break;

		// loop over rows obtained, getting data for each
		for ( iRow=0; iRow < cRowsObtained; iRow++ )
			{
			hr = pIRowset->GetData(
				rghRows[iRow],
				hAccessor,
				pRowData );
             if (FAILED(hr))
             	{
                DUMP_ERROR_LINENUMBER();
                DumpErrorHResult( hr, "pIRowset->GetData" );
                goto error;
             	}   
			
			// pretty print
            DumpRow( rgBind, cBind, cMaxColWidth, pRowData );
			}
		// release row handles
		hr = pIRowset->ReleaseRows( cRowsObtained, rghRows, NULL, NULL, NULL );
        if (FAILED(hr))
             	{
                DUMP_ERROR_LINENUMBER();
                DumpErrorHResult( hr, "pIRowset->ReleaseRows" );
                goto error;
             	} 
		}	// end while

	// free row data buffer
	free( pRowData );
	return ResultFromScode( S_OK );
    
error:
	if (pRowData)
		free( pRowData );
		    
    return ResultFromScode( hr );
}



//**********************************************************************
// 
// CleanupRowset
// 
// Purpose:
//
//     Allows the rowset to perform any necessary cleanup.
//  
// Parameters:
//
// 	IRowset*	pIRowset    - interface pointer on data provider's Rowset
//                            object
// 	HACCESSOR 	hAccessor   - accessor handle to release
// 
// Return Value:
//
//     S_OK        - Success
//     E_*         - Failure
//     
//     
// Comments:      
//
//     In this , the only cleanup that the rowset needs to do is
//     release the accessor handle. 
// 
//**********************************************************************

HRESULT CleanupRowset
	(
	IRowset*	pIRowset,
	HACCESSOR 	hAccessor
	)
{
	IAccessor*	pIAccessor = NULL;
	HRESULT		hr;
    
    assert(pIRowset != NULL);
    
	// tell the rowset object it can release the accessor, via IAccessor
	hr = pIRowset->QueryInterface( IID_IAccessor, (void**)&pIAccessor );
	if (FAILED(hr))
		{
        DUMP_ERROR_LINENUMBER();
		DumpErrorHResult( hr, "pIRowset->QI for IID_IAccessor" );
		goto error;
		} 
	hr = pIAccessor->ReleaseAccessor( hAccessor, NULL );
	if (FAILED(hr))
		{
        DUMP_ERROR_LINENUMBER();
		DumpErrorHResult( hr, "pIAccessor->ReleaseAccessor" );
		goto error;
		}    
	pIAccessor->Release();
	pIAccessor = NULL;

	return ResultFromScode( S_OK );
    
error:    
	if (pIAccessor)
    	pIAccessor->Release();

    return ResultFromScode( hr );    
}


//**********************************************************************
// 
// GetDataFromRowset
// 
// Purpose:
// 
//     Pulls the data from a Rowset object.
//     
// Parameters:
// 
// 	IRowset*	pIRowset    -  interface pointer on data provider's
//                             Rowset object
// 
// Return Value:
// 
//     S_OK        - Success
//     E_*         - Failure
// 
// Comments:      
// 
//     At a high level, a consumer pulls the data from a Rowset object by:
//     
//     1. getting metadata for the Rowset's columns
//     2. using that metadata, along with the consumer's own knowledge of
//        how it wants to recieve the data, to create bindings. Bindings
//        represent how the actual data in the Rowset's columns is
//        actually transferred to the consumer's buffer.
//     3. pass the bindings to the Rowset, and get in return an accessor
//        handle that represents that particulr set of bindings   
//     4. get the actual data
//     5. clean up the rowset (at a minumum, release the accessor) 
//     
//     GetDataFromRowset performs these steps by calling GetColumnsInfo,
//     SetupBindings, CreateAccessor, GetData, and CleanupRowset
//       
//**********************************************************************

HRESULT GetDataFromRowset
	(
	IRowset*	pIRowset
	)
{
	ULONG 			cCol;
	ULONG 			cbMaxRowSize;		// buffer size for 1 row's data
	ULONG 			cBind;
	DBBINDING		rgBind[MAX_BINDINGS];
	HACCESSOR		hAccessor		= NULL;
	DBCOLUMNINFO*	pColumnInfo 	= NULL;
	WCHAR*			pStringsBuffer  = NULL;
	HRESULT 		hr;
    


	DumpStatusMsg( "Reading all the data in the rowset...\n" );

	assert(pIRowset != NULL);
	assert(g_pIMalloc != NULL);
	
	hr = GetColumnsInfo( pIRowset, &cCol, &pColumnInfo, &pStringsBuffer );
	if (FAILED(hr))
    	{
        DUMP_ERROR_LINENUMBER();
        DumpErrorHResult( hr,  "GetColumnsInfo");
        goto error;
        }
	
	hr = SetupBindings( cCol, pColumnInfo, rgBind, &cBind, &cbMaxRowSize );
	if (FAILED(hr))
    	{
        DUMP_ERROR_LINENUMBER();
        DumpErrorHResult( hr,  "SetupBindings");
        goto error;
        }
	
	hr = CreateAccessor( pIRowset, rgBind, cBind, &hAccessor );
	if (FAILED(hr))
    	{
        DUMP_ERROR_LINENUMBER();
        DumpErrorHResult( hr,  "CreateAccessor" );
        goto error;
        }

	hr = GetData( pIRowset, cbMaxRowSize, hAccessor, rgBind, cBind, pColumnInfo, cCol );
	if (FAILED(hr))
    	{
        DUMP_ERROR_LINENUMBER();
        DumpErrorHResult( hr,  "GetData" );
        goto error;
        }

	g_pIMalloc->Free( pColumnInfo );
    pColumnInfo = NULL;
	g_pIMalloc->Free( pStringsBuffer );
    pStringsBuffer = NULL;
	
	hr = CleanupRowset( pIRowset, hAccessor );
	if (FAILED(hr))
    	{
        DUMP_ERROR_LINENUMBER();
        DumpErrorHResult( hr,  "CleanupRowset" );
        goto error;
        }
	
	return ResultFromScode( S_OK );		
    
error:    
	if (pColumnInfo)
		g_pIMalloc->Free( pColumnInfo );
	if (pStringsBuffer)
		g_pIMalloc->Free( pStringsBuffer );	    	
	    
    return ResultFromScode( hr );    
}





//**********************************************************************
//  
//  DoTests
//  
//  Purpose:
//
//     Hooks up to the  WMIOLEDB provider application,

//  Parameters:
//  
//  	none
//      
//  Return Value:
//  
//  	S_OK		- Success
//      E_*			- Failure
//  	
//      
//  Comments:      
//  
//     At a high level, an OLE DB data consumer obtains data by 
//     
//     1. Getting hooked up to a data provider's Data Source object,
//        and initializing that object
//     2. Getting a DBSession object from the Data Source object
//     3. Getting the data from the Rowset object.
//     
//     DoTests follows these steps by making calls to GetWMIOLEDBDataSource,
//     GetDBSessionDataSource, and GetDataFromRowset    
//  
//**********************************************************************


HRESULT DoTests
	(
	)
{
	IDBInitialize*	    pIDBInitialize 	= NULL;
    IOpenRowset*        pIOpenRowset    = NULL;
    IRowset*		    pIRowset		= NULL;
	HRESULT			    hr;


	hr = GetWMIOLEDBDataSource( &pIDBInitialize );
	if (FAILED(hr))
    	{
        DUMP_ERROR_LINENUMBER();
        DumpErrorHResult( hr, "GetWMIOLEDBDataSource" );
        goto error;
        }

    hr = GetDBSessionFromDataSource( pIDBInitialize, &pIOpenRowset );
    if (FAILED(hr))
        {
        DUMP_ERROR_LINENUMBER();
        DumpErrorHResult( hr, "GetDBSessionFromDataSource" );
        goto error;
        }

    pIDBInitialize->Release();
    pIDBInitialize = NULL;    

    hr = GetRowsetFromDBSession( pIOpenRowset, g_pwcsTable, &pIRowset );
    if (FAILED(hr))
        {
        DUMP_ERROR_LINENUMBER();
        DumpErrorHResult( hr, "GetRowsetFromDBCreateSession" );
        goto error;
        }
        
    pIOpenRowset->Release();
    pIOpenRowset = NULL;    

	hr = GetDataFromRowset( pIRowset );
	if (FAILED(hr))
    	{
        DUMP_ERROR_LINENUMBER();
        DumpErrorHResult( hr, "GetDataFromRowset" );
        goto error;
        }
    pIRowset->Release(); 
    pIRowset = NULL;
	CoFreeUnusedLibraries();

	DumpStatusMsg( "\nDone! ");
	printf("\n\nFor more data from this run, see the log file wmiclnt.out\n" );	
	return ResultFromScode( S_OK );
    
error:    
	if (pIRowset) 
		pIRowset->Release();
    if (pIOpenRowset)
        pIOpenRowset->Release();    
    if (pIDBInitialize)
    	pIDBInitialize->Release();	    
	
	return ResultFromScode( hr );
}						    















///////////////////////////////////////////////////////////////////////////////////////////////////////////
//extern "C" int wmain( int argc, WCHAR * argv[] )
int main( int argc, char * argv[] )
{
	DWORD   dwVersion;
	HRESULT hr;
    BOOL 	fOleInitialized = FALSE;
	char	ch;

    //=====================================================================================
    //  Parse the command line
    //=====================================================================================
    if( !ParseCommandLine(argc,argv) ){
        return 1;
    }

    InitializeLogFile();

	dwVersion = OleBuildVersion();
    if (HIWORD(dwVersion) != rmm){
		DumpErrorMsg( "Error: OLE version mismatch. Build version %ld, current version %ld\n",
			rmm, HIWORD(dwVersion) );
        goto error;    
	}

	hr = OleInitialize( NULL );
	if (FAILED(hr)){
        DUMP_ERROR_LINENUMBER();
        DumpErrorMsg("Error: OleInitialize failed\n");
        goto error;
	}
	fOleInitialized = TRUE;
     
    hr = CoGetMalloc( MEMCTX_TASK, &g_pIMalloc );
 	if (FAILED(hr))	{
        DUMP_ERROR_LINENUMBER();
        DumpErrorMsg("Error: CoGetMalloc failed\n");
        goto error;
    }
   
    hr = DoTests();
	if (FAILED(hr)){
        DUMP_ERROR_LINENUMBER();
        DumpErrorHResult( hr, "DoTests");
        goto error;
    }
    
    g_pIMalloc->Release();
	OleUninitialize();

	if (g_fpLogFile)
    	fclose(g_fpLogFile);

	/* Pause before we quit, in case user ran from an icon, so they can see our messages. */
	printf("\n\n>>> Output has gone into 'wmiclnt.out'.");
	printf("\n>>> You may wish to use a wide-column editor to view this file.\n\n");
	printf("<press any key to continue>");
	ch = _getch();
	return -1;
    
error:
    if (g_pIMalloc)
    	g_pIMalloc->Release();    
	if (fOleInitialized)
    	OleUninitialize();
	if (g_fpLogFile)
    	fclose(g_fpLogFile);
	
	/* Pause before we quit, in case user ran from an icon, so they can see our messages. */
	printf("\n\n>>> Output has gone into 'wmiclnt.out'.");
	printf("\n>>> You may wish to use a wide-column editor to view this file.\n\n");
	printf("<press any key to continue>");
	ch = _getch();

	return 2;    
}    
