.global start
.equ N, 16

.data
a: .word 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16

.text

start: mov r4, #0 @i
	   mov r5, #0 @j
	   ldr r0, =a
for_ext: cmp r4, #N
	 	 bge fin_for_ext
for_int: cmp r5,#N
 		 bge fin_for_int
 		 mov r1,r4
 		 mov r2,r5
 		 bl swap
 		 add r5,r5,#1
		 b for_int
fin_for_int: add r4, r4, #1
			 mov r5, r4
			 b for_ext
fin_for_ext: b .

swap: push {r4,r5}
	  ldr r4,[r0,r1,lsl#2]
	  ldr r5,[r0,r2,lsl#2]
	  str r5,[r0,r1,lsl#2]
	  str r4,[r0,r2,lsl#2]
	  pop {r4,r5}
	  mov pc, lr

			 .end






