#ifndef LAMBDA_LOCAL_DEFINITION_HPP
#define LAMBDA_LOCAL_DEFINITION_HPP

#include <ast/node.hpp>
#include <ast/pattern.hpp>

namespace ast
{
    class LocalDefinition : public Node
    {
    public:
        using Pointer = Node::PointerType<LocalDefinition>;

    public:
        explicit LocalDefinition(Pattern::Pointer pattern, Node::Pointer value, Node::Pointer body);

        const Pattern::Pointer &pattern() const { return mPattern; }

        const Node::Pointer &value() const { return mValue; }

        const Node::Pointer &body() const { return mBody; }

        Node::Pointer evaluate(Context &context) const override;

        Type::Pointer typecheck(TypeContext &context) override;

        Node::Pointer transform(NodeVisitor *visitor) override;

        Node::Pointer copy() const override;

        string toString() const override;

    private:
        Pattern::Pointer mPattern;
        Node::Pointer mValue;
        Node::Pointer mBody;
    };
}

#endif //LAMBDA_LOCAL_DEFINITION_HPP
