# timetemplate.asm
  # Written 2015 by F Lundevall
  # Copyright abandonded - this file is in the public domain.

.macro	PUSH (%reg)
	addi	$sp,$sp,-4
	sw	%reg,0($sp)
.end_macro

.macro	POP (%reg)
	lw	%reg,0($sp)
	addi	$sp,$sp,4
.end_macro

	.data
	.align 2
.global hexasc
hexasc: 
	andi $v0, $a3, 0xF
	li $t0, 9
	ble $v0, $t0, digit
	nop

	addi $v0, $v0, 55

	j end
	nop

digit: 
	addi $v0, $v0, 48
	j end
	nop

end:
	jr $ra
	nop
.global delay
delay:


	beq 	$a0, $zero, completed	
	nop


	while:
	li	$t1, 0 			

	for:
	slti 	$t3, $t1, 3800		
	beq	$t3, $zero, for_end	
	nop


	addi	$t1, $t1, 1		# i = i + 1
	j	for			
	nop

	for_end:
	addi	$a0, $a0, -1		


	bgt	$a0, $zero, while

	nop

	completed:
	jr	$ra
	nop
.global time2string
time2string:
	PUSH $s0
	PUSH $s1
	PUSH $s2
	PUSH $s3
	PUSH $ra

	
	# Extracting the minutes and seconds from the input parameter
	#5958 after increment
	
	srl $s0, $a1, 12 # Get the first digit 0xxx
	andi $s0, $s0, 0xF
	
	srl $s1, $a1, 8    # Get the second digit x0xx
	andi $s1, $s1, 0xF
	
	srl $s2, $a1, 4    # Get the third digit xx0x
	andi $s2, $s2, 0xF
	
	andi $s3, $a1, 0xF # Get the fourth digit xxx0
	
	# Converting NBCD-coded digits to ASCII
	move $a3, $s0  
	jal hexasc    # Convert first digit of minutes
	nop
	move $t0, $v0
	sb $t0, 0($a0)    # Store the ASCII digit in memory
	#sll $a0, $a0, 4    # Move to the next memory location
	move $a3, $s1
	jal hexasc    # Convert second digit of minutes
	nop
	move $t0, $v0
	sb $t0, 1($a0)    # Store the ASCII digit in memory
	#sll $a0, $a0, 4    # Move to the next memory location
	li $t0, 58    # ASCII code for colon character
	sb $t0, 2($a0)    # Store the colon character in memory
	#addi $a0, $a0, 1    # Move to the next memory location
	move $a3, $s2
	jal hexasc    # Convert first digit of seconds
	nop
	move $t0, $v0
	sb $t0, 3($a0)    # Store the ASCII digit in memory
	#addi $a0, $a0, 1    # Move to the next memory location
	move $a3, $s3
	jal hexasc    # Convert second digit of seconds
	nop
	move $t0, $v0
	sb $t0, 4($a0)    # Store the ASCII digit in memory
	#addi $a0, $a0, 1    # Move to the next memory location
	li $t0, 0    # Null byte to mark end of string
	sb $t0, 5($a0)    # Store the null byte in memory
	
	POP $ra
	POP $s3
	POP $s2
	POP $s1
	POP $s0
	jr $ra    # Return from subroutine
	nop
.global enable_interrupt
enable_interrupt:
	ei
	jr	$ra
	nop