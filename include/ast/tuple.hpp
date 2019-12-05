#ifndef LAMBDA_TUPLE_HPP
#define LAMBDA_TUPLE_HPP

#include <ast/node.hpp>
#include <ast/pattern.hpp>
#include <ast/operable_node.hpp>
#include <ast/node_kind.hpp>

namespace ast
{
    class Tuple : public Pattern, public OperableNode
    {
    public:
        using Pointer = Node::PointerType<Tuple>;

    public:
        explicit Tuple(vector<Node::Pointer> elements);

        template<typename Iterator>
        Tuple(Iterator begin, Iterator end) : Pattern(NodeKind::Tuple) {
            mElements = {begin, end};
        }

        const vector<Node::Pointer> &elements() const { return mElements; }

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
        vector<Node::Pointer> mElements;
    };
}

#endif //LAMBDA_TUPLE_HPP
