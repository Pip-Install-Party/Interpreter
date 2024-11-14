#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string>
#include <stack>
#include "..Tree/AST.h"               // Assignment 5: AST and expression nodes
#include "..Symbols/SymbolTable.h"     // Assignment 4: Symbol table for variable and function management
#include "..Tree/PostfixEvaluator.h"   // Assignment 5: Postfix evaluator for expressions

// Interpreter class: Central controller for executing the AST.
// This class integrates components from Assignments 1-5 to process, evaluate, and execute code.
class Interpreter {
public:
    Interpreter();
    ~Interpreter();

    // Initializes the interpreter by setting up the source code processing.
    void initialize(const std::string& sourceFile);

    // Begins execution of the program represented by the AST.
    void executeAST();

private:
    // Core functions to handle each type of AST node encountered during execution.
    void handleNode(ASTNode* node);
    void handleAssignment(ASTNode* assignmentNode);
    void handleControlFlow(ASTNode* controlNode);
    void handleFunctionCall(ASTNode* functionNode);

    // Helper functions for evaluating expressions and managing scope.
    // Uses Assignment 5's PostfixEvaluator.
    int evaluateExpression(ASTNode* expressionNode); 

    // Manages variable/function scope (Assignment 4)
    void enterScope();  

     // Exits scope when leaving functions or blocks.                             .
    void exitScope();                               

    // Components from previous assignments
    // Assignment 4: Manages variables and scope.
    SymbolTable symbolTable;    

    // Assignment 5: Evaluates expressions in postfix notation.                     
    PostfixEvaluator postfixEvaluator;   

    // Assignment 3: The root of the AST, built by the parser.            
    AST* astRoot;                                    

    // Additional data structures for program execution
    // Temporary stack for expression evaluation.
    std::stack<int> expressionStack;  

      // Stack to manage control flow and function calls.               
    std::stack<ASTNode*> programCounterStack;      
};

#endif // INTERPRETER_H