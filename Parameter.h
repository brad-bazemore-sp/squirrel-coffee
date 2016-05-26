//
// Created by brad on 5/2/16.
//

#ifndef PARAMETER_H
#define PARAMETER_H


#include "Entry.h"

using namespace std;

class Parameter : public Entry {
public:

    Parameter() { }

    AstNode::AstNodeKind getKind() {
        return kkind;
    }

    int getType() {
        return type;
    }

    void setType(int in) {
        type = in;
    }

    string toString() {
        cout << "------>Parameter:: Name:" << name;
        cout << " Type: " << type << endl;
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
    const static AstNode::AstNodeKind kkind = AstNode::DPARAMETER;
    int type;
    int index;
};

#endif //TJC_PART_2_PARAMETER_H