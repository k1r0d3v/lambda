#ifndef LAMBDA_SEQUENCE_HPP
#define LAMBDA_SEQUENCE_HPP

#include "common.hpp"
#include "node_type.hpp"
#include "node.hpp"

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
        static Node::Pointer join(const Node::Pointer &a, const Node::Pointer &b)
        {
            if (a == nullptr)
                return b;

            if (b == nullptr)
                return a;

            list<Node::Pointer> elements;

            if (a != nullptr)
            {
                if (a->nodeType() == NodeType::Sequence)
                {
                    auto aElements = Node::cast<Sequence>(a)->elements();
                    elements.insert(elements.end(), aElements.begin(), aElements.end());
                } else elements.push_back(a);
            }

            if (b != nullptr)
            {
                if (b->nodeType() == NodeType::Sequence)
                {
                    auto bElements = Node::cast<Sequence>(b)->elements();
                    elements.insert(elements.end(), bElements.begin(), bElements.end());
                } else elements.push_back(b);
            }

            return Node::make<Sequence>(elements);
        }

    public:
        explicit Sequence(const list<Node::Pointer> &elements)
                : Node(NodeType::Sequence), mElements(elements) { }

        const list<Node::Pointer> &elements() const
        {
            return mElements;
        }

        Node::Pointer evaluate(Context &context) const override
        {
            auto r = Node::cast<Node>(Node::make<Unit>());

            for (const auto& i : mElements)
                r = i->evaluate(context);

            // Return the last sequence element
            return r;
        }

        Node::Pointer resolve(Context &context) const override
        {
            Type::Pointer type = Type::make<ConstantType>(Unit::TYPE_NAME);
            list<Node::Pointer> elements;

            for (const auto& i : mElements)
            {
                auto iResolved = Node::cast<TypedValue>(i->resolve(context));
                type = iResolved->type();
                elements.push_back(iResolved->value());
            }

            return Node::make<TypedValue>(Node::make<Sequence>(elements), type);
        }

        Node::Pointer replace(Node::Pointer a, Node::Pointer b) const override
        {
            list<Node::Pointer> elements;

            for (const auto& i : mElements)
                elements.push_back(i->replace(a, b));

            return Node::make<Sequence>(elements);
        }

        Node::Pointer copy() const override
        {
            return Node::make<Sequence>(mElements);
        }

        string toString() const override
        {
            auto os = std::ostringstream();

            for (const auto& i : mElements)
                os << i->toString() << ";" << std::endl;
            return os.str();
        }

    private:
        list<Node::Pointer> mElements;
    };
}

#endif //LAMBDA_SEQUENCE_HPP
