#ifndef LAMBDA_OPERABLE_NODE_HPP
#define LAMBDA_OPERABLE_NODE_HPP

#include <ast/node.hpp>

namespace ast
{
    class OperableNode
    {
    public:
        /**
         *  Is used to access a especific value of subclass. Example: tuple, register...
         * @return value
         */
        virtual Node::Pointer operatorDot(const Node::Pointer &b, Context &context) const = 0;
    };
}
#endif //LAMBDA_OPERABLE_NODE_HPP
