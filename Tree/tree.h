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

const std::vector<std::string> varTypes = {"int", "void", "char", "bool", "string", "short", "long"};
const std::vector<std::string> equationOperators = {"+", "-", "/", "%", "*", "(", ")", "=", "==", "'", "<", ">", "<=", ">=", "&&"};

class Tree {
    private:
        Table* symbolTable;
        bool isCall = false;
        void printTree(Token*, Token*, std::ofstream&);
        bool contains(const std::vector<std::string>, std::string);
        bool isFunction(std::string tokenName);
        Token* handleAssignment(Token*, std::ofstream&);
        Token* handleIndex(Token *, std::vector<Token*>&);
        Token* handleFunction(Token *,std::vector<Token*>& equationAsVec, bool& isFunctionCall);
        int getPrecedence(std::string op);
        bool isIndex(std::string);
        bool isOperator(std::string c);
        std::vector<Token*> infixToPostfix(const std::vector<Token*> infix, bool isFunctionCall);
	    short spaceCount = 0;

    public: 
        Tree(Token* head, Table* table) { 
            symbolTable = table; 
             // Make an output filestream
            std::string filename = "AST_test_file_output.txt";
            std::ofstream ASTOutput(filename);
            printTree(head, nullptr, ASTOutput);    
        }
        ~Tree();
};

#endif // TREE_H
