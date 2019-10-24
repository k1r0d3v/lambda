#ifndef LAMBDA_UNIT_HPP
#define LAMBDA_UNIT_HPP

#include "common.hpp"
#include "node_type.hpp"
#include "node.hpp"

namespace ast
{
    class Unit : public Node
    {
    public:
        using Pointer = Node::PointerType<Unit>;

    public:
        explicit Unit()
                : Node(NodeType::Unit) { }

        Node::Pointer evaluate(const Context &context) const override
        {
            return this->copy();
        }

        Node::Pointer replace(Node::Pointer a, Node::Pointer b) const override
        {
            return this->copy();
        }

        Node::Pointer copy() const override
        {
            return Node::make<Unit>();
        }

        string toString() const override
        {
            return "() : Unit";
        }
    };
}


#endif //LAMBDA_UNIT_HPP
