#ifndef TREE_H
#define TREE_H
#include <iostream>
#include "token.h"

const std::vector<std::string> varTypes = {"int", "void", "char", "bool", "string", "short", "long"};

class Tree {
    private:
        void printTree(Token*);
        bool contains(std::string);
    public: 
        Tree(Token* head) { printTree(head); }
        ~Tree();
};

#endif // TREE_H