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
        std::stack<int> loopStack;
        std::unordered_map<std::string, std::vector<Entry*>> symbolTable;         // stores variable values
        std::map<std::string, Node*> functionMap;    // Stores the node of the start of a function 
        std::vector<std::string> functionVector;
        Entry* rawTable;
        std::vector<std::string> insertOrder = {};
        int curTableIndex = 0;
        int mainline = 0;
        std::string result = "";
        short curScope = 2;

        void executeStatement(Node*);

        Node* nextStatement();
        Node* nextNode(Node*);
        void setProgramCounter(Node* node);
        void handleAssignment(Node*);
        void handlePrintf(Node*);
        std::string handleReturn(Node*);
        void handleFunction(Node*);
        void handleProcedure(Node*);   
        void handleIf(Node*);   
        void handleElse(Node*);  
        void handleWhile(Node*);   
        void handleFor(Node*);  


        /* ... add more functions as needed */

        // Helper functions
        std::unordered_map<std::string, std::vector<Entry*>> convertTable(Table*);
        bool isOperator(const std::string&);
        bool isBooleanOperator(const std::string&);
        bool isArithmeticOperator(const std::string&);
        std::string evaluatePostfix(Node*);
        int performPostfixOperation(int, int, const std::string&);
        bool evaluateBooleanPostfix(Node*);
        bool performBooleanOperation(int, int, const std::string&);
        Node* skipBlock(Node*); 
        Node* executeBlock(Node*);
        std::string executeCall(Node*);
        short numFunctions(Entry*);
        std::string intToHex(int);
        std::string hexToInt(std::string);
        Entry* getEntry(std::string);
        void printSymbols();




    public:
        Interpreter(Table* table, Tree* tree, int line) {
            ast = tree;
            rawTable = table->getHead();
            symbolTable = convertTable(table);
            mainline = line;
        }
        void begin(Node*);
        ~Interpreter();
};

#endif // INTERPRETER_H
