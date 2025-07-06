// tac.cpp
// Julia Mombach da Silva

#include "tac.hpp"
#include <cstdio>

string TACtypeNames[] = {
    "TAC_ROOT",
    "TAC_SYMBOL",
    "TAC_EXPRESSION",
    "TAC_VAR_DEC",
    "TAC_TYPE_INT",
    "TAC_ADD",
    "TAC_SUB",
    "TAC_MULT",
    "TAC_DIV",
    "TAC_LESS",
    "TAC_GREATER",
    "TAC_AND",
    "TAC_OR", 
    "TAC_NOT",
    "TAC_DIF", 
    "TAC_EQ", 
    "TAC_GE",
    "TAC_LE",
    "TAC_IF0",
    "TAC_LABEL",
    "TAC_ASS",
    "TAC_PRINT",
    "TAC_EXP_VEC",
    "TAC_VECTOR_DEC",
    "TAC_VECTOR_INIT",
    "TAC_BEGIN_FUNC",
    "TAC_END_FUNC",
    "TAC_PARAM",
    "TAC_FUNC_CALL",
    "TAC_ARG",
    "TAC_READ",
    "TAC_RETURN",
    "TAC_JUMP",
    "TAC_IF1"

}; 

TAC* makeVectorDec(AST* node);
TAC* makeVecAssign(AST* node);
TAC* binOperations(int type, TAC* code1, TAC* code2);
TAC* makeFunc(AST* node, TAC* code1, TAC* code2);
TAC* makeFuncCall(AST* node);
TAC* makeIfThen(TAC* code0, TAC* code1);
TAC* makeIfThenElse(TAC* code0, TAC* code1, TAC* code2);
TAC* makeWhileDo(TAC* code0, TAC* code1);
TAC* makeDoWhile(TAC* bodyCode, TAC* condCode);
TAC* makePrint(AST* node);

//----------------------------------------------------------------------------------------------------------------------------------
// FUNCTIONS
//----------------------------------------------------------------------------------------------------------------------------------

TAC* tacJoin(TAC* l1, TAC* l2){
    TAC* t;
    if(!l1) return l2;
    if(!l2) return l1;
    for(t = l2; t ->prev; t = t->prev) ;
    t->prev = l1;
    return l2;
}

void tacPrintSingle(TAC* tac) {
    if (!tac) return;

    if (tac->type < 0 || tac->type >= (int)(sizeof(TACtypeNames) / sizeof(string))) {
        fprintf(stderr, "ERRO: Tipo de TAC inválido: %d\n", tac->type);
        return;
    }
    fprintf(stderr, "TAC(%s, %s, %s, %s)\n",
        TACtypeNames[tac->type].c_str(),
        tac->res ? tac->res->text.c_str() : "",
        tac->op1 ? tac->op1->text.c_str() : "",
        tac->op2 ? tac->op2->text.c_str() : "");
}

void tacPrintBackwards(TAC* tac){
    for(; tac; tac = tac->prev){
        if(tac->type != TAC_SYMBOL && tac->type != TAC_EXPRESSION) tacPrintSingle(tac);
        //tacPrintSingle(tac);
    }
}

TAC* generateCode(AST* node) {
    TAC* result = 0;
    //TAC* code[MAX_SONS]={0,0,0,0} ;
    std::vector<TAC*> code;
    code.reserve(MAX_SONS); 
    if(!node) return nullptr;
    if(node->type != AST_VECTOR_DEC && node->type != AST_PRINT && node->type != AST_FUNCTION_CALL && node->type != AST_COMMAND_VEC_EQ && node->type != AST_COMMAND_RETURN){
        for(auto s: node->son){
        //code[i++] = generateCode(s);
            code.push_back(generateCode(s));
        }
    }
    switch (node->type) {
        case AST_SYMBOL: 
            result = new TAC(TAC_SYMBOL, node->symbol, 0, 0); 
            break;
        case AST_EXPRESSION: 
            result = new TAC(TAC_EXPRESSION, node->symbol, 0, 0);
            break;
        case AST_VAR_DEC:                                                                       // TAC(TAC_ASS, y, 7, ) 
            result = tacJoin(code[1], new TAC(TAC_ASS, node->symbol, code[1]?code[1]->res:0,0)); // nome da var, oq vai inserir nela
            break;
        case AST_VECTOR_DEC: {                                                               // TAC(TAC_VECTOR_DEC, v, 4, )
            result = makeVectorDec(node);                                                       // nome, tamanho
            break;                                                                              // TAC(TAC_VECTOR_INIT, v, 0, 1) 
        }                                                                                   // nome, indice, valor

        // commands
        case AST_COMMAND_EQ:{             
                                                           // igual dec_var
            TAC* assign_tac = new TAC(TAC_ASS, node->symbol, code[0] ? code[0]->res : nullptr, nullptr);
            result = tacJoin(code[0], assign_tac); 
            //result = tacJoin(code[0], new TAC(TAC_ASS, node->symbol, code[0]?code[0]->res:0,0));
            break;}
        case AST_EXPRESSION_VEC: {                                                              // TAC(TAC_EXP_VEC, temp1, v, 3)
            TAC* indexCode = code[0];
            TAC* vecRead = new TAC(TAC_EXP_VEC, makeTemp(), node->symbol, indexCode?indexCode->res:0);
            return tacJoin(indexCode, vecRead);
        }
        case AST_COMMAND_VEC_EQ:
            result = makeVecAssign(node);
            break;
        case AST_ADD:                                                                           // TAC(TAC_ADD, temp0, 2, 4)
            result = binOperations(TAC_ADD, code[0], code[1]);                                  // onde vai armazenar, val1, val2
            break;
        case AST_SUB:
            result = binOperations(TAC_SUB, code[0], code[1]);
            break;
        case AST_MULT:
            result = binOperations(TAC_MULT, code[0], code[1]);
            break;
        case AST_DIV:
            result = binOperations(TAC_DIV, code[0], code[1]);
            break;
        case AST_LESS:
            result = binOperations(TAC_LESS, code[0], code[1]);
            break;
        case AST_GREATER:
            result = binOperations(TAC_GREATER, code[0], code[1]);
            break;
        case AST_AND:
            result = binOperations(TAC_AND, code[0], code[1]);
            break;
        case AST_OR:
            result = binOperations(TAC_OR, code[0], code[1]);
            break;
        case AST_DIF:
            result = binOperations(TAC_DIF, code[0], code[1]);
            break;
        case AST_EQ:
            result = binOperations(TAC_EQ, code[0], code[1]);
            break;
        case AST_GE:
            result = binOperations(TAC_GE, code[0], code[1]);
            break;
        case AST_LE:
            result = binOperations(TAC_LE, code[0], code[1]);
            break;
        case AST_NOT:
            result = tacJoin(tacJoin(code[0], code[1]), new TAC(TAC_NOT, makeTemp(), code[0]?code[0]->res:0, 0));
            break;
        case AST_FUNCTION_DEC:
            result = makeFunc(node, code[1], code[2]);
            break; 
        case AST_PARAM: {
            result = new TAC(TAC_SYMBOL, node->symbol, nullptr, nullptr);
            break;
        }
        case AST_FUNCTION_CALL:
            result = makeFuncCall(node);
            break;
        case AST_COMMAND_IF:
            result = makeIfThen(code[0], code[1]);
            break;
        case AST_COMMAND_IF_ELSE:
            result = makeIfThenElse(code[0], code[1], code[2]);
            break;
        case AST_COMMAND_WHILE_DO:
            result = makeWhileDo(code[0], code[1]);
            break;
        case AST_COMMAND_DO_WHILE:
            result = makeDoWhile(code[0], code[1]);
            break;
        case AST_PRINT: 
            result = makePrint(node);
            break;
        case AST_COMMAND_READ:
            result = new TAC(TAC_READ, node->symbol, 0,0);
            break;
        case AST_COMMAND_RETURN:{
            TAC* exprCode = generateCode(node->son[0]);
            TAC* tacReturn = new TAC(TAC_RETURN, exprCode ? exprCode->res : node->son[0]->symbol,0,0);
            result = tacJoin(exprCode, tacReturn);
            break;
        }

        default: {
            if(code.empty()){
                result = nullptr;
            }else{
                result = code[0];
                for(size_t i = 1; i < code.size(); i++){
                    result = tacJoin(result, code[i]);
                }
            }
            break;
        }
    }

    return result;
}


//----------------------------------------------------------------------------------------------------------------------------------
// PRIVATE FUNCTIONS
//----------------------------------------------------------------------------------------------------------------------------------

TAC* makeVectorDec(AST* node){
    SYMBOL* vectorSymbol = node->symbol;
    SYMBOL* sizeSymbol = node->son[1]->symbol;
    TAC* result = new TAC(TAC_VECTOR_DEC, vectorSymbol, sizeSymbol, 0);
    // init do vetor
    AST* initNode = node->son[2];
    if (initNode) {
        for (size_t i = 0; i < initNode->son.size(); i++) {
            TAC* valueCode = generateCode(initNode->son[i]);
            SYMBOL* indexSymbol = makeConst(i);
            TAC* vecWrite = new TAC(TAC_VECTOR_INIT,vectorSymbol,indexSymbol,valueCode ? valueCode->res : initNode->son[i]->symbol);
            result = tacJoin(result, tacJoin(valueCode, vecWrite));
        }
    }
    return result;
}
TAC* makeVecAssign(AST* node) {
    TAC* indexCode = generateCode(node->son[0]); 
    TAC* valueCode = generateCode(node->son[1]);
    TAC* vecWrite = new TAC(TAC_VECTOR_INIT, node->symbol, indexCode ? indexCode->res : node->son[0]->symbol, valueCode ? valueCode->res : node->son[1]->symbol);
    return tacJoin(tacJoin(indexCode, valueCode), vecWrite);
}

TAC* binOperations(int tacType, TAC* code0, TAC* code1){
    return tacJoin(tacJoin(code0, code1), new TAC(tacType, makeTemp(), code0?code0->res:0, code1?code1->res:0));
}

TAC* makeFunc(AST* node, TAC* code1, TAC* code2) {
    TAC* beginFunc = new TAC(TAC_BEGIN_FUNC, node->symbol, 0, 0);
    TAC* endFunc = new TAC(TAC_END_FUNC, node->symbol, 0, 0);
    return tacJoin(tacJoin(tacJoin(beginFunc, code1), code2), endFunc);
}

TAC* makeFuncCall(AST* node) {
    TAC* argsCode = nullptr;
    AST* argsNode = node->son[0];
    if (argsNode) {
        for (size_t i = 0; i < argsNode->son.size(); i++) {
            SYMBOL* indexSymbol = makeConst(i);
            TAC* argCode = generateCode(argsNode->son[i]);
            TAC* tacArg = new TAC(TAC_ARG, argCode ? argCode->res : argsNode->son[i]->symbol, node->symbol, indexSymbol);
            argsCode = tacJoin(argsCode, tacJoin(argCode, tacArg));
        }
    }
    TAC* funcCall = new TAC(TAC_FUNC_CALL, makeTemp(), node->symbol, 0);
    return tacJoin(argsCode, funcCall);
}

TAC* makeIfThen(TAC* code0, TAC* code1){
    TAC *tacIf = 0;
    TAC *tacLabel = 0;
    SYMBOL *newLabel = makeLabel();

    tacIf = new TAC(TAC_IF0, newLabel, code0?code0->res:0, 0);
    tacLabel = new TAC(TAC_LABEL, newLabel, 0, 0);
    tacIf->prev = code0;
    tacLabel->prev = code1;
    return tacJoin(tacIf, tacLabel);
}

TAC* makeIfThenElse(TAC* code0, TAC* code1, TAC* code2){
    SYMBOL *labelElse = makeLabel();
    SYMBOL *labelEnd = makeLabel();

    TAC* tacIf = new TAC(TAC_IF0, labelElse, code0?code0->res:0, 0); // se eh falso vai pro else
    TAC* tacJump = new TAC(TAC_JUMP, labelEnd, 0, 0); // se é vdd, faz o if e pula pro fim (pós else)
    TAC* tacLabelElse = new TAC(TAC_LABEL, labelElse, 0, 0);
    TAC* tacLabelEnd = new TAC(TAC_LABEL, labelEnd, 0, 0);
    return tacJoin(tacJoin(tacJoin(tacJoin(tacJoin(code0, tacIf), code1), tacJump), tacLabelElse), tacJoin(code2, tacLabelEnd));
}

TAC* makeWhileDo(TAC* code0, TAC* code1) {
    SYMBOL* labelStart = makeLabel();
    SYMBOL* labelEnd = makeLabel();

    TAC* tacLabelStart = new TAC(TAC_LABEL, labelStart, 0, 0);
    TAC* tacLabelEnd = new TAC(TAC_LABEL, labelEnd, 0, 0);

    TAC* tacIf = new TAC(TAC_IF0, labelEnd, code0 ? code0->res :0, 0); // pula pro fim se é falso
    TAC* tacJump = new TAC(TAC_JUMP, labelStart,0, 0); //volta pro inicio se é true

    // Junta tudo na ordem correta
    return tacJoin(tacLabelStart,tacJoin(code0, tacJoin(tacIf,tacJoin(code1,tacJoin(tacJump, tacLabelEnd)))));
}

TAC* makeDoWhile(TAC* code0, TAC* code1) {
    SYMBOL* labelStart = makeLabel();
    TAC* tacLabelStart = new TAC(TAC_LABEL, labelStart, 0, 0);
    TAC* tacIf = new TAC(TAC_IF1,labelStart,code1 ? code1->res : 0, 0); // se é true volta pro começo
    return tacJoin(tacLabelStart,tacJoin(code0,tacJoin(code1,tacIf)));
}

TAC* makePrint(AST* node) {
    TAC* result = 0;
    for (auto s : node->son) {
        SYMBOL* childCode = s->symbol;
        TAC* printTac = new TAC(TAC_PRINT, childCode, 0, 0);
        result = tacJoin(result, printTac);
    }
    return result;
}

void freeTAC(TAC* tac) {
    TAC* current = tac;
    while (current) {
        TAC* prevTac = current->prev; 
        delete current;
        current = prevTac;
    }
}