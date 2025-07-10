// tac.hpp
// Júlia Mombach da Silva

#ifndef TAC_HEADER
#define TAC_HEADER

#include "symbols.hpp"
#include "ast.hpp"
#include <string>
#include <vector>

#define MAX_SONS 4

using namespace std;

enum TACtypes {
    TAC_ROOT,
    TAC_SYMBOL,
    TAC_EXPRESSION,
    TAC_VAR_DEC,
    TAC_TYPE_INT,
    TAC_ADD,
    TAC_SUB,
    TAC_MULT,
    TAC_DIV,
    TAC_LESS,
    TAC_GREATER,
    TAC_AND,
    TAC_OR, 
    TAC_NOT,
    TAC_DIF, 
    TAC_EQ, 
    TAC_GE,
    TAC_LE,
    TAC_IF0,
    TAC_LABEL,
    TAC_ASS,
    TAC_PRINT,
    TAC_EXP_VEC,
    TAC_VECTOR_DEC,
    TAC_VECTOR_INIT,
    TAC_BEGIN_FUNC,
    TAC_END_FUNC,
    TAC_PARAM,
    TAC_FUNC_CALL,
    TAC_ARG,
    TAC_READ,
    TAC_RETURN,
    TAC_JUMP,
    TAC_IF1,
    TAC_ASS_DEC,
    TAC_VECTOR_ASS
};

struct TAC {
    int type;
    SYMBOL* res;
    SYMBOL* op1;
    SYMBOL* op2;
    TAC* prev;
    TAC* next;

    TAC(int t, SYMBOL* r, SYMBOL* o1, SYMBOL* o2)
        : type(t), res(r), op1(o1), op2(o2), prev(nullptr), next(nullptr) {}
};

void tacPrintSingle(TAC* tac);
void tacPrintBackwards(TAC* tac);
TAC* tacJoin(TAC* l1, TAC* l2);
TAC* generateCode(AST* node);
TAC* makeIfThen(TAC* code0, TAC* code1);
void freeTAC(TAC* tac);
TAC* tacReverse(TAC* tac);

#endif
