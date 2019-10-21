#ifndef LAMBDA_ABSTRACTION_HPP
#define LAMBDA_ABSTRACTION_HPP

#include <utility>

#include "common.hpp"
#include "identifier.hpp"
#include "node_type.hpp"
#include "util.hpp"

namespace ast
{
    class Abstraction : public Node
    {
    public:
        using Pointer = Node::PointerType<Abstraction>;

    public:
        Abstraction(Identifier::Pointer argument, Node::Pointer body)
                : Node(NodeType::Abstraction), mArgument(std::move(argument)), mBody(std::move(body)) {}

        const Identifier::Pointer &argument() const
        {
            return mArgument;
        }

        const Node::Pointer &body() const
        {
            return mBody;
        }

        Node::Pointer evaluate(const Context &context) const override
        {
            return Node::make<Abstraction>(mArgument, mBody); // Can not be evaluated more
        }

        Node::Pointer copy() const override
        {
            return Node::make<Abstraction>(Node::cast<Identifier>(mArgument->copy()), mBody->copy());
        }

        string toString() const override
        {
            auto os = std::ostringstream();
            os << "(\xce\xbb" << argument()->toString() << ". "
               << util::replaceIdentifier(mArgument, body(), mArgument)->toString() << ")";
            return os.str();
        }

    private:
        Identifier::Pointer mArgument;
        Node::Pointer mBody;
    };
}

#endif
