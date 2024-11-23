#ifndef INTERPRETER_H
#define INTERPRETER_H
#include <iostream>
#include <stack>
#include <unordered_map>
#include "../Symbols/entry.h"
#include "../Tree/tree.h"
#include "../Tree/Node.h"


class Interpreter {
    private:
        Node* programCounter = nullptr;    // keeps track of AST's current instruction
        Tree* ast = nullptr;
        std::stack<int> expressionStack;    // stack to evaluate expressions
        std::unordered_map<std::string, Entry*> symbolTable;         // stores variable values

    public:
        Interpreter(Table* table, Tree* tree) {
            ast = tree;
            symbolTable = convertTable(table);
        }

        void begin(Node*);
        void executeStatement(Node*);

        Node* nextStatement();
        void handleDeclaration(Node*);
        void handleAssignment(Node*);
        void handleIteration(Node*);
        void handleSelection(Node*);
        void handlePrintf(Node*);
        void handleReturn(Node*);
        void handleFunction(Node*);
        void handleProcedure(Node*);   

        /* ... add more functions as needed */

        // Helper functions
        std::unordered_map<std::string, Entry*> convertTable(Table*);
        bool isOperator(const std::string&);
        std::string evaluatePostfix(Node* node);
        int performPostfixOperation(int, int, const std::string&);
        

        ~Interpreter();
};

#endif // INTERPRETER_H
