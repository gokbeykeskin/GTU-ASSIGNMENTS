load R2, [Negative]
load R3, 1
load R0, [CheckValue]
load R7, [fix]
load R9, [0x70]
load RB, 0x70
load RC, [Bin]

Add2: addi RA, R9, RA
      store RA, [0xF0]
      load R1, [0XF0]
      jmpLE RB<=R0, Counter
      addi RB, R3, RB
      jmp Add1

Add1: load R9, [RB]
      jmp Add2


Counter: addi R4, R4, R3

Try: addi R1, R1, R2
     jmpEQ R4=R0, Check1
     jmp Counter
Check1: addi R1, R1, R0

complementing1: xor R1, R1, RC
                addi R1, R1, R3
                jmp Check
        

      
Check: jmpLE R1<=R0, Set
       jmp Restart


Restart: load RE, [zero]
         load R4, [zero]
         load R1, [0xF0]
         addi R2, R2, R3
         jmp Counter


Set: addi R2, R2, R0

Complementing2: xor  R2, R2, RC
                addi R2, R2, R3
                addi R2, R2, R7
                move R0, R2
                halt

Negative: db 10000000b
CheckValue: db 15
zero: db 0
fix: db 0x96
Bin: db 11111111b