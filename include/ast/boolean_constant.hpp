#ifndef LAMBDA_BOOLEAN_CONSTANT_HPP
#define LAMBDA_BOOLEAN_CONSTANT_HPP

#include <ast/node.hpp>

namespace ast
{
    class BooleanConstant : public Node
    {

    public:
        using Pointer = Node::PointerType<BooleanConstant>;

    public:
        explicit BooleanConstant(bool value);

        const bool &value() const { return mValue; }

        Node::Pointer evaluate(Context &context) const override;

        Type::Pointer typecheck(TypeContext &context) const override;

        Node::Pointer transform(NodeVisitor *visitor) override;

        Node::Pointer copy() const override;

        string toString() const override;

    private:
        bool mValue;
    };
}

#endif //LAMBDA_BOOLEAN_CONSTANT_HPP
