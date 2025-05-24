#include "semantic.hpp"
#include "symbols.hpp"
#include <map>
int semanticErrors = 0; 
int vectorSize=0;
extern map<string, SYMBOL*> SymbolTable;
map<string, vector<int>> functionParamCount;
int currentFunctionDatatype = 0;

//-------------------------------------------------------------------------
// CHECK VARIABLES
//-------------------------------------------------------------------------
void setLiteralTypes(AST* node) {
    if (!node) return;

    if (node->type == AST_SYMBOL && node->symbol) {
        switch(node->symbol->type){
            case SYMBOL_LIT_INT: 
                node->symbol->dataType = DATATYPE_INT;
                break;
            case SYMBOL_LIT_REAL:
                node->symbol->dataType = DATATYPE_REAL;
                break;
            case SYMBOL_LIT_CHAR: 
                node->symbol->dataType = DATATYPE_CHAR;
                break;
            case SYMBOL_LIT_STRING:
                node->symbol->dataType = DATATYPE_STRING;
                break;
            case SYMBOL_LIT_BYTE:
                node->symbol->dataType = DATATYPE_BYTE;
                break;
            default: // undefined
                node->symbol->dataType = DATATYPE_UNDEFINED;
                break;
        }
    }

    for (AST* s : node->son) {
        setLiteralTypes(s);
    }
}

void checkDeclarations(AST* node){
    if(!node) return;

    switch (node->type){
        
        case AST_VAR_DEC:
            if (node->symbol->type != SYMBOL_TK_IDENTIFIER) {
                fprintf(stderr, "SEMANTIC ERROR: variable %s redeclared\n", node->symbol->text.c_str());
                semanticErrors++; 
            } else {
                // marca como variável já vista 
                node->symbol->type = SYMBOL_TK_IDENTIFIER_VAR;
                switch(node->son[0]->type){
                    case AST_TYPE_INT: 
                        node->symbol->dataType = DATATYPE_INT;
                        break;
                    case AST_TYPE_REAL: 
                        node->symbol->dataType = DATATYPE_REAL;
                        break;
                    case AST_TYPE_BYTE:
                        node->symbol->dataType = DATATYPE_BYTE;
                        break;
                    default: // undefined
                        node->symbol->dataType = DATATYPE_UNDEFINED;
                        break;
                }
            }
            // ve se o dataType da variavel é o mesmo do literal
            if(node->symbol->dataType == (node->son[1]->symbol->dataType)){

            }else if(node->symbol->dataType == DATATYPE_INT && node->son[1]->symbol->dataType == DATATYPE_CHAR){

            }else if(node->symbol->dataType == DATATYPE_BYTE && node->son[1]->symbol->dataType == DATATYPE_INT){

            }else{
                semanticErrors++;
                fprintf(stderr, "SEMANTIC ERROR: wrong lit type in var %s\n", node->symbol->text.c_str());
            }
            break;
            
        case AST_VECTOR_DEC:
            if (node->symbol->type != SYMBOL_TK_IDENTIFIER) {
                fprintf(stderr, "SEMANTIC ERROR: variable %s redeclared\n", node->symbol->text.c_str());
                semanticErrors++; 
            } else {
                // marca como variável já vista 
                node->symbol->type = SYMBOL_TK_IDENTIFIER_VECTOR;
                node->son[1]->symbol->type = SYMBOL_VECTOR_SIZE;
                node->son[1]->symbol->dataType = DATATYPE_INT;

                switch(node->son[0]->type){
                    case AST_TYPE_INT: 
                        node->symbol->dataType = DATATYPE_INT;
                        break;
                    case AST_TYPE_REAL: 
                        node->symbol->dataType = DATATYPE_REAL;
                        break;
                    default: // undefined
                        node->symbol->dataType = DATATYPE_UNDEFINED;
                        break;
                }
            }
            if (node->son.size() >= 3 && node->son[2]) { // tem AST_VECTOR_INIT
                AST* vectorInit = node->son[2];
                for (size_t i = 0; i < vectorInit->son.size(); ++i) {
                    vectorSize++;
                    AST* element = vectorInit->son[i];
                    if (element && element->symbol) {
                        if (!areCompatible(element->symbol->dataType, node->symbol->dataType)) {
                            fprintf(stderr, "SEMANTIC ERROR: element %zu is incompatible with vector's %s type \n", i, node->symbol->text.c_str());
                            semanticErrors++;
                        }
                    } else {
                        fprintf(stderr, "SEMANTIC ERROR: invalid element\n");
                        semanticErrors++;
                    }
                }
                // ve se foi inicializado com a qtd certa de variaveis
                int qtdInit = vectorInit->son.size();
                int declaredSize = stoi(node->son[1]->symbol->text);
                if(qtdInit != declaredSize){
                    fprintf(stderr, "SEMANTIC ERROR: missing variables in vector %s\n", node->symbol->text.c_str());
                    semanticErrors++;
                }


            }
            break;

        case AST_FUNCTION_DEC:{
            if (node->symbol->type != SYMBOL_TK_IDENTIFIER) {
                fprintf(stderr, "SEMANTIC ERROR: variable %s redeclared\n", node->symbol->text.c_str());
                semanticErrors++; 
            } else {
                // marca como variável já vista 
                node->symbol->type = SYMBOL_TK_IDENTIFIER_FUNCTION;
                switch(node->son[0]->type){
                    case AST_TYPE_INT: 
                        node->symbol->dataType = DATATYPE_INT;
                        break;
                    case AST_TYPE_REAL: 
                        node->symbol->dataType = DATATYPE_REAL;
                        break;
                    case AST_TYPE_BYTE:
                        node->symbol->dataType = DATATYPE_BYTE;
                        break;
                    default: // undefined
                        node->symbol->dataType = DATATYPE_UNDEFINED;
                        break;
                }
            }
            std::vector<int> paramTypes;
            AST* paramList = node->son[1]; // AST_PARAM_LIST

            if (paramList && paramList->type == AST_PARAM_LIST) {
                for (AST* param : paramList->son) {
                    if (param->type == AST_PARAM) {
                        if (param->symbol->type != SYMBOL_TK_IDENTIFIER) {
                            fprintf(stderr, "SEMANTIC ERROR: variable %s redeclared\n", param->symbol->text.c_str());
                            semanticErrors++;
                        }else{
                            param->symbol->type = SYMBOL_TK_IDENTIFIER_VAR;
                            AST* paramTypeNode = param->son[0]; // Primeiro filho é o tipo (AST_TYPE_INT ou AST_TYPE_REAL)
                            int paramType = DATATYPE_UNDEFINED;
                            
                            switch (paramTypeNode->type) {
                                case AST_TYPE_INT:
                                    paramType = DATATYPE_INT;
                                    break;
                                case AST_TYPE_REAL:
                                    paramType = DATATYPE_REAL;
                                    break;
                                default:
                                    paramType = DATATYPE_UNDEFINED;
                                    break;
                            }
                            param->symbol->dataType = paramType;
                            paramTypes.push_back(paramType);
                        }
                    }
                }
            }
            functionParamCount[node->symbol->text.c_str()] = paramTypes;
            //printf("Função %s tem %zu parâmetros.\n", node->symbol->text.c_str(), paramTypes.size());
            break;}
        /*    
        case AST_PARAM:
            if (node->symbol->type != SYMBOL_TK_IDENTIFIER) {
                fprintf(stderr, "redeclaração do %s\n", node->symbol->text.c_str());
                semanticErrors++; 
            } else {
                // marca como variável já vista 
                node->symbol->type = SYMBOL_TK_IDENTIFIER_VAR;
                switch(node->son[0]->type){
                    case AST_TYPE_INT: 
                        node->symbol->dataType = DATATYPE_INT;
                        break;
                    case AST_TYPE_REAL: 
                        node->symbol->dataType = DATATYPE_REAL;
                        break;
                    case AST_TYPE_BYTE:
                        node->symbol->dataType = DATATYPE_BYTE;
                        break;
                    default: // undefined
                        node->symbol->dataType = DATATYPE_UNDEFINED;
                        break;
                }
            }

            break;    */ 
        }

    for (AST* s : node->son) {
        checkDeclarations(s);
    }
}


void checkUndeclared(){
    for (auto s : SymbolTable) {
        if (s.second->dataType == DATATYPE_UNDEFINED) {
            fprintf(stderr, "SEMATIC ERROR: %s was not declared\n", s.second->text.c_str());
            semanticErrors++;
        }
    }
}

// --------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------

int checkNodeDataType(AST* node){
    if(!node) return DATATYPE_UNDEFINED;

    if (node->type == AST_SYMBOL) {
        if (node->symbol) {
            node->dataType = node->symbol->dataType;
        } else {
            node->dataType = DATATYPE_INVALID;
            semanticErrors++;
        }
        return node->dataType;
    }
    if(node->type == AST_EXPRESSION){
        if(node->symbol->type == SYMBOL_TK_IDENTIFIER_VAR){
            node->dataType = node->symbol->dataType;
        }else{
            node->dataType = DATATYPE_INVALID;
            semanticErrors++;
            fprintf(stderr, "SEMANTIC ERROR: %s is vector or function, not variable\n", node->symbol->text.c_str());
        }
        return node->dataType;
    }
    if(node->type == AST_EXPRESSION_VEC){
        if(node->symbol->type == SYMBOL_TK_IDENTIFIER_VECTOR){
            int vectorIndex = checkNodeDataType(node->son[0]);
            if(vectorIndex == DATATYPE_INT || vectorIndex == DATATYPE_CHAR){
                node->dataType = node->symbol->dataType;
            }else{
                node->dataType = DATATYPE_INVALID;
                semanticErrors++;
                fprintf(stderr, "SEMANTIC ERROR: vector position needs to be int\n");
            }
        }else{
            node->dataType = DATATYPE_INVALID;
            semanticErrors++;
            fprintf(stderr, "SEMANTIC ERROR: %s is variable or function, not vector\n", node->symbol->text.c_str());
        }
        return node->dataType;
    }
    if(node->type == AST_LIT){
        node->dataType = checkNodeDataType(node->son[0]);
        return node->dataType;
    }
    if(node->type == AST_COMMAND_EQ){
        int exprType = checkNodeDataType(node->son[0]);
        int varType = node->symbol->dataType;
        if(varType != exprType){
             fprintf(stderr, "SEMANTIC ERROR: incompatible variable and expression types\n");
             semanticErrors++;
             node->dataType = DATATYPE_INVALID;
         }else{
             node->dataType = varType;
         }
        return node->dataType;
    }
    if(node->type == AST_FUNCTION_CALL){
        if (functionParamCount.find(node->symbol->text) == functionParamCount.end()) {
            fprintf(stderr, "SEMANTIC ERROR: function %s was not declared\n", node->symbol->text.c_str());
            node->dataType = DATATYPE_INVALID;
            semanticErrors++;
            return node->dataType;
        }
        vector<int> expectedParamTypes = functionParamCount[node->symbol->text];
        vector<int> argumentTypes;
        AST* paramNode = node->son[0];
        for (AST* param2 : paramNode->son) {
            if (param2->type == AST_PARAM2) {
                AST* expr = param2->son[0]; 
                int argType = checkNodeDataType(expr);
                argumentTypes.push_back(argType);
            }
        }
        // ve se qtd ta ok
        if (argumentTypes.size() != expectedParamTypes.size()) {
            fprintf(stderr, "SEMANTIC ERROR: %s needs %zu arguments, but got %zu.\n",
                    node->symbol->text.c_str(), expectedParamTypes.size(), argumentTypes.size());
            node->dataType = DATATYPE_INVALID;
            semanticErrors++;
            return node->dataType;
        }

        // ve se os tipos sao compativeis
        for (size_t i = 0; i < argumentTypes.size(); ++i) {
            if (!areCompatible(argumentTypes[i],expectedParamTypes[i])) {
                fprintf(stderr, "SEMANTIC ERROR: Argument %zu in %s received type %d, but needs type %d\n",
                        i + 1, node->symbol->text.c_str(), argumentTypes[i], expectedParamTypes[i]);
                node->dataType = DATATYPE_INVALID;
                semanticErrors++;
                return node->dataType;
            }
        }

        node->dataType = node->symbol->dataType;
        return node->dataType;
    }
    if(node->type == AST_FUNCTION_DEC){
        currentFunctionDatatype = node->symbol->dataType;
        //fprintf(stderr, "aaaaaaaafuncao atual %d\n", currentFunctionDatatype);
    }
    if(node->type == AST_COMMAND_RETURN){
        node->dataType = checkNodeDataType(node->son[0]);
        //fprintf(stderr, "funcao atual %d\n", currentFunctionDatatype);
        //fprintf(stderr, "datatype %d\n", node->dataType);
        if(!areCompatible(node->dataType, currentFunctionDatatype)){
            node->dataType = DATATYPE_INVALID;
            fprintf(stderr, "SEMANTIC ERROR: return not compatible with function's type\n");
            semanticErrors++;
        }
        return node->dataType;
    }

    if(node->type == AST_PARENTHESIS){
        node->dataType = checkNodeDataType(node->son[0]);
        return node->dataType;
    }
    
    if(isAritmethic(node->type)){
        int leftType = checkNodeDataType(node->son[0]);
        int rightType = checkNodeDataType(node->son[1]);
        //fprintf(stderr, "esquerda e direita %d e %d.\n", node->son[0]->type, node->son[0]->dataType);
        if(areCompatible(leftType, rightType)){
            //fprintf(stderr, "add ta show");
            node->dataType = leftType;
        }else{
            fprintf(stderr, "SEMANTIC ERROR: expressions not compatible\n");
            semanticErrors++;
            node->dataType = DATATYPE_INVALID;
        }
        return node->dataType;
    }

    if(isLogical(node->type)){
        if(node->type == AST_NOT){
            if(node->son[0]->dataType != DATATYPE_BOOL){
                fprintf(stderr, "SEMANTIC ERROR: variable %s is not bool\n", node->son[0]->symbol->text.c_str());
                semanticErrors++;
            }
        }else if(node->type == AST_AND || node->type == AST_OR){
            if(node->son[0]->dataType != DATATYPE_BOOL || node->son[1]->dataType != DATATYPE_BOOL){
                fprintf(stderr, "SEMANTIC ERROR: variable %s is not bool", node->son[0]->symbol->text.c_str());
                semanticErrors++;
            }
        }
        node->dataType = DATATYPE_BOOL;
    }

    if(isRelational(node->type)){
        int leftType = checkNodeDataType(node->son[0]);
        int rightType = checkNodeDataType(node->son[1]);
        //fprintf(stderr, "esquerda e direita %d e %d.\n", node->son[0]->type, node->son[0]->dataType);
        if(areCompatible(leftType, rightType)){
            //fprintf(stderr, "dois lados do relacional compativeis\n");
            node->dataType = DATATYPE_BOOL;
        }else{
            fprintf(stderr, "SEMANTIC ERROR: incompatible types for relational comparison\n");
            semanticErrors++;
            node->dataType = DATATYPE_INVALID;
        }
        return node->dataType;
    }

    if(node->type == AST_COMMAND_IF || node->type == AST_COMMAND_IF_ELSE){
        int condType = checkNodeDataType(node->son[0]);
        if(condType != DATATYPE_BOOL){
            semanticErrors++;
            fprintf(stderr, "SEMANTIC ERROR: invalid condition\n");
        }
    }
    if(node->type == AST_COMMAND_DO_WHILE){
        int condType = checkNodeDataType(node->son[1]);
        if(condType != DATATYPE_BOOL){
            semanticErrors++;
            fprintf(stderr, "SEMANTIC ERROR: invalid condition\n");
        }
    }
    if(node->type == AST_COMMAND_WHILE_DO){
        int condType = checkNodeDataType(node->son[0]);
        if(condType != DATATYPE_BOOL){
            semanticErrors++;
            fprintf(stderr, "SEMANTIC ERROR: invalid condition\n");
        }
    }
    for (AST* s : node->son) {
        checkNodeDataType(s);
    }

    node->dataType = DATATYPE_UNDEFINED;
    return node->dataType;
}

bool areCompatible(int dataType1, int dataType2){
    // só char e inteiro sãocompativeis entre si 
    if (dataType1 == DATATYPE_INT && dataType2 == DATATYPE_INT) return true;
    else if (dataType1 == DATATYPE_INT && dataType2 == DATATYPE_CHAR) return true;
    else if (dataType1 == DATATYPE_CHAR && dataType2 == DATATYPE_INT) return true;
    else if (dataType1 == DATATYPE_CHAR && dataType2 == DATATYPE_CHAR) return true;
    else if (dataType1 == DATATYPE_REAL && dataType2 == DATATYPE_REAL) return true;
    else if (dataType1 == DATATYPE_INT && dataType2 == DATATYPE_INT) return true;
    else if (dataType1 == DATATYPE_BYTE && dataType2 == DATATYPE_BYTE) return true;
    else if (dataType1 == DATATYPE_BOOL && dataType2 == DATATYPE_BOOL) return true;
    else return false;
}

bool isAritmethic(int nodeType){
    if(nodeType == AST_ADD || nodeType == AST_SUB || nodeType == AST_MULT || nodeType == AST_DIV){
        return true;
    }else{
        return false;
    }
}

bool isRelational(int nodeType){
    if(nodeType == AST_LESS || nodeType == AST_GREATER || nodeType == AST_DIF || nodeType == AST_EQ || nodeType == AST_GE || nodeType == AST_LE){
        return true;
    }else{
        return false;
    }
}
bool isLogical(int nodeType){
    if(nodeType == AST_AND || nodeType == AST_OR || nodeType == AST_NOT){
        return true;
    }else{
        return false;
    }
}


/*

 operadores aritméticos: + − * / %
• operadores relacionais: < > <= >= == !=
• operadores lógicos: & | ~ 
*/
