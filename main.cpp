#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <sstream>

#include "Comments/commentDFA.h"
#include "Tokens/tokenizer.h"
#include "Parser/parser.h"
#include "Symbols/table.h"
#include "Tree/tree.h"
#include "Tests/testFiles.h"
#include <ncurses.h>

std::string filename = "Interpreter_Output.txt";
std::ofstream interpreterOutput(filename);

struct componentElements {
    int componentNum;
    const std::filesystem::path* testFiles;
    int numFiles;
    void (*processFunction)(std::ifstream&, std::ostringstream&, int);
};

// function to remove comments
void removeComments(std::ifstream& testFile, std::ostringstream& outputFile, int) {
    CommentDFA *removeComments = new CommentDFA();
    std::ostringstream tempBuffer;
    removeComments->begin(testFile, tempBuffer);

    // Copy the buffer
    std::string str = tempBuffer.str();

    // Write the buffer to the output file
    interpreterOutput << str;
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
    interpreterOutput << "Token List\n";

    for (int i = 0; i < tokenList.size(); i++){
        interpreterOutput << "Token type: " << tokenList[i].getType() << '\n';
        interpreterOutput << "Token: " << tokenList[i].getValue() << "\nLine Number: " << tokenList[i].getLineNumber() << '\n';
        interpreterOutput << "\n";
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

    // Call the print function and pass in the ofstream
    parser->printTree(interpreterOutput);
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
    table->printTable(interpreterOutput);
    table->printParameters(interpreterOutput);
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

void interpreter(std::ifstream& testFile, std::ostringstream& outputFile, int filenum) {
    // CommentDFA *removeComments = new CommentDFA();
    // std::ostringstream tempBuffer;
    // removeComments->begin(testFile, tempBuffer);

    // Tokenizer *tokenizer = new Tokenizer();
    // std::istringstream tokenStream(tempBuffer.str());
    // tokenizer->begin(tokenStream);
    // std::vector<Token> tokenList = tokenizer->getTokens();

    // Parser *parser = new Parser(tokenList);
    // parser->begin();

    // Table *table = new Table;

    // table->begin(parser->getHead());

    // Tree* tree = new Tree(parser->getHead(), table);



    /*
        WAIT!!! Before uncommenting this, remove the check in main that causes the output

        "This component is not yet functional.".

        If this function has been implemented, we should print 

        "Results printed to Interpreter_Output.txt"

        regardless of component seleciton. 
    */



}

const componentElements components[] = {
    {1, c1Tests, std::size(c1Tests), removeComments},
    {2, c2Tests, std::size(c2Tests), tokenize},
    {3, c3Tests, std::size(c3Tests), parse},
    {4, c4Tests, std::size(c4Tests), symbolTable},
    {5, c5Tests, std::size(c5Tests), abstractSyntaxTree},
    {6, c6Tests, std::size(c6Tests), interpreter}
};

// chooses test file based on user selection and includes error messages
std::ifstream openSelectedFile(const componentElements& config, int fileNum) {
    std::ifstream file(config.testFiles[fileNum]);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << config.testFiles[fileNum] << std::endl;
        exit(1);
    }

    return file;
}

// main function that handles user prompts, files opening and closing, and initial state
int main() {
    // Initialize ncurses
    initscr();               // Start ncurses mode
    cbreak();                // Disable line buffering
    noecho();                // Disable echoing of typed characters
    keypad(stdscr, TRUE);    // Enable function keys like arrow keys

    int componentNum = 0;

    printw("**********************************************************************************************\n");
    printw("*                                                                                            *\n");
    printw("*                                     C Interpreter                                          *\n");
    printw("*                                                                                            *\n");
    printw("*  A Project By Blake Marshall, Brandon Robinson, Holden Ea, Jacob Sellers, & Rolando Yax.   *\n");
    printw("*  Completed To Satisfy The Final Project of CS460 At Sonoma State University In Fall 2024.  *\n");
    printw("*                                                                                            *\n");
    printw("*                                                                                            *\n");
    printw("*  This Program Acts As An Interpreter For The C Language And Was Built Over The Course Of   *\n");
    printw("*  Six Stages. Each Of These Components Are Testable And Can Be Selected Individially From   *\n");
    printw("*                                  The Following Page.                                       *\n");
    printw("*                                                                                            *\n");
    printw("*                                                                                            *\n");
    printw("*          Great things are done by a series of small things brought together.               *\n");
    printw("*                                   -Vincent van Gogh                                        *\n");
    printw("*                                                                                            *\n");
    printw("**********************************************************************************************\n");
    printw("\nPress any key to continue. . .");
    getch(); // Wait for user to press a key before continuing
    refresh();
    clear();

    printw("Select A Component:\n");
    printw("1 - Comment Removal\n");
    printw("2 - Tokenization\n");
    printw("3 - Parsing\n");
    printw("4 - Symbol Table Generation\n");
    printw("5 - Abstract Syntax Tree Generation\n");
    printw("6 - Full Interpreter\n");
    printw("Selection: ");
    refresh();

    // Get user input for component selection
    echo();
    mvscanw(getcury(stdscr), getcurx(stdscr), (char*)"%d", &componentNum);
    clear();
    if (componentNum < 1 || componentNum > 6) {
        printw("Invalid component selection. Exiting.\n");
        refresh();
        getch();  // Wait for a key press
        endwin(); // End ncurses mode
        return 1;
    }

    const componentElements& config = components[componentNum - 1];
    
    int fileNum = 0;
    printw("Choose A Test File 1 - %d,\n", config.numFiles);  // Just prints the file number
    printw("Selection: ");
    refresh();

    // Get user input for file selection
    mvscanw(getcury(stdscr), getcurx(stdscr), (char*)"%d", &fileNum);
    --fileNum;

    // Attempt to open selected file
    std::ifstream file = openSelectedFile(config, fileNum);
    if (!file.is_open()) {
        printw("Error: Could not open selected file.\n");
        refresh();
        getch();
        endwin();
        return 1;
    }

    std::ostringstream buffer;
    config.processFunction(file, buffer, fileNum); // Process based on selection
    interpreterOutput.flush();
    interpreterOutput.close();

    // Display the result buffer in ncurses window
    clear();
    if (componentNum != 6) {
        printw("Results printed to Interpreter_Output.txt");
    } else {
        printw("This component is not yet functional.");
    }
    printw("\n\nPress any key to quit. . .");
    refresh();
    
    getch(); // Wait for user to press a key before exiting
    endwin(); // End ncurses mode

    return 0;
}