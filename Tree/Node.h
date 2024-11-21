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
    Node(std::string id, bool isReserved) { name = id, reserved = isReserved; }
    bool isReserved() { return reserved; }
    Node* getSibling() { return rSibling; }
    Node* getChild() { return lChild; }
    void setSibling( Node* sibling ) { rSibling = sibling; }
    void setChild( Node* child ) { lChild = child; }
    ~Node(); 
};
#endif // NODE_H