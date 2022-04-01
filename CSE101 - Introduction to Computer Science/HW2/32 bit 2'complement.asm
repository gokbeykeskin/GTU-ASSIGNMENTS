load R0, [Binary1]
load R1, [Binary2]
load R2, [Binary3]
load R3, [Binary4]
load R4, [BinaryCheck]
load R6, 1

Complementing1: xor R0,R0,R4
		    addi R0,R0,R6
Complementing2: xor R1,R1,R4
		    addi R1,R1,R6
Complementing3: xor R2,R2,R4
		    addi R2,R2,R6
Complementing4: xor R3,R3,R4
		    addi R3,R3,R6
		    halt


Binary1: db 11010011b
Binary2: db 10111001b
Binary3: db 01101110b
Binary4: db 10101010b
BinaryCheck: db 11111111b