#include <iostream>
#include "parser.h"
#include <string>
using namespace TokenTypes;
std::string currTokenType;

// Constructor that takes in our tokenList that our tokenizer created and builds a queue with it
Parser::Parser(std::vector<Token>& tokenList)
{
    this->tokenList = tokenList;
    head = &tokenList.at(0);
    // Populate the queue with pointers to each token in the token list
    for(int i = 0; i < tokenList.size(); i++){
        tokenQueue.push(&tokenList.at(i));
    }
}

// Begin state, preps queue and the pointer for being passed into state 1
void Parser::state0(){
    Token *token = tokenQueue.front();
    tokenQueue.pop();
    currTokenType = token->getType();
    state1(token);
}

// Main loop state that doesn't stop until the queue is empty
// Basically our "outer while loop" for the states
void Parser::state1(Token* lastToken) {
    while (!tokenQueue.empty()) {

        Token *token = tokenQueue.front();
        tokenQueue.pop();
        currTokenType = token->getType();

        // Checks if we have a semicolon or braces
        if (currTokenType == SEMICOLON) {
            // Checks if we need to ignore semicolons at all
            if (this->ignore > 0) {
                lastToken->setSibling(token);
                ignore--;
                state1(token);
            }
            // Otherwise go to state 2
            else {
                lastToken->setSibling(token);
                state2(token);
            }
        } else if (currTokenType == L_BRACE ) {
            // Set the last token's child to the current token
            lastToken->setChild(token);
            state2(token);
        } else if (lastToken->getType() == R_BRACE) {
            // Set the last token's child to the current token if the last token is a right brace
            lastToken->setChild(token);
        } else if (lastToken->getType() == L_BRACKET) {
            // Handle left bracket by going to state 3
            state3(token);
            lastToken->setSibling(token);
        } else if (token->getType() == IDENTIFIER){
            // Set the last token's sibling to the current token if it's an identifier
            lastToken->setSibling(token);
            if (contains(lastToken->getValue())) {
                // Handle reserved words
                lastToken->setSibling(token);
                state4(lastToken, token);
            }
        } else {
            // Set the last token's sibling to the current token for all other cases
            lastToken->setSibling(token);
        }
        // Continue to the next iteration of state 1
        state1(token);
    }
}

// Handles the next state after processing a semicolon or left brace
void Parser::state2(Token* lastToken){
    // Base case for if the queue is empty
    if (tokenQueue.empty()) return;

    // Storing the first token in the queue
    Token *token = tokenQueue.front();
    tokenQueue.pop(); // Popping it off
    // Setting lastToken's child to be token
    lastToken->setChild(token);

    // Check for "for" loops
    if (token->getValue() == "for") {
        // Figure out a way to ignore the next two semicolons
        this->ignore += 2;
    }
    // Go back to state 1
    state1(token);
}

// Handles the tokens inside square brackets, performs validation
void Parser::state3(Token* token){
    if (token->getType() != INTEGER) {
        if (token->getType() == IDENTIFIER) {
            if (contains(token->getValue())) {
                std::cout << currTokenType;
                std::cerr << "Error on line: " << token->getLineNumber() << " incompatbile token within square braces.";
                exit(1);
            }
        } else {
            // If the token is not an integer or identifier, output an error
            std::cout << currTokenType;
            std::cerr << "Error on line :" << token->getLineNumber() << " incompatbile token within square braces.";
            exit(1);
        }
    } else if (std::stoi(token->getValue()) < 0) {
        // Array declaration size must be positive
        std::cerr << "Error on line :" << token->getLineNumber() << " array declaration size must be a positive integer.";
        exit(1);
    }
    return;
}

// Handles special cases involving reserved words and identifiers
void Parser::state4(Token* lastToken, Token* token){
    if (lastToken->getValue() != "procedure" && lastToken->getValue() != "function") {
        if (contains(token->getValue())) {
            // Error if reserved word is used as a variable name
            std::cerr << "Syntax error on line " << token->getLineNumber() << ": reserved word \"" << token->getValue()  << "\" cannot be used for the name of a variable.";
            exit(1);
        }
        lastToken->setSibling(token);
    } else {
        // Handle function or procedure case
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

// Handles reserved word checks for function names
void Parser::state5(Token* lastToken, Token* token){
    if (contains(lastToken->getValue()) && contains(token->getValue())) {
        // Error if reserved word is used as a function name
        std::cerr << "Syntax error on line " << token->getLineNumber() << ": reserved word \"" << token->getValue()  << "\" cannot be used for the name of a function.";
        exit(1);
    } else {
        lastToken->setSibling(token);
    }
}

// Checks if the token is a reserved word
bool Parser::contains(std::string token){
    for (int i = 0; i < reserved.size(); i++) {
        if (token == reserved.at(i)){
            return true;
        }
    }
    return false;
}

// Prints the syntax tree to the provided output stream
void Parser::printTree(std::ofstream &rdpOutput){
    // Pointer for root of tree and counters for width/height
    Token *temp = head;
    int colCount = 0;

    // Main loop for printing the nodes
    while (temp != nullptr) {
        // Grab the first token value and print it
        std::string tokenName = temp->getValue();
        rdpOutput << tokenName;
        colCount += tokenName.size(); // Adjust column width

        // Check if there is a sibling
        if (temp->getSibling() != nullptr) {
            rdpOutput << "--->";
            colCount += 4; // Adjust column width
            temp = temp->getSibling();
        } else if (temp->getChild() != nullptr) { // Check if there is a child
            rdpOutput << '\n';
            for (int j = 1; j < colCount; j++) {
                // Loop that prints spaces for our width
                rdpOutput << " ";
            }
            rdpOutput << "|\n";
            for (int k = 1; k < colCount; k++) {
                // Loop that prints spaces for our width
                rdpOutput << " ";
            }
            rdpOutput << "⌄\n";
            for (int i = 0; i < colCount; i++) {
                // Loop that prints spaces for our width
                rdpOutput << " ";
            }
            temp = temp->getChild();
        } else {
            return;
        }
    }
}
