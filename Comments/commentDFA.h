// commentDFA.h
#ifndef COMMENTDFA_H // include guard
#define COMMENTDFA_H 

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <sstream> 

class CommentDFA
{
private:
    int lineCount = 1;
    // Function prototypes for the different DFA states
    void state0(std::ifstream& file, int& lineCount, std::ostringstream& buffer);  // Normal state, reading code
    void state1(std::ifstream& file, int& lineCount, std::ostringstream& buffer);  // Detected '/', checking if it's a comment
    void state2(std::ifstream& file, int& lineCount, std::ostringstream& buffer);  // Inside a C++-style comment, ignore until newline
    bool state3(std::ifstream& file, int& lineCount, int& commentLineCount, std::ostringstream& buffer);  // Inside a C-style block comment
    bool state4(std::ifstream& file, int& lineCount, int& commentLineCount, std::ostringstream& buffer);  // Checking for end of C-style block comment
    bool state5(std::ifstream& file, int& lineCount, int& quoteLinecount, std::ostringstream& buffer); // Inside a quoted string, skip over characters until end of quote  
    bool state6(std::ifstream& file, int& lineCount, std::ostringstream& buffer);  // Detected Asterisk, check if unterminated comment
public:
    CommentDFA() {}
    ~CommentDFA() { delete this; };
    void begin(std::ifstream& file, std::ostringstream& buffer) { state0(file, lineCount, buffer); }
};

#endif /* COMMENTDFA_H */
