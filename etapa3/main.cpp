// Julia Mombach da Silva 
// /mnt/c/Users/julia/OneDrive/Documentos/GitHub/compiladores/etapa3

#include <stdio.h>
#include <stdlib.h>
#include "symbols.hpp"
#include "ast.hpp"

extern FILE* yyin;
extern char* yytext;
extern int yylex();
extern int lineNumber;

int getLineNumber();
int isRunning();
int yywrap();
void initMe();
extern int yyparse();
extern AST* root;


int main(int argc, char** argv){
    initMe();

    if (argc < 3){
        fprintf(stderr, "Call %s filename\n", argv[0]); 
        exit(1);
    } 

    //entrada
    yyin = fopen(argv[1], "r");
    if(yyin == 0){
        fprintf(stderr, "Error opening read file");
        exit(1);
    }

    //saida
    FILE* output = fopen(argv[2], "w");
    if(output == 0){
        fprintf(stderr, "Error opening out file");
        exit(1);
    }

    yyparse();
    
    fprintf(stderr, "File had %d lines\n", lineNumber);

    printf("\nSymbols table: \n\n");
    printSymbolsTable();

    printf("\nTree: \n\n");

    if (root) {
        fprintf(stderr, "Árvore gerada:\n");
        astPrint(root);
    } else {
        fprintf(stderr, "Nenhuma AST gerada.\n");
    }

    astToFile(root, output);

    fclose(yyin);
    fclose(output);
    

    exit(0);
}