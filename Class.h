//
// Created by brad on 5/2/16.
//

#ifndef CLASS_H
#define CLASS_H


#include "Entry.h"
#include <map>

using namespace std;

class Class : public Entry {
public:

    map<std::string, Entry *> classTable;

    Class() { }

    AstNode::AstNodeKind getKind() {
        return kkind;
    }

    string toString() {
        cout << "-->Class:: Name: " << name << endl;
        for (const auto &i:classTable) {
            i.second->toString();
        }
        return "";
    }

    Entry* findEntry(string n){
        if(classTable.find(n)!=classTable.end()){
            return classTable[n];
        }else{
            return NULL;
        }
    }

private:
    const static AstNode::AstNodeKind kkind = AstNode::DCLASS;
};

#endif //TJC_PART_2_CLASS_H