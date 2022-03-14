CPPFLAGS=-Wall -Wextra -std=c++17 -g
OBJS=Main.o Scanner.o Parser.o AST.o

all: interpreter

interpreter: $(OBJS)
	$(CXX) $(CPPFLAGS) -o interpreter $(OBJS)

Main.o: src/Main.cpp src/Utils.h
	$(CXX) $(CPPFLAGS) -c src/Main.cpp -o Main.o

AST.o: src/AST.cpp src/AST.h src/Utils.h
	$(CXX) $(CPPFLAGS) -c src/AST.cpp -o AST.o

Scanner.o: src/Scanner.cpp src/Scanner.h src/Utils.h
	$(CXX) $(CPPFLAGS) -c src/Scanner.cpp -o Scanner.o

Parser.o: src/Parser.cpp src/Parser.h src/Utils.h AST.o Scanner.o
	$(CXX) $(CPPFLAGS) -c src/Parser.cpp -o Parser.o

clean:
	rm $(OBJS) interpreter

format:
	clang-format -i -style=google src/*.cpp src/*.h
