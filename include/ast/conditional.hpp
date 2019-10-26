#ifndef LAMBDA_CONDITIONAL_HPP
#define LAMBDA_CONDITIONAL_HPP

#include "common.hpp"
#include "exception.hpp"
#include "node.hpp"
#include "node_type.hpp"
#include "boolean_constant.hpp"


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
                  mElse(std::move(elseBranch))
        { }

        Node::Pointer evaluate(Context &context) const override
        {
            auto evalResult = mCondition->evaluate(context);
            if (evalResult->nodeType() != NodeType::BooleanConstant)
                throw TypeException("\'" + evalResult->toString() + "\' is not a boolean");

            if (Node::cast<BooleanConstant>(evalResult)->value())
                return mThen->evaluate(context);
            else
                return mElse->evaluate(context);
        }

        Node::Pointer resolve(Context &context) const override
        {
            auto resolvedCondition = Node::cast<TypedValue>(mCondition->resolve(context));
            auto resolvedThen = Node::cast<TypedValue>(mThen->resolve(context));
            auto resolvedElse = Node::cast<TypedValue>(mElse->resolve(context));

            if (resolvedCondition->type()->distinct(Type::make<ConstantType>(BooleanConstant::TYPE_NAME)))
                throw TypeException("Expected a boolean value");

            if (resolvedThen->type()->distinct(resolvedElse->type()))
                throw TypeException("Expected the same type in then and else");

            return Node::make<TypedValue>(Node::make<Conditional>(resolvedCondition->value(), resolvedThen->value(), resolvedElse->value()), resolvedThen->type());
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
