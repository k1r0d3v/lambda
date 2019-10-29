#ifndef LAMBDA_ABSTRACTION_HPP
#define LAMBDA_ABSTRACTION_HPP

#include "common.hpp"
#include "identifier.hpp"
#include "node_type.hpp"
#include "variable.hpp"
#include "ast/types/arrow_type.hpp"

namespace ast
{
    class Abstraction : public Node
    {
    public:
        using Pointer = Node::PointerType<Abstraction>;

    public:
        Abstraction(Variable::Pointer argument, Node::Pointer body)
                : Node(NodeType::Abstraction),
                  mArgument(std::move(argument)),
                  mBody(std::move(body))
        { }

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
            // Must be frozen when its evaluated. For example on:
            // let y = 0;
            // let x = lambda x : Nat. y;
            // x is the same that (lambda x : Nat. 0);
            return this->freeze(context);
        }

        Node::Pointer freeze(Context &context) const override
        {
            // Push argument
            auto lastArgumentValue = context.setValue(mArgument->name(), mArgument);

            // Freeze
            auto freezeValue = Node::make<Abstraction>(mArgument, mBody->freeze(context));

            // Pop argument
            context.setValue(mArgument->name(), lastArgumentValue);
            return freezeValue;
        }

        Type::Pointer typecheck(TypeContext &context) const override
        {
            // Push argument
            auto lastArgumentType = context.setTypeFor(mArgument->name(), mArgument->type());

            // Typecheck
            auto arrowType = Type::make<ArrowType>(mArgument->type(), mBody->typecheck(context));

            // Pop argument
            context.setTypeFor(mArgument->name(), lastArgumentType);
            return arrowType;
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
