#ifndef LAMBDA_TYPED_VALUE_HPP
#define LAMBDA_TYPED_VALUE_HPP

#include "common.hpp"
#include "node.hpp"
#include "node_type.hpp"
#include "type.hpp"

namespace ast
{
    class TypedValue : public Node
    {
    public:
        using Pointer = Node::PointerType<TypedValue>;

    public:
        explicit TypedValue(Node::Pointer value, Type::Pointer type)
                : Node(NodeType::TypedValue), mValue(std::move(value)), mType(std::move(type)) { }

        Node::Pointer value() const {
            return mValue;
        }

        Type::Pointer type() const {
            return mType;
        }

        Node::Pointer evaluate(Context &context) const override
        {
            return mValue->evaluate(context);
        }

        Node::Pointer resolve(Context &context) const override
        {
            return mValue->resolve(context);
        }

        Node::Pointer replace(Node::Pointer a, Node::Pointer b) const override
        {
            return mValue->replace(a, b);
        }

        Node::Pointer copy() const override
        {
            return Node::make<TypedValue>(mValue, mType);
        }

        string toString() const override
        {
            return mValue->toString();
        }

    private:
        Node::Pointer mValue;
        Type::Pointer mType;
    };
}

#endif //LAMBDA_TYPED_VALUE_HPP
