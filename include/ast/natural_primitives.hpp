#ifndef LAMBDA_NATURAL_PRIMITIVES_HPP
#define LAMBDA_NATURAL_PRIMITIVES_HPP

#include "common.hpp"
#include "boolean_constant.hpp"
#include "natural_constant.hpp"
#include "node_type.hpp"
#include "exception.hpp"
#include "abstraction_type.hpp"

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
                : Node(NodeType::Primitive), mArgument(std::move(argument)) { }

        Node::Pointer evaluate(Context &context) const override
        {
            auto t = mArgument;
            if (t->nodeType() != NodeType::NaturalConstant)
                t = t->evaluate(context);

            if (t->nodeType() != NodeType::NaturalConstant)
                throw TypeException("\'" + t->toString() + "\' is not a natural");

            return Node::make<BooleanConstant>(Node::cast<NaturalConstant>(t)->value() == 0);
        }

        Node::Pointer resolve(Context &context) const override
        {
            auto typedValue = Node::cast<TypedValue>(mArgument->resolve(context));
            auto expectedArgumentType = Type::make<ConstantType>(NaturalConstant::TYPE_NAME);
            auto resultType = Type::make<ConstantType>(BooleanConstant::TYPE_NAME);

            if (typedValue->type()->distinct(expectedArgumentType))
                throw TypeException("FOOOO");

            auto value = Node::make<IsZero>(typedValue->value());
            return Node::make<TypedValue>(value, Type::make<AbstractionType>(expectedArgumentType, resultType));
        }

        Node::Pointer replace(Node::Pointer a, Node::Pointer b) const override
        {
            return Node::make<IsZero>(mArgument->replace(a, b));
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
                : Node(NodeType::Primitive), mArgument(std::move(argument)) { }

        Node::Pointer evaluate(Context &context) const override
        {
            auto t = mArgument;
            if (t->nodeType() != NodeType::NaturalConstant)
                t = t->evaluate(context);

            if (t->nodeType() != NodeType::NaturalConstant)
                throw TypeException("\'" + t->toString() + "\' is not a natural");

            return Node::make<NaturalConstant>(Node::cast<NaturalConstant>(t)->value() + 1);
        }

        Node::Pointer resolve(Context &context) const override
        {
            auto typedValue = Node::cast<TypedValue>(mArgument->resolve(context));
            auto expectedArgumentType = Type::make<ConstantType>(NaturalConstant::TYPE_NAME);
            auto resultType = Type::make<ConstantType>(NaturalConstant::TYPE_NAME);

            if (typedValue->type()->distinct(expectedArgumentType))
                throw TypeException("FOOOO");

            auto value = Node::make<Successor>(typedValue->value());
            return Node::make<TypedValue>(value, Type::make<AbstractionType>(expectedArgumentType, resultType));
        }

        Node::Pointer replace(Node::Pointer a, Node::Pointer b) const override
        {
            return Node::make<Successor>(mArgument->replace(a, b));
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
                : Node(NodeType::Primitive), mArgument(std::move(argument)) { }


        Node::Pointer evaluate(Context &context) const override
        {
            auto t = mArgument;
            if (t->nodeType() != NodeType::NaturalConstant)
                t = t->evaluate(context);

            if (t->nodeType() != NodeType::NaturalConstant)
                throw TypeException("\'" + t->toString() + "\' is not a natural");

            auto value = Node::cast<NaturalConstant>(t)->value();
            return Node::make<NaturalConstant>(value > 0 ? value - 1 : 0);
        }

        Node::Pointer resolve(Context &context) const override
        {
            auto typedValue = Node::cast<TypedValue>(mArgument->resolve(context));
            auto expectedArgumentType = Type::make<ConstantType>(NaturalConstant::TYPE_NAME);
            auto resultType = Type::make<ConstantType>(NaturalConstant::TYPE_NAME);

            if (typedValue->type()->distinct(expectedArgumentType))
                throw TypeException("FOOOO");

            auto value = Node::make<Predecessor>(typedValue->value());
            return Node::make<TypedValue>(value, Type::make<AbstractionType>(expectedArgumentType, resultType));
        }

        Node::Pointer replace(Node::Pointer a, Node::Pointer b) const override
        {
            return Node::make<Predecessor>(mArgument->replace(a, b));
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
