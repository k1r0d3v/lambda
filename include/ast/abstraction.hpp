#ifndef LAMBDA_ABSTRACTION_HPP
#define LAMBDA_ABSTRACTION_HPP

#include "common.hpp"
#include "identifier.hpp"
#include "node_type.hpp"
#include "variable.hpp"
#include "abstraction_type.hpp"

namespace ast
{
    class Abstraction : public Node
    {
    public:
        using Pointer = Node::PointerType<Abstraction>;

    public:
        Abstraction(Variable::Pointer argument, const Node::Pointer& body)
                : Node(NodeType::Abstraction),
                  mArgument(std::move(argument))
        {
            // The replacement of id's by variables
            // is to avoid ugly situations like:
            // let foo = x in λx. λy. x y foo;
            // with result: λx. λy. x y x
            //
            // But with variables we have instead:
            // with result: λx. λy. var(x) var(y) x
            // witch is correct
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

        Node::Pointer resolve(Context &context) const override
        {
            auto resolvedBody = Node::cast<TypedValue>(mBody->resolve(context));
            return Node::make<TypedValue>(Node::make<Abstraction>(mArgument, resolvedBody->value()), Node::make<AbstractionType>(mArgument->type(), resolvedBody->type()));
        }

        Node::Pointer replace(Node::Pointer a, Node::Pointer b) const override
        {
            // Treat this like a node but no delete it
            auto abs = Node::makeNoDeletablePtr(this);

            // Duplicated variable names inside abstraction
            if (a->nodeType() == NodeType::Variable && mArgument->name() == Node::cast<Variable>(a)->name())
            {
                // FIXME: Esto puede causar problemas si en el body ya hay un @code{_x}
                // Generate new name
                // TODO: Use the correct type
                auto _x = Node::make<Variable>(mArgument->name() + "\'", mArgument->type());
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
            os << "(\xce\xbb" << mArgument->toString() << " : "
               << mArgument->type()->toString() << ". "
               << mBody->toString() << ")";
            return os.str();
        }

    private:
        Variable::Pointer mArgument;
        Node::Pointer mBody;
    };
}

#endif
