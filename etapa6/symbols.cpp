// Júlia Mombach da Silva

#include "symbols.hpp"
#include <string>
#include <map>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

map<string, SYMBOL*> SymbolTable;

map<int, string> TokenNamesMap = {
    {SYMBOL_LIT_INT, "SYMBOL_LIT_INT"},
    {SYMBOL_LIT_REAL, "SYMBOL_LIT_REAL"},
    {SYMBOL_LIT_CHAR, "SYMBOL_LIT_CHAR"},
    {SYMBOL_LIT_STRING, "SYMBOL_LIT_STRING"},
    {SYMBOL_TK_IDENTIFIER, "SYMBOL_TK_IDENTIFIER"},
    {SYMBOL_TK_IDENTIFIER_VAR, "SYMBOL_TK_IDENTIFIER_VAR"},
    {SYMBOL_TK_IDENTIFIER_VECTOR, "SYMBOL_TK_IDENTIFIER_VECTOR"},
    {SYMBOL_TK_IDENTIFIER_FUNCTION, "SYMBOL_TK_IDENTIFIER_FUNCTION"},
    {SYMBOL_VECTOR_SIZE, "SYMBOL_VECTOR_SIZE"}
};

map<int, string> SymbolsDataTypeNameMap = {
    {DATATYPE_UNDEFINED, "DATATYPE_UNDEFINED"},
    {DATATYPE_INT, "DATATYPE_INT"},
    {DATATYPE_REAL, "DATATYPE_REAL"},
    {DATATYPE_CHAR, "DATATYPE_CHAR"},
    {DATATYPE_STRING, "DATATYPE_STRING"},
    {DATATYPE_BYTE, "DATATYPE_BYTE"},
    {DATATYPE_BOOL, "DATATYPE_BOOL"},
    {DATATYPE_INVALID, "DATATYPE_INVALID"}
};

SYMBOL *symbolInsert(int type, char *text){
    if(SymbolTable.find(string(text)) != SymbolTable.end()){
        return SymbolTable.find(string(text))->second;
    }
    SYMBOL *newSymbol = new SYMBOL(type, string(text));
    SymbolTable[string(text)] = newSymbol;
    return newSymbol;

}

void printSymbolsTable(){
    for(auto s: SymbolTable){
        string typeName = (TokenNamesMap.find(s.second->type) != TokenNamesMap.end()) 
                            ? TokenNamesMap[s.second->type] 
                            : to_string(s.second->type);
        string dataTypeName = (SymbolsDataTypeNameMap.find(s.second->dataType) != SymbolsDataTypeNameMap.end())
                            ? SymbolsDataTypeNameMap[s.second->dataType]
                            : to_string(s.second->dataType);
        printf("Symbol[%s, %s], datatype: %s\n", typeName.c_str(), s.second->text.c_str(), dataTypeName.c_str());
    }
}

SYMBOL* makeTemp(void){
    static int serialNumber = 0;
    static char buffer[32] = "";
    snprintf(buffer,32, "temp%d", serialNumber++);
    return symbolInsert(SYMBOL_TK_IDENTIFIER, buffer);
}

SYMBOL* makeLabel(void){
    static int serialNumber = 0;
    static char buffer[32] = "";
    snprintf(buffer,32, "label%d", serialNumber++);
    return symbolInsert(SYMBOL_TK_IDENTIFIER, buffer);
}

SYMBOL* makeConst(int value) {
    char buffer[64];
    sprintf(buffer, "%d", value);
    return symbolInsert(SYMBOL_LIT_INT, buffer);
}

string makeStringTemp(void){
    static int serialNumber = 0;
    static char buffer[32] = "";
    snprintf(buffer,32, "string%d", serialNumber++);
    return buffer;
}

void freeSymbols() {
    for (auto& entry : SymbolTable) {
        delete entry.second;  // libera o símbolo
    }
    SymbolTable.clear();  // limpa o mapa
}

map<string, string> createSymbolMap() {
    map<string, string> variableValues;
    string name;
    for (auto s : SymbolTable) {
        if (s.second->type == SYMBOL_TK_IDENTIFIER_VAR || s.second->type == SYMBOL_TK_IDENTIFIER) {
            if (s.second->text.substr(0, 5) != "label") variableValues[s.second->text] = "0";
        }else if(s.second->type == SYMBOL_LIT_STRING){
            name = makeStringTemp();
            variableValues[name] = s.second->text;
        }else if (s.second->type == SYMBOL_TK_IDENTIFIER_VECTOR) {
            variableValues[s.second->text] = "VECTOR";
        }
    }
    return variableValues;
}