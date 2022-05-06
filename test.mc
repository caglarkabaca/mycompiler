PUSH $x #5
PRINT "Lütfen isminizi giriniz: "
GETTXT $x
PRINTLN $x
PRINT "Lütfen bir sayı giriniz: "
GETINT $a
SET $buff #0
SETPOS $loop
PRINT $buff " - "
ADD $buff $buff #1
GOLT $loop $buff $a
HLT