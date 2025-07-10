CXX = g++
CXXFLAGS = -std=c++11 -Wall -g

TARGET = etapa7
OBJS = lex.yy.o main.o symbols.o parser.tab.o ast.o semantic.o tac.o asm.o

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

lex.yy.cpp: scanner.l parser.tab.hpp
	flex -o lex.yy.cpp scanner.l

parser.tab.cpp parser.tab.hpp: parser.ypp
	bison -d parser.ypp

clean:
	rm -f $(TARGET) lex.yy.cpp parser.tab.* *.o
