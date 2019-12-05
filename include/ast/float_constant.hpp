#ifndef LAMBDA_FLOAT_CONSTANT_HPP
#define LAMBDA_FLOAT_CONSTANT_HPP

#include <ast/node.hpp>

namespace ast
{
    class FloatConstant : public Node
    {
    public:
        using Pointer = Node::PointerType<FloatConstant>;
        using FloatValueType = double;

    public:
        explicit FloatConstant(FloatValueType value);

        const FloatValueType &value() const { return mValue; }

        Node::Pointer evaluate(Context &context) const override;

        Type::Pointer typecheck(TypeContext &context) override;

        Node::Pointer transform(NodeVisitor *visitor) override;

        Node::Pointer copy() const override;

        string toString() const override;
    private:
        FloatValueType mValue;
    };
}

#endif //LAMBDA_FLOAT_CONSTANT_HPP
