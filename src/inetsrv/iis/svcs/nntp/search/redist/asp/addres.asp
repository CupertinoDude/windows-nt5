<%
 REM localization
 
 L_PAGETITLETXT			= "News Offline Search Admin"
 L_NEWSOFFLINESEARCHTXT	= "Your query has been added."
 
 rem END LOCALIZATION
 
 %>

<!DOCTYPE HTML PUBLIC "-//IETF//DTD HTML//EN">
<html>

<head>
<meta http-equiv="Content-Type"
content="text/html; charset=iso-8859-1">
<meta name="GENERATOR" content="Microsoft FrontPage 2.0">
<title><%= L_PAGETITLETXT %></title>
</head>
<body background="images\gnback.gif" text="white">
<%if IsNumeric( Request.Form("search_frequency") ) = true or Request.Form("search_frequency") = "" then%> 
<%if (Request.Form("by_news") = "on" and Request.Form("news_group") <> "") or Request.Form("by_news") <> "on" then%>
<%if (Request.Form("by_mail") = "on" and Request.Form("email_address") <> "") or Request.Form("by_mail") <> "on" then%>
<%if Request.Form("by_mail") <> "" or Request.Form("by_news") <> "" then%>
<%Set querybase = Server.CreateObject("req.req.1")%>
<% set x = querybase.NewX
   x.EmailAddress = Request.Form("email_address")
   x.QueryString = Request.Form("query_string")
   x.LastDate = Request.Form("last_date")
   x.SearchFrequency = Request.Form("search_frequency")
   x.Message_Template = Request.Form("message_template")
   x.URL_Template = Request.Form("url_template")
   x.NewsGroup = Request.Form("news_group")
   if  Request.Form("by_mail") = "on" and Request.Form("by_news") = "on" then
   		x.ReplyMode = "both"
   else
   	   if Request.Form("by_mail") = "on" then
	   		x.ReplyMode = "mail"
	   else
	   	    if Request.Form("by_news") = "on" then
	   	    	x.ReplyMode = "news"
			else
			    x.ReplyMode = "mail"
			end if
	   end if
   end if
   querybase.Save(x)
%>
<p align="center"><font size="4"><strong><%= L_NEWSOFFLINESEARCHTXT %></strong></font></p>
<%else%>
	  <script language="javascript">
	      alert("You must specify at least one reply mode. Use browser back botton to go back.")
	  </script>
<%end if%>
<%else%>
	  <script language="javascript">
	  	  alert("You must enter email address if you want email reply.  Use browser back botton to go back.")
	  </script>
<%end if%>
<%else%>
	  <script language="javascript">
	  	  alert("You must enter news group if you want news reply. Use browser back botton to go back.")
	  </script>
<%end if%>
<%else%>
	  <script language="javascript">
	      alert("Search frequency must be number ! Use browser back button to go back.")
	  </script>
<%end if%>
</body>
</html>
