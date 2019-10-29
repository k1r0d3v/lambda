#ifndef LAMBDA_LOCAL_DEFINITION_HPP
#define LAMBDA_LOCAL_DEFINITION_HPP

#include "common.hpp"
#include "node.hpp"
#include "node_type.hpp"

namespace ast
{
    class LocalDefinition : public Node
    {
    public:
        using Pointer = Node::PointerType<LocalDefinition>;

    public:
        explicit LocalDefinition(Identifier::Pointer id, Node::Pointer value, Node::Pointer body)
                : Node(NodeType::LocalDefinition),
                  mId(std::move(id)),
                  mValue(std::move(value)),
                  mBody(std::move(body)) { }

        const Identifier::Pointer &id() const
        {
            return mId;
        }

        const Node::Pointer &value() const
        {
            return mValue;
        }

        const Node::Pointer &body() const
        {
            return mBody;
        }

        Node::Pointer evaluate(Context &context) const override
        {
            auto
            return mBody->evaluate(context);
        }

        Node::Pointer freeze(Context &context) const override
        {
            return Node::make<LocalDefinition>(mId, mValue->freeze(context), mBody->freeze(context));
        }

        Type::Pointer typecheck(TypeContext &context) const override
        {
            // Push argument
            auto valueType = mValue->typecheck(context);
            auto lastValueType = context.setTypeFor(mId->name(), valueType);

            // Typecheck
            auto bodyType = mBody->typecheck(context);

            // Pop argument
            context.setTypeFor(mId->name(), lastValueType);
            return bodyType;
        }

        Node::Pointer copy() const override
        {
            return Node::make<LocalDefinition>(mId, mValue->copy(), mBody->copy());
        }

        string toString() const override
        {
            auto os = std::ostringstream();
            os << "let " << mId->toString() << " = " << mValue->toString() << " in " << mBody->toString();
            return os.str();
        }

    private:
        Identifier::Pointer mId;
        Node::Pointer mValue;
        Node::Pointer mBody;
    };
}

#endif //LAMBDA_LOCAL_DEFINITION_HPP
