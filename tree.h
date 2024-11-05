#ifndef TREE_H
#define TREE_H
#include <iostream>
#include "token.h"

const std::vector<std::string> varTypes = {"int", "void", "char", "bool", "string", "short", "long"};
const std::vector<std::string> equationOperators = {"+", "-", "/", "%", "*", "(", ")", "=", "'", "<", ">", "<=", ">=", "&&"};

class Tree {
    private:
        void printTree(Token*, Token*);
        bool contains(const std::vector<std::string>, std::string);
        Token* handleAssignment(Token*);
        int getPrecedence(char op);
        bool isOperator(char c);
        std::string infixToPostfix(const std::string& infix);


    public: 
        Tree(Token* head) { printTree(head, nullptr); }
        ~Tree();
};

#endif // TREE_H