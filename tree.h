#ifndef TREE_H
#define TREE_H
#include <iostream>
#include "token.h"
#include "table.h"
#include <vector>

const std::vector<std::string> varTypes = {"int", "void", "char", "bool", "string", "short", "long"};
const std::vector<std::string> equationOperators = {"+", "-", "/", "%", "*", "(", ")", "=", "==", "'", "<", ">", "<=", ">=", "&&"};

class Tree {
    private:
        Table* symbolTable;
        bool isCall = false;
        void printTree(Token*, Token*);
        bool contains(const std::vector<std::string>, std::string);
        bool isFunction(std::string tokenName);
        Token* handleAssignment(Token*);
        Token* handleIndex(Token *, std::vector<Token*>&);
        Token* handleFunction(Token *,std::vector<Token*>& equationAsVec, bool& isFunctionCall);
        int getPrecedence(std::string op);
        bool isIndex(std::string);
        bool isOperator(std::string c);
        std::vector<Token*> infixToPostfix(const std::vector<Token*> infix, bool isFunctionCall);
	short spaceCount = 0;

    public: 
        Tree(Token* head, Table* table) { symbolTable = table; printTree(head, nullptr); }
        ~Tree();
};

#endif // TREE_H
