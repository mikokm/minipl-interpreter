CPPFLAGS=-Wall -Wextra -std=c++17 -g
OBJS=Main.o Scanner.o Parser.o

all: interpreter

interpreter: $(OBJS)
	$(CXX) $(CPPFLAGS) -o interpreter $(OBJS)

Main.o: src/Main.cpp
	$(CXX) $(CPPFLAGS) -c src/Main.cpp -o Main.o

Scanner.o: src/Scanner.cpp src/Scanner.h
	$(CXX) $(CPPFLAGS) -c src/Scanner.cpp -o Scanner.o

Parser.o: src/Parser.cpp src/Parser.h
	$(CXX) $(CPPFLAGS) -c src/Parser.cpp -o Parser.o

clean:
	rm $(OBJS) interpreter

format:
	clang-format -i -style=google src/*.cpp src/*.h
