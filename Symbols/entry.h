#ifndef ENTRY_H
#define ENTRY_H

#include <string>
#include <vector>

class Entry {
    private:
    std::string idName = "";
    std::string idType = "";
    std::string dType = "";
    std::string val = "0";
    bool isArray = false;
    int arraySize = 0;
    short scope;

    Entry* nextEntry = nullptr;
    public:
    Entry(std::string idName, std::string idType, std::string dType, bool isArray, int arraySize, short scope)
        : idName(idName), idType(idType), dType(dType), isArray(isArray), arraySize(arraySize), scope(scope){}; 
    
    void setNext(Entry* entry) { nextEntry = entry; }
    std::string getIDName(){ return idName; }
    std::string getIDType(){ return idType; }
    std::string getDType() { return dType; }
    std::string getValue() { return val; }
    void setValue(std::string value) { val = value; }
    void setIsArray() { this->isArray = true; };
    bool getIsArray() { return isArray; }
    int getArraySize() { return arraySize; }
    short getScope() { return scope; }
    Entry* getNext() { return nextEntry; }
    void setArray(int size){ isArray = true; arraySize = size; }
    std::vector<Entry*> parameters;
    std::vector<Entry*> getParameterList() { return parameters; }
};

#endif // ENTRY_H