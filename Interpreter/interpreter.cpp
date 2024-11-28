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

void Interpreter::setProgramCounter(Node* node){
    programCounter = node;
}

Node* Interpreter::nextNode(Node* node){
    while( node->getSibling() != nullptr ) {
        node = node->getSibling();
    }
    return node->getChild(); 
}

// processes a single AST node
void Interpreter::executeStatement(Node* curNode/*pass current AST node here*/){
    std::cout << "token value is a " << curNode->getValue() << std::endl; // **** debug 

    if(curNode->getValue() == "DECLARATION"){       /*❌IN PROGRESS*/
        return handleDeclaration(curNode);
    }
    else if(curNode->getValue() == "ASSIGNMENT"){   /*✅ might have an error in test case 2 double check */
        //return handleAssignment(curNode);
    }
    else if(curNode->getValue() == "WHILE"){    /*❌need to add a "handle body" to the handle while*/
        return handleWhile(curNode);
    }
    else if (curNode->getValue() == "FOR_EXPRESSION1") {    /*❌IN PROG*/
      return handleFor(curNode); 
    }
    else if (curNode->getValue() == "FOR_EXPRESSION2") {    /*❌IN PROG*/
      return; //skip, already handled above in for expr1
    }
    else if (curNode->getValue() == "FOR_EXPRESSION3") {    /*❌IN PROG*/
      return; //skip, already handled above in for expr1
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
    else if(curNode->getValue() == "IF"){         /*❌ needs to handle entire if else no matter which is actually ends up executing*/
        return handleIf(curNode); 
    } 
    else if(curNode->getValue() == "WHILE"){        /*❌IN PROG*/
        handleWhile(curNode);
        for (int i = 0; i < 2; i++) {
            while (curNode->getSibling() != nullptr) { // move passed expr2 and 3 
                curNode = curNode->getSibling();
            }
            curNode = curNode->getChild();
        }
        
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
    std::cout << "      DECLARATION name: " << currentEntry->getIDName() << std::endl;
    std::cout << "      " << currentEntry->getIDName() << " is a " << currentEntry->getIDType() << std::endl;

    if(currentEntry->getIDType() == "function"){
        std::vector <Entry*> parameters = currentEntry->getParameterList();

        Entry* paramList = getParamListForEntry(parameters, currentEntry->getIDName(), currentEntry->getScope());   //this function needs to return the parameter list for the specific entry

        std::cout << std::endl << "      parameter(s) for " << currentEntry->getIDName() << ": " << std::endl;
        std::cout << "          " << paramList->getDType() << " " << paramList->getIDName() << std::endl;
        std::cout << (paramList->getIsArray() ? "               is an array" : "            is not an array") << std::endl;
        std::cout << "            array size is : " << paramList->getArraySize() << std::endl;
        std::cout << "            scope is " << paramList->getScope() << std::endl;
        std::cout << std::endl;

        // logic needed to actually do something with this information
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
        symbolTable.find(node->getValue())->second->setValue(evaluatePostfix(node->getSibling())); 
    }
}

void Interpreter::handleWhile(Node* node) {
    Node* tempNode;
    loopStack.push(1);
    int stackCount = loopStack.size();
    while( evaluateBooleanPostfix(node) ) {
        tempNode = nextNode(node);
        while( !(tempNode->getValue() == "END_BLOCK" && loopStack.size() == stackCount)) {
            if (tempNode->getValue() == "END_BLOCK") {
                loopStack.pop();
            }
            if (loopStack.size() == stackCount) {
                executeStatement(tempNode);
            }
            tempNode = nextNode(tempNode); 
        }
    }
    if (loopStack.size() == stackCount) {
        setProgramCounter(tempNode);
    }
}

void Interpreter::handleFor(Node* node) {
    // Get the three expressions
    Node* temp = node;
    Node* expr1 = node->getSibling();  // Initialization (pointing to the variable to be initialized)
    while (temp->getSibling() != nullptr) {
        temp = temp->getSibling();
    }
    temp = temp->getChild();
    Node* expr2 = temp;  // Condition
    while (temp->getSibling() != nullptr) {
        temp = temp->getSibling();
    }
    temp = temp->getChild();
    Node* expr3 = temp->getSibling();  // Iteration (pointing to the variable to be initialized)
    
    // Evaluate the first expression (initialization) and set the value of the variable
    std::string result1 = evaluatePostfix(expr1);
    symbolTable.find(expr1->getValue())->second->setValue(result1);

    // Push the current loop to the stack to handle nested loops
    loopStack.push(1);  
    int stackCount = loopStack.size();
    // Loop on the second expression (condition)
    Node* tempNode; 
    while (evaluateBooleanPostfix(expr2)) {
        // Handle the body of the loop 
        tempNode = nextNode(expr3); 
        while( !(tempNode->getValue() == "END_BLOCK" && loopStack.size() == stackCount) ) {
            if (tempNode->getValue() == "END_BLOCK") {
                loopStack.pop();
            }
            if (loopStack.size() == stackCount) {
                executeStatement(tempNode);
            }
            tempNode = nextNode(tempNode); 
        }
        // Evaluate the third expression (iteration) to continue loop
        symbolTable.find(expr3->getValue())->second->setValue(evaluatePostfix(expr3));
    }
    if (loopStack.size() == stackCount) {
        setProgramCounter(tempNode);
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

void Interpreter::handleIf(Node* node){
    //new attempt at this function V
    Node* ifNode = node;
    if (evaluateBooleanPostfix(ifNode)) {
        setProgramCounter( skipBlock( executeBlock(nextNode(node)) ) );
    } 
    else { // skip the if logic and find the next 
        setProgramCounter( skipBlock(node) );
    }
    //new attempt at this function ^
    
    //First attempt at this function V
    //Node* ifNode = node;
    // node = nextNode(node);
    // loopStack.push(1);
    // int stackCount = loopStack.size();
    // while (!(node->getValue() == "END_BLOCK" && loopStack.size() == stackCount)) {
    //     if (node->getValue() == "END_BLOCK") {
    //         loopStack.pop();
    //     }
    //     if (evaluateBooleanPostfix(ifNode) && loopStack.size() == stackCount) {
    //         executeStatement(node);
    //     } 
    //     node = nextNode(node);
    // }
    // if (loopStack.size() == stackCount) {
    //     setProgramCounter(node);
    // }
}

void Interpreter::handleElse(Node* node){
    Node* elseNode = node;
    node = nextNode(node);
    loopStack.push(1);
    int stackCount = loopStack.size();
    while (!(node->getValue() == "END_BLOCK" && loopStack.size() == stackCount)) {
        if (node->getValue() == "END_BLOCK") {
            loopStack.pop();
        }
        if (evaluateBooleanPostfix(elseNode) && loopStack.size() == stackCount) {
            executeStatement(node);
        } 
        node = nextNode(node);
    }
    if (loopStack.size() == stackCount) {
        setProgramCounter(node);
    }
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

bool Interpreter::performBooleanOperation(int a, int b, const std::string& op) {
    if (op == "&&") return a && b;
    if (op == "||") return a || b;
    if (op == "!") return !a;
    if (op == "<=") return a <= b;
    if (op == ">=") return a >= b;
    if (op == "<") return a < b;
    if (op == ">") return a > b;
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

            // for parameter Entries, ID type stores the function or procedure that holds the parameters
            std::cout << "      looking up parameter list for " << curEntry->getIDType() << " in symbol table..."<< std::endl;
            if(curEntry->getIDType() == entryName && curEntry->getScope() == scope){
                return curEntry;
            }   
        }
        std::cout << "      parameter list for " << entryName << " not found, exiting";
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
        //std::cout << "Token: " << current->getValue() << std::endl; //This will print the current symbol being processed ****
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
    std::stack<int> stack;  // Use int stack to align with treating booleans as integers (0 or 1)
    Node* current = node->getSibling();

    while (current != nullptr) {
        const std::string& token = current->getValue();

        try {
            // Check if the token is a variable in the symbol table
            auto it = symbolTable.find(token);
            if (it != symbolTable.end()) {
                Entry* entry = it->second;
                if (entry != nullptr) {
                    int value = std::stoi(entry->getValue());
                    stack.push(value);
                } else {
                    std::cerr << "Error: Null entry found for token: " << token << std::endl;
                }
            }
            // Handle operators
            else if (isOperator(token)) {
                int a = -1;
                int b = -1;

                if (token == "!") {
                    // Unary operator: only one operand is needed
                    if (stack.size() < 1) {
                        throw std::runtime_error("Invalid boolean postfix expression: not enough operands for '!' operator");
                    }
                    a = stack.top();
                    stack.pop();
                    std::cout << "Applying '!': operand = " << a << std::endl;
                    stack.push(!a);  // Apply negation for '!'
                } else {
                    // Binary operators: two operands are needed
                    if (stack.size() < 2) {
                        throw std::runtime_error("Invalid boolean postfix expression: not enough operands for operator");
                    }
                    b = stack.top(); stack.pop();
                    a = stack.top(); stack.pop();
                    int result = performBooleanOperation(a, b, token);
                    stack.push(result);
                }
            } else {
                // Attempt to convert token to an integer
                int value = std::stoi(token);
                stack.push(value);
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

    return stack.top();  // Return the final boolean value 
}

Node* Interpreter::skipBlock(Node* node){
    std::stack<int> scopeStack;
    scopeStack.push(1);
    Node* tempNode = nextNode(node);

    while (scopeStack.size() != 0) {
        if ( node->getValue() == "BEGINBLOCK" ){
            scopeStack.push(1);
        } else if ( node->getValue() == "ENDBLOCK" ) {
            scopeStack.pop();
        }
        Node* tempNode = nextNode(tempNode);
    }
    return tempNode;
}

Node* Interpreter::executeBlock(Node* node){
    std::stack<int> scopeStack;
    scopeStack.push(1);
    Node* tempNode = nextNode(node);

    while (scopeStack.size() != 0) {
        if ( node->getValue() == "BEGINBLOCK" ){
            scopeStack.push(1);
        } else if ( node->getValue() == "ENDBLOCK" ) {
            scopeStack.pop();
        } else {
            executeBlock(tempNode);
        }
        Node* tempNode = nextNode(tempNode);
    }
    return tempNode;
}
