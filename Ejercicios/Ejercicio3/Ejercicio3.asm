.global start
.equ N, 3

.data
a: .word 1, 2, 3, 4, 5, 6, 7, 8, 9

.bss
b: .space 4 * N * N

.text

start: mov r0, #0 @i
	   mov r1, #0 @j
	   ldr r2, =a
	   ldr r3, =b
for1:  cmp r0,#N
	   bge fin
for2:  cmp r1,#N
	   bge ffor2
	   mov r4,#N
	   mul r5,r0,r4
	   add r5,r5,r1
	   ldr r6,[r2,r5,lsl#2]
	   mov r5,#0
	   mul r5,r0,r4
	   add r5,r5,r1
	   str r6,[r3,r5,lsl#2]
	   mov r5,#0
	   add r1,r1,#1
	   b for2
ffor2: mov r1,#0
	   add r0,r0,#1
	   b for1
fin:   b .
	   .end

