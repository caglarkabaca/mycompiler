!Dieses Programm dekrementiert die Eingabe um eins, bis sie Null erreicht. Diese Zeile ist ein Kommentar
SETPOS $BEGIN
GETINT $reg1 ! receive input from user
ADD $reg1 $reg1 #2 ! reg1 = reg1 + 2
SETPOS $LOOPSTART
PRINTLN "Current value of reg1 " $reg1
SUB $reg1 $reg1 #1 ! reg1--
GOGT $LOOPSTART $reg1 #0 ! jump if reg1 greater than 0
GOTO $BEGIN ! jump BEGIN, infinite loop
HLT