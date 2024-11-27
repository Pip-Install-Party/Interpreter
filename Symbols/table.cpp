#include <iostream>
#include "table.h"
#include <iomanip>
#include <sstream>
#include <filesystem>
#include <fstream>


void Table::begin(Token* token){
    if (token->getType() == "IDENTIFIER") {
        Entry* tempEntry = nullptr;
        if (token->getSibling() != nullptr) {
            return build(token->getSibling(), token, tempEntry);
        } else if (token->getChild() != nullptr) {
            token = token->getChild();
            return build(token->getChild(), token, tempEntry);
        } else {
                return; 
        }
    } else {
         if (token->getSibling() != nullptr) {
                begin(token->getSibling());
            } else if (token->getChild() != nullptr) {
                begin(token->getChild());
            } else {
                return; 
        }
    }
}

void Table::build(Token* token, Token* prevToken, Entry* prevEntry) {
    if (prevToken == nullptr || token == nullptr) {
        return; // Ensure the tokens are valid before proceeding
    }
    Entry* entry = prevEntry;  // Start with the previous entry

    // Checks if function isn't paused, if prevToken is reserved, and token isn't {,},(,),[,]
    if (!pause && contains(prevToken->getValue()) && 
        token->getValue() != "{" && token->getValue() != "}" && 
        token->getValue() != "(" && token->getValue() != ")" && 
        token->getValue() != "[" && token->getValue() != "]") {
        
        pause = true;   // Pauses identifier creation until function parameters or array info is processed
        
        if (prevToken->getValue() == "procedure" || prevToken->getValue() == "function") {
            scope++; 
            inProcOrFuncScope = true;

            if (prevToken->getValue() == "procedure") { // If prevToken is a procedure
                exists(token, scope);   

                // New entry created for this procedure
                entry = new Entry(token->getValue(), prevToken->getValue(), "NOT APPLICABLE", false, 0, scope);
                
                if (prevEntry == nullptr){  //
                    head = entry;
                }
            } 
            else {  // If prevToken is a function
                exists(token, scope);

                // New entry created for this function
                entry = new Entry(token->getSibling()->getValue(), prevToken->getValue(), token->getValue(), false, 0, scope);
                
                if (prevEntry == nullptr){  // If prevEntry is null, set head to entry
                    head = entry;
                } 
                else {  
                    prevEntry->setNext(entry);  // Links entry to prevEntry  
                }
                // Recursive call to process the next sibling
                return build(token->getSibling()->getSibling(), token->getSibling(), entry);
            }
        } 
        else {  // prevToken is not a procedure or function
            if (inProcOrFuncScope){
                exists(token, scope);
                entry = new Entry(token->getValue(), "datatype", prevToken->getValue(), false, 0, scope);
            }
            else{
                exists(token, 0); 
                entry = new Entry(token->getValue(), "datatype", prevToken->getValue(), false, 0, 0);
            }
            
            
        
            if (prevEntry == nullptr){
                head = entry;
            }

            if (token->getSibling() != nullptr){    // If token has a sibling, process sibling token

                if (token->getSibling()->getValue() == "["){    // Indicates start of an array
                    
                    if (prevEntry != nullptr){    
                        prevEntry->setNext(entry);  // Links entry to prevEntry
                    }

                    return setArray(token->getSibling(), entry);    // Handles the array
                } 
                else if (token->getSibling()->getValue() == ","){   // Indicates an initalizer list for variables
                    
                    if (prevEntry != nullptr){
                        prevEntry->setNext(entry);  // Links entry to prevEntry
                    }

                    return handleInitList(prevToken->getValue(), token->getSibling()->getSibling(), entry); // Handles initializer list
                }
            }
        }

        if (prevEntry != nullptr){
            prevEntry->setNext(entry);  // Update the next pointer of prevEntry
        }

    }
    else if (pause == true){    // If true, we are processing parameters for a function or procedure
            if (contains(prevToken->getValue()) && token->getType() == "IDENTIFIER"){
                exists(token, scope);
                
                // Entry created for a parameter
                // Parameter Entry uses IDType spot to hold what function or procedure is holding the parameter
                Entry *newEntry = new Entry(token->getValue(), entry->getIDName(), prevToken->getValue(), false, 0, scope);
                
                // Processes an array parameter
                if (token->getSibling()->getValue() == "["){
                    token = token->getSibling()->getSibling();
                    newEntry->setIsArray();
                    newEntry->setArray(stoi(token->getValue()));
                }
                prevEntry->parameters.push_back(newEntry);
            }
    }

    // Checks for the ending of a function or procedure, or a '}'
    if (token->getValue() == "}") {
        pause = false; // Resume normal processing
        inProcOrFuncScope = false;
    }

    // Recursively traverse siblings or children
    if (token->getSibling() != nullptr){
        build(token->getSibling(), token, entry);  // Traverse sibling
    } 
    else if (token->getChild() != nullptr){
        pause = false;
        build(token->getChild(), token, entry);  // Traverse child
    }

}

// Checks if the token is a reserved word
bool Table::contains(std::string token){
    for (int i = 0; i < reserved.size(); i++){
        if (token == reserved.at(i)){
            return true;
        }
    }
    return false;
}

// Checks if the token exists in the symbol table
void Table::exists(Token* token, int scope){
    auto temp = head;
    while(temp != nullptr){
        if (temp->getIDName() == token->getValue() && !contains(token->getValue()) ){
            if (temp->getScope() == 0){
                std::cerr << "Error on line: " << token->getLineNumber() << " variable \"" << token->getValue() << "\" already defined globally\n";
                exit(1);
            }
            if (temp->getScope() == scope){
                std::cerr << "Error on line: " << token->getLineNumber() << " variable \"" << token->getValue() << "\" already defined locally\n";
                exit(1);
            }
        }
        for (int i = 0; i < temp->parameters.size(); i++){
            if (temp->parameters.at(i)->getIDName() == token->getValue() && !contains(token->getValue()) ){
                if (temp->parameters.at(i)->getScope() == 0){
                    std::cerr << "Error on line: " << token->getLineNumber() << " variable \"" << token->getValue() << "\" already defined globally\n";
                    exit(1);
                }
                if (temp->parameters.at(i)->getScope() == scope){
                    std::cerr << "Error on line: " << token->getLineNumber() << " variable \"" << token->getValue() << "\" already defined locally\n";
                    exit(1);
                }
            }
        }
        temp = temp->getNext();
    }
}


void Table::setArray(Token* token, Entry* entry){
    entry->setArray(std::stoi(token->getSibling()->getValue()));
    build(token->getSibling()->getSibling(), token, entry); 
}


void Table::handleInitList(std::string type, Token* token, Entry* prevEntry){
    Entry *entry;
    if(inProcOrFuncScope){
        entry = new Entry(token->getValue(), "datatype", type, false, 0, scope);
    }
    else{
        entry = new Entry(token->getValue(), "datatype", type, false, 0, 0);
    }
    
    prevEntry->setNext(entry);  // Update the next pointer of prevEntry
    
    if (token->getSibling()->getValue() == ";"){
        return build(token->getSibling()->getChild(), token->getSibling(), entry);
    }

    return handleInitList(type, token->getSibling()->getSibling(), entry);
}

void Table::printTable(std::ofstream& symbolOutput){
    const int colonWidth = 25;
    //make temp head pointer
    Entry* tempHead = this->head;

    while(tempHead != nullptr){

        symbolOutput << std::setw(colonWidth) << std::right << "IDENTIFIER_NAME: " << tempHead->getIDName() << std::endl;
        symbolOutput << std::setw(colonWidth) << std::right << "IDENTIFIER_TYPE: " << tempHead->getIDType() << std::endl;
        symbolOutput << std::setw(colonWidth) << std::right << "DATATYPE: " << tempHead->getDType() << std::endl;
        symbolOutput<< std::setw(colonWidth) << std::right << "DATATYPE_IS_ARRAY: " << (tempHead->getIsArray() ? "yes" : "no") << std::endl;
        symbolOutput << std::setw(colonWidth) << std::right << "DATATYPE_ARRAY_SIZE: " << tempHead->getArraySize() << std::endl;
        symbolOutput << std::setw(colonWidth) << std::right << "SCOPE: " << tempHead->getScope() << std::endl;
        symbolOutput << std::endl; 
        
        tempHead = tempHead->getNext();
    }
}

// Print function for any Entries that have parameters
void Table::printParameters(std::ofstream& symbolOutput){

    // make local head pointer
    Entry* tempHead = this->head;
    const int colonWidth = 25;

    // Cycle through all entries in the table
    while(tempHead != nullptr){
        if (tempHead->parameters.size() > 0){  // If current entry has parameters...
            symbolOutput << std::setw(colonWidth) << std::right << "PARAMETER LIST FOR: " << tempHead->getIDName() << std::endl;
            for (int i = 0; i < tempHead->parameters.size(); i++) { // Print each parameter
                symbolOutput << std::setw(colonWidth) << std::right << "IDENTIFIER_NAME: " << tempHead->parameters.at(i)->getIDName() << std::endl;
                symbolOutput << std::setw(colonWidth) << std::right << "DATATYPE: " << tempHead->parameters.at(i)->getDType() << std::endl;
                symbolOutput << std::setw(colonWidth) << std::right << "DATATYPE_IS_ARRAY: " << (tempHead->parameters.at(i)->getIsArray() ? "yes" : "no") << std::endl;
                symbolOutput << std::setw(colonWidth) << std::right << "DATATYPE_ARRAY_SIZE: " << tempHead->parameters.at(i)->getArraySize() << std::endl;
                symbolOutput << std::setw(colonWidth) << std::right << "SCOPE: " << tempHead->parameters.at(i)->getScope() << std::endl;
                symbolOutput << std::endl;
            }
        }
        tempHead = tempHead->getNext();
    }
}