@rem       1                  "ť�н� QP180 (���)"
@rem       2                  "ť�н� QP2100 (���)"
@rem       3                  "ť�н� QP2700 (���)"
@rem       4                  "ť�н� QP3100 (���)"
@rem       5                  "ť�н� QP3300 (���)"
@rem       6                  "�ݼ� PRT-5850U (NAIS)"
@rem       7                  "�ﺸ LQ-2500H (���)"
@rem       8                  "�ﺸ GP-2800 (���)"
@rem       9                  "�ﺸ GP-2800P (���)"
@rem       10                 "�ﺸ EP-2000 (������)"

gpc2gpd -S2 -Ihangmang.gpc -M1 -RHANMKRES.DLL -OQXP180HK.GPD -N"ť�н� QP180 (���)"
gpc2gpd -S2 -Ihangmang.gpc -M2 -RHANMKRES.DLL -OQXP210HK.GPD -N"ť�н� QP2100 (���)"
gpc2gpd -S2 -Ihangmang.gpc -M3 -RHANMKRES.DLL -OQXP270HK.GPD -N"ť�н� QP2700 (���)"
gpc2gpd -S2 -Ihangmang.gpc -M4 -RHANMKRES.DLL -OQXP310HK.GPD -N"ť�н� QP3100 (���)"
gpc2gpd -S2 -Ihangmang.gpc -M5 -RHANMKRES.DLL -OQXP330HK.GPD -N"ť�н� QP3300 (���)"
gpc2gpd -S2 -Ihangmang.gpc -M6 -RHANMKRES.DLL -OLGP585HK.GPD -N"�ݼ� PRT-5850U (NAIS)"
gpc2gpd -S2 -Ihangmang.gpc -M7 -RHANMKRES.DLL -OTGL250HK.GPD -N"�ﺸ LQ-2500H (���)"
gpc2gpd -S2 -Ihangmang.gpc -M8 -RHANMKRES.DLL -OTGP280HK.GPD -N"�ﺸ GP-2800 (���)"
gpc2gpd -S2 -Ihangmang.gpc -M9 -RHANMKRES.DLL -OTGP28PHK.GPD -N"�ﺸ GP-2800P (���)"
gpc2gpd -S2 -Ihangmang.gpc -M10 -RHANMKRES.DLL -OTGEP20HK.GPD -N"�ﺸ EP-2000 (������)"


mkgttufm -vac hangmang.rc HANMKRES.cmd > HANMKRES.txt

@rem Create codepage txt file.

@rem Run epap1000.cmd

@rem Create NT5.0's RC file

@rem Change sources to build NT5.0's RC file.

@rem Build.
