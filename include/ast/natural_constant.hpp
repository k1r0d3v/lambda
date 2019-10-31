#ifndef LAMBDA_NATURAL_CONSTANT_HPP
#define LAMBDA_NATURAL_CONSTANT_HPP

#include <ast/types/nat_type.hpp>
#include "common.hpp"
#include "node.hpp"
#include "node_type.hpp"
#include "ast/types/type.hpp"
#include "ast/types/constant_type.hpp"

namespace ast
{
    class NaturalConstant : public Node
    {
    public:
        using Pointer = Node::PointerType<NaturalConstant>;
        using NaturalValueType = unsigned long;

    public:
        explicit NaturalConstant(NaturalValueType value)
                : Node(NodeType::NaturalConstant), mValue(value)
        { }

        const NaturalValueType &value() const
        {
            return mValue;
        }

        Node::Pointer evaluate(const Node::Pointer &self, Context &context) const override
        {
            return self;
        }

        Type::Pointer typecheck(TypeContext &context) const override
        {
            return NatType::INSTANCE;
        }

        Node::Pointer copy() const override
        {
            return Node::make<NaturalConstant>(mValue);
        }

        string toString() const override
        {
            return to_string(mValue);
        }

    private:
        NaturalValueType mValue;
    };
}

#endif //LAMBDA_NATURAL_CONSTANT_HPP
