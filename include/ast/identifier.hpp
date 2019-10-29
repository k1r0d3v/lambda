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

        Node::Pointer evaluate(Context &context) const override
        {
            // Evaluate can be called when the identifier has not been replaced
            // If that is the case then is expected that the tree is a only identifier
            // or whats the same the identifier hast not haven't time to be frozen
            auto valueOfName = context.getValue("::" + mName);
            if (valueOfName == nullptr)
                throw NameException("Name \'" + mName + "\' is not defined");

            return valueOfName->evaluate(context);
        }

        // TODO: Move to abstraction
        Node::Pointer freeze(Context &context) const override
        {
            auto valueOfName = context.getValue(mName);
            if (valueOfName == nullptr)
                valueOfName = context.getValue("::" + mName);

            if (valueOfName == nullptr)
                throw NameException("Name \'" + mName + "\' is not defined");

            // Replace this node by the value
            return valueOfName;
        }

        Type::Pointer typecheck(TypeContext &context) const override
        {
            auto typeOfName = context.getTypeOf(mName);
            if (typeOfName == nullptr)
                typeOfName = context.getTypeOf("::" + mName);

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
