#ifndef LAMBDA_BOOLEAN_CONSTANT_HPP
#define LAMBDA_BOOLEAN_CONSTANT_HPP

#include <ast/types/bool_type.hpp>
#include "common.hpp"
#include "node.hpp"
#include "node_type.hpp"
#include "ast/types/constant_type.hpp"

namespace ast
{
    class BooleanConstant : public Node
    {

    public:
        using Pointer = Node::PointerType<BooleanConstant>;

        static const constexpr char *TYPE_NAME = "Bool";

    public:
        explicit BooleanConstant(bool value)
                : Node(NodeType::BooleanConstant), mValue(value)
        { }

        const bool &value() const
        {
            return mValue;
        }

        Node::Pointer evaluate(Context &context) const override
        {
            return Node::make<BooleanConstant>(mValue);
        }

        Node::Pointer freeze(Context &context) const override
        {
            return Node::make<BooleanConstant>(mValue);
        }

        Type::Pointer typecheck(TypeContext &context) const override
        {
            return BoolType::BOOL;
        }

        Node::Pointer copy() const override
        {
            return Node::make<BooleanConstant>(mValue);
        }

        string toString() const override
        {
            return string(mValue ? "true" : "false");
        }

    private:
        bool mValue;
    };
}

#endif //LAMBDA_BOOLEAN_CONSTANT_HPP
