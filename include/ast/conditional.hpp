#ifndef LAMBDA_CONDITIONAL_HPP
#define LAMBDA_CONDITIONAL_HPP

#include "common.hpp"
#include "exception.hpp"
#include "node.hpp"
#include "node_type.hpp"
#include "boolean.hpp"

namespace ast
{
    class Conditional : public Node
    {
    public:
        using Pointer = Node::PointerType<Conditional>;

    public:
        explicit Conditional(Node::Pointer condition, Node::Pointer thenBranch, Node::Pointer elseBranch)
                : Node(NodeType::Condition),
                  mCondition(std::move(condition)),
                  mThen(std::move(thenBranch)),
                  mElse(std::move(elseBranch)) { }

        const Node::Pointer &condition() const
        {
            return mCondition;
        }

        const Node::Pointer &thenBranch() const
        {
            return mThen;
        }

        const Node::Pointer &elseBranch() const
        {
            return mElse;
        }

        Node::Pointer evaluate(Context &context) const override
        {
            auto evalResult = mCondition->evaluate(context);
            if (evalResult->type() != NodeType::Boolean)
                throw TypeException("\'" + evalResult->toString() + "\' is not a boolean");

            if (Node::cast<Boolean>(evalResult)->value())
                return mThen->evaluate(context);
            else
                return mElse->evaluate(context);
        }

        Node::Pointer resolve(const Context &context) const override
        {
            return Node::make<Conditional>(mCondition->resolve(context), mThen->resolve(context), mElse->resolve(context));
        }

        Node::Pointer replace(Node::Pointer a, Node::Pointer b) const override
        {
            return Node::make<Conditional>(mCondition->replace(a, b), mThen->replace(a, b), mElse->replace(a, b));
        }

        Node::Pointer copy() const override
        {
            return Node::make<Conditional>(mCondition, mThen, mElse);
        }

        string toString() const override
        {
            auto os = std::ostringstream();
            os << "if " << mCondition->toString() << " then " << mThen->toString() << " else " << mElse->toString();
            return os.str();
        }

    private:
        Node::Pointer mCondition;
        Node::Pointer mThen;
        Node::Pointer mElse;
    };
}

#endif //LAMBDA_CONDITIONAL_HPP
