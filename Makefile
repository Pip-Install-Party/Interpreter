interpreter.x: main.o commentDFA.o tokenizer.o parser.o table.o tree.o interpreter.o
	g++ -std=c++17 -g main.o commentDFA.o tokenizer.o parser.o table.o tree.o interpreter.o -o interpreter.x

main.o: main.cpp Comments/commentDFA.h Tokens/tokenizer.h Parser/parser.h Tests/testFiles.h
	g++ -std=c++17 -g main.cpp -o main.o -c

commentDFA.o: Comments/commentDFA.cpp Comments/commentDFA.h
	g++ -std=c++17 -g Comments/commentDFA.cpp -o commentDFA.o -c

tokenizer.o: Tokens/tokenizer.cpp Tokens/tokenizer.h
	g++ -std=c++17 -g Tokens/tokenizer.cpp -o tokenizer.o -c

parser.o: Parser/parser.cpp Parser/parser.h
	g++ -std=c++17 -g Parser/parser.cpp -o parser.o -c

table.o: Symbols/table.cpp Symbols/table.h
	g++ -std=c++17 -g Symbols/table.cpp -o table.o -c

tree.o: Tree/tree.cpp Tree/tree.h
	g++ -std=c++17 -g Tree/tree.cpp -o tree.o -c

interpreter.o: Interpreter/interpreter.cpp Interpreter/interpreter.h
	g++ -std=c++17 -g Interpreter/interpreter.cpp -o interpreter.o -c

clean:
	rm -f interpreter.x *.o *.txt
