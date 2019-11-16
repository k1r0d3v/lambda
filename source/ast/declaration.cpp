#include <ast/node_kind.hpp>
#include <ast/declaration.hpp>
#include <ast/context.hpp>
#include <ast/node_visitor.hpp>
#include <ast/types/unit_type.hpp>

using namespace ast;

Declaration::Declaration(Pattern::Pointer pattern, Node::Pointer value)
    : Node(NodeKind::Declaration), mPattern(std::move(pattern)), mValue(std::move(value)) { }

Node::Pointer Declaration::evaluate(Context &context) const
{
    auto evaluatedValue = mValue->evaluate(context);
    auto matchResult = mPattern->match(evaluatedValue, context);

    for (auto &i : matchResult)
        i.second = context.setValue(i.first, i.second);

    // Returns nothing
    return this->copy();
}

Type::Pointer Declaration::typecheck(TypeContext &context) const
{
    auto valueType = mValue->typecheck(context);
    auto matchResult = mPattern->typecheckMatch(valueType, context);

    // Set new types and save the previous
    for (auto &i : matchResult)
        i.second = context.setTypeFor(i.first, i.second);

    // Returns nothing
    return UnitType::INSTANCE;
}

Node::Pointer Declaration::copy() const
{
    return Node::make<Declaration>(Node::cast<Pattern>(mPattern->copy()), mValue->copy());
}

string Declaration::toString() const
{
    auto os = std::ostringstream();
    os << "let " << mPattern->toString() << " = " << mValue->toString();
    return os.str();
}

Node::Pointer Declaration::transform(NodeVisitor *visitor)
{
    auto self = visitor->visitDeclaration(this);

    if (self != nullptr)
        return self;

    mPattern = Node::cast<Pattern>(Node::transform(mPattern, visitor));
    mValue = Node::transform(mValue, visitor);
    return nullptr;
}
