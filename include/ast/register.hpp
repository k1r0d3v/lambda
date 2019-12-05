#ifndef LAMBDA_REGISTER_HPP
#define LAMBDA_REGISTER_HPP

#include <ast/node.hpp>
#include <ast/pattern.hpp>
#include <ast/operable_node.hpp>

namespace ast
{
    class Register : public Pattern, public OperableNode
    {
    public:
        using Pointer = Node::PointerType<Register>;

    public:
        explicit Register(std::map<string, Node::Pointer> elements);

        explicit Register(const std::map<string, Pattern::Pointer>& elements);

        const std::map<string, Node::Pointer> &elements() const { return mElements; }

    public: // Pattern
        Pattern::MatchIdenfiers matchIdentifiers() const override;

        Pattern::MatchResult match(const Node::Pointer &value, Context &context) const override;

        Pattern::TypecheckMatchResult typecheckMatch(const Type::Pointer &type, TypeContext &context) const override;

    public: // Node
        Node::Pointer evaluate(Context &context) const override;

        Type::Pointer typecheck(TypeContext &context) override;

        Node::Pointer transform(NodeVisitor *visitor) override;

        Node::Pointer copy() const override;

        string toString() const override;

    public: // Operable
        Node::Pointer operatorDot(const Node::Pointer &b, Context &context) const override;

    private:
        std::map<string, Node::Pointer> mElements;
    };
}

#endif //LAMBDA_REGISTER_HPP
