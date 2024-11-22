#include "interpreter.h"
#include <iostream>

void Interpreter::begin(Node* node /*pass AST head here*/){
    programCounter = node /*AST head*/;

    while(programCounter != nullptr){
        executeStatement(programCounter);   // pass to executeStatement for further processing
        programCounter = nextStatement(); // need logic to get next statement
    } 
    std::cout << "This component is not yet functional.";
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
void Interpreter::handleAssignment(Node* node/*pass current AST node here*/){
    std::stack<int> stack;
    Node* symbol = node->getSibling();
    Node* current = node;
    while (current != nullptr) {
        const std::string& token = current->getValue();

        if (!token.empty() && std::isdigit(token[0])) {
            // If the token is a number, push it onto the stack
            stack.push(std::stoi(token));
        } else if (isOperator(token)) {
            // If the token is an operator, pop two elements and apply the operator
            if (stack.size() < 2) {
                throw std::runtime_error("Invalid postfix expression");
            }
            int b = stack.top(); stack.pop();
            int a = stack.top(); stack.pop();
            int result = performPostfixOperation(a, b, token);
            stack.push(result);
        } else {
            throw std::invalid_argument("Invalid token in expression");
        }

        current = current->getSibling();
    }

    if (stack.size() != 1) {
        throw std::runtime_error("Invalid postfix expression");
    }
    symbolTable.at(symbol->getValue()).setValue(std::to_string(stack.top())); 
}

void Interpreter::handleIteration(Node* node/*pass current AST node here*/){
}

void Interpreter::handleSelection(Node* node/*pass current AST node here*/){
}

void Interpreter::handlePrintf(Node* node/*pass current AST node here*/){
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

std::unordered_map<std::string, Entry> Interpreter::convertTable(Table* table) {
    std::unordered_map<std::string, Entry> symbolMap;
    Entry* head = table->getHead();
    while( head != nullptr) {
        symbolMap.emplace(head->getIDName(), head);
        head = head->getNext();
    }
    return symbolMap;
}
