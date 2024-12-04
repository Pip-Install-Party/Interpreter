#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <filesystem>
#include <sstream>

#include "../Tokens/token.h"       
#include "../Symbols/table.h"     
#include "Node.h"                 

// List of valid variable types used in declarations.
const std::vector<std::string> varTypes = {"int", "void", "char", "bool", "string", "short", "long"};

// List of operators used in equations and expressions.
const std::vector<std::string> equationOperators = {"+", "-", "/", "%", "*", "(", ")", "=", "==", "'", "<", ">", "<=", ">=", "&&"};

// List of reserved keywords or node types for specific operations in the AST.
const std::vector<std::string> reservedTypes = {"DECLARATION", "ASSIGNMENT", "FOR_EXPRESSION1", "FOR_EXPRESSION2", "FOR_EXPRESSION3", 
                                                "WHILE", "IF", "ELSE", "BEGIN_BLOCK", "END_BLOCK", "RETURN", "PRINTF", "CALL"};

class Tree {
private:
    Table* symbolTable;             
    bool isCall = false;            
    Node* root;                     

    // Constructs the Abstract Syntax Tree (AST) using the given tokens.
    void buildTree(Token*, Token*, std::ostringstream&);

    // Prints the tree structure to the output stream for debugging or visualization.
    void printTree(Node*);

    // Prints an arrow (e.g., "--->") with specific spacing for visualization purposes.
    void printArrow(int, std::ostringstream&);

    // Checks if a string is present in a given vector of strings.
    bool contains(const std::vector<std::string>, std::string);

    // Checks if a string represents a function in the symbol table.
    bool isFunction(std::string);

    // Handles assignment operations, including processing of identifiers and expressions.
    Token* handleAssignment(Token*, std::ostringstream&);

    // Handles array indexing operations and retrieves the associated tokens.
    Token* handleIndex(Token*, std::vector<Token*>&);

    // Handles function calls and retrieves associated parameters and tokens.
    Token* handleFunction(Token*, std::vector<Token*>&, bool&);

    // Retrieves the precedence level of an operator for parsing expressions.
    int getPrecedence(std::string op);

    // Checks if a string represents an array index (e.g., "L_BRACKET").
    bool isIndex(std::string);

    // Checks if a string is a valid operator.
    bool isOperator(std::string c);

    // Converts an infix expression to postfix notation for easier parsing and evaluation.
    std::vector<Token*> infixToPostfix(const std::vector<Token*>, bool);

    short spaceCount = 0;           

    // Saves the tree structure into a node-based representation for traversal and evaluation.
    Node* saveTree(std::ostringstream&);

    // Checks if a string is a reserved keyword or type.
    bool isReserved(std::string);

public:
    // Constructor: Initializes the tree with tokens and a symbol table.
    // Automatically builds and prints the AST.
    Tree(Token* head, Table* table) { 
        symbolTable = table; 
        // Stream for storing the AST representation.
        std::ostringstream ASTOutput;   
        buildTree(head, nullptr, ASTOutput);
         // Save the tree structure into nodes.
        root = saveTree(ASTOutput);   
        // Print the tree for debugging or visualization. 
        printTree(saveTree(ASTOutput)); 
    }

    // Returns the root node of the AST.
    Node* getHead() { return root; }

    // Destructor: Frees up resources associated with the tree.
    ~Tree();
};

#endif // TREE_H