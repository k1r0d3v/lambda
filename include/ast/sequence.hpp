#ifndef LAMBDA_SEQUENCE_HPP
#define LAMBDA_SEQUENCE_HPP

#include <ast/node.hpp>

namespace ast
{
    class Sequence : public Node
    {
    public:
        using Pointer = Node::PointerType<Sequence>;

        /**
         * This function joins two nodes in a @code{Sequence}
         *
         * Note: This function does not make a Sequence if any of @param{a} or @param{b} are null,
         * instead it returns the contrary node
         *
         * @param a First element of sequence
         * @param b Second element of sequence
         * @return A sequence formed by @param{a} and @param{b} or if any of params are another @code{Sequence}
         * join his elements instead
         */
        static Node::Pointer join(const Node::Pointer &a, const Node::Pointer &b);

    public:
        explicit Sequence(const list<Node::Pointer> &elements);

        const list<Node::Pointer> &elements() const { return mElements; }

        Node::Pointer evaluate(Context &context) const override;

        Type::Pointer typecheck(TypeContext &context) override;

        Node::Pointer transform(NodeVisitor *visitor) override;

        Node::Pointer copy() const override;

        string toString() const override;

    private:
        list<Node::Pointer> mElements;
    };
}

#endif //LAMBDA_SEQUENCE_HPP
