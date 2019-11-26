#ifndef LAMBDA_ASCRIPTION_HPP
#define LAMBDA_ASCRIPTION_HPP

#include <ast/node.hpp>

namespace ast
{
    class Ascription : public Node
    {
    public:
        using Pointer = Node::PointerType<Ascription>;

    public:
        explicit Ascription(Node::Pointer node, Type::Pointer type);

        Node::Pointer evaluate(Context &context) const override;

        Type::Pointer typecheck(TypeContext &context) override;

        Node::Pointer transform(NodeVisitor *visitor) override;

        Node::Pointer copy() const override;

        string toString() const override;

    private:
        Node::Pointer mNode;
        Type::Pointer mType;
    };
}

#endif //LAMBDA_ASCRIPTION_HPP
