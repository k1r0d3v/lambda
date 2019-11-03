#ifndef LAMBDA_OPERATOR_DOT_HPP
#define LAMBDA_OPERATOR_DOT_HPP

#include <ast/types/register_type.hpp>
#include "common.hpp"
#include "node_type.hpp"
#include "node.hpp"
#include "register.hpp"

namespace ast
{
    class OperatorDot : public Node
    {
    public:
        using Pointer = Node::PointerType<OperatorDot>;

    public:
        explicit OperatorDot(Node::Pointer source, string field)
            : Node(NodeType::OperatorDot), mSource(std::move(source)), mField(std::move(field))
        { }

        explicit OperatorDot(Node::Pointer source, int field)
            : Node(NodeType::OperatorDot), mSource(std::move(source)), mField(to_string(field))
        { }

        Node::Pointer evaluate(const Node::Pointer &self, Context &context) const override
        {
            auto evaluatedSource = mSource->evaluate(self, context);
            return evaluatedSource->operator_dot(mSource, mField, context);
        }

        Type::Pointer typecheck(TypeContext &context) const override
        {
            return mSource->typecheck(context)->typeOfField(mField);
        }

        Node::Pointer copy() const override
        {
            return Node::make<OperatorDot>(mSource->copy(), mField);
        }

        string toString() const override
        {
            return mSource->toString() + "." + mField;
        }

    private:
        Node::Pointer mSource;
        string mField;
    };
}

#endif //LAMBDA_OPERATOR_DOT_HPP
