<% On Error Resume Next %>
<% if (Request.ServerVariables("QUERY_STRING") = "")  then %>
	<!-- Phase 1: The first condition will be executed only for the Verification suite
	This part of the code sends the FORM and VALIDDATA tags so that client knows
     	what to do
	-->

	<HTML>
	<HEAD>
	</HEAD>
	<BODY>

	<!-- This sets up the query string that the client will send later
	-->

	<FORM  METHOD="GET" ACTION="http://localhost/smoke/ssess5.asp?test/">
	<input type=submit value="Submit Form">
	</FORM>

	<!-- This tells the client what to expect as the valid data
	-->

	<VALIDDATA>
	<P></P>
	</VALIDDATA>

	<% Session("foo") = 1 %>
	<% Session.Abandon %>		<!-- changed for bug fix -->

	</BODY>
	</HTML>

<% else %>

	<!-- Phase 2: This is the part that really tests Denali
	-->

	<HTML>
	<HEAD>
	</HEAD>
	<BODY>

	<!-- This starts sending the results that the client is expecting
	-->

	<OUTPUT>
	<P><% =Session("foo") %></P>
	</OUTPUT>

	</BODY>
	</HTML>

<% end if %>

