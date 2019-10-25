#ifndef LAMBDA_NATURAL_HPP
#define LAMBDA_NATURAL_HPP

#include "common.hpp"
#include "node.hpp"
#include "node_type.hpp"

namespace ast
{
    class Natural : public Node
    {
    public:
        using Pointer = Node::PointerType<Natural>;
        using NaturalType = unsigned long;

    public:
        explicit Natural(NaturalType value)
                : Node(NodeType::Natural), mValue(value) { }

        const NaturalType &value() const
        {
            return mValue;
        }

        Node::Pointer evaluate(Context &context) const override
        {
            return Node::make<Natural>(mValue); // Can not be evaluated more
        }

        Node::Pointer replace(Node::Pointer a, Node::Pointer b) const override
        {
            return this->copy();
        }

        Node::Pointer copy() const override
        {
            return Node::make<Natural>(mValue);
        }

        string toString() const override
        {
            return to_string(mValue);
        }

    private:
        NaturalType mValue;
    };
}

#endif //LAMBDA_NATURAL_HPP
