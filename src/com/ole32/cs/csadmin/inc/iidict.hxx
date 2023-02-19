/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 Copyright (c) 1989 Microsoft Corporation

 Module Name:
	
	iidict.hxx

 Abstract:


	definitions for class that stores the index of the class association entries
	based on class id.

 Notes:


 History:

 	VibhasC		Sep-29-1996		Created.

 ----------------------------------------------------------------------------*/
#ifndef __IIDICT_HXX__
#define __IIDICT_HXX__
/****************************************************************************
 *	include files
 ***************************************************************************/

extern "C"
	{
	#define INC_OLE2
	#include <windows.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <memory.h>
	#include <assert.h>
	}

#include "dict.hxx"
#include "message.hxx"


/****************************************************************************
 *	externs
 ***************************************************************************/
extern int CompareClassIDKey( void *, void *);
extern void PrintClassIDKey( void * );

extern int CompareClassIDKey( void *, void *);
extern void PrintClassIDKey( void * );

class IIDICT	: public Dictionary
	{
public:
	
	//
	// The dictionary constructor must be supplied with the
	// comparison and print routines.
	//

						IIDICT() : Dictionary( )
							{
							}


	//
	// The resource dictionary must delete all the resources it allocated.
	//

						~IIDICT()
							{
							Clear();
							}

	//
	// Insert an interface entry into the dictionary based on iid.
	// The entry is assumed preallocated and is not allocated by this
	// dictionary.
	//

	ITF_ENTRY		*	Insert( ITF_ENTRY * pInterfaceId);

	//
	// Given an interface ID, search for it in the dictionary.
	//

	ITF_ENTRY		*	Search( char * IID );

	//
	// Get the first item in the dictionary.
	//

	ITF_ENTRY     *   GetFirst();

	//
	// Get the next item in the dictionary. This method takes the last found
	// interface entry as a parameter and returns the next in the dictionary.
    // In conjunction with the GetFirst function, this function is useful for
    // listing all the items in the dictionary.
    // .
	//

	ITF_ENTRY     *   GetNext( ITF_ENTRY * pLastInterfaceEntry );

	//
	// Clear up the dictionary by deleting all the entries allocated.
	//

	void				Clear();

	//
	// Comparison function...
	//
	virtual
	int 			Compare (pUserType pL, pUserType pR);

	};

#endif // __IIDICT_HXX__
