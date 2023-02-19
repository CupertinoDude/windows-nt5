<HTML>
<HEAD>
<TITLE>Enumerate through properties</TITLE>
</HEAD>
<BODY>
<%
   Dim MetaUtil
   Dim Properties
   Dim Property

   'Create the MetaUtil object
   Set MetaUtil = Server.CreateObject("MSWC.MetaUtil.1")

   'Count then enumberate with a base Key of ""
   Response.Write("<FONT SIZE=+1>Base key of """" </FONT><br>" + CHR(13) + CHR(10))
   Response.Write("<br>" + CHR(13) + CHR(10))

   Set Properties = MetaUtil.EnumProperties("")

   Response.Write("Count the Properties:<br>" + CHR(13) + CHR(10))
   Response.Write(Properties.Count)
   Response.Write("<br>" + CHR(13) + CHR(10))

   Response.Write("Enumerate the Properties:<br>" + CHR(13) + CHR(10))
   For Each Property In Properties
	Response.Write(Property.Id)
        Response.Write("<br>" + CHR(13) + CHR(10))
   Next

   Response.Write("<br>" + CHR(13) + CHR(10))

   'Count then enumberate with a base Key of "LM/W3SVC"
   Response.Write("<FONT SIZE=+1>Base key of ""LM/W3SVC"" </FONT><br>" + CHR(13) + CHR(10))
   Response.Write("<br>" + CHR(13) + CHR(10))

   Set Properties = MetaUtil.EnumProperties("LM/W3SVC")

   Response.Write("Count the Properties:<br>" + CHR(13) + CHR(10))
   Response.Write(Properties.Count)
   Response.Write("<br>" + CHR(13) + CHR(10))

   Response.Write("Enumerate the Properties:<br>" + CHR(13) + CHR(10))
   For Each Property In Properties
	Response.Write(Property.Id)
        Response.Write("<br>" + CHR(13) + CHR(10))
   Next

   Response.Write("<br>" + CHR(13) + CHR(10))

   Response.Write("Done<br>")

   'Clean up the reference to IIS.MetaUtil
   Session.Abandon
   
%>
</BODY>
</HTML>