#ifndef LAMBDA_ABSTRACTION_HPP
#define LAMBDA_ABSTRACTION_HPP

#include <ast/node.hpp>
#include <ast/variable.hpp>

namespace ast
{
    class Abstraction : public Node
    {
    public:
        using Pointer = Node::PointerType<Abstraction>;

    public:
        Abstraction(Variable::Pointer variable, Node::Pointer body);

        const Variable::Pointer &variable() const { return mVariable; }

        const Node::Pointer &body() const { return mBody; }

        Node::Pointer evaluate(Context &context) const override;

        Type::Pointer typecheck(TypeContext &context) override;

        Node::Pointer transform(NodeVisitor *visitor) override;

        Node::Pointer copy() const override;

        string toString() const override;

    private:
        Variable::Pointer mVariable;
        Node::Pointer mBody;
        bool mResolved = false;
    };
}

#endif
