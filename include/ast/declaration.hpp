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
        explicit Declaration(Identifier::Pointer id, Node::Pointer value);

        const Identifier::Pointer &id() const { return mId; }

        const Node::Pointer &value() const { return mValue; }

        Node::Pointer evaluate(Context &context) const override;

        Type::Pointer typecheck(TypeContext &context) const override;

        Node::Pointer transform(NodeVisitor *visitor) override;

        Node::Pointer copy() const override;

        string toString() const override;

    private:
        // TODO: Change id by Pattern::Pointer
        Identifier::Pointer mId;
        Node::Pointer mValue;
    };
}


#endif //LAMBDA_DECLARATION_HPP
