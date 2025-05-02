// Júlia Mombach da Silva

#ifndef SYMBOLS_HEADER
#define SYMBOLS_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace std;

struct SYMBOL {
    int type;
    string text;
    public:
        SYMBOL(int t, string s): type(t), text(s) {};
};

SYMBOL *symbolInsert(int type, char *text);
void printSymbolsTable();

#endif