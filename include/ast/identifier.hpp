#ifndef LAMBDA_IDENTIFIER_HPP
#define LAMBDA_IDENTIFIER_HPP

#include "node_type.hpp"
#include "common.hpp"
#include "exception.hpp"

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
            /*
            auto value = context.getValueOfId(mName);
            if (value == nullptr)
                throw NameException("\'" + mName + "\' is not defined");
            return value;
            */
            return this->copy();
        }

        Node::Pointer replace(Node::Pointer a, Node::Pointer b) const override
        {
            // We only replace identifiers
            if (a->type() != NodeType::Identifier)
                return this->copy();

            auto id = Node::cast<Identifier>(a);
            if (id->name() == mName)
                return b;
            else
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
