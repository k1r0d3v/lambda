#include <ast/node_kind.hpp>
#include <ast/boolean_constant.hpp>
#include <ast/types/bool_type.hpp>
#include <ast/node_visitor.hpp>

using namespace ast;

BooleanConstant::BooleanConstant(bool value)
        : Node(NodeKind::BooleanConstant), mValue(value)
{ }

Node::Pointer BooleanConstant::evaluate(Context &context) const
{
    return this->copy();
}

Type::Pointer BooleanConstant::typecheck(TypeContext &context) const
{
    return BoolType::INSTANCE;
}

Node::Pointer BooleanConstant::copy() const
{
    return Node::make<BooleanConstant>(mValue);
}

string BooleanConstant::toString() const
{
    return string(mValue ? "true" : "false");
}

Node::Pointer BooleanConstant::transform(NodeVisitor *visitor)
{
    return visitor->visitBoolean(this);
}
