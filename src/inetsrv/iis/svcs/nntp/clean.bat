@if (%_echo%)==() echo off
setlocal

set flavor=%1

REM
REM Remove any stray BUILD.* files.
REM

del build%flavor%.log build%flavor%.wrn build%flavor%.err /s >nul 2>&1

REM
REM Remove all OBJ and LIB directories.
REM

(for /f %%i in ('dir obj /ad /b /s') do (rd /s /q %%i >nul 2>&1)) >nul 2>&1
rd /s /q lib >nul 2>&1

if "%flavor%" == "" goto deletetlb
(for /f %%i in ('dir obj%flavor% /ad /b /s') do (rd /s /q %%i >nul 2>&1)) >nul 2>&1
rd /s /q lib%flavor% >nul 2>&1

REM
REM Individually remove all generated files.
REM

:deletetlb
del core\fixprop\src\mailmsg.tlb >nul 2>&1
del driver\nntpfs\src\nntpfs.tlb >nul 2>&1
del idl\nntpdrv\nntpdrv.tlb >nul 2>&1
del server\newstree\src\mailmsg.tlb >nul 2>&1
del server\post\src\cdo.tlb >nul 2>&1
del server\post\src\wstgado.tlb >nul 2>&1
del server\seo\ddrop\ddrop.tlb >nul 2>&1


REM
REM Delete the propagation directory.
REM

goto End

if not (%_nttree%)==() rd /s /q %_nttree%\iis >nul 2>&1
if not (%_nt386tree%)==() rd /s /q %_nt386tree%\iis >nul 2>&1
if not (%_ntalphatree%)==() rd /s /q %_ntalphatree%\iis >nul 2>&1
if not (%_ntia64tree%)==() rd /s /q %_ntia64tree%\iis >nul 2>&1
if not (%_ntaxp64tree%)==() rd /s /q %_ntaxp64tree%\iis >nul 2>&1

:End
