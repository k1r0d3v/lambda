#ifndef LAMBDA_NATURAL_PRIMITIVES_HPP
#define LAMBDA_NATURAL_PRIMITIVES_HPP

#include "native_application.hpp"

namespace ast
{
    class IsZero : public NativeApplication
    {
    public:
        using Pointer = Node::PointerType<IsZero>;

    public:
        explicit IsZero(Node::Pointer argument);
    };

    class Successor : public NativeApplication
    {
    public:
        using Pointer = Node::PointerType<IsZero>;

    public:
        explicit Successor(Node::Pointer argument);
    };

    class Predecessor : public NativeApplication
    {
    public:
        using Pointer = Node::PointerType<IsZero>;

    public:
        explicit Predecessor(Node::Pointer argument);
    };
}

#endif //LAMBDA_NATURAL_PRIMITIVES_HPP
