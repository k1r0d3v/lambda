#ifndef LAMBDA_APPLICATION_HPP
#define LAMBDA_APPLICATION_HPP

#include "abstraction.hpp"
#include "node_type.hpp"
#include "common.hpp"
#include "util.hpp"

namespace ast
{
    class Application : public Node
    {
    public:
        Application(Node::Reference left, Node::Reference right)
                : Node(NodeType::Application), mLeft(left), mRight(right) {}

        const Node::Reference &left() const
        {
            return mLeft;
        }

        const Node::Reference &right() const
        {
            return mRight;
        }

        Node::Reference evaluate(const Context &context) const override
        {
            // Call by value resolves the right most term first
            auto v1 = mLeft;
            if (!util::isValueType(*mLeft.get()))
                v1 = mLeft->evaluate(context);

            if (!util::isValueType(*v1.get()))
                throw std::runtime_error("Expected a value");

            auto t2 = mRight->evaluate(context);

            if (v1->type() != NodeType::Abstraction)
                throw std::runtime_error("Expected an abstraction");

            auto abs = Node::castRef<Abstraction>(v1);
            return util::replaceIdentifier(abs->argument(), abs->body(), t2)->evaluate(context);
        }

        Node::Reference copy() const override
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
        Node::Reference mLeft;
        Node::Reference mRight;
    };
}

#endif
