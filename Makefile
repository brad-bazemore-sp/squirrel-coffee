#Change this to the location where the source files are
#Yes yes I know I need to move to CMake but I have other things to do

work_space =/Users/bradford_bazemore/ClionProjects/littleJava

CXX		= g++
CPPFLAGS	= -I$(work_space)/ast
CXXFLAGS	= -g -std=gnu++11
LDFLAGS		= -L$(work_space)/ast
LDLIBS		= -lAst -lfl
YACC            = bison -y -rall -fcaret -t
LEX             = flex
#YACC            = bison -y -r all -fcaret --graph -t
#LEX             = flex -d

SRCS = \
main_tjc.cpp lex.yy.c y.tab.c AstPrinter.cpp \
SymbolTable.cpp SemanticCheck.cpp CodeGenerator.cpp


HDRS = \
y.tab.h Ast.h Class.h Entry.h \
Field.h Method.h Package.h Parameter.h \
SymbolTable.h SymbolTableException.h Variable.h \
SemanticCheck.h SemanticException.h CodeGenerator.h \
JSMOpps.h


OBJS = \
main_tjc.o y.tab.o lex.yy.o AstPrinter.o \
SymbolTable.o SemanticCheck.o CodeGenerator.o



# compile rules
#
COMPILE.cpp     = $(CXX) $(CPPFLAGS) $(CXXFLAGS)  -c
LINK.cpp        = $(CXX) $(CXXFLAGS) $(LDFLAGS)

.SUFFIXES: .cpp

.cpp.o:
	$(COMPILE.cpp) $(OUTPUT_OPTION) $<

# non-standard target dependencies
CodeGenerator.o: CodeGenerator.cpp $(work_space)/ast/Ast.h
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) CodeGenerator.cpp

SymbolTable.o: SymbolTable.cpp $(work_space)/ast/Ast.h
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) SymbolTable.cpp

SemanticCheck.o: SemanticCheck.cpp $(work_space)/ast/Ast.h
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) SemanticCheck.cpp

y.tab.o: Makefile y.tab.c $(work_space)/ast/Ast.h
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) y.tab.c

lex.yy.o: Makefile lex.yy.c y.tab.h $(work_space)/ast/Ast.h
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) lex.yy.c

y.tab.c y.tab.h: Makefile tjc.y
	$(YACC) -dv tjc.y

lex.yy.c: Makefile tjc.l
	$(LEX) tjc.l

tjc: Makefile $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS) $(LDLIBS)

all: tjc

#clean targets
clean:
	/bin/rm -f *.o
	/bin/rm -f *.output
	/bin/rm -f y.dot
	/bin/rm -f y.tab.c
	/bin/rm -f y.tab.h
	/bin/rm -f lex.yy.c

#################################################################
#################################################################
## Dependencies
##
main_tjc.o: main_tjc.cpp $(work_space)/ast/Ast.h
