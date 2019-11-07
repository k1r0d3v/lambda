#ifndef LAMBDA_STRING_CONSTANT_HPP
#define LAMBDA_STRING_CONSTANT_HPP

#include <ast/node.hpp>

namespace ast
{
    class StringConstant : public Node
    {
    public:
        using Pointer = Node::PointerType<StringConstant>;

    public:
        explicit StringConstant(string value);

        const string &value() const { return mValue; }

        Node::Pointer evaluate(Context &context) const override;

        Type::Pointer typecheck(TypeContext &context) const override;

        Node::Pointer transform(NodeVisitor *visitor) override;

        Node::Pointer copy() const override;

        string toString() const override;

    private:
        string mValue;
    };
}

#endif //LAMBDA_STRING_CONSTANT_HPP
