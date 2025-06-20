// Júlia Mombach da Silva
// ast.cpp

#include "ast.hpp"
#include "symbols.hpp"
#include <stdio.h>
#include <cstring>

using namespace std;

string ASTtypeNAMES[] = {
    "AST_ROOT",
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
    "AST_COMMAND_VEC_EQ",
    "AST_COMMAND_READ",
    "AST_COMMAND_RETURN",
    "AST_COMMAND_PRINT",
    "AST_EXPRESSION",
    "AST_LIT",
    "AST_EXPRESSION_VEC",
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
    "AST_PARENTHESIS",
    "AST_FUNCTION_CALL",
    "AST_FUNCTION_ARGS",
    "AST_PARAM2",
    "AST_PRINT",
    "AST_COMMAND_IF",
    "AST_COMMAND_IF_ELSE",
    "AST_COMMAND_WHILE_DO",
    "AST_COMMAND_DO_WHILE",
    "AST_COMMAND_PV",
};



void astPrint(AST* node, int level) {
    if (!node) return;

    for (int i = 0; i < level; i++) fprintf(stderr, "    ");
    
    fprintf(stderr, "AST: %s, %s, datatype: %d\n",
        ASTtypeNAMES[node->type].c_str(),
        node->symbol ? node->symbol->text.c_str() : "", node->dataType);

    for (auto s : node->son) {
        astPrint(s, level + 1);
    }
}

void astToFile(AST* node, FILE* file){

    if (!node) return;
    switch (node->type) {
        case AST_ROOT:
            for (AST* child : node->son) {
                astToFile(child, file);
            }
            break;
        case AST_SYMBOL:
        switch (node->symbol->type) {
            case 1:{
                const char* original = node->symbol->text.c_str();
                char* temp = (char*)malloc(strlen(original) + 1);
                strcpy(temp, original);
                char* reversed = reverseeINT(temp);
                fprintf(file, "%s", reversed);
                free(temp);
                break;
            }
            case 2:{
                const char* original = node->symbol->text.c_str();
                char* temp = (char*)malloc(strlen(original) + 1);
                strcpy(temp, original);
                char* reversed = reverseeREAL(temp);
                fprintf(file, "%s", reversed);
                free(temp);
                break;
            }
            default:
                fprintf(file, "%s", node->symbol->text.c_str());
                break;
        }
            break;
        case AST_VAR_DEC:
            astToFile(node->son[0], file);
            fprintf(file, "%s = ", node->symbol->text.c_str());
            astToFile(node->son[1], file);
            fprintf(file, ";\n");
            break;
        case AST_TYPE_INT:
            fprintf(file, "int ");
            break;
        case AST_TYPE_BYTE:
            fprintf(file, "byte ");
            break;
        case AST_TYPE_REAL:
            fprintf(file, "real ");
            break;

        //vector
        case AST_VECTOR_DEC:
            astToFile(node->son[0], file); // type
            fprintf(file, "%s", node->symbol->text.c_str()); // name
            astToFile(node->son[1], file); // vector size
            astToFile(node->son[2], file); // vector init
            fprintf(file, ";\n");
            break;
        case AST_VECTOR_SIZE:{
            fprintf(file, "[");
            const char* original = node->symbol->text.c_str();
            char* temp = (char*)malloc(strlen(original) + 1);
            strcpy(temp, original);
            char* reversed = reverseeINT(temp);
            fprintf(file, "%s", reversed);
            free(temp);
            fprintf(file, "]");
            break;}
        case AST_VECTOR_INIT:
            fprintf(file, " = ");
            astToFile(node->son[0], file); // first param
            for (size_t i = 1; i < node->son.size(); ++i) { // params
                fprintf(file, ", ");
                astToFile(node->son[i], file);
            }
            break;

        // function
        case AST_FUNCTION_DEC:
            astToFile(node->son[0], file); // type
            fprintf(file, "%s", node->symbol->text.c_str()); // name
            fprintf(file, "(");
            astToFile(node->son[1], file); //param list
            fprintf(file, ")"); 
            astToFile(node->son[2], file); // command block
            break;
        case AST_PARAM_LIST:
            astToFile(node->son[0], file); // first param
            for (size_t i = 1; i < node->son.size(); ++i) { // params
                fprintf(file, ", ");
                astToFile(node->son[i], file);
            }
            break;
        case AST_PARAM:
            astToFile(node->son[0], file); // type
            fprintf(file, " %s", node->symbol->text.c_str()); // name
            break;
        case AST_COMMAND_BLOCK:
            fprintf(file, "{\n");
            astToFile(node->son[0], file); // command list
            fprintf(file, "}\n");
            break;
        case AST_COMMAND_LIST:
            for (AST* child : node->son) { // commands
                astToFile(child, file);
            }
            break;

        // commands
        case AST_COMMAND:
            astToFile(node->son[0], file); // command block
            break;
        case AST_COMMAND_EQ:
            fprintf(file, "%s = ", node->symbol->text.c_str()); // var name =
            astToFile(node->son[0], file); // command block
            fprintf(file, ";\n");
            break;
        case AST_COMMAND_VEC_EQ:
            fprintf(file, "%s[", node->symbol->text.c_str()); //vec name
            astToFile(node->son[0], file); // vec position
            fprintf(file, "] = ");
            astToFile(node->son[1], file); // expression
            fprintf(file, ";\n");
            break;
        case AST_COMMAND_READ:
            fprintf(file, "read ");
            fprintf(file, "%s", node->symbol->text.c_str()); //var
            fprintf(file, ";\n");
            break;
        case AST_COMMAND_RETURN:
            fprintf(file, "return ");
            astToFile(node->son[0], file); 
            fprintf(file, ";\n");
            break;
        case AST_COMMAND_PRINT:
            fprintf(file, "print");
            astToFile(node->son[0], file); 
            fprintf(file, ";\n");
            break;
        case AST_PRINT:
            for (AST* child : node->son) { // commands
                fprintf(file, " ");
                astToFile(child, file);
            }
            break;
        case AST_COMMAND_PV:
            fprintf(file, ";");
            break;

        // expressions
        case AST_EXPRESSION: 
            fprintf(file, "%s", node->symbol->text.c_str()); //var
            break;
        case AST_LIT:
            astToFile(node->son[0], file);
            break;
        case AST_EXPRESSION_VEC:
            fprintf(file, "%s[", node->symbol->text.c_str()); // name
            astToFile(node->son[0], file);
            fprintf(file, "]");
            break;
        case AST_ADD:
            astToFile(node->son[0], file);
            fprintf(file, " + ");
            astToFile(node->son[1], file);
            break;
        case AST_SUB:
            astToFile(node->son[0], file);
            fprintf(file, " - ");
            astToFile(node->son[1], file);
            break;
        case AST_MULT:
            astToFile(node->son[0], file);
            fprintf(file, " * ");
            astToFile(node->son[1], file);
            break;
        case AST_DIV:
            astToFile(node->son[0], file);
            fprintf(file, " / ");
            astToFile(node->son[1], file);
            break;
        case AST_LESS:
            astToFile(node->son[0], file);
            fprintf(file, " < ");
            astToFile(node->son[1], file);
            break;
        case AST_GREATER:
            astToFile(node->son[0], file);
            fprintf(file, " > ");
            astToFile(node->son[1], file);
            break;
        case AST_AND:
            astToFile(node->son[0], file);
            fprintf(file, " & ");
            astToFile(node->son[1], file);
            break;
        case AST_OR:
            astToFile(node->son[0], file);
            fprintf(file, " | ");
            astToFile(node->son[1], file);
            break;
        case AST_NOT:
            astToFile(node->son[0], file);
            fprintf(file, " ~ ");
            astToFile(node->son[1], file);
            break;
        case AST_DIF:
            astToFile(node->son[0], file);
            fprintf(file, " != ");
            astToFile(node->son[1], file);
            break;
        case AST_EQ:
            astToFile(node->son[0], file);
            fprintf(file, " == ");
            astToFile(node->son[1], file);
            break;
        case AST_GE:
            astToFile(node->son[0], file);
            fprintf(file, " >= ");
            astToFile(node->son[1], file);
            break;
        case AST_LE:
            astToFile(node->son[0], file);
            fprintf(file, " <= ");
            astToFile(node->son[1], file);
            break;
        case AST_PARENTHESIS:
            fprintf(file, "(");
            astToFile(node->son[0], file);
            fprintf(file, ")");
            break;
        case AST_FUNCTION_CALL:
            fprintf(file, "%s(", node->symbol->text.c_str()); // name
            astToFile(node->son[0], file);
            fprintf(file, ")");
            break;
        case AST_FUNCTION_ARGS:
            astToFile(node->son[0], file); // first param
            for (size_t i = 1; i < node->son.size(); ++i) { // params
                fprintf(file, ", ");
                astToFile(node->son[i], file);
            }
            break;
        case AST_PARAM2:
            astToFile(node->son[0], file);
            break;

        // conditionals
        case AST_COMMAND_IF:
            fprintf(file, "if (");
            astToFile(node->son[0], file); //condition
            fprintf(file, ")");
            astToFile(node->son[1], file); // command
            break;
        case AST_COMMAND_IF_ELSE:
            fprintf(file, "if (");
            astToFile(node->son[0], file); //condition
            fprintf(file, ")");
            astToFile(node->son[1], file); // command if
            fprintf(file, " else ");
            astToFile(node->son[2], file); // command else
            break;
        case AST_COMMAND_WHILE_DO:
            fprintf(file, "while ");
            astToFile(node->son[0], file); //condition
            fprintf(file, " do ");
            astToFile(node->son[1], file); // command
            break;
        case AST_COMMAND_DO_WHILE:
            fprintf(file, "do ");
            astToFile(node->son[0], file); //command
            fprintf(file, " while ");
            astToFile(node->son[1], file); // command if
            fprintf(file, ";");
            break;
        default:
            fprintf(file, "UNKNOWN(%d)", node->type);
            break;
    }

}


char* reverseeINT(char* num) {
    int len = strlen(num);
    for (int i = 0; i < len / 2; i++) {
        char temp = num[i];
        num[i] = num[len - i - 1];
        num[len - i - 1] = temp;
    }
    return num;
}

char* reverseeREAL(char* num){

    char* num1 = strtok(num, "/");
    char* num2 = strtok(NULL, "/");
    
    reverseeINT(num1);
    reverseeINT(num2);

    size_t revSize = strlen(num1) + strlen(num2) + 2;
    char* rev = (char*)malloc(revSize);
    snprintf(rev, revSize, "%s/%s", num1, num2);

    return rev;

}

