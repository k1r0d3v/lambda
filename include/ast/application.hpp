#ifndef LAMBDA_APPLICATION_HPP
#define LAMBDA_APPLICATION_HPP

#include "abstraction.hpp"
#include "node_type.hpp"
#include "common.hpp"
#include "exception.hpp"

namespace ast
{
    class Application : public Node
    {
    public:
        using Pointer = Node::PointerType<Application>;

    public:
        Application(Node::Pointer left, Node::Pointer right)
                : Node(NodeType::Application),
                  mLeft(std::move(left)),
                  mRight(std::move(right))
        { }

        const Node::Pointer &left() const
        {
            return mLeft;
        }

        const Node::Pointer &right() const
        {
            return mRight;
        }

        Node::Pointer evaluate(Context &context) const override
        {
            assert(mLeft != nullptr); // Empty left node not allowed
            assert(mRight != nullptr); // Empty right node not allowed

            // Creates a pointer to this with no deleter
            Node::Pointer t = Node::makeNoDeletablePtr<Node>(const_cast<Application *>(this));

            do
            {
                Application::Pointer application = Node::cast<Application>(t);

                // Call by value resolves the right most term first
                Node::Pointer v1, t2;
                v1 = application->left();
                t2 = application->right();

                // Evaluates left side if not an abstraction
                if (v1->nodeType() != NodeType::Abstraction)
                    v1 = v1->evaluate(context);

                if (v1->nodeType() != NodeType::Abstraction)
                    throw TypeException("\'" + v1->toString() + "\' is not an abstraction");

                if (t2->nodeType() == NodeType::Application)
                {
                    // Arguments are expected to be processed in order
                    // We have a curry form, reorder the nodes to apply in the correct order
                    auto rightApp = Node::cast<Application>(t2);
                    t = Node::make<Application>(Node::make<Application>(v1, rightApp->left()), rightApp->right());
                }
                else
                {
                    // Evaluates t2
                    auto abstraction = Node::cast<Abstraction>(v1);
                    auto v2 = t2->evaluate(context);
                    t = abstraction->body()->replace(abstraction->argument(), v2);
                }

                // Try to reuse the stack to avoid stack overflows
            } while (t->nodeType() == NodeType::Application);

            return t->evaluate(context);
        }

        Node::Pointer resolve(Context &context) const override
        {
            auto resolvedLeft = Node::cast<TypedValue>(mLeft->resolve(context));
            auto resolvedRight = Node::cast<TypedValue>(mRight->resolve(context));
            auto leftType = Type::cast<AbstractionType>(resolvedLeft->type());

            if (leftType == nullptr)
                throw TypeException("Expected an abstraction");

            if (leftType->left()->distinct(resolvedRight->type()))
                throw TypeException("Incomplatible types");

            return Node::make<TypedValue>(Node::make<Application>(resolvedLeft->value(), resolvedRight->value()), Type::make<AbstractionType>(leftType, resolvedRight->type()));
        }

        Node::Pointer replace(Node::Pointer a, Node::Pointer b) const override
        {
            return Node::make<Application>(mLeft->replace(a, b), mRight->replace(a, b));
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
