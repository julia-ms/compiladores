// Júlia Mombach da Silva
// ast.cpp

#include "ast.hpp"
#include <stdio.h>

using namespace std;

string ASTtypeNAMES[] = {
    "AST_ROOT",
    "AST_TYPE",
    "AST_SYMBOL",
    "AST_VAR_DEC",
    "AST_TYPE_INT",
    "AST_TYPE_BYTE",
    "AST_TYPE_REAL",
    "AST_VECTOR_DEC",
    "AST_VECTOR_SIZE",
    "AST_VECTOR_INIT",
    "AST_FUNCTION_DEC",
    "AST_PARAM",
    "AST_PARAM_LIST",
    "AST_COMMAND_BLOCK",
    "AST_COMMAND_LIST",
    "AST_COMMAND",
    "AST_COMMAND_EQ",
    "AST_EXPRESSION",
    "AST_ADD",
    "AST_SUB", 
    "AST_MULT",
    "AST_DIV", 
    "AST_LESS",
    "AST_GREATER",
    "AST_AND",
    "AST_OR", 
    "AST_NOT",
    "AST_DIF", 
    "AST_EQ", 
    "AST_GE",
    "AST_LE",
};



void astPrint(AST* node, int level) {
    if (!node) return;

    for (int i = 0; i < level; i++) fprintf(stderr, "    ");
    
    fprintf(stderr, "AST: %s, %s\n",
        ASTtypeNAMES[node->type].c_str(),
        node->symbol ? node->symbol->text.c_str() : "");

    for (auto s : node->son) {
        astPrint(s, level + 1);
    }
}
