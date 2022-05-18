SET $a #0
SETPOS $loop
ADD $a $a #1
SYSTEM "touch " $a ".txt"
GOLT $loop $a #10
HLT