#ifndef COMMENTDFA_H
#define COMMENTDFA_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

// Declaration of the CommentDFA class
class CommentDFA
{
private:
    int lineCount = 1; // Tracks the current line number being processed in the file

    // State 0: Normal state, reading and processing code character by character
    void state0(std::ifstream& file, int& lineCount, std::ostringstream& buffer);

    // State 1: Detecting a '/' to determine if it's the start of a comment or just a division operator
    void state1(std::ifstream& file, int& lineCount, std::ostringstream& buffer);

    // State 2: Inside a C++-style comment, ignoring characters until the end of the line
    void state2(std::ifstream& file, int& lineCount, std::ostringstream& buffer);

    // State 3: Inside a C-style block comment, processing until the closing '*/'
    bool state3(std::ifstream& file, int& lineCount, int& commentLineCount, int startCommentLine, std::ostringstream& buffer);

    // State 4: Checking for the end of a C-style block comment by detecting '*/'
    bool state4(std::ifstream& file, int& lineCount, int& commentLineCount, int startCommentLine, std::ostringstream& buffer);

    // State 5: Inside a quoted string, processing until the closing quote is found
    bool state5(std::ifstream& file, int& lineCount, int& quoteLineCount, std::ostringstream& buffer);

    // State 6: Detected an asterisk ('*'), checking if it's part of an unterminated comment
    bool state6(std::ifstream& file, int& lineCount, std::ostringstream& buffer);

public:
    // Default constructor
    CommentDFA() = default;

    // Destructor
    ~CommentDFA() = default;

    // Entry point to start the DFA from State 0, initializing the process
    void begin(std::ifstream& file, std::ostringstream& buffer) { state0(file, lineCount, buffer); }
};

#endif
