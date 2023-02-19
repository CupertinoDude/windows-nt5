
on error resume next

'***********************************************************
'First pass - create an ISWbemServices object with "default"
'impersonation level
'***********************************************************

set service = GetObject("winmgmts:\\ludlow")

select case service.security_.impersonationlevel

case 1
	ImpLevel = "Anonymous"
case 2 
	ImpLevel = "Identify"
case 3
	ImpLevel = "Impersonate"
case 4
	ImpLevel = "Delegate"

end select

WScript.Echo ""
WScript.Echo ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"
WScript.Echo ">>Attempting enumeration of Win32_Process with impersonation set to " _
			& ImpLevel
WScript.Echo ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"
WScript.Echo ""

for each Process in service.InstancesOf ("win32_process")
	WScript.Echo Process.ProcessId, Process.name
next

if err <> 0 then 
	WScript.Echo "*** FAILED - as expected ***"
	err.clear
else
	WScript.Echo "*** SUCCEEDED - error ***"
end if

'****************************************************************
'Second pass - create an ISWbemServices object with Impersonation
'"enabled"
'****************************************************************

set service = GetObject("winmgmts:{impersonationLevel=impersonate}!//ludlow")

select case service.security_.impersonationlevel

case 1
	ImpLevel = "Anonymous"
case 2 
	ImpLevel = "Identify"
case 3
	ImpLevel = "Impersonate"
case 4
	ImpLevel = "Delegate"

end select

WScript.Echo ""
WScript.Echo ""
WScript.Echo ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"
WScript.Echo ">>Attempting enumeration of Win32_Process with impersonation set to " _
			& ImpLevel 
WScript.Echo ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"
WScript.Echo ""



for each Process in service.InstancesOf ("win32_process")
	WScript.Echo Process.ProcessId, Process.name
next

WScript.Echo ""

if err <> 0 then 
	WScript.Echo "*** FAILED - error ***"
else
	WScript.Echo "*** SUCCEEDED - as expected ***"
	err.clear
end if

'****************************************************************
'Third pass - create an ISWbemServices object with Impersonation
'"anonymous"
'****************************************************************

set service = GetObject("winmgmts:{impersonationLevel=anonymous}!//ludlow")

select case service.security_.impersonationlevel

case 1
	ImpLevel = "Anonymous"
case 2 
	ImpLevel = "Identify"
case 3
	ImpLevel = "Impersonate"
case 4
	ImpLevel = "Delegate"

end select

WScript.Echo ""
WScript.Echo ""
WScript.Echo ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"
WScript.Echo ">>Attempting enumeration of Win32_Process with impersonation set to " _
			& ImpLevel 
WScript.Echo ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"
WScript.Echo ""



for each Process in service.InstancesOf ("win32_process")
	WScript.Echo Process.ProcessId, Process.name
next

WScript.Echo ""

if err <> 0 then 
	WScript.Echo "*** FAILED - as expected ***"
	err.clear
else
	WScript.Echo "*** SUCCEEDED - error ***"
end if

'****************************************************************
'Third pass - create an ISWbemServices object with Impersonation
'"delegate"
'****************************************************************

set service = GetObject("winmgmts:{impersonationLevel=delegate}!//ludlow")

select case service.security_.impersonationlevel

case 1
	ImpLevel = "Anonymous"
case 2 
	ImpLevel = "Identify"
case 3
	ImpLevel = "Impersonate"
case 4
	ImpLevel = "Delegate"

end select

WScript.Echo ""
WScript.Echo ""
WScript.Echo ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"
WScript.Echo ">>Attempting enumeration of Win32_Process with impersonation set to " _
			& ImpLevel 
WScript.Echo ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"
WScript.Echo ""



for each Process in service.InstancesOf ("win32_process")
	WScript.Echo Process.ProcessId, Process.name
next

WScript.Echo ""

if err <> 0 then 
	WScript.Echo "*** FAILED - as expected ***"
	err.clear
else
	WScript.Echo "*** SUCCEEDED - error ***"
end if




