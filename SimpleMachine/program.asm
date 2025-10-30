#Calculate the sum of numbers stored in 
#consecutive memory locations starting from location 200
#the number of elements is stored in location 199

#intialization part
MVI A 0		# initialize register A with 0
LOAD C 199	# load count into register C from memory location 199
MVI B 200	# initialize register B with 200

#main logic
LOOP:		# loop
LDR D B		# load the value pointed by B to D
#ADD3 A A D	# A = A + D (SLOW)
#ADD2 A D   # A = A + D (SLOW)
ADD1 D      # A = A + D (FAST)
INC B		# ++B
DCR C		# --C
JNZ LOOP	# if C != 0 go to loop

#output results
STORE A 255	# store A to 255
DISP A		# print A	
HLT			# stop