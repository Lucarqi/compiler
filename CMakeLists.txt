cmake_minimum_required(VERSION 3.0.0)
project(sysy)
set(CMAKE_CXX_STANDARD 20)
SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb -fPIC")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall -fPIC")

find_package(BISON)
find_package(FLEX)

make_directory(${CMAKE_CURRENT_BINARY_DIR}/parser)
BISON_TARGET(Parser src/parser/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser/parser.cpp)
FLEX_TARGET(Scanner src/parser/lexer.l ${CMAKE_CURRENT_BINARY_DIR}/parser/lexer.cpp)
ADD_FLEX_BISON_DEPENDENCY(Scanner Parser)

include_directories(${CMAKE_CURRENT_BINARY_DIR}/parser)
include_directories(src)

file(GLOB_RECURSE srcs src/*.cpp)

add_executable(sysy
    ${srcs}
    ${BISON_Parser_OUTPUTS}
    ${FLEX_Scanner_OUTPUTS}    
)
