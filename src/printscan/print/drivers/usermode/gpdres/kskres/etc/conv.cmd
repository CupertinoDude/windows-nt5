@rem ***** GPD and Font resource conversion
@rem        1                  "�ﺸ LQ-550H (KS)"		TG
@rem        2                  "�ﺸ LQ-570H (KS)"		TG
@rem        3                  "�ﺸ LQ-570H+ (KS)"		TG
@rem        4                  "�ﺸ LQ-850H (KS)"		TG
@rem        5                  "�ﺸ LQ-1270H (KS)"		TG
@rem        6                  "�ﺸ LQ-1550H (KS)"		TG
@rem        7                  "�ﺸ LQ-1550H+ (KS)"		TG
@rem        8                  "�ﺸ LQ-1570H (KS)"		TG
@rem        9                  "LG ���� PRT-2750 (KS)"		LG
@rem        10                 "LG ���� PRT-2950 (KS)"		LG
@rem        11                 "LG ���� PRT-3750N (KS)"		LG
@rem        12                 "LG ���� PRT-3950 (KS)"		LG
@rem        13                 "LG ���� PRT-5750 (KS)"		LG
@rem        14                 "LG ���� PRT-7580 (KS)"		LG
@rem        15                 "LG ���� PRT-7950 (KS)"		LG
@rem        16                 "LG ���� GIP-3000 (KS)"		LG
@rem        17                 "LG ���� GIP-3100+ (KS)"		LG
@rem        18                 "��� DP-2210 (KS)"		DW
@rem        19                 "��� DP-2400 (KS)"		DW
@rem        20                 "��� DP-3400 (KS)"		DW
@rem        21                 "��� DP-3500 (KS)"		DW
@rem        22                 "��� DP-3630E (KS)"		DW
@rem        23                 "��� DP-7200H (KS)"		DW
@rem        24                 "��� DP-8450 (KS)"		DW
@rem        25                 "�Ｚ SP-2411HW (KS)"		SS
@rem        26                 "�Ｚ SP-2412HW (KS)"		SS
@rem        27                 "�Ｚ SP-2417HW (KS)"		SS
@rem        28                 "�Ｚ SP-2421HW (KS)"		SS
@rem        29                 "�Ｚ SP-2422HW (KS)"		SS
@rem        30                 "�Ｚ SP-2433HW (KS)"		SS
@rem        31                 "�Ｚ SP-4011 (KS)"		SS
@rem        32                 "�Ｚ SP-4012 (KS)"		SS
@rem        33                 "�Ｚ SP-4017 (KS)"		SS
@rem        34                 "�Ｚ SP-4021 (KS)"		SS
@rem        35                 "�Ｚ SP-4022 (KS)"		SS
@rem        36                 "�Ｚ SP-4033 (KS)"		SS
@rem        37                 "��������ũ HL-550 (KS)"		HT
@rem        38                 "��������ũ HL-550s (KS)"	HT
@rem        39                 "�ڸ��� ���Ͻ� XP-11 (KS)"	KX
@rem        40                 "�������� JP-B330 (KS)"		JI
@rem        41                 "�������� JP-3300 (KS)"		JI
@rem        42                 "�������� JP-3311 (KS)"		JI
@rem        43                 "�������� JP-3400 (KS)"		JI
@rem        44                 "�������� JP-4500 (KS)"		JI
@rem        45                 "�������� JP-5600 (KS)"		JI
@rem        46                 "ť�н� QP180 (KS)"		QX
@rem        47                 "ť�н� QP2100 (KS)"		QX
@rem        48                 "ť�н� QP2700 (KS)"		QX
@rem        49                 "ť�н� QP3100 (KS)"		QX
@rem        50                 "ť�н� QP3300 (KS)"		QX
@rem        51                 "ť�н� QP4800 (KS)"		QX
@rem        52                 "�ﺸ LQ-2550H (KS)"		TG
@rem        53                 "�ﺸ LQ-3000H (KS)"		TG
@rem        54                 "�ﺸ DLQ-4000H (KS)"		TG
@rem        55                 "�ﺸ SQ-2550H (KS)"		TG
@rem        56                 "�ﺸ SQ-850H (KS)"		TG
@rem        57                 "�Ե� ĳ�� BJ-15k (KS)"		LC
@rem        58                 "ť�н� QP180 (���)"		QX
@rem        59                 "ť�н� QP2100 (���)"		QX
@rem        60                 "ť�н� QP2700 (���)"		QX
@rem        61                 "ť�н� QP3100 (���)"		QX
@rem        62                 "ť�н� QP3300 (���)"		QX
@rem        63                 "LG ���� PRT-5850U (NAIS)"	LG
@rem        64                 "�ﺸ LQ-2500H (���)"		TG
@rem        65                 "�ﺸ GP-2800 (���)"		TG
@rem        66                 "�ﺸ GP-2800P (���)"		TG
@rem        67                 "�ﺸ EP-2000 (������)"		TG

@rem	LC:Lotte Canon
@rem	DW:Daewoo
@rem	HT:Hyper Tech
@rem	JI:Jaeil Precision
@rem	KX:Korea Xerox
@rem	LG:LG Electronics
@rem	QX:Qnix
@rem	SS:Samsung
@rem	TG:Trigem

gpc2gpd -S1 -Ikskres.gpc -M1 -Rkskres.dll -Otgl550kk.gpd -N"�ﺸ LQ-550H (KS)"
gpc2gpd -S1 -Ikskres.gpc -M2 -Rkskres.dll -Otgl570kk.gpd -N"�ﺸ LQ-570H (KS)"
gpc2gpd -S1 -Ikskres.gpc -M3 -Rkskres.dll -Otgl57pkk.gpd -N"�ﺸ LQ-570H+ (KS)"
gpc2gpd -S1 -Ikskres.gpc -M4 -Rkskres.dll -Otgl850kk.gpd -N"�ﺸ LQ-850H (KS)"
gpc2gpd -S1 -Ikskres.gpc -M5 -Rkskres.dll -Otgl127kk.gpd -N"�ﺸ LQ-1270H (KS)"
gpc2gpd -S1 -Ikskres.gpc -M6 -Rkskres.dll -Otgl155kk.gpd -N"�ﺸ LQ-1550H (KS)"
gpc2gpd -S1 -Ikskres.gpc -M7 -Rkskres.dll -Otgl15pkk.gpd -N"�ﺸ LQ-1550H+ (KS)"
gpc2gpd -S1 -Ikskres.gpc -M8 -Rkskres.dll -Otgl157kk.gpd -N"�ﺸ LQ-1570H (KS)"
gpc2gpd -S1 -Ikskres.gpc -M9 -Rkskres.dll -Olgp275kk.gpd -N"LG ���� PRT-2750 (KS)"
gpc2gpd -S1 -Ikskres.gpc -M10 -Rkskres.dll -Olgp295kk.gpd -N"LG ���� PRT-2950 (KS)"
gpc2gpd -S1 -Ikskres.gpc -M11 -Rkskres.dll -Olgp375kk.gpd -N"LG ���� PRT-3750N (KS)"
gpc2gpd -S1 -Ikskres.gpc -M12 -Rkskres.dll -Olgp395kk.gpd -N"LG ���� PRT-3950 (KS)"
gpc2gpd -S1 -Ikskres.gpc -M13 -Rkskres.dll -Olgp575kk.gpd -N"LG ���� PRT-5750 (KS)"
gpc2gpd -S1 -Ikskres.gpc -M14 -Rkskres.dll -Olgp758kk.gpd -N"LG ���� PRT-7580 (KS)"
gpc2gpd -S1 -Ikskres.gpc -M15 -Rkskres.dll -Olgp795kk.gpd -N"LG ���� PRT-7950 (KS)"
gpc2gpd -S1 -Ikskres.gpc -M16 -Rkskres.dll -Olgg300kk.gpd -N"LG ���� GIP-3000 (KS)"
gpc2gpd -S1 -Ikskres.gpc -M17 -Rkskres.dll -Olgg31pkk.gpd -N"LG ���� GIP-3100+ (KS)"
gpc2gpd -S1 -Ikskres.gpc -M18 -Rkskres.dll -Odwd221kk.gpd -N"��� DP-2210 (KS)"
gpc2gpd -S1 -Ikskres.gpc -M19 -Rkskres.dll -Odwd240kk.gpd -N"��� DP-2400 (KS)"
gpc2gpd -S1 -Ikskres.gpc -M20 -Rkskres.dll -Odwd340kk.gpd -N"��� DP-3400 (KS)"
gpc2gpd -S1 -Ikskres.gpc -M21 -Rkskres.dll -Odwd350kk.gpd -N"��� DP-3500 (KS)"
gpc2gpd -S1 -Ikskres.gpc -M22 -Rkskres.dll -Odwd363kk.gpd -N"��� DP-3630E (KS)"
gpc2gpd -S1 -Ikskres.gpc -M23 -Rkskres.dll -Odwd720kk.gpd -N"��� DP-7200H (KS)"
gpc2gpd -S1 -Ikskres.gpc -M24 -Rkskres.dll -Odwd845kk.gpd -N"��� DP-8450 (KS)"
gpc2gpd -S1 -Ikskres.gpc -M25 -Rkskres.dll -Oss2411kk.gpd -N"�Ｚ SP-2411HW (KS)"
gpc2gpd -S1 -Ikskres.gpc -M26 -Rkskres.dll -Oss2412kk.gpd -N"�Ｚ SP-2412HW (KS)"
gpc2gpd -S1 -Ikskres.gpc -M27 -Rkskres.dll -Oss2417kk.gpd -N"�Ｚ SP-2417HW (KS)"
gpc2gpd -S1 -Ikskres.gpc -M28 -Rkskres.dll -Oss2421kk.gpd -N"�Ｚ SP-2421HW (KS)"
gpc2gpd -S1 -Ikskres.gpc -M29 -Rkskres.dll -Oss2422kk.gpd -N"�Ｚ SP-2422HW (KS)"
gpc2gpd -S1 -Ikskres.gpc -M30 -Rkskres.dll -Oss2433kk.gpd -N"�Ｚ SP-2433HW (KS)"
gpc2gpd -S1 -Ikskres.gpc -M31 -Rkskres.dll -Oss4011kk.gpd -N"�Ｚ SP-4011 (KS)"
gpc2gpd -S1 -Ikskres.gpc -M32 -Rkskres.dll -Oss4012kk.gpd -N"�Ｚ SP-4012 (KS)"
gpc2gpd -S1 -Ikskres.gpc -M33 -Rkskres.dll -Oss4017kk.gpd -N"�Ｚ SP-4017 (KS)"
gpc2gpd -S1 -Ikskres.gpc -M34 -Rkskres.dll -Oss4021kk.gpd -N"�Ｚ SP-4021 (KS)"
gpc2gpd -S1 -Ikskres.gpc -M35 -Rkskres.dll -Oss4022kk.gpd -N"�Ｚ SP-4022 (KS)"
gpc2gpd -S1 -Ikskres.gpc -M36 -Rkskres.dll -Oss4033kk.gpd -N"�Ｚ SP-4033 (KS)"
gpc2gpd -S1 -Ikskres.gpc -M37 -Rkskres.dll -Ohth550kk.gpd -N"��������ũ HL-550 (KS)"
gpc2gpd -S1 -Ikskres.gpc -M38 -Rkskres.dll -Ohth55skk.gpd -N"��������ũ HL-550s (KS)"
gpc2gpd -S1 -Ikskres.gpc -M39 -Rkskres.dll -Okxxp11kk.gpd -N"�ڸ��� ���Ͻ� XP-11 (KS)"
gpc2gpd -S1 -Ikskres.gpc -M40 -Rkskres.dll -Ojib330kk.gpd -N"�������� JP-B330 (KS)"
gpc2gpd -S1 -Ikskres.gpc -M40 -Rkskres.dll -Ojib33pkk.gpd -N"�������� JP-B330+ (KS)"
gpc2gpd -S1 -Ikskres.gpc -M41 -Rkskres.dll -Oji3300kk.gpd -N"�������� JP-3300 (KS)"
gpc2gpd -S1 -Ikskres.gpc -M42 -Rkskres.dll -Oji3311kk.gpd -N"�������� JP-3311 (KS)"
gpc2gpd -S1 -Ikskres.gpc -M43 -Rkskres.dll -Oji3400kk.gpd -N"�������� JP-3400 (KS)"
gpc2gpd -S1 -Ikskres.gpc -M44 -Rkskres.dll -Oji4500kk.gpd -N"�������� JP-4500 (KS)"
gpc2gpd -S1 -Ikskres.gpc -M45 -Rkskres.dll -Oji5600kk.gpd -N"�������� JP-5600 (KS)"
gpc2gpd -S1 -Ikskres.gpc -M46 -Rkskres.dll -Oqx180kk.gpd -N"ť�н� QP180 (KS)"
gpc2gpd -S1 -Ikskres.gpc -M47 -Rkskres.dll -Oqx2100kk.gpd -N"ť�н� QP2100 (KS)"
gpc2gpd -S1 -Ikskres.gpc -M48 -Rkskres.dll -Oqx2700kk.gpd -N"ť�н� QP2700 (KS)"
gpc2gpd -S1 -Ikskres.gpc -M49 -Rkskres.dll -Oqx3100kk.gpd -N"ť�н� QP3100 (KS)"
gpc2gpd -S1 -Ikskres.gpc -M50 -Rkskres.dll -Oqx3300kk.gpd -N"ť�н� QP3300 (KS)"
gpc2gpd -S1 -Ikskres.gpc -M51 -Rkskres.dll -Oqx4800kk.gpd -N"ť�н� QP4800 (KS)"
gpc2gpd -S1 -Ikskres.gpc -M52 -Rkskres.dll -Otgl255kk.gpd -N"�ﺸ LQ-2550H (KS)"
gpc2gpd -S1 -Ikskres.gpc -M53 -Rkskres.dll -Otgl300kk.gpd -N"�ﺸ LQ-3000H (KS)"
gpc2gpd -S1 -Ikskres.gpc -M54 -Rkskres.dll -Otgd4khkk.gpd -N"�ﺸ DLQ-4000H (KS)"
gpc2gpd -S1 -Ikskres.gpc -M55 -Rkskres.dll -Otgs255kk.gpd -N"�ﺸ SQ-2550H (KS)"
gpc2gpd -S1 -Ikskres.gpc -M56 -Rkskres.dll -Otgs850kk.gpd -N"�ﺸ SQ-850H (KS)"
gpc2gpd -S1 -Ikskres.gpc -M57 -Rkskres.dll -Olcb15kkk.gpd -N"�Ե� ĳ�� BJ-15k (KS)"
gpc2gpd -S1 -Ikskres.gpc -M58 -Rkskres.dll -Oqx180nkk.gpd -N"ť�н� QP180 (���)"
gpc2gpd -S1 -Ikskres.gpc -M59 -Rkskres.dll -Oqx210nkk.gpd -N"ť�н� QP2100 (���)"
gpc2gpd -S1 -Ikskres.gpc -M60 -Rkskres.dll -Oqx270nkk.gpd -N"ť�н� QP2700 (���)"
gpc2gpd -S1 -Ikskres.gpc -M61 -Rkskres.dll -Oqx310nkk.gpd -N"ť�н� QP3100 (���)"
gpc2gpd -S1 -Ikskres.gpc -M62 -Rkskres.dll -Oqx330nkk.gpd -N"ť�н� QP3300 (���)"
gpc2gpd -S1 -Ikskres.gpc -M63 -Rkskres.dll -Olg585nkk.gpd -N"LG ���� PRT-5850U (NAIS)"
gpc2gpd -S1 -Ikskres.gpc -M64 -Rkskres.dll -Otgl25nkk.gpd -N"�ﺸ LQ-2500H (���)"
gpc2gpd -S1 -Ikskres.gpc -M65 -Rkskres.dll -Otgg28nkk.gpd -N"�ﺸ GP-2800 (���)"
gpc2gpd -S1 -Ikskres.gpc -M66 -Rkskres.dll -Otgg2pnkk.gpd -N"�ﺸ GP-2800P (���)"
gpc2gpd -S1 -Ikskres.gpc -M67 -Rkskres.dll -Otge20nkk.gpd -N"�ﺸ EP-2000 (������)"

@rem mkgttufm -vac kskres.rc3 kskres.cmd > kskres.txt

@rem Create codepage txt file.

@rem Run epap1000.cmd

@rem Create NT5.0's RC file

@rem Change sources to build NT5.0's RC file.

@rem Build.
