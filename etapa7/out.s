#VARIABLES
	.data
k:	.long 0
temp0:	.long 0
temp1:	.long 0
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
	#TAC_ASS (literal)
	movl $15, %eax
	movl %eax, temp0(%rip)
	movl temp0(%rip), %eax
	addl $5, %eax
	movl %eax, temp1(%rip)
	#TAC_ASS (variable)
	movl temp1(%rip), %eax
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
