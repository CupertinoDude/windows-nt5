@rem ***** GPD and Font resource conversion
@rem       2                  "�ŵ����� LP1220S"
@rem       3                  "�ŵ����� LP1230S"
@rem       5                  "�ŵ����� LP4037-5E"

gpc2gpd -S2 -Ippdsch.gpc -M2 -Rppdskres.dll -Osr1220sk.gpd -N"�ŵ����� LP1220S"
gpc2gpd -S2 -Ippdsch.gpc -M3 -Rppdskres.dll -Osr1230sk.gpd -N"�ŵ����� LP1230S"
gpc2gpd -S2 -Ippdsch.gpc -M5 -Rppdskres.dll -Osr40375k.gpd -N"�ŵ����� LP4037-5E"

@rem mkgttufm -vac ppdsch.rc3 ppdskres.cmd > ppdskres.txt

@rem Create codepage txt file.
@rem Run epap1000.cmd
@rem Create NT5.0's RC file
@rem Change sources to build NT5.0's RC file.
@rem Build.
