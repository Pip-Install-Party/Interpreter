// commentDFA.cpp

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <sstream> 

#include "commentDFA.h"

// State 0: Normal state, reading and printing code character by character
void CommentDFA::state0(std::ifstream& file, int& lineCount, std::ostringstream& buffer) {
    char ch; 
    file.get(ch);  // Read a character from the file
    if (file.eof()) {  // Check if the end of file is reached
        buffer << "\n";
        return;  // End of input, stop the function
    } else if (ch == '/') {  // If the character is '/', move to state1
        return state1(file, lineCount, buffer);
    } else if (ch == '*'){
        if(!state6(file, lineCount, buffer)) {
            std::cerr << "ERROR: Program contains C-style, unterminated quote on line " << lineCount;
            exit(1);
        }
        return;
    } else if (ch == '"') {
        int quoteLineCount = 0;
        buffer << ch;
        if (!state5(file, lineCount, quoteLineCount, buffer)) {
            std::cerr << "ERROR: Program contains C-style, unterminated quote on line " << lineCount;
            exit(1);
        }
        return; 
    } else {  // Otherwise, print the character and stay in state0
        if (ch == '\n') {
            lineCount++;
        }
        buffer << ch;
        return state0(file, lineCount, buffer);
    }
}

// State 1: After hitting a '/', figure out if it's a comment or a division operator
void CommentDFA::state1(std::ifstream& file, int& lineCount, std::ostringstream& buffer) {
    char ch; 
    file.get(ch);  // Read the next character
    if (file.eof()) {  // Check for end of file
        return; 
    } else if (ch == '/') {  // If it's another '/', we have a C++-style comment, move to state2
        buffer << "  "; // Add whitespace
        return state2(file, lineCount, buffer);
    } else if (ch == '*') {  // If it's a '*', we have a C-style block comment, move to state3
        int commentLineCount = 0;
            buffer << "  "; // Add whitespace
        if (!state3(file, lineCount, commentLineCount, buffer)) {
            std::cerr << "ERROR: Program contains C-style, unterminated comment on line " << lineCount;
            exit(0);
        }
       return; 
    } else {  // Otherwise, it's just a '/', print it and return to state0
        if (ch == '\n') {
            lineCount++;
        }
        buffer << '/';  // Don't forget to print the first '/' since it's not part of a comment
        buffer << ch;   // Also print the current character
        return state0(file, lineCount, buffer);
    }
}

// State 2: Inside a C++-style line comment, skip characters until newline is reached
void CommentDFA::state2(std::ifstream& file, int& lineCount, std::ostringstream& buffer) {
    char ch; 
    file.get(ch);  // Read the next character
    if (ch == '\n') {  // If it's a newline, return to state0 to process the next line
        lineCount++;
        buffer << "\n"; // Add newline
        return state0(file, lineCount, buffer);
    } else {
        buffer << " "; // Add whitespace
    }
    return state2(file, lineCount, buffer);  // Otherwise, stay in state2, ignoring characters
}

// State 3: Inside a C-style block comment, look for the closing '*/'
bool CommentDFA::state3(std::ifstream& file, int& lineCount, int& commentLineCount, std::ostringstream& buffer) {
    char ch; 
    file.get(ch);  // Read the next character
    if (ch == '*') {  // If it's a '*', check if it's the start of '*/' (end of comment)
        return state4(file, lineCount, commentLineCount, buffer);
    } else if (ch == '\n') {
        buffer << "\n"; // Add newline
        commentLineCount++;
    }else if (file.eof()){
       return false;
    } else {
         buffer << " "; // Add whitespace
    }
    return state3(file, lineCount, commentLineCount, buffer);  // Otherwise, stay in state3, ignoring characters
}

// State 4: Looking for '/' to close the C-style block comment
bool CommentDFA::state4(std::ifstream& file, int& lineCount, int& commentLineCount, std::ostringstream& buffer) {
    char ch; 
    file.get(ch);  // Read the next character
    if (ch == '/') {  // If it's '/', the block comment is closed, return to state0
        buffer << "  "; // Add whitespace
        lineCount += commentLineCount; 
        state0(file, lineCount, buffer);
        return true;
    } else if (ch == '\n') {
        buffer << "\n"; // Add newline
        commentLineCount++;
    } else if (file.eof()){
       return false;
    } else if (ch == '*') {
        file.putback(ch);
    }
    buffer << " "; // Add whitespace
    return state3(file, lineCount, commentLineCount, buffer);  // Otherwise, continue checking inside the block comment
}

// State 5: Inside a quoted string (either single or double quotes), 
// ignore characters inside until the closing quote is found
bool CommentDFA::state5(std::ifstream& file, int& lineCount, int& quoteLineCount, std::ostringstream& buffer) {
    char ch; 
    file.get(ch);  
    if (file.eof()) { // If end of file is reached, return false
        return false;
    } else if (ch == '"') { // If a closing quote is found, return to state 0
        buffer << ch; // Output the character
        lineCount += quoteLineCount; // Increment the line counter
        state0(file, lineCount, buffer); // Go back to state 0
        return true; // And finally return true
    } else {  
        buffer << ch; // Otherwise, go to state 5
        return state5(file, lineCount, quoteLineCount, buffer);
    }
}

// State 6: Detected an asterisk ('*'),
// check if it's an unterminated C-style block comment
bool CommentDFA::state6(std::ifstream& file, int& lineCount, std::ostringstream& buffer) {
    char ch; 
    file.get(ch);  
    if (ch == '/') { // If a slash is found, return false
        return false;
    } else {  
        buffer << '*';
        file.putback(ch); // Otherwise, put the character back
        state0(file, lineCount, buffer); // Go to state 0
        return true; // And return true
    }
}