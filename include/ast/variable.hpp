#ifndef LAMBDA_VARIABLE_HPP
#define LAMBDA_VARIABLE_HPP

#include <ast/common.hpp>
#include <ast/node.hpp>
#include <ast/node_kind.hpp>

namespace ast
{
    class Variable : public Node
    {
    public:
        using Pointer = Node::PointerType<Variable>;

    public:
        explicit Variable(string name, Type::Pointer type);

        const string &name() const { return mName; }

        const Type::Pointer &type() const { return mType; }

        Node::Pointer evaluate(Context &context) const override;

        Type::Pointer typecheck(TypeContext &context) const override;

        Node::Pointer transform(NodeVisitor *visitor) override;

        Node::Pointer copy() const override;

        string toString() const override;

    private:
        string mName;
        Type::Pointer mType;
    };
}

#endif //LAMBDA_VARIABLE_HPP
