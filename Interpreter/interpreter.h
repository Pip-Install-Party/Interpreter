#ifndef INTERPRETER_H
#define INTERPRETER_H
#include <iostream>
#include <stack>
#include <unordered_map>
#include <map>
#include "../Symbols/entry.h"
#include "../Tree/tree.h"
#include "../Tree/Node.h"


class Interpreter {
    private:
        Node* programCounter = nullptr;    // keeps track of AST's current instruction
        Tree* ast = nullptr;
        std::stack<int> expressionStack;    // stack to evaluate expressions
        std::unordered_map<std::string, Entry*> symbolTable;         // stores variable values
        std::vector<std::string> insertOrder = {};
        int curTableIndex = 0;

        void executeStatement(Node*);

        Node* nextStatement();
        void handleDeclaration(Node*);
        void handleAssignment(Node*);
        void handleSelection(Node*);
        void handlePrintf(Node*);
        void handleReturn(Node*);
        void handleFunction(Node*);
        void handleProcedure(Node*);   
        void handleIf(Node*);   
        void handleElse(Node*);  
        void handleWhile(Node*);   
        void handleFor(Node*);  


        /* ... add more functions as needed */

        // Helper functions
        std::unordered_map<std::string, Entry*> convertTable(Table*);
        bool isOperator(const std::string&);
        std::string evaluatePostfix(Node*);
        int performPostfixOperation(int, int, const std::string&);
        Entry* getEntryByIndex(int, std::vector<std::string>&);
        Entry* getParamListForEntry(std::vector<Entry*>, std::string, int);
        bool evaluateBooleanPostfix(Node*);
        bool performBooleanOperation(int, int, const std::string&);


    public:
        Interpreter(Table* table, Tree* tree) {
            ast = tree;
            symbolTable = convertTable(table);
        }
        void begin(Node*);
        ~Interpreter();
};

#endif // INTERPRETER_H
