# DO NOT REMOVE THIS LINE WHEN SUBMITTING
.import input.S


.text  # code starts here
  

.globl main
main:
    la a2 str1
    la a3 str2
    lw a4 len1
    lw a5 len2
    ##
    li s11, 4
    mul a4, a4, s11
    mul a5, a5, s11
    #the tail
    add a4, a4, a2
    add a5, a5, a3
    #a4,a5 no longer record the length, but the tail of the list
    ##
    jal func

    li a0 1
    ecall
    li a0 10
    ecall

func:
    addi a0 zero 0
    addi sp, sp, -4
    sw ra 0(sp)#store the ra
    ####
    li a1, 0#max = 0

    add t1, a2, zero#t1---i
    add t2, a3, zero#t2---j

LOOP1:
    beq t1, a4, exit#if i == l_a
    jal LOOP2
    addi t1, t1, 4#i++
    j LOOP1


LOOP2:

    blt t2, a5, continous # if a3 < a5 then target
    add t2, a3, zero
    jr ra# back to loop1
    continous:

    addi t3, t1, 0#m
    addi t4, t2, 0#n
    addi s1, zero,0#length = 0

    while:
        lw s2, 0(t3)#a[m]
        lw s3, 0(t4)#b[n]
        bne s2, s3, next # if a[m] != b[n]
        addi t3, t3, 4
        addi t4, t4, 4
        addi s1, s1, 1
        j while

    next:
        addi t2,t2,4#j++
        bge s1, a1, target # if s1 >= a1 then target
            j LOOP2
        target:
            addi a1, s1, 0
            j LOOP2
        

exit:
    ####
    lw ra, 0(sp)#give ra back
    jr ra



