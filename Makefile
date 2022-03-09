CPPFLAGS=-Wall -Wextra -std=c++17
OBJS=Main.o Scanner.o

all: interpreter

interpreter: $(OBJS)
	$(CXX) $(CPPFLAGS) -o interpreter $(OBJS)

Main.o: src/Main.cpp
	$(CXX) $(CPPFLAGS) -c src/Main.cpp -o Main.o

Scanner.o: src/Scanner.cpp src/Scanner.h
	$(CXX) $(CPPFLAGS) -c src/Scanner.cpp -o Scanner.o

clean:
	rm $(OBJS) interpreter
