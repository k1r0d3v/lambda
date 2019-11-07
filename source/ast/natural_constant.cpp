#include <ast/node_kind.hpp>
#include <ast/natural_constant.hpp>
#include <ast/types/nat_type.hpp>
#include <ast/node_visitor.hpp>


using namespace ast;

NaturalConstant::NaturalConstant(NaturalValueType value)
        : Node(NodeKind::NaturalConstant), mValue(value) { }


Node::Pointer NaturalConstant::evaluate(Context &context) const
{
    return this->copy();
}

Type::Pointer NaturalConstant::typecheck(TypeContext &context) const
{
    return NatType::INSTANCE;
}

Node::Pointer NaturalConstant::copy() const
{
    return Node::make<NaturalConstant>(mValue);
}

string NaturalConstant::toString() const
{
    return to_string(mValue);
}

Node::Pointer NaturalConstant::transform(NodeVisitor *visitor)
{
    return visitor->visitNatural(this);
}
