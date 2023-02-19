//////////////////////////////////////////////////////////////////////
// 
// Filename:        FileList.cpp
//
// Description:     This class builds a list of image files and 
//                  allows the user to request the next one, 
//                  previous on, first and last file in the list.
//
// Copyright (C) 2000 Microsoft Corp.
//
//////////////////////////////////////////////////////////////////////

#include "precomp.h"
#include "FileList.h"
#include "LinkList.h"
#include "CtrlPanelSvc.h"

// needed for StrStrI
#include <shlwapi.h>

// wait up to 5 seconds for the thread to terminate
#define THREAD_TERMINATE_WAIT_TIME_MS       5000

///////////////////////////////
// ImageFileExt_Type
//
typedef struct ImageFileExt_TypeTag
{
    TCHAR   *pszSupportedImage;
} ImageFileExt_Type;

///////////////////////////////
// SupportedImagesTable
//
ImageFileExt_Type SupportedImagesTable[] = 
{
    {_T("*.jpg")},
    {_T("*.bmp")},
    {_T("*.gif")}
};
#define NUM_SUPPORTED_IMAGES (sizeof(SupportedImagesTable) / sizeof(ImageFileExt_Type))

///////////////////////////////
// Constructor
//
CFileList::CFileList() :
                m_pListTail(NULL),
                m_pCurrentEntry(&m_ListHead),
                m_cNumFilesInList(0),
                m_pServiceProcessor(NULL)

{
    InitializeListHead(&m_ListHead);

    memset(m_szBaseDirectory, 0, sizeof(m_szBaseDirectory));
}

///////////////////////////////
// Destructor
//
CFileList::~CFileList()
{
    DestroyFileList();
}

///////////////////////////////
// CancelBuildFileList
//
HRESULT CFileList::CancelBuildFileList()
{
    //
    // this will trigger the RecursiveFindFiles
    // function to terminate quickly, which will
    // result in the termination of the thread.
    //
    DestroyThread(true, THREAD_TERMINATE_WAIT_TIME_MS);

    return S_OK;
}

///////////////////////////////
// Refresh
//
HRESULT CFileList::Refresh()
{
    HRESULT hr = S_OK;

    DestroyFileList();

    hr = BuildFileList(m_szBaseDirectory, NULL);

    return hr;
}

///////////////////////////////
// ClearFileList
//
HRESULT CFileList::ClearFileList()
{
    HRESULT hr = S_OK;

    DestroyFileList();

    return hr;
}

///////////////////////////////
// BuildFileList
//
// Recursively walks through
// pszImageDirectory and adds
// recognized image files to 
// its list of files.
//
HRESULT CFileList::BuildFileList(const TCHAR       *pszImageDirectory,
                                 IServiceProcessor *pServiceProcessor)
{
    ASSERT(pszImageDirectory != NULL);

    HRESULT hr = S_OK;

    if (pszImageDirectory == NULL)
    {
        hr = E_INVALIDARG;

        DBG_ERR(("CFileList::BuildFileList, received NULL param, "
                 "hr = 0x%08lx", hr));
    }

    if (SUCCEEDED(hr))
    {
        // cancel our build file list if the thread is active.
        CancelBuildFileList();

        m_pServiceProcessor = pServiceProcessor;

        _tcsncpy(m_szBaseDirectory, 
                 pszImageDirectory,
                 sizeof(m_szBaseDirectory) / sizeof(TCHAR));
    
        EnsureTrailingBackslash(m_szBaseDirectory, 
                                sizeof(m_szBaseDirectory) / sizeof(TCHAR));
    
        DBG_TRC(("CFileList::BuildFileList starting Find Files Thread"));

        //
        // create the thread that will search the specified directory and add
        // files to our list.  CreateThread is the function exposed by 
        // our base class CUtilSimpleThread.  Our thread processing function
        // overrides the base class' ThreadProc.  See function "ThreadProc" below.
        //
        hr = CreateThread();
    }

    return hr;
}

///////////////////////////////
// GetNextFile
//
HRESULT CFileList::GetNextFile(TCHAR *pszFile,
                               ULONG cchFile,
                               DWORD *pImageNumber)
{
    ASSERT(pszFile != NULL);

    HRESULT hr = S_OK;

    CUtilAutoLock Lock(&m_Lock);

    if ((m_pCurrentEntry == &m_ListHead) &&
        (m_pCurrentEntry->Flink == &m_ListHead))
    {
        // if the current entry is the list head (as it was when we first
        // initialized), and the entry that the current entry is pointing
        // to is also pointing to the head, then it means that our file
        // list is empty, so return fail.

        return E_FAIL;
    }
    else if (pszFile == NULL)
    {
        return E_INVALIDARG;
    }

    if (SUCCEEDED(hr))
    {
        FileEntry_Type *pCurrentFile = NULL;

        if (m_pCurrentEntry == &m_ListHead)
        {
            // if this is the first time we are starting, then 
            // set the current entry to be the first entry in the list
            // (which is the entry pointed to by the forward link of the
            // list head - Note, list head is NOT a valid file).

            m_pCurrentEntry = m_ListHead.Flink;
        }
        else if (m_pCurrentEntry->Flink == &m_ListHead)
        {
            // if m_pCurrentEntry is the last entry in the list, then
            // it's forward link is pointing to the list head.  Since the
            // list head is not a valid file, we skip over it and go to the
            // first entry pointed to by the forward link of the list head.

            m_pCurrentEntry = m_ListHead.Flink;
        }
        else
        {
            m_pCurrentEntry = m_pCurrentEntry->Flink;
        }

        pCurrentFile = CONTAINING_RECORD(m_pCurrentEntry, FileEntry_Type, ListEntry);

        if (pCurrentFile)
        {
            _tcsncpy(pszFile, pCurrentFile->szFileName, cchFile);
            
            if (pImageNumber)
            {
                *pImageNumber = pCurrentFile->dwImageNumber;
            }
        }
    }

    return hr;
}

///////////////////////////////
// GetPreviousFile
//
HRESULT CFileList::GetPreviousFile(TCHAR *pszFile,
                                   ULONG cchFile,
                                   DWORD *pImageNumber)
{
    ASSERT(pszFile != NULL);

    HRESULT hr = S_OK;

    CUtilAutoLock Lock(&m_Lock);

    if ((m_pCurrentEntry == &m_ListHead) &&
        (m_pCurrentEntry->Blink == &m_ListHead))
    {
        // if the current entry is the list head (as it was when we first
        // initialized), and the entry that the current entry is pointing
        // to is also pointing to the head, then it means that our file
        // list is empty, so return fail.

        return E_FAIL;
    }
    else if (pszFile == NULL)
    {
        return E_INVALIDARG;
    }

    if (SUCCEEDED(hr))
    {
        FileEntry_Type *pCurrentFile = NULL;

        if (m_pCurrentEntry == &m_ListHead)
        {
            // if this is the first time we are starting, then 
            // set the current entry to be the last entry in the list
            // (which is the entry pointed to by the backward link of the
            // list head - Note, list head is NOT a valid file).

            m_pCurrentEntry = m_ListHead.Blink;
        }
        else if (m_pCurrentEntry->Blink == &m_ListHead)
        {
            // if m_pCurrentEntry is the first entry in the list, then
            // it's backward link is pointing to the list head.  Since the
            // list head is not a valid file, we skip over it and go to the
            // first entry pointed to by the backward link of the list head.

            m_pCurrentEntry = m_ListHead.Blink;
        }
        else
        {
            m_pCurrentEntry = m_pCurrentEntry->Blink;
        }

        pCurrentFile = CONTAINING_RECORD(m_pCurrentEntry, FileEntry_Type, ListEntry);

        if (pCurrentFile)
        {
            _tcsncpy(pszFile, pCurrentFile->szFileName, cchFile);

            if (pImageNumber)
            {
                *pImageNumber = pCurrentFile->dwImageNumber;
            }
        }
    }

    return hr;
}


///////////////////////////////
// GetFirstFile
//
HRESULT CFileList::GetFirstFile(TCHAR *pszFile,
                                ULONG cchFile,
                                DWORD *pImageNumber)
{
    ASSERT(pszFile != NULL);

    HRESULT hr = S_OK;

    CUtilAutoLock Lock(&m_Lock);

    if (m_ListHead.Flink == &m_ListHead)
    {
        // this means the list is empty.

        return E_FAIL;
    }
    else if (pszFile == NULL)
    {
        return E_INVALIDARG;
    }

    if (SUCCEEDED(hr))
    {
        FileEntry_Type *pCurrentFile = NULL;

        m_pCurrentEntry = m_ListHead.Flink;

        pCurrentFile = CONTAINING_RECORD(m_pCurrentEntry, FileEntry_Type, ListEntry);

        if (pCurrentFile)
        {
            _tcsncpy(pszFile, pCurrentFile->szFileName, cchFile);

            if (pImageNumber)
            {
                *pImageNumber = pCurrentFile->dwImageNumber;
            }
        }
    }

    return hr;
}

///////////////////////////////
// GetLastFile
//
HRESULT CFileList::GetLastFile(TCHAR *pszFile,
                               ULONG cchFile,
                               DWORD *pImageNumber)
{
    ASSERT(pszFile != NULL);

    HRESULT hr = S_OK;

    CUtilAutoLock Lock(&m_Lock);

    if (m_ListHead.Blink == &m_ListHead)
    {
        // means the list is empty.
        return E_FAIL;
    }
    else if (pszFile == NULL)
    {
        return E_INVALIDARG;
    }

    if (SUCCEEDED(hr))
    {
        FileEntry_Type *pCurrentFile = NULL;

        m_pCurrentEntry = m_ListHead.Blink;

        pCurrentFile = CONTAINING_RECORD(m_pCurrentEntry, FileEntry_Type, ListEntry);

        if (pCurrentFile)
        {
            _tcsncpy(pszFile, pCurrentFile->szFileName, cchFile);

            if (pImageNumber)
            {
                *pImageNumber = pCurrentFile->dwImageNumber;
            }
        }
    }

    return hr;
}

///////////////////////////////
// GetNumFilesInList
//
DWORD CFileList::GetNumFilesInList()
{
    DWORD   dwNumFilesInList = 0;

    CUtilAutoLock Lock(&m_Lock);

    dwNumFilesInList = m_cNumFilesInList;

    return dwNumFilesInList;
}

///////////////////////////////
// AddFileToList
//
HRESULT CFileList::AddFileToList(const TCHAR    *pszDirectory,
                                 const TCHAR    *pszFileName,
                                 FileEntry_Type **ppFileEntry)
{
    HRESULT         hr          = S_OK;
    FileEntry_Type  *pFileEntry = NULL;

    ASSERT(pszFileName    != NULL);
    ASSERT(pszFileName[0] != 0);

    CUtilAutoLock Lock(&m_Lock);

    if ((pszFileName    == NULL) ||
        (pszFileName[0] == 0))
    {
        hr = E_INVALIDARG;
        DBG_ERR(("CFileList::AddFileToList received an invalid arg, "
                 "hr = 0x%08lx", hr));
    }

    if (SUCCEEDED(hr))
    {
        pFileEntry = new FileEntry_Type;

        if (pFileEntry == NULL)
        {
            hr = E_OUTOFMEMORY;
            DBG_ERR(("CFileList::AddFileToList failed to allocate memory, "
                     "hr = 0x%08lx", hr));
        }
    }

    if (SUCCEEDED(hr))
    {
        memset(pFileEntry, 0, sizeof(*pFileEntry));

        InsertTailList(&m_ListHead, &pFileEntry->ListEntry);
        m_pListTail = &pFileEntry->ListEntry;


        //
        // Copy the relative directory into the node, then
        // concat the filename.
        //
        
        // If the directory is NULL then it means that the
        // files are at the base directory, in which case we
        // only store the file name.
        //
        if ((pszDirectory != NULL) && (pszDirectory[0] != 0))
        {
            // first copy the directory into the buffer
            _tcsncpy(pFileEntry->szFileName, 
                     pszDirectory, 
                     sizeof(pFileEntry->szFileName) / sizeof(TCHAR));
    
            // make sure there is a backslash on the end of it.
            EnsureTrailingBackslash(pFileEntry->szFileName,
                                    sizeof(pFileEntry->szFileName) / sizeof(TCHAR));
        }

        // append the file name to the directory.
        _tcscat(pFileEntry->szFileName, pszFileName);

        pFileEntry->dwImageNumber = m_cNumFilesInList;

        m_cNumFilesInList++;

        if (ppFileEntry)
        {
            *ppFileEntry = pFileEntry;
        }
    }

    return hr;
}

///////////////////////////////
// DestroyFileList
//
HRESULT CFileList::DestroyFileList()
{
    HRESULT         hr          = S_OK;
    LIST_ENTRY      *pEntry     = NULL;
    FileEntry_Type  *pFileEntry = NULL;

    // cancel our build file list if the thread is active.
    CancelBuildFileList();

    CUtilAutoLock Lock(&m_Lock);

    if (IsListEmpty(&m_ListHead))
    {
        return S_OK;
    }

    while (!IsListEmpty(&m_ListHead)) 
    {
        pEntry = RemoveHeadList(&m_ListHead);

        if (pEntry) 
        {
            pFileEntry = CONTAINING_RECORD(pEntry, FileEntry_Type, ListEntry);

            if (pFileEntry) 
            {
                delete pFileEntry;
                pFileEntry = NULL;
            }
        }
    }

    InitializeListHead(&m_ListHead);

    // reset our current entry as it was probably pointing to a
    // deleted memory address.

    m_pCurrentEntry = &m_ListHead;

    m_cNumFilesInList = 0;

    return hr;
}

///////////////////////////////
// EnsureTrailingBackslash
//
BOOL CFileList::EnsureTrailingBackslash(TCHAR *pszBuffer,
                                        DWORD cchBuffer)
{
    ASSERT(pszBuffer != NULL);

    if (pszBuffer)
    {
        DWORD cLen = _tcslen(pszBuffer);

        if (pszBuffer[cLen - 1] != _T('\\'))
        {
            if (cchBuffer == cLen + 1)
            {
                // buffer is too small to append a backslash
                return FALSE;
            }
            else
            {
                _tcscat(pszBuffer, _T("\\"));
            }
        }
    }

    return TRUE;
}

///////////////////////////////
// RecursiveFindFiles
//
// Adopted from ShaunIv in
// printscan\ui\ssmypics\sscommon\findfile.h
//
BOOL CFileList::RecursiveFindFiles(const TCHAR   *pszDirectory)
{
    ASSERT(pszDirectory != NULL);

    BOOL                bFindResult             = TRUE;
    BOOL                bContinue               = TRUE;
    HANDLE              hFind                   = NULL;
    WIN32_FIND_DATA     FindData;
    TCHAR               szDir[_MAX_PATH + 1]    = {0};
    TCHAR               szNewDir[_MAX_PATH + 1] = {0};

    if (pszDirectory == NULL)
    {
        DBG_ERR(("CFileList::RecursiveFindFiles received a NULL param"));

        return FALSE;
    }
    else if (IsTerminateFlagSet())
    {
        DBG_TRC(("CFileList::RecursiveFindFiles, build file list "
                 "operation was cancelled, terminating search"));

        return S_OK;
    }

    _tcsncpy(szDir, pszDirectory, sizeof(szDir) / sizeof(TCHAR));
    
    EnsureTrailingBackslash(szDir, sizeof(szDir) / sizeof(TCHAR));

    // search for all files in the directory
    _tcscat(szDir, _T("*"));

    hFind = FindFirstFile(szDir, &FindData );

    if (hFind != INVALID_HANDLE_VALUE)
    {
        while (bFindResult && bContinue)
        {
            //
            // if the found file is a directory, then lets step into it and
            // call ourselves again on this new directory.
            // This is a depth first walk of the tree.
            //
            if ((FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && 
                _tcsicmp(FindData.cFileName, TEXT(".."))               && 
                _tcsicmp(FindData.cFileName, TEXT(".")))
            {
                // remove the "*" from the path and append the new directory.

                _tcsncpy(szNewDir, szDir, sizeof(szNewDir) / sizeof(TCHAR));
                szNewDir[_tcslen(szNewDir) - 1] = 0;
                EnsureTrailingBackslash(szNewDir, sizeof(szNewDir) / sizeof(TCHAR));
                _tcscat(szNewDir, FindData.cFileName);

                bContinue = RecursiveFindFiles(szNewDir);
            }

            bFindResult = FindNextFile(hFind, &FindData);

            //
            // if we were told to terminate our search (since we are on a 
            // different thread), then stop our search and exit the
            // function.
            //
            if (IsTerminateFlagSet())
            {
                bContinue = FALSE;
            }
        }

        FindClose(hFind);
    }

    // 
    // If nobody cancelled our search yet, then continue by
    // adding the files in the current directory into our
    // linked list of files.
    //
    if (!IsTerminateFlagSet())
    {
        // Add all the image files in this directory to our image list.
        AddImageFiles(pszDirectory);
    }

    return bContinue;
}

///////////////////////////////
// AddImageFiles
//
//
HRESULT CFileList::AddImageFiles(const TCHAR   *pszDirectory)
{
    ASSERT(pszDirectory != NULL);

    HRESULT hResult = S_OK;
    int     i       = 0;

    if (pszDirectory == NULL)
    {
        hResult = E_INVALIDARG;

        DBG_ERR(("CFileList::AddImageFiles received an invalid arg, "
                 "hr = 0x%08lx", hResult));
    }

    if (SUCCEEDED(hResult))
    {
        TCHAR *pszRelativeDir = NULL;
        TCHAR *pszSubStr      = NULL;

        // get the relative path for the directory
        pszSubStr = StrStrI(pszDirectory, m_szBaseDirectory);

        if (pszSubStr != NULL)
        {
            //
            // this advances the pointer beyond the base directory part of the
            // string so that we only have the relative directory.
            //
            pszRelativeDir = pszSubStr + _tcslen(m_szBaseDirectory);
        }

        for (i = 0; (i < NUM_SUPPORTED_IMAGES) && (!IsTerminateFlagSet()); i++)
        {
            HANDLE              hFind                   = NULL;
            ImageFileExt_Type   *pImageExtension        = NULL;
            WIN32_FIND_DATA     FindData;
            TCHAR               szDir[_MAX_PATH + 1]    = {0};
            BOOL                bFound                  = TRUE;
            
            pImageExtension = &SupportedImagesTable[i];

            _tcsncpy(szDir, pszDirectory, sizeof(szDir) / sizeof(TCHAR));
            EnsureTrailingBackslash(szDir, sizeof(szDir) / sizeof(TCHAR));

            _tcscat(szDir, pImageExtension->pszSupportedImage);

            hFind = FindFirstFile(szDir, &FindData);

            if (hFind != INVALID_HANDLE_VALUE)
            {
                while ((bFound) && (!IsTerminateFlagSet()))
                {
                    FileEntry_Type  *pFileEntry = NULL;

                    // add the image to the file list.
                    AddFileToList(pszRelativeDir, FindData.cFileName, &pFileEntry);

                    if (m_pServiceProcessor)
                    {
                        BuildAction_Type BuildAction = BuildAction_ADDED_NEW_FILE;

                        //
                        // if this is the first file we are adding to the list,
                        // rather than notifying that a new file was added,
                        // notify that the first file was added.  This helps
                        // the service notify all clients of an available 
                        // image for downloading.
                        //
                        if (GetNumFilesInList() == 1)
                        {
                            BuildAction = BuildAction_ADDED_FIRST_FILE;
                        }

                        m_pServiceProcessor->ProcessFileNotification(BuildAction, 
                                                                     pFileEntry->szFileName);
                    }
    
                    // find the next file.
                    bFound = FindNextFile(hFind, &FindData);
                }
            }
        }
    }

    return hResult;
}

///////////////////////////////
// DumpFileList
//
// Used for debug, this dumps
// all the files we have in 
// our list.
//
HRESULT CFileList::DumpFileList()
{
    HRESULT         hr          = S_OK;
    LIST_ENTRY      *pEntry     = NULL;
    FileEntry_Type  *pFileEntry = NULL;

    CUtilAutoLock Lock(&m_Lock);

    if (IsListEmpty(&m_ListHead))
    {
        DBG_TRC(("CFileList::DumpFileList, file list is empty"));

        return S_OK;
    }
    
    pEntry = m_ListHead.Flink;

    while (pEntry != &m_ListHead)
    {
        pFileEntry = CONTAINING_RECORD(pEntry, FileEntry_Type, ListEntry);

        if (pFileEntry) 
        {
            DBG_TRC(("CFileList::DumpFileList, Image #: '%lu', "
                     "FileName: '%ls'",
                    pFileEntry->dwImageNumber,
                    pFileEntry->szFileName));
        }

        pEntry = pEntry->Flink;
    }

    return hr;
}

///////////////////////////////////////////////
// ThreadProc
//
DWORD CFileList::ThreadProc(void *pArg)
{
    HRESULT bSuccess = FALSE;

    // this thread procedure simply calls the Recursive
    // File Building function (RecursiveFindFiles)
    // which goes off and searchs the specified directory
    // for images, and adds them to our linked list of
    // image file names.
    
    // The RecursiveFindFiles function continuously checks
    // the IsTerminateFlagSet() fn to determine if 
    // it should terminate its search (in case we are 
    // shutting down, user specified a new directory,
    // etc.

    if (m_pServiceProcessor)
    {
        m_pServiceProcessor->ProcessFileNotification(BuildAction_STARTING_BUILD, NULL);
    }

    bSuccess = RecursiveFindFiles(m_szBaseDirectory);

    if (m_pServiceProcessor)
    {
        m_pServiceProcessor->ProcessFileNotification(BuildAction_ENDED_BUILD, NULL);
    }

    DumpFileList();

    DBG_TRC(("CFileList::ThreadProc, Find Files thread is "
             "finished, bSuccess = %lu, thread id = %lu (0x%lx)",
            bSuccess,
            GetThreadID(),
            GetThreadID()));

    return 0;
}

