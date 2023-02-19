'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
Option Explicit
On Error Resume Next

Dim Path
Path = "IIS://LocalHost/W3SVC/1/ROOT/wmi/soap"

'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

WScript.Echo "Listing scriptmaps to local machine at " & Path
WScript.Echo ""

'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' Try to get the node in the metabase
Dim oNode
Set oNode = GetObject (Path)
If (Err <> 0) Then
	WScript.Echo "Unexpected error getting path object " & Path
	WScript.Echo "Error: " & Err.Number & " (" & Hex (Err.Number) & ")"
	WScript.Echo Err.Description
	WScript.Quit (Err.Number)
End If

'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' Get ScriptMap entry in specified Path
Dim ScriptMap
ScriptMap = oNode.scriptmaps
If (Err <> 0) Then
	WScript.Echo "Unexpected error getting scriptmaps from object."
	WScript.Echo "Error: " & Err.Number & " (" & Hex (Err.Number) & ")"
	WScript.Echo Err.Description
	WScript.Quit (Err.Number)
End If

'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' Enumerate existing ScriptMap entries
Dim entry
for each entry in ScriptMap 
Wscript.Echo entry.KeyType
count = count + 1
next


