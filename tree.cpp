#include "tree.h"
#include <stack>

// Prints the abstract syntax tree to the provided output stream
void Tree::printTree(Token* head, Token* prevToken){
    bool ignore = true;
    if (head == nullptr){
        return;
    }
    if (head->getValue() == "{") {
        ignore = false;
        std::cout << "BEGIN BLOCK";
    } else if (head->getValue() == "}") {
        ignore = false;
        std::cout << "END BLOCK";
    } else if(head->getValue() == "else"){
        ignore = false;
        std::cout << "ELSE";
    } else if(head->getValue() == "return"){
        ignore = false;
        std::cout << "RETURN";
        head = head->getSibling();
    } else if (head->getValue() == "procedure" || head->getValue() == "function") {
        std::cout << "DECLARATION\n|\n|\n|\n|\nv\n";
        while(head->getSibling() != nullptr) {
            head = head->getSibling();
        }
    } else if (head->getValue() == "if"){
        ignore = false;
        std::cout << "IF ----> ";
        // This needs to break out to handleAssignment();
        head = head->getSibling();
        head = handleAssignment(head); 
        prevToken = nullptr;
        
    } else if (head->getValue() == "printf"){
        ignore = true;
        std::cout << "PRINTF";
        while (head->getSibling() != nullptr) {
            head = head->getSibling();
            if (head->getType() != "L_PAREN" && head->getType() != "R_PAREN" && head->getType() != "DOUBLE_QUOTE" && head->getType() != "COMMA" && head->getType() != "SEMICOLON") {
                std::cout << " ----> ";
                std::cout << head->getValue();
            }
        }
        std::cout << "\n|\n|\n|\n|\nv\n";
        prevToken = nullptr;
        
    } else if (head->getValue() == "for"){
        short forCount = 0;
        ignore = false;
        while (forCount < 3){
            std::cout << "FOR EXPRESSION " << forCount+1 << " ----> ";
            head = head->getSibling();
            head = handleAssignment(head); 
            prevToken = nullptr;
            if (forCount+1 < 3) {
                std::cout << "\n|\n|\n|\n|\nv\n "; 
            }
            forCount++;
        }
       
    } else if (contains(varTypes, prevToken->getValue())) {
        ignore = true; // this was false but i think should be true 
        std::cout << "DECLARATION";
        if (head->getSibling() != nullptr && head->getSibling()->getValue() == ",") {
            while(head->getSibling() != nullptr) {
                head = head->getSibling();
                if (head->getType() == "IDENTIFIER") {
                    std::cout << "\n|\n|\n|\n|\nv\nDECLARATION";
                }
            }
            std::cout << "\n|\n|\n|\n|\nv\n";
        }
    } else if (head->getType() == "IDENTIFIER") {  
        if (head->getSibling() != nullptr && head->getSibling()->getValue() == "=") {
            ignore = false;
            std::cout << "ASSIGNMENT" << " ----> ";
            // This needs to break out to handleAssignment();
            head = handleAssignment(head); 
            prevToken = nullptr;
        } 
    } else if (head->getSibling() == nullptr && head->getChild() != nullptr && head->getValue() == ";"){
        std::cout << "\n|\n|\n|\n|\nv\n";
    }
    if (head->getSibling() != nullptr) {
        if (!ignore){
            std::cout << " ----> ";
            std::cout << head->getValue();
        }
        return printTree(head->getSibling(), head);
    } else if (head->getChild() != nullptr) {
        if (!ignore) {
            std::cout << "\n|\n|\n|\n|\nv\n";
        }
        return printTree(head->getChild(), head);
    } 
    return;
}

bool Tree::contains(const std::vector<std::string> reserved, std::string type){
    for (const auto& word : reserved) {
        if (type == word) {
            return true;
        }
    }
    return false;
}

// Function that handles functions and single normal and single array parameters
Token* Tree::handleFunction(Token *head, std::vector<Token*>& equationAsVec, bool &isFunctionCall) {
    head = head->getSibling(); // Should be getting L_PAREN of the function
    equationAsVec.push_back(head);
    isFunctionCall = true;  // For infixToPostfix to know we want to eventually output () or []

    while (head->getValue() != ")") {
        head = head->getSibling();
        // Array declaration check here, []
        if (head->getType() == "IDENTIFIER" && head->getSibling() != nullptr && head->getSibling()->getValue() == "[") {
            equationAsVec.push_back(head); // Identifier pushed back
            head = head->getSibling(); 
            
            equationAsVec.push_back(head); // Starting '[' pushed back
            head = head->getSibling();
            
            equationAsVec.push_back(head); // Array size or index here
            head = head->getSibling();
            
            equationAsVec.push_back(head); // Ending ']' pusehd back
        } 
        else if (head->getValue() != ")" && head->getValue() != "(") {
            equationAsVec.push_back(head); // Normal function parameter added ex.func(param)
        }
    }
    equationAsVec.push_back(head); 
    return head->getSibling(); 
}

Token* Tree::handleAssignment(Token* head) {
    std::vector<Token*> equationAsVec;
    bool isFunctionCall = false;
    
    if (head->getValue() != "(") {
        equationAsVec.push_back(head);
    }

    head = head->getSibling();

    while(head->getValue() != ";" && (contains(equationOperators, head->getValue()) || head->getType() == "IDENTIFIER" || head->getType() == "INTEGER" || head->getType() == "CHARACTER" || head->getType() == "STRING" || head->getType() == "DOUBLE_QUOTE")) {
        // Check for function call (identifier with a L_PAREN)
        if (head->getType() == "IDENTIFIER" && head->getSibling() != nullptr && head->getSibling()->getValue() == "(") {
            // Call to handleFunction since we encountered an identifier with a L_PAREN
            equationAsVec.push_back(head); 
            head = handleFunction(head, equationAsVec, isFunctionCall);
        }
        else {
            equationAsVec.push_back(head); 
        }

        if (head ->getSibling() != nullptr) {
            head = head->getSibling();
        } 
        else {
            break;
        }
    } 
    
    // Convert infix to postfix
    std::vector<Token*> postFix = infixToPostfix(equationAsVec, isFunctionCall);

    //std::cout << "Size<: " << postFix.size() << std::endl;
    for (int i = 0; i < postFix.size(); i++) {
        std::string tokenValue = postFix.at(i)->getValue();
       // std::cout << "\ncurr token = " << tokenValue << " next token = " << postFix.at(i+1) << std::endl;

        // Print token value directly and mark that a space is needed
        if (!tokenValue.empty()) {
            std::cout << tokenValue;
            if (i != postFix.size() - 1) {
                std::cout << " ----> ";
            }
        }
    }

    return head;
}

// Function to get precedence of operators
int Tree::getPrecedence(std::string op) {
    if (op == "+" || op == "-" || op == ">=" || op == "<=" || op == "==" || op == ">" || op == "<" || op == "==" || op == "&&" || op == "!" || op == "||") return 1;
    if (op == "*" || op == "/") return 2;
    return 0;
}

// Function to check if a character is an operator
bool Tree::isOperator(std::string c) {
    return c == "+" || c == "-" || c == "*" || c == "/" || c == "=";
}

std::vector<Token*> Tree::infixToPostfix(const std::vector<Token*> infix, bool isFunctionCall) {
    std::stack<Token*> operators;
    std::vector<Token*> postfix;

    // std::cout << "PRINTING: ";
    //  for (Token* t : infix) {
    //     std::cout << t->getValue();
    //  }

    for (Token* t : infix) {
        std::string tokenType = t->getType();
        std::string tokenValue = t->getValue();
        
        // If it's an operand, add it to the postfix output
        if (tokenType == "INTEGER" || tokenType == "STRING" || tokenType == "CHARACTER" || tokenType == "IDENTIFIER" || tokenType == "SINGLE_QUOTE" || tokenType == "DOUBLE_QUOTE") {
            postfix.push_back(t);
        }
        // If it's a left parenthesis, push it onto the stack
        else if (tokenType == "L_PAREN") {
            if (isFunctionCall) {
                postfix.push_back(t);  // Push '(' if it's part of a function call to output later
            } else {
                operators.push(t);  // Processing a subexpression here (not a function)
            }
        }
        // If it's a right parenthesis, pop until the left parenthesis
        else if (tokenType == "R_PAREN") {
            if (isFunctionCall) {
                postfix.push_back(t);  // Push ')' if it's part of a function call to output later
            } else {
                // Processing it as a regular subexpression, pop operators until '(' is found
                while (!operators.empty() && operators.top()->getValue() != "(") {
                    postfix.push_back(operators.top());
                    operators.pop();
                }
                if (!operators.empty()) {
                    operators.pop(); 
                }
            }
        }
        // Add array brackets here if it is a function call, if not, it is a normal operator.
        else if (tokenType == "L_BRACKET" || tokenType == "R_BRACKET") {
            if (isFunctionCall) {
                postfix.push_back(t);  
            } else {
                operators.push(t);  
            }
        }
        // If it's an operator
        else if (isOperator(tokenValue) || tokenType == "GT_EQUAL" || tokenType == "LT_EQUAL" || tokenType == "GT" || tokenType == "LT" || tokenType == "BOOLEAN_EQUAL" || tokenType == "BOOLEAN_AND" || tokenType == "BOOLEAN_NOT") { // can code this to check for all explicit tokens like prev if checks above... can add other tokens to operators too 
            // Pop all operators with higher or equal precedence from the stack
            while (!operators.empty() && operators.top()->getType() != "L_PAREN" &&
                    getPrecedence(operators.top()->getValue()) >= getPrecedence(tokenValue)) {
                postfix.push_back(operators.top());
                operators.pop();
            }
            // Push the current operator onto the stack
            operators.push(t);
        }
    }

    // Pop any remaining operators in the stack
    while (!operators.empty()) {
        postfix.push_back(operators.top());
        operators.pop();
    }

    return postfix;
}
