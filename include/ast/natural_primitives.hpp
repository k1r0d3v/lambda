#ifndef LAMBDA_NATURAL_PRIMITIVES_HPP
#define LAMBDA_NATURAL_PRIMITIVES_HPP

#include "native_application.hpp"

namespace ast
{
    class IsZero : public NativeApplication
    {
    public:
        using Pointer = Node::PointerType<IsZero>;

    private:
        static Node::Pointer iszero(const list<NativeArgument> &args)
        {
            return Node::make<BooleanConstant>(Node::cast<NaturalConstant>(args[0].value)->value() == 0);
        }

    public:
        explicit IsZero(Node::Pointer argument)
            : NativeApplication("iszero",
                                {{std::move(argument), NatType::INSTANCE}},
                                {iszero, BoolType::INSTANCE}) { }

    };

    class Successor : public NativeApplication
    {
    public:
        using Pointer = Node::PointerType<IsZero>;

    private:
        static Node::Pointer succ(const list<NativeArgument> &args)
        {
            return Node::make<NaturalConstant>(Node::cast<NaturalConstant>(args[0].value)->value() + 1);
        }

    public:
        explicit Successor(Node::Pointer argument)
                : NativeApplication("succ",
                                    {{std::move(argument), NatType::INSTANCE}},
                                    {succ, NatType::INSTANCE}) { }

    };

    class Predecessor : public NativeApplication
    {
    public:
        using Pointer = Node::PointerType<IsZero>;

    private:
        static Node::Pointer pred(const list<NativeArgument> &args)
        {
            auto value = Node::cast<NaturalConstant>(args[0].value)->value();
            return Node::make<NaturalConstant>(value > 0 ? value - 1 : 0);
        }

    public:
        explicit Predecessor(Node::Pointer argument)
                : NativeApplication("pred",
                                    {{std::move(argument), NatType::INSTANCE}},
                                    {pred, NatType::INSTANCE}) { }

    };
}

#endif //LAMBDA_NATURAL_PRIMITIVES_HPP
