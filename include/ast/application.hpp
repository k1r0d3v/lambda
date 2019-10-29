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
            // Creates a pointer to this with no deleter
            Node::Pointer t = Node::makeNoDeletablePtr<Node>(const_cast<Application *>(this));

            do
            {
                Application::Pointer application = Node::cast<Application>(t);

                // Call by value resolves the right most term first
                Node::Pointer leftTerm, rightTerm;
                leftTerm = application->left();
                rightTerm = application->right();

                // Evaluates left side if not an abstraction
                if (leftTerm->nodeType() != NodeType::Abstraction)
                    leftTerm = leftTerm->evaluate(context);

                if (leftTerm->nodeType() != NodeType::Abstraction)
                    throw UnexpectedException("Application not resolved before evaluation, unexpected left term");

                if (rightTerm->nodeType() == NodeType::Application)
                {
                    // We have a curry form, reorder the tree to apply the nodes in the correct order
                    auto r = Node::cast<Application>(rightTerm);
                    t = Node::make<Application>(Node::make<Application>(leftTerm, r->left()), r->right());
                }
                else
                {
                    // First freeze the abstraction with evaluate
                    auto abstraction = Node::cast<Abstraction>(leftTerm->evaluate(context));

                    // Push argument
                    auto lastArgumentValue = context.setValue(abstraction->argument()->name(),
                                                              rightTerm->evaluate(context));

                    // Evaluate body
                    t = abstraction->body()->evaluate(context);

                    // Pop argument
                    context.setValue(abstraction->argument()->name(), lastArgumentValue);
                }

                // Try to reuse the stack to avoid stack overflows
            } while (t->nodeType() == NodeType::Application);

            return t->evaluate(context);
        }

        Node::Pointer freeze(Context &context) const override
        {
            return Node::make<Application>(mLeft->freeze(context), mRight->freeze(context));;
        }

        Type::Pointer typecheck(TypeContext &context) const override
        {
            auto leftType = Type::cast<ArrowType>(mLeft->typecheck(context));

            if (leftType == nullptr)
                throw TypeException("Expected an abstraction");

            if (mRight->nodeType() == NodeType::Application)
            {
                // We have a curry form, reorder the tree to apply the nodes in the correct order
                auto r = Node::cast<Application>(mRight);
                auto t = Node::make<Application>(Node::make<Application>(mLeft, r->left()), r->right());
                return t->typecheck(context);
            }
            else
            {
                Type::Pointer rightType = mRight->typecheck(context);

                if (Type::distinct(leftType->left(), rightType))
                    throw TypeException("Incompatible types");

                return leftType->right();
            }
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
