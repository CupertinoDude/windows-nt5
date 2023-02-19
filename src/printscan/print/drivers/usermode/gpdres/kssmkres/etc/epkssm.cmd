@rem       1                  "EPSON LQ-2070H (KSSM)"
@rem       2                  "EPSON LQ-2080H (KSSM)"
@rem       3                  "EPSON LQ-2570H (KSSM)"
@rem       4                  "EPSON LQ-2580H (KSSM)"
@rem       5                  "EPSON LQ-570HD (KSSM)"
@rem       6                  "EPSON DLQ-3000H+ (KSSM)"
@rem       7                  "�ﺸ LQ-570HD (KSSM)"
@rem       8                  "�ﺸ LQ-2070H (KSSM)"
@rem       9                  "�ﺸ LQ-2570H (KSSM)"
@rem       10                 "�ﺸ LQ-3000H (KSSM)"

gpc2gpd -S1 -Iepkssm.gpc -M1 -Rkssmkres.dll -Oepl207mk.gpd -N"EPSON LQ-2070H (KSSM)"
gpc2gpd -S1 -Iepkssm.gpc -M2 -Rkssmkres.dll -Oepl208mk.gpd -N"EPSON LQ-2080H (KSSM)"
gpc2gpd -S1 -Iepkssm.gpc -M3 -Rkssmkres.dll -Oepl257mk.gpd -N"EPSON LQ-2570H (KSSM)"
gpc2gpd -S1 -Iepkssm.gpc -M4 -Rkssmkres.dll -Oepl258mk.gpd -N"EPSON LQ-2580H (KSSM)"
gpc2gpd -S1 -Iepkssm.gpc -M5 -Rkssmkres.dll -Oepl570mk.gpd -N"EPSON LQ-570HD (KSSM)"
gpc2gpd -S1 -Iepkssm.gpc -M6 -Rkssmkres.dll -Oepl3khmk.gpd -N"EPSON DLQ-3000H+ (KSSM)"
gpc2gpd -S1 -Iepkssm.gpc -M7 -Rkssmkres.dll -Otgl57hmk.gpd -N"�ﺸ LQ-570HD (KSSM)"
gpc2gpd -S1 -Iepkssm.gpc -M8 -Rkssmkres.dll -Otgl207mk.gpd -N"�ﺸ LQ-2070H (KSSM)"
gpc2gpd -S1 -Iepkssm.gpc -M9 -Rkssmkres.dll -Otgl257mk.gpd -N"�ﺸ LQ-2570H (KSSM)"
gpc2gpd -S1 -Iepkssm.gpc -M10 -Rkssmkres.dll -Otgl300mk.gpd -N"�ﺸ LQ-3000H (KSSM)"

@if exist *.log del *.log

