/*============================================================================
Microsoft Simplified Chinese Proofreading Engine

Microsoft Confidential.
Copyright 1997-1999 Microsoft Corporation. All Rights Reserved.

Component: CMorph
Purpose:    Implement the numerical words binding and special M+Q usage processing
Notes:      There are some analysis exceed the scope of morphological in the M+Q 
            analysis, they are some phrase structure analyzing stuff, but I think 
            implement them here is more reasonable and effecient
Owner:      donghz@microsoft.com
Platform:   Win32
Revise:     First created by: donghz    12/27/97
============================================================================*/
#include "myafx.h"

#include "morph.h"
#include "wordlink.h"
#include "scchardef.h"
//#include "engindbg.h"

//  Define the return value of rules handler functions
#define NUM_UNPROCESS   0
#define NUM_PROCESSED   1
#define NUM_ERROR       2

/*============================================================================
Implement Private functions for Numerical words analysis
============================================================================*/
//  Level 1:

/*============================================================================
CMorph::fNumerialAnalysis():
    Numerical Analysis control function. 
Returns:
    TRUE if done
    FALSE if error occurred, and the error code in m_iecError
============================================================================*/
BOOL CMorph::fNumerialAnalysis()
{
    int     iret;

    assert(m_iecError == 0); // the error code public field should be cleared
    assert(m_pLink != NULL);

    m_pWord = m_pLink->pGetHead();
    assert(m_pWord);

    if (m_pWord->fIsTail()) {
        return TRUE;
    }

    // Scan from left to right for numerial words processing
    for (; m_pWord; m_pWord = m_pWord->pNextWord()) {
        // Test the number word, combind and parse the number word
        if ((iret = GetNumber()) == NUM_UNPROCESS) {
            continue;
        }
        if (iret == NUM_ERROR) { 
            assert(m_iecError != 0);
            return FALSE;
        }
        
        // Bind ordinal number
        if (!m_pWord->fIsHead() &&
            m_pWord->pPrevWord()->fGetAttri(LADef_numTipOrdinal)) {

            if ((iret = BindOrdinal()) == NUM_PROCESSED) {
                continue;
            }
            if (iret == NUM_ERROR) {
                assert(m_iecError != 0);
                return FALSE;
            }
        }

        // Bind decimal number
        if (!m_pWord->fIsHead() &&
            m_pWord->pPrevWord()->fGetAttri(LADef_numTipDecimal) ||
            !m_pWord->fIsTail() &&
            m_pWord->pNextWord()->fGetAttri(LADef_numTipDecimal) ) {

            if ((iret = BindDecimal()) == NUM_PROCESSED) {
                continue;
            }
            if (iret == NUM_ERROR) {
                assert(m_iecError != 0);
                return FALSE;
            }
        }

        // Bind Range of Integers ��ʮ����ʮ, �塢��ʮ, ���������� ��
        if (m_pWord->fGetAttri(LADef_numInteger)) {
            if (BindRange() == NUM_ERROR) {
                assert(m_iecError != 0);
                return FALSE;
            }
        }
    }

#ifdef DEBUG
    // Validate numNumber node
    m_pWord = m_pLink->pGetHead();
    for (; m_pWord; m_pWord = m_pWord->pNextWord()) {
        int dbg_ciTag = 0;
        if (m_pWord->fGetAttri(LADef_numNumber)) {
            if (m_pWord->fGetAttri(LADef_numInteger)) {
                dbg_ciTag++;
            }
            if (m_pWord->fGetAttri(LADef_numPercent)) {
                dbg_ciTag++;
            }
            if (m_pWord->fGetAttri(LADef_numDecimal)) {
                dbg_ciTag++;
            }
            if (m_pWord->fGetAttri(LADef_numOrdinal)) {
                dbg_ciTag++;
            }
            assert(dbg_ciTag <= 1); // Only one of these 3 can be set
            if (dbg_ciTag == 0) {
                assert(m_pWord->fGetAttri(LADef_numCode)); // Must have some attribute
                //_DUMPLINK(m_pLink, m_pWord);
            } else if (m_pWord->fGetAttri(LADef_numCode)) {
                // Only numInteger could co-exist with numCode
                assert(m_pWord->fGetAttri(LADef_numInteger)); 
                //_DUMPLINK(m_pLink, m_pWord);
            } else {
            }
        }
    }
#endif // DEBUG

    return TRUE;
}


//  Level 2: 

/*============================================================================
CMorph::GetNumber():
    Analysis number word string, check error and mark the class of the merged
    number words.
Remarks:
    number testing from current word!
============================================================================*/
inline int CMorph::GetNumber()
{
    int iret = NUM_UNPROCESS;
    if (m_pWord->fGetAttri(LADef_numSBCS)) {
        numSBCSParser();
        if (m_pWord->pPrevWord() &&
            ( m_pWord->pPrevWord()->fGetAttri(LADef_numArabic) ||
              m_pWord->pPrevWord()->fGetAttri(LADef_numChinese))) {
            // ��������
            m_pWord->SetErrID(ERRDef_NOSTDNUM);
            //_DUMPLINK(m_pLink, m_pWord);
        }
        iret = NUM_PROCESSED;
    } else if (m_pWord->fGetAttri(LADef_numArabic)) {
        if (!m_pWord->fGetFlag(CWord::WF_REDUCED)) { // Has not been processed
            numArabicParser();
        }
        if (m_pWord->pPrevWord() &&
            ( m_pWord->pPrevWord()->fGetAttri(LADef_numSBCS) ||
              m_pWord->pPrevWord()->fGetAttri(LADef_numChinese))) {
            // ��������
            m_pWord->SetErrID(ERRDef_NOSTDNUM);
            //_DUMPLINK(m_pLink, m_pWord);
        }
        iret = NUM_PROCESSED;
    } else if (m_pWord->fGetAttri(LADef_numChinese)) {
        if (!m_pWord->fGetFlag(CWord::WF_REDUCED)) { // Has not been processed
            numChineseParser();
            // Return at once when error found
            if (m_pWord->GetErrID() != ERRDef_NIL) {
                return NUM_PROCESSED;
            }
        }
        if (m_pWord->fGetFlag(CWord::WF_CHAR) &&
            !m_pWord->fGetAttri(LADef_numXi) &&
            ( m_pWord->fIsWordChar(SC_CHAR_WAN) ||
              m_pWord->fIsWordChar(SC_CHAR_QIAN) ||
              m_pWord->fIsWordChar(SC_CHAR_YI4) ) &&
            !m_pWord->fIsHead() ) {

            CWord* pPrev = m_pWord->pPrevWord();
            if (pPrev->fGetAttri(LADef_numDecimal)) {
                if (pPrev->fGetAttri(LADef_numChinese)) {
                    m_pWord = m_pLink->pLeftMerge(m_pWord, 1);
                    m_pWord->SetAttri(LADef_numChinese);
                } else {
                    m_pWord = m_pLink->pLeftMerge(m_pWord, 1);
                    m_pWord->SetAttri(LADef_numMixed);
                }
                //_DUMPCASE(m_pLink, m_pWord, 1000);
                m_pWord->SetAttri(LADef_numInteger);
            } else if (pPrev->fGetAttri(LADef_numSBCS) ||
                       pPrev->fGetAttri(LADef_numArabic) ) { // 8��, 5ǧ������, ��������
                m_pWord = m_pLink->pLeftMerge(m_pWord, 1);
                if (!m_pWord->fIsHead() &&
                    m_pWord->pPrevWord()->fGetAttri(LADef_numMixed)) {
                    // ��������ڵ㣺8��5ǧ�����ڣ�������
                    m_pWord = m_pLink->pLeftMerge(m_pWord, 1);
                }
                if (!m_pWord->fIsTail() &&
                    ( m_pWord->pNextWord()->fIsWordChar(SC_CHAR_DUO) ||
                      m_pWord->pNextWord()->fIsWordChar(SC_CHAR_YU) ) ) {
                    // 8��5ǧ ��/��
                    m_pLink->MergeWithNext(m_pWord);
                    m_pWord->SetAttri(LADef_amtApprox);
                }
                m_pWord->SetAttri(LADef_numMixed);
                m_pWord->SetAttri(LADef_numInteger);
                //_DUMPCASE(m_pLink, m_pWord, 1001);
            } else if (( pPrev->fIsWordChar(SC_CHAR_DUO) ||
                         pPrev->fIsWordChar(SC_CHAR_YU)) &&
                       !pPrev->fIsHead() &&
                       pPrev->pPrevWord()->fGetAttri(LADef_numInteger) ) {
                // ���� + �� + ��/��
                if (pPrev->pPrevWord()->fGetAttri(LADef_numChinese)) {
                    // ��ʮ ��/�� ��
                    m_pWord = m_pLink->pLeftMerge(m_pWord, 2);
                    m_pWord->SetAttri(LADef_numInteger);
                    m_pWord->SetAttri(LADef_numChinese);
                    m_pWord->SetAttri(LADef_amtApprox);
                    //_DUMPCASE(m_pLink, m_pWord, 1002);
                } else if (pPrev->pPrevWord()->fGetAttri(LADef_numArabic) ||
                           pPrev->pPrevWord()->fGetAttri(LADef_numSBCS) ) {
                    // Merge mixed number
                    m_pWord = m_pLink->pLeftMerge(m_pWord, 2);
                    if (!m_pWord->fIsHead() &&
                        m_pWord->pPrevWord()->fGetAttri(LADef_numMixed)) {
                        // 1ǧ200����
                        m_pWord = m_pLink->pLeftMerge(m_pWord, 1);
                    }
                    //_DUMPCASE(m_pLink, m_pWord, 1003);
                    m_pWord->SetAttri(LADef_numInteger);
                    m_pWord->SetAttri(LADef_numMixed);
                    m_pWord->SetAttri(LADef_amtApprox);
                } else {
                }
            } else {
            }
        } else if (!m_pWord->fIsHead() &&
                   ( m_pWord->pPrevWord()->fGetAttri(LADef_numSBCS) || 
                     m_pWord->pPrevWord()->fGetAttri(LADef_numArabic) ) ) {
            // ��������
            m_pWord->SetErrID(ERRDef_NOSTDNUM);
            //_DUMPCASE(m_pLink, m_pWord, 1004);
        } else {
        }

        m_pWord->SetAttri(LADef_numNumber);
        iret = NUM_PROCESSED;
    } else if (m_pWord->fGetAttri(LADef_numGan) && !m_pWord->fIsTail() &&
               m_pWord->pNextWord()->fGetAttri(LADef_numZhi) ) {
        // Merge ��ɵ�֧
        numGanZhiHandler();
        m_pWord->SetAttri(LADef_posT);
        iret = NUM_PROCESSED;
    } else {
    }

    return iret;
}


//  �����ʴ���
int CMorph::BindOrdinal()
{
    int iret;
    assert(!m_pWord->fIsHead() &&
           m_pWord->pPrevWord()->fGetAttri(LADef_numTipOrdinal));

    if (m_pWord->pPrevWord()->fIsWordChar(SC_CHAR_DI)) {
        iret = ordDi_Proc();    // ��
    } else if (m_pWord->pPrevWord()->fIsWordChar(SC_CHAR_CHU)) {
        iret = ordChu_Proc();   // ��
    } else {
        //_DUMPLINK(m_pLink, m_pWord);
        return NUM_UNPROCESS;
    }
    m_pWord->SetAttri(LADef_numNumber);
    return NUM_PROCESSED;
}


//  С������������
int CMorph::BindDecimal()
{
    assert(!m_pWord->fIsHead() &&
           m_pWord->pPrevWord()->fGetAttri(LADef_numTipDecimal) ||
           !m_pWord->fIsTail() &&
           m_pWord->pNextWord()->fGetAttri(LADef_numTipDecimal) );

    CWord*  pWord;
    int     iret;
    BOOL    fHit = FALSE;
    // Handle ����
    if (!m_pWord->fIsTail() &&
        m_pWord->pNextWord()->fGetAttri(LADef_numTipDecimal)) {

        pWord = m_pWord->pNextWord();
        if (pWord->fIsWordChar(SC_CHAR_DIAN3)) {        // ��
            iret = decDian_Proc();      
        } else if (pWord->fIsWordChar(SC_CHAR_BEI)) {   // ��
            iret = decBei_Proc();
        } else if (pWord->fIsWordChar(SC_CHAR_CHENG)) { // ��
            iret = decCheng_Proc();
        } else {
            //_DUMPLINK(m_pLink, m_pWord);
            iret = NUM_UNPROCESS;
        }

        if (iret == NUM_PROCESSED) {
            fHit = TRUE;
        } else if (iret == NUM_ERROR) {
            return NUM_ERROR;
        } else {
        }
    }

    // Handle ǰ��
    if (!m_pWord->fIsHead() &&
        m_pWord->pPrevWord()->fGetAttri(LADef_numTipDecimal)) {

        pWord = m_pWord->pPrevWord();
        if (pWord->fIsWordText(SC_WORD_BAIFENZHI)) {    // �ٷ�֮
            iret = decBaiFen_Proc();
        } else if (pWord->fIsWordText(SC_WORD_FENZHI)) {// ��֮
            iret = decFenZhi_Proc();
        } else if (pWord->fIsWordText(SC_WORD_QIANFENZHI) ||  // ǧ��֮
                   pWord->fIsWordText(SC_WORD_WANFENZHI))  {  // ���֮
            iret = decBaiFen_Proc();
        } else {
            //_DUMPLINK(m_pLink, m_pWord);
            iret = NUM_UNPROCESS;
        }

        if (iret == NUM_PROCESSED) {
            fHit = TRUE;
        } else if (iret == NUM_ERROR) {
            return NUM_ERROR;
        } else {
        }
    }
    if (fHit) {
        return NUM_PROCESSED;
    }
    return NUM_UNPROCESS;
}


//  �����������������: ��/��/��
inline int CMorph::BindRange()
{
    assert(m_pWord->fGetAttri(LADef_numInteger));

    if (m_pWord->fIsHead()) {
        return NUM_UNPROCESS;
    }

    USHORT iStyle;
    CWord* pWord = m_pWord->pPrevWord();
    if( pWord->fGetFlag(CWord::WF_CHAR) &&
        ( pWord->fIsWordChar(SC_CHAR_ZHI4) ||
          pWord->fIsWordChar(SC_CHAR_DUNHAO) ||
          pWord->fIsWordChar(SC_CHAR_LIANHAO) ) &&
        !pWord->fIsHead() &&
        pWord->pPrevWord()->fGetAttri(LADef_numInteger) ) { // Match!
        // Get number style of m_pWord
        if (m_pWord->fGetAttri(LADef_numSBCS)) {
            iStyle = LADef_numSBCS;
        } else if (m_pWord->fGetAttri(LADef_numArabic)) {
            iStyle = LADef_numArabic;
        } else if (m_pWord->fGetAttri(LADef_numChinese)) {
            iStyle = LADef_numChinese;
        } else if (m_pWord->fGetAttri(LADef_numMixed)) {
            iStyle = LADef_numMixed;
        } else {
            //_DUMPLINK(m_pLink, m_pWord);
            iStyle = 0;
        }
        // Check style with previous integer
        if (!pWord->pPrevWord()->fGetAttri(iStyle)) { // Unmatched style!
            if ((iStyle == LADef_numSBCS || iStyle == LADef_numArabic) &&
                ( pWord->pPrevWord()->fGetAttri(LADef_numSBCS) ||
                  pWord->pPrevWord()->fGetAttri(LADef_numArabic) ) ) {
                // Mixed usage of SBCS and Arabic numbers
                m_pWord->SetErrID(ERRDef_NOSTDNUM);
                //_DUMPLINK(m_pLink, pWord);
            }
            return NUM_UNPROCESS;
        }
        // Merge the integer range
        m_pWord = pWord->pPrevWord();
        m_pLink->MergeWithNext(m_pWord);
        m_pLink->MergeWithNext(m_pWord);
        m_pWord->SetAttri(iStyle);
        m_pWord->SetAttri(LADef_numInteger);
        m_pWord->SetAttri(LADef_amtApprox);
        m_pWord->SetAttri(LADef_numNumber);
        //_DUMPLINK(m_pLink, m_pWord);
    } else {
        //_DUMPLINK(m_pLink, m_pWord);
    }

    return NUM_UNPROCESS;
}

    
//  Level 3:
//  Parse the 3 kinds of number: numSBCS, numArabic, numChinese

//  Parser for SBCS number called by GetNumber()
void CMorph::numSBCSParser()
{
    assert(m_pWord->fGetFlag(CWord::WF_SBCS) &&
           m_pWord->fGetAttri(LADef_numSBCS));

    BOOL fFloat = FALSE;
    BOOL fPercent = FALSE;
    WCHAR* pwch = m_pWord->pwchGetText();
    WCHAR* pwchTail = m_pWord->pwchGetText()+m_pWord->cwchLen();

    // only one character
    if (m_pWord->cwchLen() == 1) {
        // Clear the numSBCS attribute if only '.' or '%' alone
        if (*pwch == SC_CHAR_ANSIPERCENT || *pwch == SC_CHAR_ANSIDIAN) {
            m_pWord->ClearAttri(LADef_numSBCS);
            return;
        }
    }

    // Multi-char SBCS number
    while (pwch < pwchTail) {
        if (*pwch == SC_CHAR_ANSIDIAN) {
            fFloat = TRUE;
        } else if (*pwch == SC_CHAR_ANSIPERCENT) {
            fPercent = TRUE;
        }
        pwch++;
    }
    if (fPercent) {
        m_pWord->SetAttri(LADef_numPercent);
    } else if (fFloat) {
        m_pWord->SetAttri(LADef_numDecimal);
    } else {
        if (!m_pWord->fIsHead() && !m_pWord->pPrevWord()->fIsHead() &&
            m_pWord->pPrevWord()->pPrevWord()->fGetAttri(LADef_numSBCS) &&
            m_pWord->pPrevWord()->pPrevWord()->fGetAttri(LADef_numInteger) &&
            *(m_pWord->pPrevWord()->pwchGetText()) == SC_CHAR_ANSISLASH ) {
            // fraction like 2/3
            m_pWord = m_pWord->pPrevWord()->pPrevWord();
            m_pLink->MergeWithNext(m_pWord);
            m_pLink->MergeWithNext(m_pWord);
            m_pWord->SetAttri(LADef_numSBCS);
            m_pWord->SetAttri(LADef_numPercent);
            //_DUMPLINK(m_pLink, m_pWord);
        } else {
            m_pWord->SetAttri(LADef_numCode);
            m_pWord->SetAttri(LADef_numInteger);
        }
    }
    // Mark numNumber
    m_pWord->SetAttri(LADef_numNumber);

    return;
}


//  Parser for DBCS Arabic number called by GetNumber()
void CMorph::numArabicParser() 
{
    assert(!m_pWord->fGetFlag(CWord::WF_SBCS) && !m_pWord->fGetFlag(CWord::WF_REDUCED));
    assert(m_pWord->fGetAttri(LADef_numArabic));
    
    // Merge continuous Arabic numbers
    BOOL fFloat = FALSE;
    BOOL fPercent = FALSE;
    while (m_pWord->pNextWord() &&
           m_pWord->pNextWord()->fGetAttri(LADef_numArabic)) {
        if (m_pWord->pNextWord()->fIsWordChar(SC_CHAR_SHUDIAN)) {
            if (!fFloat && m_pWord->pNextWord()->pNextWord() &&
                m_pWord->pNextWord()->pNextWord()->fGetAttri(LADef_numArabic)) {

                fFloat = TRUE;
            } else {
                break;
            }
        } else if (m_pWord->pNextWord()->fIsWordChar(SC_CHAR_PERCENT)) {
            fPercent = TRUE;
        } else if (fPercent) {
            m_pWord->SetErrID(ERRDef_NUMERIAL);
        }
        m_pLink->MergeWithNext(m_pWord);
    }

    if (m_pWord->fGetFlag(CWord::WF_CHAR)) {
        if (m_pWord->fIsWordChar(SC_CHAR_PERCENT) ||
            m_pWord->fIsWordChar(SC_CHAR_SHUDIAN)) { 
            if (!m_pWord->fIsHead() && m_pWord->fGetAttri(LADef_numSBCS)) {
                // DBCS "��" or "��" follow the SBCS number
                m_pWord = m_pLink->pLeftMerge(m_pWord, 1);
                m_pWord->SetErrID(ERRDef_NOSTDNUM);
                //_DUMPLINK(m_pLink, m_pWord);
            } else {
                // Only DBCS "��" or "��", return w/o mark numArabic
            }
            return;
        }
    }

    // Set the attribute for multi-char Arabic number
    if (fPercent) {
        m_pWord->SetAttri(LADef_numPercent);
        //_DUMPLINK(m_pLink, m_pWord);
    } else if (fFloat) {
        m_pWord->SetAttri(LADef_numDecimal);
        //_DUMPLINK(m_pLink, m_pWord);
    } else {
        if ( !m_pWord->fIsHead() && !m_pWord->pPrevWord()->fIsHead() &&
             m_pWord->pPrevWord()->pPrevWord()->fGetAttri(LADef_numArabic) &&
             m_pWord->pPrevWord()->pPrevWord()->fGetAttri(LADef_numInteger) &&
             m_pWord->pPrevWord()->fIsWordChar(SC_CHAR_SLASH) ) {
            // fraction like 2/3
            m_pWord = m_pWord->pPrevWord()->pPrevWord();
            m_pLink->MergeWithNext(m_pWord);
            m_pLink->MergeWithNext(m_pWord);
            m_pWord->SetAttri(LADef_numPercent);
            //_DUMPLINK(m_pLink, m_pWord);
        } else {
            m_pWord->SetAttri(LADef_numCode);
            m_pWord->SetAttri(LADef_numInteger);
            //_DUMPLINK(m_pLink, m_pWord);
        }
    }
    m_pWord->SetAttri(LADef_numArabic);
    m_pWord->SetAttri(LADef_numNumber);
    return;
}


//  Parser for DBCS Chinese number called by GetNumber()
//  Define the events constant
#define EVENT_LING  0
#define EVENT_XI    1
#define EVENT_LIANG 2
#define EVENT_SHI   3
#define EVENT_BAI   4
#define EVENT_QIAN  5
#define EVENT_WAN   6
#define EVENT_YI    7
#define EVENT_JI    8
#define EVENT_END   9

//  Define the events array
static struct { 
    WCHAR   m_wchEvent;
    char    m_iEvent;
    } v_rgEvent[] = {
                     { SC_DIGIT_YI,    EVENT_XI   },// 0x4e00 "һ"
                     { SC_DIGIT_QI,    EVENT_XI   },// 0x4e03 "��"
                     { SC_DIGIT_WAN,   EVENT_WAN  },// 0x4e07 "��"
                     { SC_DIGIT_SAN,   EVENT_XI   },// 0x4e09 "��"
                     { SC_DIGIT_LIANG, EVENT_LIANG},// 0x4e24 "��"
                     { SC_DIGIT_JIU,   EVENT_XI   },// 0x4e5d "��"
                     { SC_DIGIT_ER,    EVENT_XI   },// 0x4e8c "��"
                     { SC_DIGIT_WU,    EVENT_XI   },// 0x4e94 "��"
                     { SC_DIGIT_YI4,   EVENT_YI   },// 0x4ebf "��"
                     { SC_DIGIT_BA,    EVENT_XI   },// 0x516b "��"
                     { SC_DIGIT_LIU,   EVENT_XI   },// 0x516d "��"
                     { SC_CHAR_JI,     EVENT_JI   },// 0x51e0 "��"
                     { SC_DIGIT_SHI,   EVENT_SHI  },// 0x5341 "ʮ"
                     { SC_DIGIT_QIAN,  EVENT_QIAN },// 0x5343 "ǧ"
                     { SC_DIGIT_SI,    EVENT_XI   },// 0x56db "��"
                     { SC_DIGIT_BAI,   EVENT_BAI  },// 0x767e "��"
                     { SC_DIGIT_LING,  EVENT_LING },// 0x96f6 "��"
                     { SC_ARABIC_LING, EVENT_LING },// 0xff10 "��"
                     { SC_DBCS_LING,   EVENT_LING } // 0X3007 "��"
    };

static const char v_ciEvents = 10;

//  Define the state transfer array
//  Ambiguities! can not find errors like һ������ǧ or һ����һǧ�� (Unexpected "��")
static char v_rgStateTrans[][v_ciEvents] = { 
    //                  ��  ϵ  ��  ʮ  ��  ǧ  ��  ��  ��  End
    /*  0 ��̬      */  -1,  1,  1,  3,  6,  9, 12, 24,  1, -1,
    /*  1 ϵ��      */   2,  1,  1,  3,  6,  9, 12, 24, -1,  0,
    /*  2 ��ϵ��    */  -1, -1, -1, -1,  6,  9, 12, 24, -1, -1,
    /*  3 ʮλ      */  -1,  4, -1, -1, -1, -1, -1, -1,  4,  0,
    /*  4 ϵʮ      */   5,  4, -1, -1,  6, -1, -1, -1, -1,  0,
    /*  5 ��ϵʮ    */  -1, -1, -1, -1, -1,  9, 12, 24, -1, -1,
    /*  6 ��λ      */  -1,  7,  7, -1, -1, -1, -1, -1,  7, -1,
    /*  7 ϵ��      */   8,  7,  7, -1, -1,  9, -1, -1, -1,  0,
    /*  8 ��ϵ��    */  -1, -1, -1, -1, -1, -1, 12, 24, -1, -1,
    /*  9 ǧλ      */  -1, 10, 10, -1, -1, -1, -1, -1, 10, -1,
    /* 10 ϵǧ      */  11, 10, 10, -1, -1, -1, 12, -1, -1,  0,
    /* 11 ��ϵǧ    */  -1, -1, -1, -1, -1, -1, 12, 24, -1, -1,
    /* 12 ��λ      */  -1, 13, 13, 15, 18, 21, -1, -1, 13, -1,
    /* 13 ϵ��      */  14, 13, 13, 15, -1, -1, -1, -1, -1,  0,
    /* 14 ��ϵ��    */  -1, -1, -1, -1, 18, 21, -1, 24, -1, -1,
    /* 15 ʮ��λ    */  -1, 16, 16, -1, -1, -1, -1, -1, 16,  0,
    /* 16 ϵʮ��    */  17, 16, 16, -1, 18, -1, -1, -1, -1,  0,
    /* 17 ��ϵʮ��  */  -1, -1, -1, -1, 18, 21, -1, 24, -1, -1,
    /* 18 ����λ    */  -1, 19, 19, -1, -1, -1, -1, -1, 19, -1,
    /* 19 ϵ����    */  20, 19, 19, -1, -1, 21, -1, -1, -1,  0,
    /* 20 ��ϵ����  */  -1, -1, -1, -1, -1, 21, -1, 24, -1, -1,
    /* 21 ǧ��λ    */  -1, 22, 22, -1, -1, -1, -1, -1, 22, -1,
    /* 22 ϵǧ��    */  23, 22, 22, -1, -1, -1, -1, 24, -1,  0,
    /* 23 ��ϵǧ��  */  -1, -1, -1, -1, -1, -1, -1, 24, -1, -1,
    /* 24 ��λ      */  -1, 25, 25, 27, 30, 32, -1, -1, 25, -1,
    /* 25 ϵ��      */  26, 25, 25, 27, -1, -1, -1, -1, -1,  0,
    /* 26 ��ϵ��    */  -1, -1, -1, -1, 30, 32, -1, -1, -1, -1,
    /* 27 ʮ��λ    */  -1, 28, 28, -1, -1, -1, -1, -1, 28,  0,
    /* 28 ϵʮ��    */  29, 28, 28, -1, 30, -1, -1, -1, -1,  0,
    /* 29 ��ϵʮ��  */  -1, -1, -1, -1, -1, 32, -1, -1, -1, -1,
    /* 30 ����λ    */  -1, 31, 31, -1, -1, -1, -1, -1, 31, -1,
    /* 31 ϵ����    */  -1, 31, 31, -1, -1, 32, -1, -1, -1,  0,
    /* 32 ǧ��λ    */  -1, 33, 33, -1, -1, -1, -1, -1, 33, -1,
    /* 33 ϵǧ��    */  -1, 33, 33, -1, -1, -1, -1, -1, -1,  0
};

void CMorph::numChineseParser() 
{
    assert(!m_pWord->fGetFlag(CWord::WF_REDUCED));
    assert(m_pWord->fGetAttri(LADef_numChinese));

    char iEvent, iState;
    BOOL fXi = m_pWord->fGetAttri(LADef_numXi);
    BOOL fWei = !fXi;
    BOOL fJi = m_pWord->fIsWordChar(SC_CHAR_JI);
    // Merge the continuous Chinese number words
    while (m_pWord->pNextWord() &&
           m_pWord->pNextWord()->fGetAttri(LADef_numChinese)) {
        fJi = (fJi || m_pWord->fIsWordChar(SC_CHAR_JI));
        if (m_pWord->pNextWord()->fGetAttri(LADef_numXi)) {
            fXi = TRUE;
        } else {
            fWei = TRUE;
        }
        m_pLink->MergeWithNext(m_pWord);
    }
    m_pWord->SetAttri(LADef_numChinese);

    // Only one character numerial word
    if (m_pWord->cwchLen() == 1) {
        if ( m_pWord->fGetAttri(LADef_numXi) ||    // һ...��, ��, ��, ��
             m_pWord->fIsWordChar(SC_DIGIT_SHI) ) {// ʮ
            //_DUMPLINK(m_pLink, m_pWord);
        } else {  // �� or ��, ǧ, ��, ��
            m_pWord->SetAttri(LADef_amtApprox);
            //_DUMPLINK(m_pLink, m_pWord);
        }
        // Bind �� ʮ/��/ǧ/��/��
        if (m_pWord->pPrevWord() &&
            m_pWord->pPrevWord()->fGetAttri(LADef_amtWanQian) &&
            ( m_pWord->fGetAttri(LADef_amtApprox) &&
              !m_pWord->fIsWordChar(SC_CHAR_JI) ||
              m_pWord->fIsWordChar(SC_DIGIT_SHI)) ) {

            m_pWord = m_pWord->pPrevWord();
            m_pLink->MergeWithNext(m_pWord);
            m_pWord->SetAttri(LADef_amtApprox);
        }
        m_pWord->SetAttri(LADef_numInteger);
        return;
    }
    // Only λ words like: ǧ���򡢰�������
    if (!fXi && fWei) {
        // Sould not contain dup chars
        if (fCheckDupChar(m_pWord)) {
            m_pWord->SetErrID(ERRDef_NUMERIAL);
            return;
        }
        // Bind ��/��/�ü�/��/ÿ ʮ��/����/ǧ��
        if (m_pWord->pPrevWord() &&
            m_pWord->pPrevWord()->fGetAttri(LADef_amtWanQian)) {

            m_pWord = m_pWord->pPrevWord();
            m_pLink->MergeWithNext(m_pWord);
        }
        m_pWord->SetAttri(LADef_amtApprox);
        m_pWord->SetAttri(LADef_numInteger);
        //_DUMPLINK(m_pLink, m_pWord);
        return;
    }
    // Only ϵ words
    if (fXi && !fWei) {
        if (m_pWord->cwchLen() == 2) {
            if (fValidApproxNum(m_pWord->pwchGetText())) {
                // Valid ��λ������
                m_pWord->SetAttri(LADef_amtApprox);
                m_pWord->SetAttri(LADef_numInteger);
                //_DUMPLINK(m_pLink, m_pWord);
            } else if (!m_pWord->fIsTail() &&
                        m_pWord->pNextWord()->fGetAttri(LADef_posQ)) {
                // Other [ϵ + ϵ]
                if (!m_pWord->fIsHead() &&
                    m_pWord->pPrevWord()->fIsWordChar(SC_CHAR_DIAN3)) {
                    //_DUMPCASE(m_pLink, m_pWord, 1031);
                } else if (!m_pWord->fIsTail() &&
                    m_pWord->pNextWord()->fIsWordChar(SC_CHAR_NIAN)) {
                    //_DUMPCASE(m_pLink, m_pWord, 1021);
                } else if (m_pWord->fIsWordLastChar(SC_CHAR_LIANG)) {
                    //_DUMPCASE(m_pLink, m_pWord, 1011);
                } else if (!m_pWord->fGetFlag(CWord::WF_REDUCED)) {
                    // Don't mark error on words like ��һ����һ
                } else {
                    // �������÷����� 
                    m_pWord->SetErrID(ERRDef_NUMERIAL);
                    //_DUMPCASE(m_pLink, m_pWord, 1001);
                }
            } else {
            }
        } // end of if (m_pWord->cwchLen() == 2) 
        if (m_pWord->cwchLen() > 2 && fJi) {
            m_pWord->SetErrID(ERRDef_NUMERIAL);
            //_DUMPCASE(m_pLink, m_pWord, 1002);
        }
        m_pWord->SetAttri(LADef_numCode);
        return;
    }

    // XiWei mixed as integer
    m_pWord->SetAttri(LADef_numInteger);

    // Analyz the ϵλ�ṹ string from right to left
    fXi = FALSE; iState = 0;
    WCHAR* pwchChar = m_pWord->pwchGetText() + m_pWord->cwchLen();
    while (pwchChar > m_pWord->pwchGetText()) {
        pwchChar -= 1; // Move to the last char in the Chinese number string
        // Search in the events array and get current event
        iEvent = EVENT_END;
        int lo = 0, hi = sizeof(v_rgEvent) / sizeof(v_rgEvent[0]) - 1, mi, icmp;
        while (lo <= hi) {
            mi = (lo + hi) / 2;
            if ((icmp = *pwchChar - v_rgEvent[mi].m_wchEvent) == 0) {
                // Match!
                iEvent = v_rgEvent[mi].m_iEvent;
                break;
            } else if (icmp < 0) {
                hi = mi - 1;
            } else {
                lo = mi + 1;
            }
        }

        assert(iEvent != EVENT_END);

        // Handle some special cases for the state machine
        if ((iEvent == EVENT_XI || iEvent == EVENT_LIANG) && fXi) {
            // ϵ + ϵ (����: һ...��, ��. ������: ��, ��, ��)
            if (m_pWord->fGetAttri(LADef_amtApprox)) {
                // more than one [ϵ+ϵ] structure in the same ϵλ����
                iState = -1;
                break;
            } else {
                assert((pwchChar + 2) <= m_pWord->pNextWord()->pwchGetText());

                if (!fValidApproxNum(pwchChar)) { // [ϵ+ϵ]�ṹ��
                    iState = -1;
                    break;
                }
                // Not handle error: һ������ʮ, һ���߰�ʮ��
            }
            m_pWord->SetAttri(LADef_amtApprox); // ������ if there is a [ϵ+ϵ]�ṹ
        }
        fXi = (iEvent == EVENT_XI || iEvent == EVENT_LIANG) ? TRUE : FALSE;

        // Transfer to next state
        iState = v_rgStateTrans[iState][iEvent];
        if(iState == -1) {
            //_DUMPLINK(m_pLink, m_pWord);
            break;
        }
    }
    // Check the end event
    if (iState != -1 && v_rgStateTrans[iState][EVENT_END] == 0) {
        // Valid Chinese number!
    } else { // error found!
        if (!m_pWord->fIsHead() && 
            m_pWord->pPrevWord()->fIsWordChar(SC_CHAR_DIAN3) &&
            ( m_pWord->fIsWordLastChar(SC_CHAR_WAN) ||
              m_pWord->fIsWordLastChar(SC_CHAR_YI4) ) ) {
            // ��ʮһ �� �İ� ��
            m_pWord = m_pLink->pSplitWord(m_pWord, m_pWord->cwchLen() - 1);
            m_pWord->SetAttri(LADef_numCode);
            m_pWord->SetAttri(LADef_numChinese);
            m_pWord->pNextWord()->SetAttri(LADef_numInteger);
            m_pWord->pNextWord()->SetAttri(LADef_amtApprox);
            m_pWord->pNextWord()->SetAttri(LADef_numChinese);
            //_DUMPCASE(m_pLink, m_pWord, 1004);
        } else if ( m_pWord->cwchLen() == 2 &&
                    ( m_pWord->fIsWordText(SC_WORD_WANLIANG) ||
                      m_pWord->fIsWordText(SC_WORD_YI4LIANG) ) ) {
            // ���� or ����
            m_pWord = m_pLink->pSplitWord(m_pWord, 1);
            m_pWord->SetAttri(LADef_numInteger);
            m_pWord->SetAttri(LADef_numChinese);
            m_pWord->SetAttri(LADef_amtApprox);
            if (!fRecheckLexInfo(m_pWord->pNextWord())) {
                assert(0);
            }
            //_DUMPCASE(m_pLink, m_pWord, 1006);
        } else {
            m_pWord->SetErrID(ERRDef_NUMERIAL);
            //_DUMPCASE(m_pLink, m_pWord, 1003);
        }
    }
    return;
}


void CMorph::numGanZhiHandler()     // ��ɵ�֧
{
    // Occurs 29 times in 20M IME Corpus
    assert(!m_pWord->fIsTail());

    m_pLink->MergeWithNext(m_pWord);
    if (!m_pWord->fIsTail() && m_pWord->pNextWord()->fIsWordChar(SC_CHAR_NIAN)){
        m_pLink->MergeWithNext(m_pWord);
    }
    m_pWord->SetAttri(LADef_posT);
    //_DUMPLINK(m_pLink, m_pWord);
    return;
}


/*============================================================================
//  Following case processors:
//      Return NUM_PROCESSED if merged successfully or any error found
//      Return NUM_UNPROCESS if could not merged
//      Return NUM_ERROR if any error occurred, the error code in m_iecError
============================================================================*/

//  Ordinal number processors: called by BindOrdinal()
inline int CMorph::ordDi_Proc()         // ��
{
    if (m_pWord->fGetAttri(LADef_numInteger) &&
        !m_pWord->fGetAttri(LADef_amtApprox) ||
        m_pWord->fIsWordChar(SC_CHAR_JI)) {
        // �� + ����(�Ǹ���) or �� + ��
        USHORT    iStyle;
        if (m_pWord->fGetAttri(LADef_numChinese)) {
            iStyle = LADef_numChinese;
        } else if (m_pWord->fGetAttri(LADef_numArabic)) {
            iStyle = LADef_numArabic;
        } else if (m_pWord->fGetAttri(LADef_numSBCS)) {
            iStyle = LADef_numSBCS;
        } else if (m_pWord->fGetAttri(LADef_numMixed)){
            iStyle = LADef_numMixed;
        } else {
            assert(0);
            iStyle = 0;
        }
        m_pWord = m_pWord->pPrevWord();
        m_pLink->MergeWithNext(m_pWord);
        m_pWord->SetAttri(iStyle);
        m_pWord->SetAttri(LADef_numOrdinal);
        // Properties of LADef_numNumber are set in upper level
        //_DUMPLINK(m_pLink, m_pWord);
        return NUM_PROCESSED;
    }
    //_DUMPLINK(m_pLink, m_pWord);
    return NUM_UNPROCESS;
}

inline int CMorph::ordChu_Proc()            // ��
{
    if (m_pWord->fGetFlag(CWord::WF_CHAR) &&
        m_pWord->fGetAttri(LADef_numChinese) &&
        m_pWord->fGetAttri(LADef_numInteger) ) {
        // �� + ����һλ����
        m_pWord = m_pWord->pPrevWord();
        m_pLink->MergeWithNext(m_pWord);
        m_pWord->SetAttri(LADef_numOrdinal);
        // Properties of LADef_numNumber are set in upper level
        //_DUMPLINK(m_pLink, m_pWord);
        return NUM_PROCESSED;
    }
    //_DUMPLINK(m_pLink, m_pWord);
    return NUM_UNPROCESS;
}


//  Decimal number processors: called by BindDecimal()
inline int CMorph::decBaiFen_Proc()     // �ٷ�֮
{
    if (m_pWord->fGetAttri(LADef_numInteger) ||
        m_pWord->fGetAttri(LADef_numDecimal) ||
        m_pWord->fIsWordChar(SC_CHAR_JI) ) {

        if (m_pWord->fGetAttri(LADef_numChinese)) {
            m_pWord = m_pWord->pPrevWord();
            m_pLink->MergeWithNext(m_pWord);
            if (!m_pWord->fIsHead() &&
                m_pWord->pPrevWord()->fGetAttri(LADef_numInteger)) {

                if (m_pWord->pPrevWord()->fGetAttri(LADef_numChinese)) {
                    m_pWord = m_pWord->pPrevWord();
                    m_pLink->MergeWithNext(m_pWord);
                    //_DUMPLINK(m_pLink, m_pWord);
                } else {
                    // Error: 3�ٷ�֮��ʮ
                    m_pWord->pPrevWord()->SetErrID(ERRDef_NUMERIAL);
                    //_DUMPLINK(m_pLink, m_pWord);
                }
            }
            m_pWord->SetAttri(LADef_numPercent);
            m_pWord->SetAttri(LADef_numNumber);
            //_DUMPLINK(m_pLink, m_pWord);
            return NUM_PROCESSED;
        } else {
            // Error: �ٷ�֮50
            m_pWord->SetErrID(ERRDef_NUMERIAL);
            //_DUMPLINK(m_pLink, m_pWord);
        }
    }
    //_DUMPLINK(m_pLink, m_pWord);
    return NUM_UNPROCESS;
}


inline int CMorph::decCheng_Proc()          // ��
{
    CWord* pWord;

    assert(m_pWord->pNextWord()->fIsWordChar(SC_CHAR_CHENG));

    if (!(m_pWord->fGetFlag(CWord::WF_CHAR) && m_pWord->fGetAttri(LADef_numInteger)) &&
        !(m_pWord->cwchLen() == 2 && fValidApproxNum(m_pWord->pwchGetText())) ) {
        // ������
        m_pLink->MergeWithNext(m_pWord);
        m_pWord->SetErrID(ERRDef_NUMERIAL);
        //_DUMPLINK(m_pLink, m_pWord);
        return NUM_PROCESSED;
    }
    // Merge with ��
    m_pLink->MergeWithNext(m_pWord);

    // Test number after ��
    if (!m_pWord->fIsTail()) {
        pWord = m_pWord;
        m_pWord = m_pWord->pNextWord();
        if (GetNumber() == NUM_PROCESSED &&
            m_pWord->fGetFlag(CWord::WF_CHAR) &&
            m_pWord->fGetAttri(LADef_numInteger)) {
            // ������
            m_pLink->MergeWithNext(pWord);
            //_DUMPLINK(m_pLink, m_pWord);
        } else if (m_pWord->fIsWordChar(SC_CHAR_BAN)) { // ���ɰ�
            m_pLink->MergeWithNext(pWord);
            //_DUMPLINK(m_pLink, m_pWord);
        }
        m_pWord = pWord;
    }

    // Test һ������ or �������
    if (!m_pWord->fIsHead() && !m_pWord->pPrevWord()->fIsHead()) {
        pWord = m_pWord->pPrevWord();
        if (pWord->fGetFlag(CWord::WF_CHAR) &&
            pWord->pPrevWord()->fGetFlag(CWord::WF_CHAR) &&
            pWord->pPrevWord()->fGetAttri(LADef_numInteger) && 
            ( pWord->fIsWordChar(SC_CHAR_DUNHAO) || 
              pWord->fIsWordChar(SC_CHAR_ZHI4)) ) {

            m_pWord = pWord->pPrevWord();
            m_pLink->MergeWithNext(m_pWord);
            m_pLink->MergeWithNext(m_pWord);
            m_pWord->SetAttri(LADef_amtApprox);
            //_DUMPLINK(m_pLink, m_pWord);
        }
    }

    m_pWord->SetAttri(LADef_numPercent); // �ٷ��������
    m_pWord->SetAttri(LADef_numNumber);
    //_DUMPLINK(m_pLink, m_pWord);
    return NUM_PROCESSED;
}


inline int CMorph::decDian_Proc()           // ��
{
    CWord*  pWord;
    int     iret = NUM_PROCESSED;

    assert(m_pWord->pNextWord()->fIsWordChar(SC_CHAR_DIAN3));

    if (!m_pWord->pNextWord()->fIsTail()) {
        pWord = m_pWord;
        m_pWord = m_pWord->pNextWord()->pNextWord();
        if (GetNumber() == NUM_PROCESSED &&
            m_pWord->fGetAttri(LADef_numChinese)) {
            // ���� �� ����
            BOOL fTime = m_pWord->fGetAttri(LADef_numInteger);
            m_pLink->MergeWithNext(pWord);
            m_pLink->MergeWithNext(pWord);
            if (fTime && !pWord->fIsTail() && pWord->pNextWord()->fIsWordChar(SC_CHAR_FEN)){
                // Merge with ��: ʱ���
                m_pLink->MergeWithNext(pWord);
                if (!pWord->fIsTail() &&
                    pWord->pNextWord()->fGetAttri(LADef_numChinese)) {

                    m_pWord = pWord->pNextWord();
                    if (NUM_PROCESSED == GetNumber() &&
                        !m_pWord->fIsTail() &&
                        m_pWord->pNextWord()->fIsWordChar(SC_CHAR_MIAO)) {
                        // Merge with ���� + ��
                        m_pLink->MergeWithNext(pWord);
                        m_pLink->MergeWithNext(pWord);
                    }
                }
                pWord->SetAttri(LADef_tmGapMinute);
                pWord->SetAttri(LADef_tmPoint);
                pWord->SetAttri(LADef_posT);
                //_DUMPCASE(m_pLink, pWord, 1001);
            } else {
                if (pWord->fIsWordLastChar(SC_CHAR_SHI2)) { // ��ʮ���ʮ
                    pWord->SetErrID(ERRDef_NUMERIAL);
                } else { // Valid С��
                    pWord->SetAttri(LADef_numDecimal);
                    pWord->SetAttri(LADef_numNumber);
                    pWord->SetAttri(LADef_numChinese);
                    //_DUMPCASE(m_pLink, pWord, 1002);
                }
            }
        } else {
            iret = NUM_UNPROCESS;
            //_DUMPLINK(m_pLink, pWord);
        }
        m_pWord = pWord;
    }
    //_DUMPLINK(m_pLink, m_pWord);
    return iret;
}


inline int CMorph::decFenZhi_Proc()     // ��֮
{
    BOOL fError = FALSE;

    if (!m_pWord->fGetAttri(LADef_numChinese)) {
        fError = TRUE;
        //_DUMPLINK(m_pLink, m_pWord);
    }
    m_pWord = m_pWord->pPrevWord();
    m_pLink->MergeWithNext(m_pWord);
    if (!m_pWord->fIsHead() &&
        m_pWord->pPrevWord()->fGetAttri(LADef_numChinese)) {

        m_pWord = m_pWord->pPrevWord();
        m_pLink->MergeWithNext(m_pWord);
    } else {
        fError = TRUE;
        //_DUMPLINK(m_pLink, m_pWord);
    }
    if (fError) {
        m_pWord->SetErrID(ERRDef_NUMERIAL);
    }
    m_pWord->SetAttri(LADef_numPercent);
    m_pWord->SetAttri(LADef_numNumber);
    return NUM_PROCESSED;
}


inline int CMorph::decBei_Proc()            // ��
{
    m_pLink->MergeWithNext(m_pWord);
    if (!m_pWord->fIsTail() &&
        m_pWord->pNextWord()->fIsWordChar(SC_CHAR_BAN)) { 
        // ������
        m_pLink->MergeWithNext(m_pWord);
        //_DUMPLINK(m_pLink, m_pWord);
    }
    m_pWord->SetAttri(LADef_numPercent);
    m_pWord->SetAttri(LADef_numNumber);
    //_DUMPLINK(m_pLink, m_pWord);
    return NUM_PROCESSED;
}



//  Level 4:
//  Service routines

//  Test 2-char Chinese string, and return whether it is a valid approx number
BOOL CMorph::fValidApproxNum(WCHAR* pwchWord)
{
    static WCHAR* rgszApproxNum[] = { 
                    SC_APXNUM_YILIANG ,    // L"\x4e00\x4e24"   // "һ��"
                    SC_APXNUM_YIER    ,    // L"\x4e00\x4e8c"   // "һ��"
                    SC_APXNUM_QIBA    ,    // L"\x4e03\x516b"   // "�߰�"
                    SC_APXNUM_SANLIANG,    // L"\x4e09\x4e24"   // "����"
                    SC_APXNUM_SANWU   ,    // L"\x4e09\x4e94"   // "����"
                    SC_APXNUM_SANSI   ,    // L"\x4e09\x56db"   // "����"
                    SC_APXNUM_LIANGSAN,    // L"\x4e24\x4e09"   // "����"
                    SC_APXNUM_ERSAN   ,    // L"\x4e8c\x4e09"   // "����"
                    SC_APXNUM_WULIU   ,    // L"\x4e94\x516d"   // "����"
                    SC_APXNUM_SIWU    ,    // L"\x56db\x4e94"   // "����"
                    SC_APXNUM_LIUQI   ,    // L"\x516d\x4e03"   // "����"
                    SC_APXNUM_BAJIU        // L"\x516b\x4e5d"   // "�˾�"
    };
    for (int i = 0; i < sizeof(rgszApproxNum) / sizeof(rgszApproxNum[0]); i++) {
        if (*((DWORD UNALIGNED *)pwchWord) == *((DWORD*)(rgszApproxNum[i]))) {
            return TRUE;
        }
    }
    return FALSE;
}


// Test duplicated conjunction char in the word
BOOL CMorph::fCheckDupChar(CWord* pWord)
{
    if (pWord->pNextWord() == NULL) {
        return FALSE;
    }
    int cw = pWord->cwchLen() - 1;
    for (int i = 0; i < cw; i++) {
        LPWSTR pwChar = pWord->pwchGetText();
        if (pwChar[i] == pwChar[i+1]) {
            return TRUE;
        }
    }
    return FALSE;
}
