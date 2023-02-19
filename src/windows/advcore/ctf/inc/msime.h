/*----------------------------------------------------------------------------
    %%File: MSIME.H

    Copyright (c) 1995-1998  Microsoft Corporation
    Version 1.0

    Japanese specific definitions of IFECommon, IFELanguage, IFEDictionary,
    and Per IME Interfaces.
----------------------------------------------------------------------------*/

#ifndef __MSIME_H__
#define __MSIME_H__

#include <ole2.h>
#include <objbase.h>
#include <imm.h>

#pragma pack(1)         /* Assume byte packing throughout */

///////////////////////////
// HKEY_CLASSES_ROOT values
///////////////////////////

#define szImeJapan98        "MSIME.Japan.6"
#define szImeJapan          "MSIME.Japan"


////////////////
// CLSID and IID
////////////////

// Class ID for FE COM interfaces
// {019F7150-E6DB-11d0-83C3-00C04FDDB82E}
DEFINE_GUID(CLSID_MSIME_JAPANESE,
0x19f7150, 0xe6db, 0x11d0, 0x83, 0xc3, 0x0, 0xc0, 0x4f, 0xdd, 0xb8, 0x2e);

// Interface ID for IFECommon
// {019F7151-E6DB-11d0-83C3-00C04FDDB82E}
DEFINE_GUID(IID_IFECommon,
0x19f7151, 0xe6db, 0x11d0, 0x83, 0xc3, 0x0, 0xc0, 0x4f, 0xdd, 0xb8, 0x2e);

// Interface ID for IFELanguage
// {019F7152-E6DB-11d0-83C3-00C04FDDB82E}
DEFINE_GUID(IID_IFELanguage,
0x19f7152, 0xe6db, 0x11d0, 0x83, 0xc3, 0x0, 0xc0, 0x4f, 0xdd, 0xb8, 0x2e);

// Interface ID for IFELanguage2
// {21164102-C24A-11d1-851A-00C04FCC6B14}
DEFINE_GUID(IID_IFELanguage2,
0x21164102, 0xc24a, 0x11d1, 0x85, 0x1a, 0x0, 0xc0, 0x4f, 0xcc, 0x6b, 0x14);

// Interface ID for IFEDictionary
// {019F7153-E6DB-11d0-83C3-00C04FDDB82E}
DEFINE_GUID(IID_IFEDictionary,
0x19f7153, 0xe6db, 0x11d0, 0x83, 0xc3, 0x0, 0xc0, 0x4f, 0xdd, 0xb8, 0x2e);


////////////////////////////
// Definitions for IFECommon
////////////////////////////

#undef  INTERFACE
#define INTERFACE   IFEClassFactory

////////////////////////////////
// The IFEClassFactory Interface
////////////////////////////////

DECLARE_INTERFACE_(IFEClassFactory, IClassFactory)
{
    // IUnknown members
    STDMETHOD(QueryInterface)   (THIS_ REFIID refiid, VOID **ppv) PURE;
    STDMETHOD_(ULONG,AddRef)    (THIS) PURE;
    STDMETHOD_(ULONG,Release)   (THIS) PURE;

    // IFEClassFactory members
    STDMETHOD(CreateInstance)   (THIS_ LPUNKNOWN, REFIID, void **) PURE;
    STDMETHOD(LockServer)       (THIS_ BOOL) PURE;
};


#undef  INTERFACE
#define INTERFACE   IFECommon

//////////////////////////
// The IFECommon Interface
//////////////////////////

#define IFEC_S_ALREADY_DEFAULT          MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_ITF, 0x7400)

typedef struct _IMEDLG
{
    int         cbIMEDLG;               //size of this structure
    HWND        hwnd;                   //parent window handle
    LPWSTR      lpwstrWord;             //optional string
    int         nTabId;                 //specifies a tab in dialog
} IMEDLG;

DECLARE_INTERFACE_(IFECommon, IUnknown)
{
    // IUnknown members
    STDMETHOD(QueryInterface)   (THIS_ REFIID refiid, VOID **ppv) PURE;
    STDMETHOD_(ULONG,AddRef)    (THIS) PURE;
    STDMETHOD_(ULONG,Release)   (THIS) PURE;

    // IFECommon members
    STDMETHOD(IsDefaultIME) (THIS_
                            CHAR *szName,               //(out) name of MS-IME
                            INT cszName                 //(in) size of szName
                            ) PURE;
    STDMETHOD(SetDefaultIME)    (THIS) PURE;
    STDMETHOD(InvokeWordRegDialog) (THIS_
                            IMEDLG *pimedlg             //(in) parameters
                            ) PURE;
    STDMETHOD(InvokeDictToolDialog) (THIS_
                            IMEDLG *pimedlg             //(in) parameters
                            ) PURE;
};


///////////////////////////
// The IFELanguage interface
///////////////////////////

// Word Descriptor
typedef struct tagWDD{
    WORD        wDispPos;   // Offset of Output string
    union {
        WORD    wReadPos;   // Offset of Reading string
        WORD    wCompPos;
    };

    WORD        cchDisp;    //number of ptchDisp
    union {
        WORD    cchRead;    //number of ptchRead
        WORD    cchComp;
    };

    DWORD       nReserve;   //reserved

    WORD        nPos;       //part of speech

                            // implementation-defined
    WORD        fPhrase : 1;//start of phrase
    WORD        fAutoCorrect: 1;//auto-corrected
    WORD        fNumericPrefix: 1;//kansu-shi expansion(JPN)
    WORD        fUserRegistered: 1;//from user dictionary
    WORD        fUnknown: 1;//unknown word (duplicated information with nPos.)
    WORD        fRecentUsed: 1; //used recently flag
    WORD        :10;        //

    VOID        *pReserved; //points directly to WORDITEM
} WDD;

#pragma warning(disable:4200) // zero-size array in structure
typedef struct tagMORRSLT {
    DWORD       dwSize;             // total size of this block.
    WCHAR       *pwchOutput;        // conversion result string.
    WORD        cchOutput;          // lengh of result string.
    union {
        WCHAR   *pwchRead;          // reading string
        WCHAR   *pwchComp;
    };
    union {
        WORD    cchRead;            // length of reading string.
        WORD    cchComp;
    };
    WORD        *pchInputPos;       // index array of reading to input character.
    WORD        *pchOutputIdxWDD;   // index array of output character to WDD
    union {
        WORD    *pchReadIdxWDD;     // index array of reading character to WDD
        WORD    *pchCompIdxWDD;
    };
    WORD        *paMonoRubyPos;     // array of position of monoruby
    WDD         *pWDD;              // pointer to array of WDD
    INT         cWDD;               // number of WDD
    VOID        *pPrivate;          // pointer of private data area
    WCHAR       BLKBuff[];          // area for stored above members.
                                    //  WCHAR   wchOutput[cchOutput];
                                    //  WCHAR   wchRead[cchRead];
                                    //  CHAR    chInputIdx[cwchInput];
                                    //  CHAR    chOutputIdx[cchOutput];
                                    //  CHAR    chReadIndx[cchRead];
                                    //  ????    Private
                                    //  WDD     WDDBlk[cWDD];
}MORRSLT;
#pragma warning(default:4200) // zero-size array in structure

// request for conversion (dwRequest)
#define FELANG_REQ_CONV         0x00010000
#define FELANG_REQ_RECONV       0x00020000
#define FELANG_REQ_REV          0x00030000


// Conversion mode (dwCMode)
#define FELANG_CMODE_MONORUBY       0x00000002  //mono-ruby
#define FELANG_CMODE_NOPRUNING      0x00000004  //no pruning
#define FELANG_CMODE_KATAKANAOUT    0x00000008  //katakana output
#define FELANG_CMODE_HIRAGANAOUT    0x00000000  //default output
#define FELANG_CMODE_HALFWIDTHOUT   0x00000010  //half-width output
#define FELANG_CMODE_FULLWIDTHOUT   0x00000020  //full-width output
#define FELANG_CMODE_BOPOMOFO       0x00000040  //
#define FELANG_CMODE_HANGUL         0x00000080  //
#define FELANG_CMODE_PINYIN         0x00000100  //
#define FELANG_CMODE_PRECONV        0x00000200  //do conversion as follows:
                                                // - roma-ji to kana
                                                // - autocorrect before conversion
                                                // - periods, comma, and brackets
#define FELANG_CMODE_RADICAL        0x00000400  //
#define FELANG_CMODE_UNKNOWNREADING 0x00000800  //
#define FELANG_CMODE_MERGECAND      0x00001000  // merge display with same candidate
#define FELANG_CMODE_ROMAN          0x00002000  //
#define FELANG_CMODE_BESTFIRST      0x00004000  // only make 1st best
#define FELANG_CMODE_USENOREVWORDS  0x00008000  // use invalid revword on REV/RECONV.

#define FELANG_CMODE_NONE           0x01000000  // IME_SMODE_NONE
#define FELANG_CMODE_PLAURALCLAUSE  0x02000000  // IME_SMODE_PLAURALCLAUSE
#define FELANG_CMODE_SINGLECONVERT  0x04000000  // IME_SMODE_SINGLECONVERT
#define FELANG_CMODE_AUTOMATIC      0x08000000  // IME_SMODE_AUTOMATIC
#define FELANG_CMODE_PHRASEPREDICT  0x10000000  // IME_SMODE_PHRASEPREDICT
#define FELANG_CMODE_CONVERSATION   0x20000000  // IME_SMODE_CONVERSATION
#define FELANG_CMODE_NAME           FELANG_CMODE_PHRASEPREDICT  // Name mode (MSKKIME)
#define FELANG_CMODE_NOINVISIBLECHAR 0x40000000 // remove invisible chars (e.g. tone mark)


// Error message
#define E_NOCAND            0x30    //not enough candidates
#define E_NOTENOUGH_BUFFER  0x31    //out of string buffer
#define E_NOTENOUGH_WDD     0x32    //out of WDD buffer
#define E_LARGEINPUT        0x33    //large input string


//Morphology Info
#define FELANG_CLMN_WBREAK      0x01
#define FELANG_CLMN_NOWBREAK    0x02
#define FELANG_CLMN_PBREAK      0x04
#define FELANG_CLMN_NOPBREAK    0x08
#define FELANG_CLMN_FIXR        0x10
#define FELANG_CLMN_FIXD        0x20    // fix display of word

#define FELANG_INVALD_PO        0xFFFF  // unmatched position for input string

#undef INTERFACE
#define INTERFACE       IFELanguage

//IFELanguage template
DECLARE_INTERFACE_(IFELanguage,IUnknown)
{
    // IUnknown members
    STDMETHOD(QueryInterface)(THIS_ REFIID refiid, VOID **ppv) PURE;
    STDMETHOD_(ULONG,AddRef)(THIS) PURE;
    STDMETHOD_(ULONG,Release)(THIS) PURE;

    // Ijconv members.  must be virtual functions
    STDMETHOD(Open)(THIS) PURE;
    STDMETHOD(Close)(THIS) PURE;

    STDMETHOD(GetJMorphResult)(THIS_
                        DWORD   dwRequest,          // [in]
                        DWORD   dwCMode,            // [in]
                        INT     cwchInput,          // [in]
                        WCHAR   *pwchInput,         // [in, size_is(cwchInput)]
                        DWORD   *pfCInfo,           // [in, size_is(cwchInput)]
                        MORRSLT **ppResult ) PURE;  // [out]

    STDMETHOD(GetConversionModeCaps)(THIS_ DWORD *pdwCaps) PURE;

    STDMETHOD(GetPhonetic)(THIS_
                        BSTR    string,             // [in]
                        LONG    start,              // [in]
                        LONG    length,             // [in]
                        BSTR *  phonetic ) PURE;    // [out, retval]

    STDMETHOD(GetConversion)(THIS_
                        BSTR    string,             // [in]
                        LONG    start,              // [in]
                        LONG    length,             // [in]
                        BSTR *  result ) PURE;      // [out, retval]
};


////////////////////////////////
// Definitions for IFEDictionary
////////////////////////////////

// Part Of Speach
#define IFED_POS_NONE                   0x00000000
#define IFED_POS_NOUN                   0x00000001
#define IFED_POS_VERB                   0x00000002
#define IFED_POS_ADJECTIVE              0x00000004
#define IFED_POS_ADJECTIVE_VERB         0x00000008
#define IFED_POS_ADVERB                 0x00000010
#define IFED_POS_ADNOUN                 0x00000020
#define IFED_POS_CONJUNCTION            0x00000040
#define IFED_POS_INTERJECTION           0x00000080
#define IFED_POS_INDEPENDENT            0x000000ff
#define IFED_POS_INFLECTIONALSUFFIX     0x00000100
#define IFED_POS_PREFIX                 0x00000200
#define IFED_POS_SUFFIX                 0x00000400
#define IFED_POS_AFFIX                  0x00000600
#define IFED_POS_TANKANJI               0x00000800
#define IFED_POS_IDIOMS                 0x00001000
#define IFED_POS_SYMBOLS                0x00002000
#define IFED_POS_PARTICLE               0x00004000
#define IFED_POS_AUXILIARY_VERB         0x00008000
#define IFED_POS_SUB_VERB               0x00010000
#define IFED_POS_DEPENDENT              0x0001c000
#define IFED_POS_ALL                    0x0001ffff

// GetWord Selection Type
#define IFED_SELECT_NONE                0x00000000
#define IFED_SELECT_READING             0x00000001
#define IFED_SELECT_DISPLAY             0x00000002
#define IFED_SELECT_POS                 0x00000004
#define IFED_SELECT_COMMENT             0x00000008
#define IFED_SELECT_ALL                 0x0000000f

// Registered Word Type
#define IFED_REG_NONE                   0x00000000
#define IFED_REG_USER                   0x00000001
#define IFED_REG_AUTO                   0x00000002
#define IFED_REG_GRAMMAR                0x00000004
#define IFED_REG_ALL                    0x00000007

// Dictionary Type
#define IFED_TYPE_NONE                  0x00000000
#define IFED_TYPE_GENERAL               0x00000001
#define IFED_TYPE_NAMEPLACE             0x00000002
#define IFED_TYPE_SPEECH                0x00000004
#define IFED_TYPE_REVERSE               0x00000008
#define IFED_TYPE_ENGLISH               0x00000010
#define IFED_TYPE_ALL                   0x0000001f

// HRESULTS for IFEDictionary interface

//no more entries in the dictionary
#define IFED_S_MORE_ENTRIES             MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_ITF, 0x7200)
//dictionary is empty, no header information is returned
#define IFED_S_EMPTY_DICTIONARY         MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_ITF, 0x7201)
//word already exists in dictionary
#define IFED_S_WORD_EXISTS              MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_ITF, 0x7202)

//dictionary is not found
#define IFED_E_NOT_FOUND                MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x7300)
//invalid dictionary format
#define IFED_E_INVALID_FORMAT           MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x7301)
//failed to open file
#define IFED_E_OPEN_FAILED              MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x7302)
//failed to write to file
#define IFED_E_WRITE_FAILED             MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x7303)
//no entry found in dictionary
#define IFED_E_NO_ENTRY                 MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x7304)
//this routines doesn't support the current dictionary
#define IFED_E_REGISTER_FAILED          MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x7305)
//not a user dictionary
#define IFED_E_NOT_USER_DIC             MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x7306)
//not supported
#define IFED_E_NOT_SUPPORTED            MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x7307)
//failed to insert user comment
#define IFED_E_USER_COMMENT             MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x7308)

#define cbCommentMax            256

//Private Unicode Character
#define wchPrivate1             0xE000

// Where to place registring word
typedef enum
{
    IFED_REG_HEAD,
    IFED_REG_TAIL,
    IFED_REG_DEL,
} IMEREG;


// Type of IME dictionary
typedef enum
{
    IFED_UNKNOWN,
    IFED_MSIME2_BIN_SYSTEM,
    IFED_MSIME2_BIN_USER,
    IFED_MSIME2_TEXT_USER,
    IFED_MSIME95_BIN_SYSTEM,
    IFED_MSIME95_BIN_USER,
    IFED_MSIME95_TEXT_USER,
    IFED_MSIME97_BIN_SYSTEM,
    IFED_MSIME97_BIN_USER,
    IFED_MSIME97_TEXT_USER,
    IFED_MSIME_BIN_SYSTEM,
    IFED_MSIME_BIN_USER,
    IFED_MSIME_TEXT_USER,
    IFED_ACTIVE_DICT,
    IFED_ATOK9,
    IFED_ATOK10,
    IFED_NEC_AI_,
    IFED_WX_II,
    IFED_WX_III,
    IFED_VJE_20,
} IMEFMT;

// Type of User Comment
typedef enum
{
    IFED_UCT_NONE,
    IFED_UCT_STRING_SJIS,
    IFED_UCT_STRING_UNICODE,
    IFED_UCT_USER_DEFINED,
    IFED_UCT_MAX,
} IMEUCT;


#if _MSC_VER >= 1200
#pragma warning(push)
#pragma warning(disable:4201)
#endif

// WoRD found in a dictionary
typedef struct _IMEWRD
{
    WCHAR       *pwchReading;
    WCHAR       *pwchDisplay;
    union {
        ULONG ulPos;
        struct {
            WORD        nPos1;      //hinshi
            WORD        nPos2;      //extended hinshi
        } ;
    };
    ULONG       rgulAttrs[2];       //attributes
    INT         cbComment;          //size of user comment
    IMEUCT      uct;                //type of user comment
    VOID        *pvComment;         //user comment
} IMEWRD, *PIMEWRD;

#if _MSC_VER >= 1200
#pragma warning(pop)
#endif

// Shared Header dictionary File
typedef struct _IMESHF
{
    WORD        cbShf;              //size of this struct
    WORD        verDic;             //dictionary version
    CHAR        szTitle[48];        //dictionary title
    CHAR        szDescription[256]; //dictionary description
    CHAR        szCopyright[128];   //dictionary copyright info
} IMESHF;


#define POS_UNDEFINED      0
#define JPOS_UNDEFINED      POS_UNDEFINED
#define JPOS_MEISHI_FUTSU       100     //����
#define JPOS_MEISHI_SAHEN       101     //���ϖ���
#define JPOS_MEISHI_ZAHEN       102     //���ϖ���
#define JPOS_MEISHI_KEIYOUDOUSHI        103     //�`������
#define JPOS_HUKUSIMEISHI       104     //�����I����
#define JPOS_MEISA_KEIDOU       105     //���ό`��
#define JPOS_JINMEI     106     //�l��
#define JPOS_JINMEI_SEI     107     //��
#define JPOS_JINMEI_MEI     108     //��
#define JPOS_CHIMEI     109     //�n��
#define JPOS_CHIMEI_KUNI        110     //��
#define JPOS_CHIMEI_KEN     111     //��
#define JPOS_CHIMEI_GUN     112     //�S
#define JPOS_CHIMEI_KU      113     //��
#define JPOS_CHIMEI_SHI     114     //�s
#define JPOS_CHIMEI_MACHI       115     //��
#define JPOS_CHIMEI_MURA        116     //��
#define JPOS_CHIMEI_EKI     117     //�w
#define JPOS_SONOTA     118     //�ŗL����
#define JPOS_SHAMEI     119     //�Ж�
#define JPOS_SOSHIKI        120     //�g�D
#define JPOS_KENCHIKU       121     //���z��
#define JPOS_BUPPIN     122     //���i
#define JPOS_DAIMEISHI      123     //�㖼��
#define JPOS_DAIMEISHI_NINSHOU      124     //�l�̑㖼��
#define JPOS_DAIMEISHI_SHIJI        125     //�w���㖼��
#define JPOS_KAZU       126     //��
#define JPOS_KAZU_SURYOU        127     //����
#define JPOS_KAZU_SUSHI     128     //����
#define JPOS_5DAN_AWA       200     //����s
#define JPOS_5DAN_KA        201     //���s
#define JPOS_5DAN_GA        202     //���s
#define JPOS_5DAN_SA        203     //���s
#define JPOS_5DAN_TA        204     //���s
#define JPOS_5DAN_NA        205     //�ȍs
#define JPOS_5DAN_BA        206     //�΍s
#define JPOS_5DAN_MA        207     //�܍s
#define JPOS_5DAN_RA        208     //��s
#define JPOS_5DAN_AWAUON        209     //����s������
#define JPOS_5DAN_KASOKUON      210     //���s������
#define JPOS_5DAN_RAHEN     211     //��s�ϊi
#define JPOS_4DAN_HA        212     //�͍s�l�i
#define JPOS_1DAN       213     //��i����
#define JPOS_TOKUSHU_KAHEN      214     //���ϓ���
#define JPOS_TOKUSHU_SAHENSURU      215     //���ϓ���
#define JPOS_TOKUSHU_SAHEN      216     //���s�ϊi
#define JPOS_TOKUSHU_ZAHEN      217     //���s�ϊi
#define JPOS_TOKUSHU_NAHEN      218     //�ȍs�ϊi
#define JPOS_KURU_KI        219     //��
#define JPOS_KURU_KITA      220     //����
#define JPOS_KURU_KITARA        221     //������
#define JPOS_KURU_KITARI        222     //������
#define JPOS_KURU_KITAROU       223     //�����낤
#define JPOS_KURU_KITE      224     //����
#define JPOS_KURU_KUREBA        225     //�����
#define JPOS_KURU_KO        226     //���i�Ȃ��j
#define JPOS_KURU_KOI       227     //����
#define JPOS_KURU_KOYOU     228     //���悤
#define JPOS_SURU_SA        229     //��
#define JPOS_SURU_SI        230     //��
#define JPOS_SURU_SITA      231     //����
#define JPOS_SURU_SITARA        232     //������
#define JPOS_SURU_SIATRI        233     //������
#define JPOS_SURU_SITAROU       234     //�����낤
#define JPOS_SURU_SITE      235     //����
#define JPOS_SURU_SIYOU     236     //���悤
#define JPOS_SURU_SUREBA        237     //�����
#define JPOS_SURU_SE        238     //��
#define JPOS_SURU_SEYO      239     //����^����
#define JPOS_KEIYOU     300     //�`�e��
#define JPOS_KEIYOU_GARU        301     //�`�e�����
#define JPOS_KEIYOU_GE      302     //�`�e����
#define JPOS_KEIYOU_ME      303     //�`�e���
#define JPOS_KEIYOU_YUU     304     //�`�e����
#define JPOS_KEIYOU_U       305     //�`�e���
#define JPOS_KEIDOU     400     //�`�e����
#define JPOS_KEIDOU_NO      401     //�`�e�����
#define JPOS_KEIDOU_TARU        402     //�`�e������
#define JPOS_KEIDOU_GARU        403     //�`�e�������
#define JPOS_FUKUSHI        500     //����
#define JPOS_FUKUSHI_SAHEN      501     //���ϕ���
#define JPOS_FUKUSHI_NI     502     //�����
#define JPOS_FUKUSHI_NANO       503     //�����
#define JPOS_FUKUSHI_DA     504     //������
#define JPOS_FUKUSHI_TO     505     //�����
#define JPOS_FUKUSHI_TOSURU     506     //����Ă���
#define JPOS_RENTAISHI      600     //�A�̎�
#define JPOS_RENTAISHI_SHIJI        601     //�w���A�̎�
#define JPOS_SETSUZOKUSHI       650     //�ڑ���
#define JPOS_KANDOUSHI      670     //������
#define JPOS_SETTOU     700     //�ړ���
#define JPOS_SETTOU_KAKU        701     //�����P�ړ���
#define JPOS_SETTOU_SAI     702     //�����Q�ړ���
#define JPOS_SETTOU_FUKU        703     //�����R�ړ���
#define JPOS_SETTOU_MI      704     //�����S�ړ���
#define JPOS_SETTOU_DAISHOU     705     //�����T�ړ���
#define JPOS_SETTOU_KOUTEI      706     //�����U�ړ���
#define JPOS_SETTOU_CHOUTAN     707     //�����V�ړ���
#define JPOS_SETTOU_SHINKYU     708     //�����W�ړ���
#define JPOS_SETTOU_JINMEI      709     //�l���ړ���
#define JPOS_SETTOU_CHIMEI      710     //�n���ړ���
#define JPOS_SETTOU_SONOTA      711     //�ŗL�ړ���
#define JPOS_SETTOU_JOSUSHI     712     //�O�u������
#define JPOS_SETTOU_TEINEI_O        713     //���J�P�ړ���
#define JPOS_SETTOU_TEINEI_GO       714     //���J�Q�ړ���
#define JPOS_SETTOU_TEINEI_ON       715     //���J�R�ړ���
#define JPOS_SETSUBI        800     //�ڔ���
#define JPOS_SETSUBI_TEKI       801     //�����P�ڔ���
#define JPOS_SETSUBI_SEI        802     //�����Q�ڔ���
#define JPOS_SETSUBI_KA     803     //�����R�ڔ���
#define JPOS_SETSUBI_CHU        804     //�����S�ڔ���
#define JPOS_SETSUBI_FU     805     //�����T�ڔ���
#define JPOS_SETSUBI_RYU        806     //�����U�ڔ���
#define JPOS_SETSUBI_YOU        807     //�����V�ڔ���
#define JPOS_SETSUBI_KATA       808     //�����W�ڔ���
#define JPOS_SETSUBI_MEISHIRENDAKU      809     //�����A��
#define JPOS_SETSUBI_JINMEI     810     //�l���ڔ���
#define JPOS_SETSUBI_CHIMEI     811     //�n���ڔ���
#define JPOS_SETSUBI_KUNI       812     //���ڔ���
#define JPOS_SETSUBI_KEN        813     //���ڔ���
#define JPOS_SETSUBI_GUN        814     //�S�ڔ���
#define JPOS_SETSUBI_KU     815     //��ڔ���
#define JPOS_SETSUBI_SHI        816     //�s�ڔ���
#define JPOS_SETSUBI_MACHI      817     //���P�ڔ���
#define JPOS_SETSUBI_CHOU       818     //���Q�ڔ���
#define JPOS_SETSUBI_MURA       819     //���P�ڔ���
#define JPOS_SETSUBI_SON        820     //���Q�ڔ���
#define JPOS_SETSUBI_EKI        821     //�w�ڔ���
#define JPOS_SETSUBI_SONOTA     822     //�ŗL�ڔ���
#define JPOS_SETSUBI_SHAMEI     823     //�Ж��ڔ���
#define JPOS_SETSUBI_SOSHIKI        824     //�g�D�ڔ���
#define JPOS_SETSUBI_KENCHIKU       825     //���z���ڔ���
#define JPOS_RENYOU_SETSUBI     826     //�A�p�ڔ���
#define JPOS_SETSUBI_JOSUSHI        827     //��u������
#define JPOS_SETSUBI_JOSUSHIPLUS        828     //��u�������{
#define JPOS_SETSUBI_JIKAN      829     //���ԏ�����
#define JPOS_SETSUBI_JIKANPLUS      830     //���ԏ������{
#define JPOS_SETSUBI_TEINEI     831     //���J�ڔ���
#define JPOS_SETSUBI_SAN        832     //���J�P�ڔ���
#define JPOS_SETSUBI_KUN        833     //���J�Q�ڔ���
#define JPOS_SETSUBI_SAMA       834     //���J�R�ڔ���
#define JPOS_SETSUBI_DONO       835     //���J�S�ڔ���
#define JPOS_SETSUBI_FUKUSU     836     //�����ڔ���
#define JPOS_SETSUBI_TACHI      837     //�����P�ڔ���
#define JPOS_SETSUBI_RA     838     //�����Q�ڔ���
#define JPOS_TANKANJI       900     //�P����
#define JPOS_TANKANJI_KAO       901     //��
#define JPOS_KANYOUKU       902     //���p��
#define JPOS_DOKURITSUGO        903     //�Ɨ���
#define JPOS_FUTEIGO        904     //�s���
#define JPOS_KIGOU      905     //�L��
#define JPOS_EIJI       906     //�p��
#define JPOS_KUTEN      907     //��_
#define JPOS_TOUTEN     908     //�Ǔ_
#define JPOS_KANJI      909     //��͕s�\����
#define JPOS_OPENBRACE      910     //�J������
#define JPOS_CLOSEBRACE     911     //������


//POS table data structure
typedef struct _POSTBL
{
    WORD        nPos;                   //pos number
    BYTE        *szName;                //name of pos
} POSTBL;


//////////////////////////////
// The IFEDictionary interface
//////////////////////////////

#undef  INTERFACE
#define INTERFACE   IFEDictionary

DECLARE_INTERFACE_(IFEDictionary, IUnknown)
{
    // IUnknown members
    STDMETHOD(QueryInterface)(THIS_ REFIID refiid, VOID **ppv) PURE;
    STDMETHOD_(ULONG,AddRef)(THIS) PURE;
    STDMETHOD_(ULONG,Release)(THIS) PURE;

    // IFEDictionary members
    STDMETHOD(Open)         (THIS_
                            CHAR *pchDictPath,          //(in) dictionary path
                            IMESHF *pshf                //(out) dictionary header
                            ) PURE;
    STDMETHOD(Close)        (THIS) PURE;
    STDMETHOD(GetHeader)    (THIS_
                            CHAR *pchDictPath,          //(in) dictionary path
                            IMESHF *pshf,               //(out) dictionary header
                            IMEFMT *pjfmt,              //(out) dictionary format
                            ULONG *pulType              //(out) dictionary type
                            ) PURE;
    STDMETHOD(Reserve1)     (THIS) PURE;
    STDMETHOD(GetPosTable)  (THIS_
                            POSTBL **prgPosTbl,         //(out) pos table pointer
                            int *pcPosTbl               //(out) pos table count pointer
                            ) PURE;
    STDMETHOD(GetWords)     (THIS_
                            WCHAR *pwchFirst,           //(in) starting range of reading
                            WCHAR *pwchLast,            //(in) ending range of reading
                            WCHAR *pwchDisplay,         //(in) display
                            ULONG ulPos,                //(in) part of speech (IFED_POS_...)
                            ULONG ulSelect,             //(in) output selection
                            ULONG ulWordSrc,            //(in) user or auto registered word?
                            UCHAR *pchBuffer,           //(in/out) buffer for storing array of IMEWRD
                            ULONG cbBuffer,             //(in) size of buffer in bytes
                            ULONG *pcWrd                //(out) count of IMEWRD's returned
                            ) PURE;
    STDMETHOD(NextWords)    (THIS_
                            UCHAR *pchBuffer,           //(in/out) buffer for storing array of IMEWRD
                            ULONG cbBuffer,             //(in) size of buffer in bytes
                            ULONG *pcWrd                //(out) count of IMEWRD's returned
                            ) PURE;
    STDMETHOD(Create)       (THIS_
                            CHAR *pchDictPath,          //(in) path for the new dictionary
                            IMESHF *pshf                //(in) dictionary header
                            ) PURE;
    STDMETHOD(SetHeader)    (THIS_
                            IMESHF *pshf                //(in) dictionary header
                            ) PURE;
    STDMETHOD(ExistWord)    (THIS_
                            IMEWRD *pwrd                //(in) word to check
                            ) PURE;
    STDMETHOD(Reserve2)     (THIS) PURE;
    STDMETHOD(RegisterWord) (THIS_
                            IMEREG reg,                 //(in) type of operation to perform on IMEWRD
                            IMEWRD *pwrd                //(in) word to be registered or deleted
                            ) PURE;
};


///////////////////////////////////
// Definitions for PerIME Interface
///////////////////////////////////

/***********************************************************************
    IME Version IDs
 ***********************************************************************/
#define VERSION_ID_JAPANESE                 0x01000000
#define VERSION_ID_KOREAN                   0x02000000
#define VERSION_ID_CHINESE_TRADITIONAL      0x04000000
#define VERSION_ID_CHINESE_SIMPLIFIED       0x08000000

#define VERSION_ID_IMEJP98  (VERSION_ID_JAPANESE | 0x980)
#define VERSION_ID_IMEJP2000 (VERSION_ID_JAPANESE | 0x98a)

/***********************************************************************
    Msg:    WM_MSIME_SERVICE
    Desc:   service functions
    Dir:    Apps to IME
    wParam: reserved
    lParam: reserved
 ***********************************************************************/

// Label for RegisterWindowMessage
#define RWM_SERVICE     TEXT("MSIMEService")

//getting version number (wParam)
#define FID_MSIME_VERSION       0

/***********************************************************************
    Msg:    WM_MSIME_UIREADY
    Desc:   service functions
    Dir:    IME to Apps
    wParam: Version ID
    lParam: reserved
 ***********************************************************************/

// Label for RegisterWindowMessage
#define RWM_UIREADY     TEXT("MSIMEUIReady")



/***********************************************************************
    Msg:    WM_MSIME_MOUSE
    Desc:   mouse operation definition
    Dir:    Apps to IME
 ***********************************************************************/

// Label for RegisterWindowMessage
#define RWM_MOUSE       TEXT("MSIMEMouseOperation")

// Mouse Operation Version (return value of IMEMOUSE_VERSION)
#define VERSION_MOUSE_OPERATION     1

// Mouse operation result
#define IMEMOUSERET_NOTHANDLED      (-1)

//WParam definition for WM_IME_MOUSE.
#define IMEMOUSE_VERSION    0xff    // mouse supported?

#define IMEMOUSE_NONE       0x00    // no mouse button was pushed
#define IMEMOUSE_LDOWN      0x01
#define IMEMOUSE_RDOWN      0x02
#define IMEMOUSE_MDOWN      0x04
#define IMEMOUSE_WUP        0x10    // wheel up
#define IMEMOUSE_WDOWN      0x20    // wheel down


/***********************************************************************
    Msg:    WM_MSIME_RECONVERT
    Desc:   reconversion
    Dir:    IME to Apps
 ***********************************************************************/

// Label for RegisterWindowMessage
#define RWM_RECONVERT           TEXT("MSIMEReconvert")


/***********************************************************************
    Msg:    WM_MSIME_RECONVERTREQUEST
    Desc:   reconversion
    Dir:    Apps to IME
 ***********************************************************************/

// wParam of WM_MSIME_RECONVERTREQUEST
#define FID_RECONVERT_VERSION   0x10000000

// Private reconversion Version
#define VERSION_RECONVERSION        1

// Label for RegisterWindowMessage
#define RWM_RECONVERTREQUEST    TEXT("MSIMEReconvertRequest")


/***********************************************************************
    Msg:    WM_MSIME_DOCUMENTFEED
    Desc:   Document feeding
    Dir:    IME to Apps
    Usage:  SendMessage( hwndApp, WM_MSIME_DOCUMENTFEED, VERSION_DOCUMENTFEED,
                (RECONVERTSTRING*)pReconv );
    wParam: VERSION_DOCUMENTFEED
    lParam: Pointer of RECONVERTSTRING structure
    return: size of RECONVERTSTRING structure
 ***********************************************************************/

// wParam of WM_MSIME_DOCUMENTFEED (set current docfeed version)
#define VERSION_DOCUMENTFEED        1

// lParam is pointer of RECONVERTSTRING structure

// Label for RegisterWindowMessage
#define RWM_DOCUMENTFEED    TEXT("MSIMEDocumentFeed")

/***********************************************************************
    Msg:    WM_MSIME_QUERYPOSITION
    Desc:   composition UI
    Dir:    IME to Apps
    Usage:  SendMessage( hwndApp, WM_MSIME_QUERYPOSITION, VERSION_QUERYPOSITION, (IMEPOSITION*)pPs );
    wParam: reserved. must be 0.
    lParam: pointer of IMEPOSITION structure
    return: Non-zero = success. Zero = error.
 ***********************************************************************/

// wParam of WM_MSIME_QUERYPOSITION
#define VERSION_QUERYPOSITION       1

// Label for RegisterWindowMessage
#define RWM_QUERYPOSITION   TEXT("MSIMEQueryPosition")


/***********************************************************************
    Msg:    WM_MSIME_MODEBIAS
    Desc:   input mode bias
    Dir:    Apps to IME
    Usage:  SendMessage( hwndDefUI, WM_MSIME_MODEBIAS, MODEBIAS_xxxx, MODEBIASMODE_xxxx );
    wParam: operation of bias
    lParam: bias mode
    return: If wParam is MODEBIAS_GETVERSION,returns version number of interface.
            If wParam is MODEBIAS_SETVALUE : return non-zero value if succeeded. Returns 0 if fail.
            If wParam is MODEBIAS_GETVALUE : returns current bias mode.
 ***********************************************************************/

// Label for RegisterWindowMessage
#define RWM_MODEBIAS            TEXT("MSIMEModeBias")

// Current version
#define VERSION_MODEBIAS        1

// Set or Get (wParam)
#define MODEBIAS_GETVERSION     0
#define MODEBIAS_SETVALUE       1
#define MODEBIAS_GETVALUE       2

// Bias (lParam)
#define MODEBIASMODE_DEFAULT                0x00000000  // reset all of bias setting
#define MODEBIASMODE_FILENAME               0x00000001  // filename


/***********************************************************************
    Msg:    WM_MSIME_SHOWIMEPAD
    Desc:   show ImePad
    Usage: SendMessage( hwndDefUI, WM_MSIME_SHOWIMEPAD, wParam, lParam );
    wParam: Applet selection option
    lParam: Applet selection parameter
            (Category defined in imepad.h or a pointer to GUID for Applet)
    return: Non-zero = accepted. Zero = not accepted.
 ***********************************************************************/

// Label for RegisterWindowMessage
#define RWM_SHOWIMEPAD          TEXT("MSIMEShowImePad")

// Applet selection option
#define SHOWIMEPAD_DEFAULT                  0x00000000  // default applet
#define SHOWIMEPAD_CATEGORY                 0x00000001  // selection by applet category
#define SHOWIMEPAD_GUID                     0x00000002  // selection by applet GUID


/***********************************************************************
    Msg:    WM_MSIME_KEYMAP
    Desc:   key map sharing with apps
 ***********************************************************************/

// Label for RegisterWindowMessage
#define RWM_KEYMAP              TEXT("MSIMEKeyMap")
#define RWM_CHGKEYMAP           TEXT("MSIMEChangeKeyMap")
#define RWM_NTFYKEYMAP          TEXT("MSIMENotifyKeyMap")

#define FID_MSIME_KMS_VERSION       1
#define FID_MSIME_KMS_INIT          2
#define FID_MSIME_KMS_TERM          3
#define FID_MSIME_KMS_DEL_KEYLIST   4
#define FID_MSIME_KMS_NOTIFY        5
#define FID_MSIME_KMS_GETMAP        6
#define FID_MSIME_KMS_INVOKE        7
#define FID_MSIME_KMS_SETMAP        8

#define IMEKMS_NOCOMPOSITION        0
#define IMEKMS_COMPOSITION          1
#define IMEKMS_SELECTION            2
#define IMEKMS_IMEOFF               3
#define IMEKMS_2NDLEVEL             4   // Reserved
#define IMEKMS_INPTGL               5   // Reserved
#define IMEKMS_CANDIDATE            6   // Reserved

typedef struct tagIMEKMSINIT {
    INT         cbSize;
    HWND        hWnd;   // Window which receives notification from IME.
                        // If hWnd is NULL, no notification is posted
                        // to Input context.
} IMEKMSINIT;

typedef struct tagIMEKMSKEY {
    DWORD dwStatus;     //Shift-Control combination status.
                        //Any combination of constants below
                        //(defined in IMM.H)
                        // 0x0000 (default)
                        // MOD_CONTROL     0x0002
                        // MOD_SHIFT       0x0004
                        // Alt key and Win key is not processed by IME.

    DWORD dwCompStatus; //Composition string status
                        //One of the constants below
                        // IMEKMS_NOCOMPOSITION  No composition string
                        // IMEKMS_COMPOSITION    Some composition string
                        // IMEKMS_SELECTION      Selection exists in apps
                        // IMEKMS_IMEOFF         IME Off state


    DWORD dwVKEY;       // VKEY code defined in IMM.H
    union {
        DWORD dwControl;// IME Functionality ID
        DWORD dwNotUsed;
    };
    union {
        WCHAR pwszDscr[31];// The pointer to string of description of this functionalify
        WCHAR pwszNoUse[31];
    };
} IMEKMSKEY;

typedef struct tagIMEKMS {
    INT         cbSize;
    HIMC        hIMC;
    DWORD       cKeyList;
    IMEKMSKEY   *pKeyList;
} IMEKMS;

typedef struct tagIMEKMSNTFY {
    INT         cbSize;
    HIMC        hIMC;
    BOOL        fSelect;
} IMEKMSNTFY;

typedef struct tagIMEKMSKMP {
    INT         cbSize;         //[in] size of this structure
    HIMC        hIMC;           //[in] Input context
    LANGID      idLang;         //[in] Language ID
    WORD        wVKStart;       //[in] VKEY start
    WORD        wVKEnd;         //[in] VKEY end
    INT         cKeyList;       //[out] number of IMEKMSKEY
    IMEKMSKEY   *pKeyList;      //[out] retrieve buffer of IMEKMSKEY
                                //      Must be GlobalMemFree by clients
} IMEKMSKMP;

typedef struct tagIMEKMSINVK {
    INT         cbSize;
    HIMC        hIMC;
    DWORD       dwControl;
} IMEKMSINVK;


/***********************************************************************
    Msg:    WM_MSIME_RECONVERTOPTIONS
    Desc:   Set reconversion options
    Usage: SendMessage( hwndDefUI, WM_MSIME_RECONVERTOPTIONS, dwOpt, (LPARAM)(HIMC)hIMC );
    wParam: options
    lParam: Input context handle
    return: Non-zero = accepted. Zero = not accepted.
 ***********************************************************************/

// Label for RegisterWindowMessage
#define RWM_RECONVERTOPTIONS          TEXT("MSIMEReconvertOptions")

//WParam definition for WM_IME_RECONVERTOPTIONS.
#define RECONVOPT_NONE              0x00000000  // default
#define RECONVOPT_USECANCELNOTIFY   0x00000001  // cancel notify

// parameter of ImmGetCompositionString
#define GCSEX_CANCELRECONVERT       0x10000000

#pragma pack()


#ifdef __cplusplus
extern "C" {
#endif

// The following API replaces CoCreateInstance(), when CLSID is not used.
HRESULT WINAPI CreateIFECommonInstance(VOID **ppvObj);
typedef HRESULT (WINAPI *fpCreateIFECommonInstanceType)(VOID **ppvObj);

// The IFELanguage global export function
HRESULT WINAPI CreateIFELanguageInstance(REFCLSID clsid, VOID **ppvObj);
typedef HRESULT (WINAPI *fpCreateIFELanguageInstanceType)(REFCLSID clsid, VOID **ppvObj);

// The following API replaces CoCreateInstance(), when CLSID is not used.
HRESULT WINAPI CreateIFEDictionaryInstance(VOID **ppvObj);
typedef HRESULT (WINAPI *fpCreateIFEDictionaryInstanceType)(VOID **ppvObj);

#ifdef __cplusplus
} /* end of 'extern "C" {' */
#endif

#endif //__MSIME_H__
