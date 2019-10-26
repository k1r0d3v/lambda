#ifndef LAMBDA_BOOLEAN_CONSTANT_HPP
#define LAMBDA_BOOLEAN_CONSTANT_HPP

#include "common.hpp"
#include "node.hpp"
#include "node_type.hpp"
#include "typed_value.hpp"
#include "constant_type.hpp"

namespace ast
{
    class BooleanConstant : public Node
    {
    public:
        using Pointer = Node::PointerType<BooleanConstant>;

        static const constexpr char *TYPE_NAME = "Bool";

    public:
        explicit BooleanConstant(bool value)
                : Node(NodeType::BooleanConstant), mValue(value) { }

        const bool &value() const
        {
            return mValue;
        }

        Node::Pointer evaluate(Context &context) const override
        {
            return Node::make<BooleanConstant>(mValue); // Can not be evaluated more
        }

        Node::Pointer resolve(Context &context) const override
        {
            return Node::make<TypedValue>(Node::make<BooleanConstant>(mValue), Type::make<ConstantType>(TYPE_NAME));
        }

        Node::Pointer replace(Node::Pointer a, Node::Pointer b) const override
        {
            return this->copy();
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
