Document Type: IPF
item: Global
  Version=5.0
  Title=WBEM Script API Installation (Build 575)
  Flags=01001000
  Languages=65 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
  Japanese Font Name=MS Gothic
  Japanese Font Size=10
  Start Gradient=0 0 255
  End Gradient=0 0 0
  Windows Flags=00000100000000110010110000011000
  Message Font=MS Sans Serif
  Font Size=8
  Disk Filename=SETUP
  Patch Flags=0000000000000001
  Patch Threshold=85
  Patch Memory=4000
  FTP Cluster Size=20
  Variable Name1=_SYS_
  Variable Default1=C:\WINNT\System32
  Variable Flags1=00001000
  Variable Name2=_SMSINSTL_
  Variable Default2=C:\Program Files\Microsoft SMS Installer
  Variable Flags2=00001000
  Variable Name3=_ODBC16_
  Variable Default3=C:\WINNT\System32
  Variable Flags3=00001000
  Variable Name4=_ODBC32_
  Variable Default4=C:\WINNT\System32
  Variable Flags4=00001000
end
item: Set Variable
  Variable=GROUP
  Value=WBEM
  Flags=10000000
end
item: Set Variable
  Variable=BACKUPDIR
end
item: Check Configuration
  Message=Sorry, but you must be running NT or Windows 95 to install these components.
  Title=Installation Error
  Flags=01011010
end
item: Check if File/Dir Exists
  Pathname=%SYS%
  Flags=10000100
end
item: Set Variable
  Variable=SYS
  Value=%WIN%
end
item: End Block
end
item: Set Variable
  Variable=APPTITLE
  Value=WBEM Scripting API
  Flags=10000000
end
item: Get Registry Key Value
  Variable=WBEMDIR
  Key=Software\Microsoft\WBEM
  Value Name=Installation Directory
  Flags=00000100
end
item: If/While Statement
  Variable=WBEMDIR
end
item: Display Message
  Title=WBEM Not Installed
  Text=Sorry, but you must have already installed WBEM core/SDK to install this component.
  Flags=00001000
end
item: Exit Installation
end
item: End Block
end
item: Get Registry Key Value
  Variable=WBEMBUILD
  Key=Software\Microsoft\WBEM
  Value Name=Build
  Flags=00000100
end
item: Set Variable
  Variable=MAINDIR
  Value=%WBEMDIR%
  Flags=10000000
end
item: Open/Close INSTALL.LOG
  Pathname=%WBEMDIR%\script\install.log
  Flags=00000010
end
item: Wizard Block
  Direction Variable=DIRECTION
  Display Variable=DISPLAY
  Bitmap Pathname=%_SMSINSTL_%\DIALOGS\TEMPLATE\WIZARD.BMP
  X Position=9
  Y Position=10
  Filler Color=8421440
  Flags=00000011
end
item: Custom Dialog Set
  Name=Start Installation
  Display Variable=DISPLAY
  item: Dialog
    Title=%APPTITLE% Installation
    Title French=Installation de %APPTITLE%
    Title German=Installation von %APPTITLE%
    Title Portuguese=Instala��o do %APPTITLE%
    Title Spanish=Instalaci�n de %APPTITLE%
    Title Italian=Installazione di %APPTITLE%
    Title Danish=Installation af %APPTITLE%
    Title Finnish=%APPTITLE% Asennus
    Title Dutch=Installatie van %APPTITLE%
    Title Norwegian=Installere %APPTITLE%
    Title Swedish=%APPTITLE% Installation
    Width=271
    Height=224
    Font Name=Helv
    Font Size=8
    item: Push Button
      Rectangle=150 187 195 202
      Variable=DIRECTION
      Value=N
      Create Flags=01010000000000010000000000000001
      Text=&Next >
      Text French=&Suivant >
      Text German=&Weiter >
      Text Portuguese=&Avan�ar >
      Text Spanish=&Siguiente >
      Text Italian=&Avanti >
      Text Danish=&N�ste >
      Text Finnish=&Seuraava >
      Text Dutch=&Volgende >
      Text Norwegian=&Neste >
      Text Swedish=&N�sta >
    end
    item: Push Button
      Rectangle=105 187 150 202
      Variable=DIRECTION
      Value=B
      Create Flags=01010000000000010000000000000000
      Text=< &Back
      Text French=< &Pr�c�dent
      Text German=< &Zur�ck
      Text Portuguese=< &Voltar
      Text Spanish=< &Atr�s
      Text Italian=< &Indietro
      Text Danish=< &Tilbage
      Text Finnish=< &Takaisin
      Text Dutch=< V&orige
      Text Norwegian=< &Tilbake
      Text Swedish=< &Bak�t
    end
    item: Push Button
      Rectangle=211 187 256 202
      Action=3
      Create Flags=01010000000000010000000000000000
      Text=Cancel
      Text French=Annuler
      Text German=Abbrechen
      Text Portuguese=Cancelar
      Text Spanish=Cancelar
      Text Italian=Annulla
      Text Danish=Annuller
      Text Finnish=Peruuta
      Text Dutch=Annuleren
      Text Norwegian=Avbryt
      Text Swedish=Avbryt
    end
    item: Static
      Rectangle=8 180 256 181
      Action=3
      Create Flags=01010000000000000000000000000111
    end
    item: Static
      Rectangle=86 8 258 42
      Create Flags=01010000000000000000000000000000
      Flags=0000000000000001
      Name=Times New Roman
      Font Style=-24 0 0 0 700 255 0 0 0 3 2 1 18
      Text=Ready to Install!
      Text French=Pr�t � installer !
      Text German=Installationsbereit!
      Text Portuguese=Pronto para instalar!
      Text Spanish=Preparado para la instalaci�n.
      Text Italian=Pronto per l'installazione.
      Text Danish=Klar til at installere!
      Text Finnish=Valmiina asennukseen.
      Text Dutch=Klaar om te installeren.
      Text Norwegian=Klar til � installere.
      Text Swedish=F�rdigt f�r installation.
    end
    item: Static
      Rectangle=86 42 256 102
      Create Flags=01010000000000000000000000000000
      Text=You are now ready to install %APPTITLE%.
      Text=
      Text=Press the Next button to begin the installation or the Back button to reenter the installation information.
      Text French=Vous �tes maintenant pr�t � installer %APPTITLE%.
      Text French=
      Text French=Cliquez sur le bouton Suivant pour commencer l'installation ou sur le bouton Pr�c�dent pour entrer les informations d'installation � nouveau.
      Text German=Sie k�nnen %APPTITLE% nun installieren.
      Text German=
      Text German=Klicken Sie auf "Weiter", um mit der Installation zu beginnen. Klicken Sie auf "Zur�ck", um die Installationsinformationen neu einzugeben.
      Text Portuguese=Agora voc� est� pronto para instalar %APPTITLE%.
      Text Portuguese=
      Text Portuguese=Pressione o bot�o 'Avan�ar' para iniciar a instala��o ou o bot�o 'Voltar' para redigitar a informa��o de instala��o.
      Text Spanish=Ya est� listo para instalar %APPTITLE%.
      Text Spanish=
      Text Spanish=Elija Siguiente para iniciar la instalaci�n o Atr�s para introducir de nuevo la informaci�n para la instalaci�n.
      Text Italian=Ora � possibile installare %APPTITLE%.
      Text Italian=
      Text Italian=Premere il pulsante Avanti per avviare l'installazione o il pulsante Indietro per reinserire le informazioni di installazione.
      Text Danish=%APPTITLE% er nu klar til at blive installeret.
      Text Danish=
      Text Danish=Tryk p� knappen N�ste for at starte installationen, eller tryk p� knappen Tilbage for at genindtaste installationsoplysningerne.
      Text Finnish=Voit nyt aloittaa %APPTITLE%:n asennuksen.
      Text Finnish=
      Text Finnish=Aloita asennus valitsemalla Seuraava tai palaa asennustietoihin valitsemalla Takaisin.
      Text Dutch=%APPTITLE% kan nu ge�nstalleerd worden.
      Text Dutch=
      Text Norwegian=%APPTITLE% kan n� installeres.
      Text Norwegian=
      Text Norwegian=Klikk Neste for � starte installasjonen, eller klikk Tilbake for � angi installasjonsinformasjonen p� nytt.
      Text Norwegian=
      Text Swedish=Det �r nu f�rdigt f�r installation av %APPTITLE%.
      Text Swedish=
      Text Swedish=Klicka p� N�sta f�r att p�b�rja installationen eller klicka p� Bak�t f�r att skriva om installationsinformationen.
    end
  end
end
item: End Block
end
item: Check Disk Space
  Component=COMPONENTS
end
item: Include Script
  Pathname=%_SMSINSTL_%\INCLUDE\uninstal.ipf
end
item: Install File
  Source=G:\NT5\WBEMNT5.575\COMMON\wbemdisp.tlb
  Destination=%WBEMDIR%\wbemdisp.tlb
  Description=WBEM Script Type Library
  Flags=0000000000000010
end
item: Install File
  Source=G:\NT5\WBEMNT5.575\i386\wbemdisp.dll
  Destination=%WBEMDIR%\wbemdisp.dll
  Description=WBEM Script API
  Flags=0001000000000010
end
item: Check Configuration
  Flags=10111011
end
item: Get Registry Key Value
  Variable=STARTUPDIR
  Key=Software\Microsoft\Windows\CurrentVersion\Explorer\Shell Folders
  Default=%WIN%\Start Menu\Programs\StartUp
  Value Name=StartUp
  Flags=00000010
end
item: Get Registry Key Value
  Variable=DESKTOPDIR
  Key=Software\Microsoft\Windows\CurrentVersion\Explorer\Shell Folders
  Default=%WIN%\Desktop
  Value Name=Desktop
  Flags=00000010
end
item: Get Registry Key Value
  Variable=STARTMENUDIR
  Key=Software\Microsoft\Windows\CurrentVersion\Explorer\Shell Folders
  Default=%WIN%\Start Menu
  Value Name=Start Menu
  Flags=00000010
end
item: Get Registry Key Value
  Variable=GROUPDIR
  Key=Software\Microsoft\Windows\CurrentVersion\Explorer\Shell Folders
  Default=%WIN%\Start Menu\Programs
  Value Name=Programs
  Flags=00000010
end
item: Get Registry Key Value
  Variable=CSTARTUPDIR
  Key=Software\Microsoft\Windows\CurrentVersion\Explorer\Shell Folders
  Default=%STARTUPDIR%
  Value Name=Common Startup
  Flags=00000100
end
item: Get Registry Key Value
  Variable=CDESKTOPDIR
  Key=Software\Microsoft\Windows\CurrentVersion\Explorer\Shell Folders
  Default=%DESKTOPDIR%
  Value Name=Common Desktop
  Flags=00000100
end
item: Get Registry Key Value
  Variable=CSTARTMENUDIR
  Key=Software\Microsoft\Windows\CurrentVersion\Explorer\Shell Folders
  Default=%STARTMENUDIR%
  Value Name=Common Start Menu
  Flags=00000100
end
item: Get Registry Key Value
  Variable=CGROUPDIR
  Key=Software\Microsoft\Windows\CurrentVersion\Explorer\Shell Folders
  Default=%GROUPDIR%
  Value Name=Common Programs
  Flags=00000100
end
item: Set Variable
  Variable=CGROUP_SAVE
  Value=%GROUP%
end
item: Set Variable
  Variable=GROUP
  Value=%CGROUPDIR%\%GROUP%
end
item: Create Shortcut
  Source=%WBEMDIR%\unwise32.exe
  Destination=%GROUP%\Uninstall WBEM Script Components.lnk
  Command Options=%WBEMDIR%\script\install.log
  Working Directory=%WBEMDIR%
  Key Type=1536
  Flags=00000001
end
item: End Block
end
item: Wizard Block
  Direction Variable=DIRECTION
  Display Variable=DISPLAY
  Bitmap Pathname=%_SMSINSTL_%\DIALOGS\TEMPLATE\WIZARD.BMP
  X Position=9
  Y Position=10
  Filler Color=8421440
  Flags=00000011
end
item: Custom Dialog Set
  Name=Finished
  Display Variable=DISPLAY
  item: Dialog
    Title=%APPTITLE% Installation
    Title French=Installation de %APPTITLE%
    Title German=Installation von %APPTITLE%
    Title Portuguese=Instala��o do %APPTITLE%
    Title Spanish=Instalaci�n de %APPTITLE%
    Title Italian=Installazione di %APPTITLE%
    Title Danish=Installation af %APPTITLE%
    Title Finnish=%APPTITLE% Asennus
    Title Dutch=Installatie van %APPTITLE%
    Title Norwegian=Installere %APPTITLE%
    Title Swedish=%APPTITLE% Installation
    Width=271
    Height=224
    Font Name=Helv
    Font Size=8
    item: Push Button
      Rectangle=150 187 195 202
      Variable=DIRECTION
      Value=N
      Create Flags=01010000000000010000000000000001
      Text=&Finish
      Text French=&Terminer
      Text German=&Weiter
      Text Portuguese=&Concluir
      Text Spanish=&Finalizar
      Text Italian=&Fine
      Text Danish=&Udf�r
      Text Finnish=&Valmis
      Text Dutch=Vol&tooien
      Text Norwegian=&Fullf�r
      Text Swedish=A&vsluta
    end
    item: Push Button
      Rectangle=105 187 150 202
      Variable=DISABLED
      Value=!
      Create Flags=01010000000000010000000000000000
      Text=< &Back
      Text French=< &Pr�c�dent
      Text German=< &Zur�ck
      Text Portuguese=< &Voltar
      Text Spanish=< &Atr�s
      Text Italian=< &Indietro
      Text Danish=< &Tilbage
      Text Finnish=< &Takaisin
      Text Dutch=< V&orige
      Text Norwegian=< &Tilbake
      Text Swedish=< &Bak�t
    end
    item: Push Button
      Rectangle=211 187 256 202
      Variable=DISABLED
      Value=!
      Action=3
      Create Flags=01010000000000010000000000000000
      Text=Cancel
      Text French=Annuler
      Text German=Abbrechen
      Text Portuguese=Cancelar
      Text Spanish=Cancelar
      Text Italian=Annulla
      Text Danish=Annuller
      Text Finnish=Peruuta
      Text Dutch=Annuleren
      Text Norwegian=Avbryt
      Text Swedish=Avbryt
    end
    item: Static
      Rectangle=8 180 256 181
      Action=3
      Create Flags=01010000000000000000000000000111
    end
    item: Static
      Rectangle=86 8 258 42
      Create Flags=01010000000000000000000000000000
      Flags=0000000000000001
      Name=Times New Roman
      Font Style=-24 0 0 0 700 255 0 0 0 3 2 1 18
      Text=Installation Completed!
      Text French=Installation termin�e !
      Text German=Die Installation ist abgeschlossen!
      Text Portuguese=Instala��o conclu�da!
      Text Spanish=Instalaci�n completada.
      Text Italian=Installazione completata.
      Text Danish=Installationen er f�rdig!
      Text Finnish=Asennus suoritettu
      Text Dutch=De installatie is voltooid.
      Text Norwegian=Installasjonen er fullf�rt.
      Text Swedish=Installationen �r slutf�rd!
    end
    item: Static
      Rectangle=86 42 256 102
      Create Flags=01010000000000000000000000000000
      Text=The installation of %APPTITLE% has been successfully completed.
      Text=
      Text=Press the Finish button to exit this installation.
      Text French=%APPTITLE% est maintenant install�.
      Text French=
      Text French=Cliquez sur le bouton Terminer pour quitter l'installation.
      Text German=%APPTITLE% wurde erfolgreich installiert.
      Text German=
      Text German=Klicken Sie auf "Weiter", um die Installation zu beenden.
      Text Portuguese=A instala��o do %APPTITLE% foi conclu�da com �xito.
      Text Portuguese=
      Text Portuguese=Pressione o bot�o 'Concluir' para sair desta instala��o.
      Text Spanish=%APPTITLE% se ha instalado con �xito.
      Text Spanish=
      Text Spanish=Elija Finalizar para salir de esta instalaci�n.
      Text Italian=L'installazione %APPTITLE% � stata completata con successo.
      Text Italian=
      Text Italian=Premere il pulsante Fine per uscire dall'installazione.
      Text Danish=Installationen af %APPTITLE% er fuldf�rt.
      Text Danish=
      Text Danish=Tryk p� knappen Udf�r for at afslutte denne installation.
      Text Finnish=%APPTITLE%:n asennus on suoritettu loppuun.
      Text Finnish=
      Text Finnish=Lopeta Asennus valitsemalla Valmis.
      Text Dutch=De installatie van %APPTITLE% is met succes voltooid.
      Text Dutch=
      Text Dutch=Druk op de knop Voltooien om de installatie af te sluiten.
      Text Norwegian=%APPTITLE% er installert.
      Text Norwegian=
      Text Norwegian=Klikk Fullf�r for � avslutte denne installasjonen.
      Text Swedish=Installationen av %APPTITLE% har slutf�rts.
      Text Swedish=
      Text Swedish=Klicka p� Avsluta f�r att avsluta den h�r installationen.
    end
  end
end
item: End Block
end
item: New Event
  Name=Cancel
end
item: Include Script
  Pathname=%_SMSINSTL_%\INCLUDE\rollback.ipf
end
