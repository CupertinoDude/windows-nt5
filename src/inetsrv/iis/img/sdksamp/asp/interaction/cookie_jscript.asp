<%@ LANGUAGE = JScript %>

<!*************************
This sample is provided for educational purposes only. It is not intended to be 
used in a production environment, has not been tested in a production environment, 
and Microsoft will not provide technical support for it. 
*************************>

<%
	//Changes to the HTTP response header, in which cookies
	//are sent back to the client, must be made before
	//sending any data to the user.
	
	Response.Expires = 0;
	
	
	//Get the last visit date/time string from the cookie that
	//resides in the HTTP request header.
	
	var LastVisitCookie;
	LastVisitCookie = Request.Cookies("CookieJScript");
	
	
	//Send the current date/time string in a cookie enclosed
	//in the response header. Note that because IE now uses
	//case-sensitive cookie paths, we have explicitly set
	//the cookie path to be that of the URL path to this
	//page. By default, the path would be that of the
	//IIS Application context for this page ("IISSAMPLES").
	
	var CurrentDate = new Date();
	Response.Cookies("CookieJScript") = CurrentDate.toLocaleString();
%>

<HTML>
    <HEAD>
		<TITLE>Using Cookies</TITLE>
    </HEAD>

    <BODY BGCOLOR="White" TOPMARGIN="10" LEFTMARGIN="10">

    	<FONT SIZE="4" FACE="ARIAL, HELVETICA">
    	<B>Using Cookies</B></FONT><BR>
      
        <HR SIZE="1" COLOR="#000000">
		
		<%
			if (LastVisitCookie == "") 
			{
				//The cookie has never been set. This must
				//be the user's first visit to this page.
				
				Response.Write("Welcome to this page.");
			}
			else
			{
				//Remind the user of the last time she/he
				//visited this page.

				Response.Write("You last visited this page on " + LastVisitCookie);
			}		
		%>

		<P><A HREF="Cookie_JScript.asp">Revisit This Page</A>

	</BODY>
</HTML>
