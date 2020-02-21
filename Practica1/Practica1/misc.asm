
	.equ COEF1, 3483
	.equ COEF2, 11718
	.equ COEF3, 1183

	.text

	.global rgb2gray
	.global div16384
	.global apply_gaussian


 div16348:
 	lsr r0, r0, #14
 	mov pc, lr



 rgb2gray:
 	push {r4-r9,lr}
 	ldrb r1, [r0] @R
 	ldrb r2, [r0, #1] @G
 	ldrb r3, [r0, #2] @B
 	ldr r4, =COEF1
 	ldr r5, =COEF2
 	ldr r6, =COEF3
 	mul r7, r1, r4
 	mul r8, r2, r5
 	mul r9, r3, r6
 	add r0, r7, r8
 	add r0, r0, r9
 	bl div16348
 	pop {r4-r9,lr}
 	mov pc, lr

apply_gaussian:
 	  push {r4-r11, lr}
 	  mov r5, r0 @im1
 	  mov r6, r1 @im2
 	  mov r8, r2 @width
 	  mov r9, r3 @height
 	  mov r10, #0 @i
for_i:cmp r10, r9
	  bge fin_i
	  mov r11, #0 @j
for_j:cmp r11, r8
	  bge fin_j
	  mul r7, r8, r10 @i*width
	  add r7, r7, r11 @i*width + j
	  mov r0, r5 @r0<-im1
	  mov r1, r8 @r1<-width
  	  mov r2, r9 @r2<-height
 	  mov r3, r10 @r3<-i
 	  mov r4, r11 @r4<-j
	  push {r4} @guardo r4
	  bl gaussian
	  pop {r4} @devuelvo r4
	  strb r0, [r6, r7] @guardo la devolucion en im2[i*width+j]
	  add r11, r11, #1 @j+1
	  b for_j
fin_j:add r10, r10, #1 @i+1
	  b for_i
fin_i:pop {r4-r11,lr}
	  mov pc, lr

	b .
  		.end



