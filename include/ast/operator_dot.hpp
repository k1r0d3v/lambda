#ifndef LAMBDA_OPERATOR_DOT_HPP
#define LAMBDA_OPERATOR_DOT_HPP

#include <ast/node.hpp>
#include <ast/identifier.hpp>
#include <ast/natural_constant.hpp>

namespace ast
{
    class OperatorDot : public Node
    {
    public:
        using Pointer = Node::PointerType<OperatorDot>;

    public:
        explicit OperatorDot(Node::Pointer source, Node::Pointer field);

        Node::Pointer evaluate(Context &context) const override;

        Type::Pointer typecheck(TypeContext &context) const override;

        Node::Pointer transform(NodeVisitor *visitor) override;

        Node::Pointer copy() const override;

        string toString() const override;

    private:
        Node::Pointer mSource;
        Node::Pointer mField;
    };
}

#endif //LAMBDA_OPERATOR_DOT_HPP
