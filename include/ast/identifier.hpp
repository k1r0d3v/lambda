#ifndef LAMBDA_IDENTIFIER_HPP
#define LAMBDA_IDENTIFIER_HPP

#include "node_type.hpp"
#include "common.hpp"

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

        Node::Pointer evaluate(const Context &context) const override
        {
            return Node::make<Identifier>(mName); // Can not be evaluated more
        }

        Node::Pointer replace(Node::Pointer a, Node::Pointer b) const override
        {
            auto id = Node::cast<Identifier>(a);
            assert(id != nullptr); // We only replace identifiers

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
