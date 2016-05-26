//
// Created by brad on 5/2/16.
//

#ifndef ENTRY_H
#define ENTRY_H

#include <map>
#include "../ast/AstNode.h"
#include <string>

using namespace std;

class Entry {
public:
    Entry() { }

    virtual AstNode::AstNodeKind getKind() = 0;

    virtual string toString() = 0;

    virtual Entry* findEntry(string n)=0;

    std::string name;
};


#endif