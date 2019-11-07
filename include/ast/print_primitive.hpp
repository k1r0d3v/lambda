#ifndef LAMBDA_PRINT_PRIMITIVE_HPP
#define LAMBDA_PRINT_PRIMITIVE_HPP

#include <ast/node.hpp>
#include <ast/native_application.hpp>

namespace ast
{
    class Print : public NativeApplication
    {
    public:
        using Pointer = Node::PointerType<Print>;

    public:
        explicit Print(Node::Pointer argument);
    };
}

#endif //LAMBDA_PRINT_PRIMITIVE_HPP
