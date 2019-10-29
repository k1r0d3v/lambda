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
                throw UnexpectedException("Condition not resolved before evaluation, unexpected condition value");

            if (Node::cast<BooleanConstant>(evalResult)->value())
                return mThen->evaluate(context);
            else
                return mElse->evaluate(context);
        }

        Node::Pointer freeze(Context &context) const override
        {
            return Node::make<Conditional>(mCondition->freeze(context), mThen->freeze(context), mElse->freeze(context));
        }

        Type::Pointer typecheck(TypeContext &context) const override
        {
            auto conditionType = mCondition->typecheck(context);

            if (Type::distinct(conditionType, BoolType::BOOL))
                throw TypeException("Expected \'Bool\' not \'" + conditionType->toString() + "\'");

            auto thenType = mThen->typecheck(context);
            auto elseType = mElse->typecheck(context);

            if (Type::distinct(thenType, elseType))
                throw TypeException("Incompatible operand types \'" + thenType->toString() + "\' and \'" + elseType->toString() + "\'");

            return thenType;
        }

        Node::Pointer copy() const override
        {
            return Node::make<Conditional>(mCondition->copy(), mThen->copy(), mElse->copy());
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
