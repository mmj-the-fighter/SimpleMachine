#sandbox for push, pop, call, ret, passing arguments through stack,
#returning through stack etc.

MAIN:
MVI A 30
MVI B 20
PUSH A
PUSH B
CALL ADD_TW0_NUMBERS
POP A
DISP A
HLT

ADD_TW0_NUMBERS:
POP P
POP B
POP A
ADD3 C A B
PUSH C
PUSH P
RET
