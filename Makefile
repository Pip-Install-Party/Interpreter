# Define the compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -g

# Define the libraries
LIBS = -lncurses

# Final target
interpreter.x: main.o commentDFA.o tokenizer.o parser.o table.o tree.o interpreter.o
	$(CXX) $(CXXFLAGS) main.o commentDFA.o tokenizer.o parser.o table.o tree.o interpreter.o $(LIBS) -o interpreter.x

# Compilation of main.o
main.o: main.cpp Comments/commentDFA.h Tokens/tokenizer.h Parser/parser.h Tests/testFiles.h
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

# Compilation of commentDFA.o
commentDFA.o: Comments/commentDFA.cpp Comments/commentDFA.h
	$(CXX) $(CXXFLAGS) -c Comments/commentDFA.cpp -o commentDFA.o

# Compilation of tokenizer.o
tokenizer.o: Tokens/tokenizer.cpp Tokens/tokenizer.h
	$(CXX) $(CXXFLAGS) -c Tokens/tokenizer.cpp -o tokenizer.o

# Compilation of parser.o
parser.o: Parser/parser.cpp Parser/parser.h
	$(CXX) $(CXXFLAGS) -c Parser/parser.cpp -o parser.o

# Compilation of table.o
table.o: Symbols/table.cpp Symbols/table.h
	$(CXX) $(CXXFLAGS) -c Symbols/table.cpp -o table.o

# Compilation of tree.o
tree.o: Tree/tree.cpp Tree/tree.h
	$(CXX) $(CXXFLAGS) -c Tree/tree.cpp -o tree.o

# Compilation of interpreter.o
interpreter.o: Interpreter/interpreter.cpp Interpreter/interpreter.h
	$(CXX) $(CXXFLAGS) -c Interpreter/interpreter.cpp -o interpreter.o

# Clean the build
clean:
	rm -f interpreter.x *.o *.txt