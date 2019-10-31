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
        {
            // Curry style application, reorder the tree
            if (mRight->nodeType() == NodeType::Application)
            {
                auto myRight = Node::cast<Application>(mRight);
                mLeft = Node::make<Application>(mLeft, myRight->left());
                mRight = myRight->right();
            }
        }

        const Node::Pointer &left() const
        {
            return mLeft;
        }

        const Node::Pointer &right() const
        {
            return mRight;
        }

        Node::Pointer evaluate(const Node::Pointer &self, Context &context) const override
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
                    leftTerm = leftTerm->evaluate(leftTerm, context);

                if (leftTerm->nodeType() != NodeType::Abstraction)
                    throw UnexpectedException("Application not resolved before evaluation, unexpected left term");


                auto abstraction = Node::cast<Abstraction>(leftTerm);

                // Push argument
                context.stackPush(rightTerm->evaluate(rightTerm, context));
                // Evaluate
                t = abstraction->body()->evaluate(abstraction->body(), context);
                // Pop argument
                context.stackPop();

                // Try to reuse the stack to avoid stack overflows
            } while (t->nodeType() == NodeType::Application);

            return t->evaluate(t, context);
        }

        Node::Pointer resolve(const Node::Pointer &self, Context &context) const override
        {
            auto resolvedLeft = mLeft->resolve(mLeft, context);
            auto resolvedRight = mRight->resolve(mRight, context);

            if (resolvedLeft != mLeft || resolvedRight != mRight)
                return Node::make<Application>(resolvedLeft, resolvedRight);
            else return self;
        }

        Type::Pointer typecheck(TypeContext &context) const override
        {
            auto leftType = Type::cast<ArrowType>(mLeft->typecheck(context));

            if (leftType == nullptr)
                throw TypeException("Expected an abstraction");

            Type::Pointer rightType = mRight->typecheck(context);

            if (Type::distinct(leftType->left(), rightType))
                throw TypeException("Incompatible types");

            return leftType->right();
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
