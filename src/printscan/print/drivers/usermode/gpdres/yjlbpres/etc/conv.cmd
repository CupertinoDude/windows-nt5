@rem ***** GPD and Font resource conversion
@rem       1                  "���� ���ʷ� K100"
@rem       2                  "���� ���ʷ� K200"
@rem       3                  "���� ���ʷ� Y300"
@rem       4                  "���� ���ʷ� Y600"
@rem       5                  "���� ���ʷ� C1600"
@rem       6                  "���� ���ʷ� A344"
@rem       7                  "���� ���ʷ� A344*"
@rem       8                  "���� ���ʷ� Z100"
@rem       9                  "���� ���ʷ� Z200"
gpc2gpd -S2 -Iyjlbp.gpc -M1 -Ryjlbpres.dll -Oygk100k.gpd -N"���� ���ʷ� K100"
gpc2gpd -S2 -Iyjlbp.gpc -M2 -Ryjlbpres.dll -Oygk200k.gpd -N"���� ���ʷ� K200"
gpc2gpd -S2 -Iyjlbp.gpc -M3 -Ryjlbpres.dll -Oygy300k.gpd -N"���� ���ʷ� Y300"
gpc2gpd -S2 -Iyjlbp.gpc -M4 -Ryjlbpres.dll -Oygy600k.gpd -N"���� ���ʷ� Y600"
gpc2gpd -S2 -Iyjlbp.gpc -M5 -Ryjlbpres.dll -Oygc1600k.gpd -N"���� ���ʷ� C1600"
gpc2gpd -S2 -Iyjlbp.gpc -M6 -Ryjlbpres.dll -Oyga344k.gpd -N"���� ���ʷ� A344"
gpc2gpd -S2 -Iyjlbp.gpc -M7 -Ryjlbpres.dll -Oyga3441k.gpd -N"���� ���ʷ� A344*"
gpc2gpd -S2 -Iyjlbp.gpc -M8 -Ryjlbpres.dll -Oygz100k.gpd -N"���� ���ʷ� Z100"
gpc2gpd -S2 -Iyjlbp.gpc -M9 -Ryjlbpres.dll -Oygz200k.gpd -N"���� ���ʷ� Z200"

mkgttufm -vac yjlbp.rc3 yjlbpres.cmd > yjlbpres.txt

@rem Create codepage txt file.

@rem Run epap1000.cmd

@rem Create NT5.0's RC file

@rem Change sources to build NT5.0's RC file.

@rem Build.
