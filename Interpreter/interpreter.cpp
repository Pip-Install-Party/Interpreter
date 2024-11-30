#include "interpreter.h"
#include <iostream>

std::string file = "Interpreter_Output.txt";
std::ofstream output(file);

void Interpreter::begin(Node* node /*pass AST head here*/){

    std::cout << "Main is on line: " << mainline << std::endl;
    short num = numFunctions(rawTable);
    std::cout << "Num functions: " << num << std::endl;

    while (node != nullptr) {
        if (node->getValue() == "DECLARATION") {
            if (nextNode(node)->getValue() == "BEGIN_BLOCK") {
                functionMap.emplace(functionVector.at(functionVector.size() - num), node);
                num--;
            }
        }

        if (num == 0) {
            programCounter = node;
            break;
        }

        node = nextNode(node);
    }
    

    while(programCounter != nullptr){
        executeStatement(programCounter); // pass to executeStatement for further processing
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
        return handleAssignment(curNode);
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
        handleReturn(curNode);
        return;
    } else if(curNode->getValue() == "CALL"){       /*❌*/
        //executeCall(curNode);
        return;
    } 
    // else if(curNode->getValue() == "FUNCTION"){     /*❌*/ // maybe this is CALL instead of function
    //     return handleFunction(curNode);
    // }
    // else if(curNode->getValue() == "PROCEDURE"){    /*❌*/
    //     return handleProcedure(curNode);        
    // } 
    else if(curNode->getValue() == "IF"){         /*❌ needs to handle entire if else no matter which is actually ends up executing*/
        return handleIf(curNode); 
    } else if(curNode->getValue() == "ELSE"){         /*❌ needs to handle entire if else no matter which is actually ends up executing*/
        std::cout << "Calling else";
        return handleElse(curNode); 
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
    // Entry* currentEntry = getEntryByIndex(curTableIndex, insertOrder);    // get the entry from the symbol table corresponding to the iterator
    // curTableIndex++;
    // std::cout << "      DECLARATION name: " << currentEntry->getIDName() << std::endl;
    // std::cout << "      " << currentEntry->getIDName() << " is a " << currentEntry->getIDType() << std::endl;

    // if(currentEntry->getIDType() == "function"){
    //     std::vector <Entry*> parameters = currentEntry->getParameterList();

    //     Entry* paramList = getParamListForEntry(parameters, currentEntry->getIDName(), currentEntry->getScope());   //this function needs to return the parameter list for the specific entry

    //     std::cout << std::endl << "      parameter(s) for " << currentEntry->getIDName() << ": " << std::endl;
    //     std::cout << "          " << paramList->getDType() << " " << paramList->getIDName() << std::endl;
    //     std::cout << (paramList->getIsArray() ? "               is an array" : "            is not an array") << std::endl;
    //     std::cout << "            array size is : " << paramList->getArraySize() << std::endl;
    //     std::cout << "            scope is " << paramList->getScope() << std::endl;
    //     std::cout << std::endl;

    //     // logic needed to actually do something with this information
    // }
    // else if(currentEntry->getIDType() == "procedure"){
    //     // needs implementation
    // }
    // else if(currentEntry->getIDType() == "datatype"){ // I don't think this should ever be "datatye". The datatype is the 
    //     // needs implementation
    // }

    // // full implementation still needed

}

void Interpreter::handleAssignment(Node* node) {
    node = node->getSibling();
    // Print all entries in the symbolTable first
    // std::cout << "Symbol Table Contents:" << std::endl;
    // for (const auto& pair : symbolTable) {
    //     std::cout << "Key: '" << pair.first << "' -> Value: " 
    //             << (pair.second ? pair.second->getValue() : "null") << std::endl;
    // }

    // Print the key we're trying to look up
    // std::cout << "Looking up key: '" << node->getValue() << "'" << std::endl;

    // Now check and update the specific entry
    auto it = symbolTable.find(node->getValue()); // Perform the lookup once
    if (it != symbolTable.end()) { // Check if the key exists
        if (it->second) { // Ensure the value (mapped pointer) is not null
            it->second->setValue(evaluatePostfix(node));
        } else {
            std::cerr << "Error: Value for key '" << node->getValue() << "' is null." << std::endl;
        }
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
    // if (loopStack.size() == stackCount) {
    //     setProgramCounter(tempNode);
    // }
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
    // if (loopStack.size() == stackCount) {
    //     setProgramCounter(tempNode);
    // }
}


void Interpreter::handleSelection(Node* node/*pass current AST node here*/){
}

void Interpreter::handlePrintf(Node* node /* pass current AST node here */) {
    Node* print = node->getSibling();
    std::vector<std::string> results;

    // Collect values to replace in the format string
    if (print && print->getSibling() != nullptr) {
        Node* temp = print->getSibling();
        while (temp != nullptr) {
            // Find the value in the symbolTable and validate it
            auto it = symbolTable.find(temp->getValue());
            if (it != symbolTable.end() && it->second != nullptr) {
                results.push_back(it->second->getValue());
            } else {
                std::cerr << "Error: Key '" << temp->getValue() 
                          << "' not found in symbolTable or has a null value." << std::endl;
            }
            temp = temp->getSibling();
        }
    }

    // Get the format string from the print node
    std::string formatString = print ? print->getValue() : "";
    if (formatString.empty()) {
        std::cerr << "Error: Print node format string is empty or null." << std::endl;
        return;
    }

    // Replace each %d and %s in the format string with elements from results
    size_t pos = 0;
    size_t resultIndex = 0;

    while ((pos = formatString.find("%", pos)) != std::string::npos) {
        if (pos + 1 < formatString.size()) {
            char nextChar = formatString[pos + 1];

            // Handle %d (integer replacement)
            if (nextChar == 'd' && resultIndex < results.size()) {
                int value = std::stoi(results[resultIndex]);  // Convert to integer
                formatString.replace(pos, 2, std::to_string(value)); // Replace %d with the integer value
                resultIndex++;
            }
            // Handle %s (string replacement)
            else if (nextChar == 's' && resultIndex < results.size()) {
                formatString.replace(pos, 2, results[resultIndex]);  // Replace %s with the string value
                resultIndex++;
            }
        }
        pos += 2; // Move past the placeholder
    }

    // Split the format string by \n and print each segment
    size_t newlinePos = 0;
    while ((newlinePos = formatString.find("\\n")) != std::string::npos) {
        // Print up to the newline character
        std::cout << formatString.substr(0, newlinePos) << std::endl;
        // Remove the part up to and including the newline sequence
        formatString = formatString.substr(newlinePos + 2);
    }

    // Print the remaining part of the format string
    if (!formatString.empty()) {
        std::cout << formatString << std::endl;
    }
}

std::string Interpreter::handleReturn(Node* node/*pass current AST node here*/){
    if (node->getSibling() != nullptr) {
        auto it = symbolTable.find(node->getSibling()->getValue());
        if (it != symbolTable.end()) {
            // If found, return the value associated with the symbol in the symbolTable
            std::cout << "Found in symbolTable: " << it->second->getValue() << std::endl;
            return it->second->getValue();  // Returning the value found in the symbol table
        } else {
            // If the sibling's value is not found in the symbolTable
            std::cerr << "Error: Symbol '" << node->getSibling()->getValue() << "' not found in symbolTable." << std::endl;
            return "";  // Return an empty string or an error value
        }
    } else {
                std::cout << "Does Not Exist" << std::endl;

        return "";
    }
}

void Interpreter::handleFunction(Node* node/*pass current AST node here*/){

}

void Interpreter::handleProcedure(Node* node/*pass current AST node here*/){
}

void Interpreter::handleIf(Node* node){
    //new attempt at this function V
    Node* ifNode = node;
    std::cout << "Testing condition" << std::endl;
    if (evaluateBooleanPostfix(ifNode)) {
        std::cout << "Condition Passed" << std::endl;
        executeBlock(nextNode(node));
        std::cout << "Returned From executeBlock()" << std::endl;
    } 
    else { // skip the if logic and find the next 
        std::cout << "Condition Failed" << std::endl;
    }
}

void Interpreter::handleElse(Node* node){
    short stackSize = 0; 
    std::stack<int> elseStack;
    node = nextNode(node);
   do {
        if (node->getValue() == "BEGIN_BLOCK") {
            elseStack.push(1);
        } else if ( node->getValue() == "END_BLOCK") {
            elseStack.pop();
        }
        executeStatement(node);
        node = nextNode(node);

   } while( stackSize != 0 );
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
std::unordered_map<std::string, std::vector<Entry*>> Interpreter::convertTable(Table* table) {
    
    std::unordered_map<std::string, std::vector<Entry*>> symbolMap;

    Entry* head = table->getHead();

    while( head != nullptr) {

        auto it = symbolMap.find(head->getIDName());
        if ( it != symbolMap.end() ) {
            it->second.push_back(head);
        } else {
            std::vector<Entry*> newVec;
            newVec.push_back(head);
            symbolMap.emplace(head->getIDName(), newVec);

        }

        // pushes symbol table entry's identifier's name into insertOrder to preserve CORRECT symbol table order
       // insertOrder.push_back(head->getIDName());   

        head = head->getNext();
    }
    // Add parameters
    head = table->getHead();
    while( head != nullptr) {
        auto list = head->getParameterList();
        for ( Entry* parameter : list ) {
            symbolMap.emplace(parameter->getIDName(), parameter);
        }

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
    bool isString = false;

    // This segment will print the expression being evaluated
    auto temp = current;
    while (temp != nullptr) {
        std::cout << temp->getValue() << " ";
        temp = temp->getSibling();
    }
    std::cout << std::endl;

    while (current != nullptr) {
        //std::cout << "Token: " << current->getValue() << std::endl; //This will print the current symbol being processed ****
        const std::string& token = current->getValue();

        try {
            auto it = symbolTable.find(token);

            if (token.find("\"") != std::string::npos) {
                isString = true;
            } else if (isString) {  
                return token;
            }
            // Check if the token is a variable in the symbol table
            else if (it != symbolTable.end()) {
                std::cout << "\nWe're in the symbol table";
                Entry* entry = it->second;
                if (entry != nullptr) {
                    std::cout << "\n Not Null";
                    auto idType = entry->getIDType();
                    std::cout << "\nGot Type";
                    if (idType == "procedure" || idType == "function") { // This if block will need to be adapted since there is a CALL
                            // Assuming the function name or identifier is stored in 'node->getValue()'
                            std::string functionName = token;
                            std::cout << "\nis func";
                            // Look up the function in the functionMap
                            auto it = functionMap.find(functionName);
                            if (it != functionMap.end()) {  // If the function exists in the map
                                // This may overwrite variables in scope. Will need to adjust if we end up caring about scope. 
                                if ( current->getSibling() != nullptr ){ 
                                    Node* sibling = current->getSibling();
                                    sibling = sibling->getSibling();

                                    std::string ch = "Z"; // For testing

                                    std::cout << "sib is: " << sibling->getValue();
                                    auto val = symbolTable.find(sibling->getValue());
                                    if (val != symbolTable.end()) { 
                                        std::cout << "PASSED THIS";
                                       if ( val->second->getIsArray() ) {
                                            std::cout << "here";
                                            sibling = sibling->getSibling();
                                            sibling = sibling->getSibling();
                                            std::cout << "\nVal: " << val->first;
                                            std::cout << "\nIndex: " << sibling->getValue();
                                            std::cout << "\nVal->second: " << val->second->getValue();
                                            std::cout << "\ni: " << symbolTable.at(sibling->getValue())->getValue();
                                            std::cout << "\ni int: " << std::stoi(symbolTable.at(sibling->getValue())->getValue());

                                           auto temp = val->second->getValue().at(std::stoi(symbolTable.at(sibling->getValue())->getValue()));
                                           std::cout << "\nTemp: " << temp;
                                            ch = std::string(1, temp);  // Convert the char 'temp' to a string
                                                                                    std::cout << "\ndone";

                                       }
                                       
                                    }

                                    auto param = symbolTable.find(functionName);
                                    if (param != symbolTable.end()) { 
                                        auto temp = param->second->getParameterList();
                                        std::cout << "LOOK HERE LOOK HERE!";
                                        std::cout << temp.at(0)->getIDName();
                                        temp.at(0)->setValue(ch);

                                    } 
                                       

                                }
                               return executeCall(it->second);
                            } else {
                                std::cerr << "Error: Function '" << functionName << "' not found in functionMap." << std::endl;
                                return "0"; // Or some error value
                            }
                    }
                    std::cout << "\nTraced!";
                    std::cout << "\nEntry is: " << entry->getValue();

                    int value;
                    if ( entry->getValue().length() > 1 ) {
                        value = std::stoi(entry->getValue());
                    } else {
                        value = std::stoi(hexToInt(entry->getValue()));
                    }
                    std::cout << "\nConverted is: " << value;

                    stack.push(value);
                    std::cout << "\nPushed!";

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
                std::cout << "Setting to: " << std::to_string(stack.top());
                return std::to_string(stack.top());
            } else {
                 // Attempt to convert token to an integer
                if ( token != "'") {
                    int value;
                    if ( token.length() > 1 ) {
                        value = std::stoi(token);
                    } else {
                        value = std::stoi(hexToInt(token));
                    }
                    stack.push(value);
                }
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

    // Print all entries in the symbolTable first
    std::cout << "Symbol Table Contents:" << std::endl;
    for (const auto& pair : symbolTable) {
        std::cout << "Key: '" << pair.first << "' -> Value: " 
                << (pair.second ? pair.second->getValue() : "null") << std::endl;
    }
    std::stack<int> stack;  // Use int stack to align with treating booleans as integers (0 or 1)
    Node* current = node->getSibling();

    while (current != nullptr) {
        const std::string& token = current->getValue();
        std::cout << "\nCurrent: " << token;

        try {
            // Check if the token is a variable in the symbol table
            auto it = symbolTable.find(token);
            if (it != symbolTable.end()) {
                std::cout << "Should go here";
                Entry* entry = it->second;
                if (entry != nullptr) {
                    std::cout << "\nNot nullptr";
                    std::cout << "\nEntry: " << entry->getIDName();
                    std::string value = entry->getValue();
                    std::cout << "\nValue: " << value;
                    int result = 0;
                     if ( value.length() > 1 ) {
                        std::cout << "\nisIndeed\n";
                        result = std::stoi(value);
                        std::cout << "Done";
                    } else {
                        std::cout << "\nisNot\n";
                         std::cout << "\nTrying";

                        std::string hex = hexToInt(value);
                        if ( hex == "-1") {
                            result = std::stoi(symbolTable.at(value)->getValue());
                        } else {
                            std::cout << "\nRight Place";
                            result = std::stoi(hex);
                        }
                    }
                    std::cout << "Converted: " << result;
                    stack.push(result);
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
                std::cout << "In the else";
                // Attempt to convert token to an integer
                int value;
                if ( token != "'") { 
                    if ( token.length() > 1 ) {
                        value = std::stoi(token);
                    } else {
                        value = std::stoi(hexToInt(token));
                    }
                    stack.push(value);
                }
            }
        } catch (const std::invalid_argument&) {
            throw std::runtime_error("Invalid token in boolean expression: " + token);
        } catch (const std::out_of_range&) {
            throw std::runtime_error("Number out of range: " + token);
        }

        current = current->getSibling();
    }

    if (stack.size() != 1) {
        std::cout << "Still on stack: ";
        while(!stack.empty()) {
            std::cout << " " << stack.top();
            stack.pop();
        }
        throw std::runtime_error("Invalid boolean postfix expression: stack size mismatch");
    }

    return stack.top();  // Return the final boolean value 
}

Node* Interpreter::skipBlock(Node* node){
    std::stack<int> scopeStack;
    Node* tempNode = nextNode(node);

    do {
        if ( tempNode->getValue() == "BEGIN_BLOCK" ){
            scopeStack.push(1);
        } else if ( tempNode->getValue() == "END_BLOCK" ) {
            scopeStack.pop();
        }
        tempNode = nextNode(tempNode);
    } while (scopeStack.size() != 0);
    return tempNode;
}

Node* Interpreter::executeBlock(Node* node){
    std::stack<int> scopeStack;
    scopeStack.push(1);
    Node* tempNode = nextNode(node);

    while (scopeStack.size() != 0) {
        if ( tempNode->getValue() == "BEGIN_BLOCK" ){
            scopeStack.push(1);
        } else if ( tempNode->getValue() == "END_BLOCK" ) {
            scopeStack.pop();
        } else {
            executeStatement(tempNode);
        }
        tempNode = nextNode(tempNode);

    }
    return tempNode;
}

std::string Interpreter::executeCall(Node* node){
    std::stack<int> scopeStack;
    scopeStack.push(1);
    Node* tempNode = nextNode(node);
    
    std::cout << "\n\n\n\n CALL!!! \n\n\n\n";

    while (scopeStack.size() != 0) {
        if ( tempNode->getValue() == "BEGIN_BLOCK" ){
            scopeStack.push(1);
        } else if ( tempNode->getValue() == "END_BLOCK" ) {
            scopeStack.pop();
        } else if ( tempNode->getValue() == "RETURN" ){
            std::cout << "Returning: " << handleReturn(tempNode) << std::endl;
            return  handleReturn(tempNode);
        }
        else {
            executeStatement(tempNode);
        }
        if (tempNode->getValue() == "IF") {
             std::cout << "Skipping" << std::endl;
            if ( evaluateBooleanPostfix(tempNode) ) {
                tempNode = skipBlock(tempNode);
                if ( tempNode->getValue() == "ELSE" ) {
                    tempNode = skipBlock(tempNode);
                }

            } else {
                tempNode = skipBlock(tempNode);
            }
            std::cout << "Skipped to: " << tempNode->getValue() << std::endl;;
            std::cout << "If handled" << std::endl;
        } else {
            tempNode = nextNode(tempNode);
        }
    }
    return "";
}

short Interpreter::numFunctions(Entry* entry) {
    Entry* temp = entry;

    short num = 0;
    while (temp != nullptr) {
        if ( temp->getIDType() == "function"  || temp->getIDType() == "procedure" ) {
                    std::cout << temp->getIDName() << std::endl;
                    functionVector.push_back(temp->getIDName());
            num++;
        }
        temp = temp->getNext();
    }
    return num;
}

std::string Interpreter::intToHex(int number) {

    // Handle zero case
    if (number == 0) {
        return "0";
    }

    std::string hexString;
    bool isNegative = number < 0; // Check if the number is negative

    // Use unsigned int for conversion
    unsigned int unsignedNumber;

if (isNegative) {
    unsignedNumber = -number; // Convert negative number to its positive equivalent
} else {
    unsignedNumber = number; // Use the number as is if it's positive
}

    // Map for hexadecimal digits
    const char hexMap[] = "0123456789ABCDEF";

    // Convert number to hexadecimal
    while (unsignedNumber > 0) {
        int remainder = unsignedNumber % 16;
        hexString = hexMap[remainder] + hexString;
        unsignedNumber /= 16;
    }

    // Add a negative sign if the original number was negative
    if (isNegative) {
        hexString = "-" + hexString;
    }

    return hexString;
}

std::string Interpreter::hexToInt(std::string ch) {
    // Ensure the string is not empty
    if (ch.empty()) {
        throw std::invalid_argument("Input string is empty");
    }

    // Convert the first character of the string to a char
    char hexChar = ch[0];

    // Check and convert the character to its corresponding integer value
    if (hexChar >= '0' && hexChar <= '9') {
        return std::to_string(hexChar - '0'); // Converts '0'-'9' to 0-9
    } else if (hexChar >= 'A' && hexChar <= 'F') {
        return std::to_string(hexChar - 'A' + 10); // Converts 'A'-'F' to 10-15
    } else if (hexChar >= 'a' && hexChar <= 'f') {
        return std::to_string(hexChar - 'a' + 10); // Converts 'a'-'f' to 10-15 (lowercase support)
    } else {
        return "-1";
    }
}