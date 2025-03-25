// Julia Mombach da Silva (00281023)
// /mnt/c/Users/julia/OneDrive/Documentos/GitHub/compiladores/etapa1/trabalho

#include <stdio.h>
#include <stdlib.h>
#include "tokens.h"

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
        case KW_BYTE: fprintf(stderr, "line %d: KW_BYTE \n", getLineNumber());
            break;
        case KW_INT: fprintf(stderr, "line %d: KW_INT \n", getLineNumber());
            break;
        case KW_REAL: fprintf(stderr, "line %d: KW_REAL \n", getLineNumber());
            break;
        case KW_IF: fprintf(stderr, "line %d: KW_IF \n", getLineNumber());
            break;
        case KW_ELSE: fprintf(stderr, "line %d: KW_ELSE \n", getLineNumber());
            break;
        case KW_DO: fprintf(stderr, "line %d: KW_DO \n", getLineNumber());
            break;
        case KW_WHILE: fprintf(stderr, "line %d: KW_WHILE \n", getLineNumber());
            break;
        case KW_READ: fprintf(stderr, "line %d: KW_READ \n", getLineNumber());
            break;
        case KW_PRINT: fprintf(stderr, "line %d: KW_PRINT \n", getLineNumber());
            break;
        case KW_RETURN: fprintf(stderr, "line %d: KW_RETURN \n", getLineNumber());
            break;
        case OPERATOR_LE: fprintf(stderr, "line %d: OPERATOR_LE \n", getLineNumber());
            break;
        case OPERATOR_GE: fprintf(stderr, "line %d: OPERATOR_GE \n", getLineNumber());
            break;
        case OPERATOR_EQ: fprintf(stderr, "line %d: OPERATOR_EQ \n", getLineNumber());
            break;
        case OPERATOR_DIF: fprintf(stderr, "line %d: OPERATOR_DIF \n", getLineNumber());
            break;
        case TK_IDENTIFIER: fprintf(stderr, "line %d: TK_IDENTIFIER \n", getLineNumber());
            break;
        case LIT_INT: fprintf(stderr, "line %d: LIT_INT \n", getLineNumber());
            break;
        case LIT_CHAR: fprintf(stderr, "line %d: LIT_CHAR \n", getLineNumber());
            break;
        case LIT_REAL: fprintf(stderr, "line %d: LIT_REAL \n", getLineNumber());
            break;
        case LIT_STRING: fprintf(stderr, "line %d: LIT_STRING \n", getLineNumber());
            break;
        case TOKEN_ERROR: fprintf(stderr, "line %d: TOKEN_ERROR \n", getLineNumber());
            break;
        
        default: fprintf(stderr, "line %d: %d %s \n", getLineNumber(), tok, yytext);
            break;
        }
    }

    fprintf(stderr, "File had %d lines\n", lineNumber);
    return 0;
}