#ifndef __h323fltr_crv_h__
#define __h323fltr_crv_h__

HRESULT InitCrvAllocator();

HRESULT CleanupCrvAllocator();

// This should be moved into some common.h file
// The H.225 spec calls for a 2 byte call reference value
typedef WORD  CALL_REF_TYPE;

// If this flag is set in the CallReferenceValue then the
// PDU is sent by the originator of the call and vice-versa.
#define CALL_REF_FLAG 0x8000

// allocate a call reference value
// uses random numbers to exploit sparse usage of the
// call reference value space
BOOL    AllocCallRefVal(
    OUT CALL_REF_TYPE &CallRefVal
    );

// frees a currently allocated call ref value
void    DeallocCallRefVal(
    IN CALL_REF_TYPE CallRefVal
    );

#endif // __h323fltr_crv_h__
