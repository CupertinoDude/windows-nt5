/******************************************************************************\
 *	FILE:	toolprs.c
 *
 *	This is the command line argument parsing code called by the template
 *	toolmain.c, which is used in all the command line tools.  Since it may
 *	be necessary to supp
\******************************************************************************/

// Standard Headers.
// SETUP: all tools need common.h, some need other headers as well, 
// add any needed ones.
#include <stdlib.h>
#include <direct.h>		// For _wgetcwd()
#include <time.h>		// For time() & ctime()
#include <fcntl.h>
#include <io.h>
#include "common.h"
#include "toolmain.h"

// Place to store program name.
static wchar_t		g_pProgram[_MAX_FNAME];

// Timing information used by start and end status procedures.
static time_t		g_timeStart, g_timeEnd;

// Code to actually process a switch.
// SETUP: You only need to change this if you have a new switch type.
// Return of -1 is error, otherwise, this specifies how many extra
// parameters were used.
static int
ProcessSwitch(
	SWITCH_SPEC	const *pSwitchSpec,	// Selected switch spec.
	int			argc,				// Number of remaining arguments
	wchar_t		**argv				// Pointer to remaining arguments
) {
	switch (pSwitchSpec->switchType) {
		case SWITCH_HELP :		// Return error, whitch prints help.
			return -1;

		case SWITCH_BOOL :		// Boolean,
			*(BOOL *)pSwitchSpec->pSwitch	= TRUE;
			break;

		case SWITCH_STRING :	// A string.
			// Make sure value exists.
			if (argc < 1) {
				fwprintf(stderr, L"Missing value for switch '%c'\n",
					pSwitchSpec->wchSwitch
				);
				return -1;
			}

			*(wchar_t **)pSwitchSpec->pSwitch	= argv[0];
			return 1;

		case SWITCH_UNSIGNED :	{	// An unsigned number.
			unsigned int		iValue;
			wchar_t				*pEnd;

			// Make sure value exists.
			if (argc < 1) {
				fwprintf(stderr, L"Missing value for switch '%c'\n",
					pSwitchSpec->wchSwitch
				);
				return -1;
			}

			// Convert number
			iValue	= wcstoul(argv[0], &pEnd, 10);
			if (*pEnd != L'\0' || pEnd == argv[0]) {
				fwprintf(stderr, L"Invalid switch value for switch '%c'\n",
					pSwitchSpec->wchSwitch
				);
				return -1;
			}

			*(unsigned int *)pSwitchSpec->pSwitch	= iValue;
			return 1;
		}

		case SWITCH_INT :	{	// An integer
			int			iValue;
			wchar_t		*pEnd;

			// Make sure value exists.
			if (argc < 1) {
				fwprintf(stderr, L"Missing value for switch '%c'\n",
					pSwitchSpec->wchSwitch
				);
				return -1;
			}

			// Convert number
			iValue	= wcstol(argv[0], &pEnd, 10);
			if (*pEnd != L'\0' || pEnd == argv[0]) {
				fwprintf(stderr, L"Invalid switch value for switch '%c'\n",
					pSwitchSpec->wchSwitch
				);
				return -1;
			}

			*(int *)pSwitchSpec->pSwitch	= iValue;
			return 1;
		}

		case SWITCH_DOUBLE :	{	// A floating point number.
			float				eValue;
			int					count;
			int					length;
			wchar_t				*pEnd;

			if (argc < 1) {
				fwprintf(stderr, L"Missing value for switch '%c'\n",
					pSwitchSpec->wchSwitch
				);
				return -1;
			}

			// Convert number
			count	= swscanf(argv[0], L"%f%n", &eValue, &length);
			pEnd	= argv[0] + length;
			if (count != 1 || *pEnd != L'\0' || pEnd == argv[0]) {
				fwprintf(stderr, L"Invalid switch value for switch '%c'\n",
					pSwitchSpec->wchSwitch
				);
				return -1;
			}

			*(double *)pSwitchSpec->pSwitch	= (double)eValue;
			return 1;
		}

		default:
			fwprintf(stderr, L"Program error in switch parser!\n");
			exit(-2);
	}

	return 0;  // Default, no extra args used.
}

// Return a string representing the value of the passed in switch.
// SETUP: You only need to change this if you added a new type above.
static wchar_t *
SwitchToString(SWITCH_SPEC	const *pSwitchSpec)
{
	static wchar_t		aRetBuf[256];

	switch (pSwitchSpec->switchType) {
		case SWITCH_HELP :		// Don't bother showing state for help.
			return (wchar_t *)0;;

		case SWITCH_BOOL :		// Boolean,
			if (*(BOOL *)pSwitchSpec->pSwitch) {
				return L"TRUE";
			} else {
				return L"FALSE";
			}
			break;	// Should never get here.

		case SWITCH_STRING :	// A string.
			return *(wchar_t **)pSwitchSpec->pSwitch;

		case SWITCH_UNSIGNED :	// An unsigned number.
			swprintf(aRetBuf, L"%u", *(unsigned int *)pSwitchSpec->pSwitch);
			return aRetBuf;

		case SWITCH_INT :	// An integer.
			swprintf(aRetBuf, L"%d", *(int *)pSwitchSpec->pSwitch);
			return aRetBuf;

		case SWITCH_DOUBLE :	// An unsigned number.
			swprintf(aRetBuf, L"%f", *(double *)pSwitchSpec->pSwitch);
			return aRetBuf;

		default:
			fwprintf(stderr, L"Program error in switch printing!\n");
			exit(-2);
	}

	// Should never get here, but don't fail if we do.
	return (wchar_t *)0;;
}

// Code to actually process an argument.
// SETUP: You only need to change this if you have a new argument type.
// Return of FALSE is error, TRUE is success.
static BOOL
ProcessArgument(ARG_SPEC const *pArgSpec, wchar_t *pArgText)
{
	BOOL	fFixMode;
	FILE	*pFile;
	wchar_t	pError[256];
	wchar_t	pFileInput[256];

	// Record text of argument.
	*pArgSpec->ppText	= pArgText;

	// Now process based on type of argument.
	switch (pArgSpec->argType) {
		case ARG_STRING :		// Any old string
			// Saving the string is all we needed to do.
			break;

		case ARG_FILE :			// File to open with specified mode.
			// Check for special names for standard IO files.
			fFixMode	= FALSE;
			if (wcscmp(pArgText, ARG_STDIN) == 0) {
				pFile		= stdin;
				fFixMode	= TRUE;
			} else if (wcscmp(pArgText, ARG_STDOUT) == 0) {
				pFile		= stdout;
				fFixMode	= TRUE;
			} else if (wcscmp(pArgText, ARG_STDERR) == 0) {
				pFile		= stderr;
				fFixMode	= TRUE;
			} else {
				// Normal open code.
				pFile = _wfopen(pArgText, pArgSpec->pMode);
				if (!pFile) {
					swprintf(pError, L"Couldn't open %s with mode %s", 
						pArgText, pArgSpec->pMode
					);
					_wperror(pError);
					return FALSE;;
				}
			}

			// For already opened files, we may need to make them binary.
			if (fFixMode 
				&& (pArgSpec->pMode[wcscspn(pArgSpec->pMode, L"bB")] != '\0')
			) {
				int		result;

				result = _setmode(_fileno(pFile), _O_BINARY);
				if( result == -1 ) {
					swprintf(pError, L"Couldn't set %s to binary mode",pArgText);
					_wperror(pError);
				}
			}
			*(FILE **)pArgSpec->pValue	= pFile;
			break;

		case ARG_FILE_UTIL :	// File to open with UtilOpen
			// Do the open.
			pFile = UtilOpen(
				pArgText, pArgSpec->pMode, 
				pFileInput, 256
			);

			// Did it work?
			if (!pFile) {
				swprintf(pError, L"Couldn't open %s with mode %s", 
					pArgText, pArgSpec->pMode
				);
				_wperror(pError);
				return FALSE;;
			}

			// Return results, note that the path is never freed.
			*pArgSpec->ppText	= _wcsdup(pFileInput);
			*(FILE **)pArgSpec->pValue	= pFile;
			break;

		case ARG_UNSIGNED :	{	// An unsigned number.
			unsigned int		iValue;
			wchar_t				*pEnd;

			// Convert number
			iValue	= wcstol(pArgText, &pEnd, 10);
			if (*pEnd != L'\0' || pEnd == pArgText) {
				fwprintf(stderr, L"Not a valid unsigned number: %s\n", pArgText);
				return FALSE;
			}

			*(unsigned int *)pArgSpec->pValue	= iValue;
			break;
		}

		case ARG_DOUBLE :	{	// A floating point number.
			float				eValue;
			int					count;
			int					length;
			wchar_t				*pEnd;

			// Convert number
			count	= swscanf(pArgText, L"%f%n", &eValue, &length);
			pEnd	= pArgText + length;
			if (count != 1 || *pEnd != L'\0' || pEnd == pArgText) {
				fwprintf(stderr, L"Not a valid floating point number: %s\n", pArgText);
				return FALSE;
			}

			*(double *)pArgSpec->pValue	= (double)eValue;
			break;
		}

		default:
			fwprintf(stderr, L"Program error in argument parser!\n");
			exit(-2);
	}

	return TRUE;  // Default, success.
}

// Code to cleanup anything related to an argument just before the
// program exits.  Typically this is where we close the files we
// opened at startup time.
// SETUP: You only need to change this if you have a new argument type.
// Return of FALSE is error, TRUE is success.
static BOOL
CleanupArgument(ARG_SPEC const *pArgSpec)
{
	// Now process based on type of argument.
	switch (pArgSpec->argType) {
		case ARG_FILE :			// File to open with specified mode.
		case ARG_FILE_UTIL :	// File to open with UtilOpen
			// Close up the file.
			if (fclose(*(FILE **)pArgSpec->pValue) != 0) {
				return FALSE;
			}
			break;

		case ARG_STRING :		// Any old string
		default:
			// No processing needed before exit.
			break;
	}

	return TRUE;  // Default, success.
}

// Code to parse the arguments.
BOOL
ToolMainParseArguments(PARSE_INFO const *pParseInfo, int argc, wchar_t **argv)
{
	int		cArgsParsed;
	wchar_t	pDrive[_MAX_DRIVE], pDir[_MAX_DIR], pExt[_MAX_EXT];

	// Deal with program name.
	_wsplitpath(argv[0], pDrive, pDir, g_pProgram, pExt); 
	--argc;
	++argv;

	// Scan all the arguments.
	cArgsParsed		= 0;
	while (argc > 0) {
		// Check for switches
		if (argv[0][0] == L'-') {
			// We have a switch list
			wchar_t		*pScan;
			int			cSwitchValues;

			pScan			= argv[0] + 1;
			--argc;
			++argv;
			for (; *pScan; ++pScan) {
				SWITCH_SPEC		const *pScanSS;
				SWITCH_SPEC		const *pLimitSS;

				// Figure out which switch it is.
				pLimitSS  = pParseInfo->pSwitchSpecs + pParseInfo->cSwitchSpecs;
				for (
					pScanSS = pParseInfo->pSwitchSpecs;
					pScanSS < pLimitSS;
					++pScanSS
				) {
					if (*pScan == pScanSS->wchSwitch) {
						// Found switch
						break;
					}
				}

				// Check for falling off end of list.
				if (pScanSS >= pLimitSS) {
					fwprintf(stderr, L"Unknown switch '%c'\n", *pScan);
					return FALSE;
				}

				// Found switch, process it.
				cSwitchValues	= ProcessSwitch(pScanSS, argc, argv);
				if (cSwitchValues < 0) {
					return FALSE;
				}
				argc			-= cSwitchValues;
				argv			+= cSwitchValues;
			}
		} else {
			// We have an argument.  Have we run out of known ones?
			if (cArgsParsed >= pParseInfo->cArgSpecs) {
				fwprintf(stderr, L"Too many arguments provided.\n");
				return FALSE;
			}

			// OK, it is expected, process it.
			if (!ProcessArgument(
				pParseInfo->pArgSpecs + cArgsParsed, argv[0]
			)) {
				return FALSE;
			}

			// Argument successfully processed.
			++cArgsParsed;
			--argc;
			++argv;
		}
	}
	
	// Handle defaulted arguments.
	while (cArgsParsed < pParseInfo->cArgSpecs) {
		if (!pParseInfo->pArgSpecs[cArgsParsed].pDefault) {
			fwprintf(stderr, L"Too few arguments specified.\n");
			return FALSE;
		}

		// Handle default value.
		if (!ProcessArgument(
			pParseInfo->pArgSpecs + cArgsParsed,
			pParseInfo->pArgSpecs[cArgsParsed].pDefault
		)) {
			return FALSE;
		}

		// Argument successfully defaulted.
		++cArgsParsed;
	}

	return TRUE;
}

// Load in locale information.
BOOL
ToolMainLoadLocale(
	PARSE_INFO		const *pParseInfo,
	LOCRUN_INFO		*pLocRunInfo,
	LOCTRAIN_INFO	*pLocTrainInfo
) {
	wchar_t			aPath[128];
	FILE			*pLogTo		= *pParseInfo->ppLogFile ? *pParseInfo->ppLogFile : stderr;

	// Generate path for locale files.
	FormatPath(aPath, *pParseInfo->ppTreeRoot, L"tsunami\\dll",
		*pParseInfo->ppLocale, *pParseInfo->ppConfigName, (wchar_t *)0
	);

	// Load runtime localization information from a file.
	if (!LocRunLoadFile(pLocRunInfo, aPath)) {
		fwprintf(pLogTo, L"Can't open runtime localization file.\n");
		return FALSE;
	}

	// Load runtime localization information from a file.
	if (!LocTrainLoadFile(pLocRunInfo, pLocTrainInfo, aPath)) {
		fwprintf(pLogTo, L"Can't open train time localization file.\n");
		return FALSE;
	}

	return TRUE;
}

// Load in locale information.
BOOL
ToolMainLoadLocaleEx(
	PARSE_INFO		const *pParseInfo,
	LOCRUN_INFO		*pLocRunInfo,
	LOCTRAIN_INFO	*pLocTrainInfo,
	wchar_t			*pRecognizer
) {
	wchar_t			aPath[128];
	FILE			*pLogTo		= *pParseInfo->ppLogFile ? *pParseInfo->ppLogFile : stderr;

	// Generate path for locale files.
	FormatPath(aPath, *pParseInfo->ppTreeRoot, pRecognizer, L"dll",
		*pParseInfo->ppLocale, *pParseInfo->ppConfigName
	);

	// Load runtime localization information from a file.
	if (!LocRunLoadFile(pLocRunInfo, aPath)) {
		fwprintf(pLogTo, L"Can't open runtime localization file.\n");
		return FALSE;
	}

	// Load runtime localization information from a file.
	if (!LocTrainLoadFile(pLocRunInfo, pLocTrainInfo, aPath)) {
		fwprintf(pLogTo, L"Can't open train time localization file.\n");
		return FALSE;
	}

	return TRUE;
}

// Code to print out usage message.
void
ToolMainUsage(PARSE_INFO const *pParseInfo)
{
	int		ii;

	fwprintf(stderr, L"Usage: %s %s\n",
		g_pProgram, pParseInfo->ppUsageStrings[0]
	);

	for (ii = 1; ii < pParseInfo->cUsageStrings; ++ii) {
		fwprintf(stderr, L" %s\n", pParseInfo->ppUsageStrings[ii]);
	}
}

// Output header for log file.
void
ToolMainLogHeader(PARSE_INFO const *pParseInfo, int argc, wchar_t **argv)
{
	FILE	* const pLogFile	= *pParseInfo->ppLogFile;
	int			ii;
	int			maxName;
	wchar_t		pWorkingDir[256];

	// Log name of program and the command line.
	fwprintf(pLogFile, L"Application: %s\n", g_pProgram);

	fwprintf(pLogFile, L"Command line: ");
	for (ii = 0; ii < argc; ++ii) {
		fwprintf(pLogFile, L"%s ", argv[ii]);
	}
	putwc('\n',pLogFile);

	// Log directory we are running from.
	_wgetcwd(pWorkingDir, 256);
	fwprintf(pLogFile, L"Working Directory: %s\n", pWorkingDir);

	// Log the values for each switch.
	fwprintf(pLogFile, L"Program switch values:\n");
	for (ii = 0; ii < pParseInfo->cSwitchSpecs; ++ii) {
		wchar_t		*pValueStr;

		// Make sure we have a value worth printing.
		pValueStr	= SwitchToString(pParseInfo->pSwitchSpecs + ii);
		if (pValueStr) {
			fwprintf(pLogFile, L"  -%c is %s\n",
				pParseInfo->pSwitchSpecs[ii].wchSwitch, pValueStr
			);
		}
	}

	// Figure out longest label to use in formating list.
	maxName	= 0;
	for (ii = 0; ii < pParseInfo->cArgSpecs; ++ii) {
		int		cName;

		cName	= wcslen(pParseInfo->pArgSpecs[ii].pName);
		if (maxName < cName) {
			maxName	= cName;
		}
	}

	// Log values for each parameter
	fwprintf(pLogFile, L"Program argument values:\n");
	for (ii = 0; ii < pParseInfo->cArgSpecs; ++ii) {
		fwprintf(pLogFile, L"%*s: %s\n",
			maxName + 2, pParseInfo->pArgSpecs[ii].pName,
			*pParseInfo->pArgSpecs[ii].ppText
		);
	}

	// Log the time we started.
	time(&g_timeStart);
	fwprintf(pLogFile, L"Start Time: %s", _wctime(&g_timeStart));

	// Blank line to seperate this from anything the program logs.
	putwc('\n', pLogFile);
}

// Output Trailer for log file.
void
ToolMainLogTrailer(PARSE_INFO const *pParseInfo)
{
	FILE	* const pLogFile	= *pParseInfo->ppLogFile;

	// A Blank line to seperate from any logged messages.
	putwc('\n', pLogFile);

	// When did we finish, and how long did we run?
	time(&g_timeEnd);
	fwprintf(pLogFile, L"End Time: %s", _wctime(&g_timeEnd));
	fwprintf(pLogFile, L"Elapsed Time: %d seconds\n", g_timeEnd - g_timeStart);
}

// Close all the files we opened up.
BOOL
ToolMainCleanup(
	PARSE_INFO		const *pParseInfo,
	LOCRUN_INFO		*pLocRunInfo,
	LOCTRAIN_INFO	*pLocTrainInfo
) {
	BOOL	status;
	int		ii;

	// Default to success.
	status	= TRUE;

	// Close up the arguments.
	for (ii = 0; ii < pParseInfo->cArgSpecs; ++ii) {
		if (!CleanupArgument(pParseInfo->pArgSpecs + ii)) {
			status	= FALSE;
		}
	}

	// Close up locale files if needed.
	if (pLocRunInfo && !LocRunUnloadFile(pLocRunInfo)) {
		status	= FALSE;
	}
	if (pLocTrainInfo && !LocTrainUnloadFile(pLocTrainInfo)) {
		status	= FALSE;
	}

	// Give some indication of a problem to the outside world if cleanup
	// was not successfull.
	if (!status) {
		fwprintf(stderr, L"Error during cleanup?!?\n");
	}

	// Let caller know status.
	return status;
}

