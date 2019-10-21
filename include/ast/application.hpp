#ifndef LAMBDA_APPLICATION_HPP
#define LAMBDA_APPLICATION_HPP

#include <utility>

#include "abstraction.hpp"
#include "node_type.hpp"
#include "common.hpp"
#include "util.hpp"

namespace ast
{
    class Application : public Node
    {
    public:
        using Pointer = Node::PointerType<Application>;

    public:
        Application(Node::Pointer left, Node::Pointer right)
                : Node(NodeType::Application), mLeft(std::move(left)), mRight(std::move(right)) { }

        const Node::Pointer &left() const
        {
            return mLeft;
        }

        const Node::Pointer &right() const
        {
            return mRight;
        }

        Node::Pointer evaluate(const Context &context) const override
        {
            // Call by value resolves the right most term first
            auto v1 = mLeft;
            if (mLeft->type() != NodeType::Abstraction)
                v1 = mLeft->evaluate(context);

            if (v1->type() != NodeType::Abstraction)
                throw std::runtime_error("Expected a value");

            auto t2 = mRight->evaluate(context);

            if (v1->type() != NodeType::Abstraction)
                throw std::runtime_error("Expected an abstraction");

            auto abs = Node::cast<Abstraction>(v1);
            return util::replaceIdentifier(abs->argument(), abs->body(), t2)->evaluate(context);
        }

        Node::Pointer copy() const override
        {
            return Node::make<Application>(mLeft->copy(), mRight->copy());
        }

        string toString() const override
        {
            auto os = std::ostringstream();
            os << mLeft->toString() << " " << mRight->toString();
            return os.str();
        }

    private:
        Node::Pointer mLeft;
        Node::Pointer mRight;
    };
}

#endif
