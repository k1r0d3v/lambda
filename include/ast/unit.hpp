#ifndef LAMBDA_UNIT_HPP
#define LAMBDA_UNIT_HPP

#include "common.hpp"
#include "node_type.hpp"
#include "node.hpp"
#include "constant_type.hpp"

namespace ast
{
    class Unit : public Node
    {
    public:
        using Pointer = Node::PointerType<Unit>;

        static const constexpr char *TYPE_NAME = "Unit";

    public:
        explicit Unit()
                : Node(NodeType::Unit) 
        {
            this->setType(Type::make<ConstantType>(TYPE_NAME));
        }

        Node::Pointer evaluate(Context &context) const override
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
            return "()";
        }
    };
}


#endif //LAMBDA_UNIT_HPP
