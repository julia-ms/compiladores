// asm.cpp
// Julia Mombach da Silva

#include "tac.hpp"
#include "symbols.hpp"
#include <cstdio>
#include <algorithm>

map<string, string> printSymbolsASM(FILE* fout, TAC* first);
void printInit(FILE* fout);
string findVariableByValue(const map<string, string>& variableValues, const string& value);
bool isNumber(const std::string& s);

void generateASM(TAC* first){
    fprintf(stderr, "\nentrei no asm\n");
    TAC* tac;
    FILE* fout;
    fout = fopen("out.s", "w");

    // tabela de simbolos
    map<string, string>variableValues = printSymbolsASM(fout, first);
    printInit(fout);

    fprintf(fout, "\t.text\n");
    for(tac = first; tac; tac = tac->next){
        switch (tac->type){
        case TAC_BEGIN_FUNC:
            fprintf(fout,   "\n\t#TAC_BEGIN_FUNC\n"
                            "\t.globl %s\n" 
                            "%s:\n"
                            "\tpushq   %%rbp\n"
                            "\tmovq    %%rsp, %%rbp\n", tac->res->text.c_str(), tac->res->text.c_str());
            break;
        case TAC_END_FUNC:
            fprintf(fout,   "\n\t#TAC_END_FUNC\n"
                            "\tmovl    $0, %%eax\n"
                            "\tpopq    %%rbp\n"
                            "\tret\n");
            break;
        case TAC_PRINT:
            if (tac->res->text[0] == '"') {
                string var = findVariableByValue(variableValues, tac->res->text);
                fprintf(fout,   "\n\t#TAC_PRINT\n"
                                "\tleaq %s(%%rip), %%rsi\n"
                                "\tleaq print_string(%%rip), %%rdi\n"
                                "\tmovl $0, %%eax\n"
                                "\tcall printf\n", var.c_str());
            }else{
                fprintf(fout,   "\n\t#TAC_PRINT\n"
                                "\tmovl %s(%%rip), %%esi\n"
                                "\tleaq print_int(%%rip), %%rdi\n"
                                "\tmovl $0, %%eax\n"
                                "\tcall printf\n",
                                tac->res->text.c_str());
            }
            break;
        case TAC_ASS:{
            string op1 = tac->op1->text;
            string res = tac->res->text;
            if (isNumber(op1)) {
                fprintf(fout,   "\t#TAC_ASS (literal)\n"
                                "\tmovl $%s, %%eax\n"
                                "\tmovl %%eax, %s(%%rip)\n",
                                op1.c_str(),
                                res.c_str());
            } else {
                fprintf(fout,   "\t#TAC_ASS (variable)\n"
                                "\tmovl %s(%%rip), %%eax\n"
                                "\tmovl %%eax, %s(%%rip)\n",
                                op1.c_str(),
                                res.c_str());
            }
            break;
        }
        case TAC_ADD: {
            std::string op1 = tac->op1->text;
            std::string op2 = tac->op2->text;
            std::string res = tac->res->text;
            if (isNumber(op1)) {
                fprintf(fout, "\tmovl $%s, %%eax\n", op1.c_str());
            } else {
                fprintf(fout, "\tmovl %s(%%rip), %%eax\n", op1.c_str());
            }
            if (isNumber(op2)) {
                fprintf(fout, "\taddl $%s, %%eax\n", op2.c_str());
            } else {
                fprintf(fout, "\taddl %s(%%rip), %%eax\n", op2.c_str());
            }
            fprintf(fout, "\tmovl %%eax, %s(%%rip)\n", res.c_str());
            break;
        }

        case TAC_SUB: {
            std::string op1 = tac->op1->text;
            std::string op2 = tac->op2->text;
            std::string res = tac->res->text;
            if (isNumber(op1)) {
                fprintf(fout, "\tmovl $%s, %%eax\n", op1.c_str());
            } else {
                fprintf(fout, "\tmovl %s(%%rip), %%eax\n", op1.c_str());
            }
            if (isNumber(op2)) {
                fprintf(fout, "\tsubl $%s, %%eax\n", op2.c_str());
            } else {
                fprintf(fout, "\tsubl %s(%%rip), %%eax\n", op2.c_str());
            }
            fprintf(fout, "\tmovl %%eax, %s(%%rip)\n", res.c_str());
            break;
        }

        case TAC_MULT: {
            std::string op1 = tac->op1->text;
            std::string op2 = tac->op2->text;
            std::string res = tac->res->text;
            if (isNumber(op1)) {
                fprintf(fout, "\tmovl $%s, %%eax\n", op1.c_str());
            } else {
                fprintf(fout, "\tmovl %s(%%rip), %%eax\n", op1.c_str());
            }
            if (isNumber(op2)) {
                fprintf(fout, "\timull $%s, %%eax\n", op2.c_str());
            } else {
                fprintf(fout, "\timull %s(%%rip), %%eax\n", op2.c_str());
            }
            fprintf(fout, "\tmovl %%eax, %s(%%rip)\n", res.c_str());
            break;
        }

        case TAC_DIV: {
            std::string op1 = tac->op1->text;
            std::string op2 = tac->op2->text;
            std::string res = tac->res->text;
            if (isNumber(op1)) {
                fprintf(fout, "\tmovl $%s, %%eax\n", op1.c_str());
            } else {
                fprintf(fout, "\tmovl %s(%%rip), %%eax\n", op1.c_str());
            }
            fprintf(fout, "\tcltd\n"); // estende sinal para %edx
            if (isNumber(op2)) {
                fprintf(fout, "\tidivl $%s\n", op2.c_str());
            } else {
                fprintf(fout, "\tidivl %s(%%rip)\n", op2.c_str());
            }
            fprintf(fout, "\tmovl %%eax, %s(%%rip)\n", res.c_str());
            break;
        }

        case TAC_LESS:
        case TAC_GREATER:
        case TAC_EQ:
        case TAC_DIF:
        case TAC_GE:
        case TAC_LE:
        {
            string op1 = tac->op1->text;
            string op2 = tac->op2->text;
            string res = tac->res->text;

            string setInstr;
            switch (tac->type) {
                case TAC_LESS:    setInstr = "setl"; break;
                case TAC_GREATER: setInstr = "setg"; break;
                case TAC_EQ:      setInstr = "sete"; break;
                case TAC_DIF:     setInstr = "setne"; break;
                case TAC_GE:      setInstr = "setge"; break;
                case TAC_LE:      setInstr = "setle"; break;
            }
            std::string op1str = isNumber(op1) ? "$" + op1 : op1 + "(%rip)";
            std::string op2str = isNumber(op2) ? "$" + op2 : op2 + "(%rip)";
            fprintf(fout,   "\t# %s = (%s op %s)\n"
                            "\tmovl %s, %%eax\n"
                            "\tcmpl %s, %%eax\n"
                            "\t%s %%al\n"
                            "\tmovzbl %%al, %%eax\n"
                            "\tmovl %%eax, %s(%%rip)\n",
                            res.c_str(),
                            op1.c_str(),
                            op2.c_str(),
                            op1str.c_str(),
                            op2str.c_str(),
                            setInstr.c_str(),
                            res.c_str());
            break;
        }

        case TAC_IF0:
            fprintf(fout,   "\t#TAC_IF0\n"
                            "\tmovl %s(%%rip), %%eax\n"
                            "\tcmpl $0, %%eax\n"
                            "\tje %s\n",
                            tac->op1->text.c_str(),
                            tac->res->text.c_str());
            break;
        case TAC_IF1:
            fprintf(fout,   "\t#TAC_IF0\n"
                            "\tmovl %s(%%rip), %%eax\n"
                            "\tcmpl $0, %%eax\n"
                            "\tjne %s\n",
                            tac->op1->text.c_str(),
                            tac->res->text.c_str());
            break;
        case TAC_JUMP:
            fprintf(fout,   "\t#TAC_JUMP\n"
                            "\tjmp %s\n",
                            tac->res->text.c_str());
            break;
        case TAC_LABEL:
                fprintf(fout, "%s:\n", tac->res->text.c_str());
        default:
            break;
        }
    }

    fclose(fout);
}

map<string, string> printSymbolsASM(FILE* fout, TAC* first) {
    map<string, string> variableValues = createSymbolMap();

    for (TAC* tac = first; tac; tac = tac->next) {
        if (tac->type == TAC_ASS_DEC) {
            if (tac->res && tac->op1) {
                variableValues[tac->res->text] = tac->op1->text;
            }
        }
    }

    fprintf(fout, "#VARIABLES\n");
    fprintf(fout, "\t.data\n");

    for (auto it : variableValues) {
        const std::string& val = it.second;

        if (!val.empty() && val[0] == '"') {
            // É string
            fprintf(fout, "%s:\t.string %s\n", 
                it.first.c_str(),
                val.c_str());
        } else {
            // É número
            fprintf(fout, "%s:\t.long %s\n", 
                it.first.c_str(),
                val.c_str());
        }
    }
    fprintf(fout, "\n");

    return variableValues;
}


void printInit(FILE* fout){
    fprintf(fout, "\n");
    fprintf(fout, "print_int:\n");
    fprintf(fout, "\t.string \"%%d\"\n");
    fprintf(fout, "print_string:\n");
    fprintf(fout, "\t.string \"%%s\"\n");
    fprintf(fout, "\n");
}

string findVariableByValue(const map<string, string>& variableValues, const string& value) {
    for (const auto& pair : variableValues) {
        if (pair.second == value) {
            return pair.first; // Retorna o nome da variável
        }
    }
    return ""; // Se não encontrar
}

bool isNumber(const std::string& s) {
    return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}