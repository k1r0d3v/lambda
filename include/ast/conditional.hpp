#ifndef LAMBDA_CONDITIONAL_HPP
#define LAMBDA_CONDITIONAL_HPP

#include <ast/node.hpp>

namespace ast
{
    class Conditional : public Node
    {
    public:
        using Pointer = Node::PointerType<Conditional>;

    public:
        explicit Conditional(Node::Pointer condition, Node::Pointer thenBranch, Node::Pointer elseBranch);

        Node::Pointer evaluate(Context &context) const override;

        Type::Pointer typecheck(TypeContext &context) const override;

        Node::Pointer transform(NodeVisitor *visitor) override;

        Node::Pointer copy() const override;

        string toString() const override;

    private:
        Node::Pointer mCondition;
        Node::Pointer mThen;
        Node::Pointer mElse;
    };
}

#endif //LAMBDA_CONDITIONAL_HPP
