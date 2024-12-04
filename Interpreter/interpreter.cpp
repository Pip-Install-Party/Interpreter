#include "interpreter.h"


#include <iostream>
#if defined(__APPLE__) || defined(__unix__)
#include <ncurses.h>
#endif

std::string fileName = "Output.txt";
std::ofstream cursesBuffer(fileName);

/**
 * Entry point to the interpreter.
 * This function processes the Abstract Syntax Tree (AST) from the beginning node,
 * executing all the statements in the program in order.
 */
void Interpreter::begin(Node* node) {
    short num = numFunctions(rawTable); // Calculate the number of functions.

    // Iterate through nodes until all function declarations are mapped
    while (node != nullptr) {
        if (node->getValue() == "DECLARATION") {
            if (nextNode(node)->getValue() == "BEGIN_BLOCK") {
                // Map function names to their respective AST nodes
                functionMap.emplace(functionVector.at(functionVector.size() - num), node);
                num--;
            }
        }
        // Once all functions have been mapped, set the starting point for execution
        if (num == 0) {
            programCounter = node;
            break;
        }
        node = nextNode(node); // Move to the next node in the AST
    }

    // Execute each statement starting from the programCounter node
    while (programCounter != nullptr) {
        executeStatement(programCounter); // Pass to executeStatement for further processing
        if (programCounter->getValue() == "WHILE") {
            // Skip the block after processing the WHILE loop to avoid re-execution
            programCounter = skipBlock(programCounter);
        } else {
            // Move to the next statement in the program
            programCounter = nextStatement();
        }
    }
    cursesBuffer.close();
}

/**
 * Determine the next statement to be executed by following the tree structure.
 * Skips over parts of control structures as necessary.
 */
Node* Interpreter::nextStatement() {
    // Skip certain types of nodes that are part of control structures (e.g., for loop parts and IF statements)
    if (programCounter->getValue() != "FOR_EXPRESSION1" && programCounter->getValue() != "FOR_EXPRESSION2" &&
        programCounter->getValue() != "FOR_EXPRESSION3" && programCounter->getValue() != "IF") {

        // Move to the sibling nodes until there are no more siblings
        while (programCounter->getSibling() != nullptr) {
            programCounter = programCounter->getSibling();
        }
        return programCounter->getChild(); // Move to the first child node if available
    }
    return skipBlock(programCounter); // Skip the current block if needed
}

/**
 * Update the current statement that is being executed (i.e., the program counter).
 */
void Interpreter::setProgramCounter(Node* node) {
    programCounter = node;
}

/**
 * Move to the next node in the AST (either sibling or child).
 */
Node* Interpreter::nextNode(Node* node) {
    // Move to the next sibling node until there are no more siblings
    while (node->getSibling() != nullptr) {
        node = node->getSibling();
    }
    return node->getChild(); // Move to the first child node if available
}

/**
 * Execute a specific node in the AST.
 * Depending on the node type, different handling is performed.
 */
void Interpreter::executeStatement(Node* curNode) {
    if (curNode->getValue() == "DECLARATION") {
        return; // Do nothing for declarations
    } else if (curNode->getValue() == "ASSIGNMENT") {
        return handleAssignment(curNode); // Handle assignment statements
    } else if (curNode->getValue() == "WHILE") {
        return handleWhile(curNode); // Handle while loop statements
    } else if (curNode->getValue() == "FOR_EXPRESSION1") {
        return handleFor(curNode); // Handle for loop statements
    } else if (curNode->getValue() == "FOR_EXPRESSION2" || curNode->getValue() == "FOR_EXPRESSION3") {
        return; // Skip these, already handled by FOR_EXPRESSION1
    } else if (curNode->getValue() == "PRINTF") {
        return handlePrintf(curNode); // Handle print statements
    } else if (curNode->getValue() == "RETURN") {
        handleReturn(curNode); // Handle return statements
        return;
    } else if (curNode->getValue() == "IF") {
        return handleIf(curNode); // Handle if statements
    } else if (curNode->getValue() == "ELSE") {
        return handleElse(curNode); // Handle else statements
    } else if (curNode->getValue() == "BEGIN_BLOCK" || curNode->getValue() == "END_BLOCK") {
        return; // Do nothing for blocks themselves
    } else {
        // Handle unrecognized statements
        std::cerr << "Statement type not recognized: " << curNode->getValue() << std::endl;
        exit(3);
    }
}

/**
 * Handle assignment statements, updating the symbol table.
 */
void Interpreter::handleAssignment(Node* node) {
    node = node->getSibling();
    // Check if the symbol exists in the symbol table and update its value
    if (getEntry(node->getValue())) {
        getEntry(node->getValue())->setValue(evaluatePostfix(node));
    } else {
        // Error if the symbol is not found
        std::cerr << "Error: Value for key " << node->getValue() << " is null." << std::endl;
    }
}

/**
 * Handle while loop statements.
 * Evaluates the condition and executes the loop body while the condition is true.
 */
void Interpreter::handleWhile(Node* node) {
    Node* tempNode;
    auto begin = nextNode(node); // Get the beginning of the while loop body

    // Continue looping while the condition evaluates to true
    while (evaluateBooleanPostfix(node)) {
        tempNode = nextNode(begin); // Start executing the body of the loop
        loopStack.push(1); // Push the current loop to handle nested loops
        int stackCount = loopStack.size();

        // Execute statements within the loop block
        while (loopStack.size() >= 1) {
            if (tempNode->getValue() == "END_BLOCK") {
                loopStack.pop(); // Pop the loop stack when reaching the end of the block
            }
            if (loopStack.size() == stackCount) {
                if (tempNode->getValue() == "CALL") {
                    // Handle function call within the loop
                    auto functionNode = functionMap.find(tempNode->getSibling()->getValue());
                    // Get the value for the parameter
                    std::string var = getEntry(tempNode->getSibling()->getSibling()->getSibling()->getValue())->getValue();
                    // Set the value of the parameter
                    auto param = symbolTable.find(functionNode->first);
                    if (param != symbolTable.end()) {
                        auto temp = getEntry(param->first)->getParameterList();
                        temp.at(0)->setValue(var);
                    }
                    executeCall(nextNode(functionNode->second)); // Execute the function call
                } else {
                    executeStatement(tempNode); // Execute other statements within the loop
                }
            }
            // Handle IF statements within the loop
            if (tempNode->getValue() == "IF") {
                if (evaluateBooleanPostfix(tempNode)) {
                    tempNode = skipBlock(tempNode); // Skip the block if the condition is true
                    if (tempNode->getValue() == "ELSE") {
                        tempNode = skipBlock(tempNode); // Skip the ELSE block as well
                    }
                } else {
                    tempNode = skipBlock(tempNode); // Skip the IF block if the condition is false
                }
            } else {
                tempNode = nextNode(tempNode); // Move to the next node
            }
        }
    }
}

/**
 * Handle for loop statements.
 * Processes initialization, condition, and iteration parts of the for loop.
 */
void Interpreter::handleFor(Node* node) {
    // Get the three expressions in the for loop
    Node* temp = node;
    Node* expr1 = temp->getSibling(); // Initialization expression
    temp = nextNode(temp);
    Node* expr2 = temp; // Condition expression
    temp = nextNode(temp);
    Node* expr3 = temp->getSibling(); // Iteration expression

    // Evaluate the initialization expression
    std::string result1 = evaluatePostfix(expr1);
    getEntry(expr1->getValue())->setValue(result1);

    // Push the current loop to handle nested loops
    loopStack.push(1);
    int stackCount = loopStack.size();

    // Execute the loop while the condition is true
    Node* tempNode;
    short loop = 1;

    while (evaluateBooleanPostfix(expr2)) {
        loop++;
        tempNode = nextNode(expr3); // Start executing the loop body
        while (!(tempNode->getValue() == "END_BLOCK" && loopStack.size() == stackCount)) {
            if (tempNode->getValue() == "END_BLOCK") {
                loopStack.pop(); // Pop the loop stack when reaching the end of the block
            }
            if (loopStack.size() == stackCount) {
                executeStatement(tempNode); // Execute statements within the loop
            }
            // Handle IF statements within the loop
            if (tempNode->getValue() == "IF") {
                break; // Exit the loop body to handle the IF condition
            } else {
                tempNode = nextNode(tempNode); // Move to the next node
            }
        }
        // Evaluate the iteration expression to update the loop variable
        getEntry(expr3->getValue())->setValue(evaluatePostfix(expr3));
    }
}

/**
 * Handle printf-like functionality for output.
 * This function processes a format string and replaces placeholders with values.
 */
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
            } else {
                // Error if the key is not found in the symbolTable or has a null value
                std::cerr << "Error: Key '" << temp->getValue() << "' not found in symbolTable or has a null value." << std::endl;
            }
            temp = temp->getSibling(); // Move to the next value to be printed
        }
    }

    // Get the format string from the print node
    std::string formatString = print ? print->getValue() : "";
    if (formatString.empty()) {
        // Error if the format string is empty or not provided
        std::cerr << "Error: Print node format string is empty or null." << std::endl;
        return;
    }

    // Replace underscores with spaces to properly format the string
    size_t pos = 0;
    while ((pos = formatString.find('_', pos)) != std::string::npos) {
        formatString.replace(pos, 1, " ");
        pos++;
    }

    // Replace each %d and %s in the format string with elements from results vector
    pos = 0;
    size_t resultIndex = 0;
    while ((pos = formatString.find("%", pos)) != std::string::npos) {
        if (pos + 1 < formatString.size()) {
            char nextChar = formatString[pos + 1];

            // Handle %d (integer replacement)
            if (nextChar == 'd' && resultIndex < results.size()) {
                int value = std::stoi(results[resultIndex]);  // Convert the string to integer
                formatString.replace(pos, 2, std::to_string(value)); // Replace %d with integer value
                resultIndex++;
            }
            // Handle %s (string replacement)
            else if (nextChar == 's' && resultIndex < results.size()) {
                formatString.replace(pos, 2, results[resultIndex]);  // Replace %s with string value
                resultIndex++;
            }
        }
        pos += 2; // Move past the placeholder
    }

    // Remove null terminators (\x0) from the format string
    pos = 0;
    while ((pos = formatString.find("\\x0")) != std::string::npos) {
        formatString.erase(pos, 3);
    }

    // Split the format string by \n and print each segment to handle newlines
    size_t newlinePos = 0;
    while ((newlinePos = formatString.find("\\n")) != std::string::npos) {
        // Print the part up to the newline
 	if (newlinePos != std::string::npos) {
        cursesBuffer << formatString.substr(0, newlinePos).c_str();
    } else {
        cursesBuffer << formatString.c_str();
    }        // Remove the printed part including the newline sequence
        formatString = formatString.substr(newlinePos + 2);
    }

    // Print the remaining part of the format string
    if (!formatString.empty()) {
      cursesBuffer << formatString;
    }
}

/**
 * Handle return statements by retrieving the value from the symbol table.
 * If the value is not found, an error message is displayed.
 */
std::string Interpreter::handleReturn(Node* node) {
    if (node->getSibling() != nullptr) {
        auto it = symbolTable.find(node->getSibling()->getValue());
        if (it != symbolTable.end()) {
            // Return the value associated with the symbol in the symbolTable
            return getEntry(it->first)->getValue();
        } else {
            // Error if the symbol is not found in the symbolTable
            std::cerr << "Error: Symbol '" << node->getSibling()->getValue() << "' not found in symbolTable." << std::endl;
            return "";  // Return an empty string as an error value
        }
    } else {
        // No value to return (empty return)
        return "";
    }
}

/**
 * Handle if statements.
 * Executes the block if the condition is true.
 */
void Interpreter::handleIf(Node* node) {
    Node* ifNode = node;
    if (evaluateBooleanPostfix(ifNode)) {
        executeBlock(nextNode(node)); // Execute the block if the condition is true
    } else {
        // Condition failed, do nothing
    }
}

/**
 * Handle else statements.
 * Executes the else block if present.
 */
void Interpreter::handleElse(Node* node) {
    short stackSize = 0;
    std::stack<int> elseStack;

    node = nextNode(node);
    // Execute each statement within the else block until we reach the end of the block
    do {
        if (node->getValue() == "BEGIN_BLOCK") {
            elseStack.push(1); // Track the nested BEGIN_BLOCK
        } else if (node->getValue() == "END_BLOCK") {
            elseStack.pop(); // Track the end of a block
        }
        executeStatement(node); // Execute the current statement
        node = nextNode(node);  // Move to the next node in the block

    } while (stackSize != 0);
}

// Helper functions to determine the type of operator used in the expressions

/**
 * Determine if the token is an operator (arithmetic or boolean).
 */
bool Interpreter::isOperator(const std::string& token) {
    return token == "+" || token == "-" || token == "*" || token == "/" ||
           token == "&&" || token == "||" || token == "!" || token == "<" ||
           token == ">" || token == "<=" || token == ">=" || token == "==" ||
           token == "%" ;
}

/**
 * Determine if the token is an arithmetic operator.
 */
bool Interpreter::isArithmeticOperator(const std::string& token) {
    return token == "+" || token == "-" || token == "*" || token == "/" || token == "%";
}

/**
 * Determine if the token is a boolean operator.
 */
bool Interpreter::isBooleanOperator(const std::string& token) {
    return token == "&&" || token == "||" || token == "!" || token == "<" ||
           token == ">" || token == "<=" || token == ">=" || token == "==";
}

/**
 * Perform arithmetic operation on two integers based on the given operator.
 */
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

/**
 * Perform boolean operation on two integers based on the given operator.
 */
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

/**
 * Convert the symbol table to an unordered_map.
 * This function ensures correct order when adding parameters to the symbol map.
 */
std::unordered_map<std::string, std::vector<Entry*>> Interpreter::convertTable(Table* table) {
    std::unordered_map<std::string, std::vector<Entry*>> symbolMap;
    Entry* head = table->getHead();

    // Traverse through the linked list and add each entry to the symbolMap
    while (head != nullptr) {
        auto it = symbolMap.find(head->getIDName());
        if (it != symbolMap.end()) {
            it->second.push_back(head);
        } else {
            std::vector<Entry*> newVec;
            newVec.push_back(head);
            symbolMap.emplace(head->getIDName(), newVec);
        }
        head = head->getNext();
    }

    // Add parameters from each entry to the symbolMap
    head = table->getHead();
    while (head != nullptr) {
        auto list = head->getParameterList();
        for (Entry* parameter : list) {
            auto it = symbolMap.find(parameter->getIDName());
            if (it != symbolMap.end()) {
                it->second.push_back(parameter);
            } else {
                std::vector<Entry*> newVec;
                newVec.push_back(parameter);
                symbolMap.emplace(parameter->getIDName(), newVec);
            }
        }
        head = head->getNext();
    }
    return symbolMap;
}
std::string Interpreter::evaluatePostfix(Node* node) {
    std::stack<int> stack;
    Node* current = node->getSibling();
    bool isString = false;

    // Loop to traverse the nodes (not performing any operations here)
    auto temp = current;
    while (temp != nullptr) {
        temp = temp->getSibling();
    }

    // Main loop to evaluate the postfix expression
    while (current != nullptr) {
        const std::string& token = current->getValue();
        try {
            auto it = symbolTable.find(token);

            // If the token contains a quote, treat it as a string literal
            if (token.find("\"") != std::string::npos) {
                isString = true;
            }
            // If a string was found, return it
            else if (isString) {
                return token;
            }
            // Check if the token is a variable in the symbol table
            else if (it != symbolTable.end()) {
                Entry* entry = getEntry(it->first);
                if (entry != nullptr) {
                    auto idType = entry->getIDType();

                    // If the entry is a procedure or function, execute it
                    if (idType == "procedure" || idType == "function") {
                        std::string functionName = token;
                        auto it = functionMap.find(functionName);
                        if (it != functionMap.end()) {  // If the function exists in the map
                            // Handle function parameter, if it exists
                            if (current->getSibling() != nullptr) {
                                Node* sibling = current->getSibling();
                                sibling = sibling->getSibling();
                                std::string ch = "Z"; // Default character for testing
                                auto val = symbolTable.find(sibling->getValue());
                                if (val != symbolTable.end()) {
                                    // Check if the entry is an array and retrieve the correct value
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
                                // Set the value of the function parameter
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
                            // Error if the function is not found in the function map
                            std::cerr << "Error: Function '" << functionName << "' not found in functionMap." << std::endl;
                            return "0"; // Return some error value
                        }
                    }

                    // If the entry is not a function, push its value to the stack
                    int value;
                    if (entry->getValue().length() > 1) {
                        value = std::stoi(entry->getValue());
                    } else {
                        value = std::stoi(hexToInt(entry->getValue()));
                    }
                    stack.push(value);
                } else {
                    // Error if the entry is null
                    std::cerr << "Error: Null entry found for token: " << token << std::endl;
                }
            }
            // If the token is an operator, perform the operation
            else if (isOperator(token)) {
                if (stack.size() < 2) {
                    throw std::runtime_error("Invalid postfix expression: not enough operands for operator");
                }
                int b = stack.top(); stack.pop();
                int a = stack.top(); stack.pop();
                stack.push(performPostfixOperation(a, b, token));
            }
            // Handle the assignment operator
            else if (token == "=") {
                if (stack.size() != 1) {
                    throw std::runtime_error("Invalid postfix expression: stack size mismatch at '='");
                }
                return std::to_string(stack.top());
            }
            // Attempt to convert the token to an integer and push it to the stack
            else {
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

    // Create a temporary linked list of nodes for subexpression evaluation
    while (temp != nullptr) {
        Node* newNode = new Node(temp->getValue(), false);
        if (isArithmeticOperator(temp->getValue())) {
            containsSubexpression = true;
            next = temp->getSibling();
            prev->setSibling(newNode);
            Node* equalNode = new Node("=", false); // Add an equal sign to ensure correct stack size in evaluatePostfix()
            newNode->setSibling(equalNode);
            break;
        }
        if (tempHead == nullptr) {
            tempHead = newNode;
            Node* secondaryNode = new Node(temp->getValue(), false); // Duplicate the value to handle evaluatePostfix() skipping the first
            tempHead->setSibling(secondaryNode);
            temp = temp->getSibling();
            prev = secondaryNode;
            continue;
        }
        if (prev != nullptr) {
            prev->setSibling(newNode);
            prev = newNode;
        } else {
            prev = newNode;
        }
        temp = temp->getSibling();
    }

    // Evaluate the postfix boolean expression
    while (current != nullptr) {
        std::string token = current->getValue();
        std::string token2 = "";
        if (containsSubexpression) {
            std::string token = evaluatePostfix(tempHead);
            token2 = token;
        }
        try {
            auto it = symbolTable.find(token);

            if (containsSubexpression) {
                stack.push(std::stoi(token2));
            }
            // If the token is a variable in the symbol table, push its value to the stack
            else if (it != symbolTable.end()) {
                Entry* entry = getEntry(it->first);
                if (entry != nullptr) {
                    std::string value = entry->getValue();
                    int result = 0;
                    if (value.length() > 1) {
                        result = std::stoi(value);
                    } else {
                        std::string hex = hexToInt(value);
                        if (hex == "-1") {
                            result = std::stoi(getEntry(value)->getValue());
                        } else {
                            result = std::stoi(hex);
                        }
                    }
                    stack.push(result);
                } else {
                    std::cerr << "Error: Null entry found for token: " << token << std::endl;
                }
            }
            // If the token is an operator, perform the boolean operation
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
            // Attempt to convert the token to an integer and push to the stack
            else {
                int value;
                if (token != "'") {
                    if (token.length() > 1) {
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
        if (containsSubexpression) {
            current = next;
            containsSubexpression = false;
        } else {
            current = current->getSibling();
        }
    }

    // Verify that only one value is left on the stack, representing the final result
    if (stack.size() != 1) {
      cursesBuffer << "Still on stack: ";
        while (!stack.empty()) {
          cursesBuffer << " " << stack.top();
            stack.pop();
        }
        throw std::runtime_error("Invalid boolean postfix expression: stack size mismatch");
    }

    return stack.top();  // Return the final boolean value 
}
Node* Interpreter::skipBlock(Node* node) {
    // Stack to keep track of scope levels (BEGIN_BLOCK and END_BLOCK)
    std::stack<int> scopeStack;
    Node* tempNode = nextNode(node);

    // Loop until all BEGIN_BLOCKs are matched with END_BLOCKs
    do {
        if (tempNode->getValue() == "BEGIN_BLOCK") {
            scopeStack.push(1); // Entering a new block, increase scope depth
        } else if (tempNode->getValue() == "END_BLOCK") {
            scopeStack.pop(); // Leaving a block, decrease scope depth
        }
        tempNode = nextNode(tempNode); // Move to the next node
    } while (scopeStack.size() != 0); // Continue until all nested blocks are closed

    return tempNode; // Return the node after the block
}

Node* Interpreter::executeBlock(Node* node) {
    // Stack to keep track of the scope levels
    std::stack<int> scopeStack;
    scopeStack.push(1); // Initialize stack with the current block
    Node* tempNode = nextNode(node);

    // Execute nodes inside the block until the scope ends
    while (scopeStack.size() != 0) {
        if (tempNode->getValue() == "BEGIN_BLOCK") {
            scopeStack.push(1); // Entering a nested block
        } else if (tempNode->getValue() == "END_BLOCK") {
            scopeStack.pop(); // Leaving a block
        } else {
            executeStatement(tempNode); // Execute non-block statements
        }
        tempNode = nextNode(tempNode); // Move to the next node
    }

    return tempNode; // Return the node after the block
}

std::string Interpreter::executeCall(Node* node) {
    curScope--; // Decrement the scope (going to function scope)
    std::stack<int> scopeStack;
    scopeStack.push(1); // Push the function scope onto the stack
    Node* tempNode = nextNode(node);

    // Execute nodes inside the function until the function scope ends
    while (scopeStack.size() != 0) {
        if (tempNode->getValue() == "BEGIN_BLOCK") {
            scopeStack.push(1); // Entering a new block, increase scope depth
        } else if (tempNode->getValue() == "END_BLOCK") {
            scopeStack.pop(); // Leaving a block, decrease scope depth
        } else if (tempNode->getValue() == "RETURN") {
            return handleReturn(tempNode); // Handle return statement, return value from the function
        } else {
            executeStatement(tempNode); // Execute other statements
        }

        // Handle IF conditions within the function
        if (tempNode->getValue() == "IF") {
            if (evaluateBooleanPostfix(tempNode)) {
                // Skip the IF block if the condition is true
                tempNode = skipBlock(tempNode);
                if (tempNode->getValue() == "ELSE") {
                    tempNode = skipBlock(tempNode); // Skip ELSE block as well
                }
            } else {
                tempNode = skipBlock(tempNode); // Skip the entire IF block
            }
        } else {
            tempNode = nextNode(tempNode); // Move to the next node
        }
    }

    curScope++; // Restore the scope level after function execution
    return ""; // Return empty if no return statement is reached
}

short Interpreter::numFunctions(Entry* entry) {
    Entry* temp = entry;
    short num = 0;

    // Traverse through all entries and count the functions/procedures
    while (temp != nullptr) {
        if (temp->getIDType() == "function" || temp->getIDType() == "procedure") {
            functionVector.push_back(temp->getIDName()); // Add function/procedure name to the vector
            num++;
        }
        temp = temp->getNext(); // Move to the next entry
    }

    return num; // Return the count of functions/procedures
}

std::string Interpreter::intToHex(int number) {
    // Handle the zero case specifically
    if (number == 0) {
        return "0";
    }

    std::string hexString;
    bool isNegative = number < 0; // Check if the number is negative
    unsigned int unsignedNumber;

    // Convert the number to its positive equivalent if negative
    if (isNegative) {
        unsignedNumber = -number;
    } else {
        unsignedNumber = number;
    }

    // Map for hexadecimal digits
    const char hexMap[] = "0123456789ABCDEF";

    // Convert the number to hexadecimal representation
    while (unsignedNumber > 0) {
        int remainder = unsignedNumber % 16;
        hexString = hexMap[remainder] + hexString; // Add the corresponding hex character to the string
        unsignedNumber /= 16;
    }

    // Add a negative sign if the original number was negative
    if (isNegative) {
        hexString = "-" + hexString;
    }

    return hexString; // Return the hexadecimal string representation
}

std::string Interpreter::hexToInt(std::string ch) {
    // Ensure the string is not empty
    if (ch.empty()) {
        throw std::invalid_argument("Input string is empty");
    }

    // Convert the first character of the string to a char
    char hexChar = ch[0];

    // Convert the character to its corresponding integer value
    if (hexChar >= '0' && hexChar <= '9') {
        return std::to_string(hexChar - '0'); // Converts '0'-'9' to 0-9
    } else if (hexChar >= 'A' && hexChar <= 'F') {
        return std::to_string(hexChar - 'A' + 10); // Converts 'A'-'F' to 10-15
    } else if (hexChar >= 'a' && hexChar <= 'f') {
        return std::to_string(hexChar - 'a' + 10); // Converts 'a'-'f' to 10-15
    } else {
        return "-1"; // Invalid character for hexadecimal, return "-1"
    }
}

Entry* Interpreter::getEntry(std::string name) {
    auto it = symbolTable.at(name);

    // Search for an entry with the matching scope or function/procedure type
    for (Entry* entry : it) {
        if (curScope == entry->getScope() || entry->getIDType() == "procedure" || entry->getIDType() == "function") {
            return entry;
        }
    }

    // If no entry is found, print an error and return nullptr
    std::cerr << "\nEntry with appropriate scope not found for " << name << "!";
    return nullptr;
}

void Interpreter::printSymbols() {
    // Iterate over all entries in the symbol table
    for (const auto& pair : symbolTable) {
        for (Entry* entry : pair.second) {
            // Print the identifier name, scope, and value of each entry
            cursesBuffer << entry->getIDName() << " Scope: " << entry->getScope() << " Value: " << entry->getValue() << std::endl;
        }
        cursesBuffer << std::endl;
    }
}
