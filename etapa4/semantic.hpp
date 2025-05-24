#include "ast.hpp"

// seta o datatype dos literais
void setLiteralTypes(AST* node);

// ve se o datatype do literal é o mesmo da declaração
// ve se a variavel não ta sendo redeclarada
// ve se foi inicializado com a qtd certa de variaveis
void checkDeclarations(AST* node);

// ve se alguma variavel usada não foi declarada
void checkUndeclared();

/*
b. Uso correto – o uso dos identificadores deve ser compatível com sua declaração.
Variáveis somente podem ser usadas sem indexação, vetores somente podem ser
usados com indexação, funções apenas devem ser usadas com chamada, isto é,
seguidas da lista de argumentos entre parênteses;
c. Tipos de dados – As declarações também devem registrar os tipos de dados, em um
novo campo, dataType, na tabela de símbolos. Com o auxílio dessa informação,
quando necessário, os tipos de dados corretos devem ser verificados onde forem
usados, em expressões aritméticas, relacionais, lógicas, índices de vetores;
d. Argumentos e parâmetros – A lista de argumentos deve ser verificada contra a lista de
parâmetros formais na declaração da função. Cada chamada de função deve prover um
argumento para cada parâmetro, e ter o tipo compatível;
e. Outros: verificação do tipo da expressão de retorno no “return”;
*/
int checkNodeDataType(AST* node);
bool areCompatible(int nodeType1, int nodeType2);
bool isAritmethic(int nodeType);
bool isRelational(int nodeType);
bool isLogical(int nodeType);
