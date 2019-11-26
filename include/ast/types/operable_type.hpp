#ifndef LAMBDA_OPERABLE_TYPE_HPP
#define LAMBDA_OPERABLE_TYPE_HPP

#include <ast/types/type.hpp>
#include <ast/node.hpp>

namespace ast
{
    class OperableType : public Type
    {
    public:
        using Pointer = Type::PointerType<OperableType>;

    public:
        explicit OperableType(int kind) : Type(kind) { }

        virtual Type::Pointer typeOfDot(const Node::Pointer &b) = 0;
    };
}


#endif //LAMBDA_OPERABLE_TYPE_HPP
