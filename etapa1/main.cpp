// Julia Mombach da Silva 
// /mnt/c/Users/julia/OneDrive/Documentos/GitHub/compiladores/etapa1/trabalho

#include <stdio.h>
#include <stdlib.h>
#include "tokens.h"
#include "symbols.hpp"

extern FILE* yyin;
extern char* yytext;
extern int yylex();
extern int lineNumber;

int getLineNumber();
int isRunning();
int yywrap();
void initMe();


int main(int argc, char** argv){
    initMe();
    int tok;

    if (argc < 2){
        fprintf(stderr, "Call %s filename\n", argv[0]); 
        exit(1);
    } 

    yyin = fopen(argv[1], "r");

    if(yyin == 0){
        fprintf(stderr, "Error opening file");
        exit(1);
    }

    while(isRunning()){
        tok = yylex();
        if(!isRunning()) break;

        switch (tok){
        case KW_BYTE: fprintf(stderr, "line %d: %s -> KW_BYTE \n", getLineNumber(), yytext);
            break;
        case KW_INT: fprintf(stderr, "line %d: %s -> KW_INT \n", getLineNumber(), yytext);
            break;
        case KW_REAL: fprintf(stderr, "line %d: %s -> KW_REAL \n", getLineNumber(), yytext);
            break;
        case KW_IF: fprintf(stderr, "line %d: %s -> KW_IF \n", getLineNumber(), yytext);
            break;
        case KW_ELSE: fprintf(stderr, "line %d: %s -> KW_ELSE \n", getLineNumber(), yytext);
            break;
        case KW_DO: fprintf(stderr, "line %d: %s -> KW_DO \n", getLineNumber(), yytext);
            break;
        case KW_WHILE: fprintf(stderr, "line %d: %s -> KW_WHILE \n", getLineNumber(), yytext);
            break;
        case KW_READ: fprintf(stderr, "line %d: %s -> KW_READ \n", getLineNumber(), yytext);
            break;
        case KW_PRINT: fprintf(stderr, "line %d: %s -> KW_PRINT \n", getLineNumber(), yytext);
            break;
        case KW_RETURN: fprintf(stderr, "line %d: %s -> KW_RETURN \n", getLineNumber(), yytext);
            break;
        case OPERATOR_LE: fprintf(stderr, "line %d: %s -> OPERATOR_LE \n", getLineNumber(), yytext);
            break;
        case OPERATOR_GE: fprintf(stderr, "line %d: %s -> OPERATOR_GE \n", getLineNumber(), yytext);
            break;
        case OPERATOR_EQ: fprintf(stderr, "line %d: %s -> OPERATOR_EQ \n", getLineNumber(), yytext);
            break;
        case OPERATOR_DIF: fprintf(stderr, "line %d: %s -> OPERATOR_DIF \n", getLineNumber(), yytext);
            break;
        case TK_IDENTIFIER: fprintf(stderr, "line %d: %s -> TK_IDENTIFIER \n", getLineNumber(), yytext);
            break;
        case LIT_INT: fprintf(stderr, "line %d: %s -> LIT_INT \n", getLineNumber(), yytext);
            break;
        case LIT_CHAR: fprintf(stderr, "line %d: %s -> LIT_CHAR \n", getLineNumber(), yytext);
            break;
        case LIT_REAL: fprintf(stderr, "line %d: %s -> LIT_REAL \n", getLineNumber(), yytext);
            break;
        case LIT_STRING: fprintf(stderr, "line %d: %s -> LIT_STRING \n", getLineNumber(), yytext);
            break;
        case TOKEN_ERROR: fprintf(stderr, "line %d: %s -> TOKEN_ERROR \n", getLineNumber(), yytext);
            break;
        default: fprintf(stderr, "line %d: %s -> %s \n", getLineNumber(), yytext, yytext);
            break;
        }
    }
    
    fprintf(stderr, "File had %d lines\n", lineNumber);

    printf("\nSymbols table: \n\n");
    printSymbolsTable();
    return 0;
}