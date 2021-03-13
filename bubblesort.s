.data
array: .word 5, 3, 1, 6, 2, 8, 9, 10, 1, 8

.text
.globl main
main:
    la $t1,array
    li $s1,40
    li $s2,0
    li $s3,4
    bne $s1,$s2,L2
L2: 
    beq $s1,$s2,out
    add $t1,$t1,$s2
    lw $t0,0($t1)   #a[i]
L3: 
    beq $s3,$s1,incc
    add $t1,$t1,$s3
    lw $t2,0($t1)   #a[j]
    slt $t3,$t0,$t2
    beq $t3,$0,swap
L4: 
    addi $s3,$s3,4
    j L3

swap:   
    sw $t0,0($t1)
    sub $t1,$t1,$s2
    sw $t2,0($t1)
    j L4

incc:
    addi $s2,$s2,4
    j L2


out:
li $v0,10
syscall

