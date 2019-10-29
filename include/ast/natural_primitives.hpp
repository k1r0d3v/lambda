#ifndef LAMBDA_NATURAL_PRIMITIVES_HPP
#define LAMBDA_NATURAL_PRIMITIVES_HPP

#include <ast/types/bool_type.hpp>
#include "common.hpp"
#include "boolean_constant.hpp"
#include "natural_constant.hpp"
#include "node_type.hpp"
#include "exception.hpp"
#include "ast/types/arrow_type.hpp"

namespace ast
{
    //
    // TODO: Simplify this?
    //

    class IsZero : public Node
    {
    public:
        using Pointer = Node::PointerType<IsZero>;

    public:
        explicit IsZero(Node::Pointer argument)
                : Node(NodeType::Primitive), mArgument(std::move(argument))
        { }

        Node::Pointer evaluate(Context &context) const override
        {
            auto t = mArgument;
            if (t->nodeType() != NodeType::NaturalConstant)
                t = t->evaluate(context);

            if (t->nodeType() != NodeType::NaturalConstant)
                throw UnexpectedException("IsZero not resolved before evaluation, unexpected right term");

            return Node::make<BooleanConstant>(Node::cast<NaturalConstant>(t)->value() == 0);
        }

        Node::Pointer freeze(Context &context) const override
        {
            return Node::make<IsZero>(mArgument->freeze(context));
        }

        Type::Pointer typecheck(TypeContext &context) const override
        {
            auto argumentType = mArgument->typecheck(context);
            if (Type::distinct(argumentType, NatType::NAT))
                throw TypeException("\'" + argumentType->toString() + "\' is not a \'Nat\'");

            return BoolType::BOOL;
        }

        Node::Pointer copy() const override
        {
            return Node::make<IsZero>(mArgument);
        }

        string toString() const override
        {
            auto os = std::ostringstream();
            os << "(iszero " << mArgument->toString() << ")";
            return os.str();
        }

    private:
        Node::Pointer mArgument;
    };

    class Successor : public Node
    {
    public:
        using Pointer = Node::PointerType<Successor>;

    public:
        explicit Successor(Node::Pointer argument)
                : Node(NodeType::Primitive), mArgument(std::move(argument))
        { }

        Node::Pointer evaluate(Context &context) const override
        {
            auto t = mArgument;
            if (t->nodeType() != NodeType::NaturalConstant)
                t = t->evaluate(context);

            if (t->nodeType() != NodeType::NaturalConstant)
                throw UnexpectedException("Successor not resolved before evaluation, unexpected right term");

            return Node::make<NaturalConstant>(Node::cast<NaturalConstant>(t)->value() + 1);
        }

        Node::Pointer freeze(Context &context) const override
        {
            return Node::make<Successor>(mArgument->freeze(context));
        }

        Type::Pointer typecheck(TypeContext &context) const override
        {
            auto argumentType = mArgument->typecheck(context);
            if (Type::distinct(argumentType, NatType::NAT))
                throw TypeException("\'" + argumentType->toString() + "\' is not a \'Nat\'");

            return NatType::NAT;
        }

        Node::Pointer copy() const override
        {
            return Node::make<Successor>(mArgument);
        }

        string toString() const override
        {
            auto os = std::ostringstream();
            os << "(succ " << mArgument->toString() << ")";
            return os.str();
        }

    private:
        Node::Pointer mArgument;
    };

    class Predecessor : public Node
    {
    public:
        using Pointer = Node::PointerType<Predecessor>;

    public:
        explicit Predecessor(Node::Pointer argument)
                : Node(NodeType::Primitive), mArgument(std::move(argument))
        { }

        Node::Pointer evaluate(Context &context) const override
        {
            auto t = mArgument;
            if (t->nodeType() != NodeType::NaturalConstant)
                t = t->evaluate(context);

            if (t->nodeType() != NodeType::NaturalConstant)
                throw UnexpectedException("Predecessor not resolved before evaluation, unexpected right term");

            auto value = Node::cast<NaturalConstant>(t)->value();
            return Node::make<NaturalConstant>(value > 0 ? value - 1 : 0);
        }

        Node::Pointer freeze(Context &context) const override
        {
            return Node::make<Predecessor>(mArgument->freeze(context));
        }

        Type::Pointer typecheck(TypeContext &context) const override
        {
            auto argumentType = mArgument->typecheck(context);
            if (Type::distinct(argumentType, NatType::NAT))
                throw TypeException("\'" + argumentType->toString() + "\' is not a \'Nat\'");

            return NatType::NAT;
        }

        Node::Pointer copy() const override
        {
            return Node::make<Predecessor>(mArgument);
        }

        string toString() const override
        {
            auto os = std::ostringstream();
            os << "(pred " << mArgument->toString() << ")";
            return os.str();
        }

    private:
        Node::Pointer mArgument;
    };
}

#endif //LAMBDA_NATURAL_PRIMITIVES_HPP
