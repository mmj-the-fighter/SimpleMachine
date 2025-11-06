#sandbox for push, pop, call, ret, passing arguments through stack,
#returning through stack etc.

MAIN:
MVI A 30
MVI B 20
PUSH A
PUSH B
CALL M1
POP A
DISP A
HLT

M1:
POP P
POP B
POP A
ADD3 C A B
PUSH C
PUSH P
RET
