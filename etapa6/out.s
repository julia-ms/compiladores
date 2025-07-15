#VARIABLES
	.data
a:	.long 7
k:	.long 0
string0:	.string "\n"
string1:	.string "voltei pra main "
string2:	.string "y: "
string3:	.string "z: "
temp0:	.long 0
temp1:	.long 0
temp2:	.long 0
temp3:	.long 0
temp4:	.long 0
temp5:	.long 0
v:
	.long 11
	.long 80
	.long 33
	.long 44
	.long 55
x:	.long 4
y:	.long 0
z:	.long 0

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
	movl $3, %eax
	movl %eax, x(%rip)
	# TAC_VECTOR_ASS (literal index, literal value)
	movl $50, v+8(%rip)
	#TAC_ARG (literal)
	movl $2, %eax
	movl %eax, y(%rip)
	#TAC_ARG (variable)
	movl a(%rip), %eax
	movl %eax, z(%rip)
	call dois
	movl %eax, temp0(%rip)
	#TAC_ASS (variable)
	movl temp0(%rip), %eax
	movl %eax, x(%rip)

	#TAC_PRINT
	leaq string1(%rip), %rsi
	leaq print_string(%rip), %rdi
	movl $0, %eax
	call printf

	#TAC_PRINT
	movl x(%rip), %esi
	leaq print_int(%rip), %rdi
	movl $0, %eax
	call printf

	#TAC_PRINT
	leaq string0(%rip), %rsi
	leaq print_string(%rip), %rdi
	movl $0, %eax
	call printf

	#TAC_END_FUNC
	movl    $0, %eax
	popq    %rbp
	ret

	#TAC_BEGIN_FUNC
	.globl dois
dois:
	pushq   %rbp
	movq    %rsp, %rbp

	#TAC_PRINT
	leaq string2(%rip), %rsi
	leaq print_string(%rip), %rdi
	movl $0, %eax
	call printf

	#TAC_PRINT
	movl y(%rip), %esi
	leaq print_int(%rip), %rdi
	movl $0, %eax
	call printf

	#TAC_PRINT
	leaq string0(%rip), %rsi
	leaq print_string(%rip), %rdi
	movl $0, %eax
	call printf

	#TAC_PRINT
	leaq string3(%rip), %rsi
	leaq print_string(%rip), %rdi
	movl $0, %eax
	call printf

	#TAC_PRINT
	movl z(%rip), %esi
	leaq print_int(%rip), %rdi
	movl $0, %eax
	call printf

	#TAC_PRINT
	leaq string0(%rip), %rsi
	leaq print_string(%rip), %rdi
	movl $0, %eax
	call printf
	movl y(%rip), %eax
	addl z(%rip), %eax
	movl %eax, temp1(%rip)

	# TAC_EXP_VEC
	movl v+8(%rip), %eax
	movl %eax, temp2(%rip)
	movl temp1(%rip), %eax
	addl temp2(%rip), %eax
	movl %eax, temp3(%rip)

	# TAC_EXP_VEC
	movl v+4(%rip), %eax
	movl %eax, temp4(%rip)
	movl temp3(%rip), %eax
	addl temp4(%rip), %eax
	movl %eax, temp5(%rip)
	# TAC_RETURN
	movl temp5(%rip), %eax
	popq %rbp
	ret

	#TAC_END_FUNC
	movl    $0, %eax
	popq    %rbp
	ret
