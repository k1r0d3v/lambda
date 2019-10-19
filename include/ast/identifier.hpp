#ifndef LAMBDA_IDENTIFIER_HPP
#define LAMBDA_IDENTIFIER_HPP

#include "node_type.hpp"
#include "common.hpp"

namespace ast
{
    class Identifier : public Node
    {
    public:
        using Reference = std::shared_ptr<Identifier>;

    public:
        Identifier(string name)
                : Node(NodeType::Identifier), mName(name) {}

        const string &name() const
        {
            return mName;
        }

        Node::Reference evaluate(const Context &context) const override
        {
            return Node::make<Identifier>(mName); // Can not be evaluated more
        }

        Node::Reference copy() const override
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
