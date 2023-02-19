/*============================================================================
Microsoft Simplified Chinese Proofreading Engine

Microsoft Confidential.
Copyright 1997-1999 Microsoft Corporation. All Rights Reserved.

Component:  SLMDef
Purpose:    Declare constants and the file structure of Statistical Language Model.
                1. Define the syntactic categories used in SLM.
                2. Define the special WordID, semantic categories in some point of view.
                3. Define the file structure of the runtime WordMatrix.
            This is only a header file w/o any CPP, this header will be included
            by all SLM modules. 
            
Notes:      We drop this file in Engine sub project only because we want to make 
            Engine code self-contained
Owner:      donghz@microsoft.com
Platform:   Win32
Revise:     First created by: donghz    2/6/98
============================================================================*/
#ifndef _SLMDEF_H_
#define _SLMDEF_H_

//  Define the type of WordID
typedef WORD WORDID;

/*============================================================================
Define the syntactic categories used in the SLM.
============================================================================*/

//  Count of syntactic category
#define SLMDef_CountOfSynCat    19

//  All syntactic categories defined in SLMDef_syn prefix
#define SLMDef_synChar      0
#define SLMDef_synVN        1   // ������(׼ν������)��������
#define SLMDef_synVA        2   // �������ݴʼ���
#define SLMDef_synV         3   // ����
#define SLMDef_synAN        4   // ������(׼ν�����ݴ�)����������
#define SLMDef_synA         5   // ���ݴ�
#define SLMDef_synN         6   // ����
#define SLMDef_synT         7   // ʱ���
#define SLMDef_synS         8   // ������
#define SLMDef_synF         9   // ��λ��
#define SLMDef_synM         10  // ����
#define SLMDef_synQ         11  // ���ʼ������ṹ
#define SLMDef_synB         12  // �����
#define SLMDef_synR         13  // ����
#define SLMDef_synZ         14  // ״̬��
#define SLMDef_synD         15  // ����
#define SLMDef_synP         16  // ���
#define SLMDef_synC         17  // ����
#define SLMDef_synMisc      18  // �����ʡ������ʡ����ϰ��(��������)��������


/*============================================================================
Define the special WordID, it stands for the semantic categories in some point of view.
============================================================================*/

//  Count of semantic category (special WordID)
#define SLMDef_CountOfSemCat    55

//  All semantic categories defines in SLMDef_sem prefix
#define SLMDef_semNone      0   // Words non't involved in SLM check
//  ������������
#define SLMDef_semPerson    1   // ����
#define SLMDef_semPlace     2   // ����
#define SLMDef_semOrg       3   // ������
#define SLMDef_semTM        4   // �̱���
#define SLMDef_semTerm      5   // ����ר��
//  ��������
#define SLMDef_semInteger   6   // ����
#define SLMDef_semCode      7   // ����
#define SLMDef_semDecimal   8   // С��
#define SLMDef_semPercent   9   // �������ٷ�������
#define SLMDef_semOrdinal   10  // ����
//  ��������
#define SLMDef_semRRen      11  // �˳ƴ���
//  ��׺������
#define SLMDef_semChang     12  // <��>
#define SLMDef_semDan       13  // <��>
#define SLMDef_semDui       14  // <��>
#define SLMDef_semEr        15  // <��>
#define SLMDef_semFa        16  // <��>
#define SLMDef_semFang      17  // <��>
#define SLMDef_semGan       18  // <��>
#define SLMDef_semGuan      19  // <��>
#define SLMDef_semHua       20  // <��>
#define SLMDef_semJi        21  // <��>
#define SLMDef_semJia       22  // <��>
#define SLMDef_semJie       23  // <��>
#define SLMDef_semLao       24  // <��>
#define SLMDef_semLun       25  // <��>
#define SLMDef_semLv        26  // <��>
#define SLMDef_semMen       27  // <��>
#define SLMDef_semPin       28  // <Ʒ>
#define SLMDef_semQi        29  // <��>
#define SLMDef_semSheng     30  // <��>
#define SLMDef_semSheng3    31  // <ʡ>
#define SLMDef_semShi       32  // <ʽ>
#define SLMDef_semShi1      33  // <ʦ>
#define SLMDef_semShi4      34  // <��>
#define SLMDef_semTi        35  // <��>
#define SLMDef_semTing      36  // <ͧ>
#define SLMDef_semTou       37  // <ͷ>
#define SLMDef_semXing2     38  // <��>
#define SLMDef_semXing4     39  // <��>
#define SLMDef_semXue       40  // <ѧ>
#define SLMDef_semYan       41  // <��>
#define SLMDef_semYe        42  // <ҵ>
#define SLMDef_semYi        43  // <��>
#define SLMDef_semYuan      44  // <Ա>
#define SLMDef_semZhang     45  // <��>
#define SLMDef_semZhe       46  // <��>
#define SLMDef_semZheng     47  // <֢>
#define SLMDef_semZi        48  // <��>
#define SLMDef_semZhi       49  // <��>
//  �ص���Pattern
#define SLMDef_semDup       50  // �ص�
#define SLMDef_semPattern   51  // Pattern
//  ����������
#define SLMDef_semIdiom     52  // ����
#define SLMDef_semPunct     53  // ���(�������﷨��)
#define SLMDef_semMisc      54  // �������ִ�


//------------------------------------------------------------------------------------------
//  Define the file structure of the runtime WordMatrix.
//------------------------------------------------------------------------------------------
#pragma pack(1)
// Define the WordMatrix header
struct CWordMatrixHeader {
    DWORD   m_dwLexVersion;
    DWORD   m_ciWordID;
    DWORD   m_ofbMatrix;        // Start position of the matrix
    DWORD   m_cbMatrix;         // Length of the matrix, only for verification
};

// Define the WordMatrix index item
struct CWordMatrixIndex {
    DWORD   m_ofbMatrix;
    UINT    m_ciLeftNode    : (32 - SLMDef_CountOfSynCat);
    UINT    m_bitLeft       : SLMDef_CountOfSynCat;
    UINT    m_ciRightNode   : (32 - SLMDef_CountOfSynCat);
    UINT    m_bitRight      : SLMDef_CountOfSynCat;
};

// All WordMatrix node listed one by one continuously, no separators between sections

#pragma pack()


#endif  // _SLMDEF_H_