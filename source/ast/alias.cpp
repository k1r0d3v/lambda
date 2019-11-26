#include <ast/alias.hpp>
#include <ast/node_kind.hpp>
#include <ast/unit.hpp>
#include <ast/types/unit_type.hpp>
#include <ast/node_visitor.hpp>

using namespace ast;

Alias::Alias(string alias, Type::Pointer type) : Node(NodeKind::Alias)
{
    mAlias = std::move(alias);
    mType = std::move(type);
}

Node::Pointer Alias::evaluate(Context &context) const
{
    return Node::make<Unit>();
}

Type::Pointer Alias::typecheck(TypeContext &context)
{
    context.setTypeFor(mAlias, mType);
    return UnitType::INSTANCE;
}

Node::Pointer Alias::transform(NodeVisitor *visitor)
{
    return visitor->visitAlias(this);
}

Node::Pointer Alias::copy() const
{
    return Node::make<Alias>(mAlias, mType);
}

string Alias::toString() const
{
    return string("alias ") + mAlias + " = " + mType->toString();
}
