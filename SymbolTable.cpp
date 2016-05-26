//
// Created by brad on 5/1/16.
//
#include <iostream>
#include "SymbolTable.h"
//TODO check types match
using namespace std;

map<std::string, Entry *> symbolTable;

int index = 0;

void SymbolTable::visit(ClassDeclaration *aDeclNode) {
    if (aDeclNode == NULL) {
        throw SymbolTableException("Symbol Table Exception:: ClassDeclaration: NULL argument");
    }
    Class *temp = new Class;
    temp->name = aDeclNode->getName();
    dynamic_cast<Package *>(scope.back())->packageTable[temp->name] = temp;
    scope.push_back(temp);
    if (aDeclNode->getMembers() == NULL) {
        cout << "Nothing in this class to put on symbol table, just going to leave this here." << endl;
    } else {
        for (int i = 0; i < aDeclNode->getMembers()->size(); i++) {
            if (aDeclNode->getMember(i) == NULL) {
                throw SymbolTableException("Symbol Table Exception:: DCLASS: member is NULL");
            }
            aDeclNode->getMember(i)->accept(this);
        }
    }

    //cheat for simple io
    Method *readInt = new Method();
    readInt->name = "readInt";
    readInt->setReturnType(AstNode::TINT);
    temp->classTable[readInt->name] = readInt;

    Method *readFloat = new Method();
    readFloat->name = "readFloat";
    readFloat->setReturnType(AstNode::TFLOAT);
    temp->classTable[readFloat->name] = readFloat;

    Method *println = new Method();
    println->name = "println";
    println->setReturnType(AstNode::TVOID);
    temp->classTable[println->name] = println;

    Method *printInt = new Method();
    printInt->name = "printInt";
    printInt->setReturnType(AstNode::TVOID);
    temp->classTable[printInt->name] = printInt;

    Parameter *printIntPar = new Parameter();
    printIntPar->name = "printIntPar";
    printIntPar->setIndex(0);
    printIntPar->setType(AstNode::TINT);
    printInt->parameterTable.push_back(printIntPar);

    Method *printFloat = new Method();
    printFloat->name = "printFloat";
    printFloat->setReturnType(AstNode::TVOID);
    temp->classTable[printFloat->name] = printFloat;

    Parameter *printFloatPar = new Parameter();
    printFloatPar->name = "printFloatPar";
    printFloatPar->setType(AstNode::TFLOAT);
    printFloatPar->setIndex(0);
    printFloat->parameterTable.push_back(printFloatPar);

    Method *printString = new Method();
    printString->name = "printString";
    printString->setReturnType(AstNode::TVOID);
    temp->classTable[printString->name] = printString;

    Parameter *printStringPar = new Parameter();
    printStringPar->name = "printStringPar";
    printStringPar->setIndex(0);
    printStringPar->setType(AstNode::TSTRING);
    printString->parameterTable.push_back(printStringPar);

    scope.pop_back();
}

void SymbolTable::visit(FieldDeclaration *aDeclNode) {
    if (aDeclNode == NULL) {
        throw SymbolTableException("Symbol Table Exception:: FieldDeclaration: NULL argument");
    }

    Field *temp = new Field();
    temp->name = aDeclNode->getName();
    temp->setType(aDeclNode->getType());
    temp->setValue(aDeclNode->getInitValue());
    dynamic_cast<Class *>(scope.back())->classTable[temp->name] = temp;
    aDeclNode->setEntry(temp);
}

void SymbolTable::visit(MethodDeclaration *aDeclNode) {
    if (aDeclNode == NULL) {
        throw AstException("AstPrinter::visit: MethodDeclaration: NULL argument");
    }

    if (aDeclNode->getBody() == NULL) {
        throw AstException("AstPrinter::visit: MethodDeclaration: body is NULL");
    }

    Method *temp = new Method();
    temp->name = aDeclNode->getName();
    temp->setReturnType(aDeclNode->getRetType());
    dynamic_cast<Class *>(scope.back())->classTable[temp->name] = temp;
    aDeclNode->setEntry(temp);
    scope.push_back(temp);

    if (aDeclNode->getParameters() != NULL) {
        for (int i = 0; i < aDeclNode->getParameters()->size(); i++) {
            aDeclNode->getParameters()->at(i)->accept(this);
            index++;
        }
    }

    if (aDeclNode->getVariables() != NULL) {
        for (int i = 0; i < aDeclNode->getVariables()->size(); i++) {
            aDeclNode->getVariables()->at(i)->accept(this);
            index++;
        }
    }
    index = 0;
    scope.pop_back();
}

void SymbolTable::visit(ParameterDeclaration *aDeclNode) {
    if (aDeclNode == NULL) {
        throw AstException("AstPrinter::visit: ParameterDeclaration: NULL argument");
    }
    Parameter *temp = new Parameter();
    if (static_cast<Class *>(scope[scope.size() - 1])->classTable.find(aDeclNode->getName()) !=
        static_cast<Class *>(scope[scope.size() - 1])->classTable.end()) {
        throw SymbolTableException("Symbol Table Exception:: VariableDeclaration: duplicate");
    }
    temp->name = aDeclNode->getName();
    temp->setType(aDeclNode->getType());
    temp->setIndex(index);
    static_cast<Method *>(scope.back())->parameterTable.push_back(temp);
    aDeclNode->setEntry(temp);
}

void SymbolTable::visit(VariableDeclaration *aDeclNode) {
    if (aDeclNode == NULL) {
        throw AstException("AstPrinter::visit: VariableDeclaration: NULL argument");
    }
    if (static_cast<Method *>(scope.back())->variableTable.find(std::string(aDeclNode->getName())) !=
            static_cast<Method *>(scope.back())->variableTable.end()) {
        throw SymbolTableException("Symbol Table Exception:: VariableDeclaration: duplicate");
    }
    for (int i = 0; i < static_cast<Method *>(scope.back())->parameterTable.size(); i++) {
        if (static_cast<Method *>(scope.back())->parameterTable.at(i)->name == std::string(aDeclNode->getName())) {
            throw SymbolTableException("Symbol Table Exception:: VariableDeclaration: duplicate");
        }
    }
    if (static_cast<Class *>(scope[scope.size() - 2])->classTable.find(std::string(aDeclNode->getName())) !=
            static_cast<Class *>(scope[scope.size() - 2])->classTable.end()) {
        throw SymbolTableException("Symbol Table Exception:: VariableDeclaration: duplicate");
    }
    Variable *temp = new Variable();
    temp->name = aDeclNode->getName();
    temp->setType(aDeclNode->getType());
    temp->setValue(aDeclNode->getInitValue());
    temp->setIndex(index);
    dynamic_cast<Method *>(scope.back())->variableTable[temp->name] = temp;
    aDeclNode->setEntry(temp);
}