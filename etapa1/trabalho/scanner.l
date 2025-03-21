%{
// Júlia Mombach da Silva
int lineNumber = 1;
int running = 1;

%}

%%

"byte"                      { printf("KW_BYTE "); }
"int"                       { printf("KW_INT "); }
"real"                      { printf("KW_REAL "); }

"if"                        { printf("KW_IF "); }
"else"                      { printf("KW_ELSE "); }
"do"                        { printf("KW_DO "); }
"while"                     { printf("KW_WHILE "); }
"read"                      { printf("KW_READ "); } 
"print"                     { printf("KW_PRINT "); }
"return"                    { printf("KW_RETURN "); } 

"<="                        { printf("OPERATOR_LE "); }
">="                        { printf("OPERATOR_GE "); }
"=="                        { printf("OPERATOR_EQ "); }
"!="                        { printf("OPERATOR_DIF "); }

[,;:()\[\]{}=\+\-*/%<>&|~]  { printf("OPERATOR %c " , yytext[0]); }

[0-9]+                      { printf("LIT_INT "); }
[0-9]+\/[0-9]+              { printf("LIT_REAL %s ", yytext); }
\"([^\\\"]|\\.)*\"          { printf("STRING %s ", yytext); }
\'[^\']\'                   { printf("LIT_CHAR "); }

[ \t]+ ;
\n { lineNumber++; }

%%

int main(int argc, char** argv){

    int tok;

    if (argc < 2){
        printf("Call %s filename\n", argv[0]); 
        exit(1);
    } 

    yyin = fopen(argv[1], "r");

    if(yyin == NULL){
        printf("Error");
        exit(1);
    }

    while(running){
        yylex();
        if(!running) break;
    }

    printf("File had %d lines", lineNumber);
    fclose(yyin);
    return 0;
}

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