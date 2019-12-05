#ifndef LAMBDA_LIST_PRIMITIVES_HPP
#define LAMBDA_LIST_PRIMITIVES_HPP

#include <ast/native_application.hpp>

namespace ast
{
    class IsEmpty : public NativeApplication
    {
    public:
        using Pointer = Node::PointerType<IsEmpty>;

    public:
        explicit IsEmpty(Node::Pointer argument);
    };
}

#endif //LAMBDA_LIST_PRIMITIVES_HPP
