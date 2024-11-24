#include "interpreter.h"
#include <iostream>

std::string file = "Interpreter_Output.txt";
std::ofstream output(file);

void Interpreter::begin(Node* node /*pass AST head here*/){
    
    programCounter = node /*AST head*/;

    while(programCounter != nullptr){
        executeStatement(programCounter);   // pass to executeStatement for further processing
        programCounter = nextStatement(); // need logic to get next statement
    } 
    std::cout << "Execution completed. Results may not be complete as this component is not fully functional." << std::endl;
}

Node* Interpreter::nextStatement(){
    while( programCounter->getSibling() != nullptr ) {
        programCounter = programCounter->getSibling();
    }
    return programCounter->getChild();
}

// processes a single AST node
void Interpreter::executeStatement(Node* curNode/*pass current AST node here*/){
    
    if(curNode->getValue() == "DECLARATION"){       /*❌IN PROGRESS*/
        return handleDeclaration(curNode);
    }
    else if(curNode->getValue() == "ASSIGNMENT"){   /*✅*/
        return handleAssignment(curNode);
    }
    else if(curNode->getValue() == "ITERATION"){    /*❌*/
        return handleIteration(curNode);
    }
    else if(curNode->getValue() == "SELECTION"){    /*❌*/
        return handleSelection(curNode);
    }
    else if(curNode->getValue() == "PRINTF"){       /*✅*/
        return handlePrintf(curNode);
    }
    else if(curNode->getValue() == "RETURN"){       /*❌*/
        return handleReturn(curNode);
    }
    else if(curNode->getValue() == "FUNCTION"){     /*❌*/ // maybe this is CALL instead of function
        return handleFunction(curNode);
    }
    else if(curNode->getValue() == "PROCEDURE"){    /*❌*/
        return handleProcedure(curNode);        
    } 
    else if(curNode->getValue() == "IF"){         /*❌*/
        return handleIf(curNode);        
    } 
    else if(curNode->getValue() == "ELSE"){       /*❌*/
        return handleElse(curNode);        
    } 
    else if(curNode->getValue() == "WHILE"){        /*❌*/
        return handleWhile(curNode);
    } 
    else if(curNode->getValue() == "BEGIN_BLOCK"){ /*❌*/
        return;    
    } 
    else if(curNode->getValue() == "END_BLOCK"){  /*❌*/
        // Do we need to do anything considering scope is changing?
        // No, since we preserve the order of the symbol table using the strings we can just omit this or keep the return probably 
        return;        
    }
    /* ... add other types here */
    else {
        std::cerr << "Statement type not recognized: " << curNode->getValue() << std::endl;
        exit(3);
    }
}

// Gets the symbol table entry associated with the current DECLARATION from the AST
void Interpreter::handleDeclaration(Node* node){
    Entry* currentEntry = getEntryByIndex(curTableIndex, insertOrder);    // get the entry from the symbol table corresponding to the iterator
    curTableIndex++;
    std::cout << "Found declaration for " << currentEntry->getIDName() << std::endl;

    // full implementation still needed

}

void Interpreter::handleAssignment(Node* node /* pass current AST node here */) {
   evaluatePostfix(node->getSibling());
}

void Interpreter::handleIteration(Node* node/*pass current AST node here*/){
}

void Interpreter::handleSelection(Node* node/*pass current AST node here*/){
}

void Interpreter::handlePrintf(Node* node/*pass current AST node here*/){
    Node* curNode = node->getSibling();

    while ( curNode != nullptr ) {
        if( symbolTable.find(curNode->getValue()) != symbolTable.end()) {
            output << symbolTable.at(curNode->getValue())->getValue();
        } else {
            output << curNode->getValue();
        }
        curNode = curNode->getSibling();
   }
}

void Interpreter::handleReturn(Node* node/*pass current AST node here*/){
}

void Interpreter::handleFunction(Node* node/*pass current AST node here*/){

}

void Interpreter::handleProcedure(Node* node/*pass current AST node here*/){
}

void Interpreter::handleIf(Node* node/*pass current AST node here*/){
}

void Interpreter::handleElse(Node* node/*pass current AST node here*/){
}

void Interpreter::handleWhile(Node* node/*pass current AST node here*/){
}

// Helper functions 

bool Interpreter::isOperator(const std::string& token) {
    return token == "+" || token == "-" || token == "*" || token == "/";
}

int Interpreter::performPostfixOperation(int a, int b, const std::string& op) {
    if (op == "+") return a + b;
    if (op == "-") return a - b;
    if (op == "*") return a * b;
    if (op == "/") return a / b;
    throw std::invalid_argument("Invalid operator");
}


// Function called in the interpreter.h to initialize symbolTable
// IMPORTANT, unordered maps are hashed, so their order does not correspond to insertion order 
std::unordered_map<std::string, Entry*> Interpreter::convertTable(Table* table) {
    
    std::unordered_map<std::string, Entry*> symbolMap;

    Entry* head = table->getHead();

    while( head != nullptr) {
        symbolMap.emplace(head->getIDName(), head);

        // pushes symbol table entry's identifier's name into insertOrder to preserve CORRECT symbol table order
        insertOrder.push_back(head->getIDName());   

        head = head->getNext();
    }

    return symbolMap;
}

// Function that returns an entry corresponding to the current symbol table index
Entry* Interpreter::getEntryByIndex(int curTableIndex, std::vector<std::string>& insertOrder) {
    if (curTableIndex < 0 || curTableIndex >= insertOrder.size()) {
        std::cout << "Current Symbol Table index is out of bounds." << std::endl;
        exit(3);
    }

    // uses the vector of strings of identifier names to return correct identifier name
    std::string identifierName = insertOrder[curTableIndex];

    // identifier name then used to look up Entry in UNORDERED map
    auto iterator = symbolTable.find(identifierName);

    if (iterator != symbolTable.end()) {
        return iterator->second;    // returns second part of the map, which is the symbol table Entry
    }
    else {
        std::cout << "Entry not found with identifier name " << identifierName << " and index " << curTableIndex << "." << std::endl;
        exit(3);
    }
}

std::string Interpreter::evaluatePostfix(Node* node) {
    std::stack<int> stack;
    Node* current = node->getSibling();

    // This segment will print the expression being evaluated
    // auto temp = current;
    // while (temp != nullptr) {
    //     std::cout << temp->getValue() << " ";
    //     temp = temp->getSibling();
    // }
    // std::cout << std::endl;

    while (current != nullptr) {
        // std::cout << "Token: " << current->getValue() << std::endl; This will print the current symbol being processed
        const std::string& token = current->getValue();

        try {
            // Check if the token is a variable in the symbol table
            auto it = symbolTable.find(token);
            if (it != symbolTable.end()) {
                Entry* entry = it->second;
                if (entry != nullptr) {
                    auto idType = entry->getIDType();
                    if (idType == "procedure" || idType == "function") { // This if block will need to be adapted since there is a CALL
                        std::cout << "Skipping assignment since CALL not yet implemented" << std::endl;
                        return "0";
                    }

                    stack.push(std::stoi(entry->getValue()));
                } else {
                    std::cerr << "Error: Null entry found for token: " << token << std::endl;
                }
            } else if (isOperator(token)) {
                // Handle operator
                if (stack.size() < 2) {
                    throw std::runtime_error("Invalid postfix expression: not enough operands for operator");
                }
                int b = stack.top(); stack.pop();
                int a = stack.top(); stack.pop();
                stack.push(performPostfixOperation(a, b, token));
            } else if (token == "=") {
                // Assignment operator (end of expression)
                if (stack.size() != 1) {
                    throw std::runtime_error("Invalid postfix expression: stack size mismatch at '='");
                }
                return std::to_string(stack.top());
            } else {
                // Attempt to convert token to a number
                stack.push(std::stoi(token));
            }
        } catch (const std::invalid_argument&) {
            throw std::runtime_error("Invalid token in expression: " + token);
        } catch (const std::out_of_range&) {
            throw std::runtime_error("Number out of range: " + token);
        }

        current = current->getSibling();
    }

    throw std::runtime_error("Invalid postfix expression: missing '='");
}
