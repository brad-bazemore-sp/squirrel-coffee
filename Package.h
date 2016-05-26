//
// Created by brad on 5/2/16.
//

#ifndef PACKAGE_H
#define PACKAGE_H


#include "Entry.h"

using namespace std;

class Package : public Entry {
public:

    map<std::string, Entry *> packageTable;

    Package() { }

    AstNode::AstNodeKind getKind() {
        return kkind;
    }

    string toString() {
        cout << ">Package:: Name: " << name;
        cout << endl;
        for (auto const &i:packageTable) {
            i.second->toString();
        }
        return "";
    }

    Entry* findEntry(string n){
        return NULL;
    }

private:
    const static AstNode::AstNodeKind kkind = AstNode::DPACKAGE;
};

#endif //TJC_PART_2_PACKAGE_H