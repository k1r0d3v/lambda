#include <ast/local_definition.hpp>
#include <ast/context.hpp>
#include <ast/node_kind.hpp>
#include <ast/node_visitor.hpp>

using namespace ast;

LocalDefinition::LocalDefinition(Pattern::Pointer pattern, Node::Pointer value, Node::Pointer body)
                : Node(NodeKind::LocalDefinition),
                  mPattern(std::move(pattern)),
                  mValue(std::move(value)),
                  mBody(std::move(body)) { }

Node::Pointer LocalDefinition::evaluate(Context &context) const
{
    auto evaluatedValue = mValue->evaluate(context);
    auto matchResult = mPattern->match(evaluatedValue, context);

    // Set values and save previous
    for (auto &i : matchResult)
        i.second = context.setValue(i.first, i.second);

    auto evaluatedBody = mBody->evaluate(context);

    // Restore values
    for (const auto &i : matchResult)
        context.setValue(i.first, i.second);

    return evaluatedBody;
}

Type::Pointer LocalDefinition::typecheck(TypeContext &context)
{
    // Push argument
    auto valueType = mValue->typecheck(context);
    auto matchResult = mPattern->typecheckMatch(valueType, context);

    // Set new types and save the previous
    for (auto &i : matchResult)
        i.second = context.setTypeFor(i.first, i.second);

    // Typecheck
    // TODO: !!!!!!!!!!!!!!!!!! catch the exceptions to restore the context if this fails
    // TODO: Check the others typecheck for this
    auto bodyType = mBody->typecheck(context);

    // Restore types
    for (const auto &i : matchResult)
        context.setTypeFor(i.first, i.second);

    return bodyType;
}

Node::Pointer LocalDefinition::copy() const
{
    return Node::make<LocalDefinition>(Node::cast<Pattern>(mPattern->copy()), mValue->copy(), mBody->copy());
}

string LocalDefinition::toString() const
{
    auto os = std::ostringstream();
    os << "let " << mPattern->toString() << " = " << mValue->toString() << " in " << mBody->toString();
    return os.str();
}

Node::Pointer LocalDefinition::transform(NodeVisitor *visitor)
{
    auto self = visitor->visitLocalDefinition(this);

    if (self != nullptr)
        return self;

    mPattern = Node::cast<Pattern>(Node::transform(mPattern, visitor));
    mValue = Node::transform(mValue, visitor);
    return nullptr;
}
