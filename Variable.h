//
// Created by brad on 5/2/16.
//

#ifndef VARIABLE_H
#define VARIABLE_H

#include "Entry.h"

using namespace std;

class Variable : public Entry {
public:

    Variable() { }

    AstNode::AstNodeKind getKind() {
        return kkind;
    }

    int getType() {
        return type;
    }

    void setType(int in) {
        type = in;
    }

    std::string getValue() {
        return value;
    }

    void setValue(std::string in) {
        value = in;
    }

    string toString() {
        cout << "------>Variable:: Name: " << name;
        cout << " Type: " << type;
        cout << " Value: " << value << endl;
        return "";
    }

    Entry* findEntry(string n){
        return NULL;
    }

    void setIndex(int in){
        index=in;
    }

    int getIndex(){
        return index;
    }

private:
    const static AstNode::AstNodeKind kkind = AstNode::DVARIABLE;
    int type;
    std::string value;
    int index;
};

#endif