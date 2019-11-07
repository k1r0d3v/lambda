#include <ast/unit.hpp>
#include <ast/node_kind.hpp>
#include <ast/types/unit_type.hpp>
#include <ast/node_visitor.hpp>


using namespace ast;

Unit::Unit() : Node(NodeKind::Unit) { }

Node::Pointer Unit::evaluate(Context &context) const
{
    return this->copy();
}

Type::Pointer Unit::typecheck(TypeContext &context) const
{
    return UnitType::INSTANCE;
}

Node::Pointer Unit::copy() const
{
    return Node::make<Unit>();
}

string Unit::toString() const
{
    return "()";
}

Node::Pointer Unit::transform(NodeVisitor *visitor)
{
    return visitor->visitUnit(this);
}
