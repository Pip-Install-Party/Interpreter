

#ifndef TOKEN_H
#define TOKEN_H

#include <string>

class Token 
{
private: 
    Token* lChild = nullptr;           
    Token* rSibling = nullptr;         
    std::string tokenType = "";         
    std::string token = "";           
    int lineNumber = 0;              

public:
    // Constructor: Initializes the token with its type, value, and line number.
    Token(const std::string& tokenType, const std::string& token, int lineNumber)
        : tokenType(tokenType), token(token), lineNumber(lineNumber) {}

    // Get the type of the token (e.g., IDENTIFIER, INTEGER).
    std::string getType() const {
        return tokenType;
    }

    // Get the value of the token (e.g., variable name, literal value).
    std::string getValue() const {
        return token;
    }

    // Get the line number where the token was found in the source code.
    int getLineNumber() const {
        return lineNumber;
    }

    // Set the sibling node for the current token.
    void setSibling(Token* newSibling) {
        rSibling = newSibling;
    }

    // Get the sibling node of the current token.
    Token* getSibling() {
        return rSibling;
    }

    // Set the child node for the current token.
    void setChild(Token* newChild) {
        lChild = newChild;
    }

    // Get the child node of the current token.
    Token* getChild() {
        return lChild;
    }
};

#endif // TOKEN_H