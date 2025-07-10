// Júlia Mombach da Silva

#ifndef SYMBOLS_HEADER
#define SYMBOLS_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>

using namespace std;

struct SYMBOL {
    int type;
    int dataType;
    string text;
    public:
        SYMBOL(int t, string s): type(t), text(s) {};
};

enum TokenNames{
    SYMBOL_LIT_INT = 1,
    SYMBOL_LIT_REAL = 2,
    SYMBOL_LIT_STRING = 3,
    SYMBOL_LIT_CHAR = 4,
    SYMBOL_LIT_BYTE = 5,
    SYMBOL_TK_IDENTIFIER = 7,
    SYMBOL_TK_IDENTIFIER_VAR = 8,
    SYMBOL_TK_IDENTIFIER_VECTOR = 9,
    SYMBOL_TK_IDENTIFIER_FUNCTION = 10,
    SYMBOL_VECTOR_SIZE=11
};

enum SymbolsDataTypeName{
    DATATYPE_UNDEFINED  = 0,
    DATATYPE_INT = 1,
    DATATYPE_REAL = 2,
    DATATYPE_STRING = 3,
    DATATYPE_CHAR = 4,
    DATATYPE_BYTE = 5,
    DATATYPE_BOOL = 6,
    DATATYPE_VECTOR = 7,
    DATATYPE_INVALID = 8
};

SYMBOL *symbolInsert(int type, char *text);
void printSymbolsTable();

// TACS
SYMBOL* makeTemp();
SYMBOL* makeLabel();
SYMBOL* makeConst(int value);

void freeSymbols();
map<string, string> createSymbolMap();
#endif