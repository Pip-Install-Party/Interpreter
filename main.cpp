#include <fstream>
#include <string>
#include <filesystem>
#include <sstream>

#if defined(_WIN32) || defined(_WIN64)
    #include <iostream> // Use standard output stream on Windows
#else
    //#include <ncurses.h> // Use ncurses on Unix-based systems
    #include <iostream> // Use standard output stream on Windows
#endif

#include "Comments/commentDFA.h"
#include "Tokens/tokenizer.h"
#include "Parser/parser.h"
#include "Symbols/table.h"
#include "Tree/tree.h"
#include "Tests/testFiles.h"
#include "Interpreter/interpreter.h"


struct componentElements {
    int componentNum;
    const std::filesystem::path* testFiles;
    int numFiles;
    void (*processFunction)(std::ifstream&, std::ostringstream&, int);
};

int findMain(std::ifstream& file) {
    if (!file.is_open()) {
        std::cerr << "Error: File is not open." << std::endl;
        return -1; // Indicate error
    }

     // Reset file pointer to the beginning of the file
    file.clear();  // Clear any error flags
    file.seekg(0, std::ios::beg);  // Move the pointer to the beginning
    
    std::string line;
    int lineNumber = 0;
    while (std::getline(file, line)) {
        lineNumber++;
        if (line.find("procedure main") != std::string::npos) {
            return lineNumber;
        }
    }

    return -1; // Indicate not found
}

// function to remove comments
void removeComments(std::ifstream& testFile, std::ostringstream& outputFile, int) {
    std::string filename = "Output.txt";
    std::ofstream interpreterOutput(filename);
    
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
    std::string filename = "Output.txt";
    std::ofstream interpreterOutput(filename);
    
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
    std::string filename = "Output.txt";
    std::ofstream interpreterOutput(filename);
    
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
    std::string filename = "Output.txt";
    std::ofstream interpreterOutput(filename);
    
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
    interpreterOutput << "\nPrinting Symbol Table:\n\n";
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
        
    bool isInterpreter = false;
    Tree* tree = new Tree(parser->getHead(), table, isInterpreter);
}

void interpreter(std::ifstream& testFile, std::ostringstream& outputFile, int filenum) {
    
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


    bool isInterpreter = true;
    Tree* tree = new Tree(parser->getHead(), table, isInterpreter);

    int mainLine = findMain(testFile);

    Interpreter* interpreter = new Interpreter(table, tree, mainLine);

    interpreter->begin(tree->getHead());

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

void displayMessage(const std::string& message) {
#if defined(__APPLE__) || defined(__unix__)
    //clear();
    //printw(message.c_str());
   // refresh();
       std::cout << message;

#else
    std::cout << message;
#endif
}

void waitForInput() {
#if defined(__APPLE__) || defined(__unix__)
    //getch();  // ncurses-specific wait
        std::cin.get();  // For Windows, wait for input via std::cin
#else
    std::cin.get();  // For Windows, wait for input via std::cin
#endif
}

int getComponentSelection() {
    int selection = 0;
#if defined(__APPLE__) || defined(__unix__)
    // echo();
    // mvscanw(getcury(stdscr), getcurx(stdscr), (char*)"%d", &selection);
        std::cin >> selection;

#else
    std::cin >> selection;
#endif
    return selection;
}

int getFileSelection(int numFiles) {
    int fileNum = 0;
    displayMessage("Choose A Test File 1 - " + std::to_string(numFiles) + ",\nSelection: ");
    
#if defined(__APPLE__) || defined(__unix__)
    // mvscanw(getcury(stdscr), getcurx(stdscr), (char*)"%d", &fileNum);
        std::cin >> fileNum;
    --fileNum;
#else
    std::cin >> fileNum;
    --fileNum;
#endif
    return fileNum;
}

int main() {
    int componentNum = 0;

    // Initialize ncurses for Unix systems
#if defined(__APPLE__) || defined(__unix__)
    // initscr();               // Start ncurses mode
    // cbreak();                // Disable line buffering
    // noecho();                // Disable echoing of typed characters
    // keypad(stdscr, TRUE);    // Enable function keys like arrow keys
#endif

    // Common introduction message
    displayMessage("**********************************************************************************************\n"
                   "*                                                                                            *\n"
                   "*                                     C Interpreter                                          *\n"
                   "*                                                                                            *\n"
                   "*  A Project By Blake Marshall, Brandon Robinson, Holden Ea, Jacob Sellers, & Rolando Yax.   *\n"
                   "*  Completed To Satisfy The Final Project of CS460 At Sonoma State University In Fall 2024.  *\n"
                   "*                                                                                            *\n"
                   "*                                                                                            *\n"
                   "*  This Program Acts As An Interpreter For The C Language And Was Built Over The Course Of   *\n"
                   "*  Six Stages. Each Of These Components Are Testable And Can Be Selected Individially From   *\n"
                   "*                                  The Following Page.                                       *\n"
                   "*                                                                                            *\n"
                   "*                                                                                            *\n"
                   "*          Great things are done by a series of small things brought together.               *\n"
                   "*                                   -Vincent van Gogh                                        *\n"
                   "*                                                                                            *\n"
                   "**********************************************************************************************\n"
                   "\nPress any key to continue. . .");

    waitForInput(); // Wait for user input

    displayMessage("\nSelect A Component:\n"
                   "1 - Comment Removal\n"
                   "2 - Tokenization\n"
                   "3 - Parsing\n"
                   "4 - Symbol Table Generation\n"
                   "5 - Abstract Syntax Tree Generation\n"
                   "6 - Full Interpreter\n"
                   "Selection: ");

    componentNum = getComponentSelection();

    if (componentNum < 1 || componentNum > 6) {
        displayMessage("Invalid component selection. Exiting.\n");
        waitForInput();
#if defined(__APPLE__) || defined(__unix__)
      //  endwin();
#endif
        return 1;
    }

    // Assuming `components` is an array of structures with `numFiles` and `processFunction` as part of the struct.
    const componentElements& config = components[componentNum - 1];
    
    int fileNum = getFileSelection(config.numFiles);

    std::ifstream file = openSelectedFile(config, fileNum);
    if (!file.is_open()) {
        displayMessage("Error: Could not open selected file.\n");
        waitForInput();
#if defined(__APPLE__) || defined(__unix__)
      //  endwin();
#endif
        return 1;
    }

    std::ostringstream buffer;
    config.processFunction(file, buffer, fileNum); // Process based on selection

    // Display results
        //interpreterOutput.flush();
        //interpreterOutput.close();
        //displayMessage("Results printed to Interpreter_Output.txt\n\n"
                       //"Press any key to quit. . .");

    waitForInput();  // Wait for user to press a key before exiting

#if defined(__APPLE__) || defined(__unix__)
   //endwin(); // End ncurses mode
#endif

    return 0;
}