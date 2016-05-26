//
// Created by brad on 5/7/16.
//
#include "CodeGenerator.h"
#include "JSMOpps.h"

using namespace std;

std::string jsm_code = "";
std::string class_name;

void CodeGenerator::visit(ClassDeclaration *aDeclNode) {
    if (aDeclNode == NULL) {
        throw AstException("AstPrinter::visit: ClassDeclaration: NULL argument");
    }
    scope.push_back(aDeclNode);
    class_name=aDeclNode->getName();
    jsm_code = ";" + std::string(aDeclNode->getName()) + ".jsm\n";
    jsm_code = jsm_code + ".bytecode 50.0\n";
    jsm_code = jsm_code + ".source " + std::string(aDeclNode->getName()) + ".java\n";
    jsm_code = jsm_code + ".class public " + std::string(aDeclNode->getName()) + "\n";
    jsm_code = jsm_code + ".super java/lang/Object\n";
    if (aDeclNode->getMembers() != NULL) {
        for (int i = 0; i < aDeclNode->getMembers()->size(); i++) {
            if (aDeclNode->getMember(i)->getKind() == AstNode::DFIELD) {
                if (static_cast<FieldDeclaration *>(aDeclNode->getMember(i))->getType() == AstNode::TINT) {
                    jsm_code = jsm_code + ".field static " + aDeclNode->getName() + " I\n";
                } else if (static_cast<FieldDeclaration *>(aDeclNode->getMember(i))->getType() == AstNode::TFLOAT) {
                    jsm_code = jsm_code + ".field static " + aDeclNode->getName() + " F\n";
                }
            }
        }
        jsm_code = jsm_code + ".method public <init>()V\n";
        jsm_code = jsm_code + ".limit stack 1\n";
        jsm_code = jsm_code + ".limit locals 1\n";
        jsm_code = jsm_code + "aload_0\n";
        jsm_code = jsm_code + "invokespecial java/lang/Object/<init>()V\n";
        jsm_code = jsm_code + retur();
        jsm_code = jsm_code + ".end method\n";
        for (int i = 0; i < aDeclNode->getMembers()->size(); i++) {
            if (aDeclNode->getMember(i)->getKind() == AstNode::DMETHOD) {
                aDeclNode->getMember(i)->accept(this);
            }
        }
        jsm_code = jsm_code + ".method static <clinit>()V\n";
        jsm_code = jsm_code + ".limit stack 1\n";
        jsm_code = jsm_code + ".limit locals 0\n";
        for (int i = 0; i < aDeclNode->getMembers()->size(); i++) {
            if (aDeclNode->getMember(i)->getKind() == AstNode::DFIELD) {
                aDeclNode->getMember(i)->accept(this);
            }
        }
        jsm_code = jsm_code + retur();
        jsm_code = jsm_code + ".end method";
    }
    scope.pop_back();
}

void CodeGenerator::visit(FieldDeclaration *aDeclNode) {
    if (aDeclNode == NULL) {
        throw AstException("AstPrinter::visit: FieldDeclaration: NULL argument");
    }
    if (aDeclNode->getInitLiteral() != NULL) {
        aDeclNode->getInitLiteral()->accept(this);
    } else {
        jsm_code = jsm_code + ldc(aDeclNode->getInitValue());
    }
    jsm_code = jsm_code + putstatic(
            std::string(static_cast<ClassDeclaration *>(scope.back())->getName()) + "/" + aDeclNode->getName());
}

void CodeGenerator::visit(MethodDeclaration *aDeclNode) {
    if (aDeclNode == NULL) {
        throw AstException("AstPrinter::visit: MethodDeclaration: NULL argument");
    }
    if (aDeclNode->getBody() == NULL) {
        throw AstException("AstPrinter::visit: MethodDeclaration: body is NULL");
    }
    unsigned long locals_size = static_cast<Method *>(aDeclNode->getEntry())->parameterTable.size() +
                                static_cast<Method *>(aDeclNode->getEntry())->variableTable.size();
    if (std::string(aDeclNode->getName()) == "main") {
        jsm_code = jsm_code + ".method public static main([Ljava/lang/String;)V\n";
        jsm_code = jsm_code + ".limit stack 99\n";
        jsm_code = jsm_code + ".limit locals " + std::to_string(locals_size+1) + "\n";
        if (aDeclNode->getVariables() != NULL) {
            for (unsigned long i = 0; i < aDeclNode->getVariables()->size(); i++) {
                aDeclNode->getVariables()->at(i)->accept(this);
            }
        }
        aDeclNode->getBody()->accept(this);
    } else {
        std::string method_desc = std::string(aDeclNode->getName()) + "(";
        if (aDeclNode->getParameters() != NULL) {
            for (int i = 0; i < aDeclNode->getParameters()->size(); i++) {
                int arg_t = static_cast<ParameterDeclaration *>(aDeclNode->getParameters()->at(i))->getType();
                if (arg_t == AstNode::TINT) {
                    method_desc = method_desc + "I";
                } else if (arg_t == AstNode::TFLOAT) {
                    method_desc = method_desc + "F";
                } else if (arg_t == AstNode::TSTRING) {
                    method_desc = method_desc + "Ljava/lang/String;";
                }
            }
        }
        method_desc = method_desc + ")";
        if (aDeclNode->getRetType() == AstNode::TVOID) {
            method_desc = method_desc + "V";
        } else if (aDeclNode->getRetType() == AstNode::TINT) {
            method_desc = method_desc + "I";
        } else if (aDeclNode->getRetType() == AstNode::TFLOAT) {
            method_desc = method_desc + "F";
        }
        jsm_code = jsm_code + ".method static " + method_desc + "\n";
        jsm_code = jsm_code + ".limit stack 99\n";
        jsm_code = jsm_code + ".limit locals " + std::to_string(locals_size) + "\n";
        if (aDeclNode->getParameters() != NULL) {
            for (unsigned long i = 0; i < aDeclNode->getParameters()->size(); i++) {
                aDeclNode->getParameters()->at(i)->accept(this);
            }
        }
        if (aDeclNode->getVariables() != NULL) {
            for (unsigned long i = 0; i < aDeclNode->getVariables()->size(); i++) {
                aDeclNode->getVariables()->at(i)->accept(this);
            }
        }
        aDeclNode->getBody()->accept(this);
    }
    jsm_code = jsm_code + ".end method\n";
}

void CodeGenerator::visit(ParameterDeclaration *aDeclNode) {
    if (aDeclNode == NULL) {
        throw AstException("AstPrinter::visit: ParameterDeclaration: NULL argument");
    }

}

void CodeGenerator::visit(VariableDeclaration *aDeclNode) {
    if (aDeclNode == NULL) {
        throw AstException("AstPrinter::visit: VariableDeclaration: NULL argument");
    }
    int l_index = static_cast<Variable *>(aDeclNode->getEntry())->getIndex();
    if (aDeclNode->getInitLiteral() != NULL) {
        aDeclNode->getInitLiteral()->accept(this);
    } else {
        jsm_code = jsm_code + ldc(aDeclNode->getInitValue());
    }
    switch (aDeclNode->getType()) {
        case AstNode::TINT:
            jsm_code = jsm_code + istore(std::to_string(l_index));
            break;
        case AstNode::TFLOAT:
            jsm_code = jsm_code + fstore(std::to_string(l_index));
            break;
    }
}

void CodeGenerator::visit(MethodCallStatement *anStmtNode) {
    if (anStmtNode == NULL) {
        throw AstException("AstPrinter::visit: MethodCallStatement: NULL argument");
    }
    if (anStmtNode->getExpression() == NULL) {
        throw AstException("PrintStatementVisitor::visit: MethodCallStatement: MethodCallExpression is NULL");
    }
    anStmtNode->getExpression()->accept(this);
}

void CodeGenerator::visit(MethodCallExpression *anExpNode) {
    if (anExpNode == NULL) {
        throw AstException("AstPrinter::visit: NULL argument in MethodCallExpression");
    }
    std::string method_desc;
    std::string m_name=anExpNode->getMethodName();
    if(anExpNode->getArguments()!=NULL){
        for(int i=0;i<anExpNode->getArguments()->size();i++){
            anExpNode->getArgument(i)->accept(this);
        }
    }
    if(m_name=="readInt"||m_name=="readFloat"||m_name=="println"||m_name=="printInt"||m_name=="printString"||m_name=="printFLoat"){
        method_desc = "SimpleIO/" + std::string(anExpNode->getMethodName()) + "(";
    }else {
        method_desc = class_name + "/" + std::string(anExpNode->getMethodName()) + "(";
    }
    Method *m = static_cast<Method *>(anExpNode->getEntry());
    if (anExpNode->getArguments() != NULL) {
        for (int i = 0; i < anExpNode->getArguments()->size(); i++) {
            int arg_t = anExpNode->getArgument(i)->getType();
            if (arg_t == AstNode::TINT) {
                method_desc = method_desc + "I";
            } else if (arg_t == AstNode::TFLOAT) {
                method_desc = method_desc + "F";
            } else if (arg_t == AstNode::TSTRING) {
                method_desc = method_desc + "Ljava/lang/String;";
            }
        }
    }
    method_desc = method_desc + ")";
    if (m->getReturnType() == AstNode::TVOID) {
        method_desc = method_desc + "V";
    } else if (m->getReturnType() == AstNode::TINT) {
        method_desc = method_desc + "I";
    } else if (m->getReturnType() == AstNode::TFLOAT) {
        method_desc = method_desc + "F";
    }
    jsm_code = jsm_code + invokestatic(method_desc);
}

void CodeGenerator::visit(ReturnStatement *anStmtNode) {
    if (anStmtNode == NULL) {
        throw AstException("AstPrinter::visit: NULL argument in ReturnStatement");
    }
    if (anStmtNode->getExpression() != NULL) {
        anStmtNode->getExpression()->accept(this);
        if (anStmtNode->getExpression()->getType() == AstNode::TINT) {
            jsm_code = jsm_code + ireturn();
        } else if (anStmtNode->getExpression()->getType() == AstNode::TFLOAT) {
            jsm_code = jsm_code + freturn();
        }
    } else {
        jsm_code = jsm_code + retur();
    }
}

void CodeGenerator::visit(NewExpression *anExpNode) {
    if (anExpNode == NULL) {
        throw AstException("AstPrinter::visit: NewExpression: NULL argument");
    }
    if (anExpNode->getExpression() == NULL) {
        throw AstException("AstPrinter::visit: NewExpression: size expression is NULL");
    }
    anExpNode->getExpression()->accept(this);
}

void CodeGenerator::visit(BlockStatement *anStmtNode) {
    if (anStmtNode == NULL) {
        throw AstException("AstPrinter::visit: BlockStatement: NULL argument");
    }
    if (anStmtNode->getStatements() == NULL) {
        throw AstException("AstPrinter::visit: BlockStatement: statements are NULL");
    }
    for (int i = 0; i < anStmtNode->getStatements()->size(); i++) {
        if (anStmtNode->getStatements()->at(i) == NULL) {
            throw AstException("AstPrinter::visit: BlockStatement: operand statement at position %d is NULL");
        }
        anStmtNode->getStatements()->at(i)->accept(this);
    }
}

void CodeGenerator::visit(IfStatement *anStmtNode) {
    if (anStmtNode == NULL) {
        throw AstException("AstPrinter::visit: IfStatement: NULL argument");
    }
    if (anStmtNode->getExpression() == NULL) {
        throw AstException("AstPrinter::visit: IfStatement: expression is NULL");
    }
    if (anStmtNode->getThenStatement() == NULL) {
        throw AstException("AstPrinter::visit: IfStatement: then statement is NULL");
    }
    int op = static_cast<BinaryExpression *>(anStmtNode->getExpression())->getOperator();
    int lt = static_cast<BinaryExpression *>(anStmtNode->getExpression())->getLeftOperand()->getType();
    int rt = static_cast<BinaryExpression *>(anStmtNode->getExpression())->getRightOperand()->getType();
    std::string else_label = "else-label" + label_index;
    label_index++;
    std::string end_label = "end-label" + label_index;
    label_index++;
    if (lt == AstNode::TINT && rt == AstNode::TINT) {
        anStmtNode->getExpression()->accept(this);
        switch (op) {
            case AstNode::EQOP:
                jsm_code = jsm_code + if_icmpeq(else_label);
                break;
            case AstNode::NEOP:
                jsm_code = jsm_code + if_icmpne(else_label);
                break;
            case AstNode::GTOP:
                jsm_code = jsm_code + if_icmpgt(else_label);
                break;
            case AstNode::LTOP:
                jsm_code = jsm_code + if_icmplt(else_label);
                break;
            case AstNode::GEOP:
                jsm_code = jsm_code + if_icmpge(else_label);
                break;
            case AstNode::LEOP:
                jsm_code = jsm_code + if_icmple(else_label);
                break;
        }
        anStmtNode->getThenStatement()->accept(this);
        jsm_code = jsm_code + jsm_goto(end_label);
        if (anStmtNode->getElseStatement() != NULL) {
            jsm_code = jsm_code + else_label + ":\n";
            anStmtNode->getElseStatement()->accept(this);
        }
        jsm_code = jsm_code + end_label + ":\n";
    } else if (rt == AstNode::TFLOAT || lt == AstNode::TFLOAT) {
        anStmtNode->getExpression()->accept(this);
        jsm_code = jsm_code + fcmpg();
        switch (op) {
            case AstNode::EQOP:
                jsm_code = jsm_code + ifeq(else_label);
                break;
            case AstNode::NEOP:
                jsm_code = jsm_code + ifne(else_label);
                break;
            case AstNode::GTOP:
                jsm_code = jsm_code + ifgt(else_label);
                break;
            case AstNode::LTOP:
                jsm_code = jsm_code + iflt(else_label);
                break;
            case AstNode::GEOP:
                jsm_code = jsm_code + ifge(else_label);
                break;
            case AstNode::LEOP:
                jsm_code = jsm_code + ifle(else_label);
                break;
        }
        anStmtNode->getThenStatement()->accept(this);
        jsm_code = jsm_code + jsm_goto(end_label);
        if (anStmtNode->getElseStatement() != NULL) {
            anStmtNode->getElseStatement()->accept(this);
            anStmtNode->getElseStatement()->accept(this);
        }
        jsm_code = jsm_code + end_label + ":\n";
    }
}

void CodeGenerator::visit(WhileStatement *anStmtNode) {
    if (anStmtNode == NULL) {
        throw AstException("AstPrinter::visit: WhileStatement: NULL argument");
    }
    if (anStmtNode->getExpression() == NULL) {
        throw AstException("AstPrinter::visit: WhileStatement: expression is NULL");
    }
    if (anStmtNode->getBodyStatement() == NULL) {
        throw AstException("AstPrinter::visit: WhileStatement: statement is NULL");
    }
    int op = static_cast<BinaryExpression *>(anStmtNode->getExpression())->getOperator();
    int lt = static_cast<BinaryExpression *>(anStmtNode->getExpression())->getLeftOperand()->getType();
    int rt = static_cast<BinaryExpression *>(anStmtNode->getExpression())->getRightOperand()->getType();
    std::string again_label = "again-label" + std::to_string(label_index);
    label_index++;
    std::string end_label = "end-label" + std::to_string(label_index);
    label_index++;
    jsm_code = jsm_code + again_label + ":\n";
    anStmtNode->getExpression()->accept(this);
    if (lt == AstNode::TINT && rt == AstNode::TINT) {
        if (op == AstNode::EQOP) {
            jsm_code = jsm_code + if_icmpne(end_label);
        } else if (op == AstNode::NEOP) {
            jsm_code = jsm_code + if_icmpeq(end_label);
        } else if (op == AstNode::GTOP) {
            jsm_code = jsm_code + if_icmplt(end_label);
        } else if (op == AstNode::LTOP) {
            jsm_code = jsm_code + if_icmpgt(end_label);
        } else if (op == AstNode::GEOP) {
            jsm_code = jsm_code + if_icmple(end_label);
        } else if (op == AstNode::LEOP) {
            jsm_code = jsm_code + if_icmpge(end_label);
        }
        anStmtNode->getBodyStatement()->accept(this);
        jsm_code = jsm_code + jsm_goto(again_label);
    } else if (lt == AstNode::TFLOAT || rt == AstNode::TFLOAT) {
        jsm_code = jsm_code + fcmpg();
        if (op == AstNode::EQOP) {
            jsm_code = jsm_code + ifne(end_label);
        } else if (op == AstNode::NEOP) {
            jsm_code = jsm_code + ifeq(end_label);
        } else if (op == AstNode::GTOP) {
            jsm_code = jsm_code + iflt(end_label);
        } else if (op == AstNode::LTOP) {
            jsm_code = jsm_code + ifgt(end_label);
        } else if (op == AstNode::GEOP) {
            jsm_code = jsm_code + ifle(end_label);
        } else if (op == AstNode::LEOP) {
            jsm_code = jsm_code + ifge(end_label);
        }
        anStmtNode->getBodyStatement()->accept(this);
        jsm_code = jsm_code + jsm_goto(again_label);
    }
    jsm_code = jsm_code + end_label + ":\n";
}

void CodeGenerator::visit(ForStatement *anStmtNode) {
    if (anStmtNode == NULL) {
        throw AstException("AstPrinter::visit: ForStatement: NULL argument");
    }

    if (anStmtNode->getInit() == NULL) {
        throw AstException("AstPrinter::visit: ForStatement: initiatlization expression is NULL");
    }

    if (anStmtNode->getTerm() == NULL) {
        throw AstException("AstPrinter::visit: ForStatement: termination expression is NULL");
    }

    if (anStmtNode->getUpd() == NULL) {
        throw AstException("AstPrinter::visit: ForStatement: update expression is NULL");
    }

    if (anStmtNode->getBodyStatement() == NULL) {
        throw AstException("AstPrinter::visit: ForStatement: statement is NULL");
    }
    if (anStmtNode->getIndex() != NULL) {
        anStmtNode->getIndex()->accept(this);
    }
    anStmtNode->getInit()->accept(this);
    anStmtNode->getTerm()->accept(this);
    anStmtNode->getUpd()->accept(this);
    anStmtNode->getBodyStatement()->accept(this);
}

void CodeGenerator::visit(AssignStatement *anStmtNode) {
    if (anStmtNode == NULL) {
        throw AstException("AstPrinter::visit: AssignStatement: NULL argument");
    }
    anStmtNode->getExpression()->accept(this);
    if (anStmtNode->getExpression() == NULL) {
        throw AstException("PrintStatementVisitor::visit: AssignStatement: NULL expression");
    }
    if (anStmtNode->getEntry()->getKind() == AstNode::DVARIABLE) {
        int l_index = static_cast<Variable *>(anStmtNode->getEntry())->getIndex();
        switch (anStmtNode->getExpression()->getType()) {
            case AstNode::TINT:
                jsm_code = jsm_code + istore(std::to_string(l_index));
                break;
            case AstNode::TFLOAT:
                jsm_code = jsm_code + fstore(std::to_string(l_index));
                break;
        }
    } else if (anStmtNode->getEntry()->getKind() == AstNode::DPARAMETER) {
        int l_index = static_cast<Parameter *>(anStmtNode->getEntry())->getIndex();
        switch (anStmtNode->getExpression()->getType()) {
            case AstNode::TINT:
                jsm_code = jsm_code + istore(std::to_string(l_index));
                break;
            case AstNode::TFLOAT:
                jsm_code = jsm_code + fstore(std::to_string(l_index));
                break;
        }
    } else if (anStmtNode->getKind() == AstNode::DFIELD) {
        jsm_code=jsm_code+putstatic(class_name+anStmtNode->getLHSName());
    }
}

void CodeGenerator::visit(CastExpression *anExpNode) {
    if (anExpNode == NULL) {
        throw AstException("AstPrinter::visit: CastExpression: NULL argument");
    }
    anExpNode->getOperand()->accept(this);
    if (anExpNode->getOperand() == NULL) {
        throw AstException("AstPrinter::visit: ECAST operand is NULL");
    }
    if (anExpNode->getCastToType() == AstNode::TINT) {
        jsm_code = jsm_code + f2i();
    } else if (anExpNode->getCastToType() == AstNode::TFLOAT) {
        jsm_code = jsm_code + i2f();
    }
}

void CodeGenerator::visit(BinaryExpression *anExpNode) {
    if (anExpNode == NULL) {
        throw AstException("AstPrinter::visit: BinaryExpression: NULL argument");
    }
    if (anExpNode->getLeftOperand() == NULL) {
        throw AstException("AstPrinter.visit: EBINARY left operand is NULL");
    }
    if (anExpNode->getRightOperand() == NULL) {
        throw AstException("AstPrinter.visit: EBINARY right operand is NULL");
    }
    int lt = anExpNode->getLeftOperand()->getType();
    int rt = anExpNode->getRightOperand()->getType();
    if (lt != rt) {
        if (lt == AstNode::TINT) {
            anExpNode->getLeftOperand()->accept(this);
            jsm_code = jsm_code + i2f();
        } else if (rt == AstNode::TINT) {
            anExpNode->getRightOperand()->accept(this);
            jsm_code = jsm_code + i2f();
        }
    } else {
        anExpNode->getLeftOperand()->accept(this);
        anExpNode->getRightOperand()->accept(this);
    }
    switch (anExpNode->getOperator()) {
        case AstNode::ADDOP:
            if (rt == AstNode::TINT) {
                jsm_code = jsm_code + iadd();
            } else if (rt == AstNode::TFLOAT) {
                jsm_code = jsm_code + fadd();
            }
            break;
        case AstNode::SUBOP:
            if (rt == AstNode::TINT) {
                jsm_code = jsm_code + isub();
            } else if (rt == AstNode::TFLOAT) {
                jsm_code = jsm_code + fsub();
            }
            break;
        case AstNode::MULOP:
            if (rt == AstNode::TINT) {
                jsm_code = jsm_code + imul();
            } else if (rt == AstNode::TFLOAT) {
                jsm_code = jsm_code + fmul();
            }
            break;
        case AstNode::DIVOP:
            if (rt == AstNode::TINT) {
                jsm_code = jsm_code + idiv();
            } else if (rt == AstNode::TFLOAT) {
                jsm_code = jsm_code + fdiv();
            }
            break;
    }
}

void CodeGenerator::visit(UnaryExpression *anExpNode) {
    if (anExpNode == NULL) {
        throw AstException("AstPrinter::visit: UnaryExpression: NULL argument");
    }
    anExpNode->getOperand()->accept(this);
    if (anExpNode->getOperand() == NULL) {
        throw AstException("AstPrinter.visit: UnaryExpression: operand is NULL");
    }
    int type = anExpNode->getType();
    switch (anExpNode->getOperator()) {
        case AstNode::SUBOP:
            if (type == AstNode::TINT) {
                jsm_code = jsm_code + ineg();
            } else {
                jsm_code = jsm_code + fneg();
            }
            break;
    }
}

void CodeGenerator::visit(ReferenceExpression *anExpNode) {
    if (anExpNode == NULL) {
        throw AstException("AstPrinter::visit: ReferenceExpression: NULL argument");
    }
    if (anExpNode->getEntry()->getKind() == AstNode::DVARIABLE) {
        int l_index = static_cast<Variable *>(anExpNode->getEntry())->getIndex();
        int type= static_cast<Variable*>(anExpNode->getEntry())->getType();
        switch (type) {
            case AstNode::TINT:
                jsm_code = jsm_code + iload(std::to_string(l_index));
                break;
            case AstNode::TFLOAT:
                jsm_code = jsm_code + fload(std::to_string(l_index));
                break;
        }
    } else if (anExpNode->getEntry()->getKind() == AstNode::DPARAMETER) {
        int l_index = static_cast<Parameter *>(anExpNode->getEntry())->getIndex();
        int type= static_cast<Parameter*>(anExpNode->getEntry())->getType();
        switch (type) {
            case AstNode::TINT:
                jsm_code = jsm_code + iload(std::to_string(l_index));
                break;
            case AstNode::TFLOAT:
                jsm_code = jsm_code + fload(std::to_string(l_index));
                break;
        }
    } else if (anExpNode->getKind() == AstNode::DFIELD) {
        //TODO figure out how to get the field spec
    }
    if (anExpNode->getExpression() != NULL) {
        anExpNode->getExpression()->accept(this);
    }
}

void CodeGenerator::visit(LiteralExpression *anExpNode) {
    if (anExpNode == NULL) {
        throw AstException("AstPrinter::visit: NULL argument in LiteralExpression");
    }
    jsm_code = jsm_code + ldc(anExpNode->getLiteral());
}

void CodeGenerator::visit(EmptyStatement *anStmtNode) {
    if (anStmtNode == NULL) {
        throw AstException("AstPrinter::visit: EmptyStatement: NULL argument");
    }
    jsm_code = jsm_code + "";
}
