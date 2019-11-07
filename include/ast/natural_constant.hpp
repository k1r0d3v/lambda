#ifndef LAMBDA_NATURAL_CONSTANT_HPP
#define LAMBDA_NATURAL_CONSTANT_HPP

#include <ast/node.hpp>

namespace ast
{
    class NaturalConstant : public Node
    {
    public:
        using Pointer = Node::PointerType<NaturalConstant>;
        using NaturalValueType = unsigned long;

    public:
        explicit NaturalConstant(NaturalValueType value);

        const NaturalValueType &value() const { return mValue; }

        Node::Pointer evaluate(Context &context) const override;

        Type::Pointer typecheck(TypeContext &context) const override;

        Node::Pointer transform(NodeVisitor *visitor) override;

        Node::Pointer copy() const override;

        string toString() const override;
    private:
        NaturalValueType mValue;
    };
}

#endif //LAMBDA_NATURAL_CONSTANT_HPP
