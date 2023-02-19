The source code in this directory can be used to build a sample Policy Module
for Microsoft Certificate Services.  It is meant to run on Windows 2000 or
later.  Certificate Services must already be installed.

This policy module should mimic the Standalone policy module that ships with
the Windows 2000 Certificate Services.

Certificate Services calls the Policy Module through the ICertPolicy
interface, and the Policy Module can call back to Certificate Services
through the ICertServerPolicy interface.

Each time Certificate Services receives a certificate request, it passes
control to the ICertPolicy::VerifyRequest method in policy.cpp.  The passed
Context parameter is used with the ICertServerPolicy interface to retrieve
properties from the request and potential certificate.  The VerifyRequest
method may add, modify or enable Certificate Extensions, modify the NotBefore
and NotAfter dates and Subject name RDN (Relative Distinguished Name) strings
for the potential certificate.  It must also perform any validation required,
and decide the disposition of the request.  The method should return one of
VR_PENDING, VR_INSTANT_OK or VR_INSTANT_BAD to cause the request to be made
pending, to grant the request and issue the certificate, or to fail the
request.

Once the certpsam.dll DLL is built, its COM interface must be registered
via the following command:
    regsvr32 certpsam.dll

Once registered, the Certification Authority management console snapin can
be used to make this the active policy module.

If you wish to see the debug print output from the sample Policy Module,
attach a debugger to the running service process, or stop the Certificate
Services service, then start Certificate Services as a console application
via the following command:
    certsrv -z

Files:
------
atl.cpp      -- ActiveX Template Library COM support code

cedebug.cpp  -- Implements debug print routine

ceerror.cpp  -- Implements error handling routines

ceformat.cpp -- Implements formatting routine

celib.cpp    -- Implements support routines

certpsam.cpp -- Implements COM and initialization entry points:
			DllMain
			DllCanUnloadNow
			DllGetClassObject
			DllRegisterServer
			DllUnregisterServer

certpsam.def -- Exports COM entry points

certpsam.idl -- Interface Definitions

certpsam.rc  -- Version Resource

module.cpp   -- Implements ICertManageModule

module.h     -- Implements ICertManageModule

pch.cpp      -- Precompiled Header file

policy.cpp   -- Implements ICertPolicy

policy.h     -- Implements ICertPolicy

resource.h   -- Resource ID definitions
