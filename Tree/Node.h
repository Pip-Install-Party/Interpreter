

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
    // Constructor: Initializes the node with its name and reserved status.
    Node(std::string id, bool isReserved) { 
        this->name = id; 
        this->reserved = isReserved; 
    }

    // Checks if the node represents a reserved keyword or type.
    bool isReserved() const { 
        return this->reserved; 
    }

    // Retrieves the pointer to the right sibling of this node.
    Node* getSibling() const { 
        return this->rSibling; 
    }

    // Retrieves the pointer to the left child of this node.
    Node* getChild() const { 
        return this->lChild; 
    }

    // Sets the right sibling pointer for this node.
    void setSibling(Node* sibling) { 
        this->rSibling = sibling; 
    }

    // Sets the left child pointer for this node.
    void setChild(Node* child) { 
        this->lChild = child; 
    }

    // Retrieves the name or value associated with this node.
    std::string getValue() const { 
        return name; 
    }

    // Destructor: Frees resources associated with the node.
    ~Node(); 
};

#endif // NODE_H