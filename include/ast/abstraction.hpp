#ifndef LAMBDA_ABSTRACTION_HPP
#define LAMBDA_ABSTRACTION_HPP

#include "common.hpp"
#include "identifier.hpp"
#include "node_type.hpp"
#include "variable.hpp"

namespace ast
{
    class Abstraction : public Node
    {
    public:
        using Pointer = Node::PointerType<Abstraction>;

    public:
        Abstraction(Variable::Pointer argument, const Node::Pointer& body)
                : Node(NodeType::Abstraction), mArgument(std::move(argument))
        {
            mBody = body->replace(Node::make<Identifier>(mArgument->name()), mArgument);
        }

        const Variable::Pointer &argument() const
        {
            return mArgument;
        }

        const Node::Pointer &body() const
        {
            return mBody;
        }

        Node::Pointer evaluate(Context &context) const override
        {
            // Can not be evaluated more
            return Node::make<Abstraction>(mArgument, mBody);
        }

        Node::Pointer resolve(const Context &context) const override
        {
            return Node::make<Abstraction>(mArgument, mBody->resolve(context));
        }

        Node::Pointer replace(Node::Pointer a, Node::Pointer b) const override
        {
            // Treat this like a node but no delete it
            auto abs = Node::makeNoDeletablePtr(this);

            // Duplicated variable names inside abstraction
            if (a->type() == NodeType::Variable && mArgument->name() == Node::cast<Variable>(a)->name())
            {
                // FIXME: Esto puede causar problemas si en el body ya hay un @code{_x}
                // Generate new name
                auto _x = Node::make<Variable>(mArgument->name() + "\'");
                // Replace in body
                abs = Node::make<Abstraction>(_x, mBody->replace(mArgument, _x));
            }

            return Node::make<Abstraction>(abs->argument(), abs->body()->replace(a, b));
        }

        Node::Pointer copy() const override
        {
            return Node::make<Abstraction>(Node::cast<Variable>(mArgument->copy()), mBody->copy());
        }

        string toString() const override
        {
            // Note: Replace will rename problematic entries

            auto os = std::ostringstream();
            os << "(\xce\xbb" << argument()->toString() << ". "
               << mBody->replace(mArgument, mArgument)->toString() << ")";
            return os.str();
        }

    private:
        Variable::Pointer mArgument;
        Node::Pointer mBody;
    };
}

#endif
