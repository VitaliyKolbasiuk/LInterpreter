cmake_minimum_required(VERSION 3.16.3)

set(CMAKE_CXX_STANDARD 20)

project(LInterpreter)

set( SRC_LIST interpreter/interpreter.cpp interpreter/LInterpreter.cpp interpreter/Scanner.h interpreter/SExpr.h interpreter/Parser.h
interpreter/LInterpreter.h interpreter/Log.h interpreter/pseudoTable.cpp)

add_subdirectory(Oleksii)
#add_executable(LInterpreter ${SRC_LIST})
