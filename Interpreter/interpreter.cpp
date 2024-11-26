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
    std::cout << "token value is " << curNode->getValue() << std::endl; // **** debug 

    if(curNode->getValue() == "DECLARATION"){       /*❌IN PROGRESS*/
        //return handleDeclaration(curNode);
    }
    else if(curNode->getValue() == "ASSIGNMENT"){   /*✅ might have an error in test case 2 double check */
        //return handleAssignment(curNode);
    }
    else if(curNode->getValue() == "WHILE"){    /*❌need to add a "handle body" to the handle while*/
        return handleWhile(curNode);
    }
    else if (curNode->getValue().find("FOR_EXPRESSION") != std::string::npos) {    /*❌IN PROG*/
      //return handleFor(curNode); Causing seg faulting.
      return;
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
    // else if(curNode->getValue() == "FUNCTION"){     /*❌*/ // maybe this is CALL instead of function
    //     return handleFunction(curNode);
    // }
    // else if(curNode->getValue() == "PROCEDURE"){    /*❌*/
    //     return handleProcedure(curNode);        
    // } 
    else if(curNode->getValue() == "IF"){         /*❌*/
        return handleIf(curNode);        
    } 
    else if(curNode->getValue() == "ELSE"){       /*❌*/
        return handleElse(curNode);        
    } 
    else if(curNode->getValue() == "WHILE"){        /*❌IN PROG*/
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

    if(currentEntry->getIDType() == "function"){
        std::vector <Entry*> parameters = currentEntry->getParameterList();

        Entry* paramList = getParamListForEntry(parameters, currentEntry->getIDName(), currentEntry->getScope());   //this function needs to return the parameter list for the specific entry

        std::cout << "Parameter list for " << currentEntry->getIDName() << std::endl;
        std::cout << paramList->getIDName() << std::endl;
        std::cout << paramList->getDType() << std::endl;
        std::cout << (paramList->getIsArray() ? "yes" : "no") << std::endl;
        std::cout << paramList->getArraySize() << std::endl;
        std::cout << paramList->getScope() << std::endl;
    }
    else if(currentEntry->getIDType() == "procedure"){
        // needs implementation
    }
    else if(currentEntry->getIDType() == "datatype"){ // I don't think this should ever be "datatye". The datatype is the 
        // needs implementation
    }

    // full implementation still needed

}

void Interpreter::handleAssignment(Node* node) {
    if (symbolTable.find(node->getValue()) != symbolTable.end()) {
        symbolTable.find(node->getValue())->second->setValue(evaluatePostfix(node->getSibling())); // we sure we should get sib BEFORE making the call??
        // Yes, because we are assigning it above. evaluatePostifx() is not compatible with passing the variable being assigned.
    }
}

void Interpreter::handleWhile(Node* node){
    bool result = evaluateBooleanPostfix(node);
    if(result) {
        //handle body
    } else {
        //skip to end block 
    }
}

void Interpreter::handleFor(Node* node) {
    // Get the three expressions
    Node* expr1 = node->getChild();  // initialization
    Node* expr2 = expr1->getChild();  // condition
    Node* expr3 = expr2->getChild();  // iteration

    // Evaluate the first expression (initialization)
    std::string result1 = evaluatePostfix(expr1);  // Evaluate the initialization expression
    std::cout << "value found = " << result1 << std::endl; // **** debug
    Node* targetVar1 = expr1->getChild();  // The target variable (leftmost child in expr1)
    if (targetVar1 != nullptr) {
        const std::string& varName = targetVar1->getValue();
        auto it = symbolTable.find(varName);
        if (it != symbolTable.end() && it->second != nullptr) {
            it->second->setValue(result1);  // Save the initialization result as a string
        } else {
            std::cerr << "Error: Variable '" << varName << "' not found in symbol table during initialization.\n";
        }
    }

    // Evaluate the second expression (condition)
    while (evaluateBooleanPostfix(expr2)) {
        // handle body function needed here ****
        std::cout << "Executing loop body...\n";

        // Evaluate the third expression (iteration)
        std::string result3 = evaluatePostfix(expr3);  // Evaluate the iteration expression
        Node* targetVar3 = expr3->getChild();  // The target variable (leftmost child in expr3)
        if (targetVar3 != nullptr) {
            const std::string& varName = targetVar3->getValue();
            auto it = symbolTable.find(varName);
            if (it != symbolTable.end() && it->second != nullptr) {
                it->second->setValue(result3);  // Save the iteration result as a string
            } else {
                std::cerr << "Error: Variable '" << varName << "' not found in symbol table during iteration.\n";
            }
        }
    }
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



// Helper functions 

bool Interpreter::isOperator(const std::string& token) {
    return token == "+" || token == "-" || token == "*" || token == "/" ||
           token == "&&" || token == "||" || token == "!" || token == "<" || 
           token == ">" || token == "<=" || token == ">=" || token == "==";
}


int Interpreter::performPostfixOperation(int a, int b, const std::string& op) {
    if (op == "+") return a + b;
    if (op == "-") return a - b;
    if (op == "*") return a * b;
    if (op == "/") {
        if (b == 0) throw std::runtime_error("Division by zero");
        return a / b;
    }
    throw std::invalid_argument("Invalid operator");
}

bool Interpreter::performBooleanOperation(bool a, bool b, const std::string& op) {
    if (op == "&&") return a && b;
    if (op == "||") return a || b;
    if (op == "!") return !a;
    if (op == "<=") return a <= b;
    if (op == ">=") return a >= b;
    if (op == "==") return a == b;

    throw std::invalid_argument("Invalid boolean operator");
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


Entry* Interpreter::getParamListForEntry(std::vector<Entry*> parameters, std::string entryName, int scope) {
        
        for(int i = 0; i < parameters.size(); i++) {
            Entry* curEntry = parameters.at(i);
            if(curEntry->getIDName() == entryName && curEntry->getScope() == scope){
                return curEntry;
            }   
        }
        std::cout << "Parameter list for " << entryName << " not found.";
        exit(3);
    };

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

bool Interpreter::evaluateBooleanPostfix(Node* node) {
    std::stack<bool> stack;
    Node* current = node->getSibling();

    while (current != nullptr) {
        const std::string& token = current->getValue();

        try {
            // Check if the token is a variable in the symbol table
            auto it = symbolTable.find(token);
            if (it != symbolTable.end()) {
                Entry* entry = it->second;
                if (entry != nullptr) {
                    stack.push(entry->getValue() == "1");  // Assuming "1" is true, "0" is false
                } else {
                    std::cerr << "Error: Null entry found for token: " << token << std::endl;
                }
            } 
            // Handle operators
            else if (isOperator(token)) {
                bool b = false;
                bool a = false;

                if (token == "!") {
                    // Unary operator: only one operand is needed
                    if (stack.size() < 1) {
                        throw std::runtime_error("Invalid boolean postfix expression: not enough operands for '!' operator");
                    }
                    a = stack.top();
                    stack.pop();
                    stack.push(!a);  // Apply negation for '!'
                } else {
                    // Binary operators: two operands are needed
                    if (stack.size() < 2) {
                        throw std::runtime_error("Invalid boolean postfix expression: not enough operands for operator");
                    }
                    b = stack.top(); stack.pop();
                    a = stack.top(); stack.pop();
                    stack.push(performBooleanOperation(a, b, token));  // Perform logical operation (&&, ||, etc.)
                }
            } else {
                // Attempt to convert token to a boolean (0 or 1)
                stack.push(token == "1");
            }
        } catch (const std::invalid_argument&) {
            throw std::runtime_error("Invalid token in expression: " + token);
        } catch (const std::out_of_range&) {
            throw std::runtime_error("Number out of range: " + token);
        }

        current = current->getSibling();
    }

    if (stack.size() != 1) {
        throw std::runtime_error("Invalid boolean postfix expression: stack size mismatch");
    }

    std::cout << "Got final boolean value for while: " << stack.top() << std::endl; // **** debug 

    return stack.top();  // Return the final boolean value 
}
