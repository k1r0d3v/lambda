#ifndef LAMBDA_UNIT_HPP
#define LAMBDA_UNIT_HPP

#include <ast/node.hpp>

namespace ast
{
    class Unit : public Node
    {
    public:
        using Pointer = Node::PointerType<Unit>;

    public:
        explicit Unit();

        Node::Pointer evaluate(Context &context) const override;

        Type::Pointer typecheck(TypeContext &context) override;

        Node::Pointer transform(NodeVisitor *visitor) override;

        Node::Pointer copy() const override;

        string toString() const override;
    };
}


#endif //LAMBDA_UNIT_HPP
