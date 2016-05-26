#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <fstream>

#include "Ast.h"
#include "AstPrinter.h"
#include "SymbolTable.h"
#include "SemanticCheck.h"
#include "CodeGenerator.h"

void fatal(const char *, const char *);

void error(const char *fmt, ...);


char *pgm_name= nullptr;
extern FILE *yyin;
extern int yylineno;

int yyparse(void);

Declaration *root = nullptr;
Statement *method_body_block = nullptr;

int errCnt = 0;
//#define YYDEBUG
#ifdef YYDEBUG
extern int yydebug;
#endif


int main(int argc, char **argv) {
    pgm_name = argv[0];
    std::string file_name;
#ifdef YYDEBUG
    yydebug = 1;
#endif
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (yyin == NULL) {
            fatal("Cannot open input file: ", argv[1]);
        }
        file_name=std::string(argv[1]);
        file_name=file_name.substr(0,file_name.size()-5);
    }
    cout << "Parsing..." << endl;
    cout << "Building Abstract Syntax Tree..." << endl;
    yyparse();
    if (errCnt == 0 && root != NULL) {
        cout << "Building symbol table..." << endl;
        root->accept(new SymbolTable());
        cout << "############Symbol table:############" << endl;
        symbolTable["Default Package"]->toString();
        cout << "#####################################" << endl;
        cout << "Analyzing semantics..." << endl;
        root->accept(new SemanticCheck());
        cout << "########Abstract syntax tree:########" << endl;
        root->accept(new AstPrinter(0));
        cout << "#####################################" << endl;
        cout << "Generating assembly code..." << endl;
        root->accept(new CodeGenerator());
        cout << "###########Assembly code:############" << endl;
        cout << jsm_code << endl;
        cout << "#####################################" << endl;
        cout << "Writing to file..." << endl;
        std::ofstream out(file_name+".jsm");
        out << jsm_code;
        out.close();
        std::string cmd="java -jar jasmin-2.4/jasmin.jar "+file_name+".jsm";
        system(cmd.c_str());
    }else{
        cout <<"Parsing error!!!"<<endl;
    }
    return 0;
}

void error(const char *fmt, ...) {
    va_list ap;
    errCnt++;
    va_start(ap, fmt);
    fprintf(stderr, "Line %d: ", yylineno);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    fflush(stderr);
}

void fatal(const char *msg1, const char *msg2) {
    printf("%s%s\n", msg1, msg2);
    exit(1);
}
