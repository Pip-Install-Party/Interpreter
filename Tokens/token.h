// The Token class defines the attributes of a Token, including its type, contents, and line number.
#ifndef TOKEN_H
#define TOKEN_H
class Token 
{
private: 
    Token *lChild = nullptr;
    Token *rSibling = nullptr;
    std::string tokenType = "";
    std::string token = "";
    int lineNumber = 0;
public:

    Token(const std::string& tokenType, const std::string& token, int lineNumber)
        : tokenType(tokenType), token(token), lineNumber(lineNumber) {}

    // Get the type of the token.
    std::string getType() const {
        return tokenType;
    }

    // Get the value of the token.
    std::string getValue() const {
        return token;
    }

    // Get the line number where the token was found
    int getLineNumber() const { return lineNumber;}

    void setSibling(Token *newSibling) { rSibling = newSibling; }

    Token* getSibling(){ return rSibling; }

    void setChild(Token *newChild) { lChild = newChild; }

    Token* getChild(){ return lChild; }
};
#endif // TOKEN_H