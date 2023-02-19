;/*
; *                      Microsoft Confidential
; *                      Copyright (C) Microsoft Corporation 1991
; *                      All Rights Reserved.
; */

/***************************************************************************/
/*																									*/
/* MESSAGE.H                                						 */
/* 																								*/
/*	Include file for MS-DOS set version program.										*/
/* 																								*/
/*	johnhe	05-01-90																			*/
/***************************************************************************/

char *ErrorMsg[]=
{
	"\r\n����: ",
	"����ġ�� Ʋ���ϴ�.",
	"���� �̸��� Ʋ���ϴ�.",
	"�޸𸮰� �����մϴ�.",
	"���� ��ȣ�� Ʋ���ϴ�. 2.11���� 9.99 ������ �����̾�� �մϴ�.",
	"������ �׸��� ���� ���̺� �����ϴ�.",
	"SETVER.EXE ������ ã�� �� �����ϴ�.",
	"����̺� �����ڰ� Ʋ���ϴ�.",
	"����ٿ� �Ű� ������ �ʹ� �����ϴ�.",
	"�Ű� ������ �����ϴ�.",
	"SETVER.EXE ���� �д� ��",
	"���� ���̺��� �ջ�Ǿ����ϴ�.",
	"������ ��ο� �ִ� SETVER ������ ȣȯ�Ǵ� ������ �ƴմϴ�.",
	"���� ���̺� �� �׸��� �߰��� ������ �����ϴ�.",
	"SETVER.EXE ���� ��� ��"
	"SETVER.EXE�� ������ ��ΰ� Ʋ���ϴ�."
};

char *SuccessMsg 		= "\r\n���� ���̺��� ���� �� ���ƽ��ϴ�.";
char *SuccessMsg2		= "���� ������ ������ �ý����� �ٽ� ������ �Ŀ� ����˴ϴ�.";
char *szMiniHelp 		= "       ������ �ʿ��ϸ� \"SETVER /?\"�� ����Ͻʽÿ�.";
char *szTableEmpty	= "\r\n���� ���̺� �ƹ� �׸� �����ϴ�.";

char *Help[] =
{
        "MS-DOS�� ���α׷��� �����ϴ� ���� ��ȣ�� �����մϴ�.\r\n",
        "���� ���� ���̺� ǥ��:        SETVER [����̺�:���]",
        "�׸� �߰�:                    SETVER [����̺�:���] ���� �̸� n.nn",
        "�׸� ����:                    SETVER [����̺�:���] ���� �̸� /DELETE [/QUIET]\r\n",
        "  [����̺�:���]     SETVER.EXE ���� ��ġ�� �����մϴ�.",
        "  ���� �̸�           ���α׷� ���� �̸��� �����մϴ�.",
        "  n.nn                ���α׷��� ����� MS-DOS ������ �����մϴ�.",
        "  /DELETE �Ǵ� /D     ������ ���α׷��� ���� ���̺� �׸��� �����մϴ�.",
        "  /QUIET              ���� ���̺� �׸��� ������ �� ǥ�õǴ� �޽�����",
        "                    ����ϴ�.",
	NULL

};
char *Warn[] =
{
   "\n��� - ����ڰ� MS-DOS ���� ���̺� �߰��Ϸ��� ���� ���α׷���, ",
   "Microsoft�� �� ������ MS-DOS���� Ȯ������ ���� ���� ���� �ֽ��ϴ�.  ",
   "���� ���α׷��� �� ������ MS-DOS���� �ùٷ� �۵������� ���� ",
   "������ ����Ʈ���� �Ǹ� ��ü�� �����Ͻʽÿ�. ",
   "MS-DOS�� �Ͽ��� �ٸ� MS-DOS ���� ��ȣ�� �����ϵ��� �Ͽ� ",
   "�� ���� ���α׷��� �����ϸ�, �����͸� �Ұų� �ջ��ų ���� �ְ� ",
   "�ý����� �Ҿ����ϰ� ���� ���� �ֽ��ϴ�. �׷��� ��Ȳ������, ",
   "� ���ؿ� ���ؼ��� Microsoft�� å������ �ʽ��ϴ�.",
   NULL
};

char *szNoLoadMsg[] =						/* M001 */
{
	"",
	"����: SETVER ����̽��� �о�鿩���� �ʾҽ��ϴ�. SETVER ���� ���� ",
   "      ����Ϸ��� CONFIG.SYS���� SETVER.EXE ����̽��� �о�鿩�� �մϴ�.",
	NULL
};

#ifdef BILINGUAL
char *ErrorMsg2[]=
{
	"\r\nERROR: ",
	"Invalid switch.",
	"Invalid filename.",
	"Insuffient memory.",
	"Invalid version number, format must be 2.11 - 9.99.",
	"Specified entry was not found in the version table.",
	"Could not find the file SETVER.EXE.",
	"Invalid drive specifier.",
	"Too many command line parameters.",
	"Missing parameter.",
	"Reading SETVER.EXE file.",
	"Version table is corrupt.",
	"The SETVER file in the specified path is not a compatible version.",
	"There is no more space in version table new entries.",
	"Writing SETVER.EXE file."
	"An invalid path to SETVER.EXE was specified."
};

char *SuccessMsg_2 		= "\r\nVersion table successfully updated";
char *SuccessMsg2_2		= "The version change will take effect the next time you restart your system";
char *szMiniHelp2 		= "       Use \"SETVER /?\" for help";
char *szTableEmpty2	= "\r\nNo entries found in version table";

char *Help2[] =
{
        "Sets the version number that MS-DOS reports to a program.\r\n",
        "Display current version table:  SETVER [drive:path]",
        "Add entry:                      SETVER [drive:path] filename n.nn",
        "Delete entry:                   SETVER [drive:path] filename /DELETE [/QUIET]\r\n",
        "  [drive:path]    Specifies location of the SETVER.EXE file.",
        "  filename        Specifies the filename of the program.",
        "  n.nn            Specifies the MS-DOS version to be reported to the program.",
        "  /DELETE or /D   Deletes the version-table entry for the specified program.",
        "  /QUIET          Hides the message typically displayed during deletion of",
        "                  version-table entry.",
	NULL

};
char *Warn2[] =
{
										/* m100	*/
   "\nWARNING - The application you are adding to the MS-DOS version table ",
   "may not have been verified by Microsoft on this version of MS-DOS.  ",
   "Please contact your software vendor for information on whether this ",
   "application will operate properly under this version of MS-DOS.  ",
   "If you execute this application by instructing MS-DOS to report a ",
   "different MS-DOS version number, you may lose or corrupt data, or ",
   "cause system instabilities.  In that circumstance, Microsoft is not ",
   "reponsible for any loss or damage.",
   NULL
};

char *szNoLoadMsg2[] =						/* M001 */
{
	"",
	"NOTE: SETVER device not loaded. To activate SETVER version reporting",
   "      you must load the SETVER.EXE device in your CONFIG.SYS.",
	NULL
};
#endif
