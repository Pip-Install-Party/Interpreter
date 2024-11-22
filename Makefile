all: tree.x

# Linking ncurses with the target binary
tree.x: main.o commentDFA.o tokenizer.o parser.o table.o tree.o
	g++ -std=c++17 -g main.o commentDFA.o tokenizer.o parser.o table.o tree.o -o tree.x -lncurses

main.o: main.cpp Comments/commentDFA.h Tokens/tokenizer.h Parser/parser.h Tests/testFiles.h
	g++ -std=c++17 -g -I./Comments -I./Tokens -I./Parser -I./Tests -I./Symbols -I./Tree main.cpp -o main.o -c

commentDFA.o: Comments/commentDFA.cpp Comments/commentDFA.h
	g++ -std=c++17 -g -I./Comments Comments/commentDFA.cpp -o commentDFA.o -c

tokenizer.o: Tokens/tokenizer.cpp Tokens/tokenizer.h
	g++ -std=c++17 -g -I./Tokens Tokens/tokenizer.cpp -o tokenizer.o -c

parser.o: Parser/parser.cpp Parser/parser.h
	g++ -std=c++17 -g -I./Parser Parser/parser.cpp -o parser.o -c

table.o: Symbols/table.cpp Symbols/table.h
	g++ -std=c++17 -g -I./Symbols Symbols/table.cpp -o table.o -c

tree.o: Tree/tree.cpp Tree/tree.h
	g++ -std=c++17 -g -I./Tree Tree/tree.cpp -o tree.o -c

# Clean the compiled files
clean:
	rm -f tree.x *.o *.txt
