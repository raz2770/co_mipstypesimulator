  
.data # data to be stored in memory
SIZE: .word 5
N: .word 6, 8, 2, 5, 10
SUM: .word 0

.text
.globl main
main:

la $t1, N # address of N
lw $t0, SIZE # store value of SIZE
lw $s0, N # store maximum value in N

loop: # Loop that iterates for SIZE times
	lw $t2, 0($t1) # load value at $t1 address in $t2
	
	addi $t1, $t1, 4 # increase address by 4
	slt $t3, $s0, $t2 # check if max < current number
	bne $t3, $zero, save # if max < current the save in $s0
	j exit
save:
	add $s0, $t2, $zero # save in $s0
exit:
	addi $t0, $t0, -1 # decrease number of elements by 1
	bne $t0, $zero, loop # loop if number of elements != 0
sw $s0, 0($gp)
li $v0, 10
syscall