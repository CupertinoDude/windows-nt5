<%@ LANGUAGE = VBScript %>
<% Option Explicit %>
<!-- #include file="directives.inc" -->


<% if Session("FONTSIZE") = "" then %>
	<!--#include file="iito.inc"-->
<% else %>
	<!--#include file="iifiltcm.str"-->
<HTML>
<HEAD>
	<TITLE></TITLE>
		<SCRIPT LANGUAGE="JavaScript">
		top.title.Global.siteProperties = false;
		function loadHead(){
			self.head.location.href = "iifiltmn.asp";		
		}
	</SCRIPT>
</HEAD>
	<FRAMESET ROWS="<%= iVScale(L_PAGEHEADHEIGHT_NUM) %>,<%= iVScale(L_LISTHEADHEIGHT_NUM) %>,*,<%= iVScale(L_TABLEBORDER_NUM) %>" BORDER=0 FRAMEBORDER=0 FRAMESPACING=0  onLoad="loadHead();">	
		<FRAME NAME="head" SRC="blank.htm" SCROLLING="no">
			<FRAMESET COLS="<%= iHScale(L_TABLEBORDER_NUM) %>,<%= iHScale(L_FILTTABLEWIDTH_NUM) %>,*" BORDER=0 FRAMESPACING=0 FRAMEBORDER=0>				
				<FRAME NAME="pad1" SRC="blank.htm" SCROLLING="no" MARGINHEIGHT=5 MARGINWIDTH=5 FRAMEBORDER=0>
				<FRAME NAME="colhds" SRC="iifilthd.asp" SCROLLING="no" MARGINHEIGHT=0 MARGINWIDTH=0 BORDER=0 FRAMEBORDER=0>
				<FRAME NAME="pad1" SRC="blank.htm" SCROLLING="no" MARGINHEIGHT=5 MARGINWIDTH=5 FRAMEBORDER=0>
			</FRAMESET>	
			<FRAMESET COLS="<%= iHScale(L_TABLEBORDER_NUM) %>,<%= iHScale(L_FILTTABLEWIDTH_NUM) %>,*" BORDER=0 FRAMESPACING=0 FRAMEBORDER=0>				
				<FRAME NAME="pad1" SRC="blank.htm" SCROLLING="no" MARGINHEIGHT=5 MARGINWIDTH=5 FRAMEBORDER=0>
				<FRAME NAME="list" SRC="blank.htm" SCROLLING="AUTO" MARGINHEIGHT=0 MARGINWIDTH=0 BORDER=1 FRAMEBORDER=1>
				<% if Session("IsAdmin") then %>
					<FRAME NAME="buttons" SRC="iilist.asp?extra=up&extra=down" SCROLLING="no" MARGINHEIGHT=5 MARGINWIDTH=5 FRAMEBORDER=0>
				<% else %>
					<FRAME NAME="pad1" SRC="blank.htm" SCROLLING="no" MARGINHEIGHT=5 MARGINWIDTH=5 FRAMEBORDER=0>
				<% end if %>
			</FRAMESET>	
		<FRAME NAME="pad3" SRC="blank.htm" SCROLLING="no" MARGINHEIGHT=5 MARGINWIDTH=5 FRAMEBORDER=0>	
	</FRAMESET>
</HTML>

<% end if %>