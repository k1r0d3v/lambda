#ifndef LAMBDA_NATURAL_PRIMITIVES_HPP
#define LAMBDA_NATURAL_PRIMITIVES_HPP

#include "common.hpp"
#include "boolean.hpp"
#include "natural.hpp"
#include "node_type.hpp"
#include "exception.hpp"

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
            if (t->type() != NodeType::Natural)
                t = t->evaluate(context);

            if (t->type() != NodeType::Natural)
                throw TypeException("\'" + t->toString() + "\' is not a natural");

            return Node::make<Boolean>(Node::cast<Natural>(t)->value() == 0);
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
            if (t->type() != NodeType::Natural)
                t = t->evaluate(context);

            if (t->type() != NodeType::Natural)
                throw TypeException("\'" + t->toString() + "\' is not a natural");

            return Node::make<Natural>(Node::cast<Natural>(t)->value() + 1);
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
            if (t->type() != NodeType::Natural)
                t = t->evaluate(context);

            if (t->type() != NodeType::Natural)
                throw TypeException("\'" + t->toString() + "\' is not a natural");

            auto value = Node::cast<Natural>(t)->value();
            return Node::make<Natural>(value > 0 ? value - 1 : 0);
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
