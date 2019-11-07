#include <ast/node_visitor.hpp>
#include <ast/context.hpp>

using namespace ast;


IdentifierResolver::IdentifierResolver(Context &ctx) : mContext(ctx) { }

Node::Pointer IdentifierResolver::visitIdentifier(Identifier *node)
{
    /*
     * TODO: Differentiate between existent nodes that are not mean to be replaced
     * TODO: and nodes that are not pushed to the context
     */
    return mContext.getValue(node->name());
}

Node::Pointer IdentifierResolver::visitAbstraction(Abstraction *node)
{
    /*
     * We must push the abstraction variable to the context
     * before process the body
     */

    // Resolve identifiers of variables
    auto previous = mContext.setValue(node->variable()->name(), node->variable());

    // Transform body
    auto body = Node::transform(node->body(), this);

    // Restore stack
    mContext.setValue(node->variable()->name(), previous);

    return Node::make<Abstraction>(node->variable(), body);
}

Node::Pointer IdentifierResolver::visitLocalDefinition(LocalDefinition *node)
{
    /*
     * LocalDefinition objects can have identifiers in the patter field
     * we are not interested in these
     */

    // Resolve value
    auto value = Node::transform(node->value(), this);

    // Do not replace pattern identifiers
    Pattern::MatchResult previous;
    for (const auto &i : node->pattern()->matchIdentifiers())
        previous.push_back({i, mContext.setValue(i, nullptr)});

    // Transform body
    auto body = Node::transform(node->body(), this);

    // Restore pattern identifiers previous values if any
    for (const auto &i : previous)
        mContext.setValue(i.first, i.second);

    return Node::make<LocalDefinition>(node->pattern(), value, body);
}


VariableReplacer::VariableReplacer(const Variable::Pointer &arg, Context &ctx)
    : mArg(arg), mContext(ctx) { }


Node::Pointer VariableReplacer::visitVariable(Variable *node)
{
    // If returns nullptr it will be not replaced
    return mContext.getValue(node->name());
}

Node::Pointer VariableReplacer::visitAbstraction(Abstraction *node)
{
    /*
     * Abstractions contains variables in the argument variable,
     * we must skip that and ignore other variables that will not be replaced
     */

    // Do not replace node->argument
    auto previous = mContext.setValue(node->variable()->name(), nullptr);

    // Continue visiting
    auto body = Node::transform(node->body(), this);

    // Restore previous value if any
    mContext.setValue(node->variable()->name(), previous);

    return Node::make<Abstraction>(node->variable(), body);
}