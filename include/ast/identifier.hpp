#ifndef LAMBDA_IDENTIFIER_HPP
#define LAMBDA_IDENTIFIER_HPP

#include "node.hpp"
#include "node_type.hpp"
#include "common.hpp"
#include "exception.hpp"
#include "context.hpp"
#include "typed_value.hpp"

namespace ast
{
    class Identifier : public Node
    {
    public:
        using Pointer = Node::PointerType<Identifier>;

    public:
        explicit Identifier(string name)
                : Node(NodeType::Identifier), mName(std::move(name)) { }

        const string &name() const
        {
            return mName;
        }

        Node::Pointer evaluate(Context &context) const override
        {
            // Can not be evaluated more
            return this->copy();
        }

        Node::Pointer resolve(Context &context) const override
        {
            auto value = context.getValue(mName);
            auto type = context.getType(mName);

            if (value == nullptr)
                throw NameException("name '" + mName + "' is not defined");
            if (type == nullptr)
                throw TypeException("Expected a typed value");

            return Node::make<TypedValue>(value, type);
        }

        Node::Pointer replace(Node::Pointer a, Node::Pointer b) const override
        {
            if (a->nodeType() == NodeType::Identifier)
            {
                auto id = Node::cast<Identifier>(a);
                if (id->name() == mName)
                    return b;
            }

            return this->copy();
        }

        Node::Pointer copy() const override
        {
            return Node::make<Identifier>(mName);
        }

        string toString() const override
        {
            return mName;
        }

    private:
        string mName;
    };
}

#endif
