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

#include "lib/ftxui/include/ftxui/component/screen_interactive.hpp" // Add this header for ScreenInteractive
#include "lib/ftxui/include/ftxui/component/component.hpp"         // For Menu and other components
#include "lib/ftxui/include/ftxui/screen/screen.hpp"
#include "lib/ftxui/include/ftxui/dom/elements.hpp"
#include "lib/ftxui/include/ftxui/component/component.hpp"

using namespace ftxui;

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
    auto screen = ScreenInteractive::TerminalOutput();

    std::vector<std::string> menu_options = {"Run Interpreter", "Test Components"};
    int selected_option = 0;

    // Create the informational content to display
    auto content = Renderer([] {
        return vbox({
            text("C Interpreter") | center,
            separator(),
            text("Created for CS460 (Programming Languages) at Sonoma State University in fall 2024.") | center,
            separator(),
            text("Press Q to quit at any time.") | center,
        }) | vcenter;
    });

    // Create the menu for selection options
    auto menu = Menu(&menu_options, &selected_option);

    // Create the sub-menu for test components
    std::vector<std::string> test_components_options = {
        "Remove Comments",
        "Tokenize",
        "Parse",
        "Symbol Table",
        "Abstract Syntax Tree"
    };
    int selected_test_component = 0;

    auto test_components_menu = Menu(&test_components_options, &selected_test_component);

    // Combine content and menu
    auto main_layout = Renderer([&] {
        return vbox({
            content->Render(),
            separator(),
            text("Select an option:") | center,
            menu->Render() | center,  // Center the menu within the vbox
        }) | vcenter | center;
    });

    auto test_components_layout = Renderer([&] {
        return vbox({
            text("Test Components") | center,
            separator(),
            test_components_menu->Render() | center,  // Center the sub-menu within the vbox
        }) | vcenter | center;
    });

    // Wrap the main layout with a quit listener for 'Q' key
    auto quit_listener = CatchEvent(main_layout, [&](Event event) {
        if (event == Event::Character('q') || event == Event::Character('Q')) {
            screen.ExitLoopClosure()();
            return true;  // Event handled
        }
        if (event == Event::Character('w')) { // Move up
            selected_option = (selected_option - 1 + menu_options.size()) % menu_options.size();
            return true;
        }
        if (event == Event::Character('s')) { // Move down
            selected_option = (selected_option + 1) % menu_options.size();
            return true;
        }
        if (event == Event::Return) {
            if (selected_option == 0) {
                // Handle "Run Interpreter"
                std::cout << "Running Interpreter..." << std::endl;
            } else if (selected_option == 1) {
                // Handle "Test Components" -> display test components menu
                screen.Loop(test_components_layout);  // Transition to the test components menu
            }
            return true;
        }
        return false;  // Event not handled
    });

    // Handle events for Test Components sub-menu
    auto test_components_listener = CatchEvent(test_components_layout, [&](Event event) {
        if (event == Event::Character('q') || event == Event::Character('Q')) {
            screen.ExitLoopClosure()();
            return true;  // Exit on Q
        }

        if (event == Event::Character('w')) { // Move up
            selected_test_component = (selected_test_component - 1 + test_components_options.size()) % test_components_options.size();
            return true;
        }
        if (event == Event::Character('s')) { // Move down
            selected_test_component = (selected_test_component + 1) % test_components_options.size();
            return true;
        }
        if (event == Event::Return) {
            // Handle the selected option
            if (selected_test_component == 0) {
                std::cout << "Removing Comments..." << std::endl;
            } else if (selected_test_component == 1) {
                std::cout << "Tokenizing..." << std::endl;
            } else if (selected_test_component == 2) {
                std::cout << "Parsing..." << std::endl;
            } else if (selected_test_component == 3) {
                std::cout << "Displaying Symbol Table..." << std::endl;
            } else if (selected_test_component == 4) {
                std::cout << "Displaying Abstract Syntax Tree..." << std::endl;
            }
            return true;
        }
        return false;  // Event not handled
    });

    // Render the main menu initially
    screen.Loop(quit_listener);  // Initially load the main menu

    return 0;
}

 /*
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
    */