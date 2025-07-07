    .data
var1: .long 8
var2: .long 3

    .section .rodata
fmt:
    .string "%d e %d\n"
msg:
    .string "oi"

    .text
    .globl main
main:
    pushq   %rbp
    movq    %rsp, %rbp

    movl    var2(%rip), %edx      # var2 → %edx
    movl    var1(%rip), %eax      # var1 → %eax
    movl    %eax, %esi            # %esi = var1
    leaq    fmt(%rip), %rdi       # %rdi = endereço do formato
    movl    $0, %eax              # limpar eax para printf
    call    printf

    leaq    msg(%rip), %rdi       # %rdi = endereço da string "oi"
    call    puts

    movl    $0, %eax
    popq    %rbp
    ret
