//
// Created by brad on 5/7/16.
//

#ifndef CODEGENERATOR_H
#define CODEGENERATOR_H

#include <iostream>
#include "Ast.h"
#include "Entry.h"
#include "Variable.h"
#include "Field.h"
#include "Parameter.h"
#include "Method.h"
#include "Class.h"
#include "Package.h"


using namespace std;

extern std::string jsm_code;

class CodeGenerator: public AstVisitor{

public:

    int label_index=0;

    vector<Declaration *> scope;

    CodeGenerator(){}

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


#endif //TJC_PART_2_CODEGENERATOR_H
