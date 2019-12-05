#ifndef LAMBDA_LIST_HPP
#define LAMBDA_LIST_HPP

#include <ast/node.hpp>
#include <ast/node_kind.hpp>
#include "pattern.hpp"

namespace ast
{
    class List : public Pattern
    {
    public:
        using Pointer = Node::PointerType<List>;

    public:
        explicit List(vector<Node::Pointer> elements);

        explicit List(Type::Pointer elementType);

        explicit List(Node::Pointer head, List::Pointer tail);

        const Node::Pointer &head() const { return mHead; }

        const Pointer &tail() const { return mTail; }

    public: // Node
        Node::Pointer evaluate(Context &context) const override;

        Type::Pointer typecheck(TypeContext &context) override;

        Node::Pointer transform(NodeVisitor *visitor) override;

        Node::Pointer copy() const override;

        string toString() const override;

    public: // Pattern
        Pattern::MatchIdenfiers matchIdentifiers() const override;

        Pattern::MatchResult match(const Node::Pointer &value, Context &context) const override;

        Pattern::TypecheckMatchResult typecheckMatch(const Type::Pointer &type, TypeContext &context) const override;

    private:
        /**
         * Use it is used as an auxiliary function of toString
         * @return string
         */
        string toStringTail() const;

    private:
        List::Pointer mTail;
        Node::Pointer mHead;
        vector<Node::Pointer> mConcatList;
        Type::Pointer mElementType; // Only for empty
    };
}

#endif //LAMBDA_LIST_HPP
