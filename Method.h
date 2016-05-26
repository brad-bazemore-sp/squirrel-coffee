//
// Created by brad on 5/2/16.
//

#ifndef METHOD_H
#define METHOD_H


#include "Entry.h"
#include <map>
#include <vector>

using namespace std;

class Method : public Entry {
public:

    map<std::string, Entry*> variableTable;
    vector<Entry*> parameterTable;

    Method() { }

    AstNode::AstNodeKind getKind() {
        return kkind;
    }

    int getReturnType() {
        return returnType;
    }

    void setReturnType(int in) {
        returnType = in;
    }

    string toString() {
        cout << "---->Method:: Name: " << name;
        cout << " Return Type: " << returnType << endl;
        for (int i = 0; i < parameterTable.size(); i++) {
            parameterTable[i]->toString();
        }
        for (const auto &i:variableTable) {
            i.second->toString();
        }
        return "";
    }

    Entry* findEntry(string n){
        if(variableTable.find(n)!=variableTable.end()){
            return variableTable[n];
        }else{
            for(int i=0;i<parameterTable.size();i++){
                if(parameterTable[i]->name==n){
                    return parameterTable[i];
                }
            }
            return NULL;
        }
    }

private:
    const static AstNode::AstNodeKind kkind = AstNode::DMETHOD;
    int returnType;
};

#endif //TJC_PART_2_METHOD_H
