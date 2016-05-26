//
// Created by brad on 5/4/16.
//

#ifndef SEMANTICCHECK_H
#define SEMANTICCHECK_H

#include <iostream>
#include "Ast.h"
#include "SemanticException.h"
#include "Entry.h"
#include "Variable.h"
#include "Field.h"
#include "Parameter.h"
#include "Method.h"
#include "Class.h"
#include "Package.h"
#include <string>
#include <vector>

using namespace std;

extern map<std::string, Entry *> symbolTable;

class SemanticCheck : public AstVisitor {

public:

    vector<Entry *> scope;

    SemanticCheck() {
        scope.push_back(symbolTable["Default Package"]);
    }

    void visit(MethodDeclaration *aDeclNode);

    void visit(FieldDeclaration *aDeclNode);

    void visit(ParameterDeclaration *aDeclNode);

    void visit(VariableDeclaration *aDeclNode);

    void visit(ClassDeclaration *aDeclNode);

    void visit(LiteralExpression *anExpNode);

    void visit(ReferenceExpression *anExpNode);

    void visit(NewExpression *anExpNode);

    void visit(UnaryExpression *anExpNode);

    void visit(BinaryExpression *anExpNode);

    void visit(CastExpression *anExpNode);

    void visit(MethodCallExpression *anExpNode);

    void visit(AssignStatement *anStmtNode);

    void visit(WhileStatement *anStmtNode);

    void visit(ForStatement *anStmtNode);

    void visit(IfStatement *anStmtNode);

    void visit(ReturnStatement *anStmtNode);

    void visit(BlockStatement *anStmtNode);

    void visit(MethodCallStatement *anStmtNode);

    void visit(EmptyStatement *anStmtNode);
};


#endif //TJC_PART_2_SEMANTICCHECK_H
