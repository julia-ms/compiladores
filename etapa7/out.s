#VARIABLES
	.data
a:	.long 0
b:	.long 0
c:	.long 0
d:	.long 0
i:	.long 0
k:	.long 0
temp0:	.long 0
temp1:	.long 0
temp2:	.long 0
temp3:	.long 0
temp4:	.long 0
temp5:	.long 0

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
label0:
	# temp0 = (i op 10000)
	movl i(%rip), %eax
	cmpl $10000, %eax
	setl %al
	movzbl %al, %eax
	movl %eax, temp0(%rip)
	#TAC_IF0
	movl temp0(%rip), %eax
	cmpl $0, %eax
	je label1
	#TAC_ASS (literal)
	movl $15, %eax
	movl %eax, a(%rip)
	#TAC_ASS (literal)
	movl $15, %eax
	movl %eax, b(%rip)
	#TAC_ASS (literal)
	movl $15, %eax
	movl %eax, c(%rip)
	#TAC_ASS (literal)
	movl $15, %eax
	movl %eax, d(%rip)
	movl i(%rip), %eax
	addl $1, %eax
	movl %eax, temp5(%rip)
	#TAC_ASS (variable)
	movl temp5(%rip), %eax
	movl %eax, i(%rip)
	#TAC_JUMP
	jmp label0
label1:

	#TAC_PRINT
	movl a(%rip), %esi
	leaq print_int(%rip), %rdi
	movl $0, %eax
	call printf

	#TAC_END_FUNC
	movl    $0, %eax
	popq    %rbp
	ret
