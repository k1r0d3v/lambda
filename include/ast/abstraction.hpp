#ifndef LAMBDA_ABSTRACTION_HPP
#define LAMBDA_ABSTRACTION_HPP

#include <cassert>
#include "common.hpp"
#include "identifier.hpp"
#include "node_type.hpp"

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

        Node::Pointer replace(Node::Pointer a, Node::Pointer b) const override
        {
            auto id = Node::cast<Identifier>(a);
            assert(id != nullptr); // We expect a id to be replaced

            auto abs = Node::makeNoDeletablePtr(this);

            // Duplicated variable name inside abstraction
            if (mArgument->name() == id->name())
            {
                // FIXME: Esto puede causar problemas si en el body ya hay un @code{_x}
                // Generate new name
                auto _x = Node::make<Identifier>(mArgument->name() + "\'");
                // Replace in body
                abs = Node::make<Abstraction>(_x, mBody->replace(mArgument, _x));
            }

            return Node::make<Abstraction>(abs->argument(), abs->body()->replace(a, b));
        }

        Node::Pointer copy() const override
        {
            return Node::make<Abstraction>(Node::cast<Identifier>(mArgument->copy()), mBody->copy());
        }

        string toString() const override
        {
            auto os = std::ostringstream();
            os << "(\xce\xbb" << argument()->toString() << ". "
               << mBody->replace(mArgument, mArgument)->toString() << ")";
            return os.str();
        }

    private:
        Identifier::Pointer mArgument;
        Node::Pointer mBody;
    };
}

#endif
