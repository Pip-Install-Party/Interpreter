#ifndef TREE_H
#define TREE_H
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <filesystem>
#include <sstream>

#include "../Tokens/token.h"
#include "../Symbols/table.h"
#include "Node.h"

const std::vector<std::string> varTypes = {"int", "void", "char", "bool", "string", "short", "long"};
const std::vector<std::string> equationOperators = {"+", "-", "/", "%", "*", "(", ")", "=", "==", "'", "<", ">", "<=", ">=", "&&"};
const std::vector<std::string> reservedTypes = {"DECLARATION", "ASSIGNMENT", "FOR_EXPRESSION1", "FOR_EXPRESSION2", "FOR_EXPRESSION3", "WHILE", "IF", "ELSE",
                                                    "BEGIN_BLOCK", "END_BLOCK", "RETURN", "PRINTF", "CALL"};

class Tree {
    private:
        Table* symbolTable;
        bool isCall = false;
        void printTree(Token*, Token*, std::ofstream&);
        bool contains(const std::vector<std::string>, std::string);
        bool isFunction(std::string);
        Token* handleAssignment(Token*, std::ofstream&);
        Token* handleIndex(Token *, std::vector<Token*>&);
        Token* handleFunction(Token *,std::vector<Token*>&, bool&);
        int getPrecedence(std::string op);
        bool isIndex(std::string);
        bool isOperator(std::string c);
        std::vector<Token*> infixToPostfix(const std::vector<Token*>, bool);
	    short spaceCount = 0;
        Node* saveTree(std::ostringstream&);
        bool isReserved(std::string);
    public: 
        Tree(Token* head, Table* table) { 
            symbolTable = table; 
             // Make an output filestream
            std::string filename = "Interpreter_Output.txt";
            std::ofstream ASTOutput(filename);
            printTree(head, nullptr, ASTOutput);    
        }
        ~Tree();
};

#endif // TREE_H
