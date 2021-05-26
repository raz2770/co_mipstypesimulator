.data
array:  .word 5, 3, 1, 6, 2, 8, 9, 10, 1, 8,12

.text
.globl main
main:
	la $s0, array
	addi $s1, $zero, 10
	j bubbleSort
	
bubbleSort:
	beq  $zero, $s1, out
	add  $s2, $s1, $zero
	j loop

loop:
	beq $zero, $s2, jmp
	lw  $t0, 0($s0)
	lw  $t1, 4($s0)

	slt $t2, $t0, $t1
	bne $zero, $t2, jump
	sw  $t0, 4($s0)
	sw  $t1, 0($s0)
	j jump

jump:
	addi $s0, $s0, 4
	li $t5 ,1
	sub $s2, $s2, $t5 
	j loop

jmp:
	li $t6 ,1
	sub $s1, $s1, $t6
	la $s0, array
	j bubbleSort 
	
out:
	addi $s1, $zero, 11
	j print
	
print:
	
	lw $t5, 0($s0)
    sw $t5, 0($gp)
	addi $s0, $s0, 4
    lw $t5, 0($s0)
    sw $t5, 4($gp)
	addi $s0, $s0, 4
    lw $t5, 0($s0)
    sw $t5, 8($gp)
	addi $s0, $s0, 4
    lw $t5, 0($s0)
    sw $t5, 12($gp)
	addi $s0, $s0, 4
    lw $t5, 0($s0)
    sw $t5, 16($gp)
	addi $s0, $s0, 4
    lw $t5, 0($s0)
    sw $t5, 20($gp)
	addi $s0, $s0, 4
    lw $t5, 0($s0)
    sw $t5, 24($gp)
	addi $s0, $s0, 4
    lw $t5, 0($s0)
    sw $t5, 28($gp)
	addi $s0, $s0, 4
    lw $t5, 0($s0)
    sw $t5, 32($gp)
	addi $s0, $s0, 4
	lw $t5, 0($s0)
    sw $t5, 36($gp)
	addi $s0, $s0, 4
    lw $t5, 0($s0)
    sw $t5, 40($gp)
	addi $s0, $s0, 4
    
	j jmpp

jmpp:
	li $v0,10
    syscall