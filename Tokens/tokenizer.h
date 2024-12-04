#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <filesystem>
#include <vector>
#include <unordered_map>
#include "token.h"

class Tokenizer {
private:
    // Vector to store a list of tokens
    std::vector<Token> tokenList;

    // Variable to track the current line number in the input stream, starting from 1.
    int lineCount = 1;

    // Declare all the different states of the tokenizer. Each state processes a different
    // type of token or character in the input stream.

    // Main state that branches to other states based on input.
    void state0(std::istringstream &inputStream, int &lineCount);

    // State to handle string literals (enclosed by double quotes).
    void state1(std::istringstream &inputStream, int &lineCount, std::string &strLiteral);

    // State to handle the minus sign and potentially negative numbers.
    void state2(std::istringstream &inputStream, int &lineCount);

    // State to process a sequence of digits, forming integers.
    void state3(std::istringstream &inputStream, int &lineCount, std::string &negativeInt);

    // State to process identifiers (alphanumeric characters and underscores).
    void state4(std::istringstream &inputStream, int &lineCount, std::string &identifier);

    // State to handle character literals (enclosed by single quotes).
    void state5(std::istringstream &inputStream, int &lineCount);

    // State to process the '>' (greater than) operator and '>=' (greater than or equal to).
    void state6(std::istringstream &inputStream, int &lineCount);

    // State to process the '<' (less than) operator and '<=' (less than or equal to).
    void state7(std::istringstream &inputStream, int &lineCount);

    // State to process escape sequences in string literals.
    void state8(std::istringstream &inputStream, int &lineCount, std::string &strLiteral);

    // Fail state
    void state9(const std::string &var, const std::string &message, const std::string &details);

    // State to process boolean AND ('&&') or bitwise AND ('&').
    void state10(std::istringstream &inputStream, int &lineCount);

    // State to process a continuous sequence of digits (integer values).
    void state12(std::istringstream &inputStream, int &lineCount, std::string &intLiteral);

    // State to process the assignment operator ('=') and equality operator ('==').
    void state13(std::istringstream &inputStream, int &lineCount);

    // State to validate the termination of integer tokens.
    void state14(std::istringstream &inputStream, int &lineCount);

    // Helper functions to reduce redundancy
    void createToken(const std::string &type, const std::string &value, int lineCount);
    void throwError(const std::string &message, const std::string &details, int lineCount);
    char getNextChar(std::istringstream &inputStream, int &lineCount);
    void handleComparisonOperator(std::istringstream &inputStream, int &lineCount, const std::string &singleTokenType, const std::string &doubleTokenType, char comparisonChar);
    // Helper function to handle escape sequences within a string literal.
    void processEscapeSequence(std::istringstream &inputStream, int &lineCount, std::string &strLiteral);
    void addTokenIfNew(const std::string &type, const std::string &value, int lineCount);



public:
    // Constructor for the Tokenizer class. Initializes an empty object.
    Tokenizer() {}

    // Destructor for the Tokenizer class.
    // Cleans up resources used by the class (deletes this object).
    ~Tokenizer() {}

    // Begin the tokenization process from the initial state (state0).
    // This method takes the input stream and a buffer to store token output.
    void begin(std::istringstream &inputStream) { state0(inputStream, lineCount); }

    // Returns the list of tokens processed by the tokenizer.
    const std::vector<Token>& getTokens() const { return tokenList; }

    // Returns a mapping of single-character tokens to their token types.
    std::unordered_map<char, std::string> getSingleCharTokens() const;

    // Creates a new token and adds it to the token list.
    void createAndPushToken(const std::string &type, char ch, int lineCount);

    // Declaration of createStringToken
    //void createStringToken(const std::string &strLiteral, int lineCount);

};

#endif // TOKENIZER_H