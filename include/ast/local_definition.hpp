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

        /*
        // Preserving let node

        Node::Pointer evaluate(const Node::Pointer &self, Context &context) const override
        {
            auto lastValue = context.setValue(mId->name(), mValue->evaluate(mValue, context));
            auto evaluatedBody = mBody->evaluate(mBody, context);
            context.setValue(mId->name(), lastValue);
            return evaluatedBody;
        }

        Node::Pointer resolve(const Node::Pointer &self, Context &context) const override
        {
            auto resolvedValue = mValue->resolve(mValue, context);

            // Conserve id's equals to mId, will be resolved in evaluation time
            auto lastValue = context.setValue(mId->name(), nullptr);
            auto resolvedBody = mValue->resolve(mValue, context);
            // Restore id value if any
            context.setValue(mId->name(), lastValue);

            if (resolvedValue != mValue || resolvedBody != mBody)
                return Node::make<LocalDefinition>(mId, resolvedValue, resolvedBody);

            return self;
        }
        */

        Node::Pointer evaluate(const Node::Pointer &self, Context &context) const override
        {
            auto resolved = this->resolve(self, context);
            return resolved->evaluate(resolved, context);
        }

        Node::Pointer resolve(const Node::Pointer &self, Context &context) const override
        {
            auto resolvedValue = mValue->resolve(mValue, context);

            // Conserve id's equals to mId, will be resolved in evaluation time
            auto lastValue = context.setValue(mId->name(), resolvedValue);
            auto resolvedBody = mValue->resolve(mValue, context);
            // Restore id value if any
            context.setValue(mId->name(), lastValue);

            return resolvedBody;
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
