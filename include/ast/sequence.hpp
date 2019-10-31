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

        Node::Pointer evaluate(const Node::Pointer &self, Context &context) const override
        {
            auto r = Node::cast<Node>(Node::make<Unit>());

            for (const auto& i : mElements)
                r = i->evaluate(i, context);

            // Return the last sequence element
            return r;
        }

        Node::Pointer resolve(const Node::Pointer &self, Context &context) const override
        {
            list<Node::Pointer> elements;
            for (const auto &i : mElements)
                elements.push_back(i->resolve(i, context));

            return Node::make<Sequence>(elements);
        }

        Type::Pointer typecheck(TypeContext &context) const override
        {
            Type::Pointer type = UnitType::INSTANCE;

            for (const auto& i : mElements)
                type = i->typecheck(context);

            return type;
        }

        Node::Pointer copy() const override
        {
            return Node::make<Sequence>(mElements);
        }

        string toString() const override
        {
            auto os = std::ostringstream();

            for (const auto& i : mElements)
                os << i->toString() << "; ";

            auto str = os.str();

            // Remove trailing ;
            if (!str.empty())
            {
                str.pop_back();
                str.pop_back();
            }

            return str;
        }

    private:
        list<Node::Pointer> mElements;
    };
}

#endif //LAMBDA_SEQUENCE_HPP
