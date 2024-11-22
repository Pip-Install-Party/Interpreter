// The Token class defines the attributes of a Token, including its type, contents, and line number.
#ifndef NODE_H
#define NODE_H

#include <iostream>
class Node 
{
private: 
    Node* lChild = nullptr;
    Node* rSibling = nullptr;
    std::string name = "";
    bool reserved = false;
public:
    Node(std::string id, bool isReserved) { this->name = id, this->reserved = isReserved; }
    bool isReserved() { return this->reserved; }
    Node* getSibling() { return this->rSibling; }
    Node* getChild() { return this->lChild; }
    void setSibling( Node* sibling ) { this->rSibling = sibling; }
    void setChild( Node* child ) { this->lChild = child; }
    ~Node(); 
};
#endif // NODE_H