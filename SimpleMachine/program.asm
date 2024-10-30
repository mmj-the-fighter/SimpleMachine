#Calculate the sum of numbers stored in 
#consecutive memory locations starting from location 200
#the number of elements is stored in location 199
MVI A 0	
LOAD C 199
MVI B 200
LOOP:		
LDR D B
ADD3 A A D
INC B
DCR C
JNZ LOOP
STORE A 255
HLT