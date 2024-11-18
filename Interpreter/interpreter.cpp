#include "interpreter.h"
#include <iostream>

Interpreter::Interpreter(){
    symbolTable = nullptr;
    programCounter = nullptr;
}

void Interpreter::begin(/*pass AST head here*/){
    programCounter = /*AST head*/;

    while(programCounter != nullptr){
        executeStatement(programCounter);   // pass to executeStatement for further processing
        programCounter = nextStatement(programCounter); // need logic to get next statement
    } 

}

void Interpreter::nextStatement(/*pass AST node here*/){
    // implement logic to find the next statement here
    // this should traverse right and then down until another declaration, assignment, iteration, etc. is found
    // return next statement node
}

// processes a single AST node
void Interpreter::executeStatement(/*pass current AST node here*/){
    if(curNode->getValue() == "DECLARATION"){
        handleDeclaration(curNode);
    }
    else if(curNode->getValue() == "ASSIGNMENT"){
        handleAssignment(curNode);
    }
    else if(curNode->getValue() == "ITERATION"){
        handleIteration(curNode);
    }
    else if(curNode->getValue() == "SELECTION"){
        handleSelection(curNode);
    }
    else if(curNode->getValue() == "PRINTF"){
        handlePrintf(curNode);
    }
    else if(curNode->getValue() == "RETURN"){
        handleReturn(curNode);
    }
    else if(curNode->getValue() == "FUNCTION"){
        handleFunction(curNode);
    }
    else if(curNode->getValue() == "PROCEDURE"){
        handleProcedure(curNode);        
    }
    /* ... add other types here */
    else{
        std::cerr << "Statement type not recognized" << std::endl;
        exit(3);
    }
}

void Interpreter::handleDeclaration(){

}
void Interpreter::handleAssignment(){
}

void Interpreter::handleIteration(){
}

void Interpreter::handleSelection(){
}

void Interpreter::handlePrintf(){
}

void Interpreter::handleReturn(){
}

void Interpreter::handleFunction(){
}

void Interpreter::handleProcedure(){
}