//
// Created by brad on 5/4/16.
//
//TODO have fatal errors exit but not warnings
#include "SemanticCheck.h"
#include <iostream>

using namespace std;

void SemanticCheck::visit(ClassDeclaration *aDeclNode) {
    /*TODO Check if class name and file name match */
    if (aDeclNode == NULL) {
        throw SemanticException("SemanticCheck::visit: ClassDeclaration: NULL argument");
    }
    scope.push_back(static_cast<Package *>(scope.back())->packageTable[aDeclNode->getName()]);
    if (aDeclNode->getMembers() == NULL) {
        cout << "No members" << endl;
        cout.flush();
    } else {
        for (int i = 0; i < aDeclNode->getMembers()->size(); i++) {
            if (aDeclNode->getMember(i) == NULL) {
                throw SemanticException("SemanticCheck::visit: DCLASS: member is NULL");
            }
            aDeclNode->getMember(i)->accept(this);
        }
    }
    scope.pop_back();
}

void SemanticCheck::visit(FieldDeclaration *aDeclNode) {
    if (aDeclNode == NULL) {
        throw SemanticException("SemanticCheck::visit: FieldDeclaration: NULL argument");
    }
    if (static_cast<Class *>(scope.back())->classTable[aDeclNode->getName()] == NULL) {
        throw SemanticException("SemanticCheck::visit: FieldDeclaration: does not exsist on symbol table");
    }
}

void SemanticCheck::visit(MethodDeclaration *aDeclNode) {
    if (aDeclNode == NULL) {
        throw SemanticException("SemanticCheck::visit: MethodDeclaration: NULL argument");
    }
    if (aDeclNode->getBody() == NULL) {
        throw SemanticException("SemanticCheck::visit: MethodDeclaration: body is NULL");
    }
    if (static_cast<Class *>(scope.back())->classTable[aDeclNode->getName()] == NULL) {
        throw SemanticException("SemanticCheck::visit: MethodDeclaration: can't find on symbol table");
    } else {
        scope.push_back(static_cast<Class *>(scope.back())->classTable[aDeclNode->getName()]);
    }
    if (static_cast<Method *>(scope.back())->getReturnType() != aDeclNode->getRetType()) {
        throw SemanticException("SemanticCheck::visit: MethodDeclaration: return type at declaration is wrong");
    }
    if (aDeclNode->getParameters() != NULL) {
        for (int i = 0; i < aDeclNode->getParameters()->size(); i++) {
            aDeclNode->getParameters()->at(i)->accept(this);
        }
    }
    if (aDeclNode->getVariables() != NULL) {
        for (int i = 0; i < aDeclNode->getVariables()->size(); i++) {
            aDeclNode->getVariables()->at(i)->accept(this);
        }
    }
    aDeclNode->getBody()->accept(this);
    scope.pop_back();
}

void SemanticCheck::visit(ParameterDeclaration *aDeclNode) {
    if (aDeclNode == NULL) {
        throw SemanticException("SemanticCheck::visit: ParameterDeclaration: NULL argument");
    }
    if (static_cast<Method *>(scope.back())->parameterTable.back() == NULL) {
        throw SemanticException("SemanticCheck::visit: ParameterDeclaration: can't find on symbol table");
    }
}

void SemanticCheck::visit(VariableDeclaration *aDeclNode) {
    if (aDeclNode == NULL) {
        throw SemanticException("SemanticCheck::visit: VariableDeclaration: NULL argument");
    }
    if (static_cast<Method *>(scope.back())->variableTable[aDeclNode->getName()] == NULL) {
        throw SemanticException("SemanticCheck::visit: VariableDeclaration: can't find on symbol table");
    }
}

void SemanticCheck::visit(LiteralExpression *anExpNode) {
    if (anExpNode == NULL) {
        throw SemanticException("SemanticCheck::visit: NULL argument in LiteralExpression");
    }
}

void SemanticCheck::visit(ReferenceExpression *anExpNode) {
    Method *m = static_cast<Method *>(scope.back());
    if (anExpNode == NULL) {
        throw SemanticException("SemanticCheck::visit: ReferenceExpression: NULL argument");
    }
    if (m->variableTable.find(anExpNode->getName()) == m->variableTable.end()) {
        if (static_cast<Class *>(scope[scope.size() - 2])->classTable[anExpNode->getName()] == NULL) {
            for (int i = 0; i < static_cast<Method *>(scope.back())->parameterTable.size(); i++) {
                if (static_cast<Method *>(scope.back())->parameterTable.at(i)->name ==
                    std::string(anExpNode->getName())) {
                    anExpNode->setEntry(static_cast<Method *>(scope.back())->parameterTable[i]);
                    anExpNode->setType(static_cast<Parameter *>(anExpNode->getEntry())->getType());
                    break;
                }
            }
            if(anExpNode->getEntry()==NULL) {
                throw SemanticException("SemanticCheck::visit: ReferenceExpression: can't be found on symbol table ");
            }
        } else {
            anExpNode->setEntry(static_cast<Class *>(scope.back())->classTable[anExpNode->getName()]);
            anExpNode->setType(static_cast<Field *>(anExpNode->getEntry())->getType());
        }
    } else {
        anExpNode->setEntry(static_cast<Method *>(scope.back())->variableTable[std::string(anExpNode->getName())]);
        anExpNode->setType(static_cast<Variable *>(anExpNode->getEntry())->getType());
    }
    if (anExpNode->getEntry()->getKind() != AstNode::DVARIABLE &&
        anExpNode->getEntry()->getKind() != AstNode::DFIELD &&
        anExpNode->getEntry()->getKind() != AstNode::DPARAMETER) {
        throw SemanticException(
                "Error: Symbol table entry is not a variable, field, or parameter: " + anExpNode->getLineNo());
    }
    if (anExpNode->getExpression() != NULL) {
        anExpNode->getExpression()->accept(this);
    }
}

//TODO deal with this if you have time
void SemanticCheck::visit(NewExpression *anExpNode) {
    if (anExpNode == NULL) {
        throw SemanticException("SemanticCheck::visit: NewExpression: NULL argument");
    }
    if (anExpNode->getExpression() == NULL) {
        throw SemanticException("SemanticCheck::visit: NewExpression: size expression is NULL");
    }
    anExpNode->getExpression()->accept(this);
}

void SemanticCheck::visit(UnaryExpression *anExpNode) {
    if (anExpNode == NULL) {
        throw SemanticException("SemanticCheck::visit: UnaryExpression: NULL argument");
    }
    anExpNode->getOperand()->accept(this);
    if (anExpNode->getOperand() == NULL) {
        throw SemanticException("SemanticCheck.visit: UnaryExpression: operand is NULL");
    }
    if (anExpNode->getOperand()->getType() != AstNode::TFLOAT && anExpNode->getOperand()->getType() != AstNode::TINT) {
        throw SemanticException("SemanticCheck.visit: UnaryExpression: operand is not a number");
    }
    anExpNode->setType(anExpNode->getOperand()->getType());
}

void SemanticCheck::visit(BinaryExpression *anExpNode) {
    int op;
    int lt;
    int rt;
    int et;
    if (anExpNode == NULL) {
        throw SemanticException("SemanticCheck::visit: BinaryExpression: NULL argument");
    }
    anExpNode->getLeftOperand()->accept(this);
    anExpNode->getRightOperand()->accept(this);
    if (anExpNode->getLeftOperand() == NULL) {
        throw SemanticException("SemanticCheck.visit: EBINARY left operand is NULL");
    }
    if (anExpNode->getRightOperand() == NULL) {
        throw SemanticException("SemanticCheck.visit: EBINARY right operand is NULL");
    }
    op = anExpNode->getOperator();
    lt = anExpNode->getLeftOperand()->getType();
    rt = anExpNode->getRightOperand()->getType();
    if (rt == lt) {
        if (rt != AstNode::TFLOAT && rt != AstNode::TINT) {
            throw SemanticException("SemanticCheck.visit: no a number");
        } else {
            et = rt;
        }
    } else {
        if (rt != AstNode::TFLOAT && rt != AstNode::TINT && lt != AstNode::TFLOAT && lt != AstNode::TINT) {
            throw SemanticException("SemanticCheck.visit: no a number");
        } else {
            if (rt == AstNode::TFLOAT || lt == AstNode::TFLOAT) {
                et = AstNode::TFLOAT;
            }
        }
    }
    if (op == AstNode::MULOP || op == AstNode::DIVOP || op == AstNode::ADDOP || op == AstNode::SUBOP) {
        anExpNode->setType(et);
    } else if (op == AstNode::LTOP || op == AstNode::GTOP || op == AstNode::LEOP || op == AstNode::GEOP ||
               op == AstNode::EQOP || op == AstNode::NEOP) {
        anExpNode->setType(AstNode::TBOOL);
    }
}

void SemanticCheck::visit(CastExpression *anExpNode) {
    if (anExpNode == NULL) {
        throw SemanticException("SemanticCheck::visit: CastExpression: NULL argument");
    }
    anExpNode->getOperand()->accept(this);
    if (anExpNode->getOperand() == NULL) {
        throw SemanticException("SemanticCheck::visit: CastExpression: operand is NULL");
    }
    if (anExpNode->getOperand()->getType() != AstNode::TINT && anExpNode->getOperand()->getType() != AstNode::TFLOAT) {
        throw SemanticException("SemanticCheck::visit: CastExpression: operand is not a number");
    } else {
        anExpNode->setType(anExpNode->getCastToType());
    }
}

void SemanticCheck::visit(MethodCallExpression *anExpNode) {
    if (anExpNode == NULL) {
        throw SemanticException("SemanticCheck::visit: NULL argument in MethodCallExpression");
    }
    if (static_cast<Class *>(scope.at(scope.size() - 2))->classTable[anExpNode->getMethodName()] == NULL) {
        throw SemanticException("SemanticCheck::visit: MethodCallExpression: can't find method on symbol table");
    } else {
        if (static_cast<Class *>(scope.at(scope.size() - 2))->classTable[anExpNode->getMethodName()]->getKind() ==
            AstNode::DMETHOD) {
            anExpNode->setEntry(
                    static_cast<Class *>(scope.at(scope.size() - 2))->classTable[anExpNode->getMethodName()]);
        } else {
            throw SemanticException("SemanticCheck::visit: MethodCallExpression: symbol table entry is not a method");
        }
    }
    if (anExpNode->getArguments() != NULL) {
        Method *m = static_cast<Method *>(anExpNode->getEntry());
        if (anExpNode->getArguments()->size() == static_cast<Method *>(anExpNode->getEntry())->parameterTable.size()) {
            for (int i = 0; i < anExpNode->getArguments()->size(); i++) {
                anExpNode->getArgument(i)->accept(this);
                Parameter *temp = static_cast<Parameter *>(m->parameterTable[i]);
                if (anExpNode->getArgument(i)->getType() != temp->getType()) {
                    throw SemanticException("SemanticCheck::visit: MethodCallExpression: parameters don't match");
                }
            }
        }
    }
    anExpNode->setType(static_cast<Method *>(anExpNode->getEntry())->getReturnType());
}

void SemanticCheck::visit(AssignStatement *anStmtNode) {
    int lt;
    if (anStmtNode == NULL) {
        throw SemanticException("SemanticCheck::visit: AssignStatement: NULL argument");
    }
    if (static_cast<Method *>(scope.back())->variableTable[anStmtNode->getLHSName()] != NULL) {
        anStmtNode->setEntry(static_cast<Method *>(scope.back())->variableTable[anStmtNode->getLHSName()]);
        lt = static_cast<Variable *>(anStmtNode->getEntry())->getType();
    } else if (static_cast<Class *>(scope.at(scope.size() - 2))->classTable[anStmtNode->getLHSName()] != NULL &&
               static_cast<Class *>(scope.at(scope.size() - 2))->classTable[anStmtNode->getLHSName()]->getKind() ==
               AstNode::DFIELD) {
        anStmtNode->setEntry(static_cast<Class *>(scope.at(scope.size() - 2))->classTable[anStmtNode->getLHSName()]);
        lt = static_cast<Field *>(anStmtNode->getEntry())->getType();
    } else {
        for (int i = 0; i < static_cast<Method *>(scope.back())->parameterTable.size(); i++) {
            if (static_cast<Method *>(scope.back())->parameterTable.at(i) != NULL) {
                anStmtNode->setEntry(static_cast<Method *>(scope.back())->parameterTable.at(i));
                lt = static_cast<Parameter *>(anStmtNode->getEntry())->getType();
                break;
            }
        }
    }
    anStmtNode->getExpression()->accept(this);
    if (anStmtNode->getExpression() == NULL) {
        throw SemanticException("Semantic Check::visit: AssignStatement: NULL expression");
    }
    if (lt != anStmtNode->getExpression()->getType()) {
        if (lt != AstNode::TFLOAT) {
            throw SemanticException("Semantic Check::visit:AssignStatement left hand is not float");
        } else {
            if (anStmtNode->getExpression()->getType() != AstNode::TFLOAT) {
                throw SemanticException("Semantic Check::visit:AssignStatement right hand is not float");
            }
        }
    } else {
        if (lt != AstNode::TFLOAT && lt != AstNode::TINT) {
            throw SemanticException("Semantic Check::visit:AssignStatement not of float or int");
        }
    }
}

void SemanticCheck::visit(WhileStatement *anStmtNode) {
    if (anStmtNode == NULL) {
        throw SemanticException("SemanticCheck::visit: WhileStatement: NULL argument");
    }
    anStmtNode->getExpression()->accept(this);
    if (anStmtNode->getExpression()->getType() != AstNode::TBOOL) {
        throw SemanticException("SemanticCheck::visit: WhileStatement: control statement is not a boolean");
    }
    if (anStmtNode->getExpression() == NULL) {
        throw SemanticException("SemanticCheck::visit: WhileStatement: expression is NULL");
    }
    anStmtNode->getBodyStatement()->accept(this);
    if (anStmtNode->getBodyStatement() == NULL) {
        throw SemanticException("SemanticCheck::visit: WhileStatement: statement is NULL");
    }
}

//TODO Dear god don't make me do this, just use a while statement for the love of all that is holly
void SemanticCheck::visit(ForStatement *anStmtNode) {
    if (anStmtNode == NULL) {
        throw SemanticException("SemanticCheck::visit: ForStatement: NULL argument");
    }

    if (anStmtNode->getInit() == NULL) {
        throw SemanticException("SemanticCheck::visit: ForStatement: initiatlization expression is NULL");
    }

    if (anStmtNode->getTerm() == NULL) {
        throw SemanticException("SemanticCheck::visit: ForStatement: termination expression is NULL");
    }

    if (anStmtNode->getUpd() == NULL) {
        throw SemanticException("SemanticCheck::visit: ForStatement: update expression is NULL");
    }

    if (anStmtNode->getBodyStatement() == NULL) {
        throw SemanticException("SemanticCheck::visit: ForStatement: statement is NULL");
    }
    if (anStmtNode->getIndex() != NULL) {
        anStmtNode->getIndex()->accept(this);
    }
    anStmtNode->getInit()->accept(this);
    anStmtNode->getTerm()->accept(this);
    anStmtNode->getUpd()->accept(this);
    anStmtNode->getBodyStatement()->accept(this);
}

void SemanticCheck::visit(IfStatement *anStmtNode) {
    if (anStmtNode == NULL) {
        throw SemanticException("SemanticCheck::visit: IfStatement: NULL argument");
    }
    anStmtNode->getExpression()->accept(this);
    if (anStmtNode->getExpression() == NULL) {
        throw SemanticException("SemanticCheck::visit: IfStatement: expression is NULL");
    }
    if (anStmtNode->getExpression()->getType() != AstNode::TBOOL) {
        throw SemanticException("Semantic Check::visit: If statement: the control expression is not a boolean");
    }
    anStmtNode->getThenStatement()->accept(this);
    if (anStmtNode->getThenStatement() == NULL) {
        throw SemanticException("SemanticCheck::visit: IfStatement: then statement is NULL");
    }
    if (anStmtNode->getElseStatement() != NULL) {
        anStmtNode->getElseStatement()->accept(this);
    }
}

void SemanticCheck::visit(ReturnStatement *anStmtNode) {
    if (anStmtNode == NULL) {
        throw SemanticException("SemanticCheck::visit: NULL argument in ReturnStatement");
    }
    anStmtNode->setEntry(static_cast<Method *>(scope.back()));
    if (anStmtNode->getExpression() != NULL) {
        anStmtNode->getExpression()->accept(this);
        if (anStmtNode->getExpression()->getType() != static_cast<Method *>(anStmtNode->getEntry())->getReturnType()) {
            throw SemanticException("SemanticCheck::visit: return expression and method return type do not match");
        }
    } else {
        if (static_cast<Method *>(anStmtNode->getEntry())->getReturnType() != AstNode::TVOID) {
            throw SemanticException("SemanticCheck::visit: no return value but method is not void");
        }
    }
}

void SemanticCheck::visit(BlockStatement *anStmtNode) {
    if (anStmtNode == NULL) {
        throw SemanticException("SemanticCheck::visit: BlockStatement: NULL argument");
    }

    if (anStmtNode->getStatements() == NULL) {
        throw SemanticException("SemanticCheck::visit: BlockStatement: statements are NULL");
    }
    for (int i = 0; i < anStmtNode->getStatements()->size(); i++) {
        if (anStmtNode->getStatements()->at(i) == NULL) {
            throw SemanticException("SemanticCheck::visit: BlockStatement: operand statement at position %d is NULL");
        }
        anStmtNode->getStatements()->at(i)->accept(this);
    }
}

void SemanticCheck::visit(MethodCallStatement *anStmtNode) {
    if (anStmtNode == NULL) {
        throw SemanticException("SemanticCheck::visit: MethodCallStatement: NULL argument");
    }
    anStmtNode->getExpression()->accept(this);
    if (anStmtNode->getExpression() == NULL) {
        throw SemanticException("PrintStatementVisitor::visit: MethodCallStatement: MethodCallExpression is NULL");
    }
}

void SemanticCheck::visit(EmptyStatement *anStmtNode) {
    if (anStmtNode == NULL) {
        throw SemanticException("SemanticCheck::visit: EmptyStatement: NULL argument");
    }
}