#VARIABLES
	.data
i:	.long 0
string0:	.string "\n"
string1:	.string "entrei\n"
string2:	.string "resultado: "
temp0:	.long 0
temp1:	.long 0
temp2:	.long 0
temp3:	.long 0
var1:	.long 5
var2:	.long 1
x:	.long 0


print_int:
	.string "%d"
print_string:
	.string "%s"

	.text

	#TAC_BEGIN_FUNC
	.globl main
main:
	pushq   %rbp
	movq    %rsp, %rbp
label0:
	# temp0 = (i op 5)
	movl i(%rip), %eax
	cmpl $5, %eax
	setl %al
	movzbl %al, %eax
	movl %eax, temp0(%rip)
	#TAC_IF0
	movl temp0(%rip), %eax
	cmpl $0, %eax
	je label1
	movl x(%rip), %eax
	addl var1(%rip), %eax
	movl %eax, temp1(%rip)
	#TAC_ASS (variable)
	movl temp1(%rip), %eax
	movl %eax, x(%rip)
	movl i(%rip), %eax
	addl $1, %eax
	movl %eax, temp2(%rip)
	#TAC_ASS (variable)
	movl temp2(%rip), %eax
	movl %eax, i(%rip)
	#TAC_JUMP
	jmp label0
label1:
	# temp3 = (x op 999)
	movl x(%rip), %eax
	cmpl $999, %eax
	setl %al
	movzbl %al, %eax
	movl %eax, temp3(%rip)
	#TAC_IF0
	movl temp3(%rip), %eax
	cmpl $0, %eax
	je label2

	#TAC_PRINT
	leaq string1(%rip), %rsi
	leaq print_string(%rip), %rdi
	movl $0, %eax
	call printf
label2:

	#TAC_PRINT
	leaq string2(%rip), %rsi
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
