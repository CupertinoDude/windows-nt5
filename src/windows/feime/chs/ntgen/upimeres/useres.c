#include <windows.h>            // required for all Windows applications   
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <winnls.h>
#include "upimeres.h"
void main()
{
//	if(ImeUpdateRes("C:\\WINPY.IME", "", "", "�ҵ�ǿ������뷨 �汾1.0000", "��Ц÷", 0x0f))
	if(ImeUpdateRes("C:\\WINPY.dll", "C:\\NEW.BMP", "C:\\NEW.ICO", "�ҵ�ǿ������뷨 �汾1.0000", "��Ц÷", 0x0f))
//	if(ImeUpdateRes("C:\\WINPPY.IME", "C:\\NEW.BMP", "C:\\NEW.ICO", "���뷨ģ�� �汾2.0", "Microsoft Corporation", 0))
		exit(0);
	else
		exit(-1);


}
