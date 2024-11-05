#include "tree.h"

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
        
    }else if (contains(varTypes, prevToken->getValue())) {
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
    std::string equationAsString;
    equationAsString += head->getValue();
    head = head->getSibling();

    while(contains(equationOperators, head->getValue()) || head->getType() == "IDENTIFIER" || head->getType() == "INTEGER" || head->getType() == "CHARACTER") {
        equationAsString += head->getValue();
        if (head ->getSibling() != nullptr) {
            head = head->getSibling();
        } else {
            break;
        }
    } 
        //head = head->getSibling(); // Remove ?

    for (char ch : infixToPostfix(equationAsString)) {
     //   std::cout << ch << " ----> ";
    }
    std::cout << "Postfix Expression";
    return head;
}


// Function to get precedence of operators
int Tree::getPrecedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// Function to check if a character is an operator
bool Tree::isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

// Function to convert infix expression to postfix
std::string Tree::infixToPostfix(const std::string& infix) {
    std::stack<char> operators;
    std::string postfix;

    for (char c : infix) {
        // If the character is an operand, add it to postfix output
        if (std::isdigit(c) || std::isalpha(c)) {
            postfix += c;
        }
        // If the character is '(', push it onto the stack
        else if (c == '(') {
            operators.push(c);
        }
        // If the character is ')', pop and add operators until '(' is found
        else if (c == ')') {
            while (!operators.empty() && operators.top() != '(') {
                postfix += operators.top();
                operators.pop();
            }
            operators.pop(); // Pop '(' from the stack
        }
        // If the character is an operator
        else if (isOperator(c)) {
            while (!operators.empty() && getPrecedence(operators.top()) >= getPrecedence(c)) {
                postfix += operators.top();
                operators.pop();
            }
            operators.push(c);
        }
    }

    // Pop all remaining operators in the stack
    while (!operators.empty()) {
        postfix += operators.top();
        operators.pop();
    }

    return postfix;
}