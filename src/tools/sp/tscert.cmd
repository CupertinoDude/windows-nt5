@echo off
REM  ------------------------------------------------------------------
REM
REM  tscert.cmd
REM     applies signature to terminal server files
REM
REM  owner: MadanA
REM
REM  Copyright (c) Microsoft Corporation. All rights reserved.
REM
REM  ------------------------------------------------------------------
if defined _CPCMAGIC goto CPCBegin
perl -x "%~f0" %*
goto :EOF
#!perl
use strict;
use lib $ENV{RAZZLETOOLPATH} . "\\PostBuildScripts";
use lib $ENV{RAZZLETOOLPATH};
use PbuildEnv;
use ParseArgs;
use Logmsg;

sub Usage { print<<USAGE; exit(1) }
tscert [-l <language>]

Applies signature to terminal server files
USAGE

sub Dependencies {
   if ( !open DEPEND, ">>$ENV{_NTPOSTBLD}\\..\\build_logs\\dependencies.txt" ) {
      errmsg("Unable to open dependency list file.");
      die;
   }
   print DEPEND<<DEPENDENCIES;
\[$0\]
IF {
   termdd.sys
   tdasync.sys
   tdipx.sys
   tdnetb.sys
   tdpipe.sys
   tdspx.sys
   tdtcp.sys
   tsddd.dll
   rdpdd.dll
   rdpwd.sys
   rdpwsx.dll
}
ADD {}

DEPENDENCIES
   close DEPEND;
   exit;
}

my $qfe;
parseargs('?'    => \&Usage,
          'plan' => \&Dependencies,
          'qfe:' => \$qfe);

if ( -f "$ENV{_NTPOSTBLD}\\..\\build_logs\\skip.txt" ) {
   if ( !open SKIP, "$ENV{_NTPOSTBLD}\\..\\build_logs\\skip.txt" ) {
      errmsg("Unable to open skip list file.");
      die;
   }
   while (<SKIP>) {
      chomp;
      exit if lc$_ eq lc$0;
   }
   close SKIP;
}


#            *** NEXT FEW LINES ARE TEMPLATE ***
$ENV{"_CPCMAGIC"}++;exit(system($0)>>8);
__END__
:CPCBegin
set _CPCMAGIC=
setlocal ENABLEDELAYEDEXPANSION ENABLEEXTENSIONS
REM          *** BEGIN YOUR CMD SCRIPT BELOW ***

set TsFileList=termdd.sys tdasync.sys tdipx.sys tdnetb.sys tdpipe.sys tdspx.sys
set TSFileList=%TsFileList% tdtcp.sys tsddd.dll rdpdd.dll rdpwd.sys rdpwsx.dll

for %%f in (%TsFileList%) do call :TsSign %_NTPOSTBLD%\%%f

goto end

:TSSign
REM Only sign file if it is not already signed
if not exist %1 (
  call logmsg.cmd "Image %1 does not exist"
  goto end
)
tscrtvfy.exe %1
if "%errorlevel%" == "0" (
  call logmsg.cmd "Image %1 is already signed (Not signing again)"
  goto end
)

call ExecuteCmd "tscrtadd.exe %1"
if not "%errorlevel%" == "0" (
  set errors=%errorlevel%
)
goto end


:end
seterror.exe "%errors%"& goto :EOF
