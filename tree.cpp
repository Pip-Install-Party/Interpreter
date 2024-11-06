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
        
    } else if (contains(varTypes, prevToken->getValue())) {
        ignore = false;
        std::cout << "DECLARATION";
        if (head->getSibling() != nullptr && head->getSibling()->getValue() == ",") {
            while(head->getSibling() != nullptr) {
                head = head->getSibling();
                if (head->getType() == "IDENTIFIER") {
                    std::cout << "\n|\n|\n|\n|\nv\nDECLARATION";
                }
            }
        }
    } else if (head->getType() == "IDENTIFIER") {
        if (head->getSibling() != nullptr && head->getSibling()->getValue() == "=") {
            ignore = false;
            std::cout << "ASSIGNMENT" << " ----> ";
            // This needs to break out to handleAssignment();
            head = handleAssignment(head); 
            prevToken = nullptr;
        } 
    } 
    if (head->getSibling() != nullptr) {
        if (!ignore){
            std::cout << " ----> ";
            std::cout << head->getValue();
        }
        return printTree(head->getSibling(), head);
    } else if (head->getChild() != nullptr) {
        if (!ignore) {
            std::cout << "\n|\n|\n|\n|\nv\n ";
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

Token* Tree::handleAssignment(Token* head) {
    std::vector<Token*> equationAsVec;
    equationAsVec.push_back(head);
    head = head->getSibling();

    while(contains(equationOperators, head->getValue()) || head->getType() == "IDENTIFIER" || head->getType() == "INTEGER" || head->getType() == "CHARACTER") {
        equationAsVec.push_back(head); 
        if (head ->getSibling() != nullptr) {
            head = head->getSibling();
        } else {
            break;
        }
    } 
    
    // Convert infix to postfix
    for (Token* token : infixToPostfix(equationAsVec)) {
        std::string tokenValue = token->getValue();
        //std::cout << "tokenVal = " << tokenValue << std::endl; // debug method *****
        // Print token value directly and mark that a space is needed
        if (!tokenValue.empty()) {
            std::cout << tokenValue;
            std::cout << " ----> ";
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

std::vector<Token*> Tree::infixToPostfix(const std::vector<Token*> infix) {
    std::stack<Token*> operators;
    std::vector<Token*> postfix;

    for (Token* t : infix) {
        std::string tokenType = t->getType();
        std::string tokenValue = t->getValue();
        
        // If it's an operand, add it to the postfix output
        if (tokenType == "INTEGER" || tokenType == "STRING" || tokenType == "CHARACTER" || tokenType == "IDENTIFIER") {
            postfix.push_back(t);
        }
        // If it's a left parenthesis, push it onto the stack
        else if (tokenType == "L_PAREN") {
            operators.push(t);
        }
        // If it's a right parenthesis, pop until the left parenthesis
        else if (tokenType == "R_PAREN") {
            while (!operators.empty() && operators.top()->getValue() != "(") {
                postfix.push_back(operators.top());
                operators.pop();
            }
            if (!operators.empty()) { // Pop the left parenthesis
                operators.pop();
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
