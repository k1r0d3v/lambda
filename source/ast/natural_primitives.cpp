#include <ast/natural_primitives.hpp>
#include <ast/types/nat_type.hpp>
#include <ast/types/bool_type.hpp>
#include <ast/boolean_constant.hpp>
#include <ast/natural_constant.hpp>

using namespace ast;


static Node::Pointer iszero(const NativeFunction::ArgumentsType &args)
{
    return Node::make<BooleanConstant>(Node::cast<NaturalConstant>(args[0].value)->value() == 0);
}

static Node::Pointer succ(const NativeFunction::ArgumentsType &args)
{
    return Node::make<NaturalConstant>(Node::cast<NaturalConstant>(args[0].value)->value() + 1);
}

static Node::Pointer pred(const NativeFunction::ArgumentsType &args)
{
    auto value = Node::cast<NaturalConstant>(args[0].value)->value();
    return Node::make<NaturalConstant>(value > 0 ? value - 1 : 0);
}

IsZero::IsZero(Node::Pointer argument)
            : NativeApplication("iszero",
                                {{std::move(argument), NatType::INSTANCE}},
                                {iszero, BoolType::INSTANCE}) { }

Successor::Successor(Node::Pointer argument)
            : NativeApplication("succ",
                                {{std::move(argument), NatType::INSTANCE}},
                                {succ, NatType::INSTANCE}) { }

Predecessor::Predecessor(Node::Pointer argument)
            : NativeApplication("pred",
                                {{std::move(argument), NatType::INSTANCE}},
                                {pred, NatType::INSTANCE}) { }