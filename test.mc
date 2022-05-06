PRINT "Lütfen bir sayı giriniz: "
GETINT $a
SET $buff #0
SETPOS $loop
PRINT $buff " - "
ADD $buff $buff #1
GOLT $loop $buff $a
HLT