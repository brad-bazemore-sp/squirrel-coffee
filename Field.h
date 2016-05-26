//
// Created by brad on 5/2/16.
//
#ifndef FIELD_H
#define FIELD_H


#include "Entry.h"

using namespace std;

class Field : public Entry {
public:

    Field() { }

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
        cout << "---->Field:: Name: " << name;
        cout << " Type: " << type;
        cout << " Value: " << value;
        cout << endl;
        return "";
    }

    Entry* findEntry(string n){
        return NULL;
    }

private:
    const static AstNode::AstNodeKind kkind = AstNode::DFIELD;
    int type;
    std::string value;
};

#endif