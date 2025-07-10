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
map<string, vector<string>> extractFunctionParams(TAC* first);
extern void tacPrintSingle(TAC* tac);
void tacPrintFoward(TAC* tac);

void functionCallOptimizer(TAC* first);
void constantFolding(TAC* first);
void tempOptimizer(TAC* first);

void generateASM(TAC* first){
    functionCallOptimizer(first);
    constantFolding(first);
    tempOptimizer(first);
    tacPrintFoward(first);
    fprintf(stderr, "\nentrei no asm\n");
    TAC* tac;
    FILE* fout;
    fout = fopen("out.s", "w");

    // tabela de simbolos
    map<string, string>variableValues = printSymbolsASM(fout, first);
    auto paramsMap = extractFunctionParams(first);
    printInit(fout);

    for (auto it = paramsMap.begin(); it != paramsMap.end(); ++it) {
        fprintf(stderr, "Function: %s\n", it->first.c_str());
        for (const auto& param : it->second) {
            fprintf(stderr, "   param: %s\n", param.c_str());
        }
    }

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
        case TAC_READ:
            fprintf(fout,
                "\t# TAC_READ %s\n"
                "\tleaq %s(%%rip), %%rsi\n"
                "\tleaq scan_fmt(%%rip), %%rdi\n"
                "\tmovl $0, %%eax\n"
                "\tcall scanf\n",
                tac->res->text.c_str(),
                tac->res->text.c_str());
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
        case TAC_VECTOR_ASS: {
            std::string vecName = tac->res->text;
            std::string indexStr = tac->op1->text;
            std::string valueStr = tac->op2->text;

            if (isNumber(indexStr)) {
                int offset = stoi(indexStr) * 4;

                if (isNumber(valueStr)) {
                    fprintf(fout,
                        "\t# TAC_VECTOR_ASS (literal index, literal value)\n"
                        "\tmovl $%s, %s+%d(%%rip)\n",
                        valueStr.c_str(),
                        vecName.c_str(),
                        offset
                    );
                } else {
                    fprintf(fout,
                        "\t# TAC_VECTOR_ASS (literal index, variable value)\n"
                        "\tmovl %s(%%rip), %%eax\n"
                        "\tmovl %%eax, %s+%d(%%rip)\n",
                        valueStr.c_str(),
                        vecName.c_str(),
                        offset
                    );
                }
            } else {
                // index é variável
                fprintf(fout,
                    "\t# TAC_VECTOR_ASS (variable index)\n"
                    "\tmovl %s(%%rip), %%ecx\n"           // ecx = índice
                    "\tleaq %s(,%%rcx,4), %%rax\n",      // rax = endereço v[index*4]
                    indexStr.c_str(),
                    vecName.c_str()
                );

                if (isNumber(valueStr)) {
                    fprintf(fout,
                        "\tmovl $%s, (%%rax)\n",
                        valueStr.c_str()
                    );
                } else {
                    fprintf(fout,
                        "\tmovl %s(%%rip), %%edx\n"
                        "\tmovl %%edx, (%%rax)\n",
                        valueStr.c_str()
                    );
                }
            }
            break;
        }
        case TAC_EXP_VEC: {
    std::string result = tac->res->text;
    std::string vecName = tac->op1->text;
    std::string indexStr = tac->op2->text;

    fprintf(fout, "\n\t# TAC_EXP_VEC\n");

    if (isNumber(indexStr)) {
        int offset = stoi(indexStr) * 4;
        fprintf(fout,
            "\tmovl %s+%d(%%rip), %%eax\n"
            "\tmovl %%eax, %s(%%rip)\n",
            vecName.c_str(),
            offset,
            result.c_str()
        );
    } else {
        fprintf(fout,
            "\tleaq %s(%%rip), %%rdx\n"
            "\tmovl %s(%%rip), %%eax\n"
            "\timull $4, %%eax\n"
            "\tmovslq %%eax, %%rax\n"
            "\tmovl (%%rdx,%%rax,1), %%eax\n"
            "\tmovl %%eax, %s(%%rip)\n",
            vecName.c_str(),
            indexStr.c_str(),
            result.c_str()
        );
    }
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
                break;
        
        case TAC_RETURN: {
            std::string op1 = tac->res->text;

            if (isNumber(op1)) {
                fprintf(fout,
                    "\t# TAC_RETURN\n"
                    "\tmovl $%s, %%eax\n",
                    op1.c_str());
            } else {
                fprintf(fout,
                    "\t# TAC_RETURN\n"
                    "\tmovl %s(%%rip), %%eax\n",
                    op1.c_str());
            }

            fprintf(fout,
                "\tpopq %%rbp\n"
                "\tret\n"
            );

            break;
        }
        case TAC_FUNC_CALL:
            fprintf(fout,
                "\tcall %s\n",
                tac->op1->text.c_str()
            );
            if (tac->res) {
                fprintf(fout,
                    "\tmovl %%eax, %s(%%rip)\n",
                    tac->res->text.c_str());
            }
            break;
        // TAC(TAC_ARG, 3, dois, 0)
        // TAC(TAC_ARG, a, dois, 1)
        case TAC_ARG:{
            string val = tac->res->text; //valor que vai inserir
            string func = tac->op1->text; // nome da função 
            int pos = stoi(tac->op2->text);
            string paramName = paramsMap[func][pos];
            if (isNumber(val)) {
                fprintf(fout,   "\t#TAC_ARG (literal)\n"
                                "\tmovl $%s, %%eax\n"
                                "\tmovl %%eax, %s(%%rip)\n",
                                val.c_str(),
                                paramName.c_str()); 
            } else {
                fprintf(fout,   "\t#TAC_ARG (variable)\n"
                                "\tmovl %s(%%rip), %%eax\n"
                                "\tmovl %%eax, %s(%%rip)\n",
                                val.c_str(),
                                paramName.c_str());
            }
            break;
        }

        default:
            break;
        }
    }

    fclose(fout);
}

map<string, string> printSymbolsASM(FILE* fout, TAC* first) {
    map<string, string> variableValues = createSymbolMap();
    map<string, vector<string>> vectorValues;
    map<string, int> vectorSizes;

    for (TAC* tac = first; tac; tac = tac->next) {
        if (tac->type == TAC_ASS_DEC) {
            if (tac->res && tac->op1) {
                variableValues[tac->res->text] = tac->op1->text;
            }
        } else if (tac->type == TAC_VECTOR_DEC) {
            if (tac->res && tac->op1) {
                // tac->res → nome do vetor
                // tac->op1 → tamanho
                int size = stoi(tac->op1->text);
                vectorSizes[tac->res->text] = size;
                variableValues[tac->res->text] = "VECTOR";
            }
        } else if (tac->type == TAC_VECTOR_INIT) {
            string vecName = tac->res->text;
            int index = stoi(tac->op1->text);
            string value = tac->op2->text;
            vectorValues[vecName].resize(max(int(vectorValues[vecName].size()), index + 1));
            vectorValues[vecName][index] = value;
        }
    }

    fprintf(fout, "#VARIABLES\n");
    fprintf(fout, "\t.data\n");

    for (auto it : variableValues) {
        const std::string& val = it.second;
        const std::string& name = it.first;

        if (val == "VECTOR") {
            fprintf(fout, "%s:\n", name.c_str());
            auto itVec = vectorValues.find(name);
            if (itVec != vectorValues.end() && !itVec->second.empty()) {
                for (auto value : itVec->second) {
                    fprintf(fout, "\t.long %s\n", value.c_str());
                }
            } else {
                int size = vectorSizes[name];
                for (int i = 0; i < size; i++) {
                    fprintf(fout, "\t.long 0\n");
                }
            }
        }else if (!val.empty() && val[0] == '"') {
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
    fprintf(fout, "print_int:\n");
    fprintf(fout, "\t.string \"%%d\"\n");
    fprintf(fout, "print_string:\n");
    fprintf(fout, "\t.string \"%%s\"\n");
    fprintf(fout, "scan_fmt:\n");
    fprintf(fout, "\t.string \"%%d\"\n");
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

map<string, vector<string>> extractFunctionParams(TAC* first) {
    map<string, vector<string>> funcParams;

    for (TAC* tac = first; tac; tac = tac->next) {
        if (tac->type == TAC_BEGIN_FUNC) {
            string funcName = tac->res->text;
            vector<string> params;

            TAC* t = tac->next;
            while (t && t->type == TAC_SYMBOL) {
                if(!isNumber(t->res->text)) params.push_back(t->res->text);
                t = t->next;
            }

            funcParams[funcName] = params;
        }
    }

    return funcParams;
}




// ETAPA 7

void functionCallOptimizer(TAC* first) {
    for (TAC* tac = first; tac != nullptr; tac = tac->next) {
        // procura padrão:
        // TAC_FUNC_CALL temp, func
        if (tac->type == TAC_FUNC_CALL) {
            TAC* nextTac = tac->next;
            if (nextTac && nextTac->type == TAC_ASS) {
                SYMBOL* temp = tac->res;
                SYMBOL* assignedVar = nextTac->res;
                SYMBOL* assignedValue = nextTac->op1;

                if (assignedValue == temp) {
                    tac->res = assignedVar;
                    tac->next = nextTac->next;
                    if (nextTac->next) {
                        nextTac->next->prev = tac;
                    }
                    delete nextTac;
                }
            }
        }
    }
}

void constantFolding(TAC* first) {
    for (TAC* tac = first; tac; tac = tac->next) {
        if ((tac->type == TAC_ADD || tac->type == TAC_SUB ||
             tac->type == TAC_MULT || tac->type == TAC_DIV) &&
            tac->op1 && tac->op2 &&
            isNumber(tac->op1->text) && isNumber(tac->op2->text)) {

            int left = stoi(tac->op1->text);
            int right = stoi(tac->op2->text);
            int result = 0;

            switch (tac->type) {
                case TAC_ADD: result = left + right; break;
                case TAC_SUB: result = left - right; break;
                case TAC_MULT: result = left * right; break;
                case TAC_DIV:
                    if (right != 0) result = left / right;
                    else fprintf(stderr, "Erro: divisão por zero\n");
                    break;
            }

            tac->type = TAC_ASS;
            tac->op1 = new SYMBOL(SYMBOL_LIT_INT, to_string(result));
            tac->op2 = nullptr;
        }
    }
}

void tempOptimizer(TAC* first) {
    for (TAC* tac = first; tac && tac->next; tac = tac->next) {
        if (tac->type == TAC_ASS && tac->res && tac->op1 &&
            tac->res->text.find("temp") == 0) {
            
            TAC* next = tac->next;
            if (next->type == TAC_ASS && next->op1 && next->op1 == tac->res) {
                // Substitui:
                next->op1 = tac->op1;

                // Remove tac:
                if (tac->prev) tac->prev->next = tac->next;
                tac->next->prev = tac->prev;
                delete tac;
                tac = next->prev ? next->prev : next;
            }
        }
    }
}



void tacPrintFoward(TAC* tac){
    fprintf(stderr, "\n\n");
    for (; tac; tac = tac->next) {
        if(tac->type != TAC_SYMBOL) tacPrintSingle(tac);
    }
}
