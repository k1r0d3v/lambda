#include <ast/node_kind.hpp>
#include <ast/ascription.hpp>
#include <ast/exception.hpp>
#include <ast/node_visitor.hpp>

using namespace ast;

Ascription::Ascription(Node::Pointer node, Type::Pointer type)
    : Node(NodeKind::Ascription), mNode(std::move(node)), mType(std::move(type)) { }

Node::Pointer Ascription::evaluate(Context &context) const
{
    return mNode->evaluate(context);
}

Type::Pointer Ascription::typecheck(TypeContext &context)
{
    // Resolve aliases
    mType->resolve(context);

    // Typecheck
    auto nodeType = mNode->typecheck(context);
    if (!nodeType->isTypeOf(mType))
        throw TypeException("Cannot convert \'" + nodeType->toString() + "\' to \'" + mType->toString() + "\'");
    return nodeType;
}

Node::Pointer Ascription::copy() const
{
    return Node::make<Ascription>(mNode->copy(), mType);
}

string Ascription::toString() const
{
    return mNode->toString();
}

Node::Pointer Ascription::transform(NodeVisitor *visitor)
{
    return visitor->visitAscription(this);
}
