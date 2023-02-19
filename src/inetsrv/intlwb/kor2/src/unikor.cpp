// unikor.cpp
// Korean Unicode routines
// Copyright 1998-2000 Microsoft Corp.
//
// Modification History:
//  16 MAR 00  bhshin   porting for WordBreaker from uni_kor.c

#include "stdafx.h"
#include "unikor.h"

#pragma setlocale(".949")

// Hangul Jamo Map
// this table maps from the "conjoining jamo" area (u1100 - u11F9)
// to the compatibility Jamo area (u3131 - u318E)
//
// subtract HANGUL_JAMO_BASE (u1100) before indexing into this table
// make sure the the char is not > HANGUL_JAMO_MAX (u11F9) before indexing
//
// to build the complete Unicode character, add the value from this
// table to HANGUL_xJAMO_PAGE (u3100).
//
// 30JUN99  GaryKac  created
unsigned char g_rgchXJamoMap[] = {
    0x31,       // 1100 - ��
    0x32,       // 1101 - ��
    0x34,       // 1102 - ��
    0x37,       // 1103 - ��
    0x38,       // 1104 - ��
    0x39,       // 1105 - ��
    0x41,       // 1106 - ��
    0x42,       // 1107 - ��
    0x43,       // 1108 - ��
    0x45,       // 1109 - ��
    0x46,       // 110A - ��
    0x47,       // 110B - ��
    0x48,       // 110C - ��
    0x49,       // 110D - ��
    0x4A,       // 110E - ��
    0x4B,       // 110F - ��

    0x4C,       // 1110 - ��
    0x4D,       // 1111 - ��
    0x4E,       // 1112 - ��
    0x64,       // 1113 - ���� - no match, use fill
    0x65,       // 1114 - ����
    0x66,       // 1115 - ����
    0x64,       // 1116 - ���� - no match
    0x64,       // 1117 - ���� - no match
    0x64,       // 1118 - ���� - no match
    0x64,       // 1119 - ���� - no match
    0x40,       // 111A - ��
    0x64,       // 111B - ���� - no match
    0x6E,       // 111C - ����
    0x71,       // 111D - ����
    0x72,       // 111E - ����
    0x64,       // 111F - ���� - no match

    0x73,       // 1120 - ����
    0x44,       // 1121 - ��
    0x74,       // 1122 - ����
    0x75,       // 1123 - ����
    0x64,       // 1124 - ���� - no match
    0x64,       // 1125 - ���� - no match
    0x64,       // 1126 - ���� - no match
    0x76,       // 1127 - ����
    0x64,       // 1128 - ���� - no match
    0x77,       // 1129 - ����
    0x64,       // 112A - ���� - no match
    0x78,       // 112B - ����
    0x79,       // 112C - ����
    0x7A,       // 112D - ����
    0x7B,       // 112E - ����
    0x7C,       // 112F - ����

    0x64,       // 1130 - ���� - no match
    0x64,       // 1131 - ���� - no match
    0x7D,       // 1132 - ����
    0x64,       // 1133 - ������ - no match
    0x64,       // 1134 - ������ - no match
    0x64,       // 1135 - ���� - no match
    0x7E,       // 1136 - ����
    0x64,       // 1137 - ���� - no match
    0x64,       // 1138 - ���� - no match
    0x64,       // 1139 - ���� - no match
    0x64,       // 113A - ���� - no match
    0x64,       // 113B - ���� - no match
    0x64,       // 113C - no match
    0x64,       // 113D - no match
    0x64,       // 113E - no match
    0x64,       // 113F - no match

    0x7F,       // 1140 - ^
    0x64,       // 1141 - ���� - no match
    0x64,       // 1142 - ���� - no match
    0x64,       // 1143 - ���� - no match
    0x64,       // 1144 - ���� - no match
    0x82,       // 1145 - ����
    0x83,       // 1146 - ��^
    0x84,       // 1147 - ����
    0x64,       // 1148 - ���� - no match
    0x64,       // 1149 - ���� - no match
    0x64,       // 114A - ���� - no match
    0x64,       // 114B - ���� - no match
    0x64,       // 114C - �� - no match
    0x64,       // 114D - ���� - no match
    0x64,       // 114E - no match
    0x64,       // 114F - no match

    0x64,       // 1150 - no match
    0x64,       // 1151 - no match
    0x64,       // 1152 - ���� - no match
    0x64,       // 1153 - ���� - no match
    0x64,       // 1154 - no match
    0x64,       // 1155 - no match
    0x64,       // 1156 - ���� - no match
    0x84,       // 1157 - ����
    0x85,       // 1158 - ����
    0x86,       // 1159 - �Ѥ�
    0x64,       // 115A - unused
    0x64,       // 115B - unused
    0x64,       // 115C - unused
    0x64,       // 115D - unused
    0x64,       // 115E - unused
    0x64,       // 115F - fill

    0x64,       // 1160 - fill
    0x4F,       // 1161 - ��
    0x50,       // 1162 - ��
    0x51,       // 1163 - ��
    0x52,       // 1164 - ��
    0x53,       // 1165 - ��
    0x54,       // 1166 - ��
    0x55,       // 1167 - ��
    0x56,       // 1168 - ��
    0x57,       // 1169 - ��
    0x58,       // 116A - ��
    0x59,       // 116B - ��
    0x5A,       // 116C - ��
    0x5B,       // 116D - ��
    0x5C,       // 116E - ��
    0x5D,       // 116F - ��

    0x5E,       // 1170 - ��
    0x5F,       // 1171 - ��
    0x60,       // 1172 - ��
    0x61,       // 1173 - ��
    0x62,       // 1174 - ��
    0x63,       // 1175 - ��
    0x64,       // 1176 - ���� - no match
    0x64,       // 1177 - ���� - no match
    0x64,       // 1178 - ���� - no match
    0x64,       // 1179 - ���� - no match
    0x64,       // 117A - �ä� - no match
    0x64,       // 117B - �ä� - no match
    0x64,       // 117C - �ä� - no match
    0x64,       // 117D - �Ť� - no match
    0x64,       // 117E - �Ť� - no match
    0x64,       // 117F - �Ǥ� - no match

    0x64,       // 1180 -  - no match
    0x64,       // 1181 -  - no match
    0x64,       // 1182 -  - no match
    0x64,       // 1183 -  - no match
    0x87,       // 1184 - �ˤ�
    0x88,       // 1185 - �ˤ�
    0x64,       // 1186 -  - no match
    0x64,       // 1187 -  - no match
    0x89,       // 1188 - �ˤ�
    0x64,       // 1189 -  - no match
    0x64,       // 118A -  - no match
    0x64,       // 118B -  - no match
    0x64,       // 118C -  - no match
    0x64,       // 118D -  - no match
    0x64,       // 118E -  - no match
    0x64,       // 118F -  - no match

    0x64,       // 1190 -  - no match
    0x8A,       // 1191 - �Ф�
    0x8B,       // 1192 - �Ф�
    0x64,       // 1193 -  - no match
    0x8C,       // 1194 - �Ф�
    0x64,       // 1195 -  - no match
    0x64,       // 1196 -  - no match
    0x64,       // 1197 -  - no match
    0x64,       // 1198 -  - no match
    0x64,       // 1199 -  - no match
    0x64,       // 119A -  - no match
    0x64,       // 119B -  - no match
    0x64,       // 119C -  - no match
    0x64,       // 119D -  - no match
    0x8D,       // 119E - .
    0x64,       // 119F -  - no match

    0x64,       // 11A0 - .�� - no match
    0x8E,       // 11A1 - .��
    0x64,       // 11A2 - .. - no match
    0x64,       // 11A3 - unused
    0x64,       // 11A4 - unused
    0x64,       // 11A5 - unused
    0x64,       // 11A6 - unused
    0x64,       // 11A7 - unused
    0x31,       // 11A8 - ��
    0x32,       // 11A9 - ��
    0x33,       // 11AA - ��
    0x34,       // 11AB - ��
    0x35,       // 11AC - ��
    0x36,       // 11AD - ��
    0x37,       // 11AE - ��
    0x39,       // 11AF - ��

    0x3A,       // 11B0 - ��
    0x3B,       // 11B1 - ��
    0x3C,       // 11B2 - ��
    0x3D,       // 11B3 - ��
    0x3E,       // 11B4 - ��
    0x3F,       // 11B5 - ��
    0x40,       // 11B6 - ��
    0x41,       // 11B7 - ��
    0x42,       // 11B8 - ��
    0x44,       // 11B9 - ��
    0x45,       // 11BA - ��
    0x46,       // 11BB - ��
    0x47,       // 11BC - ��
    0x48,       // 11BD - ��
    0x4A,       // 11BE - ��
    0x4B,       // 11BF - ��

    0x4C,       // 11C0 - ��
    0x4D,       // 11C1 - ��
    0x4E,       // 11C2 - ��
    0x64,       // 11C3 - ���� - no match
    0x64,       // 11C4 - ���� - no match
    0x64,       // 11C5 - ���� - no match
    0x66,       // 11C6 - ����
    0x67,       // 11C7 - ����
    0x68,       // 11C8 - ��^
    0x64,       // 11C9 - ���� - no match
    0x64,       // 11CA - ���� - no match
    0x64,       // 11CB - ���� - no match
    0x69,       // 11CC - ����
    0x64,       // 11CD - ���� - no match
    0x6A,       // 11CE - ����
    0x64,       // 11CF - ������ - no match

    0x64,       // 11D0 - ���� - no match
    0x64,       // 11D1 - ���� - no match
    0x64,       // 11D2 - ���� - no match
    0x6B,       // 11D3 - ����
    0x64,       // 11D4 - ���� - no match
    0x64,       // 11D5 - ���� - no match
    0x64,       // 11D6 - ���� - no match
    0x6C,       // 11D7 - ��^
    0x64,       // 11D8 - ���� - no match
    0x6D,       // 11D9 - ���Ѥ�
    0x64,       // 11DA - ���� - no match
    0x64,       // 11DB - ���� - no match
    0x6E,       // 11DC - ����
    0x6F,       // 11DD - ����
    0x64,       // 11DE - ������ - no match
    0x70,       // 11DF - ��^

    0x64,       // 11E0 - ���� - no match
    0x64,       // 11E1 - ���� - no match
    0x71,       // 11E2 - ����
    0x64,       // 11E3 - ���� - no match
    0x64,       // 11E4 - ���� - no match
    0x64,       // 11E5 - ���� - no match
    0x78,       // 11E6 - ����
    0x7A,       // 11E7 - ����
    0x7C,       // 11E8 - ����
    0x64,       // 11E9 - ���� - no match
    0x7D,       // 11EA - ����
    0x7F,       // 11EB - ^
    0x64,       // 11EC - ���� - no match
    0x64,       // 11ED - ������ - no match
    0x80,       // 11EE - ����
    0x64,       // 11EF - ���� - no match

    0x81,       // 11F0 - ��
    0x82,       // 11F1 - ����
    0x83,       // 11F2 - ��^
    0x64,       // 11F3 - ���� - no match
    0x84,       // 11F4 - ����
    0x64,       // 11F5 - ���� - no match
    0x64,       // 11F6 - ���� - no match
    0x64,       // 11F7 - ���� - no match
    0x64,       // 11F8 - ���� - no match
    0x86,       // 11F9 - �Ѥ�
    0x64,       // 11FA - unused
    0x64,       // 11FB - unused
    0x64,       // 11FC - unused
    0x64,       // 11FD - unused
    0x64,       // 11FE - unused
    0x64,       // 11FF - unused
};


// decompose_jamo
//
// break the precomposed hangul syllables into the composite jamo
//
// Parameters:
//  wzDst        -> (WCHAR*) ptr to output buffer
//               <- (WCHAR*) expanded (decomposed) string
//  wzSrc        -> (WCHAR*) input string to expand
//  rgCharInfo   -> (CHAR_INFO*) ptr to CharInfo buffer
//               <- (char*) CharStart info for string
//  wzMaxDst     -> (int) size of output buffer
//
// Note: this code assumes that wzDst is large enough to hold the
// decomposed string.  it should be 3x the size of wzSrc.
//
// Result:
//  (void)
//
// 16MAR00  bhshin   porting for WordBreaker
void
decompose_jamo(WCHAR *wzDst, const WCHAR *wzSrc, CHAR_INFO_REC *rgCharInfo, int nMaxDst)
{
    const WCHAR *pwzS;
    WCHAR *pwzD, wch;
    CHAR_INFO_REC *pCharInfo = rgCharInfo;
    unsigned short nToken = 0;
    
    pwzS = wzSrc;
    pwzD = wzDst;
    for (; *pwzS != L'\0'; pwzS++, nToken++)
    {
        ATLASSERT(nMaxDst > 0);
        
		wch = *pwzS;

        if (fIsHangulSyllable(wch))
        {
            int nIndex = (wch - HANGUL_PRECOMP_BASE);
            int nL, nV, nT;
            WCHAR wchL, wchV, wchT;

            nL = nIndex / (NUM_JUNGSEONG * NUM_JONGSEONG);
            nV = (nIndex % (NUM_JUNGSEONG * NUM_JONGSEONG)) / NUM_JONGSEONG;
            nT = nIndex % NUM_JONGSEONG;

            // output L
            wchL = HANGUL_CHOSEONG + nL;
            *pwzD++ = wchL;
            pCharInfo->nToken = nToken;
            pCharInfo->fValidStart = 1;
            pCharInfo->fValidEnd = 0;
            pCharInfo++;

            // output V
            wchV = HANGUL_JUNGSEONG + nV;
            *pwzD++ = wchV;
            pCharInfo->nToken = nToken;
            pCharInfo->fValidStart = 0;
			if (nT != 0)
	            pCharInfo->fValidEnd = 0;	// 3-char syllable - not a valid end
			else
	            pCharInfo->fValidEnd = 1;	// 2-char syllable - mark end as valid
            pCharInfo++;

            // output T (if present)
            if (nT != 0)
            {
                wchT = HANGUL_JONGSEONG + (nT-1);
                *pwzD++ = wchT;
	            pCharInfo->nToken = nToken;
                pCharInfo->fValidStart = 0;
                pCharInfo->fValidEnd = 1;
                pCharInfo++;
            }
        }
        else
        {
            // just copy over the char
            *pwzD++ = *pwzS;
            pCharInfo->nToken = nToken;
            pCharInfo->fValidStart = 1;
            pCharInfo->fValidEnd = 1;
            pCharInfo++;
        }
    }
    *pwzD = L'\0';
    pCharInfo->nToken = nToken;
    pCharInfo++;
}


// compose_jamo
//
// take the jamo chars and combine them into precomposed forms
//
// Parameters:
//  pwzDst  <- (WCHAR*) human-readable bit string
//  pwzSrc  -> (WCHAR*) string buffer to write output string
//  wzMaxDst -> (int) size of output buffer
//
// Result:
//  (int)  number of chars in output string
//
// 11APR00  bhshin   check output buffer overflow
// 16MAR00  bhshin   porting for WordBreaker
int
compose_jamo(WCHAR *wzDst, const WCHAR *wzSrc, int nMaxDst)
{
    const WCHAR *pwzS;
    WCHAR *pwzD, wchL, wchV, wchT, wchS;
    int nChars=0;

    pwzS = wzSrc;
    pwzD = wzDst;
    for (; *pwzS != L'\0';)
    {
        ATLASSERT(nChars < nMaxDst);

		// output buffer overflow
		if (nChars >= nMaxDst)
		{
			// make output string empty
			*wzDst = L'0';
			return 0;
		}
        
		wchL = *pwzS;
        wchV = *(pwzS+1);

        // if the L or V aren't valid, consume 1 char and continue
        if (!fIsChoSeong(wchL) || !fIsJungSeong(wchV))
        {
            if (fIsHangulJamo(wchL))
            {
                // convert from conjoining-jamo to compatibility-jamo
                wchS = g_rgchXJamoMap[wchL-HANGUL_JAMO_BASE];
                wchS += HANGUL_xJAMO_PAGE;
                *pwzD++ = wchS;
                pwzS++;
            }
            else
            {
                // just copy over the unknown char
                *pwzD++ = *pwzS++;
            }
            nChars++;
            continue;
        }

        wchL -= HANGUL_CHOSEONG;
        wchV -= HANGUL_JUNGSEONG;
        pwzS += 2;

        // calc (optional) T
        wchT = *pwzS;
        if (!fIsJongSeong(wchT))
            wchT = 0;
        else
        {
            wchT -= (HANGUL_JONGSEONG-1);
            pwzS++;
        }

        wchS = ((wchL * NUM_JUNGSEONG + wchV) * NUM_JONGSEONG) + wchT + HANGUL_PRECOMP_BASE;
        ATLASSERT(fIsHangulSyllable(wchS));
        
        *pwzD++ = wchS;
        nChars++;
    }
    *pwzD = L'\0';

    return nChars;
}

// compose_length
//
// get the composed string length of input decomposed jamo
//
// Parameters:
//  wszInput  <- (const WCHAR*) input decomposed string (NULL terminated)
//
// Result:
//  (int)  number of chars in composed string
//
// 21MAR00  bhshin   created
int 
compose_length(const WCHAR *wszInput)
{
	const WCHAR *pwzInput;
	
	pwzInput = wszInput;
	
	int cch = 0;
	while (*pwzInput != L'\0')
	{
		if (!fIsChoSeong(*pwzInput) && !fIsJongSeong(*pwzInput))
			cch++;

		pwzInput++;
	}

	return cch;
}

// compose_length
//
// get the composed string length of input decomposed jamo
//
// Parameters:
//  wszInput  <- (const WCHAR*) input decomposed string (NULL terminated)
//  cchInput  <- (int) length of input string
//
// Result:
//  (int)  number of chars in composed string
//
// 15MAY00  bhshin   created
int 
compose_length(const WCHAR *wszInput, int cchInput)
{
	const WCHAR *pwzInput;
	
	pwzInput = wszInput;
	
	int cch = 0;
	int idxInput = 0;
	while (*pwzInput != L'\0' && idxInput < cchInput)
	{
		if (!fIsChoSeong(*pwzInput) && !fIsJongSeong(*pwzInput))
			cch++;

		pwzInput++;
		idxInput++;
	}

	return cch;
}


