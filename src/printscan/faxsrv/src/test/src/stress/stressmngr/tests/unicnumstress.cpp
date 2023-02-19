#pragma warning(disable :4786)

// standard
#include <iostream>
#include <string>
#include <vector>

// utilities
#include <testruntimeerr.h>
#include <params.h>
#include <DirectoryUtilities.h>
#include <log\log.h>

// project specific
#include "..\CTestManager.h"
#include "..\PrepareRandomJob.cpp"

using namespace std;

// Definitions
//
#define MIN_SLEEP_BETWEEN_SEND (3*30)
#define MAX_SLEEP_BETWEEN_SEND (5*30)

DWORD main(int argc,char** argv)
{
	CInput input(GetCommandLine());
	tstring tstrServerName;
	DWORD dwTestDuration = 0;
	BOOL fGenericFail = FALSE;
	std::vector<tstring> FaxFileVector;
	std::vector<tstring> CoverPageVector;
	std::vector<tstring> RecipientNumbersList;

	::lgInitializeLogger();
	::lgBeginSuite(TEXT("Stress"));
	::lgBeginCase(0, TEXT("0"));
	
	// We want exception to be printed before the very long destructor process of this object. 
	CTestManager* pTest = NULL;	

	try
	{
		if(input.IsExists(TEXT("d")))
		{
			tstring tstrDocumentPath = input[TEXT("d")];
 			FaxFileVector = GetDirectoryFileNames(tstrDocumentPath);
		}
		else
		{
			std::cout << "StressMng Usage:\n" <<
						 "/d:[Fax File Path or Fax Documents Repository Directory] mandatory parameter \n" <<
						 "/s:[Server Name] by default server is the local machine\n" <<
						 "/p:[Cover Page Path or CP Repository Directory] optional parameter \n"<<
						 "/t:[Minutes to execute test] by default will run \"forever\" \n" <<
						 "Note: spaces in path are allowed do not use the \" char \n";
			
			THROW_TEST_RUN_TIME_WIN32(GetLastError(), TEXT(" Main, bad application usage"));
		}

		if(input.IsExists(TEXT("s")))
		{
			tstrServerName = input[TEXT("s")];
		}

		if(input.IsExists(TEXT("p")))
		{
			tstring tstrCoverPagePath = input[TEXT("p")];
			CoverPageVector = GetDirectoryFileNames(tstrCoverPagePath);
		}
		if(input.IsExists(TEXT("t")))
		{
			dwTestDuration = input.GetNumber(TEXT("t"));
		}


		::lgInitializeLogger();
		::lgBeginSuite(TEXT("Stress"));
		::lgBeginCase(0, TEXT("0"));
		

		srand(GetTickCount());

		DWORD x_ComputerNameLenth = MAX_COMPUTERNAME_LENGTH;
			
		// if server name wasn't specified, use local machine name as the server.
		if(tstrServerName == TEXT(""))
		{
			TCHAR buffServerName[MAX_COMPUTERNAME_LENGTH + 1 ];
			if(!GetComputerName(buffServerName, &x_ComputerNameLenth))
			{
				THROW_TEST_RUN_TIME_WIN32(GetLastError(), TEXT(" GetComputerName"));
			}
			
			tstrServerName = buffServerName;
		}
	
		// initialize test manager
		pTest = new CTestManager(tstrServerName, dwTestDuration * 60);
		
		if(!pTest)
		{
			THROW_TEST_RUN_TIME_WIN32( ERROR_OUTOFMEMORY, TEXT(" Main, new CTestManager"));
		}
		
	
		DWORD dwJobNumber = 0;
		tstring tstrTestPref(TEXT(" StressTest"));

		// loop while test duration hasn't passed.
		while(!pTest->TestTimePassed())
		{
			JOB_PARAMS_EX pJparams;
		
			// Prepare job name
			++dwJobNumber;
			otstringstream otstrTestNum;
			otstrTestNum << dwJobNumber;
			tstring tstrTestName = tstrTestPref + otstrTestNum.str();
			
			PrepareRandomJob(pJparams,
							 FaxFileVector,
							 tstrTestName.c_str(),
							 CoverPageVector,
 							 RecipientNumbersList);
			
			DWORD dwAddRetVal;
			if(dwAddRetVal = pTest->AddSimpleJobEx(pJparams))
			{
				THROW_TEST_RUN_TIME_WIN32(dwAddRetVal, TEXT(" AddSimpleJobEx"));
			}

		}

		
	}
	catch(Win32Err& err)
	{
		::lgLogError(LOG_SEV_1, TEXT("Exception:%s."), err.Desc());
		fGenericFail = TRUE;
	}

  	
	::lgEndCase();
	::lgEndSuite();
	::lgCloseLogger();  

	if(	fGenericFail)
	{
		return 0;
	}
	return x_FaxNumberIndex;
}


