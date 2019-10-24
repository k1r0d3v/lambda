#ifndef LAMBDA_BOOLEAN_HPP
#define LAMBDA_BOOLEAN_HPP

#include "common.hpp"
#include "node.hpp"
#include "node_type.hpp"

namespace ast
{
    class Boolean : public Node
    {
    public:
        using Pointer = Node::PointerType<Boolean>;

    public:
        explicit Boolean(bool value)
                : Node(NodeType::Boolean), mValue(value) { }

        const bool &value() const
        {
            return mValue;
        }

        Node::Pointer evaluate(const Context &context) const override
        {
            return Node::make<Boolean>(mValue); // Can not be evaluated more
        }

        Node::Pointer replace(Node::Pointer a, Node::Pointer b) const override
        {
            return this->copy();
        }

        Node::Pointer copy() const override
        {
            return Node::make<Boolean>(mValue);
        }

        string toString() const override
        {
            return string(mValue ? "true" : "false");
        }

    private:
        bool mValue;
    };
}

#endif //LAMBDA_BOOLEAN_HPP
