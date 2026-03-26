cmake_minimum_required(VERSION 3.1)
project(P2C-Transpiler)


find_package(FLEX)
find_package(BISON)
if(FLEX_FOUND AND BISON_FOUND)
  BISON_TARGET(Parser ${CMAKE_BINARY_DIR}/../src/parser.y  ${CMAKE_BINARY_DIR}/../src/parser.tab.cc  DEFINES_FILE  ${CMAKE_BINARY_DIR}/../include/parser.tab.h COMPILE_FLAGS "-t")
  FLEX_TARGET(Lexer  ${CMAKE_BINARY_DIR}/../src/lex.l  ${CMAKE_BINARY_DIR}/../src/lex.yy.cc)
  ADD_FLEX_BISON_DEPENDENCY(Lexer Parser)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

include_directories("include")

message(STATUS "Linux")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__linux__ -Wno-error=register")

file(GLOB_RECURSE include "include/*.h")
file(GLOB_RECURSE src "src/*.c" "src/*.cc")

add_executable(P2C-Transpiler
        ${src}
        src/lex.yy.cc
        src/parser.tab.cc
        )



