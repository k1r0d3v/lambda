#ifndef LAMBDA_DECLARATION_HPP
#define LAMBDA_DECLARATION_HPP

#include <ast/node.hpp>
#include <ast/identifier.hpp>

namespace ast
{
    class Declaration : public Node
    {
    public:
        using Pointer = Node::PointerType<Declaration>;

    public:
        explicit Declaration(Pattern::Pointer pattern, Node::Pointer value);

        const Pattern::Pointer &pattern() const { return mPattern; }

        const Node::Pointer &value() const { return mValue; }

        Node::Pointer evaluate(Context &context) const override;

        Type::Pointer typecheck(TypeContext &context) const override;

        Node::Pointer transform(NodeVisitor *visitor) override;

        Node::Pointer copy() const override;

        string toString() const override;

    private:
        Pattern::Pointer mPattern;
        Node::Pointer mValue;
    };
}


#endif //LAMBDA_DECLARATION_HPP
