#ifndef LAMBDA_ALIAS_HPP
#define LAMBDA_ALIAS_HPP

#include <ast/node.hpp>
#include <ast/types/constant_type.hpp>

namespace ast
{
    class Alias : public Node
    {
    public:
        using Pointer = Node::PointerType<Alias>;

    public:
        explicit Alias(string alias, Type::Pointer type);

        const string &alias() const { return mAlias; }

        const Type::Pointer &type() const { return mType; }

        Node::Pointer evaluate(Context &context) const override;

        Type::Pointer typecheck(TypeContext &context) override;

        Node::Pointer transform(NodeVisitor *visitor) override;

        Node::Pointer copy() const override;

        string toString() const override;

    private:
        string mAlias;
        Type::Pointer mType;
    };
}

#endif //LAMBDA_ALIAS_HPP
