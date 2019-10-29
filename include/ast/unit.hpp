#ifndef LAMBDA_UNIT_HPP
#define LAMBDA_UNIT_HPP

#include <ast/types/unit_type.hpp>
#include "common.hpp"
#include "node_type.hpp"
#include "node.hpp"
#include "ast/types/constant_type.hpp"

namespace ast
{
    class Unit : public Node
    {
    public:
        using Pointer = Node::PointerType<Unit>;

    public:
        explicit Unit() : Node(NodeType::Unit)
        { }

        Node::Pointer evaluate(Context &context) const override
        {
            return Node::make<Unit>();
        }

        Node::Pointer freeze(Context &context) const override
        {
            return Node::make<Unit>();
        }

        Type::Pointer typecheck(TypeContext &context) const override
        {
            return UnitType::UNIT;
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
