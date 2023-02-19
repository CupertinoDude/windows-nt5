/*============================================================================
Microsoft Simplified Chinese Proofreading Engine

Microsoft Confidential.
Copyright 1997-1999 Microsoft Corporation. All Rights Reserved.

Component: CMorph
Purpose:    Define the morphological analysis on the sentence:
                1. Merge DBCS foreign character string (morph.cpp)
                2. Hnadle punctuation pair check and combind short quotation (morph.cpp)
                3. Resegment on some specific ambiguous words (morph1.cpp)
                4. Binding numerial words (morph2.cpp)
                5. Handle special M+Q usage (morph2.cpp)
                6. Handle affix attachment and usage of some specific words (morph3.cpp)
                7. Identify morphological patterns(Repeat, Pattern and 
                   Separacte words) (morph4.cpp)
                8. Merge 2-char compond verb and noun that are OOV (morph5.cpp)
            Morph-analysis is the first step in the Chinese parsing
Notes:      In order to make the Morphological module easy to manage, this class
            will be implemented in severial cpp files:
                morph.cpp, morph1.cpp, morph2.cpp, morph3.cpp, morph4.cpp, morph5.cpp
            All these cpp files share this header file
Owner:      donghz@microsoft.com
Platform:   Win32
Revise:     First created by: donghz    12/27/97
============================================================================*/
#ifndef _MORPH_H_
#define _MORPH_H_

// Forward declaration of classes
class CLexicon;
class CWordLink;
struct CWord;
struct CWordInfo;

//   Define the CMorph class
class CMorph
{
    public:
        CMorph();
        ~CMorph();

        // Initialize the morph class
        int ecInit(CLexicon* pLexicon);

        // process affix attachment
        int ecDoMorph(CWordLink* pLink, BOOL fAfxAttach = TRUE);

    private:
        int         m_iecError; // runtime error code

        CWordLink*  m_pLink;
        CLexicon*   m_pLex;

        CWord*      m_pWord;

    private:
        // Terminate the Morph class
        void TermMorph(void);

        /*============================================================================
        Private functions for pre-combind process
        ============================================================================*/

        //  Pre-combind process control function.
        //  One pass scan the WordLink and call process functions
        BOOL fPreCombind();

        //  DBForeignHandler combind the conjunctive DB foreign characters
        int DBForeignHandler(void);
        //  Short quotation merge proc
        int QuoteHandler(void);

        /*============================================================================
        In order to handle different operation for different quote marks pair, 
        I use a separate process function for each kind of quote pair
        ============================================================================*/
        int preQuote1_Proc(void);   // �� ��
        int preQuote2_Proc(void);   // �� ��
        int preQuote3_Proc(void);   // �� ��
        int preQuote4_Proc(void);   // �� ��
        int preQuote5_Proc(void);   // �� ��
        int preQuote6_Proc(void);   // �� ��
        int preQuote7_Proc(void);   // �� ��
        int preQuote8_Proc(void);   // �� ��
        int preQuote9_Proc(void);   // �� ��
        int preQuote10_Proc(void);  // �� ��
        
        /*
        *   Common routine to handle �� ���� ���� ���� ���� �ݣ� ��
        *   Merge into one node means will not proofread on the quote text any more!!!
        */
        int preQuoteMerge(WCHAR wchLeft, WCHAR wchRight);


        /*============================================================================
        //  Private functions for adjusting specific kind of ambiguities
        ============================================================================*/
        //  Scan the word link and handle the specific class of words (LADef_genAmbiMorph)
        //  We use table driven again to handle the specific words
        BOOL fAmbiAdjust();

        //  Dispatch the control to specific word processor
        int ResegWordsHandler();

        /*
        *   Following ambi words processors:
        *       Return AMBI_RESEGED if ambi reseg successfully or any error found
        *       Return AMBI_UNRESEG if could not reseg
        *       Return AMBI_ERROR if any error occurred, the error code in m_iecError
        */
        int ambiShiFen_Proc();  // ʮ��
        int ambiZhiYi_Proc();   // ֮һ
        int ambiYiDian_Proc();  // һ��
        int ambiYiShi_Proc();   // һʱ
        int ambiBaDu_Proc();    // �˶�
        int ambiBaiNian_Proc(); // ����
        int ambiWanFen_Proc();  // ���

        //  Break a multi-char words into single-char words and reset their property by
        //  lookup the lexicon char by char. 
        //  Return TRUE if successful, and keep m_pWord point to the first single-char word
        //  Return FALSE if any error occurred
        BOOL fBreakIntoChars();

        //  Lookup the lexicon for the given word node, and reset the lex prop of it.
        //  Return TRUE if the word can be found in the lexicon
        //  Reture FALSE if the word can not be found in the lexicon
        BOOL fRecheckLexInfo(CWord* pWord);


        /*============================================================================
        //  Private functions for Numerical words analysis
        ============================================================================*/
        /*--------- Level 1 ---------*/
        //  Numerical Analysis control function. return TRUE if done
        //  Return FALSE if error occurred, and the error code in m_iecError
        BOOL fNumerialAnalysis();


        /*--------- Level 2 ---------*/
        //  Analysis number word string, check error and mark the class of the merged
        //  number words.
        //  Note: number testing from current word!
        int GetNumber();

        //  �����ʴ���
        int BindOrdinal();
        //  С������������
        int BindDecimal();
        //  �����������������: ��/��/��
        int BindRange();
        
        /*--------- Level 3 ---------*/
        //  Parser for SBCS number called by GetNumber()
        void numSBCSParser(); 
        //  Parser for DBCS Arabic number called by GetNumber()
        void numArabicParser(); 
        //  Parser for DBCS Chinese number called by GetNumber()
        void numChineseParser(); 
        //  Bind ��ɵ�֧ called by GetNumber()
        void numGanZhiHandler();
        
        /*
        *   Following case processors:
        *       Return NUM_PROCESSED if merged successfully or any error found
        *       Return NUM_UNPROCESS if could not merged
        *       Return NUM_ERROR if any error occurred, the error code in m_iecError
        */
        //  Ordinal number processors: called by BindOrdinal()
        int ordDi_Proc();           // ��
        int ordChu_Proc();          // ��

        //  Decimal number processors: called by BindDecimal()
        int decBaiFen_Proc();       // �ٷ�֮, ǧ��֮, ���֮
        int decBei_Proc();          // ��
        int decCheng_Proc();        // ��
        int decDian_Proc();         // ��
        int decFenZhi_Proc();       // ��֮

        /*--------- Level 4 ---------*/
        //  Service routines
        //  Test 2-char Chinese string, and return whether it is a valid approx number
        BOOL fValidApproxNum(WCHAR* pwchWord);
        // Test duplicated conjunction char in the word
        BOOL fCheckDupChar(CWord* pWord);


        //--------------------------------------------------------------------------------
        //  Private functions for affix attachment
        //--------------------------------------------------------------------------------
        //  Affix attachment control function. Return TRUE if done.
        //  Return FALSE if error occurred, and set error code in m_iecError
        BOOL fAffixAttachment();

        /* 
        *   Prefix and suffix handler functions:
        *       Return AFFIX_ATTACHED if attached successfully
        *       Return AFFIX_UNATTACH if could not attached
        *       Return AFFIX_ERROR if runtime error occurred
        */
        int PrefixHandler(void);
        int SuffixHandler(void);
        
        //  Get Prefix ID, return -1 if pWord is not a prefix
        int GetPrefixID(void);
        //  Get Suffix ID, return -1 if pWord is not a suffix
        int GetSuffixID(void);

        /* 
        *   Prefix process functions:
        *       Return AFFIX_ATTACHED if attached successfully
        *       Return AFFIX_UNATTACH if could not attached
        *       Return AFFIX_ERROR if runtime error occurred
        */
        int pfxAa_Proc(void);       // ��
        int pfxChao_Proc(void);     // ��
        int pfxDai_Proc(void);      // ��
        int pfxFan_Proc(void);      // ��
        int pfxFei_Proc(void);      // ��
        int pfxFu_Proc(void);       // ��
        int pfxGuo_Proc(void);      // ��
        int pfxLao_Proc(void);      // ��
        int pfxWei1_Proc(void);     // ΢
        int pfxWei3_Proc(void);     // α
        int pfxXiao_Proc(void);     // С
        int pfxZhun_Proc(void);     // ׼
        int pfxZong_Proc(void);     // ��

        /* 
        *   Suffix process functions:
        *       Return AFFIX_ATTACHED if attached successfully
        *       Return AFFIX_UNATTACH if could not attached
        *       Return AFFIX_ERROR if runtime error occurred
        */
        int sfxZhang_Proc(void);    // ��
        int sfxChang_Proc(void);    // ��
        int sfxDan_Proc(void);      // ��
        int sfxDui_Proc(void);      // ��
        int sfxEr_Proc(void);       // ��
        int sfxFa_Proc(void);       // ��
        int sfxFang_Proc(void);     // ��
        int sfxGan_Proc(void);      // ��
        int sfxGuan_Proc(void);     // ��
        int sfxHua_Proc(void);      // ��
        int sfxJi_Proc(void);       // ��
        int sfxJia_Proc(void);      // ��
        int sfxJie_Proc(void);      // ��
        int sfxLao_Proc(void);      // ��
        int sfxLv_Proc(void);       // ��
        int sfxLun_Proc(void);      // ��
        int sfxMen_Proc(void);      // ��
        int sfxPin_Proc(void);      // Ʒ
        int sfxQi_Proc(void);       // ��
        int sfxSheng_Proc(void);    // ��
        int sfxSheng3_Proc(void);   // ʡ
        int sfxShi1_Proc(void);     // ʦ
        int sfxShi4_Proc(void);     // ��
        int sfxShi_Proc(void);      // ʽ
        int sfxTi_Proc(void);       // ��
        int sfxTing_Proc(void);     // ͧ
        int sfxTou_Proc(void);      // ͷ
        int sfxXing2_Proc(void);    // ��
        int sfxXing4_Proc(void);    // ��
        int sfxXue_Proc(void);      // ѧ
        int sfxYan_Proc(void);      // ��
        int sfxYe_Proc(void);       // ҵ
        int sfxYi_Proc(void);       // ��
        int sfxYuan_Proc(void);     // Ա
        int sfxZhe_Proc(void);      // ��
        int sfxZheng_Proc(void);    // ֢
        int sfxZhi_Proc(void);      // ��
        int sfxZi_Proc(void);       // ��
        
        //  sfxXing2_Proc() service function
        BOOL fCheckXingQian(CWord* pWord);
        //  sfxShi_Proc() service function
        BOOL fCheckShiQian(CWord* pWord);


        /*============================================================================
        //  Private functions for pattern identification
        ============================================================================*/
        /*
        *   Pattern match control function. 
        *   WordLink scan, procedure control and error handling. Return TRUE if finished, 
        *   or FALSE if runtime error, and set error code to m_iecError.
        */
        BOOL fPatternMatch(void);

        // DupHandler: find duplicate cases and call coordinate proc functions
        int DupHandler(void);
        // PatHandler: find pattern and call coordinate proc functions
        int PatHandler(void);
        // SepHandler: find separate word and call coordinate proc functions
        int SepHandler(void);

        // Duplicate word processing functions
        int dupNN_Proc(void);       // *N N
        int dupNAABB_Proc(void);    // A *AB B
        int dupMM_Proc(void);       // *M M
        int dupMABAB_Proc(void);    // *AB AB
        int dupMAABB_Proc(void);    // A *AB B
        int dupQQ_Proc(void);       // *Q Q
        int dupVV_Proc(void);       // *V V
        int dupVABAB_Proc(void);    // *AB AB
        int dupVAABB_Proc(void);    // A *AB B
        int dupVVO_Proc(void);      // V *VO
        int dupAA_Proc(void);       // *A A
        int dupAAABB_Proc(void);    // A *AB B
        int dupAABAB_Proc(void);    // *AB AB
        int dupABB_Proc(void);      // *AB B
        int dupZABAB_Proc(void);    // *AB AB
        int dupDD_Proc(void);       // *D D
        int dupDAABB_Proc(void);    // A *AB B
        int dupDABAB_Proc(void);    // *AB AB

        // Pattern processing functions
        int patV1_Proc(void);       // *V һ V
        int patV2_Proc(void);       // *V �� V
        int patV3_Proc(void);       // *V ��һ V
        int patV4_Proc(void);       // *V �� V ȥ
        int patV5_Proc(void);       // *V �� V ��
        int patA1_Proc(void);       // A �� *AB
        int patD1_Proc(void);       // *D A D B
        int patABuA_Proc(void);     // *V �� V
        int patVMeiV_Proc(void);    // *V û V

        // Separate word processing functions
        int sepVO_Proc(CWord* pBin, CWordInfo* pwinfo); // �������
        int sepVR_Proc(CWord* pJie, CWordInfo* pwinfo); // ����ʽ�������
        int sepVG_Proc(CWord* pQu, CWordInfo* pwinfo);  // ����ʽ�������

};

#endif // _MORPH_H_