#ifndef LAMBDA_ABSTRACTION_HPP
#define LAMBDA_ABSTRACTION_HPP

#include "common.hpp"
#include "identifier.hpp"
#include "node_type.hpp"
#include "util.hpp"

namespace ast
{
    class Abstraction : public Node
    {
    public:
        Abstraction(Identifier::Reference argument, Node::Reference body)
                : Node(NodeType::Abstraction), mArgument(argument), mBody(body) {}

        const Identifier::Reference &argument() const
        {
            return mArgument;
        }

        const Node::Reference &body() const
        {
            return mBody;
        }

        Node::Reference evaluate(const Context &context) const override
        {
            return Node::make<Abstraction>(mArgument, mBody); // Can not be evaluated more
        }

        Node::Reference copy() const override
        {
            return Node::make<Abstraction>(mArgument->copyCast<Identifier>(), mBody->copy());
        }

        string toString() const override
        {
            auto os = std::ostringstream();
            os << "(\xce\xbb" << argument()->toString() << ". "
               << util::replaceIdentifier(mArgument, body(), mArgument)->toString() << ")";
            return os.str();
        }

    private:
        Identifier::Reference mArgument;
        Node::Reference mBody;
    };
}

#endif
