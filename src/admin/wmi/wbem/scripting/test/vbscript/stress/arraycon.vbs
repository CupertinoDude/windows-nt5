'***************************************************************************
'This script tests the manipulation of context values, in the case that the
'context value is an array type
'***************************************************************************
On Error Resume Next

while true
Set Context = CreateObject("WbemScripting.SWbemNamedValueSet")


Context.Add "n1", Array (1, 2, 3)
str = "The initial value of n1 is {"
for x=LBound(Context("n1")) to UBound(Context("n1"))
	str = str & Context("n1")(x)
	if x <> UBound(Context("n1")) Then
		str = str & ", "
	End if
next
str = str & "}"
WScript.Echo str

WScript.Echo ""

'Verify we can report the value of an element of the context value
v = Context("n1")
WScript.Echo "By indirection the first element of n1 has value:",v(0)

'Verify we can report the value directly
WScript.Echo "By direct access the first element of n1 has value:", Context("n1")(0)

'Verify we can set the value of a single named value element
Context("n1")(1) = 11 
WScript.Echo "After direct assignment the first element of n1 has value:", Context("n1")(1)

'Verify we can set the value of a single named value element
Set v = Context("n1")
v(1) = 345
WScript.Echo "After indirect assignment the first element of n1 has value:", Context("n1")(1)

'Verify we can set the value of an entire context value
Context("n1") = Array (5, 34, 178871)
WScript.Echo "After direct array assignment the first element of n1 has value:", Context("n1")(1)

str = "After direct assignment the entire value of n1 is {"
for x=LBound(Context("n1")) to UBound(Context("n1"))
	str = str & Context("n1")(x)
	if x <> UBound(Context("n1")) Then
		str = str & ", "
	End if
next
str = str & "}"
WScript.Echo str

if Err <> 0 Then
	WScript.Echo Err.Description
	Err.Clear
End if

wend