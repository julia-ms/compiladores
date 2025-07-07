%{
// Júlia Mombach da Silva 

#include "symbols.hpp"
#include "ast.hpp"
#include "parser.tab.hpp"

#define SYMBOL_LIT_INTEGER 1
#define SYMBOL_LIT_REAL 2
#define SYMBOL_LIT_STRING 3
#define SYMBOL_LIT_CHAR 4
#define SYMBOL_IDENTIFIER 7

int lineNumber = 1;
int running = 1;

char* reverseINT(char* num);
char* reverseREAL(char* num);

%}

%option nounput
%option noinput

%x COMMENT

%%

"byte"                      { return KW_BYTE; }
"int"                       { return KW_INT; }
"real"                      { return KW_REAL; }

"if"                        { return KW_IF; }
"else"                      { return KW_ELSE; }
"do"                        { return KW_DO; }
"while"                     { return KW_WHILE; }
"read"                      { return KW_READ; } 
"print"                     { return KW_PRINT; }
"return"                    { return KW_RETURN; } 

"<="                        { return OPERATOR_LE; }
">="                        { return OPERATOR_GE; }
"=="                        { return OPERATOR_EQ; }
"!="                        { return OPERATOR_DIF; }

[,;:()\[\]{}=\+\-*/%<>&|~]  { return yytext[0]; } //menos na frente?

[a-zA-Z_][a-zA-Z0-9_]*      { yylval.symbol = symbolInsert(SYMBOL_IDENTIFIER, yytext); return TK_IDENTIFIER; }

[0-9]+                      { yylval.symbol = symbolInsert(SYMBOL_LIT_INTEGER, reverseINT(yytext)); return LIT_INT; }
[0-9]+\/[0-9]+              { char* reversed = reverseREAL(yytext); yylval.symbol = symbolInsert(SYMBOL_LIT_REAL, reversed); free(reversed); return LIT_REAL; }
\"([^\\\"]|\\.)*\"          { yylval.symbol = symbolInsert(SYMBOL_LIT_STRING, yytext); return LIT_STRING; }
\'[^\']\'                   { yylval.symbol = symbolInsert(SYMBOL_LIT_CHAR, yytext); return LIT_CHAR; }

[ \t]                       { }
\r?\n                       { lineNumber++; }

"//".*

"\/--"                      { BEGIN(COMMENT); }

.                           { return TOKEN_ERROR; } 

<COMMENT>\n                 { ++lineNumber; }
<COMMENT>"--/"              { BEGIN(INITIAL); }
<COMMENT>. 


%%


int getLineNumber(){
    return lineNumber;
}

int isRunning(){
    return running;
}

void initMe() {

}

int yywrap(){
    running = 0;
    return 1;
}

char* reverseINT(char* num) {
    int len = strlen(num);
    for (int i = 0; i < len / 2; i++) {
        char temp = num[i];
        num[i] = num[len - i - 1];
        num[len - i - 1] = temp;
    }
    return num;
}

char* reverseREAL(char* num){

    char* num1 = strtok(num, "/");
    char* num2 = strtok(NULL, "/");
    
    reverseINT(num1);
    reverseINT(num2);

    size_t revSize = strlen(num1) + strlen(num2) + 2;
    char* rev = (char*)malloc(revSize);
    snprintf(rev, revSize, "%s/%s", num1, num2);

    return rev;

}