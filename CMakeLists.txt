cmake_minimum_required(VERSION 3.14)
project(lambda)

set(CMAKE_CXX_STANDARD 17)

find_package(BISON)
find_package(FLEX)

# Create flex/bison output directory
set(FLEX_BISON_OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/generated/lambda)
file(MAKE_DIRECTORY ${FLEX_BISON_OUTPUT_DIR})

# Add flex and bison
BISON_TARGET(lambda_parser source/parser.y ${FLEX_BISON_OUTPUT_DIR}/parser.cpp VERBOSE ${FLEX_BISON_OUTPUT_DIR})
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
        include/ast/node_kind.hpp
        include/driver.hpp
        include/ast_driver.hpp
        source/driver.cpp
        include/ast/natural_constant.hpp
        include/ast/float_constant.hpp
        include/ast/boolean_constant.hpp
        include/ast/conditional.hpp
        include/ast/local_definition.hpp
        include/ast/natural_primitives.hpp
        include/ast/sequence.hpp
        include/ast/unit.hpp
        include/interpreter.hpp
        include/ast/exception.hpp
        include/terminal_colors.hpp
        include/ast/variable.hpp
        include/ast/declaration.hpp
        include/ast/types/type.hpp
        include/ast/types/constant_type.hpp
        include/ast/fix.hpp
        include/ast/node_visitor.hpp
        include/ast/types/arrow_type.hpp
        include/ast/types/bool_type.hpp
        include/ast/types/nat_type.hpp
        include/ast/types/float_type.hpp
        include/ast/types/type_context.hpp
        source/ast/types/type_instances.cpp
        include/ast/types/unit_type.hpp
        include/ast/identifier.hpp
        include/ast/native_application.hpp
        include/ast/types/str_type.hpp
        include/ast/string_constant.hpp
        include/ast/print_primitive.hpp
        include/ast/ascription.hpp
        include/ast/types/register_type.hpp
        include/ast/register.hpp
        include/ast/operator_dot.hpp
        include/ast/types/tuple_type.hpp
        include/ast/tuple.hpp
        include/ast/types/top_type.hpp
        include/ast/pattern.hpp
        source/ast/abstraction.cpp
        source/ast/variable.cpp
        source/ast/application.cpp
        source/ast/ascription.cpp
        source/ast/boolean_constant.cpp
        source/ast/conditional.cpp
        source/ast/context.cpp
        source/ast/declaration.cpp
        source/ast/identifier.cpp
        source/ast/local_definition.cpp
        source/ast/native_application.cpp
        source/ast/natural_constant.cpp
        source/ast/float_constant.cpp
        source/ast/natural_primitives.cpp
        source/ast/operator_dot.cpp
        source/ast/print_primitive.cpp
        source/ast/register.cpp
        source/ast/sequence.cpp
        source/ast/string_constant.cpp
        source/ast/tuple.cpp
        source/ast/unit.cpp
        source/ast/fix.cpp
        source/ast/types/constant_type.cpp
        source/ast/types/arrow_type.cpp
        source/ast/types/register_type.cpp
        source/ast/types/tuple_type.cpp
        source/ast/node.cpp
        include/ast/operable_object.hpp
        include/ast/types/operable_type.hpp
        source/ast/node_visitor.cpp
        source/ast/alias.cpp
        include/ast/alias.hpp
        include/ast/types/undefined_type.hpp
        source/ast/types/undefined_type.cpp)

# Add include folder to include directories
target_include_directories(lambda PRIVATE include/)
target_include_directories(lambda PRIVATE ${FLEX_BISON_OUTPUT_DIR})