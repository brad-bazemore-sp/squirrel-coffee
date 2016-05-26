%{
#include <iostream>
using namespace std;
#include "Ast.h"
#include "AstException.h"



void yyerror( const char *msg );
int yylex( void );
void fatal(const char*, const char*);
extern int yylineno;
extern Declaration *root;
extern Statement *method_body_block;
%}

%union {
    Expression *expVal;
    Declaration *decVal;
    Statement *stVal;
	LiteralExpression *lExp;
	std::vector<Expression *> *ex_vec;
	std::vector<Declaration *> *decl_vec;
	std::vector<Statement *> *stmt_vec;
	const char *str;
	int token;
}

%token <str> IDENT

%token <str> D_INT
%token <str> F_POINT
%token <str> STR_L

%token <token> PLUS
%token <token> MINUS
%token <token> TIMES
%token <token> DIVIDE
%token <token> EQUALS

%token <token> LE
%token <token> GE
%token <token> LEQ
%token <token> GEQ
%token <token> EQ
%token <token> NE

%token <token> L_PAR
%token <token> R_PAR
%token <token> L_BRAC
%token <token> R_BRAC
%token <token> L_BRC
%token <token> R_BRC

%token <token> SEMICOLON
%token <token> COMMA
%token <token> PERIOD

%token <token> T_INT
%token <token> T_FLOAT
%token <token> T_NULL

%token <token> PUBLIC
%token <token> CLASS
%token <token> STATIC
%token <token> VOID
%token <token> RETURN
%token <token> NEW

%token <token> IF
%token <token> ELSE
%token <token> WHILE
%token <token> FOR

%token <token> INC
%token <token> DEC

%type <str> qualified_name
%type <expVal> unary_expression relational_expression multiplicative_expression additive_expression primary_expression expression method_invocation
%type <decVal> class_decl local_decl formal_param method_decl member_decl field_decl
%type <stVal> return_statement statement statement_list method_statement_list
%type <token> type
%type <lExp> literal
%type <ex_vec> argument_list
%type <decl_vec> local_decl_list formal_param_list member_decl_list method_body

%start tiny_java_program

%%

tiny_java_program:
    class_decl
	{
		root=$1;
	}
    ;

class_decl:
    PUBLIC CLASS IDENT L_BRAC member_decl_list R_BRAC
	{
		try{		
			ClassDeclaration *temp = new ClassDeclaration(yylineno, $3);
			if(temp==NULL){
				fatal("factor: ", "could not create class");
			}else{
				int i=0;
				for(;i<$5->size();i++){
					temp->addMember($5->at(i));					
				}
				$$=temp;
			}
		}catch(AstException& e){
			cout << "Error: " <<  e.what(); YYABORT;
		}catch(...){
			cout << "Error unkown exception at class_decl:PUBLIC CLASS IDENT L_BRAC member_decl_list R_BRAC" << endl;
		}
	}
    ;

member_decl_list:
    member_decl member_decl_list
	{
		$$=$2;
		$$->push_back($1);
	}
	|
	/* empty */
	{
		$$=new vector<Declaration *>();
	}
    ;

member_decl:
    field_decl
	{
		$$=$1;
	}
    |
    method_decl
	{
		$$=$1;
	}
    ;

method_decl:
    STATIC type IDENT L_PAR formal_param_list R_PAR L_BRAC method_body R_BRAC
	{
		try{
			MethodDeclaration *temp=new MethodDeclaration(yylineno, $3, $2);
			if(temp==NULL){
				fatal("factor: ", "could not create method decle");
			}else{
				temp->setParameters($5);
				if(method_body_block!=NULL){
					temp->setBody(method_body_block);
				}
				method_body_block=nullptr;
				if($8!=NULL){
					temp->setVariables($8);
				}
				$$=temp;
			}
		}catch(AstException& e){
			cout << "Error: " <<  e.what(); YYABORT;
		}catch(...){
			cout << "Error unkown exception at method_decl:STATIC return_type IDENT L_PAR formal_param_list R_PAR L_BRAC method_body R_BRAC" << endl;
		}
	}
    |
    STATIC type IDENT L_PAR R_PAR L_BRAC method_body R_BRAC
	{
		try{
			MethodDeclaration *temp=new MethodDeclaration(yylineno, $3, $2);
			if(temp==NULL){
				fatal("factor: ", "could not create method decel");
			}else{
				if(method_body_block!=NULL){
					temp->setBody(method_body_block);
				}
				method_body_block=NULL;
				if($7!=NULL){
					temp->setVariables($7);
				}
				$$=temp;
			}
		}catch(AstException& e){
			cout << "Error: " <<  e.what(); YYABORT;
		}catch(...){
			cout << "Error unkown exception at method_decl:STATIC return_type IDENT L_PAR R_PAR L_BRAC method_body R_BRAC" << endl;
		}
	}
    |
    PUBLIC STATIC VOID IDENT L_PAR IDENT L_BRC R_BRC IDENT R_PAR L_BRAC method_body R_BRAC
	{
		try{
			MethodDeclaration *temp=new MethodDeclaration(yylineno, $4, AstNode::TVOID);
			if(temp==NULL){
				fatal("factor: ", "could not create method decel");
			}else{
				if(method_body_block!=NULL){
					temp->setBody(method_body_block);
				}
				method_body_block=NULL;
				if($12!=NULL){
					temp->setVariables($12);
				}
				$$=temp;
			}
		}catch(AstException& e){
			cout << "Error: " <<  e.what(); YYABORT;
		}catch(...){
			cout << "Error unkown exception at method_decl:PUBLIC STATIC VOID IDENT L_PAR IDENT L_BRC R_BRC IDENT R_PAR L_BRAC method_body R_BRAC" << endl;
		}
	}
    ;

field_decl:
    STATIC type IDENT EQUALS literal SEMICOLON
	{
		try{
			$$=new FieldDeclaration(yylineno, $3, $2, $5);
			if($$==NULL){
				fatal("factor: ", "could not create field");
			}
		}catch(AstException& e){
			cout << "Error: " <<  e.what(); YYABORT;
		}catch(...){
			cout << "Error unkown exception at field_decl:STATIC type IDENT EQUALS literal SEMICOLON" << endl;
		}
	}
    ;

formal_param_list:
    formal_param
	{
		$$=new vector<Declaration*>();
		$$->push_back($1);
	}
    |
    formal_param COMMA formal_param_list
	{
		$$=$3;
		$$->push_back($1);
	}
    ;

formal_param:
    type IDENT
	{
		try{
			$$=new ParameterDeclaration(yylineno, $2, $1);
			if($$==NULL){
				fatal("factor: ", "could not create a formal parameter");
			}
		}catch(AstException& e){
			cout << "Error: " <<  e.what(); YYABORT;
		}catch(...){
			cout << "Error unkown exception at formal_param:type IDENT" << endl;
		}
	}
    ;

method_body:
    local_decl_list method_statement_list
	{
		$$=$1;
		method_body_block=$2;
	}
    ;

local_decl_list:
	local_decl local_decl_list
	{
		$$=$2;
		$$->push_back($1);
	}
	|
	/* empty */
	{
		$$=new vector<Declaration *>();
	}
    ;

local_decl:
    type IDENT EQUALS literal SEMICOLON
	{
		try{
			$$=new VariableDeclaration(yylineno, $2, $1, $4);
			if($$==NULL){
				fatal("factor: ", "could not create variable");
			}
		}catch(AstException& e){
			cout << "Error: " <<  e.what(); YYABORT;
		}catch(...){
			cout << "Error unkown exception at local_decl:type IDENT EQUALS literal SEMICOLON" << endl;
		}
	}
    ;

method_statement_list:
    statement method_statement_list
	{
		$$=$2;
		static_cast<BlockStatement*>($$)->prependStatement($1);
	}
    |
    return_statement
	{
		try{
			BlockStatement *temp=new BlockStatement(yylineno);
			if(temp==NULL){
				fatal("factor: ", "could not create variable");
			}else{
				temp->addStatement($1);
			}
			$$=temp;
		}catch(AstException& e){
			cout << "Error: " <<  e.what(); YYABORT;
		}catch(...){
			cout << "Error unkown exception at method_statement_list:return_statement" << endl;
		}
	}
    ;

statement_list:
	statement statement_list
	{
		$$=$2;
		static_cast<BlockStatement*>($$)->addStatement($1);
	}
	/*empty*/
	|
	{
		try{
			$$=new BlockStatement(yylineno);
			if($$==NULL){
				fatal("factor: ", "could not create variable");
			}
		}catch(AstException& e){
			cout << "Error: " <<  e.what(); YYABORT;
		}catch(...){
			cout << "Error unkown exception at statement_list:/empty/" << endl;
		}
	}
    ;

statement:
    IDENT EQUALS expression SEMICOLON
	{
		try{
			$$=new AssignStatement(yylineno, $1, $3);
			if($$==NULL){
				fatal("factor: ", "could not assign to variable");
			}
		}catch(AstException& e){
			cout << "Error: " <<  e.what(); YYABORT;
		}catch(...){
			cout << "Error unkown exception at statement:IDENT EQUALS expression SEMICOLON" << endl;
		}
	}
    |
    FOR L_PAR expression SEMICOLON expression SEMICOLON expression R_PAR statement
	{
			
	}
    |
    IF L_PAR expression R_PAR statement
	{
		try{
			$$=new IfStatement(yylineno, $3, $5, NULL);
			if($$==NULL){
				fatal("factor: ", "could not do a if then");
			}
		}catch(AstException& e){
			cout << "Error: " <<  e.what(); YYABORT;
		}catch(...){
			cout << "Error unkown exception at statement:IF L_PAR expression R_PAR statement" << endl;
		}
	}
    |
    IF L_PAR expression R_PAR statement ELSE statement
	{
		try{
			$$=new IfStatement(yylineno, $3, $5, $7);
			if($$==NULL){
				fatal("factor: ", "could not do and if then else");
			}
		}catch(AstException& e){
			cout << "Error: " <<  e.what(); YYABORT;
		}catch(...){
			cout << "Error unkown exception at statement:IF L_PAR expression R_PAR statement ELSE statement" << endl;
		}
	}
    |
    WHILE L_PAR expression R_PAR statement
	{
		try{
			$$=new WhileStatement(yylineno, $3, $5);
			if($$==NULL){
				fatal("factor: ", "could not do a while loop");
			}
		}catch(AstException& e){
			cout << "Error: " <<  e.what(); YYABORT;
		}catch(...){
			cout << "Error unkown exception at statement:WHILE L_PAR expression R_PAR statement" << endl;
		}
	}
    |
    method_invocation SEMICOLON
	{
		try{
			$$=new MethodCallStatement(yylineno,$1);
			if($$==NULL){
				fatal("factor: ", "could not do rap a madoo");
			}
		}catch(AstException& e){
			cout << "Error: " <<  e.what(); YYABORT;
		}catch(...){
			cout << "Error unkown exception at statement:method_invocation SEMICOLON" << endl;
		}
	}
    |
    L_BRAC statement_list R_BRAC
	{
		$$=$2;
	}
    |
    SEMICOLON
	{
		try{
			$$=new EmptyStatement(yylineno);
			if($$==NULL){
				fatal("factor: ", "could not do rap a madoo");
			}
		}catch(AstException& e){
			cout << "Error: " <<  e.what(); YYABORT;
		}catch(...){
			cout << "Error unkown exception at statement:SEMICOLON" << endl;
		}
	}
    ;

return_statement:
    RETURN expression SEMICOLON
	{
		try{
			$$=new ReturnStatement(yylineno, NULL, $2);
			if($$==NULL){
				fatal("factor: ", "could not create return with expression");
			}
		}catch(AstException& e){
			cout << "Error: " <<  e.what(); YYABORT;
		}catch(...){
			cout << "Error unkown exception at return_statement:RETURN expression SEMICOLON" << endl;
		}
	}
    |
    RETURN SEMICOLON
	{
		try{
			$$=new ReturnStatement(yylineno, NULL, NULL);
			if($$==NULL){
				fatal("factor: ", "could not create return");
			}
		}catch(AstException& e){
			cout << "Error: " <<  e.what(); YYABORT;
		}catch(...){
			cout << "Error unkown exception at return_statement:RETURN SEMICOLON" << endl;
		}
	}
    ;

method_invocation:
    qualified_name L_PAR argument_list R_PAR
	{
		try{
			$$=new MethodCallExpression(yylineno, NULL, $1, $3);
			if($$==NULL){
				fatal("method_invoke: ", "could not invoke method");
			}
		}catch(AstException& e){
			cout << "Error: " <<  e.what(); YYABORT;
		}catch(...){
			cout << "Error unkown exception at method_invocation:qualified_name L_PAR argument_list R_PAR" << endl;
		}
	}
    |
    qualified_name L_PAR R_PAR
	{
		try{
			$$=new MethodCallExpression(yylineno, NULL, $1, NULL);
			if($$==NULL){
				fatal("method_invoke: ", "could not invoke method");
			}
		}catch(AstException& e){
			cout << "Error: " <<  e.what(); YYABORT;
		}catch(...){
			cout << "Error unkown exception at method_invocation:qualified_name L_PAR R_PAR" << endl;
		}
	}
    ;

qualified_name:
    IDENT PERIOD IDENT
	{
		$$=$3;
	}
    |
    IDENT
	{
		$$=$1;
	}
    ;

argument_list:
    expression
	{
		$$=new vector<Expression *>();
		$$->push_back($1);
	}
    |
    expression COMMA argument_list
	{
		$$=$3;
		$$->push_back($1);
	}
    ;

expression:
    relational_expression
	{
		$$=$1;
	}
    |
    relational_expression EQ relational_expression
	{
		try{
			$$=new BinaryExpression(yylineno, AstNode::EQOP, $1, $3);
			if($$==NULL){
				fatal("factor: ", "could not equal");
			}
		}catch(AstException& e){
			cout << "Error: " <<  e.what(); YYABORT;
		}catch(...){
			cout << "Error unkown exception at expression:relational_expression EQ relational_expression" << endl;
		}
	}
    |
    relational_expression NE relational_expression
	{
		try{
			$$=new BinaryExpression(yylineno, AstNode::NEOP, $1, $3);
			if($$==NULL){
				fatal("factor: ", "could not not equal");
			}
		}catch(AstException& e){
			cout << "Error: " <<  e.what(); YYABORT;
		}catch(...){
			cout << "Error unkown exception at expression:relational_expression NE relational_expression" << endl;
		}
	}
	|
    primary_expression INC
	{
		try{
			$$=new UnaryExpression(yylineno, AstNode::INCOP, $1);
			if($$==NULL){
				fatal("factor: ", "could not increment variable");
			}
		}catch(AstException& e){
			cout << "Error: " <<  e.what(); YYABORT;
		}catch(...){
			cout << "Error unkown exception at expression:IDENT INC SEMICOLON" << endl;
		}
	}
    |
    primary_expression DEC
	{
		try{
			$$=new UnaryExpression(yylineno, AstNode::DECOP, $1);
			if($$==NULL){
				fatal("factor: ", "could not increment variable");
			}
		}catch(AstException& e){
			cout << "Error: " <<  e.what(); YYABORT;
		}catch(...){
			cout << "Error unkown exception at expression:IDENT DEC SEMICOLON" << endl;
		}
	}
    ;

relational_expression:
    additive_expression
	{
		$$=$1;
	}
    |
    additive_expression LE additive_expression
	{
		try{
			$$=new BinaryExpression(yylineno, AstNode::LTOP, $1, $3);
			if($$==NULL){
				fatal("factor: ", "could not less than");
			}
		}catch(AstException& e){
			cout << "Error: " <<  e.what(); YYABORT;
		}catch(...){
			cout << "Error unkown exception at relational_expression:additive_expression LE additive_expression" << endl;
		}
	}
    |
    additive_expression GE additive_expression
	{
		try{
			$$=new BinaryExpression(yylineno, AstNode::GTOP, $1, $3);
			if($$==NULL){
				fatal("factor: ", "could not greater than");
			}
		}catch(AstException& e){
			cout << "Error: " <<  e.what(); YYABORT;
		}catch(...){
			cout << "Error unkown exception at relational_expression:additive_expression GE additive_expression" << endl;
		}
	}
    |
    additive_expression LEQ additive_expression
	{
		try{
			$$=new BinaryExpression(yylineno, AstNode::LEOP, $1, $3);
			if($$==NULL){
				fatal("factor: ", "could not less than or eq");
			}
		}catch(AstException& e){
			cout << "Error: " <<  e.what(); YYABORT;
		}catch(...){
			cout << "Error unkown exception at relational_expression:additive_expression LEQ additive_expression" << endl;
		}
	}
    |
    additive_expression GEQ additive_expression
	{
		try{
			$$=new BinaryExpression(yylineno, AstNode::GEOP, $1, $3);
			if($$==NULL){
				fatal("factor: ", "could not greater than or eq");
			}
		}catch(AstException& e){
			cout << "Error: " <<  e.what(); YYABORT;
		}catch(...){
			cout << "Error unkown exception at relational_expression:additive_expression GEQ additive_expression" << endl;
		}
	}
    ;

additive_expression:
    multiplicative_expression
	{
		$$=$1;
	}
    |
    additive_expression PLUS multiplicative_expression
	{
		try{
			$$=new BinaryExpression(yylineno, AstNode::ADDOP, $1, $3);
			if($$==NULL){
				fatal("factor: ", "could not add");
			}
		}catch(AstException& e){
			cout << "Error: " <<  e.what(); YYABORT;
		}catch(...){
			cout << "Error unkown exception at additive_expression:additive_expression PLUS multiplicative_expression" << endl;
		}
	}
    |
    additive_expression MINUS multiplicative_expression
	{
		try{
			$$=new BinaryExpression(yylineno, AstNode::SUBOP, $1, $3);
			if($$==NULL){
				fatal("factor: ", "could not mius");
			}
		}catch(AstException& e){
			cout << "Error: " <<  e.what(); YYABORT;
		}catch(...){
			cout << "Error unkown exception at additive_expression:additive_expression MINUS multiplicative_expression" << endl;
		}
	}
    ;

multiplicative_expression:
    unary_expression
	{
		$$=$1;
	}
    |
    multiplicative_expression TIMES unary_expression
	{
		try{
			$$=new BinaryExpression(yylineno, AstNode::MULOP, $1, $3);
			if($$==NULL){
				fatal("factor: ", "could not multiply");
			}
		}catch(AstException& e){
			cout << "Error: " <<  e.what(); YYABORT;
		}catch(...){
			cout << "Error unkown exception at multiplicative_expression:multiplicative_expression TIMES unary_expression" << endl;
		}
	}
    |
    multiplicative_expression DIVIDE unary_expression
	{
		try{
			$$=new BinaryExpression(yylineno, AstNode::DIVOP, $1, $3);
			if($$==NULL){
				fatal("factor: ", "could not div op");
			}
		}catch(AstException& e){
			cout << "Error: " <<  e.what(); YYABORT;
		}catch(...){
			cout << "Error unkown exception at multiplicative_expression:multiplicative_expression DIVIDE unary_expression" << endl;
		}
	}
    ;

unary_expression:
    primary_expression
	{
		$$=$1;
	}
    |
    PLUS unary_expression
	{
		try{
			$$=new UnaryExpression(yylineno, AstNode::ADDOP, $2);
			if($$==NULL){
				fatal("factor: ", "could not do the PLUS then unary");
			}
		}catch(AstException& e){
			cout << "Error: " <<  e.what(); YYABORT;
		}catch(...){
			cout << "Error unkown exception at unary_expression:PLUS unary_expression" << endl;
		}
	}
    |
    MINUS unary_expression
	{
		try{
			$$=new UnaryExpression(yylineno, AstNode::SUBOP, $2);
			if($$==NULL){
				fatal("factor: ", "could not do the MINUS then unary");
			}
		}catch(AstException& e){
			cout << "Error: " <<  e.what(); YYABORT;
		}catch(...){
			cout << "Error unkown exception at unary_expression:MINUS unary_expression" << endl;
		}
	}
    |
    L_PAR type R_PAR unary_expression
	{
		try{
			$$=new CastExpression(yylineno, $2, $4);
			if($$==NULL){
				fatal("factor: ", "could not cast");
			}
		}catch(AstException& e){
			cout << "Error: " <<  e.what(); YYABORT;
		}catch(...){
			cout << "Error unkown exception at unary_expression:L_PAR type R_PAR unary_expression" << endl;
		}
	}
    ;

primary_expression:
    literal
	{
		$$=$1;
	}
    |
    IDENT
	{
		try{
			$$=new ReferenceExpression(yylineno, $1);
			if($$==NULL){
				fatal("factor: ", "could not get identafier");
			}
		}catch(AstException& e){
			cout << "Error: " <<  e.what(); YYABORT;
		}catch(...){
			cout << "Error unkown exception at primary_expression:IDENT" << endl;
		}
	}
    |
    method_invocation
	{
		$$=$1;
	}
    |
    L_PAR expression R_PAR
	{
		$$=$2;
	}
    ;

type:
    T_INT
	{
		$$=AstNode::TINT;
	}
    |
    T_FLOAT
	{
		$$=AstNode::TFLOAT;
	}
	|
	VOID
	{
		$$=AstNode::TVOID;
	}
    ;

literal:
    D_INT
	{
		try{
			$$=new LiteralExpression(yylineno, $1, AstNode::TINT);
			if($$==NULL){
				fatal("factor: ", "I tried really hard to create your int but it would not work I am sorry.");
			}
		}catch(AstException& e){
			cout << "Error: " <<  e.what(); YYABORT;
		}catch(...){
			cout << "Error unkown exception at literal:D_INT" << endl;
		}
	}
    |
    F_POINT
	{
		try{
			$$=new LiteralExpression(yylineno, $1, AstNode::TFLOAT);
			if($$==NULL){
				fatal("factor: ", "I know this float was important but it just did not work out.");
			}
		}catch(AstException& e){
			cout << "Error: " <<  e.what(); YYABORT;
		}catch(...){
			cout << "Error unkown exception at literal:F_POINT" << endl;
		}
	}	
    |
    STR_L
	{
		try{
			$$=new LiteralExpression(yylineno, $1, AstNode::TSTRING);
			if($$==NULL){
				fatal("factor: ", "Well some things are not meant to be, and apparently this string is one of them.");
			}
		}catch(AstException& e){
			cout << "Error: " <<  e.what(); YYABORT;
		}catch(...){
			cout << "Error unkown exception at literal:STR_L" << endl;
		}
	}
    ;

%%

void yyerror( const char *msg ){}
