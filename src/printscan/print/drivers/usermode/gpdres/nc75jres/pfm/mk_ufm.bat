
for %%i in (
    co10 co12 co17 co5 co6 co8
    ocrb10
    roman10 roman12 roman17 roman5 roman6 roman8 romanps
    sans10 sans12 sans17 sans5 sans6 sans8 sansps
) do (
    pfm2ufm -r1 nc75j.001 %%i.pfm ..\ctt\necxta.gtt %%i.ufm
)

for %%i in (
    gothout
    minchob minout
    mouout
    vgothout
    vminchob vminout
    vmouout
) do (
    pfm2ufm -f -p nc75j.001 %%i.pfm -13 %%i.ufm
)


