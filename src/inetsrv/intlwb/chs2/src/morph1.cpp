/*============================================================================
Microsoft Simplified Chinese Proofreading Engine

Microsoft Confidential.
Copyright 1997-1999 Microsoft Corporation. All Rights Reserved.

Component: CMorph
Purpose:    Implement resegmentation of some secific ambiguous words
Notes:      
Owner:      donghz@microsoft.com
Platform:   Win32
Revise:     First created by: donghz    12/27/97
============================================================================*/
#include "myafx.h"

#include "morph.h"
#include "wordlink.h"
#include "lexicon.h"
#include "scchardef.h"
//#include "engindbg.h"
#include "proofec.h"

#define AMBI_UNRESEG    0
#define AMBI_RESEGED    1
#define AMBI_ERROR      2

/*============================================================================
Private functions for adjusting specific kind of ambiguities
============================================================================*/

/*============================================================================
CMorph::fAmbiAdjust():
    Scan the word link and handle the specific class of words (LADef_genAmbiMorph)
    We use table driven again to handle the specific words
============================================================================*/
BOOL CMorph::fAmbiAdjust()
{
    assert(m_iecError == 0); // the error code public field should be cleared
    assert(m_pLink != NULL);

    m_pWord = m_pLink->pGetHead();
    assert(m_pWord);

    if (m_pWord->fIsTail()) {
        return TRUE;
    }

    // Scan from left to right for pattern match
    for (; m_pWord && m_pWord->pNextWord() != NULL;
           m_pWord = m_pWord->pNextWord()) {
        if (m_pWord->fGetAttri(LADef_genAmbiMorph)) {
            if (ResegWordsHandler() == AMBI_ERROR) {
                return FALSE;
            }
        }
    }
    return TRUE;
}


#define ID_ambiShiFen   1       // ʮ��
#define ID_ambiZhiYi    2       // ֮һ
#define ID_ambiYiDian   3       // һ��
#define ID_ambiYiShi    4       // һʱ
#define ID_ambiBaDu     5       // �˶�
#define ID_ambiBaiNian  6       // ����
#define ID_ambiWanFen   7       // ���

//  Dispatch the control to specific word processor
int CMorph::ResegWordsHandler()
{
    static struct { 
        WCHAR*  m_lpszKey;
        int     m_idEntry;
        } rgResegEntry[] = {
            { SC_WORD_YISHI, ID_ambiYiShi },    // L"\x4e00\x65f6"  // "һʱ"
            { SC_WORD_YIDIAN, ID_ambiYiDian },  // L"\x4e00\x70b9"  // "һ��"
            { SC_WORD_WANFEN, ID_ambiWanFen },  // L"\x4e07\x5206"  // "���"
            { SC_WORD_ZHIYI, ID_ambiZhiYi },    // L"\x4e4b\x4e00"  // "֮һ"
            { SC_WORD_BADU, ID_ambiBaDu },      // L"\x516b\x5ea6"  // "�˶�"
            { SC_WORD_SHIFEN, ID_ambiShiFen },  // L"\x5341\x5206"  // "ʮ��"
            { SC_WORD_BAINIAN, ID_ambiBaiNian } // L"\x767e\x5e74"  // "����"
        };

    assert(m_pWord->fGetAttri(LADef_genAmbiMorph));

    WCHAR*  pwchWord = m_pWord->pwchGetText();
    int     lo = 0, hi = sizeof(rgResegEntry) / sizeof(rgResegEntry[0]) - 1;
    int     mi, icmp = 0;
    int     idEntry = -1;

    while (lo <= hi) {
        mi = (lo + hi) / 2;
        // compare the text
        WCHAR*  pwchKey = rgResegEntry[mi].m_lpszKey;
        int     i = 0;
        while (1) {
            if (pwchKey[i] == L'\0') { 
                if (i < m_pWord->cwchLen()) {
                    icmp = 1; 
                }
                break; 
            }
            if (i >= m_pWord->cwchLen()) {
                icmp = -1;
                break;
            }
            if ((icmp = (pwchWord[i] - pwchKey[i])) != 0) {
                break;
            }
            i++;
        }
        // locate next mid point
        if (icmp < 0) {
            hi = mi - 1;
        } else if (icmp > 0) {
            lo = mi + 1;
        } else { // match!
            idEntry = rgResegEntry[mi].m_idEntry;
            break;
        }
    }
    if (idEntry == -1) {
        assert(0); // Uncovered cases
        return AMBI_UNRESEG;
    }
    // Handle case by case
    switch (idEntry) {
        case ID_ambiShiFen:
            return ambiShiFen_Proc();

        case ID_ambiZhiYi:
            return ambiZhiYi_Proc();

        case ID_ambiYiDian:
            return ambiYiDian_Proc();

        case ID_ambiYiShi:
            return ambiYiShi_Proc();

        case ID_ambiBaDu:       // �˶�
            return ambiBaDu_Proc();

        case ID_ambiBaiNian:    // ����
            return ambiBaiNian_Proc();

        case ID_ambiWanFen:     // ���
            return ambiWanFen_Proc();

        default:
            assert(0);
            break;
    }
    return AMBI_UNRESEG;
}


/*============================================================================
*   Following ambi words processors:
*       Return AMBI_RESEGED if ambi reseg successfully or any error found
*       Return AMBI_UNRESEG if could not reseg
*       Return AMBI_ERROR if any error occurred, the error code in m_iecError
============================================================================*/

inline int CMorph::ambiShiFen_Proc()    // ʮ��
{
    if (!m_pWord->fIsHead() &&
        ( m_pWord->pPrevWord()->fGetAttri(LADef_numChinese) ||
          m_pWord->pPrevWord()->fIsWordChar(SC_CHAR_SHI) ||  // ʱ
          m_pWord->pPrevWord()->fIsWordChar(SC_CHAR_DIAN3)) ) { // ��
        // ��ʾʱ��
        //_DUMPLINK(m_pLink, m_pWord);
    } else if (!m_pWord->fIsTail() && 
               ( m_pWord->pNextWord()->fIsWordText(SC_WORD_ZHIYI) ||   // ֮һ
                 m_pWord->pNextWord()->fIsWordChar(SC_CHAR_ZHI)) ) {   // ֮
        // ��ʾ����
        //_DUMPLINK(m_pLink, m_pWord);
    } else {
        // Used as adv. 
        // Occur 3983 times in 40M Corpus
        //_DUMPLINK(m_pLink, m_pWord);
        return AMBI_UNRESEG;
    }

    // Occur 209 times in 40M Corpus. Re-break it into two single-char words
    if (!fBreakIntoChars()) {
        return AMBI_ERROR;
    }
    return AMBI_RESEGED;
}


inline int CMorph::ambiZhiYi_Proc() // ֮һ
{
    if (!m_pWord->fIsHead() && m_pWord->pPrevWord()->fIsWordChar(SC_CHAR_FEN)){
        if (!fBreakIntoChars()) {
            return AMBI_ERROR;
        }
        // Merge ֮ with previous ��
        m_pWord = m_pWord->pPrevWord();
        m_pLink->MergeWithNext(m_pWord);
        if (!fRecheckLexInfo(m_pWord)) {
            return AMBI_ERROR;
        }
        //_DUMPLINK(m_pLink, m_pWord);
        return AMBI_RESEGED;
    }
    //_DUMPLINK(m_pLink, m_pWord);
    return AMBI_UNRESEG;
}


inline int CMorph::ambiYiDian_Proc()    // һ��
{
    if( !m_pWord->fIsTail() &&
        m_pWord->pNextWord()->fGetAttri(LADef_numChinese) ||
        !m_pWord->fIsHead() &&
        m_pWord->pPrevWord()->fGetAttri(LADef_numChinese) ) {

        if (!fBreakIntoChars()) {
            return AMBI_ERROR;
        }
        //_DUMPLINK(m_pLink, m_pWord);
        return AMBI_RESEGED;
    }
    //_DUMPLINK(m_pLink, m_pWord);
    return AMBI_UNRESEG;
}


inline int CMorph::ambiYiShi_Proc() // һʱ
{
    if (!m_pWord->fIsHead() && 
        ( m_pWord->pPrevWord()->fGetAttri(LADef_numChinese) ||
          m_pWord->pPrevWord()->fGetAttri(LADef_posT) ||
          m_pWord->pPrevWord()->fIsWordChar(SC_CHAR_CHU) ||
          m_pWord->pPrevWord()->fIsWordChar(SC_CHAR_GAO)) ) {
        if (!fBreakIntoChars()) {
            return AMBI_ERROR;
        }
        //_DUMPLINK(m_pLink, m_pWord);
        return AMBI_RESEGED;
    }
    //_DUMPLINK(m_pLink, m_pWord);
    return AMBI_UNRESEG;
}


inline int CMorph::ambiBaDu_Proc()      // �˶�
{
    if (!m_pWord->fIsHead() &&
        m_pWord->pPrevWord()->fGetAttri(LADef_numChinese)) {
        // ���� �˶� => break �˶�
        if (!fBreakIntoChars()) {
            return AMBI_ERROR;
        }
        //_DUMPLINK(m_pLink, m_pWord);
        return AMBI_RESEGED;
    }
    //_DUMPLINK(m_pLink, m_pWord);
    return AMBI_UNRESEG;
}


inline int CMorph::ambiBaiNian_Proc()   // ����
{
    if (!m_pWord->fIsHead() &&
        m_pWord->pPrevWord()->fGetAttri(LADef_numChinese)) {
        // ���� ���� => break ����
        if (!fBreakIntoChars()) {
            return AMBI_ERROR;
        }
        //_DUMPLINK(m_pLink, m_pWord);
        return AMBI_RESEGED;
    }
    //_DUMPLINK(m_pLink, m_pWord);
    return AMBI_UNRESEG;
}


inline int CMorph::ambiWanFen_Proc()    // ���
{
    if (!m_pWord->fIsTail() && 
        ( m_pWord->pNextWord()->fIsWordText(SC_WORD_ZHIYI) ||   // ֮һ
          m_pWord->pNextWord()->fIsWordChar(SC_CHAR_ZHI)) ) {   // ֮
        // ��ʾ����
        if (!fBreakIntoChars()) {
            return AMBI_ERROR;
        }
        //_DUMPLINK(m_pLink, m_pWord);
        return AMBI_RESEGED;
    }
    //_DUMPLINK(m_pLink, m_pWord);
    return AMBI_UNRESEG;
}


/*============================================================================
Service functions for adjusting specific kind of ambiguities
============================================================================*/

/*============================================================================
CMorph::fBreakIntoChars():
    Break a multi-char words into single-char words and reset their property by
    lookup the lexicon char by char. 
Return:
    TRUE if successful, and keep m_pWord point to the first single-char word
    FALSE if any error occurred
============================================================================*/
BOOL CMorph::fBreakIntoChars()
{
    if (m_pWord->fGetFlag(CWord::WF_SBCS) ||
        m_pWord->fGetFlag(CWord::WF_REDUCED)) {
        // Could not rebreak the reduced node or SBCS word node
        assert(0); 
        m_iecError = PRFEC::gecUnknown;
        return FALSE;
    }
    CWord* pWord = m_pWord;
#ifdef DEBUG
    CWord* dbg_pWord = m_pWord->pPrevWord();
#endif // DEBUG
    CWordInfo winfo;
    assert(!pWord->fGetFlag(CWord::WF_CHAR));
    while (!pWord->fGetFlag(CWord::WF_CHAR)) {
        if ((pWord = m_pLink->pSplitWord(pWord, 1)) == NULL) { // OOM in pSplitWord()
            m_iecError = PRFEC::gecOOM;
            return FALSE;
        }
        if (!m_pLex->fGetCharInfo(*(pWord->pwchGetText()), &winfo)) {
            m_iecError = PRFEC::gecUnknown;
            return FALSE;
        }
        // Set the word property
        pWord->SetWordID(winfo.GetWordID());
        pWord->SetLexHandle(winfo.GetLexHandle());
        for (USHORT i = 0; i < winfo.AttriNum(); i++) {
            pWord->SetAttri(winfo.GetAttri(i));
        }
        pWord = pWord->pNextWord();
    }
    // Lookup the last char in the lexicon
    if (!m_pLex->fGetCharInfo(*(pWord->pwchGetText()), &winfo)) {
        m_iecError = PRFEC::gecUnknown;
        return FALSE;
    }
    // Set the word property
    pWord->SetWordID(winfo.GetWordID());
    pWord->SetLexHandle(winfo.GetLexHandle());
    for (USHORT i = 0; i < winfo.AttriNum(); i++) {
        pWord->SetAttri(winfo.GetAttri(i));
    }

#ifdef DEBUG
    assert(dbg_pWord == m_pWord->pPrevWord());
#endif // DEBUG
    
    return TRUE;
}

/*============================================================================
CMorph::fRecheckLexInfo(CWord* pWord):
    Lookup the lexicon for the given word node, and reset the lex prop of it.
Return:
    TRUE if the word can be found in the lexicon
    FALSE if the word can not be found in the lexicon
============================================================================*/
BOOL CMorph::fRecheckLexInfo(CWord* pWord)
{
    assert(pWord->pNextWord());
    assert(!pWord->fGetFlag(CWord::WF_SBCS));

    CWordInfo   winfo;
    USHORT      cwchLen = pWord->cwchLen();
    // Lookup the lexicon for the word
    if (cwchLen != m_pLex->cwchMaxMatch(pWord->pwchGetText(),cwchLen, &winfo)){
        m_iecError = PRFEC::gecUnknown;
        return FALSE;
    }
    // Set the word property
    pWord->SetWordID(winfo.GetWordID());
    pWord->SetLexHandle(winfo.GetLexHandle());
    for (USHORT i = 0; i < winfo.AttriNum(); i++) {
        pWord->SetAttri(winfo.GetAttri(i));
    }
    if (cwchLen == 1) {
        pWord->SetFlag(CWord::WF_CHAR);
    }
    return TRUE;
}
