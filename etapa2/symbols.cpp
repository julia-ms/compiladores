// Júlia Mombach da Silva

#include "symbols.hpp"
#include <string>
#include <map>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

map<string, SYMBOL*> SymbolTable;

map<int, string> TokenNames = {
    {7, "SYMBOLS_TK_IDENTIFIER"},
    {1, "SYMBOLS_LIT_INT"},
    {3, "SYMBOLS_LIT_CHAR"},
    {2, "SYMBOLS_LIT_REAL"},
    {4, "SYMBOLS_LIT_STRING"}
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
        string typeName = (TokenNames.find(s.second->type) != TokenNames.end()) 
                            ? TokenNames[s.second->type] 
                            : to_string(s.second->type);
        printf("Symbol[%s, %s]\n", typeName.c_str(), s.second->text.c_str());
    }
}