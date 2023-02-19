
/*************************************************
 *  abcw2.h                                      *
 *                                               *
 *  Copyright (C) 1995-1999 Microsoft Inc.       *
 *                                               *
 *************************************************/

extern CHAR SKLayoutS[NumsSK];
extern CHAR SKLayout[NumsSK][48*2]; 
extern CHAR PcKb[48*2],SdaKb[48*2];
extern CHAR SPcKb[48*2],SSdaKb[48*2];

extern LPIMEL lpImeL;
extern IMEG sImeG ;


#define WM_NEW_DEF_CODE         1993+0x400      //1993.4.19
#define WM_NEW_DEF_RESTORE      1993+0x401      //1993.4.19


HIMC ghIMC = 0;
LPINPUTCONTEXT glpIMC;
LPPRIVCONTEXT  glpIMCP; 
int wait_flag , waitzl_flag;                   //waitzl    6
int TypeOfOutMsg;
int biaodian_len;

BYTE I_U_Flag=0;        //produce the "i, u" input

            


struct INPUT_TISHI prompt[1];

struct INPT_BF in={40,0,{0}};
struct W_SLBL wp;
struct ATTR msx_area[120];
struct INDEX ndx;
struct INDEX kzk_ndx;
struct TBF FAR *cisu;
struct PD_TAB pindu;
struct FMT now={10,53,27};
struct T_REM tmmr;

int form[]  ={10,/*9*/8,5,/*4*/4,3,2,2,1,1,1};        //form[0] not used

BYTE spbx_tab[((87-15)*94+15)/16*16+16]={0};

BYTE logging_stack[0x400]={0};
WORD logging_stack_size=sizeof logging_stack;
                        

/*******************************************
 display result area DATA
**********************************************/
unsigned char space_char[]={0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20};
BYTE out_svw[400];
unsigned char group_counter[]="0.";
    
/*********************************************
 input area DATA
**********************************************/
int input_cur;
int new_no;
int jlxw_mode;
int jiyi_mode;

/******************************************************
 result area DATA
*******************************************************/
int result_area_pointer;
BYTE result_area[40];
BYTE out_result_area[40];
WORD out_bfb[40];
int out_pointer;
int now_cs;
int now_cs_dot;


/********************************************
 biaodian table
*********************************************/
//unsigned char biaodian_table[]="~`!@?():<>,.;_\x22\x27";                 //Note \0x22 will be 0 x 2 2
//unsigned char cc_biaodian[]="����������������������������������������";

unsigned char biaodian_table[]="@&$^\\!@?():<>,.;_\x22\x27";                 //Note \0x22 will be 0 x 2 2
unsigned char cc_biaodian[]="����������������������������������������������";


int biaodian_pos;
WORD biaodian_value;
BYTE yinhao_flag=0;

/*******************************************
 control variabe
********************************************/
//BYTE int_asc_mode=0;    // int or half character switch
extern BYTE step_mode;
//BYTE cp_ajust_flag;
BYTE bdd_flag;          // the function of punctuation switch
//BYTE cbx_flag =0;       // for written-stroke input flag
            // =0 NO writen-stroke input module
            // =1 the method with STD_MODE
            // =2 ............... SDA_MODE
BOOL IfTopMost;        // top window flag

BYTE word_back_flag;
BYTE msg_type;

/*****************************************
temp memory area
******************************************/
BYTE temp_rem_area[512];
BYTE rem_area[512];

/**************************************
display buffer parameter
***************************************/
BYTE out_length;
BYTE last_out_length;
BYTE cap_mode;


/******************************************
user_definition parameter
*******************************************/
WORD mulu_record_length=10;
WORD data_record_length=32;
WORD mulu_true_length=56;
WORD data_start=0x1000;
WORD mulu_max_length=0x1000;
BYTE user_word_max_length=32;


OFSTRUCT reopen;
HWND active_win_keep;
BYTE d_yinhao_flag=0,book_name=0,book_name_sub=0;  


/*******************************************************************
This part of defination is cut from the beginning of ABCW.c
********************************************************************/



BYTE SdaPromptOpen=0,DefNewNow=0;             //cock

FARPROC _hh1,_hh2;
HANDLE hInst;
HANDLE  cisu_hd;
HCURSOR hCursor;        //1993.2

FARPROC FAR *hh1,*hh2;

FARPROC lpFunc,lpFunc2;
HANDLE mdl;
BYTE opt_flag;

BYTE kb_buffer[35];
int OldCaps;

BYTE in_buffer[1];              /* buffer for input */

int n,end_flg,CharHi,CharWi;
HFONT hFont;
HFONT hOldFont;
HFONT hSFont=0;
HPEN hPen;

BYTE V_Flag=0;                  // V input flag

OFSTRUCT ofstruct;

HDC hDC;
HDC hMemoryDC;
HBITMAP cur_h;
int count2;

int pass_word=0;
HWND NowFocus,OptFocus;                    //keep the user's focus

BOOL cur_flag=0,op_flag;
WORD old_curx=CUR_START_X,cur_hibit=1,cur_start_ps=0,cur_start_count=0;

HWND hWnd,act_focus;

int input_count=0;
int kb_mode=CIN_STD,kb_flag = 0xffff;
int local_focus=0;
int timer_counter=0;
int msg_count=0;
BYTE msg_bf[50];
TimerCounter=2;

KeyBoardState=0;
SdaInst = 0;
HWND Return;
HWND act_win;

HANDLE Hdopt;                   //for dynamic call .dll proc
unsigned char jiyi[]="{����}";
WORD sda_trans[5]={9,9,9,9,0};

HWND hInputWnd;
HWND hABCWnd;

char ExeCmdLine[]="abcwin 0";
BYTE UpdateFlag=0;              //1993.3 increase the user.rem
char jiyi_wenjian_cuo[]="�ģ��û������ļ�������!";

BYTE InputBuffer[43];
WORD SoftKeyNum=0;

TCHAR    tmmr_rem[MAX_PATH];
TCHAR    user_lib[MAX_PATH];

HANDLE hAccTable;

HANDLE hImeL;
LPIMEL lpImeL;

int MoveFlag = 0;
POINT pot;
HWND hSetOp =0,NewWordWin=0;

char  *szMsgStr[] = { "0","0"};          // messages shown on diff sections

BYTE Sd_Open_flag=0;            //flag for display the keyboard
BYTE kb_mode_save=0;            //1993.4.19

/******************************************************************
This part of defination is cut from ABCWM.c
*******************************************************************/
char TMMR_OPEN_WRONG[]= "�����ļ��򿪴�";

/******************************************************************
This part of defination is cut before DIAMAN.c
*******************************************************************/
BYTE cpjy,bxsr,qj,bdzh;

/******************************************************************
This part of defination is cut before OKRETURN.c
*******************************************************************/
OFSTRUCT ofs;

struct SLBL sb;
struct N_SLBL neg;

BYTE slbl_tab[]="ZH00\1"
             "SH00\2"
             "CH00\3"
             "ING0\4"
             "AI00\5"
             "AN00\6"
             "ANG0\7"
     "AO00\x8"
     "EI00\x9"
     "EN00\xa"
     "ENG0\xb"
     "IA00\xc"
     "IAN0\xd"
     "IANG\xe"
     "IAO0\xf"
     "IE00\x10"
     "IN00\x11"
     "IU00\x12"
     "ONG0\x13"
     "OU00\x14"
     "UA00\x15"
     "UAI0\x16"
     "UAN0\x17"
     "UE00\x18"
     "UN00\x19"
     "UENG\x1a"                     //SC4K6c?*DBASE={<I5D1AH
     "UI00\x1b"
     "UO00\x1c"
     "UANG\x1d"
     "ER00\x1e"
     "IONG\x1f"
     "VE00\x18"
     "UEN0\x19"
     "VEN0\x19"
     "UEI0\x1b"
     "IOU0\x12";

OFSTRUCT openbuf;
OFSTRUCT openbuf_kzk,open_user,open_tmmr;


BYTE buffer[30];

BYTE cmp_head,cmp_state,cmp_bx,by_cchar_flag;
WORD cmp_yj,cmp_cisu;

// about search lib
LONG r_addr;
WORD out_svw_cnt,msx_area_cnt;
WORD search_start,search_end,kzk_search_start,kzk_search_end;
WORD item_length,kzk_item_length,last_item_name,item_addr,slib_addr;
BYTE word_lib_state;
WORD lib_w[0xa00];
WORD kzk_lib_w[0x400];
BYTE auto_mode,word_source,xs_flag,sfx_attr,jiyi_pindu,system_info;
BYTE stack1_move_counter;
WORD extb_ps;

/******************************************************************
This part of defination is cut before READ_A_PAGE.c
*******************************************************************/
char *std_dct="winabc.cwd";
char *user_dct="user.rem";
BYTE last_flag;
LONG last_start_ps;
WORD last_size;

/******************************************************************
This part of defination is cut before REM_PD1.c
*******************************************************************/
BYTE stack1_move_counter;



//................................................................
//               function tables definition
//
char fk_tab[]="0��1һ2��3��4��5��6��7��8��9��SʮB��Qǧ"
           "\0��\1Ҽ\2��\3��\4��\5��\6½\7��\x8��\x9��sʰb��qǪ"
           "G��" 
           "W��" 
           "Z��" 
           "N��" 
           "Y��" 
           "R��" 
           "J��" 
           "D��" 
           "$Ԫ" 
           "K��" 
           "M��" 
           "F��" 
           "L��" 
           "T��" 
           "E��" 
           "O��" 
           "Hʱ" 
           "P��" 
           "C��" 
           "U΢" 
           "A��" 
           "I��" 
           "X��" 
           "+��"                  
           "-��" 
           "*��" 
           "/��";    
          
          

WORD sfx_table[]={
                //��    DA-                 1 
    0x102, 0x80CC,              //0x80C4,
                // ��    DE-                  1
    0x202, 0x80E6,              //0x80DE,
                // ��    BA-                  1
    0x202, 0x2048,
                // ��    BAI-                1 
    0x102, 0x2056,
                // ��    BAN-                 1
    0x202, 0x205F,
                // ��    BAN-                1 
    0x102, 0x206A,
                // ��    BAO-                 1
    0x202, 0x2085,
                // ��    BEN-                1 
    0x102, 0x209B,
                // ��    BIAN-                1
    0x202, 0x20BC,
                // ��    BIAO-                1
    0x202, 0x20CA,
                // ��    BU-                 1 
    0x102, 0x8058,               //0x8054,
                // ��    BU-                  1
    0x202, 0x20FA,
                // ��    C,ANG-              1 
    0x102, 0x8088,               //0x8080,
                // ��    C,ANG-               1
    0x202, 0x808E,              //0x8086,
                // ��    C,AO-               1 
    0x102, 0x2145,
                // ��    C,E-                 1
    0x202, 0x8098,               //0x8090,
                // 3�    C,U-                 1
    0x202, 0x218F,
                // ��    C,U-                 1
    0x202, 0x219D,
                // ��    C,UN-               1 
    0x102, 0x21B6,
                // ��    CONG-                1
    0x202, 0x21CB,
                // ��    CUN-                 1
    0x202, 0x21DC,
                // ��    DAI-                 1
    0x202, 0x21EF,
                // ��    DAN-                1 
    0x102, 0x80D6,              //0x80CE,
                // ��    DANG-                1
    0x202, 0x2208,
                // ��    DAO-                 1
    0x202, 0x2212,
                // ��    DI-                 1 
    0x102, 0x2222,
                // ��    DI-                 1 
    0x102, 0x80F4,              //0x80EC,
                // ��    DI-                  1
    0x202, 0x80F8,              //0x80F0,
                // ��    DIAN-                1
    0x202, 0x2238,
                // ��    DIAN-               1 
    0x102, 0x223C,
                // ����  DIAN-ZI-            1 
    0x104, 0x223C, 0x2EA4,
                // ��    DIAO-                1
    0x202, 0x2249,
                // �,    DU-                  1
    0x202, 0x8116,               //0x810E,
                // ��    DUAN-               1 
    0x102, 0x227F,
                // ��    DUI-                 1
    0x202, 0x2286,
                // ��    DUO-                11
    0x302, 0x2293,
                // ��    E-                   1
    0x202, 0x22A1,
                // ��    FA-                  1
    0x202, 0x22B9,
                // ��    FANG-               1 
    0x102, 0x22D1,
                // ��    FEI-                1 
    0x102, 0x22D8,
                // ��    FEI-                 1
    0x202, 0x22E2,
                // ��    FEN-                 1
    0x202, 0x22EC,
                // ��    FENG-                1
    0x202, 0x22F8,
                // ��    FU-                 1 
    0x102, 0x2320,
                // ��    GAI-                1 
    0x102, 0x2332,
                // ��    GAN-                 1
    0x202, 0x233F,
                // ��    GAO-                1 
    0x102, 0x234E,
                // ��    GE-                 1 
    0x102, 0x2366,
                // ��    GENG-               1 
    0x102, 0x236B,
                // ��    GONG-                1
    0x202, 0x2371,
                // ��    GONG-               1 
    0x102, 0x237F,
                // ��    GU-                 1 
    0x102, 0x2392,
                // ��    GUAN-                1
    0x202, 0x23A8,
                // ��    GUO-                 1
    0x202, 0x23C7,
                // ��    ,AO-                1 
    0x102, 0x23EE,
                // ��    ,EI-                1 
    0x102, 0x2405,
                // ��    ,EN-                1 
    0x102, 0x2407,
                // ��    ,ONG-               1 
    0x102, 0x819E,              //0x8194,
                // ��    ,OU-                 1
    0x202, 0x241E,
                // ;�    ,U-                  1
    0x202, 0x2430,
                // ��    ,UA-                 1
    0x202, 0x2438,
                // ��    ,UAI-               1 
    0x102, 0x243E,
                // ��    ,UI-                 1
    0x202, 0x81B0,              //0x81A6,
                // ��    ,UO-                 1
    0x202, 0x247E,
                // ��    JI-                  1
    0x202, 0x2483,
                // ��    JIA-                 1
    0x202, 0x81CC,              //0x81C2,
                // ��    JIAN-                1
    0x202, 0x24E5,
                // ��    JIANG-               1
    0x202, 0x24F6,
                // ��    JIE-                 1
    0x202, 0x252C,
                // ��    JIN-                 1
    0x202, 0x2535,
                // ��    JIU-                1 
    0x102, 0x256C,
                // ��    JU-                  1
    0x202, 0x2579,
                // ��    JUN-                 1
    0x202, 0x259F,
                // ��    KE-                  1
    0x202, 0x25C4,
                // ��    KE-                  1
    0x202, 0x25C6,
                // ��    KE-                  1
    0x202, 0x25C7,
                // ��    KE-                 1 
    0x102, 0x25CA,
                // ��    KE-                  1
    0x202, 0x25CF,
                // ��    KOU-                 1
    0x202, 0x25DB,
                // ��    LAO-                1 
    0x102, 0x262E,
                // ��    LE-                  1
    0x202, 0x827A,              //0x826C,
                // ��    LEI-                 1
    0x202, 0x263F,
                // ��    LI-                  1
    0x202, 0x264E,
                // ��    LI-                  1
    0x202, 0x2663,
                // ����  LI-LUN-              1
    0x204, 0x264C, 0x26F6,
                // ��    LIANG-               1
    0x202, 0x267C,
                // ��    LIN-                 1
    0x202, 0x2693,
                // ��    LUN-                 1
    0x202, 0x26F6,
                // ��    LV-                  1
    0x202, 0x828E,              //0x8280,
                // ÿ    MEI-                1 
    0x102, 0x2738,
                // ��    MEN-                 1
    0x202, 0x2740,
                // ��    MI-                  1
    0x202, 0x274D,
                // ��    MIAN-               1 
    0x102, 0x2757,
                // ��    MIAN-                1
    0x202, 0x275F,
                // ĳ    MOU-                1 
    0x102, 0x278A,
                // ��    NAN-                1 
    0x102, 0x27A7,
                // ��    NAO-                1 
    0x102, 0x27AD,
                // ��    NEI-                11
    0x302, 0x27B1,
                // ��    NIAN-                1
    0x202, 0x27C1,
                // Ů    NV-                 1 
    0x102, 0x27E3,
                // ��    PAI-                 1
    0x202, 0x8320,               //0x8312,
                // ��    PANG-                1
    0x202, 0x2809,
                // Ƥ    PI-                  1
    0x202, 0x2837,
                // Ƭ    PIAN-                1
    0x202, 0x283F,
                // Ʒ    PIN-                 1
    0x202, 0x284A,
                // ,�    QAUN-               1 
    0x102, 0x28FA,
                // ��    QI-                 1 
    0x102, 0x8354,               //0x8346,
                // ��    QI-                  1
    0x202, 0x288A,
                // ǰ    QIAN-                1
    0x202, 0x28A1,
                // ��    QU-                  1
    0x202, 0x8388,               //0x837A,
                // ,�    QUAN-                1
    0x202, 0x28F7,
                // ,�    QUN-                 1
    0x202, 0x2909,
                // ,�    REN-                 1
    0x202, 0x291A,
                // ,�    RI-                  1
    0x202, 0x2924,
                // ��    S,ANG-              11
    0x302, 0x297C,
                // ��    S,E-                 1
    0x202, 0x2994,
                // ��    S,ENG-               1
    0x202, 0x29A6,
                // ʡ    S,ENG-               1
    0x202, 0x83BE,              //0x83B0,
                // ��    S,I-                 1
    0x202, 0x83E2,              //0x83D4,
                // ��    S,I-                 1
    0x202, 0x29DD,
                // ��    S,OU-                1
    0x202, 0x29E1,
                // ��    S,OU-               1 
    0x102, 0x29E7,
                // ��    S,U-                 1
    0x202, 0x29F4,
                // ˫    S,UANG-             1 
    0x102, 0x2A14,
                // ˮ    S,UI-                1
    0x202, 0x2A17,
                // ˾    SI-                  1
    0x202, 0x2A27,
                // ˼��  SI-XIANG-            1
    0x204, 0x8404, 0x2BCC,  //0x83F8, 0x2BCC,
                // ��    SUO-                 1
    0x202, 0x2A62,
                // ̨    TAI-                 1
    0x202, 0x2A6F,
                // ��    TI-                  1
    0x202, 0x2AAC,
                // ��    TIAO-                1
    0x202, 0x2ABC,
                // ��    TIE-                1 
    0x102, 0x2AC1,
                // ͷ    TOU-                 1
    0x202, 0x2ADC,
                // ��    WAI-                11
    0x302, 0x2B07,
                // Ϊ    WEI-                 1
    0x202, 0x2B2D,
                // ��    WU-                 1 
    0x102, 0x8450,               //0x8444,
                // ��    WU-                  1
    0x202, 0x2B72,
                // ϵ    XI-                  1
    0x202, 0x8458,               //0x844C,
                // ��    XIA-                11
    0x302, 0x2BA3,
                // ��    XIAN-                1
    0x202, 0x2BB9,
                // ��    XIANG-               1
    0x202, 0x2BC8,
                // ��Ŀ  XIANG-MU-            1
    0x204, 0x2BCF, 0x2796,
                // С    XIAO-               1 
    0x102, 0x2BE0,
                // ��    XIN-                1 
    0x102, 0x2C01,
                // ��    XIN-                 1
    0x202, 0x2C03,
                // ��    XING-                1
    0x202, 0x2C13,

                // ��    XING-                1
    0x202, 0x2C0C,
                // ����  XING-Z,I-            1
    0x204, 0x2C13, 0x2E3D,
                // ѧ    XUE-                 1
    0x202, 0x2C44,
                // ѧ˵  XUE-S,UO-            1
    0x204, 0x2C44, 0x83FE,   //0x83F2,
                // ��    YAN-                 1
    0x202, 0x2C78,
                // ��    YANG-               1 
    0x102, 0x2C90,
                // ҵ    YE-                  1
    0x202, 0x2CB0,
                // ��    YI-                  1
    0x202, 0x2CC2,
                // ��    YOU-                1 
    0x102, 0x2D29,
                // ��    YU-                  1
    0x202, 0x2D33,
                // ԭ    YUAN-               1 
    0x102, 0x2D64,
                // Ա    YUAN-                1
    0x202, 0x84C0,              //0x84B4,
                // Ժ    YUAN-                1
    0x202, 0x2D71,
                // �˶�  YUN-DONG-            1
    0x204, 0x2D82, 0x2262,
                // ��    ZI-                  1
    0x202, 0x2EA4,

                // ��    ZAI-                1 
    0x102, 0x2D90,
                // ��    ZAI-                1 
    0x102, 0x2D91,
                // վ    Z,AN-                1
    0x202, 0x2DD3,
                // ��    Z,ANG-               1
    0x202, 0x808A,              //0x8082,
                // ��    Z,E-                 1
    0x202, 0x2DF4,
                // ��    Z,E-                 1
    0x202, 0x8532,               //0x8526,
                // ��    Z,EN-               1 
    0x102, 0x2DFB,
                // ��    Z,EN-                1
    0x202, 0x2E07,
                // ��    Z,ENG-              1 
    0x102, 0x2E12,
                // ��    Z,I-                 1
    0x202, 0x2E39,
                // ��    Z,ONG-              11
    0x302, 0x2E43,
                // ��    Z,U-                1 
    0x102, 0x2E6A,
                // ס    Z,U-                 1
    0x202, 0x2E72,
                // ����  Z,U-YI-              1
    0x204, 0x2E6A, 0x2CE0,
                // רҵ  Z,UAN-YE-            1
    0x204, 0x2E79, 0x2CB0,
                // ״    Z,UANG-              1
    0x202, 0x2E85,
                // ׼    Z,UN-               1 
    0x102, 0x2E8D,
                // ��    ZONG-               1 
    0x102, 0x2EAD,
                // ��    ZU-                  1
    0x202, 0x2EBA,
                // ��    ZUI-                1 
    0x102, 0x2EBF,
                // ��    ZUO-                1 
    0x102, 0x2EC8};

WORD sfx_table_size=sizeof sfx_table;

int FAR PASCAL sda_proc(WORD, LPWORD, BYTE, HIMC);
int FAR PASCAL tran_data(int, HIMC, BYTE);

