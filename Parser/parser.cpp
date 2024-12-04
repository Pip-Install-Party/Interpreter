#include <iostream>
#include "parser.h"
#include <string>

using namespace TokenTypes;
std::string currTokenType;

// Constructor: Initializes the parser with a list of tokens and prepares a queue for processing.
Parser::Parser(std::vector<Token>& tokenList) {
    this->tokenList = tokenList; // Store the list of tokens.
    head = &tokenList.at(0);     // Set the head pointer to the first token.

    // Populate the queue with pointers to each token in the token list.
    for (int i = 0; i < tokenList.size(); i++) {
        tokenQueue.push(&tokenList.at(i));
    }
}

// State 0: The starting point of the parser. Prepares the queue and transitions to state 1.
void Parser::state0() {
    Token* token = tokenQueue.front(); // Get the first token from the queue.
    tokenQueue.pop();                  // Remove it from the queue.
    currTokenType = token->getType();  // Store the type of the current token.
    state1(token);                     // Transition to state 1 with the current token.
}

// State 1: The main loop of the parser. Processes tokens and determines the appropriate next state.
void Parser::state1(Token* lastToken) {
    while (!tokenQueue.empty()) {
        Token* token = tokenQueue.front();
        tokenQueue.pop();
        currTokenType = token->getType();

        // Check for semicolons and braces to determine the next action.
        if (currTokenType == SEMICOLON) {
            if (this->ignore > 0) { // Handle cases where semicolons are ignored (e.g., `for` loops).
                lastToken->setSibling(token);
                ignore--;
                state1(token);
            } else { // Otherwise, transition to state 2.
                lastToken->setSibling(token);
                state2(token);
            }
        } else if (currTokenType == L_BRACE) { // Handle the start of a block.
            lastToken->setChild(token);
            state2(token);
        } else if (lastToken->getType() == R_BRACE) { // Handle the end of a block.
            lastToken->setChild(token);
        } else if (lastToken->getType() == L_BRACKET) { // Handle array indexing.
            state3(token);
            lastToken->setSibling(token);
        } else if (token->getType() == IDENTIFIER) { // Handle identifiers.
            lastToken->setSibling(token);
            if (contains(lastToken->getValue())) { // Check for reserved words.
                lastToken->setSibling(token);
                state4(lastToken, token);
            }
        } else { // Default behavior: set sibling token.
            lastToken->setSibling(token);
        }
        state1(token); // Continue processing.
    }
}

// State 2: Handles tokens after semicolons or braces and manages block structures.
void Parser::state2(Token* lastToken) {
    if (tokenQueue.empty()) return; // Exit if no tokens remain.

    Token* token = tokenQueue.front();
    tokenQueue.pop();
    lastToken->setChild(token); // Set the current token as a child.

    // Handle "for" loops by ignoring the next two semicolons.
    if (token->getValue() == "for") {
        this->ignore += 2;
    }

    state1(token); // Transition back to state 1.
}

// State 3: Validates and processes tokens inside square brackets (e.g., array indices).
void Parser::state3(Token* token) {
    if (token->getType() != INTEGER) { // Check if the token is not an integer.
        if (token->getType() == IDENTIFIER) { // Check if it is an identifier.
            if (contains(token->getValue())) {
                std::cerr << "Error on line: " << token->getLineNumber() << " incompatible token within square braces.";
                exit(1);
            }
        } else { // Handle invalid tokens inside square brackets.
            std::cerr << "Error on line: " << token->getLineNumber() << " incompatible token within square braces.";
            exit(1);
        }
    } else if (std::stoi(token->getValue()) < 0) { // Ensure array size is positive.
        std::cerr << "Error on line: " << token->getLineNumber() << " array declaration size must be positive.";
        exit(1);
    }
}

// State 4: Handles reserved words and identifiers, ensuring correct usage.
void Parser::state4(Token* lastToken, Token* token) {
    if (lastToken->getValue() != "procedure" && lastToken->getValue() != "function") {
        if (contains(token->getValue())) { // Error for reserved word misuse.
            std::cerr << "Syntax error on line " << token->getLineNumber() << ": reserved word \"" << token->getValue() << "\" cannot be used for a variable name.";
            exit(1);
        }
        lastToken->setSibling(token);
    } else { // Handle procedures or functions.
        lastToken = token;
        token = tokenQueue.front();
        lastToken->setSibling(token);
        if (token->getType() == IDENTIFIER) {
            state5(lastToken, token);
        } else {
            state1(token);
        }
    }
}

// State 5: Processes function names and checks for reserved word misuse.
void Parser::state5(Token* lastToken, Token* token) {
    if (contains(lastToken->getValue()) && contains(token->getValue())) { // Error for reserved words as function names.
        std::cerr << "Syntax error on line " << token->getLineNumber() << ": reserved word \"" << token->getValue() << "\" cannot be used for a function name.";
        exit(1);
    } else {
        lastToken->setSibling(token);
    }
}

// Helper function to check if a string is a reserved word.
bool Parser::contains(std::string token) {
    for (int i = 0; i < reserved.size(); i++) {
        if (token == reserved.at(i)) {
            return true;
        }
    }
    return false;
}

// Prints the syntax tree to the provided output stream in a structured format.
void Parser::printTree(std::ofstream& rdpOutput) {
    Token* temp = head; // Start from the root of the tree.
    int colCount = 0;   // Track column width for formatting.

    while (temp != nullptr) {
        std::string tokenName = temp->getValue(); // Get the token's value.
        rdpOutput << tokenName;                  // Print the token.
        colCount += tokenName.size();            // Update column width.

        if (temp->getSibling() != nullptr) { // Check for siblings.
            rdpOutput << "--->";
            colCount += 4;
            temp = temp->getSibling();
        } else if (temp->getChild() != nullptr) { // Check for children.
            rdpOutput << '\n';
            for (int j = 1; j < colCount; j++) {
                rdpOutput << " "; // Print spaces for formatting.
            }
            rdpOutput << "|\n";
            for (int k = 1; k < colCount; k++) {
                rdpOutput << " "; // Print spaces for formatting.
            }
            rdpOutput << "⌄\n";
            for (int i = 0; i < colCount; i++) {
                rdpOutput << " "; // Print spaces for formatting.
            }
            temp = temp->getChild();
        } else {
            return;
        }
    }
}