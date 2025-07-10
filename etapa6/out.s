#VARIABLES
	.data
k:	.long 0
temp0:	.long 0
temp1:	.long 0
temp10:	.long 0
temp11:	.long 0
temp12:	.long 0
temp13:	.long 0
temp14:	.long 0
temp15:	.long 0
temp16:	.long 0
temp17:	.long 0
temp18:	.long 0
temp19:	.long 0
temp2:	.long 0
temp20:	.long 0
temp21:	.long 0
temp22:	.long 0
temp23:	.long 0
temp24:	.long 0
temp25:	.long 0
temp26:	.long 0
temp27:	.long 0
temp28:	.long 0
temp29:	.long 0
temp3:	.long 0
temp30:	.long 0
temp31:	.long 0
temp32:	.long 0
temp33:	.long 0
temp34:	.long 0
temp35:	.long 0
temp4:	.long 0
temp5:	.long 0
temp6:	.long 0
temp7:	.long 0
temp8:	.long 0
temp9:	.long 0
x:	.long 0

print_int:
	.string "%d"
print_string:
	.string "%s"
scan_fmt:
	.string "%d"

	.text

	#TAC_BEGIN_FUNC
	.globl main
main:
	pushq   %rbp
	movq    %rsp, %rbp
	movl $8, %eax
	addl $7, %eax
	movl %eax, temp0(%rip)
	#TAC_ASS (variable)
	movl temp0(%rip), %eax
	movl %eax, x(%rip)
	movl $8, %eax
	addl $7, %eax
	movl %eax, temp1(%rip)
	#TAC_ASS (variable)
	movl temp1(%rip), %eax
	movl %eax, x(%rip)
	movl $8, %eax
	addl $7, %eax
	movl %eax, temp2(%rip)
	#TAC_ASS (variable)
	movl temp2(%rip), %eax
	movl %eax, x(%rip)
	movl $8, %eax
	addl $7, %eax
	movl %eax, temp3(%rip)
	#TAC_ASS (variable)
	movl temp3(%rip), %eax
	movl %eax, x(%rip)
	movl $8, %eax
	addl $7, %eax
	movl %eax, temp4(%rip)
	#TAC_ASS (variable)
	movl temp4(%rip), %eax
	movl %eax, x(%rip)
	movl $8, %eax
	addl $7, %eax
	movl %eax, temp5(%rip)
	#TAC_ASS (variable)
	movl temp5(%rip), %eax
	movl %eax, x(%rip)
	movl $8, %eax
	addl $7, %eax
	movl %eax, temp6(%rip)
	#TAC_ASS (variable)
	movl temp6(%rip), %eax
	movl %eax, x(%rip)
	movl $8, %eax
	addl $7, %eax
	movl %eax, temp7(%rip)
	#TAC_ASS (variable)
	movl temp7(%rip), %eax
	movl %eax, x(%rip)
	movl $8, %eax
	addl $7, %eax
	movl %eax, temp8(%rip)
	#TAC_ASS (variable)
	movl temp8(%rip), %eax
	movl %eax, x(%rip)
	movl $8, %eax
	addl $7, %eax
	movl %eax, temp9(%rip)
	#TAC_ASS (variable)
	movl temp9(%rip), %eax
	movl %eax, x(%rip)
	movl $8, %eax
	addl $7, %eax
	movl %eax, temp10(%rip)
	#TAC_ASS (variable)
	movl temp10(%rip), %eax
	movl %eax, x(%rip)
	movl $8, %eax
	addl $7, %eax
	movl %eax, temp11(%rip)
	#TAC_ASS (variable)
	movl temp11(%rip), %eax
	movl %eax, x(%rip)
	movl $8, %eax
	addl $7, %eax
	movl %eax, temp12(%rip)
	#TAC_ASS (variable)
	movl temp12(%rip), %eax
	movl %eax, x(%rip)
	movl $8, %eax
	addl $7, %eax
	movl %eax, temp13(%rip)
	#TAC_ASS (variable)
	movl temp13(%rip), %eax
	movl %eax, x(%rip)
	movl $8, %eax
	addl $7, %eax
	movl %eax, temp14(%rip)
	#TAC_ASS (variable)
	movl temp14(%rip), %eax
	movl %eax, x(%rip)
	movl $8, %eax
	addl $7, %eax
	movl %eax, temp15(%rip)
	#TAC_ASS (variable)
	movl temp15(%rip), %eax
	movl %eax, x(%rip)
	movl $8, %eax
	addl $7, %eax
	movl %eax, temp16(%rip)
	#TAC_ASS (variable)
	movl temp16(%rip), %eax
	movl %eax, x(%rip)
	movl $8, %eax
	addl $7, %eax
	movl %eax, temp17(%rip)
	#TAC_ASS (variable)
	movl temp17(%rip), %eax
	movl %eax, x(%rip)
	movl $8, %eax
	addl $7, %eax
	movl %eax, temp18(%rip)
	#TAC_ASS (variable)
	movl temp18(%rip), %eax
	movl %eax, x(%rip)
	movl $8, %eax
	addl $7, %eax
	movl %eax, temp19(%rip)
	#TAC_ASS (variable)
	movl temp19(%rip), %eax
	movl %eax, x(%rip)
	movl $8, %eax
	addl $7, %eax
	movl %eax, temp20(%rip)
	#TAC_ASS (variable)
	movl temp20(%rip), %eax
	movl %eax, x(%rip)
	movl $8, %eax
	addl $7, %eax
	movl %eax, temp21(%rip)
	#TAC_ASS (variable)
	movl temp21(%rip), %eax
	movl %eax, x(%rip)
	movl $8, %eax
	addl $7, %eax
	movl %eax, temp22(%rip)
	#TAC_ASS (variable)
	movl temp22(%rip), %eax
	movl %eax, x(%rip)
	movl $8, %eax
	addl $7, %eax
	movl %eax, temp23(%rip)
	#TAC_ASS (variable)
	movl temp23(%rip), %eax
	movl %eax, x(%rip)
	movl $8, %eax
	addl $7, %eax
	movl %eax, temp24(%rip)
	#TAC_ASS (variable)
	movl temp24(%rip), %eax
	movl %eax, x(%rip)
	movl $8, %eax
	addl $7, %eax
	movl %eax, temp25(%rip)
	#TAC_ASS (variable)
	movl temp25(%rip), %eax
	movl %eax, x(%rip)
	movl $8, %eax
	addl $7, %eax
	movl %eax, temp26(%rip)
	#TAC_ASS (variable)
	movl temp26(%rip), %eax
	movl %eax, x(%rip)
	movl $8, %eax
	addl $7, %eax
	movl %eax, temp27(%rip)
	#TAC_ASS (variable)
	movl temp27(%rip), %eax
	movl %eax, x(%rip)
	movl $8, %eax
	addl $7, %eax
	movl %eax, temp28(%rip)
	#TAC_ASS (variable)
	movl temp28(%rip), %eax
	movl %eax, x(%rip)
	movl $8, %eax
	addl $7, %eax
	movl %eax, temp29(%rip)
	#TAC_ASS (variable)
	movl temp29(%rip), %eax
	movl %eax, x(%rip)
	movl $8, %eax
	addl $7, %eax
	movl %eax, temp30(%rip)
	#TAC_ASS (variable)
	movl temp30(%rip), %eax
	movl %eax, x(%rip)
	movl $8, %eax
	addl $7, %eax
	movl %eax, temp31(%rip)
	#TAC_ASS (variable)
	movl temp31(%rip), %eax
	movl %eax, x(%rip)
	movl $8, %eax
	addl $7, %eax
	movl %eax, temp32(%rip)
	#TAC_ASS (variable)
	movl temp32(%rip), %eax
	movl %eax, x(%rip)
	movl $8, %eax
	addl $7, %eax
	movl %eax, temp33(%rip)
	#TAC_ASS (variable)
	movl temp33(%rip), %eax
	movl %eax, x(%rip)
	movl $8, %eax
	addl $7, %eax
	movl %eax, temp34(%rip)
	#TAC_ASS (variable)
	movl temp34(%rip), %eax
	movl %eax, x(%rip)
	movl $8, %eax
	addl $7, %eax
	movl %eax, temp35(%rip)
	#TAC_ASS (variable)
	movl temp35(%rip), %eax
	movl %eax, x(%rip)

	#TAC_PRINT
	movl x(%rip), %esi
	leaq print_int(%rip), %rdi
	movl $0, %eax
	call printf

	#TAC_END_FUNC
	movl    $0, %eax
	popq    %rbp
	ret
