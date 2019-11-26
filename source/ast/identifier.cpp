#include <ast/node_kind.hpp>
#include <ast/identifier.hpp>
#include <ast/context.hpp>
#include <ast/exception.hpp>
#include <ast/node_visitor.hpp>


using namespace ast;

Identifier::Identifier(string name)
    : Pattern(NodeKind::Identifier), mName(std::move(name)) { }


Pattern::MatchIdenfiers Identifier::matchIdentifiers() const
{
    return { mName };
}

Pattern::MatchResult Identifier::match(const Node::Pointer &value, Context &context) const
{
    return { {mName, value} };
}

Pattern::TypecheckMatchResult Identifier::typecheckMatch(const Type::Pointer &type, TypeContext &context) const
{
    return { {mName, type} };
}

Node::Pointer Identifier::evaluate(Context &context) const
{
    auto valueOfName = context.getValue(mName);

    // Should not happen if there is a typecheck before, this check is only for debug purposes
    if (valueOfName == nullptr)
        throw NameException("Name \'" + mName + "\' is not defined");

    return valueOfName->evaluate(context);
}

Type::Pointer Identifier::typecheck(TypeContext &context)
{
    auto typeOfName = context.getTypeOf(mName);
    if (typeOfName == nullptr)
        throw NameException("Name \'" + mName + "\' is not defined");

    return typeOfName;
}

Node::Pointer Identifier::copy() const
{
    return Node::make<Identifier>(mName);
}

string Identifier::toString() const
{
    return mName;
}

Node::Pointer Identifier::transform(NodeVisitor *visitor)
{
    return visitor->visitIdentifier(this);
}
