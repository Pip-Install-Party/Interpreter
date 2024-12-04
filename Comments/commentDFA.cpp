#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "commentDFA.h"



// Helper function: Read the next character and update line count
bool getNextChar(std::ifstream& file, char& ch, int& lineCount) {
    file.get(ch);
    if (file.eof()) return false;
    if (ch == '\n') lineCount++;
    return true;
}

// Helper function: Report error and terminate
void reportError(const std::string& message, int errorLine) {
    std::cerr << "ERROR: " << message << " on line " << errorLine << std::endl;
    exit(1);
}

// State 0: Normal state, reading and printing code character by character
void CommentDFA::state0(std::ifstream& file, int& lineCount, std::ostringstream& buffer) {
    char ch;
    if (!getNextChar(file, ch, lineCount)) {
        buffer << "\n"; // End of input
        return;
    }

    if (ch == '/') {
        state1(file, lineCount, buffer);
    } else if (ch == '*') {
        if (!state6(file, lineCount, buffer)) {
            reportError("Program contains C-style, unterminated comment", lineCount);
        }
    } else if (ch == '"') {
        buffer << ch;
        int quoteLineCount = lineCount;
        if (!state5(file, lineCount, quoteLineCount, buffer)) {
            reportError("Program contains C-style, unterminated quote", quoteLineCount);
        }
    } else {
        buffer << ch;
        state0(file, lineCount, buffer);
    }
}

// State 1: After encountering '/', decide if it's a comment or division operator
void CommentDFA::state1(std::ifstream& file, int& lineCount, std::ostringstream& buffer) {
    char ch;
    if (!getNextChar(file, ch, lineCount)) return;

    if (ch == '/') {
        buffer << "  "; // Add whitespace for single-line comment
        state2(file, lineCount, buffer);
    } else if (ch == '*') {
        buffer << "  "; // Add whitespace for block comment
        int startCommentLine = lineCount;
        int commentLineCount = 0;
        if (!state3(file, lineCount, commentLineCount, startCommentLine, buffer)) {
            reportError("Program contains C-style, unterminated comment", startCommentLine);
        }
    } else {
        buffer << '/' << ch;
        state0(file, lineCount, buffer);
    }
}

// State 2: Inside a C++-style single-line comment
void CommentDFA::state2(std::ifstream& file, int& lineCount, std::ostringstream& buffer) {
    char ch;
    if (!getNextChar(file, ch, lineCount) || ch == '\n') {
        buffer << "\n"; // End of single-line comment
        state0(file, lineCount, buffer);
        return;
    }
    buffer << " "; // Replace comment content with spaces
    state2(file, lineCount, buffer);
}

// State 3: Inside a C-style block comment
bool CommentDFA::state3(std::ifstream& file, int& lineCount, int& commentLineCount, int startCommentLine, std::ostringstream& buffer) {
    char ch;
    if (!getNextChar(file, ch, lineCount)) return false;

    if (ch == '*') {
        return state4(file, lineCount, commentLineCount, startCommentLine, buffer);
    } else if (ch == '\n') {
        buffer << "\n";
        commentLineCount++;
    } else {
        buffer << " ";
    }
    return state3(file, lineCount, commentLineCount, startCommentLine, buffer);
}

// State 4: Check for '/' to close the C-style block comment
bool CommentDFA::state4(std::ifstream& file, int& lineCount, int& commentLineCount, int startCommentLine, std::ostringstream& buffer) {
    char ch;
    if (!getNextChar(file, ch, lineCount)) return false;

    if (ch == '/') {
        buffer << "  "; // Close block comment
        state0(file, lineCount, buffer);
        return true;
    } else if (ch == '*') {
        file.putback(ch);
    } else {
        buffer << " ";
    }
    return state3(file, lineCount, commentLineCount, startCommentLine, buffer);
}

// State 5: Inside a quoted string
bool CommentDFA::state5(std::ifstream& file, int& lineCount, int& quoteLineCount, std::ostringstream& buffer) {
    char ch;
    if (!getNextChar(file, ch, lineCount)) return false;

    buffer << ch;
    if (ch == '"') {
        state0(file, lineCount, buffer);
        return true;
    }
    return state5(file, lineCount, quoteLineCount, buffer);
}

// State 6: Detected an asterisk ('*'), check for unterminated C-style block comment
bool CommentDFA::state6(std::ifstream& file, int& lineCount, std::ostringstream& buffer) {
    char ch;
    if (!getNextChar(file, ch, lineCount) || ch != '/') {
        buffer << '*';
        if (ch != '/') file.putback(ch);
        state0(file, lineCount, buffer);
        return true;
    }
    return false;
}
