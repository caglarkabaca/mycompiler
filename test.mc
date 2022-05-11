GETINT $number
SET $old $number
SET $res #1
SETPOS $LOOP
MUL $res $res $number
SUB $number $number #1
GOGT $LOOP $number #0
PRINTLN $old "! => " $res
HLT