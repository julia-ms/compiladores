// Júlia Mombach da Silva

// ast.hpp

#ifndef AST_HEADER
#define AST_HEADER

#include "symbols.hpp"
#include <vector>

using namespace std;

enum ASTtypes {
    AST_ROOT,
    AST_SYMBOL,
    AST_VAR_DEC,
    AST_TYPE_INT,
    AST_TYPE_BYTE,
    AST_TYPE_REAL,
    AST_VECTOR_DEC,
    AST_VECTOR_SIZE,
    AST_VECTOR_INIT,
    AST_FUNCTION_DEC,
    AST_PARAM,
    AST_PARAM_LIST,
    AST_COMMAND_BLOCK,
    AST_COMMAND_LIST,
    AST_COMMAND,
    AST_COMMAND_EQ,
    AST_COMMAND_VEC_EQ,
    AST_COMMAND_READ,
    AST_COMMAND_RETURN,
    AST_COMMAND_PRINT,
    AST_EXPRESSION,
    AST_LIT,
    AST_EXPRESSION_VEC,
    AST_ADD,
    AST_SUB, 
    AST_MULT,
    AST_DIV, 
    AST_LESS,
    AST_GREATER,
    AST_AND,
    AST_OR, 
    AST_NOT,
    AST_DIF, 
    AST_EQ, 
    AST_GE,
    AST_LE,
    AST_PARENTHESIS,
    AST_FUNCTION_CALL,
    AST_FUNCTION_ARGS,
    AST_PARAM2,
    AST_PRINT,
    AST_COMMAND_IF,
    AST_COMMAND_IF_ELSE,
    AST_COMMAND_WHILE_DO,
    AST_COMMAND_DO_WHILE,
    AST_COMMAND_PV,
};

struct AST {
    int type;
    vector<AST*> son;
    SYMBOL* symbol;

    AST(int t, vector<AST*> v, SYMBOL* s = nullptr)
        : type(t), son(v), symbol(s) {}
};

void astPrint(AST* node, int level = 0);

void astToFile(AST* node, FILE* file);

char* reverseeINT(char* num);

char* reverseeREAL(char* num);

#endif
