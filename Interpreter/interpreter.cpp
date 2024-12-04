#include "interpreter.h"
#include <iostream>

std::string file = "Interpreter_Output.txt";
std::ofstream output(file);

void Interpreter::begin(Node* node){
    short num = numFunctions(rawTable);

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

    while (programCounter != nullptr){
        executeStatement(programCounter); // pass to executeStatement for further processing
        if (programCounter->getValue() == "WHILE") {
            programCounter = skipBlock(programCounter);
        } else {
            programCounter = nextStatement(); // need logic to get next statement
        }
    } 
}

Node* Interpreter::nextStatement(){
    if (programCounter->getValue() != "FOR_EXPRESSION1" && programCounter->getValue() != "FOR_EXPRESSION2" && programCounter->getValue() != "FOR_EXPRESSION3" && programCounter->getValue() != "IF") {
        while(programCounter->getSibling() != nullptr) {
            programCounter = programCounter->getSibling();
        }
        return programCounter->getChild(); 
    } 
    return skipBlock(programCounter);
}

void Interpreter::setProgramCounter(Node* node){
    programCounter = node;
}

Node* Interpreter::nextNode(Node* node){
    while(node->getSibling() != nullptr) {
        node = node->getSibling();
    }
    return node->getChild(); 
}

// processes a single AST node
void Interpreter::executeStatement(Node* curNode){
    if(curNode->getValue() == "DECLARATION"){    
        return;
    }
    else if (curNode->getValue() == "ASSIGNMENT"){  
        return handleAssignment(curNode);
    }
    else if (curNode->getValue() == "WHILE"){ 
        return handleWhile(curNode);
    }
    else if (curNode->getValue() == "FOR_EXPRESSION1") {   
      return handleFor(curNode); 
    }
    else if (curNode->getValue() == "FOR_EXPRESSION2" ||
             curNode->getValue() == "FOR_EXPRESSION3") {   
      return; //skip, already handled above in "FOR_EXPRESSION1"
    }
    else if (curNode->getValue() == "PRINTF"){      
        return handlePrintf(curNode);
    }
    else if (curNode->getValue() == "RETURN"){    
        handleReturn(curNode);
        return;
    } 
    else if (curNode->getValue() == "IF"){        
        return handleIf(curNode); 
    } 
    else if (curNode->getValue() == "ELSE"){        
        return handleElse(curNode); 
    } 
    else if(curNode->getValue() == "BEGIN_BLOCK"){ 
        return;    
    } 
    else if(curNode->getValue() == "END_BLOCK"){ 
        return;        
    }
    else {
        std::cerr << "Statement type not recognized: " << curNode->getValue() << std::endl;
        exit(3);
    }
}

void Interpreter::handleAssignment(Node* node) {
    node = node->getSibling();
    // Now check and update the specific entry
        if (getEntry(node->getValue())){ // Ensure the value (mapped pointer) is not null
            getEntry(node->getValue())->setValue(evaluatePostfix(node));
        } 
        else {
            std::cerr << "Error: Value for key " << node->getValue() << " is null." << std::endl;
        }
}

void Interpreter::handleWhile(Node* node) {
    Node* tempNode;
    auto begin = nextNode(node);

    while (evaluateBooleanPostfix(node)) {
        tempNode = nextNode(begin);
        loopStack.push(1);
        int stackCount = loopStack.size();

        while (loopStack.size() >= 1) {
            if (tempNode->getValue() == "END_BLOCK") {
                loopStack.pop();
            }
            if (loopStack.size() == stackCount) {
                if (tempNode->getValue() == "CALL") {
                    auto functionNode = functionMap.find(tempNode->getSibling()->getValue());
                    // Get the value for the parameter 
                    std::string var = getEntry(tempNode->getSibling()->getSibling()->getSibling()->getValue())->getValue();
                    // Set the value of the parameter
                    auto param = symbolTable.find(functionNode->first);
                    if (param != symbolTable.end()) { 
                        auto temp = getEntry(param->first)->getParameterList();
                        temp.at(0)->setValue(var);
                    } 
                    executeCall( nextNode(functionNode->second) ); 
                } 
                else {
                    executeStatement(tempNode);
                }
            }
            if (tempNode->getValue() == "IF") {
                if (evaluateBooleanPostfix(tempNode)) {
                    tempNode = skipBlock(tempNode);
                    if (tempNode->getValue() == "ELSE") {
                        tempNode = skipBlock(tempNode);
                    }

                } 
                else {
                    tempNode = skipBlock(tempNode);     
                }  
            }
            else {
                tempNode = nextNode(tempNode); 
            }
        }
    }
}

void Interpreter::handleFor(Node* node) {
    // Get the three expressions
    Node* temp = node;
    Node* expr1 = temp->getSibling();  // Initialization (pointing to the variable to be initialized)
    temp = nextNode(temp);
    Node* expr2 = temp;  // Condition
    temp = nextNode(temp);
    Node* expr3 = temp->getSibling();  // Iteration (pointing to the variable to be initialized)
    
    // Evaluate the first expression (initialization) and set the value of the variable
    std::string result1 = evaluatePostfix(expr1);
    getEntry(expr1->getValue())->setValue(result1);
  
    // Push the current loop to the stack to handle nested loops
    loopStack.push(1);  
    int stackCount = loopStack.size();

    // Loop on the second expression (condition)
    Node* tempNode; 
    short loop = 1;

    while (evaluateBooleanPostfix(expr2)) {
        loop++;
        // Handle the body of the loop 
        tempNode = nextNode(expr3); 
        while (!(tempNode->getValue() == "END_BLOCK" && loopStack.size() == stackCount)) {
            if (tempNode->getValue() == "END_BLOCK") {
                loopStack.pop();
            }
            if (loopStack.size() == stackCount) {
                executeStatement(tempNode);
            }
            if (tempNode->getValue() == "IF") {
                break;

            } 
            else {
                tempNode = nextNode(tempNode); 
            }
        }
        // Evaluate the third expression (iteration) to continue loop
        getEntry(expr3->getValue())->setValue(evaluatePostfix(expr3));
    }
}

void Interpreter::handlePrintf(Node* node) {
    Node* print = node->getSibling();
    std::vector<std::string> results;
    // Collect values to replace in the format string
    if (print && print->getSibling() != nullptr) {
        Node* temp = print->getSibling();
        while (temp != nullptr) {
            // Find the value in the symbolTable and validate it
            auto it = symbolTable.find(temp->getValue());
            if (it != symbolTable.end() && getEntry(it->first) != nullptr) {
                results.push_back(getEntry(it->first)->getValue());
            } 
            else {
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

    // Replace each underscore with a space
    size_t pos = 0;
    while ((pos = formatString.find('_', pos)) != std::string::npos) {
        formatString.replace(pos, 1, " ");
        pos++;
    }

    // Replace each %d and %s in the format string with elements from results
    pos = 0;
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

    pos = 0;
    while ((pos = formatString.find("\\x0")) != std::string::npos) {
        formatString.erase(pos, 3); // Remove the 4 characters corresponding to "\x0"
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
        std::cout << formatString;
    }
}

std::string Interpreter::handleReturn(Node* node){
    if (node->getSibling() != nullptr) {
        auto it = symbolTable.find(node->getSibling()->getValue());
        if (it != symbolTable.end()) {
            // If found, return the value associated with the symbol in the symbolTable
            return getEntry(it->first)->getValue();  // Returning the value found in the symbol table
        } 
        else {
            // If the sibling's value is not found in the symbolTable
            std::cerr << "Error: Symbol '" << node->getSibling()->getValue() << "' not found in symbolTable." << std::endl;
            return "";  // Return an empty string or an error value
        }
    } else {
        // Does Not Exist
        return "";
    }
}

void Interpreter::handleIf(Node* node){
    Node* ifNode = node;
    if (evaluateBooleanPostfix(ifNode)) {
        executeBlock(nextNode(node));
    } 
    else {
        //Condition Failed;
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

   } while (stackSize != 0);
}

// Helper functions 

bool Interpreter::isOperator(const std::string& token) {
    return token == "+" || token == "-" || token == "*" || token == "/" ||
           token == "&&" || token == "||" || token == "!" || token == "<" || 
           token == ">" || token == "<=" || token == ">=" || token == "==" ||
           token == "%" ;
}

bool Interpreter::isArithmeticOperator(const std::string& token) {
    return token == "+" || token == "-" || token == "*" || token == "/" || token == "%";
}

bool Interpreter::isBooleanOperator(const std::string& token) {
    return token == "&&" || token == "||" || token == "!" || token == "<" || 
           token == ">" || token == "<=" || token == ">=" || token == "==";
}

int Interpreter::performPostfixOperation(int a, int b, const std::string& op) {
    if (op == "+") return a + b;
    if (op == "-") return a - b;
    if (op == "*") return a * b;
    if (op == "%") return a % b;
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

    while (head != nullptr) {
        auto it = symbolMap.find(head->getIDName());
        if (it != symbolMap.end()) {
            it->second.push_back(head);
        } 
        else {
            std::vector<Entry*> newVec;
            newVec.push_back(head);
            symbolMap.emplace(head->getIDName(), newVec);

        }
        // pushes symbol table entry's identifier's name into insertOrder to preserve CORRECT symbol table order
        head = head->getNext();
    }
    // Add parameters
    head = table->getHead();

    while(head != nullptr) {
        auto list = head->getParameterList();
        for ( Entry* parameter : list ) {
            auto it = symbolMap.find(parameter->getIDName());
        if (it != symbolMap.end()) {
            it->second.push_back(parameter);
            //std::cout << "Pushing " << parameter->getIDName() << " from parameters at " << head->getScope() << std::endl;
        } 
        else {
            //std::cout << "Pushing something else";
            std::vector<Entry*> newVec;
            newVec.push_back(parameter);
            symbolMap.emplace(parameter->getIDName(), newVec);

        }
        }
        // pushes symbol table entry's identifier's name into insertOrder to preserve CORRECT symbol table order
        head = head->getNext();
    }
    return symbolMap;
}

std::string Interpreter::evaluatePostfix(Node* node) {
    std::stack<int> stack;
    Node* current = node->getSibling();
    bool isString = false;

    auto temp = current;
    while (temp != nullptr) {
        temp = temp->getSibling();
    }

    while (current != nullptr) {
        const std::string& token = current->getValue();
        try {
            auto it = symbolTable.find(token);

            if (token.find("\"") != std::string::npos) {
                isString = true;
            } 
            else if (isString) {  
                return token;
            }
            // Check if the token is a variable in the symbol table
            else if (it != symbolTable.end()) {
                Entry* entry = getEntry(it->first);
                if (entry != nullptr) {
                   // std::cout << "\n Not Null";
                    auto idType = entry->getIDType();
                   // std::cout << "\nGot Type";
                    if (idType == "procedure" || idType == "function") { 
                            std::string functionName = token;
                            // Look up the function in the functionMap
                            auto it = functionMap.find(functionName);
                            if (it != functionMap.end()) {  // If the function exists in the map
                                // This may overwrite variables in scope. Will need to adjust if we end up caring about scope. 
                                if ( current->getSibling() != nullptr ){ 
                                    Node* sibling = current->getSibling();
                                    sibling = sibling->getSibling();
                                    std::string ch = "Z"; // For testing? 
                                    auto val = symbolTable.find(sibling->getValue());
                                    if (val != symbolTable.end()) { 
                                       if (getEntry(val->first)->getIsArray()) {
                                            sibling = sibling->getSibling();
                                            sibling = sibling->getSibling();
                                            auto temp = getEntry(val->first)->getValue().at(std::stoi(getEntry(sibling->getValue())->getValue()));
                                            ch = std::string(1, temp);  // Convert the char 'temp' to a string
                                       } 
                                       else {
                                            ch = getEntry(sibling->getValue())->getValue();
                                       }
                                       
                                    }
                                    auto param = symbolTable.find(functionName);
                                    if (param != symbolTable.end()) { 
                                        auto temp = getEntry(param->first)->getParameterList();
                                        temp.at(0)->setValue(ch);
                                    } 
                                }
                               auto result = executeCall(it->second);
                               curScope++;
                               return result;
                            } else {
                                std::cerr << "Error: Function '" << functionName << "' not found in functionMap." << std::endl;
                                return "0"; // Or some error value
                            }
                    }
                    int value;
                    if ( entry->getValue().length() > 1 ) {
                        value = std::stoi(entry->getValue());
                    } else {
                        value = std::stoi(hexToInt(entry->getValue()));
                    }
                    stack.push(value);
                } else {
                    std::cerr << "Error: Null entry found for token: " << token << std::endl;
                }
            } 
            else if (isOperator(token)) {
                // Handle operator
                if (stack.size() < 2) {
                    throw std::runtime_error("Invalid postfix expression: not enough operands for operator");
                }
                int b = stack.top(); stack.pop();
                int a = stack.top(); stack.pop();
                stack.push(performPostfixOperation(a, b, token));
            } 
            else if (token == "=") {
                // Assignment operator (end of expression)
                if (stack.size() != 1) {
                    throw std::runtime_error("Invalid postfix expression: stack size mismatch at '='");
                }
                //std::cout << "Setting to: " << std::to_string(stack.top());
                return std::to_string(stack.top());
            } 
            else {
                 // Attempt to convert token to an integer
                if (token != "'") {
                    int value;
                    if (token.length() > 1) {
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
    auto tempNode = node; 
    std::stack<int> stack;  // Use int stack to align with treating booleans as integers (0 or 1)
    Node* current = node->getSibling();
    Node* temp = current;
    Node* prev = nullptr;
    Node* tempHead = nullptr;
    Node* next = nullptr;
    bool containsSubexpression = false;

    while (temp != nullptr) {
        Node* newNode = new Node(temp->getValue(), false);
        if (isArithmeticOperator(temp->getValue())) {
            containsSubexpression = true;
            next = temp->getSibling();
            prev->setSibling(newNode);
            Node* equalNode = new Node("=", false); // This adds an equal sign so stack is appropriate size in evaluatePostfix()
            newNode->setSibling(equalNode);
            break;
        }
        if (tempHead == nullptr) {
            tempHead = newNode;
            Node* secondaryNode = new Node(temp->getValue(), false); // This adds a duplicate since evaluatePostfix() skips the first
            tempHead->setSibling(secondaryNode);
            temp = temp->getSibling();
            prev = secondaryNode;
            continue;
        }
        if (prev != nullptr) {
            //std::cout << "Appending" << newNode->getValue() << " to " << prev->getValue() << std::endl;
            prev->setSibling(newNode);
            prev = newNode;
        } 
        else {
            prev = newNode;
        }
        temp = temp->getSibling();

    }

    while (current != nullptr) {
        std::string token = current->getValue();
        std::string token2 = "";
        if (containsSubexpression) {
            std::string token = evaluatePostfix(tempHead);
            token2 = token;
        }
        try {
            auto it = symbolTable.find(token);

             if ( containsSubexpression ) {
                        stack.push(std::stoi(token2));
            }
            // Check if the token is a variable in the symbol table
            else if (it != symbolTable.end()) {
                Entry* entry = getEntry(it->first);
                if (entry != nullptr) {
                    std::string value = entry->getValue();
                    int result = 0;
                     if ( value.length() > 1 ) {
                        result = std::stoi(value);
                    } else {
                        std::string hex = hexToInt(value);
                        if ( hex == "-1") {
                            result = std::stoi( getEntry(value)->getValue());
                        } else {
                            result = std::stoi(hex);
                        }
                    }                
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
                    stack.push(!a);  // Apply negation for '!'
                } 
                else {
                    // Binary operators: two operands are needed
                    if (stack.size() < 2) {
                        throw std::runtime_error("Invalid boolean postfix expression: not enough operands for operator");
                    }
                    b = stack.top(); stack.pop();
                    a = stack.top(); stack.pop();
                    int result = performBooleanOperation(a, b, token);
                    stack.push(result);
                }
            } 
            else {
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
        if ( containsSubexpression) {
            current = next;
            if ( current->getSibling()->getSibling() != nullptr) { // Can we remove this? 
                //std::cout << "Shouldn't be here: " << current->getSibling()->getValue() << std::endl;
            }
            containsSubexpression = false;
        } 
        else {
            current = current->getSibling();
        }
    }
    if (stack.size() != 1) {
        std::cout << "Still on stack: ";
        while (!stack.empty()) {
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
        }
        else if ( tempNode->getValue() == "END_BLOCK" ) {
            scopeStack.pop();
        }
        else {
            executeStatement(tempNode);
        }
        tempNode = nextNode(tempNode);

    }
    return tempNode;
}

std::string Interpreter::executeCall(Node* node){
    curScope--;
    std::stack<int> scopeStack;
    scopeStack.push(1);
    Node* tempNode = nextNode(node);

    while (scopeStack.size() != 0) {
        if ( tempNode->getValue() == "BEGIN_BLOCK" ){
            scopeStack.push(1);
        } else if ( tempNode->getValue() == "END_BLOCK" ) {
            scopeStack.pop();
        } else if ( tempNode->getValue() == "RETURN" ){
            return  handleReturn(tempNode);
        }
        else {
            executeStatement(tempNode);
        }
        if (tempNode->getValue() == "IF") {
            if ( evaluateBooleanPostfix(tempNode) ) {
                tempNode = skipBlock(tempNode);
                if ( tempNode->getValue() == "ELSE" ) {
                    tempNode = skipBlock(tempNode);
                } 
            } else {
                tempNode = skipBlock(tempNode);
            }
        } else {
            tempNode = nextNode(tempNode);
        }
    }
    curScope++;
    return "";
}

short Interpreter::numFunctions(Entry* entry) {
    Entry* temp = entry;
    short num = 0;

    while (temp != nullptr) {
        if ( temp->getIDType() == "function"  || temp->getIDType() == "procedure" ) {
                    //std::cout << temp->getIDName() << std::endl;
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

Entry* Interpreter::getEntry(std::string name) {
    auto it = symbolTable.at(name); 
    for ( Entry* entry : it ) {
        if ( curScope == entry->getScope() || entry->getIDType() == "procedure" || entry->getIDType() == "function") {
            return entry;
        }
    } 
    std::cerr << "\nEntry with appropriate scope not found for " << name << "!";
    return nullptr;
}

void Interpreter::printSymbols() {
    for (const auto& pair : symbolTable) {
            for (Entry* entry : pair.second) {
                std::cout << entry->getIDName() << " Scope: " << entry->getScope() << " Value: " << entry->getValue() << std::endl;
            }
            std::cout << std::endl;
    }
}
