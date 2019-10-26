#ifndef LAMBDA_NATURAL_CONSTANT_HPP
#define LAMBDA_NATURAL_CONSTANT_HPP

#include "common.hpp"
#include "node.hpp"
#include "node_type.hpp"
#include "type.hpp"
#include "constant_type.hpp"
#include "typed_value.hpp"

namespace ast
{
    class NaturalConstant : public Node
    {
    public:
        using Pointer = Node::PointerType<NaturalConstant>;
        using NaturalValueType = unsigned long;

        static const constexpr char *TYPE_NAME = "Nat";

    public:
        explicit NaturalConstant(NaturalValueType value)
                : Node(NodeType::NaturalConstant), mValue(value) { }

        const NaturalValueType &value() const
        {
            return mValue;
        }

        Node::Pointer evaluate(Context &context) const override
        {
            return Node::make<NaturalConstant>(mValue); // Can not be evaluated more
        }

        Node::Pointer resolve(Context &context) const override
        {
            return Node::make<TypedValue>(Node::make<NaturalConstant>(mValue), Type::make<ConstantType>(TYPE_NAME));
        }

        Node::Pointer replace(Node::Pointer a, Node::Pointer b) const override
        {
            return this->copy();
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
