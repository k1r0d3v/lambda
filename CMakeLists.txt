cmake_minimum_required(VERSION 3.14)
project(lambda)

set(CMAKE_CXX_STANDARD 17)

find_package(BISON)
find_package(FLEX)

# Create flex/bison output directory
set(FLEX_BISON_OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/generated/lambda)
file(MAKE_DIRECTORY ${FLEX_BISON_OUTPUT_DIR})

# Add flex and bison
BISON_TARGET(lambda_parser source/parser.y ${FLEX_BISON_OUTPUT_DIR}/parser.cpp)
FLEX_TARGET(lambda_lexer source/lexer.l ${FLEX_BISON_OUTPUT_DIR}/lexer.cpp COMPILE_FLAGS "--header-file=${FLEX_BISON_OUTPUT_DIR}/lexer.hpp")
ADD_FLEX_BISON_DEPENDENCY(lambda_lexer lambda_parser)


add_executable(lambda
        ${BISON_lambda_parser_OUTPUTS}
        ${FLEX_lambda_lexer_OUTPUTS}
        ${FLEX_lambda_lexer_OUTPUT_HEADER}
        source/main.cpp
        include/ast/ast.hpp
        include/ast/context.hpp
        include/ast/node.hpp
        include/ast/common.hpp
        include/ast/application.hpp
        include/ast/abstraction.hpp
        include/ast/identifier.hpp
        include/ast/node_type.hpp
        include/driver.hpp
        include/ast_driver.hpp source/driver.cpp include/ast/natural.hpp include/ast/boolean.hpp include/ast/conditional.hpp include/ast/local_definition.hpp include/ast/natural_primitives.hpp)

# Add include folder to include directories
target_include_directories(lambda PRIVATE include/)
target_include_directories(lambda PRIVATE ${FLEX_BISON_OUTPUT_DIR})