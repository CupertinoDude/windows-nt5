@rem ***** GPD and Font resource conversion
@rem       1                  "ť�н� QBJ 3630A"
@rem       2                  "ť�н� QBJ 3630B"
@rem       3                  "ť�н� QBJ 3630E"

gpc2gpd -S2 -Iqbj.gpc -M1 -Rqbjres.dll -Oqb3630ak.gpd -N"ť�н� QBJ 3630A"
gpc2gpd -S2 -Iqbj.gpc -M2 -Rqbjres.dll -Oqb3630bk.gpd -N"ť�н� QBJ 3630B"
gpc2gpd -S2 -Iqbj.gpc -M3 -Rqbjres.dll -Oqb3630ek.gpd -N"ť�н� QBJ 3630E"

mkgttufm -vac qbj.rc3 qbjres.cmd > qbjres.txt

@rem Create codepage txt file.

@rem Run epap1000.cmd

@rem Create NT5.0's RC file

@rem Change sources to build NT5.0's RC file.

@rem Build.
