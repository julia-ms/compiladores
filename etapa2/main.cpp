// Julia Mombach da Silva 
// /mnt/c/Users/julia/OneDrive/Documentos/GitHub/compiladores/etapa1/trabalho

#include <stdio.h>
#include <stdlib.h>
#include "symbols.hpp"

extern FILE* yyin;
extern char* yytext;
extern int yylex();
extern int lineNumber;

int getLineNumber();
int isRunning();
int yywrap();
void initMe();
extern int yyparse();


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

    yyparse();
    
    fprintf(stderr, "File had %d lines\n", lineNumber);

    printf("\nSymbols table: \n\n");
    printSymbolsTable();
    return 0;
}