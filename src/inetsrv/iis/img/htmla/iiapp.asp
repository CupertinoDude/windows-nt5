<%@ LANGUAGE = VBScript %>
<% Option Explicit %>
<!-- #include file="directives.inc" -->

<% if Session("FONTSIZE") = "" then %>
	<!--#include file="iito.inc"-->
<% else %>
	<!--#include file="iiapp.str"-->

<%
dim bScrolling

If Session("hasDHTML") then
	bScrolling = "NO"
else
	bScrolling = "YES"
end if
%>

<HTML>

<FRAMESET ROWS="<%= L_IIAPPHEADFRM_H %>,*" BORDER=NO FRAMESPACING=1 FRAMEBORDER=0>
	<FRAME SRC="iiapphd.asp" NAME="title" BORDER=NO FRAMESPACING=0 FRAMEBORDER=0 MARGINHEIGHT=0 MARGINWIDTH=0 SCROLLING=NO>
	<FRAME SRC="iiappmn.asp" NAME="head" SCROLLING=<%= bScrolling %> FRAMESPACING=0 BORDER=NO  MARGINHEIGHT=5 MARGINWIDTH=5>
</FRAMESET>

</HTML>

<% end if %>