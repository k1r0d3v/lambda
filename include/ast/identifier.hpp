#ifndef LAMBDA_IDENTIFIER_HPP
#define LAMBDA_IDENTIFIER_HPP

#include "node.hpp"
#include "node_type.hpp"
#include "common.hpp"
#include "exception.hpp"
#include "context.hpp"

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

        const Node::Pointer &value() const
        {
            return mValue;
        }

        Node::Pointer evaluate(Context &context) const override
        {
            if (mValue != nullptr) return mValue->evaluate(context);
            else throw NameException("Unresolved identifiers can not be evaluated");
        }

        void resolve(Context &context) override
        {
            // Note: Only called when we have a single id
            if (mValue == nullptr)
            {
                auto value = context.getValue(mName);

                if (value == nullptr)
                    throw NameException("name '" + mName + "' is not defined");

                if (value->type() == nullptr)
                    throw TypeException("Expected a typed value");

                mValue = value;
                this->setType(mValue->type());
            }
        }

        Node::Pointer replace(Node::Pointer a, Node::Pointer b) const override
        {
            if (mValue == nullptr && a->nodeType() == NodeType::Identifier)
            {
                auto id = Node::cast<Identifier>(a);
                if (id->name() == mName)
                    return b;
            }

            return this->copy();
        }

        Node::Pointer copy() const override
        {
            auto copy = Node::make<Identifier>(mName);
            copy->mValue = mValue;
            copy->setType(this->type());
            return copy;
        }

        string toString() const override
        {
            return mValue == nullptr ? mName : mValue->toString();
        }

    private:
        string mName;
        Node::Pointer mValue;
    };
}

#endif
