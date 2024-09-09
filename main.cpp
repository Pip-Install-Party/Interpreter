#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// Function prototypes for the different DFA states
void state0(std::ifstream& file, int& lineCount);  // Normal state, reading code
void state1(std::ifstream& file, int& lineCount);  // Detected '/', checking if it's a comment
void state2(std::ifstream& file, int& lineCount);  // Inside a C++-style comment, ignore until newline
bool state3(std::ifstream& file, int& lineCount, int& commentLineCount);  // Inside a C-style block comment
bool state4(std::ifstream& file, int& lineCount, int& commentLineCount);  // Checking for end of C-style block comment
bool state5(std::ifstream& file, int& lineCount, int& quoteLinecount);  // Inside a C++-style comment, ignore until newline
// Vector holding the file paths for test files
const std::vector<std::string> tests = {
   "Tests/programming_assignment_1-test_file_1.c",
   "Tests/programming_assignment_1-test_file_2.c",
   "Tests/programming_assignment_1-test_file_3.c",
   "Tests/programming_assignment_1-test_file_4.c",
   "Tests/programming_assignment_1-test_file_5.c",
   "Tests/programming_assignment_1-test_file_6.c"
};

// State 0: Normal state, reading and printing code character by character
void state0(std::ifstream& file, int& lineCount) {
    char ch; 
    file.get(ch);  // Read a character from the file
    if (file.eof()) {  // Check if the end of file is reached
        return;  // End of input, stop the function
    } else if (ch == '/') {  // If the character is '/', move to state1
        return state1(file, lineCount);
    } else if (ch == '"') {
        int quoteLineCount = 0;
        std::cout << ch;
        if (!state5(file, lineCount, quoteLineCount)) {
            std::cout << "ERROR: Program contains C-style, unterminated quote on line " << lineCount;
            exit(1);
        }
        return; 
    } else {  // Otherwise, print the character and stay in state0
        if (ch == '\n') {
            lineCount++;
        }
        std::cout << ch;
        state0(file, lineCount);
    }
}

// State 1: After hitting a '/', figure out if it's a comment or a division operator
void state1(std::ifstream& file, int& lineCount) {
    char ch; 
    file.get(ch);  // Read the next character
    if (file.eof()) {  // Check for end of file
        return; 
    } else if (ch == '/') {  // If it's another '/', we have a C++-style comment, move to state2
        return state2(file, lineCount);
    } else if (ch == '*') {  // If it's a '*', we have a C-style block comment, move to state3
        int commentLineCount = 0;
        if (!state3(file, lineCount, commentLineCount)) {
            std::cout << "ERROR: Program contains C-style, unterminated comment on line " << lineCount;
            exit(0);
        }
       return; 
    } else {  // Otherwise, it's just a '/', print it and return to state0
        if (ch == '\n') {
            lineCount++;
        }
        std::cout << '/';  // Don't forget to print the first '/' since it's not part of a comment
        std::cout << ch;   // Also print the current character
        return state0(file, lineCount);
    }
}

// State 2: Inside a C++-style line comment, skip characters until newline is reached
void state2(std::ifstream& file, int& lineCount) {
    char ch; 
    file.get(ch);  // Read the next character
    if (ch == '\n') {  // If it's a newline, return to state0 to process the next line
        lineCount++;
        return state0(file, lineCount);
    }
    return state2(file, lineCount);  // Otherwise, stay in state2, ignoring characters
}

// State 3: Inside a C-style block comment, look for the closing '*/'
bool state3(std::ifstream& file, int& lineCount, int& commentLineCount) {
    char ch; 
    file.get(ch);  // Read the next character
    if (ch == '*') {  // If it's a '*', check if it's the start of '*/' (end of comment)
        return state4(file, lineCount, commentLineCount);
    } else if (ch == '\n') {
        commentLineCount++;
    }else if (file.eof()){
       return false;
    }
    return state3(file, lineCount, commentLineCount);  // Otherwise, stay in state3, ignoring characters
}

// State 4: Looking for '/' to close the C-style block comment
bool state4(std::ifstream& file, int& lineCount, int& commentLineCount) {
    char ch; 
    file.get(ch);  // Read the next character
    if (ch == '/') {  // If it's '/', the block comment is closed, return to state0
        lineCount += commentLineCount; 
        state0(file, lineCount);
        return true;
    } else if (ch == '\n') {
        commentLineCount++;
    } else if (file.eof()){
       return false;
    } else if (ch == '*') {
        file.putback(ch);
    }
    return state3(file, lineCount, commentLineCount);  // Otherwise, continue checking inside the block comment
}

bool state5(std::ifstream& file, int& lineCount, int& quoteLineCount) {
    char ch; 
    file.get(ch);  
    if (file.eof()) { 
        return false;
    } else if (ch == '"') { 
        std::cout << ch;
        lineCount += quoteLineCount;
        state0(file, lineCount);
        return true;
    } else {  
        std::cout << ch;
        return state5(file, lineCount, quoteLineCount);
    }
}

int main() {
    char ch;
    int filenum;
    int lineCount = 1;

    // Prompt user for what file they want to remove comments
    std::cout << "Which file would you like to remove comments from?\n\n"
		"Type 1 for programming_assignment_1-test_file_1.c\n"
   		"Type 2 for programming_assignment_1-test_file_2.c\n"
   		"Type 3 for programming_assignment_1-test_file_3.c\n"
   		"Type 4 for programming_assignment_1-test_file_4.c\n"
   		"Type 5 for programming_assignment_1-test_file_5.c\n"
   		"Type 6 for programming_assignment_1-test_file_6.c\n";
    std::cin >> filenum;
	filenum -= 1;


    // Open the first test file from the vector
    std::ifstream file(tests[filenum]);
    
    if (!file.is_open()) {  // Check if the file was opened successfully
        std::cerr << "Error: Could not open the file " << tests[filenum] << std::endl;
        return 1;  // Exit with an error code
    }
    // Start in state0 to process the file
    state0(file, lineCount);
    file.close();  // Close the file after processing

    return 0;  // Exit successfully
}
