/*============================================================================
Microsoft Simplified Chinese Proofreading Engine

Microsoft Confidential.
Copyright 1997-1999 Microsoft Corporation. All Rights Reserved.

Component: CMorph
Purpose:    Implement the affix attachment process in the Morph-analysis class.
Notes:      This implementation use table driven, handle prefix and suffix one by one

    How to add a new prefix or suffix handler:
            1. Mark afxPrefix or afxSuffix on the specific lex
            2. Collect all words that the affix can attached in the lexicon,
               and list these words in a separated text file with attribute def header
               name the attri with "pfx..." or "sfx...", and set group as "Morpheme"
            3. Add this attribute into the lex database, and rebuild the lexicon
               and LexProp.h file
            4. Insert the CAfxMap element into the specific mapping array, carefully
               choose the correct insert position in GBK sorting order
            5. Define the interface of affix processing function for the new affix in 
               Morph.h
            6. Implement the affix processing function to handle the attachment and 
               error detection at the correct position in this file.
            7. Insert a new case of the specific attribute ID into the switch block
               in PrefixHandler or SuffixHandler function

Owner:      donghz@microsoft.com
Platform:   Win32
Revise:     First created by: donghz    12/27/97
============================================================================*/
#include "myafx.h"

#include "morph.h"
#include "lexicon.h"
#include "wordlink.h"
//#include "engindbg.h"
#include "scchardef.h"
#include "slmdef.h"

//  Define local constants
#define AFFIX_UNATTACH  0
#define AFFIX_ATTACHED  1
#define AFFIX_ERROR     2


//  Declare local datatypes of structure and global variables

// Define the affix id mapping structure
struct CAfxMap {
    WCHAR   m_wchAffix;
    int     m_iAttriID;
};

// Define the Prefix ID map
static CAfxMap v_rgpfxMap[] = {
        { SC_PFXCHAR_DAI,  LADef_pfxDai  }, // 0x4ee3  // "��"
        { SC_PFXCHAR_WEI3, LADef_pfxWei3 }, // 0x4f2a  // "α"
        { SC_PFXCHAR_ZHUN, LADef_pfxZhun }, // 0x51c6  // "׼"
        { SC_PFXCHAR_FU,   LADef_pfxFu   }, // 0x526f  // "��"
        { SC_PFXCHAR_FAN,  LADef_pfxFan  }, // 0x53cd  // "��"
        { SC_PFXCHAR_XIAO, LADef_pfxXiao }, // 0x5c0f  // "С"
        { SC_PFXCHAR_WEI1, LADef_pfxWei1 }, // 0x5fae  // "΢"
        { SC_PFXCHAR_ZONG, LADef_pfxZong }, // 0x603b  // "��"
        { SC_PFXCHAR_LAO,  LADef_pfxLao  }, // 0x8001  // "��"
        { SC_PFXCHAR_CHAO, LADef_pfxChao }, // 0x8d85  // "��"
        { SC_PFXCHAR_GUO,  LADef_pfxGuo  }, // 0x8fc7  // "��"
        { SC_PFXCHAR_AA,   LADef_pfxAa   }, // 0x963f  // "��"
        { SC_PFXCHAR_FEI,  LADef_pfxFei  }  // 0x975e  // "��"
    };

// Define the suffix ID map
static CAfxMap v_rgsfxMap[] = {
        { SC_SFXCHAR_YE,    LADef_sfxYe     },// 0x4e1a  // "ҵ"
        { SC_SFXCHAR_YI,    LADef_sfxYi     },// 0x4eea  // "��"
        { SC_SFXCHAR_MEN,   LADef_sfxMen    },// 0x4eec  // "��"
        { SC_SFXCHAR_TI,    LADef_sfxTi     },// 0x4f53  // "��"
        { SC_SFXCHAR_ER,    LADef_sfxEr     },// 0x513f  // "��"
        { SC_SFXCHAR_ZHI,   LADef_sfxZhi    },// 0x5236  // "��"
        { SC_SFXCHAR_HUA,   LADef_sfxHua    },// 0x5316  // "��"
        { SC_SFXCHAR_DAN,   LADef_sfxDan    },// 0x5355  // "��"
        { SC_SFXCHAR_YUAN,  LADef_sfxYuan   },// 0x5458  // "Ա"
        { SC_SFXCHAR_PIN,   LADef_sfxPin    },// 0x54c1  // "Ʒ"
        { SC_SFXCHAR_QI,    LADef_sfxQi     },// 0x5668  // "��"
        { SC_SFXCHAR_CHANG, LADef_sfxChang  },// 0x573a  // "��"
        { SC_SFXCHAR_XING2, LADef_sfxXing2  },// 0x578b  // "��"
        { SC_SFXCHAR_DUI,   LADef_sfxDui    },// 0x5806  // "��"
        { SC_SFXCHAR_TOU,   LADef_sfxTou    },// 0x5934  // "ͷ"
        { SC_SFXCHAR_ZI,    LADef_sfxZi     },// 0x5b50  // "��"
        { SC_SFXCHAR_XUE,   LADef_sfxXue    },// 0x5b66  // "ѧ"
        { SC_SFXCHAR_JIA,   LADef_sfxJia    },// 0x5bb6  // "��"  
        { SC_SFXCHAR_SHI4,  LADef_sfxShi4   },// 0x5e02  // "��"
        { SC_SFXCHAR_SHI1,  LADef_sfxShi1   },// 0x5e08  // "ʦ"
        { SC_SFXCHAR_SHI,   LADef_sfxShi    },// 0x5f0f  // "ʽ"
        { SC_SFXCHAR_XING4, LADef_sfxXing4  },// 0x6027  // "��"
        { SC_SFXCHAR_GAN,   LADef_sfxGan    },// 0x611f  // "��"
        { SC_SFXCHAR_FANG,  LADef_sfxFang   },// 0x65b9  // "��"
        { SC_SFXCHAR_JI,    LADef_sfxJi     },// 0x673a  // "��"
        { SC_SFXCHAR_FA,    LADef_sfxFa     },// 0x6cd5  // "��"
        { SC_SFXCHAR_YAN,   LADef_sfxYan    },// 0x708e  // "��"
        { SC_SFXCHAR_LV,    LADef_sfxLv     },// 0x7387  // "��"  
        { SC_SFXCHAR_SHENG, LADef_sfxSheng  },// 0x751f  // "��"
        { SC_SFXCHAR_JIE,   LADef_sfxJie    },// 0x754c  // "��"
        { SC_SFXCHAR_ZHENG, LADef_sfxZheng  },// 0x75c7  // "֢"
        { SC_SFXCHAR_SHENG3,LADef_sfxSheng3 },// 0x7701  // "ʡ"
        { SC_SFXCHAR_LAO,   LADef_sfxLao    },// 0x8001  // "��"
        { SC_SFXCHAR_ZHE,   LADef_sfxZhe    },// 0x8005  // "��"
        { SC_SFXCHAR_TING,  LADef_sfxTing   },// 0x8247  // "ͧ"
        { SC_SFXCHAR_GUAN,  LADef_sfxGuan   },// 0x89c2  // "��"
        { SC_SFXCHAR_LUN,   LADef_sfxLun    },// 0x8bba  // "��"
        { SC_SFXCHAR_ZHANG, LADef_sfxZhang  } // 0x957f  // "��"
    };

#ifdef DEBUG
// Verify the sorting order of the id map of prefix and suffix
static BOOL fVerifyMaps(void)
{
    for (int i = 1; i < sizeof(v_rgpfxMap) / sizeof(CAfxMap); i++) {
        if (v_rgpfxMap[i].m_wchAffix < v_rgpfxMap[i-1].m_wchAffix) {
            return FALSE;
        }
    }
    for (i = 1; i < sizeof(v_rgsfxMap) / sizeof(CAfxMap); i++) {
        if(v_rgsfxMap[i].m_wchAffix < v_rgsfxMap[i-1].m_wchAffix) {
            return FALSE;
        }
    }
    return TRUE;
}
#endif // DEBUG

                   
/*============================================================================
Implement functions of affix attachment
============================================================================*/

// Affix attachment control function
BOOL CMorph::fAffixAttachment()
{
    assert(fVerifyMaps()); // Verify the id map of prefix and suffix in debugging code
    assert(m_iecError == 0); // the error code public field should be cleared
    assert(m_pLink != NULL);

    int iret;

    m_pWord = m_pLink->pGetHead();
    assert (m_pWord != NULL); // error: missing the terminate word node!

    // First pass scan fron left to right, suffix precede to prefix
    for ( ; m_pWord; m_pWord = m_pWord->pNextWord()) {

        if (!m_pWord->fGetAttri(LADef_afxSuffix)) {
            continue;
        }
        if ((iret = SuffixHandler()) == AFFIX_ERROR) {
            assert(0); // runtime error in suffix attachment
            return FALSE;
        } else if (iret == AFFIX_ATTACHED) {
            // suffix attached successfully
            assert(m_pWord->fGetFlag(CWord::WF_REDUCED));
            if (m_pWord->pPrevWord() &&
                m_pWord->pPrevWord()->fGetAttri(LADef_afxPrefix)) {
                // previous word is a prefix, try to attach
                m_pWord = m_pWord->pPrevWord(); // move current word pointer to prev word
                if ((iret = PrefixHandler()) == AFFIX_ERROR) {
                    assert(0); // runtime error in prefix attachment
                    return FALSE;
                } else if (iret == AFFIX_UNATTACH) {
                    assert(m_pWord->pNextWord() != NULL);
                    m_pWord = m_pWord->pNextWord();
                } else {
                }
            } // end of if (prev word is a prefix)
        } else {// end of if AFFIX_ATTACHED
        }
    }

    m_pWord = m_pLink->pGetTail();
    assert (m_pWord != NULL); // error: missing the terminate word node!
    // Second pass scan from right to left, prefix precede to suffix
    for ( m_pWord = m_pWord->pPrevWord(); m_pWord;
          m_pWord = m_pWord->pPrevWord() ) {
        if (!m_pWord->fGetAttri(LADef_afxPrefix)) {
            continue;
        }
        if ((iret = PrefixHandler()) == AFFIX_ERROR) {
            assert(0); // runtime error in prefix attachment
            return FALSE;
        } else if (iret == AFFIX_ATTACHED) { // prefix attached successfully
            assert(m_pWord->fGetFlag(CWord::WF_REDUCED));
            if (m_pWord->pNextWord() &&
                m_pWord->pNextWord()->fGetAttri(LADef_afxSuffix)) {
                // next word is a suffix, try to attach
                m_pWord = m_pWord->pNextWord(); // move current word pointer to next word
                if ((iret = SuffixHandler()) == AFFIX_ERROR) {
                    assert(0); // runtime error in suffix attachment
                    return FALSE;
                } else if (iret == AFFIX_UNATTACH) {
                    assert(m_pWord->pPrevWord() != NULL);
                    m_pWord = m_pWord->pPrevWord();
                } else {
                }
            } // end of if (next word is a suffix)
        } else {
        }
    } 

    return TRUE;
}


/*============================================================================
CMorph::PrefixHandler(void):
    Prefix handler functions:
Returns:
    AFFIX_ATTACHED if attached successfully
    AFFIX_UNATTACH if could not attached
    AFFIX_ERROR if runtime error occurred
============================================================================*/

int CMorph::PrefixHandler(void)
{
    assert(m_pWord->fGetAttri(LADef_afxPrefix));

    int iret = AFFIX_UNATTACH;
    
    switch(GetPrefixID()) {
        case LADef_pfxAa:       // ��
            iret = pfxAa_Proc();
            break;

        case LADef_pfxChao:     // ��
            iret = pfxChao_Proc();
            break;

        case LADef_pfxDai:      // ��
            iret = pfxDai_Proc();
            break;

        case LADef_pfxFan:      // ��
            iret = pfxFan_Proc();
            break;

        case LADef_pfxFei:      // ��
            iret = pfxFei_Proc();
            break;

        case LADef_pfxFu:       // ��
            iret = pfxFu_Proc();
            break;

        case LADef_pfxGuo:      // ��
            iret = pfxGuo_Proc();
            break;

        case LADef_pfxLao:      // ��
            iret = pfxLao_Proc();
            break;

        case LADef_pfxWei1:     // ΢
            iret = pfxWei1_Proc();
            break;

        case LADef_pfxWei3:     // α
            iret = pfxWei3_Proc();
            break;

        case LADef_pfxXiao:     // С
            iret = pfxXiao_Proc();
            break;

        case LADef_pfxZhun:     // ׼
            iret = pfxZhun_Proc();
            break;

        case LADef_pfxZong:     // ��
            iret = pfxZong_Proc();
            break;

        default:
            // error attri id value in lexicon
            assert(0);
            break;
    }
/*  if(iret == AFFIX_ATTACHED) {
        //_DUMPLINK(m_pLink, m_pWord);
        }
*/  return iret;
}


/*============================================================================
CMorph::SuffixHandler(void):
    Suffix handler functions:
Returns:
    AFFIX_ATTACHED if attached successfully
    AFFIX_UNATTACH if could not attached
    AFFIX_ERROR if runtime error occurred
============================================================================*/
int CMorph::SuffixHandler(void)
{
    assert(m_pWord->fGetAttri(LADef_afxSuffix));

    int iret = AFFIX_UNATTACH;
    
    switch(GetSuffixID()) {
        case LADef_sfxZhang:    // ��
            iret = sfxZhang_Proc();
            break;

        case LADef_sfxChang:    // ��
            iret = sfxChang_Proc();
            break;

        case LADef_sfxDan:      // ��
            iret = sfxDan_Proc();
            break;

        case LADef_sfxDui:      // ��
            iret = sfxDui_Proc();
            break;

        case LADef_sfxEr:       // ��
            iret = sfxEr_Proc();
            break;

        case LADef_sfxFa:       // ��
            iret = sfxFa_Proc();
            break;

        case LADef_sfxFang:     // ��
            iret = sfxFang_Proc();
            break;

        case LADef_sfxGan:      // ��
            iret = sfxGan_Proc();
            break;

        case LADef_sfxGuan:     // ��
            iret = sfxGuan_Proc();
            break;

        case LADef_sfxHua:      // ��
            iret = sfxHua_Proc();
            break;

        case LADef_sfxJi:       // ��
            iret = sfxJi_Proc();
            break;

        case LADef_sfxJia:      // ��
            iret = sfxJia_Proc();
            break;

        case LADef_sfxJie:      // ��
            iret = sfxJie_Proc();
            break;

        case LADef_sfxLao:      // ��
            iret = sfxLao_Proc();
            break;

        case LADef_sfxLv:       // ��
            iret = sfxLv_Proc();
            break;

        case LADef_sfxLun:      // ��
            iret = sfxLun_Proc();
            break;

        case LADef_sfxMen:      // ��
            iret = sfxMen_Proc();
            break;

        case LADef_sfxPin:      // Ʒ
            iret = sfxPin_Proc();
            break;

        case LADef_sfxQi:       // ��
            iret = sfxQi_Proc();
            break;

        case LADef_sfxSheng:    // ��
            iret = sfxSheng_Proc();
            break;

        case LADef_sfxSheng3:   // ʡ
            iret = sfxSheng3_Proc();
            break;

        case LADef_sfxShi1:     // ʦ
            iret = sfxShi1_Proc();
            break;

        case LADef_sfxShi4:     // ��
            iret = sfxShi4_Proc();
            break;

        case LADef_sfxShi:      // ʽ
            iret = sfxShi_Proc();
            break;

        case LADef_sfxTi:       // ��
            iret = sfxTi_Proc();
            break;

        case LADef_sfxTing:     // ͧ
            iret = sfxTing_Proc();
            break;

        case LADef_sfxTou:      // ͷ
            iret = sfxTou_Proc();
            break;

        case LADef_sfxXing2:    // ��
            iret = sfxXing2_Proc();
            break;

        case LADef_sfxXing4:    // ��   
            iret = sfxXing4_Proc();
            break;

        case LADef_sfxXue:      // ѧ
            iret = sfxXue_Proc();
            break;

        case LADef_sfxYan:      // ��
            iret = sfxYan_Proc();
            break;

        case LADef_sfxYe:       // ҵ
            iret = sfxYe_Proc();
            break;

        case LADef_sfxYi:       // ��
            iret = sfxYi_Proc();
            break;

        case LADef_sfxYuan:     // Ա
            iret = sfxYuan_Proc();
            break;

        case LADef_sfxZhe:      // ��
            iret = sfxZhe_Proc();
            break;

        case LADef_sfxZheng:    // ֢
            iret = sfxZheng_Proc();
            break;

        case LADef_sfxZhi:      // ��
            iret = sfxZhi_Proc();
            break;

        case LADef_sfxZi:       // ��
            iret = sfxZi_Proc();
            break;

        default:
            // error attri id setting in lexicon
            assert(0);
            break;
        }
/*  if(iret == AFFIX_ATTACHED) {
        //_DUMPLINK(m_pLink, m_pWord);
        }
*/  return iret;
}


/*============================================================================
CMorph::GetPrefixID(void):
    Get Prefix ID
Returns:
    LADef_pfx...
    -1 if m_pWord is not a prefix
============================================================================*/
inline int CMorph::GetPrefixID(void)
{
    // only single char words mark as prefix
    if (!m_pWord->fGetFlag(CWord::WF_CHAR)) {
        assert(0); // all prefix should be single char words
        return -1;
    }

    assert(m_pWord->cwchLen() == 1);

    WCHAR*   pwchWord = m_pWord->pwchGetText();
#ifdef DEBUG
    // Serialize search to verify the result
    int dbg_iret = -1;
    for (int dbg_i =0; dbg_i < sizeof(v_rgpfxMap) / sizeof(CAfxMap); dbg_i++) {
        if (v_rgpfxMap[dbg_i].m_wchAffix == *pwchWord ) {
            dbg_iret = v_rgpfxMap[dbg_i].m_iAttriID;
            break;
        }
    }
    // the attri ID of matched prefix stored in the dbg_iret.
#endif // DEBUG

    // Binary search for the specific prefix
    int lo = 0, hi = (sizeof(v_rgpfxMap) / sizeof(CAfxMap)) - 1, mi, icmp;
    while (lo <= hi) {
        mi = (lo + hi) / 2;
        if ((icmp = *pwchWord - v_rgpfxMap[mi].m_wchAffix) == 0) {
            assert(v_rgpfxMap[mi].m_iAttriID == dbg_iret);
            return v_rgpfxMap[mi].m_iAttriID;
        } else if (icmp < 0) {
            hi = mi - 1;
        } else {
            lo = mi + 1;
        }
    }
    assert(dbg_iret == -1);
    return -1; // can not found the prefix!
}


/*============================================================================
CMorph::GetPrefixID(void):
    Get Suffix ID
Returns:
    LADef_sfx...
    -1 if m_pWord is not a suffix
Remarks: because it's likely to extend the prefix or suffix to more than single char
         words, so I dup the binary search code in two isolated functions.
         I do not like to make the assumption that prefix and suffix MUST share a same
         search mechanism, although they are same till now!
============================================================================*/
inline int CMorph::GetSuffixID(void)
{
    // only single char words mark as suffix
    if (!m_pWord->fGetFlag(CWord::WF_CHAR)) {
        assert(0); // all suffix should be single char words
        return -1;
    }

    assert(m_pWord->cwchLen() == 1);

    WCHAR*   pwchWord = m_pWord->pwchGetText();
#ifdef DEBUG
    // Serialize search to verify the result
    int dbg_iret = -1;
    for (int dbg_i =0; dbg_i < sizeof(v_rgsfxMap) / sizeof(CAfxMap); dbg_i++) {
        if (v_rgsfxMap[dbg_i].m_wchAffix == *pwchWord ) {
            dbg_iret = v_rgsfxMap[dbg_i].m_iAttriID;
            break;
        }
    }
    // the attri ID of matched prefix stored in the dbg_iret.
#endif // DEBUG

    // Binary search for the specific prefix
    int lo = 0, hi = (sizeof(v_rgsfxMap) / sizeof(CAfxMap)) - 1, mi, icmp;
    while (lo <= hi) {
        mi = (lo + hi) / 2;
        if ((icmp = *pwchWord - v_rgsfxMap[mi].m_wchAffix) == 0) {
            assert(v_rgsfxMap[mi].m_iAttriID == dbg_iret);
            return v_rgsfxMap[mi].m_iAttriID;
        } else if (icmp < 0) {
            hi = mi - 1;
        } else {
            lo = mi + 1;
        }
    }
    assert(dbg_iret == -1);
    return -1; // can not found the prefix!
}


/*============================================================================
//  Implement prefix process functions for each prefix
//  For prefix process functions:
//      Return AFFIX_ATTACHED if attached successfully
//      Return AFFIX_UNATTACH if could not attached
//      Return AFFIX_ERROR if runtime error occurred
============================================================================*/
inline int CMorph::pfxAa_Proc(void)     // ��
{
    if (m_pWord->pNextWord()->pNextWord()) {
        if (m_pWord->pNextWord()->fGetAttri(LADef_pfxAa)) {
            m_pLink->MergeWithNext(m_pWord, FALSE);
            m_pWord->SetAttri(LADef_posN);
#ifdef LADef_iwbAltPhr
            m_pWord->SetAttri(LADef_iwbAltPhr);
#endif // LADef_iwbAltPhr
            //_DUMPLINK(m_pLink, m_pWord);
            return AFFIX_ATTACHED;
        }
    }
    //_DUMPLINK(m_pLink, m_pWord);
    return AFFIX_UNATTACH;
}

inline int CMorph::pfxChao_Proc(void)   // ��
{
    if (m_pWord->pNextWord()->pNextWord()) {
        if (m_pWord->pNextWord()->fGetAttri(LADef_pfxChao)) {
            m_pLink->MergeWithNext(m_pWord, FALSE);
            m_pWord->SetAttri(LADef_posB);
#ifdef LADef_iwbAltPhr
            m_pWord->SetAttri(LADef_iwbAltPhr);
#endif // LADef_iwbAltPhr
            //_DUMPLINK(m_pLink, m_pWord);
            return AFFIX_ATTACHED;
        }
    }
    //_DUMPLINK(m_pLink, m_pWord);
    return AFFIX_UNATTACH;
}

inline int CMorph::pfxDai_Proc(void)    // ��
{
    if (m_pWord->pNextWord()->pNextWord()) {
        if (m_pWord->pNextWord()->fGetAttri(LADef_pfxDai)) {
            m_pLink->MergeWithNext(m_pWord, FALSE);
            m_pWord->SetAttri(LADef_posN);
#ifdef LADef_iwbAltPhr
            m_pWord->SetAttri(LADef_iwbAltPhr);
#endif // LADef_iwbAltPhr
            //_DUMPLINK(m_pLink, m_pWord);
            return AFFIX_ATTACHED;
        }
    }
    //_DUMPLINK(m_pLink, m_pWord);
    return AFFIX_UNATTACH;
}

inline int CMorph::pfxFan_Proc(void)    // ��
{
    if (m_pWord->pNextWord()->pNextWord()) {
        if (m_pWord->pNextWord()->fGetAttri(LADef_pfxFan)) {
            m_pLink->MergeWithNext(m_pWord, FALSE);
            m_pWord->SetAttri(LADef_posN);
#ifdef LADef_iwbAltPhr
            m_pWord->SetAttri(LADef_iwbAltPhr);
#endif // LADef_iwbAltPhr
            //_DUMPLINK(m_pLink, m_pWord);
            return AFFIX_ATTACHED;
        }
    }
    //_DUMPLINK(m_pLink, m_pWord);
    return AFFIX_UNATTACH;
}

inline int CMorph::pfxFei_Proc(void)    // ��
{
    if (m_pWord->pNextWord()->pNextWord()) {
        if (m_pWord->pNextWord()->fGetAttri(LADef_pfxFei)) {
            m_pLink->MergeWithNext(m_pWord, FALSE);
            m_pWord->SetAttri(LADef_posB);
#ifdef LADef_iwbAltPhr
            m_pWord->SetAttri(LADef_iwbAltPhr);
#endif // LADef_iwbAltPhr
            //_DUMPLINK(m_pLink, m_pWord);
            return AFFIX_ATTACHED;
        }
    }
    //_DUMPLINK(m_pLink, m_pWord);
    return AFFIX_UNATTACH;
}

inline int CMorph::pfxFu_Proc(void)     // ��
{
    if (m_pWord->pNextWord()->pNextWord()) {
        if (m_pWord->pNextWord()->fGetAttri(LADef_pfxFu)) {
            m_pLink->MergeWithNext(m_pWord, FALSE);
            m_pWord->SetAttri(LADef_posN);
#ifdef LADef_iwbAltPhr
            m_pWord->SetAttri(LADef_iwbAltPhr);
#endif // LADef_iwbAltPhr
            //_DUMPLINK(m_pLink, m_pWord);
            return AFFIX_ATTACHED;
        }
    }
    //_DUMPLINK(m_pLink, m_pWord);
    return AFFIX_UNATTACH;
}

inline int CMorph::pfxGuo_Proc(void)    // ��
{
    if (m_pWord->pNextWord()->pNextWord()) {
        if (m_pWord->pNextWord()->fGetAttri(LADef_pfxGuo)) {
            m_pLink->MergeWithNext(m_pWord, FALSE);
            m_pWord->SetAttri(LADef_posB);
#ifdef LADef_iwbAltPhr
            m_pWord->SetAttri(LADef_iwbAltPhr);
#endif // LADef_iwbAltPhr
            //_DUMPLINK(m_pLink, m_pWord);
            return AFFIX_ATTACHED;
        }
    }
    //_DUMPLINK(m_pLink, m_pWord);
    return AFFIX_UNATTACH;
}

inline int CMorph::pfxLao_Proc(void)    // ��
{
    if (m_pWord->pNextWord()->pNextWord()) {
        if (m_pWord->pNextWord()->fGetAttri(LADef_pfxLao)) {
            m_pLink->MergeWithNext(m_pWord, FALSE);
            m_pWord->SetAttri(LADef_posN);
#ifdef LADef_iwbAltPhr
            m_pWord->SetAttri(LADef_iwbAltPhr);
#endif // LADef_iwbAltPhr
            //_DUMPLINK(m_pLink, m_pWord);
            return AFFIX_ATTACHED;
        }
    }
    //_DUMPLINK(m_pLink, m_pWord);
    return AFFIX_UNATTACH;
}

inline int CMorph::pfxWei1_Proc(void)   // ΢
{
    if (m_pWord->pNextWord()->pNextWord()) {
        if (m_pWord->pNextWord()->fGetAttri(LADef_pfxWei1)) {
            m_pLink->MergeWithNext(m_pWord, FALSE);
            m_pWord->SetAttri(LADef_posN);
#ifdef LADef_iwbAltPhr
            m_pWord->SetAttri(LADef_iwbAltPhr);
#endif // LADef_iwbAltPhr
            //_DUMPLINK(m_pLink, m_pWord);
            return AFFIX_ATTACHED;
        }
    }
    //_DUMPLINK(m_pLink, m_pWord);
    return AFFIX_UNATTACH;
}

inline int CMorph::pfxWei3_Proc(void)   // α
{
    if (m_pWord->pNextWord()->pNextWord()) {
        if (m_pWord->pNextWord()->fGetAttri(LADef_pfxWei3)) {
            m_pLink->MergeWithNext(m_pWord, FALSE);
            m_pWord->SetAttri(LADef_posN);
#ifdef LADef_iwbAltPhr
            m_pWord->SetAttri(LADef_iwbAltPhr);
#endif // LADef_iwbAltPhr
            //_DUMPLINK(m_pLink, m_pWord);
            return AFFIX_ATTACHED;
        }
    }
    //_DUMPLINK(m_pLink, m_pWord);
    return AFFIX_UNATTACH;
}

inline int CMorph::pfxXiao_Proc(void)   // С
{
    if (m_pWord->pNextWord()->pNextWord()) {
        if (m_pWord->pNextWord()->fGetAttri(LADef_pfxXiao)) {
            m_pLink->MergeWithNext(m_pWord, FALSE);
            m_pWord->SetAttri(LADef_posN);
#ifdef LADef_iwbAltPhr
            m_pWord->SetAttri(LADef_iwbAltPhr);
#endif // LADef_iwbAltPhr
            //_DUMPLINK(m_pLink, m_pWord);
            return AFFIX_ATTACHED;
        }
    }
    //_DUMPLINK(m_pLink, m_pWord);
    return AFFIX_UNATTACH;
}

inline int CMorph::pfxZhun_Proc(void)   // ׼
{
    if (m_pWord->pNextWord()->pNextWord()) {
        if (m_pWord->pNextWord()->fGetAttri(LADef_pfxZhun)) {
            m_pLink->MergeWithNext(m_pWord, FALSE);
            m_pWord->SetAttri(LADef_posN);
#ifdef LADef_iwbAltPhr
            m_pWord->SetAttri(LADef_iwbAltPhr);
#endif // LADef_iwbAltPhr
            //_DUMPLINK(m_pLink, m_pWord);
            return AFFIX_ATTACHED;
        }
    }
    //_DUMPLINK(m_pLink, m_pWord);
    return AFFIX_UNATTACH;
}

inline int CMorph::pfxZong_Proc(void)   // ��
{
    if (m_pWord->pNextWord()->pNextWord()) {
        if (m_pWord->pNextWord()->fGetAttri(LADef_pfxZong)) {
            m_pLink->MergeWithNext(m_pWord, FALSE);
            m_pWord->SetAttri(LADef_posN);
#ifdef LADef_iwbAltPhr
            m_pWord->SetAttri(LADef_iwbAltPhr);
#endif // LADef_iwbAltPhr
            //_DUMPLINK(m_pLink, m_pWord);
            return AFFIX_ATTACHED;
        }
    }
    //_DUMPLINK(m_pLink, m_pWord);
    return AFFIX_UNATTACH;
}


/*============================================================================
//  Implement suffix process functions for each suffix
//  For suffix process functions:
//      Return AFFIX_ATTACHED if attached successfully
//      Return AFFIX_UNATTACH if could not attached
//      Return AFFIX_ERROR if runtime error occurred
============================================================================*/

inline int CMorph::sfxZhang_Proc(void)      // ��
{
    if (m_pWord->pPrevWord()) {
        if (m_pWord->pPrevWord()->fGetAttri(LADef_sfxZhang)) {
            m_pWord = m_pWord->pPrevWord();
            m_pLink->MergeWithNext(m_pWord, FALSE);
            m_pWord->SetAttri(LADef_posN);
#ifdef LADef_iwbAltPhr
            m_pWord->SetAttri(LADef_iwbAltPhr);
#endif // LADef_iwbAltPhr
            m_pWord->SetWordID(SLMDef_semZhang);
            //_DUMPLINK(m_pLink, m_pWord);
            return AFFIX_ATTACHED;
        }
    }
    //_DUMPLINK(m_pLink, m_pWord);
    return AFFIX_UNATTACH;
}

inline int CMorph::sfxChang_Proc(void)      // ��
{
    if (m_pWord->pPrevWord()) {
        if (m_pWord->pPrevWord()->fGetAttri(LADef_sfxChang)) {
            m_pWord = m_pWord->pPrevWord();
            m_pLink->MergeWithNext(m_pWord, FALSE);
            m_pWord->SetAttri(LADef_posN);
#ifdef LADef_iwbAltPhr
            m_pWord->SetAttri(LADef_iwbAltPhr);
#endif // LADef_iwbAltPhr
            m_pWord->SetWordID(SLMDef_semChang);
            //_DUMPLINK(m_pLink, m_pWord);
            return AFFIX_ATTACHED;
        }
    }
    //_DUMPLINK(m_pLink, m_pWord);
    return AFFIX_UNATTACH;
}

inline int CMorph::sfxDan_Proc(void)        // ��
{
    if (m_pWord->pPrevWord()) {
        if (m_pWord->pPrevWord()->fGetAttri(LADef_sfxDan)) {
            m_pWord = m_pWord->pPrevWord();
            m_pLink->MergeWithNext(m_pWord, FALSE);
            m_pWord->SetAttri(LADef_posN);
#ifdef LADef_iwbAltPhr
            m_pWord->SetAttri(LADef_iwbAltPhr);
#endif // LADef_iwbAltPhr
            m_pWord->SetWordID(SLMDef_semDan);
            //_DUMPLINK(m_pLink, m_pWord);
            return AFFIX_ATTACHED;
        }
    }
    //_DUMPLINK(m_pLink, m_pWord);
    return AFFIX_UNATTACH;
}

inline int CMorph::sfxDui_Proc(void)        // ��
{
    if (m_pWord->pPrevWord()) {
        if (m_pWord->pPrevWord()->fGetAttri(LADef_sfxDui)) {
            m_pWord = m_pWord->pPrevWord();
            m_pLink->MergeWithNext(m_pWord, FALSE);
            m_pWord->SetAttri(LADef_posN);
#ifdef LADef_iwbAltPhr
            m_pWord->SetAttri(LADef_iwbAltPhr);
#endif // LADef_iwbAltPhr
            m_pWord->SetWordID(SLMDef_semDui);
            //_DUMPLINK(m_pLink, m_pWord);
            return AFFIX_ATTACHED;
        }
    }
    //_DUMPLINK(m_pLink, m_pWord);
    return AFFIX_UNATTACH;
}

inline int CMorph::sfxEr_Proc(void)         // ��
{
    if (m_pWord->pPrevWord()) {
        if (m_pWord->pPrevWord()->fGetAttri(LADef_sfxEr)) {
            m_pWord = m_pWord->pPrevWord();
            m_pLink->MergeWithNext(m_pWord, FALSE);
            m_pWord->SetAttri(LADef_posN);
#ifdef LADef_iwbAltPhr
            m_pWord->SetAttri(LADef_iwbAltPhr);
#endif // LADef_iwbAltPhr
            m_pWord->SetWordID(SLMDef_semEr);
            //_DUMPLINK(m_pLink, m_pWord);
            return AFFIX_ATTACHED;
        }
    }
    //_DUMPLINK(m_pLink, m_pWord);
    return AFFIX_UNATTACH;
}

inline int CMorph::sfxFa_Proc(void)         // ��
{
    if (m_pWord->pPrevWord()) {
        if (m_pWord->pPrevWord()->fGetAttri(LADef_sfxFa)) {
            m_pWord = m_pWord->pPrevWord();
            m_pLink->MergeWithNext(m_pWord, FALSE);
            m_pWord->SetAttri(LADef_posN);
#ifdef LADef_iwbAltPhr
            m_pWord->SetAttri(LADef_iwbAltPhr);
#endif // LADef_iwbAltPhr
            m_pWord->SetWordID(SLMDef_semFa);
            //_DUMPLINK(m_pLink, m_pWord);
            return AFFIX_ATTACHED;
        }
    }
    //_DUMPLINK(m_pLink, m_pWord);
    return AFFIX_UNATTACH;
}

inline int CMorph::sfxFang_Proc(void)       // ��
{
    if (m_pWord->pPrevWord()) {
        if (m_pWord->pPrevWord()->fGetAttri(LADef_sfxFang)) {
            m_pWord = m_pWord->pPrevWord();
            m_pLink->MergeWithNext(m_pWord, FALSE);
            m_pWord->SetAttri(LADef_posN);
#ifdef LADef_iwbAltPhr
            m_pWord->SetAttri(LADef_iwbAltPhr);
#endif // LADef_iwbAltPhr
            m_pWord->SetWordID(SLMDef_semFang);
            //_DUMPLINK(m_pLink, m_pWord);
            return AFFIX_ATTACHED;
        }
    }
    //_DUMPLINK(m_pLink, m_pWord);
    return AFFIX_UNATTACH;
}

inline int CMorph::sfxGan_Proc(void)        // ��
{
    if (m_pWord->pPrevWord() == NULL) {
        m_pWord->SetErrID(ERRDef_WORDUSAGE);
        return AFFIX_UNATTACH;
    }
    CWord* pPrev = m_pWord->pPrevWord();
    if (pPrev->fGetAttri(LADef_sfxGan)) {
        m_pWord = pPrev;
        m_pLink->MergeWithNext(m_pWord, FALSE);
        m_pWord->SetAttri(LADef_posN);
#ifdef LADef_iwbAltPhr
            m_pWord->SetAttri(LADef_iwbAltPhr);
#endif // LADef_iwbAltPhr
        m_pWord->SetWordID(SLMDef_semGan);
        //_DUMPLINK(m_pLink, m_pWord);
        return AFFIX_ATTACHED;
    }
    if (pPrev->fIsWordChar(SC_CHAR_ZHI)) { // "֮ ��"
        if (pPrev->pPrevWord() && pPrev->pPrevWord()->cwchLen() >= 2) { // "AB.. ֮ ��"
            m_pWord = pPrev->pPrevWord();
            m_pWord = m_pLink->pRightMerge(m_pWord, 2, FALSE);
            m_pWord->SetAttri(LADef_posN);
#ifdef LADef_iwbAltPhr
            m_pWord->SetAttri(LADef_iwbAltPhr);
#endif // LADef_iwbAltPhr
            m_pWord->SetWordID(SLMDef_semGan);
            //_DUMPLINK(m_pLink, m_pWord);
            return AFFIX_ATTACHED;
        } else { // "��֮�� or A֮��"
            pPrev->SetErrID(ERRDef_WORDUSAGE); // Mark error on "֮"
            //_DUMPLINK(m_pLink, m_pWord);
            return AFFIX_UNATTACH;
        }
    }
    if (pPrev->cwchLen() == 1) {
        if (m_pLex->fIsCharFeature(m_pWord->GetLexHandle(),
                                   LFDef_verbModifier, 
                                   *(pPrev->pwchGetText()) ) ) {
            m_pWord = pPrev;
            m_pLink->MergeWithNext(m_pWord);
            m_pWord->SetAttri(LADef_posV);
            //_DUMPLINK(m_pLink, m_pWord);
            return AFFIX_ATTACHED;
        }
        //_DUMPLINK(m_pLink, m_pWord);
        return AFFIX_UNATTACH;
    }
    // "��"ǰ���÷�����
    // pPrev->m_idErr = ERRDef_WORDUSAGE;
    //_DUMPLINK(m_pLink, m_pWord);
    return AFFIX_UNATTACH;
}

inline int CMorph::sfxGuan_Proc(void)       // ��
{
    if (m_pWord->pPrevWord() != NULL) {
        if (m_pWord->pPrevWord()->fGetAttri(LADef_sfxGuan)) {
            m_pWord = m_pWord->pPrevWord();
            if (m_pWord->pPrevWord() &&
                m_pWord->pPrevWord()->fGetAttri(LADef_sfxGuan)) {
                // <��ѧ ���� ��> <��ѧ ���� ��>
                m_pWord = m_pWord->pPrevWord();
                m_pWord = m_pLink->pRightMerge(m_pWord, 2, FALSE);
            } else {
                m_pLink->MergeWithNext(m_pWord, FALSE);
            }
            m_pWord->SetAttri(LADef_posN);
#ifdef LADef_iwbAltPhr
            m_pWord->SetAttri(LADef_iwbAltPhr);
#endif // LADef_iwbAltPhr
            m_pWord->SetWordID(SLMDef_semGuan);
            //_DUMPLINK(m_pLink, m_pWord);
            return AFFIX_ATTACHED;
        }
    }
    // Mark "��" as posV, will not confuse the proper name identification rules
    m_pWord->SetAttri(LADef_posV);
    //_DUMPLINK(m_pLink, m_pWord);
    return AFFIX_UNATTACH;
}

inline int CMorph::sfxHua_Proc(void)        // ��
{
    if (m_pWord->pPrevWord() != NULL) {
        if (m_pWord->pPrevWord()->fGetAttri(LADef_sfxHua)) {
            m_pWord = m_pWord->pPrevWord();
            m_pLink->MergeWithNext(m_pWord, FALSE);
            m_pWord->SetAttri(LADef_posV);
            m_pWord->SetAttri(LADef_posA);
            m_pWord->SetAttri(LADef_pfxFei); // <�� ... ��>
#ifdef LADef_iwbAltPhr
            m_pWord->SetAttri(LADef_iwbAltPhr);
#endif // LADef_iwbAltPhr
            m_pWord->SetWordID(SLMDef_semHua);
            //_DUMPLINK(m_pLink, m_pWord);
            return AFFIX_ATTACHED;
        } else if (m_pWord->pPrevWord()->fGetAttri(LADef_miscElement)) {
            // ������: ����� ����������
            BOOL fFound = FALSE;
            if (m_pWord->pNextWord() &&
                m_pWord->pNextWord()->fGetAttri(LADef_miscElement)) { // �����

                m_pLink->MergeWithNext(m_pWord);
                fFound = TRUE;
            } else if (m_pWord->pNextWord() &&
                       m_pWord->pNextWord()->fGetAttri(LADef_posM) && 
                       m_pWord->pNextWord()->pNextWord()->pNextWord() &&
                       m_pWord->pNextWord()->pNextWord()->fGetAttri(LADef_miscElement)){
                // ��������
                m_pLink->MergeWithNext(m_pWord);
                m_pLink->MergeWithNext(m_pWord);
                fFound = TRUE;
            }
            if (fFound) {
                m_pWord = m_pWord->pPrevWord();
                if (m_pWord->pPrevWord() &&
                    m_pWord->pPrevWord()->fGetAttri(LADef_posM)){

                    m_pWord = m_pWord->pPrevWord();
                    m_pLink->MergeWithNext(m_pWord);
                }
                m_pLink->MergeWithNext(m_pWord);
                m_pWord->SetAttri(LADef_nounTerm);
                m_pWord->SetAttri(LADef_posN);
                //_DUMPLINK(m_pLink, m_pWord);
                return AFFIX_ATTACHED;
            }
        } else {
        }
    }
    if (m_pWord->pNextWord() &&
        m_pWord->pNextWord()->pNextWord() &&
        m_pWord->pNextWord()->pNextWord()->pNextWord() &&
        m_pWord->pNextWord()->pNextWord()->fIsWordChar(SC_CHAR_WEI)) {
        // Test �� A Ϊ B pattern
        m_pWord = m_pLink->pRightMerge(m_pWord, 3, FALSE);
        m_pWord->SetAttri(LADef_posV);
#ifdef LADef_iwbAltPhr
        m_pWord->SetAttri(LADef_iwbAltPhr);
#endif // LADef_iwbAltPhr
        //_DUMPLINK(m_pLink, m_pWord);
        return AFFIX_ATTACHED;
    }
    //_DUMPLINK(m_pLink, m_pWord);
    return AFFIX_UNATTACH;
}

inline int CMorph::sfxJi_Proc(void)         // ��
{
    if (m_pWord->pPrevWord()) {
        if (m_pWord->pPrevWord()->fGetAttri(LADef_sfxJi)) {
            m_pWord = m_pWord->pPrevWord();
            m_pLink->MergeWithNext(m_pWord, FALSE);
            m_pWord->SetAttri(LADef_posN);
#ifdef LADef_iwbAltPhr
            m_pWord->SetAttri(LADef_iwbAltPhr);
#endif // LADef_iwbAltPhr
            m_pWord->SetWordID(SLMDef_semJi);
            //_DUMPLINK(m_pLink, m_pWord);
            return AFFIX_ATTACHED;
        }
    }
    //_DUMPLINK(m_pLink, m_pWord);
    return AFFIX_UNATTACH;
}

inline int CMorph::sfxJia_Proc(void)        // ��
{
    if (m_pWord->pPrevWord()) {
        if (m_pWord->pPrevWord()->fGetAttri(LADef_sfxJia)) {
            m_pWord = m_pWord->pPrevWord();
            m_pLink->MergeWithNext(m_pWord, FALSE);
            m_pWord->SetAttri(LADef_posN);
#ifdef LADef_iwbAltPhr
            m_pWord->SetAttri(LADef_iwbAltPhr);
#endif // LADef_iwbAltPhr
            m_pWord->SetWordID(SLMDef_semJia);
            //_DUMPLINK(m_pLink, m_pWord);
            return AFFIX_ATTACHED;
        }
    }
    //_DUMPLINK(m_pLink, m_pWord);
    return AFFIX_UNATTACH;
}

inline int CMorph::sfxJie_Proc(void)        // ��
{
    CWord* pPrev = m_pWord->pPrevWord();
    if (pPrev) {
        if (pPrev->fGetAttri(LADef_sfxJie)) {
            m_pWord = pPrev;
            m_pLink->MergeWithNext(m_pWord, FALSE);
            m_pWord->SetAttri(LADef_posN);
#ifdef LADef_iwbAltPhr
            m_pWord->SetAttri(LADef_iwbAltPhr);
#endif // LADef_iwbAltPhr
            m_pWord->SetWordID(SLMDef_semJie);
            //_DUMPLINK(m_pLink, m_pWord);
            return AFFIX_ATTACHED;
        }
#ifndef _CHSWBRKR_DLL_IWORDBREAKER
        if (pPrev->fIsWordChar(SC_CHAR_WEI)) {
            // Try "��...Ϊ��" pattern
            int ilen = 0;
            pPrev = pPrev->pPrevWord();
            for (; ilen < 3 && pPrev; ilen++, pPrev = pPrev->pPrevWord()) {
                if (pPrev->fGetAttri(LADef_punPunct)) {
                    break;
                }
                if (pPrev->fIsWordChar(SC_CHAR_YI3)) { // Matach!!
                    while (pPrev->pNextWord() != m_pWord) {
                        m_pLink->MergeWithNext(pPrev);
                    }
                    m_pWord = pPrev;
                    m_pLink->MergeWithNext(m_pWord);
                    m_pWord->SetAttri(LADef_posP);
                    //_DUMPLINK(m_pLink, m_pWord);
                    return AFFIX_ATTACHED;
                }
            }
        }
#endif // _CHSWBRKR_DLL_IWORDBREAKER
        m_pWord->SetAttri(LADef_posN); // As noun execpt in the proper name.
    } else {
        // Should not be the first word in a sentence
        m_pWord->SetErrID(ERRDef_WORDUSAGE);
    }
    //_DUMPLINK(m_pLink, m_pWord);
    return AFFIX_UNATTACH;
}

inline int CMorph::sfxLao_Proc(void)        // ��
{
    if (m_pWord->pPrevWord()) {
        if (m_pWord->pPrevWord()->fGetAttri(LADef_sfxLao)) {
            m_pWord = m_pWord->pPrevWord();
            m_pLink->MergeWithNext(m_pWord, FALSE);
            m_pWord->SetAttri(LADef_posN);
#ifdef LADef_iwbAltPhr
            m_pWord->SetAttri(LADef_iwbAltPhr);
#endif // LADef_iwbAltPhr
            m_pWord->SetWordID(SLMDef_semLao);
            //_DUMPLINK(m_pLink, m_pWord);
            return AFFIX_ATTACHED;
        }
    }
    //_DUMPLINK(m_pLink, m_pWord);
    return AFFIX_UNATTACH;
}

inline int CMorph::sfxLv_Proc(void)         // ��
{
    if (m_pWord->pPrevWord()) {
        if (m_pWord->pPrevWord()->fGetAttri(LADef_sfxLv)) {
            m_pWord = m_pWord->pPrevWord();
            m_pLink->MergeWithNext(m_pWord, FALSE);
            m_pWord->SetAttri(LADef_posN);
#ifdef LADef_iwbAltPhr
            m_pWord->SetAttri(LADef_iwbAltPhr);
#endif // LADef_iwbAltPhr
            m_pWord->SetWordID(SLMDef_semLv);
            //_DUMPLINK(m_pLink, m_pWord);
            return AFFIX_ATTACHED;
        }
        // { V(����)+V(����)+�� } or { V(����)+N(����)+�� } ������������´ʷ���
        // ���ִ���, ��Ҫ����ı�ע�����شʵ�
    }
    if (m_pWord->pNextWord() &&
        m_pWord->pNextWord()->fGetFlag(CWord::WF_CHAR) &&
        m_pLex->fIsCharFeature(m_pWord->GetLexHandle(),
                               LFDef_verbObject, 
                               *(m_pWord->pNextWord()->pwchGetText()) ) ) {
        // �� + (~��/~��/~��/~��/~��/~ʦ) ֱ�ӹ��ɶ����ṹ(40M Corpus��δ������)
        m_pLink->MergeWithNext(m_pWord, FALSE);
        m_pWord->SetAttri(LADef_posV);
#ifdef LADef_iwbAltPhr
            m_pWord->SetAttri(LADef_iwbAltPhr);
#endif // LADef_iwbAltPhr
        //_DUMPLINK(m_pLink, m_pWord);
        return AFFIX_ATTACHED;
    }
    //_DUMPLINK(m_pLink, m_pWord);
    return AFFIX_UNATTACH;
}

inline int CMorph::sfxLun_Proc(void)        // ��
{
    if (m_pWord->pPrevWord()) {
        if (m_pWord->pPrevWord()->fGetAttri(LADef_sfxLun)) {
            m_pWord = m_pWord->pPrevWord();
            m_pLink->MergeWithNext(m_pWord, FALSE);
            m_pWord->SetAttri(LADef_posN);
#ifdef LADef_iwbAltPhr
            m_pWord->SetAttri(LADef_iwbAltPhr);
#endif // LADef_iwbAltPhr
            m_pWord->SetWordID(SLMDef_semLun);
            //_DUMPLINK(m_pLink, m_pWord);
            return AFFIX_ATTACHED;
        }
    }
    //_DUMPLINK(m_pLink, m_pWord);
    return AFFIX_UNATTACH;
}

inline int CMorph::sfxMen_Proc(void)        // ��
{
    if (m_pWord->pPrevWord()) {
        if (m_pWord->pPrevWord()->fGetAttri(LADef_sfxMen)) {
            m_pWord = m_pWord->pPrevWord();
            m_pLink->MergeWithNext(m_pWord, FALSE);
            m_pWord->SetAttri(LADef_posN);
#ifdef LADef_iwbAltPhr
            m_pWord->SetAttri(LADef_iwbAltPhr);
#endif // LADef_iwbAltPhr
            m_pWord->SetWordID(SLMDef_semMen);
            //_DUMPLINK(m_pLink, m_pWord);
            return AFFIX_ATTACHED;
        }
    }
    //_DUMPLINK(m_pLink, m_pWord);
    return AFFIX_UNATTACH;
}

inline int CMorph::sfxPin_Proc(void)        // Ʒ
{
    if (m_pWord->pPrevWord()) {
        if (m_pWord->pPrevWord()->fGetAttri(LADef_sfxPin)) {
            m_pWord = m_pWord->pPrevWord();
            m_pLink->MergeWithNext(m_pWord, FALSE);
            m_pWord->SetAttri(LADef_posN);
#ifdef LADef_iwbAltPhr
            m_pWord->SetAttri(LADef_iwbAltPhr);
#endif // LADef_iwbAltPhr
            m_pWord->SetWordID(SLMDef_semPin);
            //_DUMPLINK(m_pLink, m_pWord);
            return AFFIX_ATTACHED;
        }
    }
    //_DUMPLINK(m_pLink, m_pWord);
    return AFFIX_UNATTACH;
}

inline int CMorph::sfxQi_Proc(void)         // ��
{
    if (m_pWord->pPrevWord()) {
        if (m_pWord->pPrevWord()->fGetAttri(LADef_sfxQi)) {
            m_pWord = m_pWord->pPrevWord();
            m_pLink->MergeWithNext(m_pWord, FALSE);
            m_pWord->SetAttri(LADef_posN);
#ifdef LADef_iwbAltPhr
            m_pWord->SetAttri(LADef_iwbAltPhr);
#endif // LADef_iwbAltPhr
            m_pWord->SetWordID(SLMDef_semQi);
            //_DUMPLINK(m_pLink, m_pWord);
            return AFFIX_ATTACHED;
        }
    }
    //_DUMPLINK(m_pLink, m_pWord);
    return AFFIX_UNATTACH;
}

inline int CMorph::sfxSheng_Proc(void)      // ��
{
    if (m_pWord->pPrevWord()) {
        if (m_pWord->pPrevWord()->fGetAttri(LADef_sfxSheng)) {
            m_pWord = m_pWord->pPrevWord();
            m_pLink->MergeWithNext(m_pWord, FALSE);
            m_pWord->SetAttri(LADef_posN);
#ifdef LADef_iwbAltPhr
            m_pWord->SetAttri(LADef_iwbAltPhr);
#endif // LADef_iwbAltPhr
            m_pWord->SetWordID(SLMDef_semSheng);
            //_DUMPLINK(m_pLink, m_pWord);
            return AFFIX_ATTACHED;
        }
    }
    //_DUMPLINK(m_pLink, m_pWord);
    return AFFIX_UNATTACH;
}

inline int CMorph::sfxSheng3_Proc(void)     // ʡ
{
    if (m_pWord->pPrevWord()) {
        if (m_pWord->pPrevWord()->fGetAttri(LADef_sfxSheng3)) {
            m_pWord = m_pWord->pPrevWord();
            m_pLink->MergeWithNext(m_pWord, FALSE);
            m_pWord->SetAttri(LADef_posN);
#ifdef LADef_iwbAltPhr
            m_pWord->SetAttri(LADef_iwbAltPhr);
#endif // LADef_iwbAltPhr
            m_pWord->SetWordID(SLMDef_semSheng3);
            //_DUMPLINK(m_pLink, m_pWord);
            return AFFIX_ATTACHED;
        }
    }
    //_DUMPLINK(m_pLink, m_pWord);
    return AFFIX_UNATTACH;
}

inline int CMorph::sfxShi1_Proc(void)       // ʦ
{
    if (m_pWord->pPrevWord()) {
        if (m_pWord->pPrevWord()->fGetAttri(LADef_sfxShi1)) {
            m_pWord = m_pWord->pPrevWord();
            m_pLink->MergeWithNext(m_pWord, FALSE);
            m_pWord->SetAttri(LADef_posN);
#ifdef LADef_iwbAltPhr
            m_pWord->SetAttri(LADef_iwbAltPhr);
#endif // LADef_iwbAltPhr
            m_pWord->SetWordID(SLMDef_semShi1);
            //_DUMPLINK(m_pLink, m_pWord);
            return AFFIX_ATTACHED;
        }
    }
    //_DUMPLINK(m_pLink, m_pWord);
    return AFFIX_UNATTACH;
}

inline int CMorph::sfxShi4_Proc(void)       // ��
{
    if (m_pWord->pPrevWord()) {
        if (m_pWord->pPrevWord()->fGetAttri(LADef_sfxShi4)) {
            m_pWord = m_pWord->pPrevWord();
            m_pLink->MergeWithNext(m_pWord, FALSE);
            m_pWord->SetAttri(LADef_posN);
#ifdef LADef_iwbAltPhr
            m_pWord->SetAttri(LADef_iwbAltPhr);
#endif // LADef_iwbAltPhr
            m_pWord->SetWordID(SLMDef_semShi4);
            //_DUMPLINK(m_pLink, m_pWord);
            return AFFIX_ATTACHED;
        }
    }
    //_DUMPLINK(m_pLink, m_pWord);
    return AFFIX_UNATTACH;
}

inline int CMorph::sfxShi_Proc(void)        // ʽ
{
    if (m_pWord->pPrevWord()) {
        if (m_pWord->pPrevWord()->fGetAttri(LADef_sfxShi)) {
            m_pWord = m_pWord->pPrevWord();
            m_pLink->MergeWithNext(m_pWord, FALSE);
            m_pWord->SetAttri(LADef_posB);
            m_pWord->SetAttri(LADef_pfxFei);
            m_pWord->SetAttri(LADef_pfxChao);
#ifdef LADef_iwbAltPhr
            m_pWord->SetAttri(LADef_iwbAltPhr);
#endif // LADef_iwbAltPhr
            m_pWord->SetWordID(SLMDef_semShi);
            //_DUMPLINK(m_pLink, m_pWord);
            return AFFIX_ATTACHED;
        } else if (m_pWord->pPrevWord()->fIsWordChar(SC_CHAR_YI)) { // " һʽ "
            assert(m_pWord->pNextWord());
            if (m_pWord->pNextWord()->pNextWord() &&
                m_pWord->pNextWord()->fGetFlag(CWord::WF_CHAR) &&
                m_pWord->pNextWord()->fGetAttri(LADef_posM) &&
                m_pWord->pNextWord()->pNextWord()->pNextWord() &&
                m_pWord->pNextWord()->pNextWord()->pNextWord()->fGetFlag(CWord::WF_CHAR) &&
                m_pWord->pNextWord()->pNextWord()->fGetAttri(LADef_posQ) ) {
                // һʽ �� �� pattern 
                m_pWord = m_pWord->pPrevWord();
                m_pWord = m_pLink->pRightMerge(m_pWord, 3, FALSE);
                m_pWord->SetAttri(LADef_posD); // �����Խṹ��״��
                //_DUMPLINK(m_pLink, m_pWord);
#ifdef LADef_iwbAltPhr
                m_pWord->SetAttri(LADef_iwbAltPhr);
#endif // LADef_iwbAltPhr
            } else {
                // "һ ʽ" �ϳ�Ϊ״̬��
                m_pWord = m_pWord->pPrevWord();
                m_pLink->MergeWithNext(m_pWord, FALSE);
                m_pWord->SetAttri(LADef_posZ);
#ifdef LADef_iwbAltPhr
                m_pWord->SetAttri(LADef_iwbAltPhr);
#endif // LADef_iwbAltPhr
                //_DUMPLINK(m_pLink, m_pWord);
            }
            return AFFIX_ATTACHED;
        } else if (fCheckShiQian(m_pWord->pPrevWord())) {
#ifndef _CHSWBRKR_DLL_IWORDBREAKER
        // "ʽ"�ɽ�: ���ʡ������������Žṹ��ר���������������̱���
            do  {
                m_pWord = m_pWord->pPrevWord();
                m_pLink->MergeWithNext(m_pWord);
            } while (m_pWord->pPrevWord() && fCheckShiQian(m_pWord->pPrevWord()));
            m_pWord->SetAttri(LADef_posB);
            m_pWord->SetWordID(SLMDef_semShi);
            //_DUMPLINK(m_pLink, m_pWord);
            return AFFIX_ATTACHED;
#endif // _CHSWBRKR_DLL_IWORDBREAKER
        } else {
        }
    }
    //_DUMPLINK(m_pLink, m_pWord);
    return AFFIX_UNATTACH;
}

//  sfxShi_Proc() service function
inline BOOL CMorph::fCheckShiQian(CWord* pWord)
{
    assert(pWord->pNextWord());
    if (pWord->fGetAttri(LADef_posM) ||
        pWord->fGetAttri(LADef_numArabic) ||
        pWord->fGetAttri(LADef_nounPlace) ||
        pWord->fGetAttri(LADef_nounTM) ||
        pWord->fGetAttri(LADef_nounTerm) && pWord->fGetFlag(CWord::WF_QUOTE) ||
        pWord->fGetAttri(LADef_genDBForeign) ||
        pWord->fGetAttri(LADef_nounPerson) ) {

        return TRUE;
    }
    return FALSE;
}

inline int CMorph::sfxTi_Proc(void)         // ��
{
    if (m_pWord->pPrevWord()) {
        if (m_pWord->pPrevWord()->fGetAttri(LADef_sfxTi)) {
            m_pWord = m_pWord->pPrevWord();
            m_pLink->MergeWithNext(m_pWord, FALSE);
            m_pWord->SetAttri(LADef_posN);
#ifdef LADef_iwbAltPhr
            m_pWord->SetAttri(LADef_iwbAltPhr);
#endif // LADef_iwbAltPhr
            m_pWord->SetWordID(SLMDef_semTi);
            //_DUMPLINK(m_pLink, m_pWord);
            return AFFIX_ATTACHED;
        }
    }
    //_DUMPLINK(m_pLink, m_pWord);
    return AFFIX_UNATTACH;
}

inline int CMorph::sfxTing_Proc(void)       // ͧ
{
    if (m_pWord->pPrevWord()) {
        if (m_pWord->pPrevWord()->fGetAttri(LADef_sfxTing)) {
            m_pWord = m_pWord->pPrevWord();
            m_pLink->MergeWithNext(m_pWord, FALSE);
            m_pWord->SetAttri(LADef_posN);
#ifdef LADef_iwbAltPhr
            m_pWord->SetAttri(LADef_iwbAltPhr);
#endif // LADef_iwbAltPhr
            m_pWord->SetWordID(SLMDef_semTing);
            //_DUMPLINK(m_pLink, m_pWord);
            return AFFIX_ATTACHED;
        }
    }
    m_pWord->SetAttri(LADef_posN); // Mark as Noun if could no combind
    //_DUMPLINK(m_pLink, m_pWord);
    return AFFIX_UNATTACH;
}

inline int CMorph::sfxTou_Proc(void)        // ͷ
{
    if (m_pWord->pPrevWord()) {
        if (m_pWord->pPrevWord()->fGetAttri(LADef_sfxTou)) {
            m_pWord = m_pWord->pPrevWord();
            m_pLink->MergeWithNext(m_pWord, FALSE);
            m_pWord->SetAttri(LADef_posN);
#ifdef LADef_iwbAltPhr
            m_pWord->SetAttri(LADef_iwbAltPhr);
#endif // LADef_iwbAltPhr
            m_pWord->SetWordID(SLMDef_semTou);
            //_DUMPLINK(m_pLink, m_pWord);
            return AFFIX_ATTACHED;
        }
    }
    //_DUMPLINK(m_pLink, m_pWord);
    return AFFIX_UNATTACH;
}

inline int CMorph::sfxXing2_Proc(void)      // ��
{
    if (m_pWord->pPrevWord()) {
        if (m_pWord->pPrevWord()->fGetAttri(LADef_sfxXing2)) {
            m_pWord = m_pWord->pPrevWord();
            if (m_pWord->pPrevWord() &&
                m_pWord->pPrevWord()->fGetAttri(LADef_sfxXing2)) {
                // �˴��������ɷ���Ϊ N-X-�� �� A-X-�ͣ�����N��A�ֱ�����������ʺ͵������ݴ�
                // �磺"�� ���� ��", "�� ��ʯ ��" ��
                // �����ԭ����"��"�ṹ����ǰ��ճ��ʽ����!
                m_pWord = m_pWord->pPrevWord();
                m_pWord = m_pLink->pRightMerge(m_pWord, 2, FALSE);
                //_DUMPLINK(m_pLink, m_pWord);
            } else {
                m_pLink->MergeWithNext(m_pWord, FALSE);
            }
            m_pWord->SetAttri(LADef_posB);
            m_pWord->SetAttri(LADef_pfxFei);
            m_pWord->SetAttri(LADef_pfxChao);
#ifdef LADef_iwbAltPhr
            m_pWord->SetAttri(LADef_iwbAltPhr);
#endif // LADef_iwbAltPhr
            m_pWord->SetWordID(SLMDef_semXing2);
            // m_pWord->SetAttri(LADef_pfxQuan); "ȫ"
            // m_pWord->SetAttri(LADef_...); ����ǰ��ճ��ʽ����
            //_DUMPLINK(m_pLink, m_pWord);
            return AFFIX_ATTACHED;
        } else if (fCheckXingQian(m_pWord->pPrevWord())) {
        // "��"�ɽ�:���ʡ������������Žṹ��ר���������������̱���
#ifndef _CHSWBRKR_DLL_IWORDBREAKER
            do  {
                m_pWord = m_pWord->pPrevWord();
                m_pLink->MergeWithNext(m_pWord);
            } while(m_pWord->pPrevWord() && fCheckXingQian(m_pWord->pPrevWord()));
            m_pWord->SetAttri(LADef_posB);
            m_pWord->SetWordID(SLMDef_semXing2);
            // ��ָ�ͺ���40M IME ������δ��ǰ׺�����÷�!
            // m_pWord->SetAttri(LADef_...); ��ǰ��ճ��ʽ����, 
            // !!!���ר�����Ͳ�ͬ, �൱��ǰ��ִʣ����Ǻ���ר���÷�����һ�ص㣨ճ��ͬλ�ɷ֣�
            //_DUMPLINK(m_pLink, m_pWord);
            return AFFIX_ATTACHED;
#endif // _CHSWBRKR_DLL_IWORDBREAKER
        }
    }
    //_DUMPLINK(m_pLink, m_pWord);
    return AFFIX_UNATTACH;
}

//  sfxXing2_Proc() service function
inline BOOL CMorph::fCheckXingQian(CWord* pWord)
{
    assert(pWord->pNextWord());

    if (pWord->fGetAttri(LADef_posM) ||
        pWord->fGetAttri(LADef_numArabic) ||
        pWord->fGetAttri(LADef_nounPlace) ||
        pWord->fGetAttri(LADef_nounTM) ||
        pWord->fGetAttri(LADef_nounTerm) && pWord->fGetFlag(CWord::WF_QUOTE) ||
        pWord->fGetAttri(LADef_genDBForeign) ||
        pWord->fGetAttri(LADef_nounPerson) ) {

        return TRUE;
    }
    return FALSE;
}

inline int CMorph::sfxXing4_Proc(void)      // ��
{
    if (m_pWord->pPrevWord()) {
        if (m_pWord->pPrevWord()->fGetAttri(LADef_sfxXing4)) {
            m_pWord = m_pWord->pPrevWord();
            m_pLink->MergeWithNext(m_pWord, FALSE);
            m_pWord->SetAttri(LADef_posN);
#ifdef LADef_iwbAltPhr
            m_pWord->SetAttri(LADef_iwbAltPhr);
#endif // LADef_iwbAltPhr
            m_pWord->SetWordID(SLMDef_semXing4);
            //_DUMPLINK(m_pLink, m_pWord);
            return AFFIX_ATTACHED;
        }
    }
    //_DUMPLINK(m_pLink, m_pWord);
    return AFFIX_UNATTACH;
}

inline int CMorph::sfxXue_Proc(void)        // ѧ
{
    if (m_pWord->pPrevWord()) {
        if (m_pWord->pPrevWord()->fGetAttri(LADef_sfxXue)) {
            m_pWord = m_pWord->pPrevWord();
            m_pLink->MergeWithNext(m_pWord, FALSE);
            m_pWord->SetAttri(LADef_posN);
#ifdef LADef_iwbAltPhr
            m_pWord->SetAttri(LADef_iwbAltPhr);
#endif // LADef_iwbAltPhr
            m_pWord->SetWordID(SLMDef_semXue);
            //_DUMPLINK(m_pLink, m_pWord);
            return AFFIX_ATTACHED;
        }
    }
    //_DUMPLINK(m_pLink, m_pWord);
    return AFFIX_UNATTACH;
}

inline int CMorph::sfxYan_Proc(void)        // ��
{
    if (m_pWord->pPrevWord()) {
        if (m_pWord->pPrevWord()->fGetAttri(LADef_sfxYan)) {
            m_pWord = m_pWord->pPrevWord();
            m_pLink->MergeWithNext(m_pWord, FALSE);
            m_pWord->SetAttri(LADef_posN);
#ifdef LADef_iwbAltPhr
            m_pWord->SetAttri(LADef_iwbAltPhr);
#endif // LADef_iwbAltPhr
            m_pWord->SetWordID(SLMDef_semYan);
            //_DUMPLINK(m_pLink, m_pWord);
            return AFFIX_ATTACHED;
        }
    }
    //_DUMPLINK(m_pLink, m_pWord);
    return AFFIX_UNATTACH;
}

inline int CMorph::sfxYe_Proc(void)         // ҵ
{
    if (m_pWord->pPrevWord()) {
        if (m_pWord->pPrevWord()->fGetAttri(LADef_sfxYe)) {
            m_pWord = m_pWord->pPrevWord();
            m_pLink->MergeWithNext(m_pWord, FALSE);
            m_pWord->SetAttri(LADef_posN);
#ifdef LADef_iwbAltPhr
            m_pWord->SetAttri(LADef_iwbAltPhr);
#endif // LADef_iwbAltPhr
            m_pWord->SetWordID(SLMDef_semYe);
            //_DUMPLINK(m_pLink, m_pWord);
            return AFFIX_ATTACHED;
        }
    }
    //_DUMPLINK(m_pLink, m_pWord);
    return AFFIX_UNATTACH;
}

inline int CMorph::sfxYi_Proc(void)         // ��
{
    if (m_pWord->pPrevWord()) {
        if (m_pWord->pPrevWord()->fGetAttri(LADef_sfxYi)) {
            m_pWord = m_pWord->pPrevWord();
            m_pLink->MergeWithNext(m_pWord, FALSE);
            m_pWord->SetAttri(LADef_posN);
#ifdef LADef_iwbAltPhr
            m_pWord->SetAttri(LADef_iwbAltPhr);
#endif // LADef_iwbAltPhr
            m_pWord->SetWordID(SLMDef_semYi);
            //_DUMPLINK(m_pLink, m_pWord);
            return AFFIX_ATTACHED;
        }
    }
    //_DUMPLINK(m_pLink, m_pWord);
    return AFFIX_UNATTACH;
}

inline int CMorph::sfxYuan_Proc(void)       // Ա
{
    if (m_pWord->pPrevWord()) {
        if (m_pWord->pPrevWord()->fGetAttri(LADef_sfxYuan)) {
            m_pWord = m_pWord->pPrevWord();
            m_pLink->MergeWithNext(m_pWord, FALSE);
            m_pWord->SetAttri(LADef_posN);
#ifdef LADef_iwbAltPhr
            m_pWord->SetAttri(LADef_iwbAltPhr);
#endif // LADef_iwbAltPhr
            m_pWord->SetWordID(SLMDef_semYuan);
            //_DUMPLINK(m_pLink, m_pWord);
            return AFFIX_ATTACHED;
        }
    }
    //_DUMPLINK(m_pLink, m_pWord);
    return AFFIX_UNATTACH;
}

inline int CMorph::sfxZhe_Proc(void)        // ��
{
    if (m_pWord->pPrevWord()) {
        if (m_pWord->pPrevWord()->fGetAttri(LADef_sfxZhe)) {
            m_pWord = m_pWord->pPrevWord();
            m_pLink->MergeWithNext(m_pWord, FALSE);
            m_pWord->SetAttri(LADef_posN);
#ifdef LADef_iwbAltPhr
            m_pWord->SetAttri(LADef_iwbAltPhr);
#endif // LADef_iwbAltPhr
            m_pWord->SetWordID(SLMDef_semZhe);
            //_DUMPLINK(m_pLink, m_pWord);
            return AFFIX_ATTACHED;
        }
    }
    //_DUMPLINK(m_pLink, m_pWord);
    return AFFIX_UNATTACH;
}

inline int CMorph::sfxZheng_Proc(void)      // ֢
{
    if (m_pWord->pPrevWord()) {
        if (m_pWord->pPrevWord()->fGetAttri(LADef_sfxZheng)) {
            m_pWord = m_pWord->pPrevWord();
            m_pLink->MergeWithNext(m_pWord, FALSE);
            m_pWord->SetAttri(LADef_posN);
#ifdef LADef_iwbAltPhr
            m_pWord->SetAttri(LADef_iwbAltPhr);
#endif // LADef_iwbAltPhr
            m_pWord->SetWordID(SLMDef_semZheng);
            //_DUMPLINK(m_pLink, m_pWord);
            return AFFIX_ATTACHED;
        }
    }
    //_DUMPLINK(m_pLink, m_pWord);
    return AFFIX_UNATTACH;
}

inline int CMorph::sfxZhi_Proc(void)        // ��
{
    if (m_pWord->pPrevWord()) {
        if (m_pWord->pPrevWord()->fGetAttri(LADef_sfxZhi)) {
            m_pWord = m_pWord->pPrevWord();
            m_pLink->MergeWithNext(m_pWord, FALSE);
            m_pWord->SetAttri(LADef_posN);
#ifdef LADef_iwbAltPhr
            m_pWord->SetAttri(LADef_iwbAltPhr);
#endif // LADef_iwbAltPhr
            m_pWord->SetWordID(SLMDef_semZhi);
            //_DUMPLINK(m_pLink, m_pWord);
            return AFFIX_ATTACHED;
        }
    }
    //_DUMPLINK(m_pLink, m_pWord);
    return AFFIX_UNATTACH;
}

inline int CMorph::sfxZi_Proc(void)         // ��
{
    if (m_pWord->pPrevWord()) {
        if (m_pWord->pPrevWord()->fGetAttri(LADef_sfxZi)) {
            m_pWord = m_pWord->pPrevWord();
            m_pLink->MergeWithNext(m_pWord, FALSE);
            m_pWord->SetAttri(LADef_posN);
#ifdef LADef_iwbAltPhr
            m_pWord->SetAttri(LADef_iwbAltPhr);
#endif // LADef_iwbAltPhr
            m_pWord->SetWordID(SLMDef_semZi);
            //_DUMPLINK(m_pLink, m_pWord);
            return AFFIX_ATTACHED;
        }
    }
    //_DUMPLINK(m_pLink, m_pWord);
    return AFFIX_UNATTACH;
}

