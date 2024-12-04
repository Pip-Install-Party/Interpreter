// Include necessary libraries for input/output operations.
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <filesystem>
#include <sstream>
#include "tokenizer.h"

// **State Functions**

// This function represents the initial state (state0) of the tokenizer's DFA.
// It processes the input stream character by character, handling tokens and delegating to other states as needed.
void Tokenizer::state0(std::istringstream &inputStream, int &lineCount) {
    char ch;
    auto singleCharTokens = getSingleCharTokens();
    while (true) {
        inputStream.get(ch);
        if (inputStream.eof()) return;

        // Handle newlines properly
        if (ch == '\n') {
            lineCount++;
            continue;
        }
        // Skip whitespace
        if (isspace(ch)) continue;

        // Handle single-character tokens
        if (singleCharTokens.count(ch)) {
            createAndPushToken(singleCharTokens[ch], ch, lineCount);
            continue;
        }
        // Handle numeric literals
        if (isdigit(ch)) {
            inputStream.putback(ch); // Return the character to the stream
            std::string intLiteral;
            state12(inputStream, lineCount, intLiteral);
            //    tokenList.push_back(Token("INTEGER", intLiteral, lineCount));
            continue;
        }
        // Handle '=' and '==' tokens
        if (ch == '=') {
            state13(inputStream, lineCount);
            continue;
        }
        // Handle '-' and possible negative numbers
        if (ch == '-') {
            state2(inputStream, lineCount);
            continue;
        }
        // Handle '>' and '>=' tokens
        if (ch == '>') {
            state6(inputStream, lineCount);
            continue;
        }
        // Handle '<' and '<=' tokens
        if (ch == '<') {
            state7(inputStream, lineCount);
            continue;
        }
        // Handle string literals
        if (ch == '"') {
            //tokenList.push_back(Token("DOUBLE_QUOTE", std::string(1, ch), lineCount));
            std::string strLiteral;
            state1(inputStream, lineCount, strLiteral);
            continue;
        }
        // Handle character literals
        if (ch == '\'') {
            tokenList.push_back(Token("SINGLE_QUOTE", std::string(1, ch), lineCount));
            state5(inputStream, lineCount);
            continue;
        }
        // Handle '&' and '&&' tokens
        if (ch == '&') {
            state10(inputStream, lineCount);
            continue;
        }
        // Handle identifiers (alphanumeric strings starting with a letter or underscore)
        if (isalpha(ch) || ch == '_') {
            std::string identifier(1, ch);
            while (inputStream.get(ch) && (isalnum(ch) || ch == '_')) {
                identifier += ch;
            }
            inputStream.putback(ch); // Return the non-identifier character
            tokenList.push_back(Token("IDENTIFIER", identifier, lineCount));
            continue;
        }
        state9(std::string(1, ch), "Unrecognized token: ", " at line " + std::to_string(lineCount) + " - Terminating...\n");
    }
}

// Handles string literals enclosed by double quotes. Accumulates characters into a string literal
// and processes escape sequences within the string.
void Tokenizer::state1(std::istringstream &inputStream, int &lineCount, std::string &strLiteral) {
    char ch;
    tokenList.push_back(Token("DOUBLE_QUOTE", "\"", lineCount));
    while (inputStream.get(ch)) {
        if (ch == '\\') {
            processEscapeSequence(inputStream, lineCount, strLiteral);
        } else if (ch == '"') {
            tokenList.push_back(Token("STRING", strLiteral, lineCount));
            tokenList.push_back(Token("DOUBLE_QUOTE", "\"", lineCount));
            return;
        } else {
            strLiteral += ch;
        }
    }
    // Handle unterminated string error
    throwError("Error: Unterminated string literal", "", lineCount);
}

// Handles minus signs or negative integers. Determines if the minus sign is part of a negative
// integer or a standalone token.
void Tokenizer::state2(std::istringstream &inputStream, int &lineCount) {
    char ch = getNextChar(inputStream, lineCount);
    if (isdigit(ch)) {
        std::string negativeInt = "-" + std::string(1, ch);
        state3(inputStream, lineCount, negativeInt);
    } else {
        inputStream.putback(ch);
        createToken("MINUS", "-", lineCount);
    }
}

// Processes sequences of digits to form integer literals. Continues appending digits until
// a non-digit character is encountered.
void Tokenizer::state3(std::istringstream &inputStream, int &lineCount, std::string &negativeInt) {
    char ch = getNextChar(inputStream, lineCount);
    if (isdigit(ch)) {
        negativeInt += ch;
        state3(inputStream, lineCount, negativeInt);
    } else {
        inputStream.putback(ch);
        createToken("INTEGER", negativeInt, lineCount);
    }
}

// Processes identifiers, which are alphanumeric strings or underscores. Appends characters
// to the identifier until a non-identifier character is encountered.
void Tokenizer::state4(std::istringstream &inputStream, int &lineCount, std::string &identifier) {
    while (true) {
        char ch = getNextChar(inputStream, lineCount);
        if (isalnum(ch) || ch == '_') {
            identifier += ch;
        } else {
            inputStream.putback(ch);
            break;
        }
    }
    createToken("IDENTIFIER", identifier, lineCount);
}

// Handles character literals enclosed in single quotes. Detects and validates character literals,
// including those with escape sequences.
void Tokenizer::state5(std::istringstream &inputStream, int &lineCount) {
    char ch = getNextChar(inputStream, lineCount);
    std::string charLiteral;
    if (ch == '\\') {
        processEscapeSequence(inputStream, lineCount, charLiteral);
    } else if (ch == '\'') {
        throwError("Error: Empty character literal", "", lineCount);
    } else {
        charLiteral += ch;
    }

    char closingQuote = getNextChar(inputStream, lineCount);
    if (closingQuote != '\'') {
        throwError("Error: Unterminated character literal", "", lineCount);
    }
    tokenList.push_back(Token("CHARACTER", charLiteral, lineCount));
    //tokenList.push_back(Token("STRING", charLiteral, lineCount)); // Add as a STRING token
    tokenList.push_back(Token("SINGLE_QUOTE", "'", lineCount));
}

// Handles the '>' and '>=' operators. Determines if the '>' is a standalone operator
// or part of a 'greater-than or equal to' token.
void Tokenizer::state6(std::istringstream &inputStream, int &lineCount) {
    handleComparisonOperator(inputStream, lineCount, "GT", "GT_EQUAL", '>');
}

// Handles the '<' and '<=' operators. Determines if the '<' is a standalone operator
// or part of a 'less-than or equal to' token.
void Tokenizer::state7(std::istringstream &inputStream, int &lineCount) {
    handleComparisonOperator(inputStream, lineCount, "LT", "LT_EQUAL", '<');
}

// Handles escape sequences within character literals. Resolves escape sequences and appends
// them to the string literal being processed.
void Tokenizer::state8(std::istringstream &inputStream, int &lineCount, std::string &strLiteral) {
    char escapeChar = getNextChar(inputStream, lineCount);
    std::unordered_map<char, std::string> escapeSequences = {
            {'n', "\n"}, {'t', "\t"}, {'r', "\r"}, {'b', "\b"},
            {'\'', "\'"}, {'\"', "\""}, {'0', "\0"}, {'\\', "\\"}
    };

    auto it = escapeSequences.find(escapeChar);
    if (it != escapeSequences.end()) {
        strLiteral += it->second;
    } else {
        throwError("Error: Unrecognized escape sequence \\", std::string(1, escapeChar), lineCount);
    }
}

// Handles errors and terminates the tokenizer. Outputs an error message and stops execution.
void Tokenizer::state9(const std::string &var, const std::string &message, const std::string &details) {
    throwError(message, var + details, -1); // -1 means lineCount is not applicable.
}

// Handles boolean AND (&&) and bitwise AND (&). Determines if the '&' is standalone or part
// of a boolean AND token.
void Tokenizer::state10(std::istringstream &inputStream, int &lineCount) {
    char ch = getNextChar(inputStream, lineCount);
    if (ch == '&') {
        createToken("BOOLEAN_AND", "&&", lineCount);
    } else {
        inputStream.putback(ch);
        createToken("AMPERSAND", "&", lineCount);
    }
}



// Handles sequences of digits to form integer literals. Continues appending digits until
// a non-digit character is encountered.
void Tokenizer::state12(std::istringstream &inputStream, int &lineCount, std::string &intLiteral) {
    intLiteral.clear(); // Ensure the variable is reset before use
    char ch;
    while (inputStream.get(ch)) {
        if (isdigit(ch)) {
            intLiteral += ch;
        } else {
            inputStream.putback(ch);
            break;
        }
    }
    // Validate the termination of the integer
    state14(inputStream, lineCount);
    createToken("INTEGER", intLiteral, lineCount);
}

// Handles the '=' and '==' operators. Determines if the '=' is standalone or part of an
// equality token.
void Tokenizer::state13(std::istringstream &inputStream, int &lineCount) {
    char ch = getNextChar(inputStream, lineCount);
    if (ch == '=') {
        createToken("BOOLEAN_EQUAL", "==", lineCount);
    } else {
        inputStream.putback(ch);
        createToken("ASSIGNMENT_OPERATOR", "=", lineCount);
    }
}

// // Validates the termination of integer tokens. Checks for valid terminators and reports an
// // error if an invalid character follows the integer.
void Tokenizer::state14(std::istringstream &inputStream, int &lineCount) {
    char ch = getNextChar(inputStream, lineCount);

    if (ch != ' ' && ch != ';' && ch != ')' && ch != ']') {
        std::ostringstream errorMessage;
        errorMessage << "Syntax error on line " << lineCount << ": invalid integer";

        throwError(errorMessage.str(), "", -1);
    }
    inputStream.putback(ch);
}

// *** Helper Functions ***

// Helper that returns a mapping of single-character tokens to their corresponding token types
std::unordered_map<char, std::string> Tokenizer::getSingleCharTokens() const {
    return {
            {'(', "L_PAREN"}, {')', "R_PAREN"}, {'{', "L_BRACE"}, {'}', "R_BRACE"},
            {'[', "L_BRACKET"}, {']', "R_BRACKET"}, {';', "SEMICOLON"}, {'/', "DIVIDE"},
            {',', "COMMA"}, {'+', "PLUS"}, {'*', "ASTERISK"}, {'%', "MODULO"},
            {'\\', "BACKSLASH"}, {'!', "BOOLEAN_NOT"}, {'_', "UNDERSCORE"}
    };
}

// Helper for state0 to add single-character tokens to the token list.
void Tokenizer::createAndPushToken(const std::string &type, char ch, int lineCount) {
    tokenList.push_back(Token(type, std::string(1, ch), lineCount));
}

// Helper for state1 to handle escape sequences in string literals.
void Tokenizer::processEscapeSequence(std::istringstream &inputStream, int &lineCount, std::string &strLiteral) {

    char escapeChar;
    if (inputStream.get(escapeChar)) {
        if (escapeChar == 'n') strLiteral += "\\n";
        else if (escapeChar == 't') strLiteral += "\\t";
        else if (escapeChar == '\\') strLiteral += "\\\\";
        else if (escapeChar == '"') strLiteral += "\\\"";
        else {
            strLiteral += '\\';
            strLiteral += escapeChar;
        }
    } else {
        throwError("Error: Unterminated escape sequence", "", lineCount);
    }
}

// Helper for multiple states to fetch the next character from the input stream.
char Tokenizer::getNextChar(std::istringstream &inputStream, int &lineCount) {
    char ch;
    if (inputStream.get(ch)) {
        if (ch == '\n') lineCount++;
        return ch;
    }
    throw std::runtime_error("Unexpected EOF");
}

// Helper to creates a new token and adds it to the token list if it's not a duplicate of the previous token
void Tokenizer::createToken(const std::string &type, const std::string &value, int lineCount) {
    if (!tokenList.empty() &&
        tokenList.back().getType() == type &&
        tokenList.back().getValue() == value &&
        tokenList.back().getLineNumber() == lineCount) {
        return; // Prevent duplicate consecutive tokens
    }
    tokenList.push_back(Token(type, value, lineCount));
}

// Helper for multiple states to handle errors and terminate execution.
void Tokenizer::throwError(const std::string &message, const std::string &details, int lineCount) {
    // If lineCount is -1, do not append the line number
    if (lineCount == -1) {
        std::cerr << message << details << std::endl;
    } else {
        std::cerr << message << details << " at line " << lineCount << std::endl;
    }
    exit(1);
}

// Helper for state6, state7, and state13 to handle comparison operators like <, >, and =.
void Tokenizer::handleComparisonOperator(std::istringstream &inputStream, int &lineCount,
                                         const std::string &singleTokenType, const std::string &doubleTokenType, char comparisonChar) {

    char ch = getNextChar(inputStream, lineCount);
    if (ch == '=') {
        createToken(doubleTokenType, std::string(1, comparisonChar) + "=", lineCount);
    } else {
        inputStream.putback(ch);
        createToken(singleTokenType, std::string(1, comparisonChar), lineCount);
    }
}