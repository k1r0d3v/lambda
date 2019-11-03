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
        explicit LocalDefinition(Pattern::Pointer pattern, Node::Pointer value, Node::Pointer body)
                : Node(NodeType::LocalDefinition),
                  mPattern(std::move(pattern)),
                  mValue(std::move(value)),
                  mBody(std::move(body)) { }

        const Pattern::Pointer &pattern() const
        {
            return mPattern;
        }

        const Node::Pointer &value() const
        {
            return mValue;
        }

        const Node::Pointer &body() const
        {
            return mBody;
        }

        Node::Pointer evaluate(const Node::Pointer &self, Context &context) const override
        {
            auto evaluatedValue = mValue->evaluate(mValue, context);
            auto matchResult = mPattern->match(evaluatedValue, context);

            // Set values and save previous
            for (auto &i : matchResult)
                i.second = context.setValue(i.first, i.second);

            auto evaluatedBody = mBody->evaluate(mBody, context);

            // Restore values
            for (const auto &i : matchResult)
                context.setValue(i.first, i.second);

            return evaluatedBody;
        }

        Node::Pointer resolve(const Node::Pointer &self, Context &context) const override
        {
            auto resolvedValue = mValue->resolve(mValue, context);

            // Preserve id's equals to pattern id, these will be resolved in evaluation time
            Pattern::MatchResult tmp = {};
            for (const auto &i : mPattern->matchIdentifiers())
                tmp.push_back({i, context.setValue(i, nullptr)});

            // Resolve body
            auto resolvedBody = mBody->resolve(mBody, context);

            // Restore id value if any
            for (const auto &i : tmp)
                context.setValue(i.first, i.second);

            if (resolvedValue != mValue || resolvedBody != mBody)
                return Node::make<LocalDefinition>(mPattern, resolvedValue, resolvedBody);

            return self;
        }

        Type::Pointer typecheck(TypeContext &context) const override
        {
            // Push argument
            auto valueType = mValue->typecheck(context);
            auto matchResult = mPattern->typecheckMatch(valueType, context);

            // Set new types and save the previous
            for (auto &i : matchResult)
                i.second = context.setTypeFor(i.first, i.second);

            // Typecheck
            // TODO: !!!!!!!!!!!!!!!!!! catch the exceptions to restore the context if this fails
            // TODO: Check the others typecheck for this
            auto bodyType = mBody->typecheck(context);

            // Restore types
            for (const auto &i : matchResult)
                context.setTypeFor(i.first, i.second);

            return bodyType;
        }

        Node::Pointer copy() const override
        {
            return Node::make<LocalDefinition>(mPattern, mValue->copy(), mBody->copy());
        }

        string toString() const override
        {
            auto os = std::ostringstream();
            os << "let " << mPattern->toString() << " = " << mValue->toString() << " in " << mBody->toString();
            return os.str();
        }

    private:
        Pattern::Pointer mPattern;
        Node::Pointer mValue;
        Node::Pointer mBody;
    };
}

#endif //LAMBDA_LOCAL_DEFINITION_HPP
