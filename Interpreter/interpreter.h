#ifndef INTERPRETER_H
#define INTERPRETER_H
#include <iostream>
#include <stack>
#include "../Tree/tree.h"

class Interpreter {
    private:
        ASTNode* programCounter;    // keeps track of AST's current instruction
        std::stack<int> expressionStack;    // stack to evaluate expressions
        Table* symbolTable;         // stores variable values


    public:
        Interpreter();
        ~Interpreter();

        void begin(ASTNode* head);
        void executeStatement(ASTNode* node);

        void handleDeclaration(ASTNode*);
        void handleAssignment(ASTNode*);
        void handleIteration(ASTNode*);
        void handleSelection(ASTNode*);
        void handlePrintf(ASTNode*);
        void handleReturn(ASTNode*);
        void handleFunction(ASTNode*);
        void handleProcedure(ASTNode*);   

        /* ... add more functions as needed */
};

#endif // INTERPRETER_H
