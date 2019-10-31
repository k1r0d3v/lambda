#ifndef LAMBDA_IDENTIFIER_HPP
#define LAMBDA_IDENTIFIER_HPP

#include "node.hpp"
#include "node_type.hpp"
#include "common.hpp"
#include "exception.hpp"
#include "ast/types/type.hpp"

namespace ast
{
    class Identifier : public Node
    {
    public:
        using Pointer = Node::PointerType<Identifier>;

    public:
        explicit Identifier(string name)
                : Node(NodeType::Variable), mName(std::move(name))
        { }

        const string &name() const
        {
            return mName;
        }

        Node::Pointer evaluate(const Node::Pointer &self, Context &context) const override
        {
            auto valueOfName = context.getValue(mName);
            return valueOfName->evaluate(valueOfName, context);
        }

        Node::Pointer resolve(const Node::Pointer &self, Context &context) const override
        {
            auto valueOfName = context.getValue(mName);

            if (valueOfName == nullptr)
                return self; // We must conserve the id
            else
                return valueOfName;
        }

        Type::Pointer typecheck(TypeContext &context) const override
        {
            auto typeOfName = context.getTypeOf(mName);
            if (typeOfName == nullptr)
                throw NameException("Name \'" + mName + "\' is not defined");

            return typeOfName;
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

#endif //LAMBDA_IDENTIFIER_HPP
