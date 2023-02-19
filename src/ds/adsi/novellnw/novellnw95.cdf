#define quote(X) #X
[Version]
Class=IEXPRESS
CDFVersion=2.0
[Options]
Quantum=7
ExtractOnly=0
ShowInstallProgramWindow=0
HideExtractAnimation=0
UseLongFileName=0
RebootMode=I
InstallPrompt=%InstallPrompt%
DisplayLicense=%DisplayLicense%
FinishMessage=%FinishMessage%
TargetName=%TargetName%
FriendlyName=%FriendlyName%
AppLaunched=%AppLaunched%
PostInstallCmd=%PostInstallCmd%
SourceFiles=SourceFiles
[Strings]
InstallPrompt="Welcome to Active Directory Service Interfaces (ADSI) NWCOMPAT provider for Novell clients. This will install the binaries for the Netware 3x compatible ADSI provider for Novell clients. Are you sure you want to continue?"
DisplayLicense=LICENSEFILE
FinishMessage="Active Directory Service Interface (ADSI) NWCOMPAT provider for Novell clients installation done!"
TargetName=TARGETNAME
FriendlyName="Active Directory Service Interfaces (ADSI) NWCOMPAT Provider for Novell Clients"
AppLaunched="novellnw95.inf"
PostInstallCmd="<None>"
FILE0="adsnw3.dll"
FILE1="novellnw95.inf"
[SourceFiles]
SourceFiles0=SOURCEFILES
[SourceFiles0]
%FILE0%
%FILE1%
