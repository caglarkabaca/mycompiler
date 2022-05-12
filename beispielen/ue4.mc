SET $b #7
SET $c #3
SET $d #12
MUL $buf1 $c $d
ADD $c $c #3
DIV $buf1 $buf1 $c
MUL $buf2 $b $d
SUB $b $b #3
DIV $buf2 $buf2 $b
SUB $a $buf1 $buf2
PRINTLN $a
HLT