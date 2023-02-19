//***************************************************************************
//This script tests the manipulation of property values, in the case that the
//property is an array type.
//Note that because we have to wrap the all array values inside a VBArray it is
//not possible to test "direct" access.
//***************************************************************************
var locator = WScript.CreateObject ("WbemScripting.SWbemLocator");
var service = locator.ConnectServer (".", "root/default");
var Class = service.Get();

Class.Path_.Class = "ARRAYPROP00";
var Property = Class.Properties_.Add ("p", 19, true);

//****************************************
//First pass of tests works on non-dot API
//****************************************

WScript.Echo ("");
WScript.Echo ("PASS 1 - Use Non-Dot Notation");
WScript.Echo ("");

//Verify we can set the value of a complete property array value.

Property.Value = new Array (1, 2, 44);

var str = "After direct assignment the initial value of p is [1,2,44]: {";
var value = new VBArray (Property.Value).toArray ();
WScript.Echo ("The length of the array is [3]:", value.length);
for (var x=0; x < value.length; x++) {
	if (x != 0) {
		str = str + ", ";
	}
	str = str + value[x];
}

str = str + "}";
WScript.Echo (str);

//Verify we can report the value of an element of the property value.
var v = new VBArray (Property.Value).toArray ();
WScript.Echo ("By indirection the p[2] had value [44]:",v[2]);

//Verify we can set the value of a single property value element
var p = new VBArray (Property.Value).toArray ();
p[1] = 345;
Property.Value = p;
var value = new VBArray (Property.Value).toArray ();
WScript.Echo ("After indirect assignment p[1] has value [345]:", value[1]);

//****************************************
//Second pass of tests works on dot API
//****************************************

WScript.Echo ("");
WScript.Echo ("PASS 2 - Use Dot Notation");
WScript.Echo ("");

//Verify we can report the array of a property using the dot notation
var value = new VBArray (Class.p).toArray ();
var str = "By direct access via the dot notation the entire value of p is [1,345,44]: {";
for (var x = 0; x < value.length; x++) {
	if (x != 0) {
		str = str + ", ";
	}
	str = str + value[x];
}

str = str + "}";
WScript.Echo (str);

//Verify we can report the value of a property array element using the "dot" notation
WScript.Echo ("By direct access p1[1] has value [345]:", (new VBArray(Class.p).toArray())[1]);

//Verify we can set the entire array value using dot notation
Class.p = new Array (412, 3, 544)
var value = new VBArray (Class.p).toArray ();
var str = "By direct access via the dot notation the entire value of p has been set to [412,3,544]: {";
for (var x = 0; x < value.length; x++) {
	if (x != 0) {
		str = str + ", ";
	}
	str = str + value[x];
}

str = str + "}";
WScript.Echo (str);

//Verify we can set the value of a property array element using the "dot" notation
// NB - Note that using [] rather than () does NOT work as JScript appears to first 
// interpret this as a request to retrieve Property.Value; the assignment is therefore 
// only made on the local temporary copy of the value
Class.p(2) = 8889;
WScript.Echo ("By direct access p(2) has been set to [8889]:", (new VBArray(Class.p).toArray())[2]);

Class.Put_ ();