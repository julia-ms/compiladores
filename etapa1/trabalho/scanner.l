%{

#include "tokens.h"

int lineNumber = 1;
int running = 1;

void initMe() {

}

%}

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

[,;:()\[\]{}=\+\-*/%<>&|~]  { return yytext[0]; }

[0-9]+                      { return LIT_INT; }
[0-9]+\/[0-9]+              { return LIT_REAL; }
\"([^\\\"]|\\.)*\"          { return LIT_STRING; }
\'[^\']\'                   { return LIT_CHAR; }

[ \t] ;
\n                          { lineNumber++; }

"//".*

"\/--"                        { BEGIN(COMMENT); }

.                           { return TOKEN_ERROR; } 

<COMMENT>\n { ++lineNumber; }
<COMMENT>"--/" { BEGIN(INITIAL); }
<COMMENT>. 


%%

int getLineNumber(){
    return lineNumber;
}

int isRunning(){
    return running;
}

int yywrap(){
    running = 0;
    return 1;
}