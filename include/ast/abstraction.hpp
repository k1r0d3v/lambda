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

        Node::Pointer evaluate(const Node::Pointer &self, Context &context) const override
        {
            if (!mResolved)
            {
                auto resolved = Node::cast<Abstraction>(this->resolve(self, context));
                auto resolvedAbs = Node::make<Abstraction>(resolved->mArgument, resolved->mBody);
                resolvedAbs->mResolved = true;
                return resolvedAbs;
            }

            return self;
        }

        Node::Pointer resolve(const Node::Pointer &self, Context &context) const override
        {
            // Push
            auto indexedVariable = Node::make<IndexedVariable>(context.stackSize(), mArgument);
            auto lastValue = context.setValue(mArgument->name(), indexedVariable);
            context.stackPush(nullptr); // Increment stack size

            // Resolve
            auto resolvedBody = mBody->resolve(mBody, context);

            Node::Pointer result;
            if (resolvedBody != mBody)
                result = Node::make<Abstraction>(mArgument, resolvedBody);
            else
                result = self;

            // Pop
            context.setValue(mArgument->name(), lastValue);
            context.stackPop();

            return result;
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
        bool mResolved = false;
    };
}

#endif
