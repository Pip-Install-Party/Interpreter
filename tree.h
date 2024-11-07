#ifndef TREE_H
#define TREE_H
#include <iostream>
#include "token.h"
#include <vector>

const std::vector<std::string> varTypes = {"int", "void", "char", "bool", "string", "short", "long"};
const std::vector<std::string> equationOperators = {"+", "-", "/", "%", "*", "(", ")", "=", "'", "<", ">", "<=", ">=", "&&"};

class Tree {
    private:
        void printTree(Token*, Token*);
        bool contains(const std::vector<std::string>, std::string);
        Token* handleAssignment(Token*);
        Token* handleFunction(Token *,std::vector<Token*>& equationAsVec, bool& isFunctionCall);
        int getPrecedence(std::string op);
        bool isOperator(std::string c);
        std::vector<Token*> infixToPostfix(const std::vector<Token*> infix, bool isFunctionCall);


    public: 
        Tree(Token* head) { printTree(head, nullptr); }
        ~Tree();
};

#endif // TREE_H