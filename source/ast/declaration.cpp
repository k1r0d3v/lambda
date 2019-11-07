#include <ast/node_kind.hpp>
#include <ast/declaration.hpp>
#include <ast/context.hpp>
#include <ast/node_visitor.hpp>

using namespace ast;

Declaration::Declaration(Identifier::Pointer id, Node::Pointer value)
    : Node(NodeKind::Declaration), mId(std::move(id)), mValue(std::move(value)) { }

Node::Pointer Declaration::evaluate(Context &context) const
{
    auto evaluatedValue = mValue->evaluate(context);
    context.setValue(mId->name(), evaluatedValue);
    return evaluatedValue;
}

Type::Pointer Declaration::typecheck(TypeContext &context) const
{
    auto valueType = mValue->typecheck(context);
    context.setTypeFor(mId->name(), valueType);
    return valueType;
}

Node::Pointer Declaration::copy() const
{
    // TODO: Use pattern
    return Node::make<Declaration>(Node::cast<Identifier>(mId->copy()), mValue->copy());
}

string Declaration::toString() const
{
    auto os = std::ostringstream();
    os << "let " << mId->toString() << " = " << mValue->toString() << ";";
    return os.str();
}

Node::Pointer Declaration::transform(NodeVisitor *visitor)
{
    auto self = visitor->visitDeclaration(this);

    if (self != nullptr)
        return self;

    mId = Node::cast<Identifier>(Node::transform(mId, visitor));
    mValue = Node::transform(mValue, visitor);
    return nullptr;
}
