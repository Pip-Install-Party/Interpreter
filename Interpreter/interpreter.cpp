#include "interpreter.h"
#include <iostream>

std::string file = "Interpreter_Output.txt";
std::ofstream output(file);

void Interpreter::begin(Node* node /*pass AST head here*/){
    programCounter = node /*AST head*/;

    std::cout << "This component is not yet functional.";

    while(programCounter != nullptr){
        executeStatement(programCounter);   // pass to executeStatement for further processing
        programCounter = nextStatement(); // need logic to get next statement
    } 

}

Node* Interpreter::nextStatement(){
    // implement logic to find the next statement here
    // this should traverse right and then down until another declaration, assignment, iteration, etc. is found
    // return next statement node

    while( programCounter->getSibling() != nullptr ) {
        programCounter = programCounter->getSibling();
    }
    return programCounter->getChild();
}

// processes a single AST node
void Interpreter::executeStatement(Node* curNode/*pass current AST node here*/){
    if(curNode->getValue() == "DECLARATION"){       /*❌*/
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
    else if(curNode->getValue() == "PRINTF"){       /*❌*/
        return handlePrintf(curNode);
    }
    else if(curNode->getValue() == "RETURN"){       /*❌*/
        return handleReturn(curNode);
    }
    else if(curNode->getValue() == "FUNCTION"){     /*❌*/
        return handleFunction(curNode);
    }
    else if(curNode->getValue() == "PROCEDURE"){    /*❌*/
        return handleProcedure(curNode);        
    } else if(curNode->getValue() == "IF"){         /*❌*/
        return handleProcedure(curNode);        
    } else if(curNode->getValue() == "ELSE"){       /*❌*/
        return handleProcedure(curNode);        
    }else if(curNode->getValue() == "BEGIN_BLOCK"){ /*❌*/
        // Do we need to do anything considering scope is changing? 
        return;    
    } else if(curNode->getValue() == "END_BLOCK"){  /*❌*/
        // Do we need to do anything considering scope is changing? 
        return;        
    }
    /* ... add other types here */
    else{
        std::cerr << "Statement type not recognized: " << curNode->getValue() << std::endl;
        exit(3);
    }
}

void Interpreter::handleDeclaration(Node* node/*pass current AST node here*/){

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

std::unordered_map<std::string, Entry*> Interpreter::convertTable(Table* table) {
    std::unordered_map<std::string, Entry*> symbolMap;
    Entry* head = table->getHead();
    while( head != nullptr) {
        symbolMap.emplace(head->getIDName(), head);
        head = head->getNext();
    }
    return symbolMap;
}





std::string Interpreter::evaluatePostfix(Node* node) {
     std::stack<int> stack;
    Node* current = node->getSibling();

    auto temp = current;

    while(temp != nullptr) {
        std::cout << temp->getValue() << " ";
        temp = temp->getSibling();
    }
    std::cout << std::endl;
    while (current != nullptr) {
        const std::string& token = current->getValue();

        // This if block will be removed once was can CALL
         if (symbolTable.find(token) != symbolTable.end()) {
               if (symbolTable.find(token)->second->getIDType() == "procedure" || symbolTable.find(token)->second->getIDType() == "function") {
                    std::cout << "Skipping assignment since Call not yet implemented";
                    return 0; 
               }
            }

        try {
            // Check if the token is a variable in the symbol table
            if (symbolTable.find(token) != symbolTable.end()) {
                std::cout << symbolTable.at(token)->getValue() << std::endl;
                stack.push(std::stoi(symbolTable.at(token)->getValue()));
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