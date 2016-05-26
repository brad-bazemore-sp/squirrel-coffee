//
// Created by brad on 5/2/16.
//

#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include "Entry.h"
#include "Variable.h"
#include "Field.h"
#include "Parameter.h"
#include "Method.h"
#include "Class.h"
#include "Package.h"
#include "SymbolTableException.h"
#include "Ast.h"
#include <vector>
#include <map>

extern map<std::string, Entry *> symbolTable;
extern int index;

class SymbolTable : public AstVisitor {
public:

    vector<Entry *> scope;

    SymbolTable() {
        Package *dft = new Package();
        dft->name = "Default Package";
        symbolTable[dft->name] = dft;

        scope.push_back(dft);
    }

    void visit(ClassDeclaration *aDeclNode);

    void visit(FieldDeclaration *aDeclNode);

    void visit(MethodDeclaration *aDeclNode);

    void visit(ParameterDeclaration *aDeclNode);

    void visit(VariableDeclaration *aDeclNode);

    void visit(LiteralExpression *anExpNode) { }

    void visit(ReferenceExpression *anExpNode) { }

    void visit(NewExpression *anExpNode) { }

    void visit(UnaryExpression *anExpNode) { }

    void visit(BinaryExpression *anExpNode) { }

    void visit(CastExpression *anExpNode) { }

    void visit(MethodCallExpression *anExpNode) { }

    void visit(AssignStatement *anStmtNode) { }

    void visit(WhileStatement *anStmtNode) { }

    void visit(ForStatement *anStmtNode) { }

    void visit(IfStatement *anStmtNode) { }

    void visit(ReturnStatement *anStmtNode) { }

    void visit(BlockStatement *anStmtNode) { }

    void visit(MethodCallStatement *anStmtNode) { }

    void visit(EmptyStatement *anStmtNode) { }
};

#endif