// #ifndef INTERPRETER_H
// #define INTERPRETER_H
// #include <iostream>

// class Interpreter {
//     private:
//     public:
//     Interpreter();
//     ~Interpreter();
// };

#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string>
#include "../Tree/tree.h"             
//#include "../Tree/AST.h"
#include "../Symbols/table.h"       

// Interpreter class: Central controller for executing the AST.
// This class integrates components from Assignments 1-5 to process, evaluate, and execute code.
class Interpreter {
public:
    Interpreter();
    ~Interpreter();

    // Initializes the interpreter by setting up the source code processing pipeline.
    // Uses components from Assignments 1, 2, and 3 for preprocessing, tokenization, and parsing.
    void initialize(const std::string& sourceFile);

    // Begins execution of the program represented by the AST.
    // Uses the AST generated from Assignment 3 and evaluated in Assignment 5.
    void executeAST();

private:
    // Core functions to handle each type of AST node encountered during execution.
    void handleNode(ASTNode* node);
    // Handles assignment statements.
    void handleAssignment(ASTNode* assignmentNode); 
    // Handles control flow constructs (e.g., if, while).
    void handleControlFlow(ASTNode* controlNode);  
    // Handles function calls and manages scope. 
    void handleFunctionCall(ASTNode* functionNode); 

    // Helper functions for evaluating expressions and managing scope.
    // Uses Assignment 5's PostfixEvaluator.
    int evaluateExpression(ASTNode* expressionNode);
    // Manages variable/function scope (Assignment 4). 
    void enterScope(); 
     // Exits scope when leaving functions or blocks.
    void exitScope(); 

    // Components from previous assignments
    // Assignment 4: Manages variables and scope.
    SymbolTable symbolTable;      
    // Assignment 5: Evaluates expressions in postfix notation.  
    PostfixEvaluator postfixEvaluator; 
     // Assignment 3: The root of the AST, built by the parser.
    AST* astRoot;                  
};

#endif // INTERPRETER_H