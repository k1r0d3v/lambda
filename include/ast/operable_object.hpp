#ifndef LAMBDA_OPERABLE_OBJECT_HPP
#define LAMBDA_OPERABLE_OBJECT_HPP

#include <ast/node.hpp>

namespace ast
{
    class OperableObject
    {
    public:
        virtual Node::Pointer operator_dot(const Node::Pointer &b, Context &context) const = 0;
    };
}
#endif //LAMBDA_OPERABLE_OBJECT_HPP
