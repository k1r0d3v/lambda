#ifndef LAMBDA_VARIABLE_HPP
#define LAMBDA_VARIABLE_HPP

#include "node.hpp"
#include "node_type.hpp"
#include "common.hpp"
#include "exception.hpp"
#include "ast/types/type.hpp"

namespace ast
{
    class Variable : public Node
    {
    public:
        using Pointer = Node::PointerType<Variable>;

    public:
        explicit Variable(string name, Type::Pointer type)
                : Node(NodeType::Variable), mName(std::move(name)), mType(std::move(type))
        {
            assert(mType != nullptr);
        }

        const string &name() const
        {
            return mName;
        }

        const Type::Pointer &type() const
        {
            return mType;
        }

        // TODO: Correct checks
        Node::Pointer evaluate(Context &context) const override
        {
            auto valueOfName = context.getValue(mName);

            if (valueOfName == nullptr)
                throw UnexpectedException("No argument has been pushed");

            return valueOfName->evaluate(context);
        }

        Node::Pointer freeze(Context &context) const override
        {
            return Node::make<Variable>(mName, mType);
        }

        Type::Pointer typecheck(TypeContext &context) const override
        {
            return mType;
        }

        Node::Pointer copy() const override
        {
            return Node::make<Variable>(mName, mType);
        }

        string toString() const override
        {
            return mName;
        }

    private:
        string mName;
        Type::Pointer mType;
    };
}

#endif //LAMBDA_VARIABLE_HPP
