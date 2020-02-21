.global start
.equ N, 10

.data
a: .word 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
b: .word -4, -5, -6, -7, -8, -9, -10, -11, -12, -13

.bss
c: .space 4 * N

.text

start: mov r0, #0 @i
	   ldr r1, =a
	   ldr r2, =b
	   ldr r3, =c
	   mov r4, #9
 for: cmp r0, #N
	  bge fin_for
	  sub r5, r4, r0 @9-i
	  ldr r6, [r1,r0,lsl#2]
	  ldr r7, [r2,r5,lsl#2]
	  add r6,r6,r7
	  cmp r6, #0
	  blt abs
str:  str r6, [r3,r0,lsl#2]
	  add r0,r0,#1
	  b for
abs:  mov r7, #-1
	  mul r6,r7,r6
	  b str
fin_for: b .
		.end

