# chat gpt que fez :)

CXX = g++
CXXFLAGS = -std=c++11 -Wall

TARGET = etapa5
OBJS = lex.yy.o main.o symbols.o parser.tab.o ast.o semantic.o tac.o

# Regra principal
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

# Compilar arquivos .cpp em .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# lex.yy.cpp depende do parser.tab.hpp para garantir ordem correta
lex.yy.cpp: scanner.l parser.tab.hpp
	flex -o lex.yy.cpp scanner.l

# parser.tab.cpp e parser.tab.hpp são gerados juntos a partir do parser.ypp
parser.tab.cpp parser.tab.hpp: parser.ypp
	bison -d parser.ypp

# Limpeza dos arquivos gerados
clean:
	rm -f $(TARGET) lex.yy.cpp parser.tab.* *.o
