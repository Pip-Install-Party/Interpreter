#include "tree.h"

// Prints the abstract syntax tree to the provided output stream
void Tree::printTree(Token* head){
    if (head == nullptr){
        return;
    }
    if (head->getValue() == "{") {
        std::cout << "BEGIN BLOCK";
    } else if (head->getValue() == "}") {
        std::cout << "END BLOCK";
    } else if (head->getValue() == "procedure" || head->getValue() == "function") {
        std::cout << "DECLARATION";
    } else if (contains(head->getValue())) {
        std::cout << "DECLARATION";
    } else if (head->getType() == "IDENTIFIER") {
        if (head->getSibling() != nullptr && head->getSibling()->getValue() == "=") {
            std::cout << "ASSIGNMENT" << " ----> " << head->getValue();
            // This needs to break out to handleAssignment();
            // head = handleAssignment(); 
        }
    }
    if (head->getSibling() != nullptr) {
        head = head->getSibling();
        std::cout << " ----> ";
    } else if (head->getChild() != nullptr) {
        head = head->getChild();
        std::cout << "\n|\n|\n|\n|\nv ";
    }
    return printTree(head);
}

bool contains(std::string type){
    for (const auto& reserved : varTypes) {
        if (type == reserved) {
            return true;
        }
    }
    return false;
}