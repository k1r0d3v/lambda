#ifndef LAMBDA_FIX_HPP
#define LAMBDA_FIX_HPP

#include <ast/node.hpp>

namespace ast
{
    class Fix : public Node
    {
    public:
        using Pointer = Node::PointerType<Fix>;

    public:
        explicit Fix(Node::Pointer term);

        Node::Pointer evaluate(Context &context) const override;

        Type::Pointer typecheck(TypeContext &context) const override;

        Node::Pointer transform(NodeVisitor *visitor) override;

        Node::Pointer copy() const override;

        string toString() const override;

    private:
        Node::Pointer mTerm;
    };
}

//lambda ieio : {iseven:Nat->Bool, isodd:Nat->Bool}. {iseven: lambda x:Nat. if iszero x then true else ieio.isodd (pred x), isodd: lambda x:Nat. if iszero x then false else ieio.iseven (pred x)}

#endif //LAMBDA_FIX_HPP
