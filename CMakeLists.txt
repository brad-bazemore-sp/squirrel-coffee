project(tjc_part_2b)
cmake_minimum_required(VERSION 3.5)

include_directories(ast)

find_package(BISON)
find_package(FLEX)

BISON_TARGET(y.tab.o tjc.y ${CMAKE_CURRENT_BINARY_DIR}/y.tab.c)
FLEX_TARGET(lex.yy.o tjc.l  ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.c)
ADD_FLEX_BISON_DEPENDENCY(lex.yy.o y.tab.o)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE_FILES main_tjc.cpp AstPrinter.cpp SymbolTable.cpp SemanticCheck.cpp CodeGenerator.cpp)
add_executable(tjc ${SOURCE_FILES})
target_link_libraries(tjc lfl lAst)