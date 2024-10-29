#increment a number stored in a memory location
#load 0 200
#load 1 201
#add 0 1
#store 0 202
#hlt
#Calculate the sum of numbers stored in consecutive memory locations
mvi 0 0
load 3 199
mvi 4 200
loop:
ldr 5 4
add3 0 0 5
inc 4
dcr 3
jnz loop 
store 0 255
hlt

