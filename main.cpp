#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <sstream>

#include "commentDFA.h"
#include "tokenizer.h"
#include "parser.h"
#include "table.h"
#include "tree.h"

#include "testFiles.h"

struct assignmentElements {
    int assignmentNum;
    const std::filesystem::path* testFiles;
    int numFiles;
    void (*processFunction)(std::ifstream&, std::ostringstream&, int);
};

// function to remove comments
void removeComments(std::ifstream& testFile, std::ostringstream& outputFile, int) {
    CommentDFA *removeComments = new CommentDFA();
    removeComments->begin(testFile, outputFile);
}

// function to remove comments then tokenize input
void tokenize(std::ifstream& testFile, std::ostringstream& outputFile, int) {
    CommentDFA *removeComments = new CommentDFA();
    std::ostringstream tempBuffer;
    removeComments->begin(testFile, tempBuffer);

    Tokenizer *tokenizer = new Tokenizer();
    std::istringstream tokenStream(tempBuffer.str());
    tokenizer->begin(tokenStream);
    std::vector<Token> tokenList = tokenizer->getTokens();
    std::cout << "Token List\n";

    for (int i = 0; i < tokenList.size(); i++){
        std::cout << "Token type: " << tokenList[i].getType() << '\n';
        std::cout << "Token: " << tokenList[i].getValue() << "\nLine Number: " << tokenList[i].getLineNumber() << '\n';
        std::cout << "\n";
    }
}

// function to remove comments, tokenize input, then put the input into a CST
void parse(std::ifstream& testFile, std::ostringstream& outputFile, int filenum) {
    CommentDFA *removeComments = new CommentDFA();
    std::ostringstream tempBuffer;
    removeComments->begin(testFile, tempBuffer);

    Tokenizer *tokenizer = new Tokenizer();
    std::istringstream tokenStream(tempBuffer.str());
    tokenizer->begin(tokenStream);
    std::vector<Token> tokenList = tokenizer->getTokens();

    Parser *parser = new Parser(tokenList);
    parser->begin();

    std::string outputFilename = "test_file_" + std::to_string(filenum + 1) + "_output.txt";
    std::ofstream rdpOutput(outputFilename);

    // Call the print function and pass in the ofstream
    parser->printTree(rdpOutput);
}

// function to remove comments, tokenize input, create a CST, and generate symbol tables
void symbolTable(std::ifstream& testFile, std::ostringstream& outputFile, int) {
    CommentDFA *removeComments = new CommentDFA();
    std::ostringstream tempBuffer;
    removeComments->begin(testFile, tempBuffer);

    Tokenizer *tokenizer = new Tokenizer();
    std::istringstream tokenStream(tempBuffer.str());
    tokenizer->begin(tokenStream);
    std::vector<Token> tokenList = tokenizer->getTokens();

    Parser *parser = new Parser(tokenList);
    parser->begin();

    Table *table = new Table;

    table->begin(parser->getHead());
    std::cout << "\nPrinting Symbol Table:\n" << std::endl;
    table->printTable();
    table->printParameters();
}

void abstractSyntaxTree(std::ifstream& testFile, std::ostringstream& outputFile, int filenum) {
    CommentDFA *removeComments = new CommentDFA();
    std::ostringstream tempBuffer;
    removeComments->begin(testFile, tempBuffer);

    Tokenizer *tokenizer = new Tokenizer();
    std::istringstream tokenStream(tempBuffer.str());
    tokenizer->begin(tokenStream);
    std::vector<Token> tokenList = tokenizer->getTokens();

    Parser *parser = new Parser(tokenList);
    parser->begin();

    Table *table = new Table;

    table->begin(parser->getHead());

    Tree* tree = new Tree(parser->getHead(), table);

}

const assignmentElements assignments[] = {
    {1, a1Tests, std::size(a1Tests), removeComments},
    {2, a2Tests, std::size(a2Tests), tokenize},
    {3, a3Tests, std::size(a3Tests), parse},
    {4, a4Tests, std::size(a4Tests), symbolTable},
    {5, a5Tests, std::size(a5Tests), abstractSyntaxTree}
};

// chooses test file based on user selection and includes error messages
std::ifstream openSelectedFile(const assignmentElements& config, int fileNum) {
    std::ifstream file(config.testFiles[fileNum]);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << config.testFiles[fileNum] << std::endl;
        exit(1);
    }

    return file;
}

// main function that handles user prompts, files opening and closing, and initial state
int main() {
    int assignmentNum;
    std::cout << "Select the assignment:\n"
              << "1 - Remove Comments\n"
              << "2 - Tokenize\n"
              << "3 - Parse\n"
              << "4 - Symbol Table\n"
              << "5 - Abstract Syntax Tree\n"
              << "Selection: ";
    std::cin >> assignmentNum;

    if (assignmentNum < 1 || assignmentNum > 5) {
        std::cerr << "Invalid assignment choice. Exiting.\n";
        return 1;
    }

    const assignmentElements& config = assignments[assignmentNum - 1];
    
    int fileNum;
    std::cout << "Choose a test file:\n";
    for (int i = 0; i < config.numFiles; ++i) {
        std::cout << "(" << i + 1 << ") " << config.testFiles[i] << "\n";
    }
    std::cin >> fileNum;
    --fileNum;

    std::ifstream file = openSelectedFile(config, fileNum);

    std::ostringstream buffer;

    config.processFunction(file, buffer, fileNum); // Pass in the file number

    std::cout << buffer.str();
    return 0;  
}


















// old main below

/*
// Main program function that handles user prompt, file opening/closing, and initial state
int main() {
    char ch;
    int assignment_num;
    int filenum;
    std::ifstream file;
    std::ostringstream buffer;

    // Prompt user for what programming assignment they want to test
    std::cout << "Which programming assignment would you like to test?\n\n"
         "(1) Programming Assignment 1: Ignore Comments\n"
         "(2) Programming Assignment 2: Tokenization\n"
         "(3) Programming Assignment 3: Recursive Descent Parser\n"
         "(4) Programming Assignment 4: Symbol Table\n"
         "Selection: ";
    std::cin >> assignment_num;

    std::cout << "Which file would you like to run the program on?\n\n";

    if (assignment_num == 1) {
        for (int i = 0; i < std::size(a1Tests); i++) {
            std::cout << "(" << i+1 << ")" << a1Tests[i] << std::endl;
        }
    } else if (assignment_num == 2) {
         for (int i = 0; i < std::size(a2Tests); i++) {
            std::cout << "(" << i+1 << ")" << a2Tests[i] << std::endl;
        }
    } else if (assignment_num == 3) {
         for (int i = 0; i < std::size(a3Tests); i++) {
            std::cout << "(" << i+1 << ")" << a3Tests[i] << std::endl;
        }
    } else if (assignment_num == 4) {
         for (int i = 0; i < std::size(a4Tests); i++) {
            std::cout << "(" << i+1 << ")" << a4Tests[i] << std::endl;
        }
    }
    std::cout << "Selection: ";
    std::cin >> filenum;

    filenum -= 1;

    if (assignment_num == 1) {
        file.open(a1Tests[filenum]);
        if (!file.is_open()) {  // Check if the file was opened successfully
            std::cerr << "Error: Could not open the file " << a1Tests[filenum] << std::endl;
            exit(1);
        }
    } else if (assignment_num == 2) {
        file.open(a2Tests[filenum]);
        if (!file.is_open()) {  // Check if the file was opened successfully
            std::cerr << "Error: Could not open the file " << a2Tests[filenum] << std::endl;
            exit(1);
         }
    }   else if (assignment_num == 3) {
        file.open(a3Tests[filenum]);
        if (!file.is_open()) {  // Check if the file was opened successfully
            std::cerr << "Error: Could not open the file " << a3Tests[filenum] << std::endl;
            exit(1);
         }
    } else if (assignment_num == 4) {
        file.open(a4Tests[filenum]);
        if (!file.is_open()) {  // Check if the file was opened successfully
            std::cerr << "Error: Could not open the file " << a4Tests[filenum] << std::endl;
            exit(1);
         }
    } else {
        std::cerr << "Inavlid file selection... Exiting...";
        exit(1);
    }
    CommentDFA *removeComments = new CommentDFA();
    // Start in state0 to process the file
    removeComments->begin(file, buffer);
    file.close();  // Close the file after processing

    if (assignment_num == 1) {
        std::cout  << "\nResulting File:" << std::endl;
        std::cout << buffer.str();
    }
    else if(assignment_num == 2) {
        std::string str = buffer.str();
        str.erase(str.find_last_not_of(" \n\r\t")+1);  // Remove trailing whitespace or newlines
        std::istringstream inputStream(str);

        Tokenizer *tokenizer = new Tokenizer();

        tokenizer->begin(inputStream);

        std::vector<Token> tokenList = tokenizer->getTokens();

        std::cout << "Token List\n";

        for (int i = 0; i < tokenList.size(); i++){
            std::cout << "Token type: " << tokenList[i].getType() << '\n';
            std::cout << "Token: " << tokenList[i].getValue() << "\nLine Number: " << tokenList[i].getLineNumber() << '\n';
            std::cout << "\n";
        }
    }
    else if(assignment_num == 3) {
         std::string str = buffer.str();
        str.erase(str.find_last_not_of(" \n\r\t")+1);  // Remove trailing whitespace or newlines
        std::istringstream inputStream(str);

        Tokenizer *tokenizer = new Tokenizer();

        tokenizer->begin(inputStream);

        std::vector<Token> tokenList = tokenizer->getTokens();

        Parser *parser = new Parser(tokenList);
        parser->begin();

        // Make an output filestream
        std::ofstream rdpOutput( "test_file_" + std::to_string(filenum + 1) + "_output.txt");

        // Call the print function and pass in the ofstream
        parser->printTree(rdpOutput);
    } else if(assignment_num == 4) {
        std::string str = buffer.str();
        str.erase(str.find_last_not_of(" \n\r\t")+1);  // Remove trailing whitespace or newlines
        std::istringstream inputStream(str);

        Tokenizer *tokenizer = new Tokenizer();

        tokenizer->begin(inputStream);

        std::vector<Token> tokenList = tokenizer->getTokens();

        Parser *parser = new Parser(tokenList);
        parser->begin();

        Table *table = new Table;

        table->begin(parser->getHead());
        std::cout << "\nPrinting Symbol Table:\n" << std::endl;
        table->printTable();
    }
    else {
        std::cout << "No matching assignments exist for " << assignment_num << ".\n";
        return 1;
    }

    return 0;  // Exit successfully
}
*/