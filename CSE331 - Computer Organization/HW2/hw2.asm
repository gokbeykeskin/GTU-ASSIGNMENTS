.data

	newline: .asciiz "\n"
	comma: .asciiz ","
	
	lengthTxt: .asciiz "  Length = "
	candidadeTxt: .asciiz "A candidade increasing subsequence is = "
	longestTxt: .asciiz "The longest increasing subsequence is = "
	ArrTxt: .asciiz "Array is = "
	seperatorTxt: .asciiz "\n---------------------------------------\n"
	arr: .space  100
	temp: .space 100
	result: .space 100

.text

main:
	la $a1, arr #get address of the array
	li $a2, 0  #array index
	li $a3, 0  #temp index
	li $s1, 0  #max size

	
	#first test with array 3,10,7,9,4,11
	li $s0, 24 #new array size
	
   	li $t7, 3 #new 1st value
  	sw $t7, 0($a1) #save new value
  	li $t7, 10 #new 2nd value
  	sw $t7, 4($a1) #save new value
     	li $t7, 7 #new 3rd value
  	sw $t7, 8($a1) #save new value
     	li $t7, 9 #new 4th value
  	sw $t7, 12($a1) #save new value
     	li $t7, 4 #new 5th value
  	sw $t7, 16($a1) #save new value
     	li $t7, 11 #new 6th value
     	sw $t7, 20($a1) #save new value
     	
     	jal printArr
	jal calcLongest # calculate longest subsequence
	jal printLongest #print longest subsequence
	
	li $v0, 4 
	la $a0, seperatorTxt
	syscall
	
	#second test with array 10, 22, 9, 33, 21, 50, 41, 60, 80
	li $s0, 36 #new array size
	
   	li $t7, 10 #new 1st value
  	sw $t7 0($a1) #save new value
  	li $t7, 22 #new 2nd value
  	sw $t7 4($a1) #save new value
     	li $t7, 9 #new 3rd value
  	sw $t7 8($a1) #save new value
     	li $t7, 33 #new 4th value
  	sw $t7 12($a1) #save new value
     	li $t7, 21 #new 5th value
  	sw $t7 16($a1) #save new value
     	li $t7, 50 #new 6th value
  	sw $t7 20($a1) #save new value
     	li $t7, 41 #new 7th value
  	sw $t7 24($a1) #save new value
     	li $t7, 60 #new 8th value
  	sw $t7 28($a1) #save new value
     	li $t7, 80 #new 9th value
  	sw $t7 32($a1) #save new value
  	jal printArr #print the array
	jal calcLongest # calculate longest subsequence
	jal printLongest #print longest subsequence
	
	li $v0, 4 
	la $a0, seperatorTxt #seperate tests
	syscall
	
	#third test with array 3, 10, 2, 1, 20
	li $s0, 20 #new array size
	
   	li $t7, 3 #new 1st value
  	sw $t7 0($a1) #save new value
  	li $t7, 10 #new 2nd value
  	sw $t7 4($a1) #save new value
     	li $t7, 2 #new 3rd value
  	sw $t7 8($a1) #save new value
     	li $t7, 1 #new 4th value
     	sw $t7 12($a1) #save new value
     	li $t7, 20 #new 5th value
     	sw $t7 16($a1) #save new value
     	jal printArr
     	jal calcLongest # calculate longest subsequence
	jal printLongest #print longest subsequence
     	
     	li $v0, 4 
	la $a0, seperatorTxt #seperate tests
	syscall
     	#4th test with array 7, 10, 1, 11, 3, 12
     	li $s0, 24 #new array size
	
   	li $t7, 7 #new 1st value
  	sw $t7 0($a1) #save new value
  	li $t7, 10 #new 2nd value
  	sw $t7 4($a1) #save new value
     	li $t7, 1 #new 3rd value
  	sw $t7 8($a1) #save new value
     	li $t7, 11 #new 4th value
     	sw $t7 12($a1) #save new value
     	li $t7, 3 #new 5th value
     	sw $t7 16($a1) #save new value
     	li $t7, 12 #new 6th value
     	sw $t7 20($a1) #save new value
     	jal printArr
     	jal calcLongest # calculate longest subsequence
	jal printLongest #print longest subsequence
	
	li $v0, 4 
	la $a0, seperatorTxt #seperate tests
	syscall
	
	#5th test with array 1,2,3,5,4
	li $s0, 20 #new array size
	
   	li $t7, 1 #new 1st value
  	sw $t7 0($a1) #save new value
  	li $t7, 2 #new 2nd value
  	sw $t7 4($a1) #save new value
     	li $t7, 3 #new 3rd value
  	sw $t7 8($a1) #save new value
     	li $t7, 5 #new 4th value
     	sw $t7 12($a1) #save new value
     	li $t7, 4 #new 5th value
     	sw $t7 16($a1) #save new value
	jal printArr
     	jal calcLongest # calculate longest subsequence
	jal printLongest #print longest subsequence
	

	li $v0, 4 
	la $a0, seperatorTxt #seperate tests
	syscall
	
	
	#6th test with array 90,20,0,31,5,51,6
	li $s0, 28 #new array size
	
	li $t7, 90 #new 1st value
  	sw $t7 0($a1) #save new value
  	li $t7, 20 #new 2nd value
  	sw $t7 4($a1) #save new value
     	li $t7, 0 #new 3rd value
  	sw $t7 8($a1) #save new value
     	li $t7, 31 #new 4th value
  	sw $t7 12($a1) #save new value
     	li $t7, 5 #new 5th value
  	sw $t7 16($a1) #save new value
     	li $t7, 51 #new 6th value
  	sw $t7 20($a1) #save new value
     	li $t7, 6 #new 7th value
  	sw $t7 24($a1) #save new value
  	jal printArr
     	jal calcLongest # calculate longest subsequence
	jal printLongest #print longest subsequence
	
	li $v0, 10 #terminatation syscall code is 10
	syscall    #terminate the program
calcLongest:
	
	slt $t1,$a2,$s0
	bne $t1, $zero, continue2 #if arr index < array size jump to continue2
	blt $a3, $s1, continue #if temp index < max size  jump to continue
	
	add $s1,$a3,$zero #max size = temp index
	li $t4, 0 # i index of loop which will be used at loadToResult subroutine
	
	subi $sp, $sp, 4 #reserve 4 byte space from stack
	sw $ra, 0($sp) #save register adress to stack
	jal loadToResult #save the LÝS to result array
	lw $ra, 0($sp) 
	addi $sp, $sp, 4
	
	continue:	
		beq $a3 , $zero , exit #if temp index is zero, there is nothing to print so don't print
	
		subi $sp, $sp, 4 #reserve 4 byte space from stack
		sw $ra, 0($sp) #save register adress to stack
		jal printCandidade #print a LIS
		lw $ra, 0($sp) #get register adress from stack
		addi $sp, $sp, 4 #release reserved space
		jr $ra
		
	continue2: #if not, continue from here
		lw $t5,arr($a2) #arr[arrIndex]
		addi $a3 , $a3 ,-4
		lw $t6, temp($a3) #temp[tempIndex-1]
		addi $a3 , $a3 ,4
		
		beq $a3, $zero, continue3 #if tempindex is zero
		blt $t6,$t5 continue3 # or arr[arrIndex] > temp[tempIndex]
		j continue4 #else
		
		continue3:
			lw $t5, arr($a2)
			sw $t5 temp($a3) #save the arr[arrIndex] to temp[tempIndex]
			
			subi $sp, $sp, 12 #reserve 12 byte space from stack
			sw $a3, 8($sp) #save tempIndex to stack
			sw $a2, 4($sp) #save arrIndex to stack
			sw $ra, 0($sp) #save register adress to stack
			
			addi $a3,$a3,4 #increment the tempIndex
			addi $a2,$a2,4 #increase arr index and recursively call the same subroutine
			
			jal calcLongest
			lw $a2, 4($sp) #get arrIndex from stack
			lw $a3, 8($sp) #get tempIndex from stack
			lw $ra, 0($sp) #get register adress from stack
			addi $sp, $sp, 12 #release the reserved stack
			#subi $a3,$a3,4 #remove the last element of the temp
		continue4:
			subi $sp, $sp, 8 #reserve 8 byte space from stack
			sw $ra, 0($sp) #save register adress to stack
			sw $a2, 4($sp) #save arrIndex to stack
			
			addi $a2,$a2,4 #increase arr index and recursively call the same subroutine
			jal calcLongest
			lw $ra, 0($sp)#get register adress from stack
			lw $a2, 4($sp) #get arrIndex from stack
			addi $sp, $sp, 8 #release reserved space
	jr $ra

#next 3 subroutines does the same thing on different arrays (also uses different parameters)
printArr:
	li $v0, 4 #code for printing ASCII
	la $a0, ArrTxt
	syscall
	addi $t6,$zero,0 #set loop i to 0
	printLoop3: #print integers seperated with commas
		li $v0, 1 #code for printing integers
		lw $a0, arr($t6)
		syscall
		addi $t6,$t6,4 #code for printing ASCII
		li $v0,4
		la $a0, comma
		syscall
		blt $t6,$s0,printLoop3 #loop
	li $v0,4 #code for printing ASCII
	la $a0, lengthTxt
	syscall
	div $t7, $a3, 4
	li $v0, 1
	add $a0, $zero, $t7
	syscall
	li $v0,4 #code for printing ASCII
	la $a0, newline
	syscall
		
	jr $ra
	
printCandidade:
	li $v0, 4
	la $a0, candidadeTxt
	addi $t6,$zero,0
	syscall
	
	printLoop:
		li $v0, 1
		lw $a0, temp($t6)
		syscall
		addi $t6,$t6,4
		li $v0,4
		la $a0, comma
		syscall
		blt $t6,$a3,printLoop
	li $v0,4
	la $a0, lengthTxt
	syscall
	div $t7, $a3, 4
	li $v0, 1
	add $a0, $zero, $t7
	syscall
	li $v0,4
	la $a0, newline
	syscall
		
	jr $ra
	
printLongest:
	li $v0, 4
	la $a0, longestTxt
	addi $t6,$zero,0
	syscall
	
	
	printLoop2:
		li $v0, 1
		lw $a0, result($t6)
		syscall
		addi $t6,$t6,4
		li $v0,4
		la $a0, comma
		syscall
		blt $t6,$s1,printLoop2
	li $v0,4
	la $a0,lengthTxt
	syscall
	li $v0,1
	div $s1, $s1, 4
	add $a0, $zero, $s1
	syscall
		
	jr $ra

loadToResult: #move temp array values to result array.
	lw $t5, temp($t4)
	sw $t5, result($t4)
	addi $t4 $t4, 4 #increment index
	bne $t4, $a3,loadToResult
	
	#addi $s1,$a3,0
	
	jr $ra

exit:
jr $ra